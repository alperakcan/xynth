/***************************************************************************
    begin                : Thu Sep 22 2005
    copyright            : (C) 2005 - 2006 by Alper Akcan
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

#include "../../../lib/xynth_.h"
#include "../../server.h"
#include "pspdev.h"

#define MOUSE_ACCELL	3

static const enum PspCtrlButtons s_video_pspdev_btns[] = {
	PSP_CTRL_TRIANGLE, PSP_CTRL_CIRCLE, PSP_CTRL_CROSS, PSP_CTRL_SQUARE,
	PSP_CTRL_LTRIGGER, PSP_CTRL_RTRIGGER,
	PSP_CTRL_DOWN, PSP_CTRL_LEFT, PSP_CTRL_UP, PSP_CTRL_RIGHT,
	PSP_CTRL_SELECT, PSP_CTRL_START, PSP_CTRL_HOME, PSP_CTRL_HOLD
};

void * s_video_pspdev_event_parse (void *arg)
{
	int axisx;
	int axisy;
	SceCtrlData pad;
	s_mouse_driver_t pad_send;
	enum PspCtrlButtons changed;
	s_video_pspdev_data_t *priv = (s_video_pspdev_data_t *) server->driver->driver_data;

	while (server->window->running != 1) {
		usleep(20000);
	}

	while (server->window->running) {
		sceCtrlReadBufferPositive(&pad, 1);

		changed = priv->pad_old.buttons ^ pad.Buttons;
		priv->pad_old.buttons = pad.Buttons;

		axisx = priv->pad_old.x;
		axisy = priv->pad_old.y;
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
		axisx = MAX(axisx, priv->rx[0]);
		axisx = MIN(axisx, priv->rx[1]);
		axisy = MAX(axisy, priv->ry[0]);
		axisy = MIN(axisy, priv->ry[1]);

		if ((changed & (PSP_CTRL_CROSS | PSP_CTRL_CIRCLE)) ||
		    (axisx - priv->pad_old.x) ||
		    (axisy - priv->pad_old.y)) {
			priv->pad_old.x = axisx;
			priv->pad_old.y = axisy;

			pad_send.x = axisx;
			pad_send.y = axisy;
			pad_send.buttons = pad.Buttons & (PSP_CTRL_CROSS | PSP_CTRL_CIRCLE);

			if (priv->mouse_fd[1] != -1) {
				s_pipe_api_write(priv->mouse_fd[1], &pad_send, sizeof(s_mouse_driver_t));
			}
		}
		usleep(30000);
	}
	return NULL;
}