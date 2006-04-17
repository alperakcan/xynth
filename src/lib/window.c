/***************************************************************************
    begin                : Fri Feb 21 2003
    copyright            : (C) 2003 - 2006 by Alper Akcan
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

#include "xynth_.h"

void s_window_set_cursor (s_window_t *window, S_MOUSE_CURSOR cursor)
{
	s_socket_request(window, SOC_DATA_CURSOR, cursor);
}

void s_window_set_title (s_window_t *window, char *fmt, ...)
{
        int n;
        va_list ap;
        int size = 1;

	if (window->client->title != NULL) {
		s_free(window->client->title);
	}
	window->client->title = (char *) s_malloc(sizeof(char) * size);

	while (1) {
		va_start(ap, fmt);
		n = vsnprintf(window->client->title, size, fmt, ap);
		va_end(ap);
		if ((n > -1) && (n < size)) {
			break;
		}
		if (n > -1) {
			size = n + 1;
		} else {
			size *= 2;
		}
		window->client->title = (char *) s_realloc(window->client->title, size);
	}

	s_socket_request(window, SOC_DATA_CONFIGURE, WINDOW_NOFORM);
}

void s_window_form_draw (s_window_t *window)
{
	if (window->type & WINDOW_NOFORM) {
		return;
	}
	s_socket_request(window, SOC_DATA_FORMDRAW);
}

void s_window_hide (s_window_t *window)
{
	s_socket_request(window, SOC_DATA_HIDE);
	window->surface->mode &= ~SURFACE_REAL;
}

void s_window_show (s_window_t *window)
{
	s_socket_request(window, SOC_DATA_SHOW);
	window->surface->mode |= SURFACE_REAL;
}

void s_window_set_coor (s_window_t *window, int form, int x, int y, int w, int h)
{
	window->surface->buf->x = x;
	window->surface->buf->y = y;
	window->surface->buf->w = w;
	window->surface->buf->h = h;
	s_socket_request(window, SOC_DATA_CONFIGURE, form);
}

void s_window_set_resizeable (s_window_t *window, int resizeable)
{
	window->client->resizeable = resizeable;
	s_socket_request(window, SOC_DATA_CONFIGURE, WINDOW_NOFORM);
}

void s_window_set_alwaysontop (s_window_t *window, int alwaysontop)
{
	window->client->alwaysontop = alwaysontop;
	s_socket_request(window, SOC_DATA_CONFIGURE, WINDOW_NOFORM);
}

int s_window_new (s_window_t *window, S_WINDOW type, s_window_t *parent)
{
	if (!(type & (WINDOW_MAIN | WINDOW_CHILD | WINDOW_TEMP))) {
		goto err;
	}
	window->type = type;
	if (window->surface->width <= 0) {
		window->surface->width = window->surface->linear_buf_width;
	}
	if (window->surface->height <= 0) {
		window->surface->height = window->surface->linear_buf_height;
	}
	if (window->type & (WINDOW_TEMP | WINDOW_CHILD)) {
		window->parent = parent;
	} else {
		window->parent = NULL;
	}

	s_socket_request(window, SOC_DATA_NEW);
	window->surface->need_expose = SURFACE_NEEDSTREAM;
	s_surface_create(window);

	return 0;
err:	return 1;
}
