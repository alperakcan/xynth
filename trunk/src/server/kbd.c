/***************************************************************************
    begin                : Mon Sep 22 2003
    copyright            : (C) 2003 - 2006 by Alper Akcan
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

void s_server_kbd_switch_handler (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
        int f = -1;

        switch (event->keybd->button) {
		case S_KEYCODE_F1:  f = 1;  break;
		case S_KEYCODE_F2:  f = 2;  break;
		case S_KEYCODE_F3:  f = 3;  break;
		case S_KEYCODE_F4:  f = 4;  break;
		case S_KEYCODE_F5:  f = 5;  break;
		case S_KEYCODE_F6:  f = 6;  break;
		case S_KEYCODE_F7:  f = 7;  break;
		case S_KEYCODE_F8:  f = 8;  break;
		case S_KEYCODE_F9:  f = 9;  break;
		case S_KEYCODE_F10: f = 10; break;
	};

	if (f >= 0) {
		s_server_kdb_switch(f);
		memset(server->window->event->keybd, 0, sizeof(s_keybd_t));
		server->window->event->type &= ~KEYBD_MASK;
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

int s_server_kbd_in_f (s_window_t *window, int s)
{
	s_server_event_parse(KEYBD_EVENT);
	return 0;
}

void s_server_kbd_init (s_server_conf_t *cfg)
{
	int i;
	int fd = -1;
	s_pollfd_t *pfd;
	s_handler_t *hndl;

        if (server->driver->kbd_init != NULL) {
		fd = server->driver->kbd_init(cfg);
	}
	if (fd < 0) {
		debugf(DSER, "server->driver->kbd_init(cfg) failed");
		debugf(DSER, "keyboard support disabled");
		return;
	}

	s_pollfd_init(&pfd);
	pfd->fd = fd;
        pfd->pf_in = s_server_kbd_in_f;
        pfd->pf_close = s_server_kbd_uninit;
        s_pollfd_add(server->window, pfd);

        s_handler_init(&hndl);
        hndl->type = KEYBD_HANDLER;
	hndl->keybd.flag = KEYCODE_LALTF;
	hndl->keybd.button = S_KEYCODE_F4;
	hndl->keybd.p = s_server_kbd_window_close_handler;
	s_handler_add(server->window, hndl);

        s_handler_init(&hndl);
        hndl->type = KEYBD_HANDLER;
	hndl->keybd.flag = KEYCODE_LCTRLF | KEYCODE_LALTF;
	hndl->keybd.button = S_KEYCODE_DELETE;
	hndl->keybd.p = s_server_kbd_server_quit_handler;
	s_handler_add(server->window, hndl);

	for (i = 1; i <= 10; i++) {
	        s_handler_init(&hndl);
        	hndl->type = KEYBD_HANDLER;
		hndl->keybd.flag = KEYCODE_LCTRLF | KEYCODE_LALTF;
	        switch (i) {
			case 1:  hndl->keybd.button = S_KEYCODE_F1;  break;
			case 2:  hndl->keybd.button = S_KEYCODE_F2;  break;
			case 3:  hndl->keybd.button = S_KEYCODE_F3;  break;
			case 4:  hndl->keybd.button = S_KEYCODE_F4;  break;
			case 5:  hndl->keybd.button = S_KEYCODE_F5;  break;
			case 6:  hndl->keybd.button = S_KEYCODE_F6;  break;
			case 7:  hndl->keybd.button = S_KEYCODE_F7;  break;
			case 8:  hndl->keybd.button = S_KEYCODE_F8;  break;
			case 9:  hndl->keybd.button = S_KEYCODE_F9;  break;
			case 10: hndl->keybd.button = S_KEYCODE_F10; break;
		};
		hndl->keybd.p = s_server_kbd_switch_handler;
		s_handler_add(server->window, hndl);
	}
}

int s_server_kbd_uninit (s_window_t *window, int fd)
{
        if (server->driver->kbd_uninit != NULL) {
		server->driver->kbd_uninit();
	}
        s_server_quit(server->window);
        return 0;
}

void s_server_kbd_update (s_keybd_driver_t *keybd)
{
        if (server->driver->kbd_update != NULL) {
		server->driver->kbd_update(keybd);
	}
}

void s_server_kdb_switch (int vt)
{
        if (server->driver->kbd_switch != NULL) {
		server->driver->kbd_switch(vt);
	}
}