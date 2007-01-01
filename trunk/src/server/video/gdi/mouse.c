/***************************************************************************
    begin                : Fri Dec 30 2005
    copyright            : (C) 2005 - 2007 by Caglar Oral
    email                : caglaroral@yahoo.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#if defined(VIDEO_GDI)

#include "../../../lib/xynth_.h"
#include "server.h"
#include "gdi.h"
#include "../helper/helper.h"

int s_video_gdi_mouse_init (s_server_conf_t *cfg)
{
	s_video_gdi_data_t *priv = server->driver->driver_data;
	if (s_pipe_api_pipe(priv->mouse_fd)) {
		debugf(DSER | DSYS, "pipe failed");
		goto err0;
	}
	return priv->mouse_fd[0];
err0:	priv->mouse_fd[0] = -1;
	priv->mouse_fd[1] = -1;
	return -1;
}

int s_video_gdi_mouse_update (s_video_input_data_t *mouse)
{
        int m;
	s_video_gdi_data_t *priv = server->driver->driver_data;
        s_pipe_api_read(priv->mouse_fd[0], &m, sizeof(int));
        mouse->mouse.x = priv->mouse.x;
        mouse->mouse.y = priv->mouse.y;
        mouse->mouse.buttons = priv->mouse.buttons;
	return 0;
}

void s_video_gdi_mouse_uninit (void)
{
	s_video_gdi_data_t *priv = server->driver->driver_data;
	s_pipe_api_close(priv->mouse_fd[0]);
	s_pipe_api_close(priv->mouse_fd[1]);
	priv->mouse_fd[0] = -1;
	priv->mouse_fd[1] = -1;
	s_server_cursor_uninit();
}

#endif /* VIDEO_GDI */
