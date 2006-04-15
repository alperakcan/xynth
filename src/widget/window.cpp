/***************************************************************************
    begin                : Fri Sep 24 2004
    copyright            : (C) 2004 - 2006 by Alper Akcan
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

#include "../lib/xynth_.h"
#include "widget.h"

void SWindow::windowSetTitle (char *fmt, ...)
{
        int n;
        char *buf;
        va_list ap;
        int size = 1;

	buf = (char *) s_malloc(sizeof(char) * size);

	while (1) {
		va_start(ap, fmt);
		n = vsnprintf(buf, size, fmt, ap);
		va_end(ap);
		if ((n > -1) && (n < size)) {
			break;
		}
		if (n > -1) {
			size = n + 1;
		} else {
			size *= 2;
		}
		buf = (char *) s_realloc(buf, size);
	}

	s_window_set_title(objectWindow, buf);

	s_free(buf);
}

void SWindow::windowFormDraw (void)
{
	s_window_form_draw(objectWindow);
}

void SWindow::windowShow (void)
{
	s_window_show(objectWindow);
}

void SWindow::windowSetCoor (int x, int y, int w, int h, int with_form)
{
	s_window_set_coor(objectWindow, (with_form & WINDOW_NOFORM) ? WINDOW_NOFORM : 0, x, y, w, h);
}

void SWindow::windowSetResizeable (int resizeable)
{
	s_window_set_resizeable(objectWindow, resizeable);
}

void SWindow::windowMain (void)
{
	s_client_main(objectWindow);
}

void SWindow::windowAtExitCallback (s_window_t *window)
{
	SWindow *win;
	win = (SWindow *) window->client->user_data;
	win->windowAtExit(window);
}

void SWindow::windowAtEventCallback (s_window_t *window, s_event_t *event)
{
	SWindow *win;
	win = (SWindow *) window->client->user_data;
	win->windowAtEvent(window, event);
}

void SWindow::windowAtExit (s_window_t *window)
{
	delete objectChild;
}

void SWindow::windowAtEvent (s_window_t *window, s_event_t *event)
{
	switch (event->type & EVENT_MASK) {
		case EXPOSE_EVENT:
			if ((event->expose->change & (EXPOSE_CHNGW | EXPOSE_CHNGH)) ||
			    (objectWindow->surface->vbuf == NULL)) {
				objectWindow->surface->width = objectWindow->surface->buf->w;
				objectWindow->surface->height = objectWindow->surface->buf->h;
				objectWindow->surface->vbuf = (char *) s_realloc(objectWindow->surface->vbuf,
				                                                 objectWindow->surface->width *
				                                                 objectWindow->surface->height *
				                                                 objectWindow->surface->bytesperpixel);
				geometry(0, 0, objectWindow->surface->width, objectWindow->surface->height);
				
				objectWindow->surface->mode = SURFACE_VIRTUAL;
				draw();
				objectWindow->surface->mode = (S_SURFACE_MODE) (SURFACE_VIRTUAL | SURFACE_REAL);
			}
			break;
	}
}

void SWindow::objectDraw (void)
{
	objectFillBox(0, 0, objectWindow->surface->width, objectWindow->surface->height, objectRGBColor(220, 220, 220));
}

void SWindow::objectGeometry (int x, int y, int w, int h)
{
	objectSetRectBuffer(x, y, w, h);
	objectSetRectContents(x + 1, y + 1, w - 2, h - 2);
	if ((w != objectWindow->surface->width) ||
	    (h != objectWindow->surface->height)) {
		windowSetCoor(objectWindow->surface->buf->x, objectWindow->surface->buf->y, w, h);
	}
}

SWindow::SWindow (S_WINDOW type, s_window_t *parent) : SObject()
{
	s_client_init(&objectWindow);

	s_window_new(objectWindow, type, parent);
	
        s_client_atexit(objectWindow, windowAtExitCallback);
	s_client_atevent(objectWindow, windowAtEventCallback);

	objectWindow->client->user_data = this;

        s_free(objectWindow->surface->vbuf);
	objectWindow->surface->vbuf = NULL;
}

SWindow::~SWindow (void)
{
}
