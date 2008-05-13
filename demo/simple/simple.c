/***************************************************************************
    begin                : Thu Jan 30 2003
    copyright            : (C) 2003 - 2008 by Alper Akcan
    email                : alper.akcan@gmail.com
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

static int i = 120;
static int c = CURSOR_TYPE_ARROW;

static void simple_atevent (s_window_t *window, s_event_t *event)
{
	if (event->type & EVENT_TYPE_QUIT) {
		debugf(DCLI, "[%d] quiting", window->id);
	}
        if (event->type & EVENT_TYPE_KEYBOARD) {
		debugf(DCLI, "[%d] event: 0x%08X, f: 0x%08X, b: %d, a:%c", window->id, event->type, event->keybd->flag, event->keybd->button, event->keybd->ascii);
	}
	if (event->type & EVENT_TYPE_MOUSE) {
		debugf(DCLI, "[%d] event->type : 0x%08X, button : 0x%08X, buttons : 0x%08X", window->id, event->type, event->mouse->b, event->mouse->buttons);
	}
}

static void simple_handler1o (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_window_set_title(window, "Demo - Simple %d", i--);
}

static void simple_handler1c (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	debugf(DCLI, "[%d] click count : %d", window->id, event->mouse->clicks);
}

static void simple_handler1p (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_window_set_coor(window, 0, 0, 0, 300, 300);
}

static void simple_handler1r (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
//	s_fillbox(window->surface, 100, 100, 100, 100, s_rgbcolor(window->surface, 255, 0, 0));
}

static void simple_handler2p (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
//	s_window_set_coor(window, WINDOW_TYPE_NOFORM, 0, 0, 200, 300);
	s_window_set_cursor(window, c++);
	if (c >= CURSOR_TYPE_MAX) {
		c = CURSOR_TYPE_ARROW;
	}
}

static void simple_handler3p (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_window_set_resizeable(window, 1);
}

static void simple_handler4p (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_window_set_resizeable(window, 0);
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

	s_window_new(window, WINDOW_TYPE_MAIN, NULL);
	
	mw = window->surface->width;
	mh = window->surface->height;

	s_window_set_title(window, "Demo - %s", argv[0]);
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
	hndl->mouse.o = simple_handler1o;
	hndl->mouse.c = simple_handler1c;
	hndl->mouse.p = simple_handler1p;
	hndl->mouse.r = simple_handler1r;
	hndl->mouse.button = MOUSE_BUTTON_LEFT;
	s_handler_add(window, hndl);
	
	s_handler_init(&hndl);
	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = 0;
	hndl->mouse.y = 0;
	hndl->mouse.w = 20;
	hndl->mouse.h = 20;
	hndl->mouse.p = simple_handler2p;
	hndl->mouse.button = MOUSE_BUTTON_RIGHT;
	s_handler_add(window, hndl);

	s_fillbox(window->surface, 20, 0, 20, 20, s_rgbcolor(window->surface, 222, 222, 222));
	s_fillbox(window->surface, 22, 2, 16, 16, s_rgbcolor(window->surface, 0, 0, 0));
	s_handler_init(&hndl);
	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = 20;
	hndl->mouse.y = 0;
	hndl->mouse.w = 20;
	hndl->mouse.h = 20;
	hndl->mouse.p = simple_handler3p;
	hndl->mouse.button = MOUSE_BUTTON_LEFT;
	s_handler_add(window, hndl);

	s_handler_init(&hndl);
	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = 20;
	hndl->mouse.y = 0;
	hndl->mouse.w = 20;
	hndl->mouse.h = 20;
	hndl->mouse.p = simple_handler4p;
	hndl->mouse.button = MOUSE_BUTTON_RIGHT;
	s_handler_add(window, hndl);

	s_window_atevent(window, simple_atevent);

	s_window_show(window);

	debugf(DCLI, "[%d] window->surface->need_expose == %d", window->id, window->surface->need_expose);

	s_window_main(window);

	return 0;
}

#if defined(CONFIG_SINGLE_APPLICATION)
s_single_app_t single_simple = {
	simple_main,
	1,
	{"simple"}
};
#endif
