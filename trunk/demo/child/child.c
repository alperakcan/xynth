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
#include <unistd.h>
#include <time.h>
#include "xynth_.h"

static int i = 0;

static void child_atevent (s_window_t *window, s_event_t *event)
{
	if (event->type & FOCUS_EVENT) {
		debugf(DCLI, "[%d] %s focus", window->id, (window->pri == 0) ? "gained" : "lost");
	}
}

static void child_handler1c (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
        i++;
	i %= 3;

	s_window_set_alwaysontop(window, (i == 2) ? -1 : i);
}

static void child_handler_1 (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_thread_mutex_lock(window->childs->mut);
	if (window->childs->list->nb_elt > 0) {
		s_window_t *wc = (s_window_t *) s_list_get(window->childs->list, rand() % window->childs->list->nb_elt);
		s_window_show(wc);
	}
	s_thread_mutex_unlock(window->childs->mut);
}

static void child_handler_2 (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_window_set_coor(window, WINDOW_NOFORM, 0, 0, 0, 0);
	sleep(1);
	s_window_set_coor(window, WINDOW_NOFORM, 100, 100, 100, 100);
}

static void child_handler_open (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
        int mw;
        int mh;

	s_handler_t *hndl;

	s_window_t *child;

	s_window_init(&child);

        s_window_new(child, WINDOW_CHILD, window);

	s_window_set_coor(child, WINDOW_NOFORM, window->surface->buf->x + 100,
						window->surface->buf->y + 100,
						window->surface->buf->w,
						window->surface->buf->h);
	s_window_set_title(child, "Demo - Child ");

	mw = child->surface->width;
	mh = child->surface->height;

	s_fillbox(child->surface, mw/4, mh/4, mw/4, mh/4, s_rgbcolor(child->surface, 0, 255, 0));
	s_fillbox(child->surface, mw/2, mh/4, mw/4, mh/4, s_rgbcolor(child->surface, 0, 0, 255));
	s_fillbox(child->surface, mw/4, mh/2, mw/4, mh/4, s_rgbcolor(child->surface, 255, 0, 0));
	s_fillbox(child->surface, mw/2, mh/2, mw/4, mh/4, s_rgbcolor(child->surface, 255, 255, 255));

	s_fillbox(child->surface, 0, 0, 20, 20, s_rgbcolor(window->surface, 222, 222, 222));
	s_fillbox(child->surface, 2, 2, 16, 16, s_rgbcolor(window->surface, 255, 0, 0));

	s_handler_init(&hndl);
	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = 0;
	hndl->mouse.y = 0;
	hndl->mouse.w = 20;
	hndl->mouse.h = 20;
	hndl->mouse.c = child_handler_open;
	hndl->mouse.button = MOUSE_LEFTBUTTON;
	s_handler_add(child, hndl);

	s_fillbox(child->surface, 20, 0, 20, 20, s_rgbcolor(window->surface, 222, 222, 222));
	s_fillbox(child->surface, 22, 2, 16, 16, s_rgbcolor(window->surface, 255, 0, 0));
	s_handler_init(&hndl);
	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = 20;
	hndl->mouse.y = 0;
	hndl->mouse.w = 20;
	hndl->mouse.h = 20;
	hndl->mouse.c = child_handler1c;
	hndl->mouse.button = MOUSE_LEFTBUTTON;
	s_handler_add(child, hndl);

	s_fillbox(child->surface, 40, 0, 20, 20, s_rgbcolor(window->surface, 222, 222, 222));
	s_fillbox(child->surface, 42, 2, 16, 16, s_rgbcolor(window->surface, 255, 0, 0));
	s_handler_init(&hndl);
	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = 40;
	hndl->mouse.y = 0;
	hndl->mouse.w = 20;
	hndl->mouse.h = 20;
	hndl->mouse.p = child_handler_2;
	hndl->mouse.button = MOUSE_LEFTBUTTON;
	s_handler_add(child, hndl);

	s_window_show(child);
	s_window_atevent(child, child_atevent);

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

	srand(time(NULL));

	s_window_init(&window);

	s_window_new(window, WINDOW_MAIN, NULL);
	mw = window->surface->width;
	mh = window->surface->height;

	s_window_set_title(window, "Demo - %s ", argv[0]);
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
	hndl->mouse.c = child_handler_open;
	hndl->mouse.button = MOUSE_LEFTBUTTON;
	s_handler_add(window, hndl);

	s_fillbox(window->surface, 20, 0, 20, 20, s_rgbcolor(window->surface, 222, 222, 222));
	s_fillbox(window->surface, 22, 2, 16, 16, s_rgbcolor(window->surface, 0, 0, 0));
	s_handler_init(&hndl);
	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = 20;
	hndl->mouse.y = 0;
	hndl->mouse.w = 20;
	hndl->mouse.h = 20;
	hndl->mouse.p = child_handler_1;
	hndl->mouse.button = MOUSE_LEFTBUTTON;
	s_handler_add(window, hndl);

	s_fillbox(window->surface, 40, 0, 20, 20, s_rgbcolor(window->surface, 222, 222, 222));
	s_fillbox(window->surface, 42, 2, 16, 16, s_rgbcolor(window->surface, 0, 0, 0));
	s_handler_init(&hndl);
	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = 40;
	hndl->mouse.y = 0;
	hndl->mouse.w = 20;
	hndl->mouse.h = 20;
	hndl->mouse.p = child_handler_2;
	hndl->mouse.button = MOUSE_LEFTBUTTON;
	s_handler_add(window, hndl);

	s_window_show(window);

	s_window_atevent(window, child_atevent);

	s_window_main(window);

	return 0;
}

#if defined(SINGLE_APP)
s_single_app_t single_child = {
	child_main,
	1,
	{"child"}
};
#endif
