/***************************************************************************
    begin                : Wed Jan 29 2003
    copyright            : (C) 2003 - 2014 by Alper Akcan
    email                : alper.akcan@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is dual licensed free software; you can redistribute     * 
 *   it and/or modify it under the terms of the GNU Lesser General Public  *
 *   License, as published by the Free Software Foundation (version 2.1 or * 
 *   later) OR alternatively under the terms of BSD (3-clause) license.    *
 *                                                                         *
 ***************************************************************************/

#include "../lib/xynth_.h"
#include "server.h"

void s_server_event_parse_keyboard (s_video_input_data_keybd_t *keybd)
{
	long long time;
	s_keyboard_flag_t keycode_flag;

	time = s_gettimeofday();
	keycode_flag = xynth_server->window->event->keybd->flag;

	if (keybd->state == EVENT_TYPE_KEYBOARD_PRESSED) {
		switch (keybd->keycode) {
			case KEYBOARD_BUTTON_NUM_LOCK:
				keycode_flag ^= KEYBOARD_FLAG_NUMLOCK;
				break;
			case KEYBOARD_BUTTON_CAPS_LOCK:
				keycode_flag ^= KEYBOARD_FLAG_CAPSLOCK;
				break;
			case KEYBOARD_BUTTON_LEFTCONTROL:
				keycode_flag |= KEYBOARD_FLAG_LEFTCTRL;
				break;
			case KEYBOARD_BUTTON_RIGHTCONTROL:
				keycode_flag |= KEYBOARD_FLAG_RIGHTCTRL;
				break;
			case KEYBOARD_BUTTON_LEFTSHIFT:
				keycode_flag |= KEYBOARD_FLAG_LEFTSHIFT;
				break;
			case KEYBOARD_BUTTON_RIGHTSHIFT:
				keycode_flag |= KEYBOARD_FLAG_RIGHTSHIFT;
				break;
			case KEYBOARD_BUTTON_ALT:
				keycode_flag |= KEYBOARD_FLAG_LEFTALT;
				break;
			case KEYBOARD_BUTTON_ALTGR:
				keycode_flag |= KEYBOARD_FLAG_RIGHTALT;
				break;
		}
	} else {
		switch (keybd->keycode) {
			case KEYBOARD_BUTTON_NUM_LOCK:
			case KEYBOARD_BUTTON_CAPS_LOCK:
				break;
			case KEYBOARD_BUTTON_LEFTCONTROL:
				keycode_flag &= ~KEYBOARD_FLAG_LEFTCTRL;
				break;
			case KEYBOARD_BUTTON_RIGHTCONTROL:
				keycode_flag &= ~KEYBOARD_FLAG_RIGHTCTRL;
				break;
			case KEYBOARD_BUTTON_LEFTSHIFT:
				keycode_flag &= ~KEYBOARD_FLAG_LEFTSHIFT;
				break;
			case KEYBOARD_BUTTON_RIGHTSHIFT:
				keycode_flag &= ~KEYBOARD_FLAG_RIGHTSHIFT;
				break;
			case KEYBOARD_BUTTON_ALT:
				keycode_flag &= ~KEYBOARD_FLAG_LEFTALT;
				break;
			case KEYBOARD_BUTTON_ALTGR:
				keycode_flag &= ~KEYBOARD_FLAG_RIGHTALT;
				break;
		}
	}

	xynth_server->window->event->type |= (EVENT_TYPE_KEYBOARD | keybd->state);
	xynth_server->window->event->keybd->flag = keycode_flag;
	xynth_server->window->event->keybd->scancode = keybd->scancode;
	xynth_server->window->event->keybd->button = keybd->button;
	xynth_server->window->event->keybd->keycode = keybd->keycode;
	xynth_server->window->event->keybd->ascii = keybd->ascii;
	xynth_server->window->event->keybd->state[keybd->keycode] = keybd->state;
	xynth_server->window->event->keybd->time = time;
}

