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

#include "xynth_.h"

int s_timer_init (s_timer_t **timer)
{
	*timer = (s_timer_t *) s_malloc(sizeof(s_timer_t));
	(*timer)->timeval = -1;
	(*timer)->interval = -1;
	(*timer)->cb = NULL;
	(*timer)->data = NULL;
	return 0;
}

int s_timer_timeval (s_window_t *window, s_timer_t *timer, int timeval)
{
	s_thread_mutex_lock(window->timers->mut);
	timer->timeval = timeval;
	timer->interval = timeval;
	s_thread_mutex_unlock(window->timers->mut);
	s_window_wakeup(window);
	return 0;
}

int s_timer_uninit (s_timer_t *timer)
{
	s_free(timer);
	return 0;
}

int s_timer_del (s_window_t *window, s_timer_t *timer)
{
	int r;
	int pos;
	s_event_t *evn;
	
	if (timer == NULL) {
		return -1;
	}
	
	s_thread_mutex_lock(window->eventq->mut);
	pos = 0;
	while (!s_list_eol(window->eventq->queue, pos)) {
		evn = (s_event_t *) s_list_get(window->eventq->queue, pos);
		if ((evn->type == TIMER_EVENT) &&
		    (evn->timer == timer)) {
			s_list_remove(window->eventq->queue, pos);
			s_event_uninit(evn);
		} else {
			pos++;
		}
	}
	s_thread_mutex_unlock(window->eventq->mut);
	
	s_thread_mutex_lock(window->timers->mut);
	r = s_list_remove(window->timers->timers, s_list_get_pos(window->timers->timers, timer));
	s_thread_mutex_unlock(window->timers->mut);
	s_window_wakeup(window);

	return r;
}

int s_timer_add (s_window_t *window, s_timer_t *timer)
{
	int ret = 0;

	if ((timer->timeval < 0) ||
	    (timer->cb == NULL)) {
		goto end;
	}
	timer->interval = timer->timeval;
	
	s_thread_mutex_lock(window->timers->mut);
	if (s_list_get_pos(window->timers->timers, timer) < 0) {
		ret = s_list_add(window->timers->timers, timer, -1);
	}
	s_thread_mutex_unlock(window->timers->mut);
	s_window_wakeup(window);

end:	return ret;
}

int s_timers_init (s_window_t *window)
{
	window->timers = (s_timers_t *) s_malloc(sizeof(s_timers_t));
	if (s_list_init(&(window->timers->timers))) {
		goto err0;
	}
	if (s_thread_mutex_init(&(window->timers->mut))) {
		goto err1;
	}
	return 0;
err1:	s_list_uninit(window->timers->timers);
err0:	s_free(window->timers);
	return -1;
}

int s_timers_uninit (s_window_t *window)
{
	s_timer_t *t;

	s_thread_mutex_lock(window->timers->mut);
	while (!s_list_eol(window->timers->timers, 0)) {
		t = (s_timer_t *) s_list_get(window->timers->timers, 0);
		s_list_remove(window->timers->timers, 0);
		s_timer_uninit(t);
		
	}
	s_thread_mutex_unlock(window->timers->mut);
	s_thread_mutex_destroy(window->timers->mut);
	s_list_uninit(window->timers->timers);
	s_free(window->timers);

	return 0;
}
