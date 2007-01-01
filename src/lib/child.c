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

int s_child_add (s_window_t *window, s_window_t *child)
{
	int ret;
	s_childs_t *chl = window->childs;

	s_thread_mutex_lock(chl->mut);
	ret = s_list_add(chl->list, child, 0);
	s_thread_mutex_unlock(chl->mut);

	return ret;
}

int s_child_del (s_window_t *window, s_window_t *child)
{
        int p = 0;
        int ret = 1;
	s_window_t *w;
	s_childs_t *chl = window->childs;

	s_thread_mutex_lock(chl->mut);

	while (!s_list_eol(chl->list, p)) {
		w = (s_window_t *) s_list_get(chl->list, p);
		if (w->tid == child->tid) {
			s_list_remove(chl->list, p);
			ret = 0;
			break;
		}
		p++;
	}

	s_thread_mutex_unlock(chl->mut);

	return ret;
}

int s_childs_init (s_window_t *window)
{
        window->childs = (s_childs_t *) s_calloc(1, sizeof(s_childs_t));
        if (s_list_init(&(window->childs->list))) {
		goto err0;
	}
	if (s_thread_mutex_init(&(window->childs->mut))) {
		goto err1;
	}
	return 0;
err1:	s_list_uninit(window->childs->list);
err0:	s_free(window->childs);
	return -1;
}

int s_childs_uninit (s_window_t *window)
{
        s_thread_t *t;
	s_window_t *w;
	s_childs_t *chl = window->childs;

	while (1) {
		s_thread_mutex_lock(chl->mut);
		if (s_list_eol(chl->list, 0)) {
			s_thread_mutex_unlock(chl->mut);
			break;
		}
		w = (s_window_t *) s_list_get(chl->list, 0);
		s_list_remove(chl->list, 0);
		t = w->tid;
		s_window_quit(w);
		s_thread_mutex_unlock(chl->mut);
		s_thread_join(t, NULL);
	}

	s_thread_mutex_destroy(chl->mut);
	s_list_uninit(chl->list);
	s_free(chl);

	return 0;
}