int s_server_event_parse_mouse (s_video_input_data_mouse_t *mouse)
{
	long long time;

	time = s_gettimeofday();
	xynth_server->window->event->type |= EVENT_TYPE_MOUSE;

	if ((xynth_server->window->event->mouse->x != mouse->x) ||
	    (xynth_server->window->event->mouse->y != mouse->y)) {
	    	xynth_server->window->event->mouse->x_old = xynth_server->window->event->mouse->x;
	    	xynth_server->window->event->mouse->y_old = xynth_server->window->event->mouse->y;
		xynth_server->window->event->mouse->x = mouse->x;
		xynth_server->window->event->mouse->y = mouse->y;
		s_server_mouse_draw();
	}

	xynth_server->window->event->mouse->pbuttons = xynth_server->window->event->mouse->buttons;
	xynth_server->window->event->mouse->buttons = mouse->buttons;

        if (xynth_server->window->event->mouse->buttons > xynth_server->window->event->mouse->pbuttons) {
		xynth_server->window->event->type |= EVENT_TYPE_MOUSE_PRESSED;
		xynth_server->window->event->mouse->b = (xynth_server->window->event->mouse->buttons & ~(xynth_server->window->event->mouse->pbuttons));
		xynth_server->window->event->mouse->pb = xynth_server->window->event->mouse->b;
		xynth_server->window->event->mouse->px = xynth_server->window->event->mouse->x;
		xynth_server->window->event->mouse->py = xynth_server->window->event->mouse->y;
	} else if (xynth_server->window->event->mouse->buttons < xynth_server->window->event->mouse->pbuttons) {
		xynth_server->window->event->mouse->b = (~(xynth_server->window->event->mouse->buttons) & xynth_server->window->event->mouse->pbuttons);
		if ((xynth_server->window->event->mouse->buttons == 0) &&
		    (xynth_server->window->event->mouse->b == xynth_server->window->event->mouse->pb) &&
		    (xynth_server->window->event->mouse->x == xynth_server->window->event->mouse->px) &&
		    (xynth_server->window->event->mouse->y == xynth_server->window->event->mouse->py) &&
		    (time - xynth_server->window->event->mouse->time <= (unsigned int) 250)) {
			xynth_server->window->event->type |= (EVENT_TYPE_MOUSE_CLICKED | EVENT_TYPE_MOUSE_RELEASED);
			if (time - xynth_server->window->event->mouse->ctime <= (unsigned int) 500) {
				xynth_server->window->event->mouse->clicks++;
			} else {
				xynth_server->window->event->mouse->clicks = 1;
			}
			xynth_server->window->event->mouse->ctime = time;
		} else {
			xynth_server->window->event->type |= EVENT_TYPE_MOUSE_RELEASED;
			xynth_server->window->event->mouse->clicks = 0;
		}
	} else {
		if (xynth_server->window->event->mouse->buttons == 0) {
			xynth_server->window->event->type |= EVENT_TYPE_MOUSE_OVER;
			xynth_server->window->event->mouse->b = 0;
		} else {
			xynth_server->window->event->type |= (EVENT_TYPE_MOUSE_OVER | EVENT_TYPE_MOUSE_HINT1);
			xynth_server->window->event->mouse->b = 0;
		}
	}

	/* this is a pre-patch for wheel buttons */
	xynth_server->window->event->mouse->buttons &= ~MOUSE_MASL;

	xynth_server->window->event->mouse->time = time;

	return 0;
}

