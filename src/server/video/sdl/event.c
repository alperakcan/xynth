/***************************************************************************
    begin                : Tue Feb 22 2005
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

#if defined(VIDEO_SDL)

#include "../../../lib/xynth_.h"
#include "server.h"
#include "sdl.h"

#include <unistd.h>

void * s_video_sdl_event_parse (void *arg)
{
	SDL_Event event;
	s_video_sdl_data_t *priv = (s_video_sdl_data_t *) server->driver->driver_data;

	while (server->window->running != 1) {
		usleep(20000);
	}
	
	while (SDL_WaitEvent(&event) >= 0) {
		switch (event.type) {
			case SDL_ACTIVEEVENT:
				break;
			case SDL_KEYUP:
			case SDL_KEYDOWN:
				if (priv->keybd_fd[1] != -1) {
					s_pipe_api_write(priv->keybd_fd[1], &event.key, sizeof(event.key));
				}
				break;
			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEBUTTONDOWN:
				if (priv->mouse_fd[1] != -1) {
					s_pipe_api_write(priv->mouse_fd[1], &event.type, sizeof(event.type));
				}
				break;
			case SDL_QUIT:
				s_server_quit(server->window);
				goto end;
			default:
//				debugf(DSER, "Event received : %d", event.type);
				break;

		}
		memset(&event, 0, sizeof(SDL_Event));
	}

end:	return NULL;
}

#endif /* VIDEO_SDL */
