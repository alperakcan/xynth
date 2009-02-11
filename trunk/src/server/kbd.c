/***************************************************************************
    begin                : Mon Sep 22 2003
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
#include "kbd_map.h"

void s_server_kbd_switch_handler (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
        int f = -1;
	s_video_input_t *keybd;
	keybd = (s_video_input_t *) handler->data;

        switch (event->keybd->button) {
		case KEYBOARD_BUTTON_F1:  f = 1;  break;
		case KEYBOARD_BUTTON_F2:  f = 2;  break;
		case KEYBOARD_BUTTON_F3:  f = 3;  break;
		case KEYBOARD_BUTTON_F4:  f = 4;  break;
		case KEYBOARD_BUTTON_F5:  f = 5;  break;
		case KEYBOARD_BUTTON_F6:  f = 6;  break;
		case KEYBOARD_BUTTON_F7:  f = 7;  break;
		case KEYBOARD_BUTTON_F8:  f = 8;  break;
		case KEYBOARD_BUTTON_F9:  f = 9;  break;
		case KEYBOARD_BUTTON_F10: f = 10; break;
	};

	if (f >= 0) {
#if defined(CONFIG_VIDEO_HELPER_CONSOLE)
		int s_video_helper_console_switch (int);
		s_video_helper_console_switch(f);
#endif
		memset(xynth_server->window->event->keybd, 0, sizeof(s_keybd_t));
		xynth_server->window->event->type &= ~EVENT_TYPE_KEYBOARD_MASK;
	}
}

void s_server_kbd_window_close_handler (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_window_close(window);
}

void s_server_kbd_server_quit_handler (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_quit(window);
}

int s_server_kbd_update (s_window_t *window, s_pollfd_t *pfd)
{
	int force_release = 0;
	s_video_input_t *keybd;
	s_video_input_data_t kdata;
	keybd = (s_video_input_t *) pfd->data;
	xynth_server->window->event->type = 0;
	memset(&kdata, 0, sizeof(s_video_input_data_t));
	if (keybd->update != NULL) {
		force_release = keybd->update(&kdata);
		s_server_event_parse_keyboard(&(kdata.keybd));
	}
	s_server_event_changed();
	if (force_release) {
		xynth_server->window->event->type = 0;
		kdata.keybd.state = EVENT_TYPE_KEYBOARD_RELEASED;
		s_server_event_parse_keyboard(&(kdata.keybd));
		s_server_event_changed();
	}
	return 0;
}

void s_server_kbd_init (s_server_conf_t *cfg, s_video_input_t *keybd)
{
	int i;
	int fd = -1;
	s_pollfd_t *pfd;
	s_handler_t *hndl;

        if (keybd->init != NULL) {
		fd = keybd->init(cfg);
	}
	if (fd < 0) {
		debugf(DSER, "keybd->init(cfg) failed");
		debugf(DSER, "keyboard support disabled");
		return;
	}

	s_pollfd_init(&pfd);
	pfd->fd = fd;
        pfd->pf_in = s_server_kbd_update;
        pfd->pf_close = s_server_kbd_uninit;
        pfd->data = keybd;
        s_pollfd_add(xynth_server->window, pfd);

        s_handler_init(&hndl);
        hndl->type = KEYBD_HANDLER;
	hndl->keybd.flag = KEYBOARD_FLAG_LEFTALT;
	hndl->keybd.button = KEYBOARD_BUTTON_F4;
	hndl->keybd.p = s_server_kbd_window_close_handler;
	s_handler_add(xynth_server->window, hndl);

        s_handler_init(&hndl);
        hndl->type = KEYBD_HANDLER;
	hndl->keybd.flag = KEYBOARD_FLAG_LEFTCTRL | KEYBOARD_FLAG_LEFTALT;
	hndl->keybd.button = KEYBOARD_BUTTON_DELETE;
	hndl->keybd.p = s_server_kbd_server_quit_handler;
	s_handler_add(xynth_server->window, hndl);

	for (i = 1; i <= 10; i++) {
	        s_handler_init(&hndl);
        	hndl->type = KEYBD_HANDLER;
		hndl->keybd.flag = KEYBOARD_FLAG_LEFTCTRL | KEYBOARD_FLAG_LEFTALT;
	        switch (i) {
			case 1:  hndl->keybd.button = KEYBOARD_BUTTON_F1;  break;
			case 2:  hndl->keybd.button = KEYBOARD_BUTTON_F2;  break;
			case 3:  hndl->keybd.button = KEYBOARD_BUTTON_F3;  break;
			case 4:  hndl->keybd.button = KEYBOARD_BUTTON_F4;  break;
			case 5:  hndl->keybd.button = KEYBOARD_BUTTON_F5;  break;
			case 6:  hndl->keybd.button = KEYBOARD_BUTTON_F6;  break;
			case 7:  hndl->keybd.button = KEYBOARD_BUTTON_F7;  break;
			case 8:  hndl->keybd.button = KEYBOARD_BUTTON_F8;  break;
			case 9:  hndl->keybd.button = KEYBOARD_BUTTON_F9;  break;
			case 10: hndl->keybd.button = KEYBOARD_BUTTON_F10; break;
		};
		hndl->keybd.p = s_server_kbd_switch_handler;
		hndl->data = keybd;
		s_handler_add(xynth_server->window, hndl);
	}
}

int s_server_kbd_uninit (s_window_t *window, s_pollfd_t *pfd)
{
	s_video_input_t *keybd;
	keybd = (s_video_input_t *) pfd->data;
        if (keybd->uninit != NULL) {
		keybd->uninit();
	}
        s_server_quit(xynth_server->window);
        return 0;
}

s_keyboard_button_t s_server_keyname_to_keycode (char *name)
{
	unsigned int i;
	for (i = 0; i < KEYBOARD_BUTTON_MAX; i++) {
		if (strcmp(name, s_keycodes_map[i].name) == 0) {
			return s_keycodes_map[i].code;
		}
	}
	return 0;
}
