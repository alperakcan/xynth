/***************************************************************************
    begin                : Mon Sep 22 2003
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

#include "xynth_.h"

int s_handler_init (s_handler_t **handler)
{
	(*handler) = (s_handler_t *) s_calloc(1, sizeof(s_handler_t));
	return 0;
}

int s_handler_uninit (s_handler_t *handler)
{
	s_free(handler);
	return 0;
}

int s_handler_add (s_window_t *window, s_handler_t *handler)
{
	int ret = 0;
	s_thread_mutex_lock(window->handlers->mut);
	if (s_list_get_pos(window->handlers->list, handler) < 0) {
		ret = s_list_add(window->handlers->list, handler, -1);
	}
	s_thread_mutex_unlock(window->handlers->mut);
	return ret;
}

int s_handler_del (s_window_t *window, s_handler_t *handler)
{
	int ret = 0;
	s_thread_mutex_lock(window->handlers->mut);
	ret = s_list_remove(window->handlers->list, s_list_get_pos(window->handlers->list, handler));
	s_thread_mutex_unlock(window->handlers->mut);
	return ret;
}

int s_handlers_init (s_window_t *window)
{
	window->handlers = (s_handlers_t *) s_malloc(sizeof(s_handlers_t));
	if (s_list_init(&(window->handlers->list))) {
		goto err0;
	}
	if (s_thread_mutex_init(&(window->handlers->mut))) {
		goto err1;
	}
	return 0;
err1:	s_list_uninit(window->handlers->list);
err0:	s_free(window->handlers);
	return 1;
}

int s_handlers_uninit (s_window_t *window)
{
	s_handler_t *hndl;

	s_thread_mutex_lock(window->handlers->mut);
	while (!s_list_eol(window->handlers->list, 0)) {
		hndl = (s_handler_t *) s_list_get(window->handlers->list, 0);
		s_list_remove(window->handlers->list, 0);
		s_handler_uninit(hndl);
	}
	s_thread_mutex_unlock(window->handlers->mut);
	s_thread_mutex_destroy(window->handlers->mut);
	s_list_uninit(window->handlers->list);
	s_free(window->handlers);

	return 0;
}
