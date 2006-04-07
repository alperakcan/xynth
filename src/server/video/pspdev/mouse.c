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
#include "../helper/helper.h"
#include "pspdev.h"

int s_video_pspdev_mouse_init (s_server_conf_t *cfg)
{
	s_video_pspdev_data_t *priv = server->driver->driver_data;
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
	if (s_pipe_api_pipe(priv->mouse_fd)) {
		debugf(DSER | DSYS, "pipe failed");
		goto err0;
	}
	s_video_helper_mouse_cursor_init();
	s_mouse_setxrange(server->window, 0, server->window->surface->width);
	s_mouse_setyrange(server->window, 0, server->window->surface->height);
	server->window->event->mouse->x = server->window->surface->width / 2;
	server->window->event->mouse->y = server->window->surface->height / 2;
        priv->pad_old.x = server->window->event->mouse->x;
        priv->pad_old.y = server->window->event->mouse->y;
	return priv->mouse_fd[0];
err0:	priv->mouse_fd[0] = -1;
	priv->mouse_fd[1] = -1;
	return -1;
}

int s_video_pspdev_mouse_update (s_mouse_driver_t *mouse)
{
	s_mouse_driver_t pad_recv;
	s_video_pspdev_data_t *priv = server->driver->driver_data;
	s_pipe_api_read(priv->mouse_fd[0], &pad_recv, sizeof(s_mouse_driver_t));
	mouse->x = pad_recv.x;
	mouse->y = pad_recv.y;
	mouse->buttons = 0;
	mouse->buttons |= (pad_recv.buttons & PSP_CTRL_CROSS) ? MOUSE_LEFTBUTTON : 0;
	mouse->buttons |= (pad_recv.buttons & PSP_CTRL_CIRCLE) ? MOUSE_RIGHTBUTTON : 0;
	return 0;
}

void s_video_pspdev_mouse_uninit (void)
{
	s_video_pspdev_data_t *priv = server->driver->driver_data;
	s_pipe_api_close(priv->mouse_fd[0]);
	s_pipe_api_close(priv->mouse_fd[1]);
	priv->mouse_fd[0] = -1;
	priv->mouse_fd[1] = -1;
	s_server_cursor_uninit();
}

void s_video_pspdev_mouse_setxrange (int x1, int x2)
{
	s_video_pspdev_data_t *priv = server->driver->driver_data;
	priv->rx[0] = x1;
	priv->rx[1] = x2;
}

void s_video_pspdev_mouse_setyrange (int y1, int y2)
{
	s_video_pspdev_data_t *priv = server->driver->driver_data;
	priv->ry[0] = y1;
	priv->ry[1] = y2;
}
