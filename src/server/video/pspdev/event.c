/***************************************************************************
    begin                : Thu Sep 22 2005
    copyright            : (C) 2005 - 2007 by Alper Akcan
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

#if defined(VIDEO_PSPDEV)

#include "../../../lib/xynth_.h"
#include "server.h"
#include "pspdev.h"

#define MOUSE_ACCELL	3

static const enum PspCtrlButtons s_video_pspdev_btns[] = {
	PSP_CTRL_TRIANGLE,
	PSP_CTRL_CIRCLE,
	PSP_CTRL_CROSS,
	PSP_CTRL_SQUARE,
	PSP_CTRL_LTRIGGER,
	PSP_CTRL_RTRIGGER,
	PSP_CTRL_DOWN,
	PSP_CTRL_LEFT,
	PSP_CTRL_UP,
	PSP_CTRL_RIGHT,
	PSP_CTRL_SELECT,
	PSP_CTRL_START,
	PSP_CTRL_HOME,
	PSP_CTRL_HOLD
};

void * s_video_pspdev_event_parse (void *arg)
{
	int axisx;
	int axisy;
	SceCtrlData pad;
	s_video_input_data_mouse_t pad_send;
	enum PspCtrlButtons pad_old;
	enum PspCtrlButtons changed;
	s_video_pspdev_data_t *priv = (s_video_pspdev_data_t *) server->driver->driver_data;

	while (server->window->running != 1) {
		usleep(20000);
	}
	
	pad_old = 0;
	while (server->window->running) {
		sceCtrlReadBufferPositive(&pad, 1);

		changed = pad_old ^ pad.Buttons;
		pad_old = pad.Buttons;

		axisx = s_mouse_getx();
		axisy = s_mouse_gety();
		if (pad.Buttons & PSP_CTRL_LEFT) {
			axisx -= MOUSE_ACCELL;
		} else if (pad.Buttons & PSP_CTRL_RIGHT) {
			axisx += MOUSE_ACCELL;
		}
		if (pad.Buttons & PSP_CTRL_UP) {
			axisy -= MOUSE_ACCELL;
		} else if (pad.Buttons & PSP_CTRL_DOWN) {
			axisy += MOUSE_ACCELL;
		}

		if (pad.Lx < 30) {         axisx -= MOUSE_ACCELL + 2;
		} else if (pad.Lx < 60) {  axisx -= MOUSE_ACCELL + 1;
		} else if (pad.Lx < 90) {  axisx -= MOUSE_ACCELL;
		} else if (pad.Lx > 225) { axisx += MOUSE_ACCELL + 2;
		} else if (pad.Lx > 195) { axisx += MOUSE_ACCELL + 1;
		} else if (pad.Lx > 165) { axisx += MOUSE_ACCELL;
		}
		if (pad.Ly < 30) {         axisy -= MOUSE_ACCELL + 2;
		} else if (pad.Ly < 60) {  axisy -= MOUSE_ACCELL + 1;
		} else if (pad.Ly < 90) {  axisy -= MOUSE_ACCELL;
		} else if (pad.Ly > 225) { axisy += MOUSE_ACCELL + 2;
		} else if (pad.Ly > 195) { axisy += MOUSE_ACCELL + 1;
		} else if (pad.Ly > 165) { axisy += MOUSE_ACCELL;
		}
		
		axisx = MAX(axisx, server->mouse_rangex[0]);
		axisx = MIN(axisx, server->mouse_rangex[1]);
		axisy = MAX(axisy, server->mouse_rangey[0]);
		axisy = MIN(axisy, server->mouse_rangey[1]);

		if ((changed & (PSP_CTRL_CROSS | PSP_CTRL_CIRCLE)) ||
		    (axisx - s_mouse_getx()) ||
		    (axisy - s_mouse_gety())) {
			pad_send.x = axisx;
			pad_send.y = axisy;
			pad_send.buttons = pad.Buttons & (PSP_CTRL_CROSS | PSP_CTRL_CIRCLE);

			if (priv->mouse_fd[1] != -1) {
				s_pipe_api_write(priv->mouse_fd[1], &pad_send, sizeof(s_video_input_data_mouse_t));
			}
		}
		usleep(30000);
	}
	return NULL;
}

#endif /* VIDEO_PSPDEV */
