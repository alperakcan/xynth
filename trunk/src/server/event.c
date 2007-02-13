/***************************************************************************
    begin                : Wed Feb 19 2003
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

void s_server_event_parse_keyboard (s_video_input_data_keybd_t *keybd)
{
	long long time;
	S_KEYCODE_FLAG keycode_flag;
	
	time = s_gettimeofday();
	keycode_flag = server->window->event->keybd->flag;
        
	if (keybd->state == KEYBD_PRESSED) {
		switch (keybd->keycode) {
			case S_KEYCODE_NUM_LOCK:
				keycode_flag ^= KEYCODE_NMLCKF;
				break;
			case S_KEYCODE_CAPS_LOCK:
				keycode_flag ^= KEYCODE_CPLCKF;
				break;
			case S_KEYCODE_LEFTCONTROL:
				keycode_flag |= KEYCODE_LCTRLF;
				break;
			case S_KEYCODE_RIGHTCONTROL:
				keycode_flag |= KEYCODE_RCTRLF;
				break;
			case S_KEYCODE_LEFTSHIFT:
				keycode_flag |= KEYCODE_LSHIFTF;
				break;
			case S_KEYCODE_RIGHTSHIFT:
				keycode_flag |= KEYCODE_RSHIFTF;
				break;
			case S_KEYCODE_ALT:
				keycode_flag |= KEYCODE_LALTF;
				break;
			case S_KEYCODE_ALTGR:
				keycode_flag |= KEYCODE_RALTF;
				break;
		}
	} else {
		switch (keybd->keycode) {
			case S_KEYCODE_NUM_LOCK:
			case S_KEYCODE_CAPS_LOCK:
				break;
			case S_KEYCODE_LEFTCONTROL:
				keycode_flag &= ~KEYCODE_LCTRLF;
				break;
			case S_KEYCODE_RIGHTCONTROL:
				keycode_flag &= ~KEYCODE_RCTRLF;
				break;
			case S_KEYCODE_LEFTSHIFT:
				keycode_flag &= ~KEYCODE_LSHIFTF;
				break;
			case S_KEYCODE_RIGHTSHIFT:
				keycode_flag &= ~KEYCODE_RSHIFTF;
				break;
			case S_KEYCODE_ALT:
				keycode_flag &= ~KEYCODE_LALTF;
				break;
			case S_KEYCODE_ALTGR:
				keycode_flag &= ~KEYCODE_RALTF;
				break;
		}
	}

	server->window->event->type |= (KEYBD_EVENT | keybd->state);
	server->window->event->keybd->flag = keycode_flag;
	server->window->event->keybd->scancode = keybd->scancode;
	server->window->event->keybd->button = keybd->button;
	server->window->event->keybd->keycode = keybd->keycode;
	server->window->event->keybd->ascii = keybd->ascii;
	server->window->event->keybd->state[keybd->keycode] = keybd->state;
	server->window->event->keybd->time = time;
}

int s_server_event_parse_mouse (s_video_input_data_mouse_t *mouse)
{
	long long time;

	time = s_gettimeofday();
	server->window->event->type |= MOUSE_EVENT;

	if ((server->window->event->mouse->x != mouse->x) ||
	    (server->window->event->mouse->y != mouse->y)) {
	    	server->window->event->mouse->x_old = server->window->event->mouse->x;
	    	server->window->event->mouse->y_old = server->window->event->mouse->y;
		server->window->event->mouse->x = mouse->x;
		server->window->event->mouse->y = mouse->y;
		s_server_mouse_draw();
	}

	server->window->event->mouse->pbuttons = server->window->event->mouse->buttons;
	server->window->event->mouse->buttons = mouse->buttons;

        if (server->window->event->mouse->buttons > server->window->event->mouse->pbuttons) {
		server->window->event->type |= MOUSE_PRESSED;
		server->window->event->mouse->b = (server->window->event->mouse->buttons & ~(server->window->event->mouse->pbuttons));
		server->window->event->mouse->pb = server->window->event->mouse->b;
		server->window->event->mouse->px = server->window->event->mouse->x;
		server->window->event->mouse->py = server->window->event->mouse->y;
	} else if (server->window->event->mouse->buttons < server->window->event->mouse->pbuttons) {
		server->window->event->mouse->b = (~(server->window->event->mouse->buttons) & server->window->event->mouse->pbuttons);
		if ((server->window->event->mouse->buttons == 0) &&
		    (server->window->event->mouse->b == server->window->event->mouse->pb) &&
		    (server->window->event->mouse->x == server->window->event->mouse->px) &&
		    (server->window->event->mouse->y == server->window->event->mouse->py) &&
		    (time - server->window->event->mouse->time <= (unsigned int) 250)) {
			server->window->event->type |= (MOUSE_CLICKED | MOUSE_RELEASED);
			if (time - server->window->event->mouse->ctime <= (unsigned int) 500) {
				server->window->event->mouse->clicks++;
			} else {
				server->window->event->mouse->clicks = 1;
			}
			server->window->event->mouse->ctime = time;
		} else {
			server->window->event->type |= MOUSE_RELEASED;
			server->window->event->mouse->clicks = 0;
		}
	} else {
		if (server->window->event->mouse->buttons == 0) {
			server->window->event->type |= MOUSE_OVER;
			server->window->event->mouse->b = 0;
		} else {
			server->window->event->type |= (MOUSE_OVER | MOUSE_HINT);
			server->window->event->mouse->b = 0;
		}
	}

	/* this is a pre-patch for wheel buttons */
	server->window->event->mouse->buttons &= ~MOUSE_MASL;

	server->window->event->mouse->time = time;

	return 0;
}

