/***************************************************************************
    begin                : Thu Jun 24 2004
    copyright            : (C) 2004 - 2007 by Alper Akcan
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

int s_eventq_init (s_window_t *window)
{
	window->eventq = (s_eventq_t *) s_calloc(1, sizeof(s_eventq_t));
	window->eventq->queue = (s_list_t *) s_calloc(1, sizeof(s_list_t));
	if (s_thread_cond_init(&window->eventq->cond)) {
		goto err0;
	}
	if (s_thread_mutex_init(&window->eventq->mut)) {
		goto err1;
	}
        return 0;

err1:	s_thread_cond_destroy(window->eventq->cond);
err0:	s_free(window->eventq->queue);
	s_free(window->eventq);
	return 1;
}

int s_eventq_uninit (s_window_t *window)
{
        s_event_t *e;

	s_thread_mutex_destroy(window->eventq->mut);
	s_thread_cond_destroy(window->eventq->cond);

	while (!s_list_eol(window->eventq->queue, 0)) {
		e = (s_event_t *) s_list_get(window->eventq->queue, 0);
		s_list_remove(window->eventq->queue, 0);
		s_event_uninit(e);
	}
	s_free(window->eventq->queue);

        s_free(window->eventq);

	return 0;
}

int s_eventq_add (s_window_t *window, s_event_t *event)
{
        int ret;
	s_thread_mutex_lock(window->eventq->mut);
	event->window = window;
	ret = s_list_add(window->eventq->queue, event, -1);
	s_thread_cond_signal(window->eventq->cond);
	s_thread_mutex_unlock(window->eventq->mut);
	return ret;
}

int s_eventq_get (s_window_t *window, s_event_t **event)
{
        int ret;
        s_event_t *e;

	s_thread_mutex_lock(window->eventq->mut);

	e = (s_event_t *) s_list_get(window->eventq->queue, 0);
	if (e == NULL) {
		ret = 1;
	} else {
		s_list_remove(window->eventq->queue, 0);
		*event = e;
		ret = 0;
	}

	s_thread_mutex_unlock(window->eventq->mut);

	return ret;
}

int s_eventq_wait (s_window_t *window, s_event_t **event)
{
        int ret;
        s_event_t *e;

	s_thread_mutex_lock(window->eventq->mut);

	while (window->eventq->queue->nb_elt <= 0) {
		if (s_thread_cond_wait(window->eventq->cond, window->eventq->mut)) {
			debugf(DSYS, "s_thread_cond_wait failed");
			return -1;
		}
	}

	e = (s_event_t *) s_list_get(window->eventq->queue, 0);
	if (e == NULL) {
		ret = -1;
	} else {
		s_list_remove(window->eventq->queue, 0);
		*event = e;
		ret = 0;
	}

	s_thread_mutex_unlock(window->eventq->mut);

	return ret;
}
