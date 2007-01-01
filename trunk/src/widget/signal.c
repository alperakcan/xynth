/***************************************************************************
    begin                : Thu Mar 10 2005
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

#include "../lib/xynth_.h"
#include "widget.h"

void w_signal_send (w_object_t *from, w_object_t *to, void (*func) (w_signal_t *), void *arg)
{
	s_event_t *event;
	w_signal_t *signal;
	s_event_init(&event);
	event->type = SIGNAL_EVENT;
	signal = (w_signal_t *) s_malloc(sizeof(w_signal_t));
	signal->from = from;
	signal->to  = to;
	signal->func = func;
	signal->arg = arg;
	event->data = (void *) signal;
	s_eventq_add(to->window->window, event);
}

void w_signal_delete (w_object_t *object)
{
	int pos = 0;
	s_event_t *event;
	w_signal_t *signal;
        s_thread_mutex_lock(object->window->window->eventq->mut);
	while (!s_list_eol(object->window->window->eventq->queue, pos)) {
		event = (s_event_t *) s_list_get(object->window->window->eventq->queue, pos);
		signal = (w_signal_t *) event->data;
		if ((event->type == SIGNAL_EVENT) &&
		    ((signal->from == object) || (signal->to == object))) {
			s_list_remove(object->window->window->eventq->queue, pos);
			s_free(signal);
			s_event_uninit(event);
		} else {
			pos++;
		}
	}
	s_thread_mutex_unlock(object->window->window->eventq->mut);
}