int s_event_changed_ (s_window_t *window)
{
	int r = 1;
	s_event_t *event;
	
	if (server->ph > 0) {
		/* do not parse || send any mouse or keybd event until buttons released
		 */
		if (window->event->mouse->buttons == 0) {
			server->ph = 0;
		}
//		return 0;
	}
	
	if (window->event->type & MOUSE_EVENT) {
		if (!s_event_init(&event)) {
			event->type = window->event->type;
			memcpy(event->mouse, window->event->mouse, sizeof(s_mouse_t));
			s_event_parse_mouse(window, event);
			s_event_uninit(event);
		}
	}
	if (window->event->type & KEYBD_EVENT) {
		if (!s_event_init(&event)) {
			event->type = window->event->type;
			memcpy(event->keybd, window->event->keybd, sizeof(s_keybd_t));
			r = s_event_parse_keybd(window, event);
			s_event_uninit(event);
		}
	}
	return r;
}

void s_server_event_changed (void)
{
	int id;
	int eid;
	int oid;
	int remote;

	id = server->cursor.xyid;
	oid = server->cursor.xyid_old;
	remote = server->window->event->type & REMOTE_EVENT;
	server->window->event->type &= ~REMOTE_EVENT;
	
	if (server->window->event->type & MOUSE_EVENT) {
		server->window->event->type &= MOUSE_MASK;
		if (server->window->event->type & MOUSE_PRESSED) {
			if ((id != s_server_pri_id(0)) && (id >= 0) && (id < S_CLIENTS_MAX) && (!server->mh)) {
				if (server->window->event->mouse->b == server->window->event->mouse->buttons) {
					server->ph = 1;
					s_server_pri_set(SURFACE_FOCUS, id);
//					return;
				}
			}
		}
	} else if (server->window->event->type & KEYBD_EVENT) {
		server->window->event->type &= KEYBD_MASK;
		if (server->window->event->mouse->buttons > 0) {
			return;
		}
	} else {
		return;
	}

	if (s_event_changed_(server->window)) {
		if (server->window->event->type & MOUSE_EVENT) {
			if (server->window->event->type & MOUSE_PRESSED) {
				if ((server->window->event->mouse->b == 4) &&
				    (server->window->event->keybd->flag == KEYCODE_LALTF)) {
					s_server_window_move(server->window);
					return;
				}
			}
		}
		if (server->window->event->type & MOUSE_EVENT) {
			if (id != oid) {
				server->window->event->type |= MOUSE_ENTERED;
				server->window->event->type &= ~MOUSE_EXITED;
			}
			s_server_socket_request(SOC_DATA_EVENT, id);
			server->window->event->type &= ~MOUSE_ENTERED;
			server->window->event->type &= ~MOUSE_EXITED;
			for (eid = 0; eid < S_CLIENTS_MAX; eid++) {
				if (eid != id &&
				    server->client[eid].type & WINDOW_INPUT) {
					s_server_socket_request(SOC_DATA_EVENT, eid);
				}
			}
			if (id != oid) {
				server->window->event->type &= ~MOUSE_ENTERED;
				server->window->event->type |= MOUSE_EXITED;
				s_server_socket_request(SOC_DATA_EVENT, oid);
			}
		} else if (server->window->event->type & KEYBD_EVENT) {
			if (remote) {
				id = s_server_pri_id(1);
			} else {
				id = s_server_pri_id(0);
			}
			if (id >= 0) {
				s_server_socket_request(SOC_DATA_EVENT, id);
			}
			for (eid = 0; eid < S_CLIENTS_MAX; eid++) {
				if (eid != id &&
				    server->client[eid].type & WINDOW_INPUT) {
					s_server_socket_request(SOC_DATA_EVENT, eid);
				}
			}
		}
	}
}
