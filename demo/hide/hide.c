/***************************************************************************
    begin                : Wed Mar 9 2005
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
#include <unistd.h>
#include <time.h>
#include "xynth_.h"

static int timer_count = 0;

#define SLEEP_TIME	1000

static void hide_timer (s_window_t *window, s_timer_t *timer)
{
	timer_count = (timer_count + 1) % 5;
	switch (timer_count) {
		case 0:
			s_window_hide(window);
			break;
		case 1:
			s_window_show(window);
			break;
		case 2:
			s_window_set_coor(window, 0, rand() % window->surface->width, rand() % window->surface->height,
		        	                     rand() % window->surface->width,  rand() % window->surface->height);
		 	break;
		case 3:
			s_window_set_title(window, "Demo - hide / changed title");
			break;
		case 4:
			s_window_set_title(window, "Demo - hide");
			break;
	}
}

int main (int argc, char *argv[])
{
	int x = 60;
	int y = 60;
	int w = 300;
	int h = 200;
	int mw = 1000;
	int mh = 1000;

	s_timer_t *timer;
	s_window_t *window;

	srand(time(NULL));

	s_window_init(&window);

	s_window_new(window, WINDOW_MAIN, NULL);
	mw = window->surface->width;
	mh = window->surface->height;

	s_window_set_title(window, "Demo - %s", argv[0]);
	s_window_set_coor(window, 0, x, y, MIN(mw, w), MIN(mh, h));

	s_fillbox(window->surface, mw/4, mh/4, mw/4, mh/4, s_rgbcolor(window->surface, 255, 0, 0));
	s_fillbox(window->surface, mw/2, mh/4, mw/4, mh/4, s_rgbcolor(window->surface, 0, 255, 0));
	s_fillbox(window->surface, mw/4, mh/2, mw/4, mh/4, s_rgbcolor(window->surface, 0, 0, 255));
	s_fillbox(window->surface, mw/2, mh/2, mw/4, mh/4, s_rgbcolor(window->surface, 255, 255, 255));

	s_timer_init(&timer);
	timer->timeval = SLEEP_TIME;
	timer->cb = hide_timer;
	s_timer_add(window, timer);

	s_window_show(window);
	s_window_main(window);

	return 0;
}

#if defined(SINGLE_APP)
s_single_app_t single_hide = {
	hide_main,
	1,
	{"hide"}
};
#endif
