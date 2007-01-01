/***************************************************************************
    begin                : Thu Jan 30 2003
    copyright            : (C) 2003 - 2007 by Alper Akcan
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

#include <stdio.h>
#include <stdlib.h>
#include "xynth_.h"

static void temp_handler (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
        int mw;
        int mh;

	s_handler_t *hndl;
	s_window_t *child;
	s_window_init(&child);
	s_window_new(child, WINDOW_TEMP | WINDOW_NOFORM, window);
	s_window_set_coor(child, 0, window->surface->buf->x + 30,
	                            window->surface->buf->y + 30,
	                            150,
	                            150);
	s_window_set_title(child, "Demo - Temp");
	mw = child->surface->buf->w;
	mh = child->surface->buf->h;

	s_fillbox(child->surface, 0, 0, mw, mh, s_rgbcolor(child->surface, 255, 255, 255));
	s_fillbox(child->surface, 1, 1, mw - 2, mh - 2, s_rgbcolor(child->surface, 0, 0, 0));
	s_fillbox(child->surface, 2, 2, mw - 4, mh - 4, s_rgbcolor(child->surface, 180, 180, 180));

	s_fillbox(child->surface, 2, 2, 20, 20, s_rgbcolor(window->surface, 0, 0, 0));
	s_fillbox(child->surface, 4, 4, 16, 16, s_rgbcolor(window->surface, 255, 0, 0));

	s_handler_init(&hndl);
	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = 2;
	hndl->mouse.y = 2;
	hndl->mouse.w = 20;
	hndl->mouse.h = 20;
	hndl->mouse.p = temp_handler;
	hndl->mouse.button = MOUSE_LEFTBUTTON;
	s_handler_add(child, hndl);

	s_window_show(child);
	s_window_main(child);
}

int main (int argc, char *argv[])
{
	int x = 60;
	int y = 60;
	int w = 400;
	int h = 300;
	int mw = 1000;
	int mh = 1000;
	s_handler_t *hndl;
	s_window_t *window;

	s_window_init(&window);

	s_window_new(window, WINDOW_MAIN, NULL);
	mw = window->surface->width;
	mh = window->surface->height;

	s_window_set_title(window, "Demo - %s  ", argv[0]);
	s_window_set_coor(window, 0, x, y, MIN(mw, w), MIN(mh, h));

	s_fillbox(window->surface, mw/4, mh/4, mw/4, mh/4, s_rgbcolor(window->surface, 255, 0, 0));
	s_fillbox(window->surface, mw/2, mh/4, mw/4, mh/4, s_rgbcolor(window->surface, 0, 255, 0));
	s_fillbox(window->surface, mw/4, mh/2, mw/4, mh/4, s_rgbcolor(window->surface, 0, 0, 255));
	s_fillbox(window->surface, mw/2, mh/2, mw/4, mh/4, s_rgbcolor(window->surface, 255, 255, 255));

	s_fillbox(window->surface, 0, 0, 20, 20, s_rgbcolor(window->surface, 222, 222, 222));
	s_fillbox(window->surface, 2, 2, 16, 16, s_rgbcolor(window->surface, 0, 0, 0));
	s_handler_init(&hndl);
	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = 0;
	hndl->mouse.y = 0;
	hndl->mouse.w = 20;
	hndl->mouse.h = 20;
	hndl->mouse.p = temp_handler;
	hndl->mouse.button = MOUSE_LEFTBUTTON;
	s_handler_add(window, hndl);

	s_window_show(window);
	s_window_main(window);

	return 0;
}

#if defined(SINGLE_APP)
s_single_app_t single_temp = {
	temp_main,
	1,
	{"temp"}
};
#endif
