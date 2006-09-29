/***************************************************************************
    begin                : Thu Feb 2 2006
    copyright            : (C) 2006 by Alper Akcan
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
#include "../../server.h"
#include "../helper/helper.h"
#include "gp2x.h"

int s_video_gp2x_mouse_init (s_server_conf_t *cfg)
{
	s_video_gp2x_data_t *priv = server->driver->driver_data;
	if (s_pipe_api_pipe(priv->mouse_fd)) {
		debugf(DSER | DSYS, "pipe failed");
		goto err0;
	}
	server->window->event->mouse->x = server->window->surface->width / 2;
	server->window->event->mouse->y = server->window->surface->height / 2;
        priv->pad_old.x = server->window->event->mouse->x;
        priv->pad_old.y = server->window->event->mouse->y;
	return priv->mouse_fd[0];
err0:	priv->mouse_fd[0] = -1;
	priv->mouse_fd[1] = -1;
	return -1;
}

int s_video_gp2x_mouse_update (s_mouse_driver_t *mouse)
{
	s_mouse_driver_t pad_recv;
	s_video_gp2x_data_t *priv = server->driver->driver_data;
	s_pipe_api_read(priv->mouse_fd[0], &pad_recv, sizeof(s_mouse_driver_t));
	mouse->x = pad_recv.x;
	mouse->y = pad_recv.y;
	mouse->buttons = 0;
	mouse->buttons |= (pad_recv.buttons & GP2X_L) ? MOUSE_LEFTBUTTON : 0;
	mouse->buttons |= (pad_recv.buttons & GP2X_R) ? MOUSE_RIGHTBUTTON : 0;
	return 0;
}

void s_video_gp2x_mouse_uninit (void)
{
	s_video_gp2x_data_t *priv = server->driver->driver_data;
	s_pipe_api_close(priv->mouse_fd[0]);
	s_pipe_api_close(priv->mouse_fd[1]);
	priv->mouse_fd[0] = -1;
	priv->mouse_fd[1] = -1;
	s_server_cursor_uninit();
}

void s_video_gp2x_mouse_setxrange (int x1, int x2)
{
	s_video_gp2x_data_t *priv = server->driver->driver_data;
	priv->rx[0] = x1;
	priv->rx[1] = x2;
}

void s_video_gp2x_mouse_setyrange (int y1, int y2)
{
	s_video_gp2x_data_t *priv = server->driver->driver_data;
	priv->ry[0] = y1;
	priv->ry[1] = y2;
}

#endif /* VIDEO_GP2X */