int s_event_changed_ (s_window_t *window)
{
	int r = 1;
	s_event_t *event;

	if (xynth_server->ph > 0) {
		/* do not parse || send any mouse or keybd event until buttons released
		 */
		if (window->event->mouse->buttons == 0) {
			xynth_server->ph = 0;
		}
//		return 0;
	}

	if (window->event->type & EVENT_TYPE_MOUSE) {
		if (!s_event_init(&event)) {
			event->type = window->event->type;
			memcpy(event->mouse, window->event->mouse, sizeof(s_mouse_t));
			s_event_parse_mouse(window, event);
			s_event_uninit(event);
		}
	}
	if (window->event->type & EVENT_TYPE_KEYBOARD) {
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

	id = xynth_server->cursor.xyid;
	oid = xynth_server->cursor.xyid_old;
	remote = xynth_server->window->event->type & EVENT_TYPE_REMOTE;
	xynth_server->window->event->type &= ~EVENT_TYPE_REMOTE;

	if (xynth_server->window->event->type & EVENT_TYPE_MOUSE) {
		xynth_server->window->event->type &= EVENT_TYPE_MOUSE_MASK;
		if (xynth_server->window->event->type & EVENT_TYPE_MOUSE_PRESSED) {
			if ((id != s_server_pri_id(0)) && (id >= 0) && (id < S_CLIENTS_MAX) && (!xynth_server->mh)) {
				if (xynth_server->window->event->mouse->b == xynth_server->window->event->mouse->buttons) {
					xynth_server->ph = 1;
					s_server_pri_set(SURFACE_FOCUS, id);
//					return;
				}
			}
		}
	} else if (xynth_server->window->event->type & EVENT_TYPE_KEYBOARD) {
		xynth_server->window->event->type &= EVENT_TYPE_KEYBOARD_MASK;
		if (xynth_server->window->event->mouse->buttons > 0) {
			return;
		}
	} else {
		return;
	}

	if (s_event_changed_(xynth_server->window)) {
		if (xynth_server->window->event->type & EVENT_TYPE_MOUSE) {
			if (xynth_server->window->event->type & EVENT_TYPE_MOUSE_PRESSED) {
				if ((xynth_server->window->event->mouse->b == 4) &&
				    (xynth_server->window->event->keybd->flag == KEYBOARD_FLAG_LEFTALT)) {
					s_server_window_move(xynth_server->window);
					return;
				}
			}
		}
		if (xynth_server->window->event->type & EVENT_TYPE_MOUSE) {
			if (id != oid) {
				xynth_server->window->event->type |= EVENT_TYPE_MOUSE_ENTER;
				xynth_server->window->event->type &= ~EVENT_TYPE_MOUSE_EXIT;
			}
			s_server_socket_request(SOC_DATA_EVENT, id);
			xynth_server->window->event->type &= ~EVENT_TYPE_MOUSE_ENTER;
			xynth_server->window->event->type &= ~EVENT_TYPE_MOUSE_EXIT;
			for (eid = 0; eid < S_CLIENTS_MAX; eid++) {
				if (eid != id &&
				    xynth_server->client[eid].type & WINDOW_TYPE_INPUT) {
					s_server_socket_request(SOC_DATA_EVENT, eid);
				}
			}
			/* artec group
			 * send to the window where the mouse was clicked
			 */
			if (xynth_server->window->event->type & EVENT_TYPE_MOUSE_RELEASED) {
				if (id != xynth_server->cursor.xyid_oldclick &&
				    oid != xynth_server->cursor.xyid_oldclick) {
					s_server_socket_request(SOC_DATA_EVENT, xynth_server->cursor.xyid_oldclick);
				}
				xynth_server->cursor.xyid_oldclick = -1;
			}
			if (xynth_server->window->event->type & EVENT_TYPE_MOUSE_PRESSED) {
				xynth_server->cursor.xyid_oldclick = id;
			}
			/* moved to other window */
			if (id != oid) {
				xynth_server->window->event->type &= ~EVENT_TYPE_MOUSE_ENTER;
				xynth_server->window->event->type |= EVENT_TYPE_MOUSE_EXIT;
				s_server_socket_request(SOC_DATA_EVENT, oid);
			}
		} else if (xynth_server->window->event->type & EVENT_TYPE_KEYBOARD) {
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
				    xynth_server->client[eid].type & WINDOW_TYPE_INPUT) {
					s_server_socket_request(SOC_DATA_EVENT, eid);
				}
			}
		}
	}
}
