/***************************************************************************
    begin                : Tue Dec 26 2006
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

#if defined(VIDEO_CACA)

#include "../../../lib/xynth_.h"
#include "server.h"
#include "caca.h"

#include <unistd.h>

void * s_video_caca_event_parse (void *arg)
{
	s_rect_t coor;
	unsigned int event;
	unsigned int event_mask;
	s_video_input_data_t data;
	s_video_caca_data_t *priv = (s_video_caca_data_t *) server->driver->driver_data;
	
	event_mask = CACA_EVENT_ANY;

	while (server->window->running != 1) {
		usleep(20000);
	}
	
	while ((event = caca_wait_event(event_mask)) >= 0) {
		memset(&data, 0, sizeof(s_video_input_data_t));
		switch (event & CACA_EVENT_ANY) {
			case CACA_EVENT_KEY_PRESS:
			case CACA_EVENT_KEY_RELEASE:
				if (priv->keybd_fd[1] != -1) {
					s_pipe_api_write(priv->keybd_fd[1], &data, sizeof(s_video_input_data_t));
				}
				break;
			case CACA_EVENT_MOUSE_PRESS:
			case CACA_EVENT_MOUSE_RELEASE:
			case CACA_EVENT_MOUSE_MOTION:
				break;
			case CACA_EVENT_RESIZE:
				coor.x = 0;
				coor.y = 0;
				coor.w = server->window->surface->width;
				coor.h = server->window->surface->height;
				caca_refresh();
				s_server_surface_update(&coor);
				break;
			default:
//				debugf(DSER, "Event received : %d", event.type);
				break;

		}
		event = 0;
	}

end:	return NULL;
}

#endif /* VIDEO_CACA */
