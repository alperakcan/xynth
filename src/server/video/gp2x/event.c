/***************************************************************************
    begin                : Thu Feb 2 2006
    copyright            : (C) 2006 - 2007 by Alper Akcan
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

#if defined(VIDEO_GP2X)

#include "../../../lib/xynth_.h"
#include "server.h"
#include "gp2x.h"

#define MOUSE_ACCELL	3

void * s_video_gp2x_event_parse (void *arg)
{
	int axisx;
	int axisy;
	unsigned long pad;
	unsigned long pad_old;
	unsigned long changed;
	s_mouse_driver_t pad_send;
	s_video_input_data_keybd_t key_send;
	s_video_gp2x_data_t *priv = (s_video_gp2x_data_t *) server->driver->driver_data;

	while (server->window->running != 1) {
		usleep(20000);
	}

	pad_old = 0;
	while (server->window->running) {
		pad = priv->gp2x_io[0x1198 >> 1] & 0xff;
		if (pad == 0xfd) pad = 0xfa;
		if (pad == 0xf7) pad = 0xeb;
		if (pad == 0xdf) pad = 0xaf;
		if (pad == 0x7f) pad = 0xbe;
		pad = ~((priv->gp2x_io[0x1184 >> 1] & 0xff00) | pad | (priv->gp2x_io[0x1186 >> 1] << 16));

		changed = pad_old ^ pad;
		pad_old = pad;

		axisx = s_mouse_getx();
		axisy = s_mouse_gety();
		if (pad & GP2X_LEFT) {
			axisx -= MOUSE_ACCELL;
		} else if (pad & GP2X_RIGHT) {
			axisx += MOUSE_ACCELL;
		}
		if (pad & GP2X_UP) {
			axisy -= MOUSE_ACCELL;
		} else if (pad & GP2X_DOWN) {
			axisy += MOUSE_ACCELL;
		}
		axisx = MAX(axisx, server->mouse_rangex[0]);
		axisx = MIN(axisx, server->mouse_rangex[1]);
		axisy = MAX(axisy, server->mouse_rangey[0]);
		axisy = MIN(axisy, server->mouse_rangey[1]);

		if ((changed & (GP2X_L |
		                GP2X_R)) ||
		    (axisx - s_mouse_getx()) ||
		    (axisy - s_mouse_gety())) {
			pad_send.x = axisx;
			pad_send.y = axisy;
			pad_send.buttons = pad & (GP2X_L |
			                          GP2X_R);
			if (priv->mouse_fd[1] != -1) {
				s_pipe_api_write(priv->mouse_fd[1], &pad_send, sizeof(s_mouse_driver_t));
			}
		}
		if (changed & (GP2X_UP |
		               GP2X_LEFT |
		               GP2X_DOWN |
		               GP2X_RIGHT |
		               GP2X_START |
		               GP2X_SELECT |
		               GP2X_A |
		               GP2X_B |
		               GP2X_X |
		               GP2X_Y |
		               GP2X_VOL_UP |
		               GP2X_VOL_DOWN |
		               GP2X_PUSH)) {
			switch (changed) {
				case GP2X_UP:       key_send.button = S_KEYCODE_USER0; break;
		                case GP2X_LEFT:     key_send.button = S_KEYCODE_USER1; break;
		                case GP2X_DOWN:     key_send.button = S_KEYCODE_USER2; break;
		                case GP2X_RIGHT:    key_send.button = S_KEYCODE_USER3; break;
		                case GP2X_START:    key_send.button = S_KEYCODE_USER4; break;
		                case GP2X_SELECT:   key_send.button = S_KEYCODE_USER5; break;
		                case GP2X_A:        key_send.button = S_KEYCODE_USER8; break;
		                case GP2X_B:        key_send.button = S_KEYCODE_USER9; break;
		                case GP2X_X:        key_send.button = S_KEYCODE_USER10; break;
		                case GP2X_Y:        key_send.button = S_KEYCODE_USER11; break;
		                case GP2X_VOL_UP:   key_send.button = S_KEYCODE_USER12; break;
		                case GP2X_VOL_DOWN: key_send.button = S_KEYCODE_USER13; break;
		                case GP2X_PUSH:     key_send.button = S_KEYCODE_USER14; break;
			}
			key_send.state = (changed & pad) ? KEYBD_PRESSED : KEYBD_RELEASED;
			if (priv->keybd_fd[1] != -1) {
				s_pipe_api_write(priv->keybd_fd[1], &key_send, sizeof(s_video_input_data_keybd_t));
			}
		}
		usleep(30000);
	}
	return NULL;
}

#endif /* VIDEO_GP2X */
