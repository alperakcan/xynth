/***************************************************************************
    begin                : Wed Jan 29 2003
    copyright            : (C) 2003 - 2009 by Alper Akcan
    email                : alper.akcan@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#include "../lib/xynth_.h"
#include "server.h"

static void s_server_handler_fullscreen (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_fullscreen();
}

static void s_server_handler_screen_shot (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	int r;
	int g;
	int b;
	int x;
	int y;
	FILE *file;
	char name[100];
	static int file_n = 0;

	sprintf(name, "xynth_%d.pnm", file_n++);
	file = fopen(name, "w+");
	fprintf(file, "P3\n");
	fprintf(file, "%d %d\n", window->surface->width, window->surface->height);
	fprintf(file, "255\n");

	for (y = 0; y < window->surface->height; y++) {
		for (x = 0; x < window->surface->width; x++) {
			s_colorrgb(window->surface, s_getpixel(window->surface, x, y), &r, &g, &b);
			fprintf(file, "%d\n%d\n%d\n", r, g, b);
		}
	}

	fclose(file);
}

static void s_server_handler_change_theme (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	static int theme_n = 0;
	
	switch (theme_n) {
		case 2: s_server_theme_set("keramik");   break;
		case 1: s_server_theme_set("microgui");  break;
		case 0: s_server_theme_set("silverado"); break;
	}
	theme_n += 1;
	theme_n %= 3;
}

static void s_server_handler_quit (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_quit(window);
}

#if defined(CONFIG_PLATFORM_PSPDEV)

#include <pspkernel.h>
#include <pspdisplay.h>
#include <string.h>
#include <psputility.h>
#include <pspgu.h>
#include <pspgum.h>
#include <pspsdk.h>
#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>

PSP_MODULE_INFO("Xynth Windowing System", 0x0, 1, 1);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER | PSP_THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(-1);

int s_server_psp_exit_callback (int arg1, int arg2, void *common)
{
	sceKernelExitGame();
	return 0;
}

int s_server_psp_callback_thread (SceSize args, void *argp)
{
	int cbid;
	cbid = sceKernelCreateCallback("Exit Callback", s_server_psp_exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);
	sceKernelSleepThreadCB();
	return 0;
}

int s_server_psp_setup_callbacks (void)
{
	int thid = 0;
	thid = sceKernelCreateThread("update_thread", s_server_psp_callback_thread, 0x11, 0xFA0, PSP_THREAD_ATTR_USER, 0);
	if (thid >= 0) {
		sceKernelStartThread(thid, 0, 0);
	}
	return thid;
}

#endif

int main (int argc, char *argv[])
{
	s_handler_t *hndl;

#if defined(CONFIG_PLATFORM_PSPDEV)
	pspDebugScreenInit();
	s_server_psp_setup_callbacks();
#endif

	if (s_server_init()) {
		debugf(DSER, "s_server_init() failed");
//		s_free(server);
		return 1;
	}

	s_handler_init(&hndl);
	hndl->type = KEYBD_HANDLER;
	hndl->keybd.flag = 0;
	hndl->keybd.button = KEYBOARD_BUTTON_F10;
	hndl->keybd.p = s_server_handler_screen_shot;
	s_handler_add(xynth_server->window, hndl);

	s_handler_init(&hndl);
	hndl->type = KEYBD_HANDLER;
	hndl->keybd.flag = 0;
	hndl->keybd.button = KEYBOARD_BUTTON_F11;
	hndl->keybd.p = s_server_handler_change_theme;
	s_handler_add(xynth_server->window, hndl);

	s_handler_init(&hndl);
	hndl->type = KEYBD_HANDLER;
	hndl->keybd.flag = 0;
	hndl->keybd.button = KEYBOARD_BUTTON_F12;
	hndl->keybd.p = s_server_handler_fullscreen;
	s_handler_add(xynth_server->window, hndl);

	s_handler_init(&hndl);
	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = 0;
	hndl->mouse.y = 0;
	hndl->mouse.w = 20;
	hndl->mouse.h = 20;
	hndl->mouse.button = MOUSE_BUTTON_LEFT;
	hndl->mouse.r = s_server_handler_quit;
	s_handler_add(xynth_server->window, hndl);

#if defined(CONFIG_SINGLE_APPLICATION)
	s_server_single_start();
#endif /* CONFIG_SINGLE_APPLICATION */
	
	s_server_loop(NULL);

#if defined(CONFIG_SINGLE_APPLICATION)
	s_server_single_stop();
#endif /* CONFIG_SINGLE_APPLICATION */

	s_server_uninit();

#if defined(CONFIG_PLATFORM_PSPDEV)
	sceKernelDelayThread(2500000);
	sceKernelExitGame();
#endif /* CONFIG_PLATFORM_PSPDEV */
	
	return 0;
}
