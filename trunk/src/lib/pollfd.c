/***************************************************************************
    begin                : Tue May 25 2004
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

int s_pollfd_init (s_pollfd_t **pfd)
{
	(*pfd) = (s_pollfd_t *) s_calloc(1, sizeof(s_pollfd_t));
        (*pfd)->fd = -1;
	return 0;
}

int s_pollfd_uninit (s_pollfd_t *pfd)
{
	s_free(pfd);
	return 0;
}

s_pollfd_t * s_pollfd_find (s_window_t *window, int fd)
{
	int pos;
	s_pollfd_t *pfd;
	s_pollfd_t *ret;
	s_thread_mutex_lock(window->pollfds->mut);
	pos = 0;
	ret = NULL;
	while (!s_list_eol(window->pollfds->list, pos)) {
		pfd = (s_pollfd_t *) s_list_get(window->pollfds->list, pos);
		if (pfd->fd == fd) {
			ret = pfd;
			break;
		}
		pos++;
	}
	s_thread_mutex_unlock(window->pollfds->mut);
	return ret;
}

int s_pollfd_add (s_window_t *window, s_pollfd_t *pfd)
{
	int ret = 0;
	s_thread_mutex_lock(window->pollfds->mut);
	if (s_list_get_pos(window->pollfds->list, pfd) < 0) {
#if 0
		ret = s_list_add(window->pollfds->list, pfd, -1);
#else
		ret = s_list_add(window->pollfds->list, pfd, 2);
#endif
	}
	s_thread_mutex_unlock(window->pollfds->mut);
	s_window_wakeup(window);
	return ret;
}

int s_pollfd_del (s_window_t *window, s_pollfd_t *pfd)
{
	int ret;
	s_thread_mutex_lock(window->pollfds->mut);
	ret = s_list_remove(window->pollfds->list, s_list_get_pos(window->pollfds->list, pfd));
	s_thread_mutex_unlock(window->pollfds->mut);
	s_window_wakeup(window);
	return ret;
}

int s_pollfds_init (s_window_t *window)
{
	window->pollfds = (s_pollfds_t *) s_calloc(1, sizeof(s_pollfds_t));
	if (s_list_init(&(window->pollfds->list))) {
		goto err0;
	}
	if (s_thread_mutex_init(&(window->pollfds->mut))) {
		goto err1;
	}
	return 0;
err1:	s_list_uninit(window->pollfds->list);
err0:	s_free(window->pollfds);
	return -1;
}

int s_pollfds_uninit (s_window_t *window)
{
	s_pollfd_t *pfd;
	
	s_thread_mutex_lock(window->pollfds->mut);
	while (!s_list_eol(window->pollfds->list, 0)) {
		pfd = (s_pollfd_t *) s_list_get(window->pollfds->list, 0);
		s_list_remove(window->pollfds->list, 0);
		if (pfd->pf_close != NULL) {
			pfd->pf_close(window, pfd);
		}
		s_pollfd_uninit(pfd);
	}
	s_thread_mutex_unlock(window->pollfds->mut);
	s_thread_mutex_destroy(window->pollfds->mut);
	s_list_uninit(window->pollfds->list);
	s_free(window->pollfds);

	return 0;
}
