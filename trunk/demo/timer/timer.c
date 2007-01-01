/***************************************************************************
    begin                : Mon May 23 2005
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

#include <stdio.h>
#include <stdlib.h>
#include "xynth_.h"

static void timer1_cb (s_window_t *window, s_timer_t *timer)
{
	debugf(DCLI, "[%d]", window->id);
	s_thread_mutex_lock(window->timers->mut);
	if (s_list_get_pos(window->timers->timers, timer) >= 0) {
		debugf(DCLI, "[%d] found!", window->id);
	}
	s_thread_mutex_unlock(window->timers->mut);
}

static void timer0_cb (s_window_t *window, s_timer_t *timer)
{
	debugf(DCLI, "[%d]", window->id);
	s_thread_mutex_lock(window->timers->mut);
	if (s_list_get_pos(window->timers->timers, timer) >= 0) {
		debugf(DCLI, "[%d] found!", window->id);
	}
	s_thread_mutex_unlock(window->timers->mut);
	s_timer_del(window, timer);
	s_timer_uninit(timer);
}

static void timer_handler_p (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_timer_t *timer;
	s_timer_init(&timer);
	timer->timeval = 2000;
	timer->cb = timer0_cb;
	s_timer_add(window, timer);
}

int main (int argc, char *argv[])
{
	int x = 60;
	int y = 60;
	int w = 400;
	int h = 300;
	int mw = 1000;
	int mh = 1000;
	char *box;
	s_timer_t *timer;
	s_handler_t *hndl;
	s_window_t *window;

	s_window_init(&window);

	s_window_new(window, WINDOW_MAIN, NULL);
	mw = window->surface->width;
	mh = window->surface->height;
	box = (char *) s_malloc(sizeof(char) * (mw / 4) * (mh / 4) * window->surface->bytesperpixel);

	s_window_set_title(window, "Demo - %s", argv[0]);
	s_window_set_coor(window, 0, x, y, w, h);

	s_fillbox(window->surface, mw/4, mh/4, mw/4, mh/4, s_rgbcolor(window->surface, 255, 0, 0));
	s_fillbox(window->surface, mw/2, mh/4, mw/4, mh/4, s_rgbcolor(window->surface, 0, 255, 0));
	s_fillbox(window->surface, mw/4, mh/2, mw/4, mh/4, s_rgbcolor(window->surface, 0, 0, 255));
	s_fillbox(window->surface, mw/2, mh/2, mw/4, mh/4, s_rgbcolor(window->surface, 255, 255, 255));

	s_getbox(window->surface, mw/4, mh/4, mw/4, mh/4, box);
	s_putbox(window->surface, 0, 0, mw/4, mh/4, box);
	s_free(box);

	s_fillbox(window->surface, 0, 0, 20, 20, s_rgbcolor(window->surface, 222, 222, 222));
	s_fillbox(window->surface, 2, 2, 16, 16, s_rgbcolor(window->surface, 0, 0, 0));
	s_handler_init(&hndl);
	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = 2;
	hndl->mouse.y = 2;
	hndl->mouse.w = 16;
	hndl->mouse.h = 16;
	hndl->mouse.p = timer_handler_p;
	hndl->mouse.button = MOUSE_LEFTBUTTON;
	s_handler_add(window, hndl);

	s_timer_init(&timer);
	timer->timeval = 2000;
	timer->cb = timer0_cb;
	s_timer_add(window, timer);

	s_timer_init(&timer);
	timer->timeval = 2500;
	timer->cb = timer1_cb;
	s_timer_add(window, timer);

	s_window_show(window);
	s_window_main(window);

	return 0;
}

#if defined(SINGLE_APP)
s_single_app_t single_timer = {
	timer_main,
	1,
	{"timer"}
};
#endif
