/***************************************************************************
    begin                : Mon Feb 21 2005
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

#if defined(VIDEO_SDL)

#include "../../../lib/xynth_.h"
#include "../../server.h"
#include "sdl.h"
#include "../helper/helper.h"

#include <unistd.h>

int s_video_sdl_mouse_init (s_server_conf_t *cfg)
{
	s_video_sdl_data_t *priv = server->driver->driver_data;
	if (s_pipe_api_pipe(priv->mouse_fd)) {
		debugf(DSER | DSYS, "pipe failed");
		goto err0;
	}
	
	SDL_ShowCursor(0);
	s_video_helper_mouse_cursor_init();
	s_mouse_setxrange(server->window, 0, server->window->surface->width);
	s_mouse_setyrange(server->window, 0, server->window->surface->height);
	return priv->mouse_fd[0];
err0:	priv->mouse_fd[0] = -1;
	priv->mouse_fd[1] = -1;
	return -1;
}

int s_video_sdl_mouse_update (s_mouse_driver_t *mouse)
{
        int x;
        int y;
	SDL_Event event;
        unsigned int state;
	s_video_sdl_data_t *priv = server->driver->driver_data;
	
        s_pipe_api_read(priv->mouse_fd[0], &event.type, sizeof(event.type));

        state = SDL_GetMouseState(&x, &y);

	x = MAX(x, priv->rx[0]);
	x = MIN(x, priv->rx[1]);
	y = MAX(y, priv->ry[0]);
	y = MIN(y, priv->ry[1]);

        mouse->x = x;
        mouse->y = y;
        mouse->buttons = 0;
	
        if (state & (1 << (SDL_BUTTON_LEFT - 1))) {
		mouse->buttons |= MOUSE_LEFTBUTTON;
	}
        if (state & (1 << (SDL_BUTTON_MIDDLE - 1))) {
		mouse->buttons |= MOUSE_MIDDLEBUTTON;
	}
        if (state & (1 << (SDL_BUTTON_RIGHT - 1))) {
		mouse->buttons |= MOUSE_RIGHTBUTTON;
	}

	return 0;
}

void s_video_sdl_mouse_uninit (void)
{
	s_video_sdl_data_t *priv = server->driver->driver_data;
	s_pipe_api_close(priv->mouse_fd[0]);
	s_pipe_api_close(priv->mouse_fd[1]);
	priv->mouse_fd[0] = -1;
	priv->mouse_fd[1] = -1;
	s_server_cursor_uninit();
}

void s_video_sdl_mouse_setxrange (int x1, int x2)
{
	s_video_sdl_data_t *priv = server->driver->driver_data;
	priv->rx[0] = x1;
	priv->rx[1] = x2;
}

void s_video_sdl_mouse_setyrange (int y1, int y2)
{
	s_video_sdl_data_t *priv = server->driver->driver_data;
	priv->ry[0] = y1;
	priv->ry[1] = y2;
}

#endif /* VIDEO_SDL */
