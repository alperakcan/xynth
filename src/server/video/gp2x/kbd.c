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

int s_video_gp2x_kbd_init (s_server_conf_t *cfg)
{
	s_video_gp2x_data_t *priv = server->driver->driver_data;
	if (s_pipe_api_pipe(priv->keybd_fd)) {
		debugf(DSER | DSYS, "pipe failed");
		goto err0;
	}
	return priv->keybd_fd[0];
err0:   priv->keybd_fd[0] = -1;
	priv->keybd_fd[1] = -1;
	return -1;
}

int s_video_gp2x_kbd_update (s_video_input_data_t *keybd)
{
        s_video_input_data_keybd_t key;
	s_video_gp2x_data_t *priv = server->driver->driver_data;

	s_pipe_api_read(priv->keybd_fd[0], &key, sizeof(s_video_input_data_keybd_t));

	keybd->keybd.ascii = -1;
	keybd->keybd.scancode = key.button;
	keybd->keybd.keycode = key.button;
	keybd->keybd.button = key.button;
	keybd->keybd.state = key.state;
	return 0;
}

void s_video_gp2x_kbd_uninit (void)
{
	s_video_gp2x_data_t *priv = server->driver->driver_data;
	s_pipe_api_close(priv->keybd_fd[0]);
	s_pipe_api_close(priv->keybd_fd[1]);
	priv->keybd_fd[0] = -1;
	priv->keybd_fd[1] = -1;
}

#endif /* VIDEO_GP2X */
