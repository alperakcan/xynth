/***************************************************************************
    begin                : Wed Jan 29 2003
    copyright            : (C) 2003 - 2007 by Alper Akcan
    email                : distchx@yahoo.com
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
		case 2: s_server_theme_set(THEMEDIR "/keramik.so");   break;
		case 1: s_server_theme_set(THEMEDIR "/microgui.so");  break;
		case 0: s_server_theme_set(THEMEDIR "/silverado.so"); break;
	}
	theme_n += 1;
	theme_n %= 3;
}

static void s_server_handler_quit (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_quit(window);
}

#if defined(PLATFORM_PSPDEV)

#include <pspkernel.h>
#include <pspdebug.h>
#include <stdlib.h>
#include <string.h>

PSP_MODULE_INFO("Xynth Windowing System", 0x1000, 1, 1);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER | PSP_THREAD_ATTR_VFPU);

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

#if defined(PLATFORM_PSPDEV)
	pspDebugScreenInit();
	s_server_psp_setup_callbacks();
#endif

	if (s_server_init()) {
		debugf(DSER, "s_server_init() failed");
//		s_free(server);
		return 1;
	}

	s_server_theme_init();

	s_handler_init(&hndl);
	hndl->type = KEYBD_HANDLER;
	hndl->keybd.flag = 0;
	hndl->keybd.button = S_KEYCODE_F10;
	hndl->keybd.p = s_server_handler_screen_shot;
	s_handler_add(server->window, hndl);

	s_handler_init(&hndl);
	hndl->type = KEYBD_HANDLER;
	hndl->keybd.flag = 0;
	hndl->keybd.button = S_KEYCODE_F11;
	hndl->keybd.p = s_server_handler_change_theme;
	s_handler_add(server->window, hndl);

	s_handler_init(&hndl);
	hndl->type = KEYBD_HANDLER;
	hndl->keybd.flag = 0;
	hndl->keybd.button = S_KEYCODE_F12;
	hndl->keybd.p = s_server_handler_fullscreen;
	s_handler_add(server->window, hndl);

	s_handler_init(&hndl);
	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = 0;
	hndl->mouse.y = 0;
	hndl->mouse.w = 20;
	hndl->mouse.h = 20;
	hndl->mouse.button = MOUSE_LEFTBUTTON;
	hndl->mouse.r = s_server_handler_quit;
	s_handler_add(server->window, hndl);

#if defined(SINGLE_APP)
	s_server_single_start();
#endif /* SINGLE_APP */

	while (server->window->running > 0) {
		if (s_socket_listen_wait(server->window, -1)) {
			server->window->running = 0;
			break;
		}
	}

#if defined(SINGLE_APP)
	s_server_single_stop();
#endif /* SINGLE_APP */

	s_server_uninit();

#if defined(PLATFORM_PSPDEV)
	sceKernelDelayThread(2500000);
	sceKernelExitGame();
#endif /* PLATFORM_PSPDEV */
	
	return 0;
}
