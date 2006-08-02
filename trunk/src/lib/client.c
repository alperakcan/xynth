/***************************************************************************
    begin                : Thu Jan 30 2003
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

int s_client_init (s_window_t **window)
{
	s_window_t *w;

	w = (s_window_t *) s_calloc(1, sizeof(s_window_t));
        w->client = (s_client_t *) s_calloc(1, sizeof(s_client_t));
	if (s_surface_init(w))       { goto err1; }
	if (s_pollfds_init(w))       { goto err2; }
	if (s_timers_init(w))        { goto err3; }
	if (s_socket_init(w))        { goto err4; }
        if (s_handlers_init(w))      { goto err4; }
	if (s_childs_init(w))        { goto err5; }
	if (s_eventq_init(w))        { goto err6; }
	if (s_event_init(&w->event)) { goto err7; }

	w->running = 1;
	w->client->id = -1;
	w->client->pri = -1;
	w->client->resizeable = 1;
	w->client->alwaysontop = 0;
	w->client->cursor = MOUSE_CURSOR_ARROW;
	w->client->mouse_entered = 0;
	*window = w;

        if (s_socket_request(w, SOC_DATA_DISPLAY)) { goto err8; }

	return 0;
err8:	s_event_uninit(w->event);
err7:	s_eventq_uninit(w);
err6:	s_childs_uninit(w);
err5:	s_handlers_uninit(w);
err4:	s_timers_uninit(w);
err3:	s_pollfds_uninit(w);
err2:	s_free(w->surface);
err1:	s_free(w->client);
	s_free(w);
	debugf(DCLI | DFAT, "Cannot connect to server");
	return -1;
}

void s_client_uninit (s_window_t *window)
{
	if (window == NULL) {
		return;
	}

        window->event->type = QUIT_EVENT;
        s_event_changed(window);
	s_thread_join(window->eventq->tid, NULL);

	s_timers_uninit(window);
	s_pollfds_uninit(window);
	s_handlers_uninit(window);

	if (window->client->atexit != NULL) {
		window->client->atexit(window);
	}

	s_childs_uninit(window);

        if (window->type & (WINDOW_TEMP | WINDOW_CHILD)) {
		if (s_child_del(window->parent, window) == 0) {
			s_free(window->tid);
		}
	}

	debugf(DCLI, "[%d] Exiting (%s%s)", window->client->id, (window->type & WINDOW_MAIN) ? "WINDOW_MAIN" :
	                                                         ((window->type & WINDOW_CHILD) ? "WINDOW_CHILD" :
	                                                         ((window->type & WINDOW_TEMP) ? "WINDOW_TEMP" :
	                                                         ((window->type & WINDOW_DESKTOP) ? "WINDOW_DESKTOP" : "WINDOW_UNKNOWN"))),
	                                                        (window->type & WINDOW_NOFORM) ? " | WINDOW_NOFORM" : "");

	s_eventq_uninit(window);

	s_surface_uninit(window);
	s_event_uninit(window->event);

        s_free(window->client->device);
	s_free(window->client->title);
	s_free(window->client);
	s_free(window);
	window = NULL;
}

void s_client_exit (s_window_t *window)
{
	if (window == NULL) {
		return;
	}
	s_client_uninit(window);
}

void s_client_quit (s_window_t *window)
{
	window->running = 0;
	s_client_wakeup(window);
}

int s_client_wakeup (s_window_t *window)
{
	int w = 0;
	return s_pipe_api_write(window->wsoc, &w, sizeof(int));
}

void * s_client_loop_event (void *arg)
{
	s_event_t *event;
	s_window_t *window;

	window = (s_window_t *) arg;

	while (window->running > 0) {
		if (s_eventq_wait(window, &event) != 0) {
			continue;
		}
        	if (window->client->atevent != NULL) {
			window->client->atevent(window, event);
		}
		switch (event->type & EVENT_MASK) {
			case QUIT_EVENT:   window->running = 0;                 break;
			case MOUSE_EVENT:  s_event_parse_mouse(window, event);  break;
			case KEYBD_EVENT:  s_event_parse_keybd(window, event);  break;
			case EXPOSE_EVENT: s_event_parse_expos(window, event);  break;
			case CONFIG_EVENT: s_event_parse_config(window, event); break;
			case TIMER_EVENT:  s_event_parse_timer(window, event);  break;
			case FOCUS_EVENT:                                       break;
			case DESKTOP_EVENT:				        break;
		}
		s_event_uninit(event);
	}

	return NULL;
}

void * s_client_loop (void *arg)
{
	s_window_t *window = (s_window_t *) arg;

	window->eventq->tid = s_thread_create(&s_client_loop_event, (void *) window);

	while (window->running > 0) {
		if (s_socket_listen_wait(window, -1)) {
			break;
		}
	}

	s_client_exit(window);

	return NULL;
}

void * s_client_main (void *arg)
{
	s_window_t *window = (s_window_t *) arg;
	
	switch (window->type & ~(WINDOW_NOFORM | WINDOW_DESKTOP)) {
		case WINDOW_MAIN:
			s_client_loop(window);
			break;
		case WINDOW_TEMP:
		case WINDOW_CHILD:
			window->tid = s_thread_create(&s_client_loop, (void *) window);
			s_child_add(window->parent, window);
			break;
	}

	return NULL;
}

void s_client_atevent (s_window_t *window, void (*f) (s_window_t *, s_event_t *))
{
	window->client->atevent = f;
}

void s_client_atexit (s_window_t *window, void (*f) (s_window_t *))
{
	window->client->atexit = f;
}
