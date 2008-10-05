/***************************************************************************
    begin                : Fri Feb 21 2003
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

#include "xynth_.h"

void s_window_set_cursor (s_window_t *window, s_cursor_type_t cursor)
{
	if ((cursor >= 0) &&
	    (cursor < CURSOR_TYPE_MAX)) {
		window->cursor = cursor;
		s_socket_request(window, SOC_DATA_CONFIGURE, WINDOW_TYPE_NOFORM);
	}
}

void s_window_set_title (s_window_t *window, const char *fmt, ...)
{
        int n;
        va_list ap;
        int size = 1;

	if (window->title != NULL) {
		s_free(window->title);
	}
	window->title = (char *) s_malloc(sizeof(char) * size);

	while (1) {
		va_start(ap, fmt);
		n = vsnprintf(window->title, size, fmt, ap);
		va_end(ap);
		if ((n > -1) && (n < size)) {
			break;
		}
		if (n > -1) {
			size = n + 1;
		} else {
			size *= 2;
		}
		window->title = (char *) s_realloc(window->title, size);
	}

	s_socket_request(window, SOC_DATA_CONFIGURE, WINDOW_TYPE_NOFORM);
}

void s_window_form_draw (s_window_t *window)
{
	if (window->type & WINDOW_TYPE_NOFORM) {
		return;
	}
	s_socket_request(window, SOC_DATA_FORMDRAW);
}

void s_window_hide (s_window_t *window)
{
	s_socket_request(window, SOC_DATA_SHOW, window->id, NULL, -1);
//	window->surface->mode &= ~SURFACE_REAL;
}

void s_window_show (s_window_t *window)
{
	s_socket_request(window, SOC_DATA_SHOW, window->id, NULL, 0);
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
	window->resizeable = resizeable;
	s_socket_request(window, SOC_DATA_CONFIGURE, WINDOW_TYPE_NOFORM);
}

void s_window_set_alwaysontop (s_window_t *window, int alwaysontop)
{
	window->alwaysontop = alwaysontop;
	s_socket_request(window, SOC_DATA_CONFIGURE, WINDOW_TYPE_NOFORM);
}

int s_window_new (s_window_t *window, s_window_type_t type, s_window_t *parent)
{
	if (!(type & (WINDOW_TYPE_MAIN | WINDOW_TYPE_CHILD | WINDOW_TYPE_TEMP))) {
		goto err;
	}
	window->type = type;
	if (window->surface->width <= 0) {
		window->surface->width = window->surface->linear_buf_width;
	}
	if (window->surface->height <= 0) {
		window->surface->height = window->surface->linear_buf_height;
	}
	if (window->type & (WINDOW_TYPE_TEMP | WINDOW_TYPE_CHILD)) {
		window->parent = parent;
	} else {
		window->parent = NULL;
	}

	s_socket_request(window, SOC_DATA_NEW);
	window->surface->vbuf = (unsigned char *) s_calloc(sizeof(char), window->surface->width *
	                                                                 window->surface->height *
	                                                                 window->surface->bytesperpixel);

	return 0;
err:	return 1;
}

int s_window_init (s_window_t **window)
{
	s_window_t *w;

	w = (s_window_t *) s_calloc(1, sizeof(s_window_t));
	if (s_pollfds_init(w))       { goto err1; }
	if (s_timers_init(w))        { goto err2; }
	if (s_socket_init(w))        { goto err3; }
        if (s_handlers_init(w))      { goto err4; }
	if (s_childs_init(w))        { goto err5; }
	if (s_eventq_init(w))        { goto err6; }
	if (s_event_init(&w->event)) { goto err7; }

	w->running = 1;
	w->id = -1;
	w->pri = -1;
	w->resizeable = 1;
	w->alwaysontop = 0;
	w->cursor = CURSOR_TYPE_ARROW;
	w->mouse_entered = 0;
	*window = w;

	if (s_surface_init(w))                     { goto err8; }
	if (s_socket_request(w, SOC_DATA_DISPLAY)) { goto err9; }
	if (s_surface_attach(w))                   { goto err10; }

	return 0;
err10:
err9:	s_surface_uninit(w);
err8:	s_event_uninit(w->event);
err7:	s_eventq_uninit(w);
err6:	s_childs_uninit(w);
err5:	s_handlers_uninit(w);
err4:
err3:	s_timers_uninit(w);
err2:	s_pollfds_uninit(w);
err1:	s_free(w);
	debugf(DCLI | DFAT, "Cannot connect to server");
	return -1;
}

void s_window_uninit (s_window_t *window)
{
	if (window == NULL) {
		return;
	}

        window->event->type = EVENT_TYPE_QUIT;
        s_event_changed(window);
	s_thread_join(window->eventq->tid, NULL);

	s_timers_uninit(window);
	s_pollfds_uninit(window);
	s_handlers_uninit(window);

	if (window->atexit != NULL) {
		window->atexit(window);
	}

	s_childs_uninit(window);

        if (window->type & (WINDOW_TYPE_TEMP | WINDOW_TYPE_CHILD)) {
		if (s_child_del(window->parent, window) == 0) {
			s_free(window->tid);
		}
	}

	debugf(DCLI, "[%d] Exiting (%s%s)", window->id, (window->type & WINDOW_TYPE_MAIN) ? "WINDOW_TYPE_MAIN" :
	                                                 ((window->type & WINDOW_TYPE_CHILD) ? "WINDOW_TYPE_CHILD" :
	                                                 ((window->type & WINDOW_TYPE_TEMP) ? "WINDOW_TYPE_TEMP" :
	                                                 ((window->type & WINDOW_TYPE_DESKTOP) ? "WINDOW_TYPE_DESKTOP" : "WINDOW_UNKNOWN"))),
	                                                (window->type & WINDOW_TYPE_NOFORM) ? " | WINDOW_TYPE_NOFORM" : "");

	s_eventq_uninit(window);

	s_surface_uninit(window);
	s_event_uninit(window->event);

	s_free(window->title);
	s_free(window);
	window = NULL;
}

void s_window_exit (s_window_t *window)
{
	if (window == NULL) {
		return;
	}
	s_window_uninit(window);
}

void s_window_quit (s_window_t *window)
{
        window->event->type = EVENT_TYPE_QUIT;
        s_event_changed(window);
//	s_socket_request(window, SOC_DATA_CLOSE);
}

int s_window_wakeup (s_window_t *window)
{
	int w = 0;
	return s_pipe_api_write(window->wsoc, &w, sizeof(int));
}

void * s_window_loop_event (void *arg)
{
	s_event_t *event;
	s_window_t *window;

	window = (s_window_t *) arg;

	while (window->running > 0) {
		if (s_eventq_wait(window, &event) != 0) {
			continue;
		}
        	if (window->atevent != NULL) {
			window->atevent(window, event);
		}
		switch (event->type & EVENT_TYPE_MASK) {
//			case EVENT_TYPE_QUIT:   window->running = 0;                      break;
			case EVENT_TYPE_QUIT:
				 s_socket_request(window, SOC_DATA_CLOSE);
				 s_event_uninit(event);
				 return NULL;
				 break;
			case EVENT_TYPE_MOUSE:  s_event_parse_mouse(window, event);       break;
			case EVENT_TYPE_KEYBOARD:  s_event_parse_keybd(window, event);       break;
			case EVENT_TYPE_EXPOSE: s_event_parse_expos(window, event);       break;
			case EVENT_TYPE_CONFIG: s_event_parse_config(window, event);      break;
			case EVENT_TYPE_TIMER:  s_event_parse_timer(window, event);       break;
			case EVENT_TYPE_FOCUS:                                            break;
			case EVENT_TYPE_DESKTOP:				             break;
		}
		s_event_uninit(event);
	}

	return NULL;
}

void * s_window_loop (void *arg)
{
	s_window_t *window = (s_window_t *) arg;

	window->eventq->tid = s_thread_create(&s_window_loop_event, (void *) window);

	while (window->running > 0) {
		if (s_socket_listen_wait(window, -1)) {
			break;
		}
	}

	s_window_exit(window);

	return NULL;
}

void * s_window_main (void *arg)
{
	s_window_t *window = (s_window_t *) arg;

	switch (window->type & WINDOW_TYPES) {
		case WINDOW_TYPE_MAIN:
			s_window_loop(window);
			break;
		case WINDOW_TYPE_TEMP:
		case WINDOW_TYPE_CHILD:
			window->tid = s_thread_create(&s_window_loop, (void *) window);
			s_child_add(window->parent, window);
			break;
	}

	return NULL;
}

void s_window_atevent (s_window_t *window, void (*f) (s_window_t *, s_event_t *))
{
	window->atevent = f;
}

void s_window_atexit (s_window_t *window, void (*f) (s_window_t *))
{
	window->atexit = f;
}
