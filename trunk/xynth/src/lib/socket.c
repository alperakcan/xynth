/***************************************************************************
    begin                : Fri Feb 14 2003
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

#if !defined(IPC_UDS) && !defined(IPC_TCP) && !defined(IPC_PIPE)
	#error "You must select at least one of IPC_UDS, IPC_TCP, IPC_PIPE"
#endif

#define s_socket_recv(a, b, c)	if (s_socket_api_recv(a, b, c) != c) { return -1; }
#define s_socket_send(a, b, c)	if (s_socket_api_send(a, b, c) != c) { return -1; }

int s_socket_request_new (s_window_t *window, int soc)
{
	int pid;
	pid = (window->type & (WINDOW_TEMP | WINDOW_CHILD)) ? window->parent->client->id : -1;
	s_socket_send(soc, &window->type, sizeof(S_WINDOW));
	s_socket_send(soc, &pid, sizeof(int));
	s_socket_send(soc, &window->surface->buf, sizeof(s_rect_t));
	s_socket_recv(soc, &window->surface->buf, sizeof(s_rect_t));
	s_socket_recv(soc, &window->surface->win, sizeof(s_rect_t));
	return 0;
}

int s_socket_request_title (s_window_t *window, int soc)
{
	int i = strlen(window->client->title);
	s_socket_send(soc, &i, sizeof(int));
	s_socket_send(soc, window->client->title, sizeof(char) * (i + 1));
	return 0;
}

int s_socket_request_display (s_window_t *window, int soc)
{
        int len;
	unsigned int addr;

	s_socket_recv(soc, &window->surface->bytesperpixel, sizeof(int));
	s_socket_recv(soc, &window->surface->bitsperpixel, sizeof(int));
	s_socket_recv(soc, &window->surface->colors, sizeof(int));
	s_socket_recv(soc, &window->surface->blueoffset, sizeof(int));
	s_socket_recv(soc, &window->surface->greenoffset, sizeof(int));
	s_socket_recv(soc, &window->surface->redoffset, sizeof(int));
	s_socket_recv(soc, &window->surface->bluelength, sizeof(int));
	s_socket_recv(soc, &window->surface->greenlength, sizeof(int));
	s_socket_recv(soc, &window->surface->redlength, sizeof(int));

	s_socket_recv(soc, &addr, sizeof(unsigned int));
	window->surface->linear_buf = (char *) addr;
	s_socket_recv(soc, &addr, sizeof(unsigned int));
	window->surface->matrix = (unsigned char *) addr;

	s_socket_recv(soc, &window->surface->linear_mem_size, sizeof(unsigned int));
	s_socket_recv(soc, &window->surface->linear_mem_base, sizeof(unsigned int));
	s_socket_recv(soc, &window->surface->linear_buf_width, sizeof(int));
	s_socket_recv(soc, &window->surface->linear_buf_pitch, sizeof(int));
	s_socket_recv(soc, &window->surface->linear_buf_height, sizeof(int));
	s_socket_recv(soc, &window->surface->shm_mid, sizeof(int));
	s_socket_recv(soc, &window->client->id, sizeof(int));

	s_socket_recv(soc, &window->surface->shm_sid, sizeof(int));
	s_socket_recv(soc, &window->surface->need_expose, sizeof(int));

	s_socket_recv(soc, &len, sizeof(int));
	window->client->device = (char *) s_malloc(sizeof(char) * (len + 1));
	s_socket_recv(soc, window->client->device, sizeof(char) * (len + 1));
	s_socket_recv(soc, &len, sizeof(int));
	window->client->driver = (char *) s_malloc(sizeof(char) * (len + 1));
	s_socket_recv(soc, window->client->driver, sizeof(char) * (len + 1));

	return 0;
}

int s_socket_request_configure (s_window_t *window, int soc, S_WINDOW form)
{
	s_socket_send(soc, &form, sizeof(S_WINDOW));
	s_socket_send(soc, &window->surface->buf, sizeof(s_rect_t));
	s_socket_send(soc, &window->client->resizeable, sizeof(int));
	s_socket_send(soc, &window->client->alwaysontop, sizeof(int));
	return 0;
}

int s_socket_request_desktop (s_window_t *window, int soc, int id)
{
	s_socket_send(soc, &id, sizeof(int));
	return 0;
}

int s_socket_request_expose (s_window_t *window, int soc, s_rect_t *coor)
{
        coor->x += window->surface->buf.x;
        coor->y += window->surface->buf.y;
        s_socket_send(soc, coor, sizeof(s_rect_t));
	return 0;
}

int s_socket_request_stream (s_window_t *window, int soc, s_stream_t *stream)
{
        stream->rect.x += window->surface->buf.x;
        stream->rect.y += window->surface->buf.y;
	s_socket_send(soc, stream, sizeof(s_stream_t));
	s_socket_send(soc, stream->buf, stream->blen);
	return 0;
}

int s_socket_request_cursor (s_window_t *window, int soc, S_MOUSE_CURSOR cursor)
{
	s_socket_send(soc, &cursor, sizeof(S_MOUSE_CURSOR));
	return 0;
}

int s_socket_request (s_window_t *window, S_SOC_DATA req, ...)
{
	int id;
	va_list ap;
	int ret = 0;
	s_rect_t *coor;
	s_stream_t *stream;
	struct pollfd pollfd;
	S_MOUSE_CURSOR cursor;

again:	if (window->running <= 0) {
		return -1;
	}

	s_thread_mutex_lock(window->socket_mutex);

	pollfd.fd = window->soc;
	pollfd.events = POLLOUT | POLLIN;
	pollfd.revents = 0;

	if (s_socket_api_poll(&pollfd, 1, -1) <= 0) {
		goto err0;
	}
	if (pollfd.revents & (POLLERR | POLLHUP | POLLNVAL)) {
		goto err0;
	}
	if (pollfd.revents & POLLIN) {
		s_thread_mutex_unlock(window->socket_mutex);
		s_socket_listen_parse(window, pollfd.fd);
		goto again;
	}
	if (pollfd.revents != POLLOUT) {
		debugf(DCLI, "[%d] Error occured when requesting (%d) from server", window->client->id, req);
		goto err0;
	}
	if (s_socket_api_send(pollfd.fd, &req, sizeof(req)) != sizeof(req)) {
		debugf(DCLI, "[%d] Error occured when requesting (%d) from server", window->client->id, req);
		goto err0;
	}

	switch (req) {
		case SOC_DATA_NEW:
			ret = s_socket_request_new(window, pollfd.fd);
			break;
		case SOC_DATA_TITLE:
			ret = s_socket_request_title(window, pollfd.fd);
			break;
		case SOC_DATA_DISPLAY:
			ret = s_socket_request_display(window, pollfd.fd);
			break;
		case SOC_DATA_CONFIGURE:
			ret = s_socket_request_configure(window, pollfd.fd, ~WINDOW_NOFORM);
			break;
		case SOC_DATA_CONFIGURE_NOFORM:
			ret = s_socket_request_configure(window, pollfd.fd, WINDOW_NOFORM);
			break;
		case SOC_DATA_DESKTOP:
			va_start(ap, req);
			id = (int) va_arg(ap, int);
			ret = s_socket_request_desktop(window, pollfd.fd, id);
			va_end(ap);
			break;
		case SOC_DATA_EXPOSE:
			va_start(ap, req);
			coor = (s_rect_t *) va_arg(ap, s_rect_t *);
			ret = s_socket_request_expose(window, pollfd.fd, coor);
			va_end(ap);
			break;
		case SOC_DATA_STREAM:
			va_start(ap, req);
			stream = (s_stream_t *) va_arg(ap, s_stream_t *);
			ret = s_socket_request_stream(window, pollfd.fd, stream);
			va_end(ap);
			break;
		case SOC_DATA_CURSOR:
			va_start(ap, req);
			cursor = (S_MOUSE_CURSOR) va_arg(ap, S_MOUSE_CURSOR);
			ret = s_socket_request_cursor(window, pollfd.fd, cursor);
			va_end(ap);
			break;
                case SOC_DATA_HIDE:
		case SOC_DATA_SHOW:
		case SOC_DATA_EVENT:
		case SOC_DATA_CLOSE:
		case SOC_DATA_NOTHING:
		case SOC_DATA_FORMDRAW:
			break;
	}

	s_thread_mutex_unlock(window->socket_mutex);
	return ret;
err0:	s_thread_mutex_unlock(window->socket_mutex);
	return -1;
}

int s_socket_listen_event (s_window_t *window, int soc)
{
	s_socket_recv(soc, &window->event->type, sizeof(S_EVENT));
	if (window->event->type & MOUSE_EVENT) {
		s_socket_recv(soc, window->event->mouse, sizeof(s_mouse_t));
	}
	if (window->event->type & KEYBD_EVENT) {
		s_socket_recv(soc, window->event->keybd, sizeof(s_keybd_t));
	}
	s_event_changed(window);
	return 0;
}

int s_socket_listen_close (s_window_t *window, int soc)
{
        window->running = 0;
        return 0;
}

int s_socket_listen_expose (s_window_t *window, int soc)
{
        int p_old;
        s_rect_t old;
	s_rect_t changed;

	old = window->surface->win;
	p_old = window->client->pri;

	s_socket_recv(soc, &window->surface->buf, sizeof(s_rect_t));
	s_socket_recv(soc, &window->surface->win, sizeof(s_rect_t));
	s_socket_recv(soc, &window->client->pri, sizeof(int));
	s_socket_recv(soc, &changed, sizeof(s_rect_t));
	s_socket_recv(soc, &window->surface->linear_buf_width, sizeof(int));
	s_socket_recv(soc, &window->surface->linear_buf_pitch, sizeof(int));
	s_socket_recv(soc, &window->surface->linear_buf_height, sizeof(int));

	window->event->type = EXPOSE_EVENT;
	window->event->expose->change= 0;
	window->event->expose->rect = changed;

        if (old.x != window->surface->win.x) { window->event->expose->change |= EXPOSE_CHNGX; }
        if (old.y != window->surface->win.y) { window->event->expose->change |= EXPOSE_CHNGY; }
        if (old.w != window->surface->win.w) { window->event->expose->change |= EXPOSE_CHNGW; }
        if (old.h != window->surface->win.h) { window->event->expose->change |= EXPOSE_CHNGH; }
        if ((p_old != window->client->pri) &&
            ((p_old == 0) || (window->client->pri == 0))) {
		window->event->expose->change |= EXPOSE_CHNGF;
	}

        s_event_changed(window);

	return 0;
}

int s_socket_listen_desktop (s_window_t *window, int soc)
{
	int count;
        s_event_t *event;
        s_desktop_client_t *desktopc;

        s_event_init(&event);
	event->type = DESKTOP_EVENT;

        s_socket_recv(soc, &count, sizeof(int));
	while (count--) {
		desktopc = (s_desktop_client_t *) s_malloc(sizeof(s_desktop_client_t));
		s_socket_recv(soc, &desktopc->id, sizeof(int));
		s_socket_recv(soc, &desktopc->pri, sizeof(int));
		s_socket_recv(soc, &desktopc->title_l, sizeof(int));
		desktopc->title = (char *) s_malloc((desktopc->title_l + 1) * sizeof(char));
		s_socket_recv(soc, desktopc->title, desktopc->title_l);
		desktopc->title[desktopc->title_l] = '\0';
		s_list_add(event->desktop->clients, desktopc, -1);
	}
	s_eventq_add(window, event);

	return 0;
}

int s_socket_listen_parse (s_window_t *window, int soc)
{
        int ret = 0;
	struct pollfd pollfd;
	S_SOC_DATA req = SOC_DATA_NOTHING;

        s_thread_mutex_lock(window->socket_mutex);

	pollfd.fd = soc;
	pollfd.events = POLLIN;
	pollfd.revents = 0;

	if ((s_socket_api_poll(&pollfd, 1, 0) <= 0) ||
	    (pollfd.revents != POLLIN)) {
	        s_thread_mutex_unlock(window->socket_mutex);
	        return 0;
	}
	if (s_socket_api_recv(soc, &req, sizeof(req)) <= 0) {
		goto err0;
	}

	switch (req) {
		case SOC_DATA_EVENT:
			ret = s_socket_listen_event(window, soc);
			break;
		case SOC_DATA_CLOSE:
			ret = s_socket_listen_close(window, soc);
			break;
		case SOC_DATA_EXPOSE:
			ret = s_socket_listen_expose(window, soc);
			break;
		case SOC_DATA_DESKTOP:
			ret = s_socket_listen_desktop(window, soc);
			break;
		case SOC_DATA_NEW:
                case SOC_DATA_HIDE:
		case SOC_DATA_SHOW:
		case SOC_DATA_TITLE:
		case SOC_DATA_CURSOR:
		case SOC_DATA_STREAM:
		case SOC_DATA_NOTHING:
		case SOC_DATA_DISPLAY:
		case SOC_DATA_FORMDRAW:
		case SOC_DATA_CONFIGURE:
		case SOC_DATA_CONFIGURE_NOFORM:
			break;
	}

        s_thread_mutex_unlock(window->socket_mutex);
	return ret;
err0:	s_thread_mutex_unlock(window->socket_mutex);
	return -1;
}

int s_socket_listen_wait (s_window_t *window, int timeout)
{
        int i;
        int len;
	int ret;
	int retval;
	long long tv[2];
	s_timer_t *tmr;
	s_pollfd_t *pfd;
	struct pollfd *polfd;

	if (window->running <= 0) {
		return -1;
	}

	s_thread_mutex_lock(window->pollfds->mut);
	len = window->pollfds->list->nb_elt;
        polfd = (struct pollfd *) s_malloc(len * sizeof(struct pollfd));
        for (i = 0; i < len; i++) {
		pfd = (s_pollfd_t *) s_list_get(window->pollfds->list, i);
		polfd[i].fd = pfd->fd;
		polfd[i].events = POLLIN;
		polfd[i].revents = 0;
	}
	s_thread_mutex_unlock(window->pollfds->mut);

	timeout = -1;
	i = 0;
	s_thread_mutex_lock(window->timers->mut);
	while (!s_list_eol(window->timers->timers, i)) {
		tmr = (s_timer_t *) s_list_get(window->timers->timers, i);
		if (tmr->timeval > 0) {
			if (timeout <= 0) {
				timeout = tmr->timeval;
			}
			if (tmr->timeval < timeout) {
				timeout = tmr->timeval;
			}
			if (tmr->interval <= 0) {
				tmr->interval = tmr->timeval;
			}
		} else {
			s_list_remove(window->timers->timers, i);
			continue;
		}
		i++;
	}
	s_thread_mutex_unlock(window->timers->mut);

	tv[0] = s_gettimeofday();
	ret = s_socket_api_poll(polfd, len, timeout);
	tv[1] = s_gettimeofday();

	retval = 0;
	switch (ret) {
		case 0:
			goto end;
		case -1:
			retval = -1;
			goto end;
		default :
			for (i = 0; i < len; i++) {
				pfd = s_pollfd_find(window, polfd[i].fd);
				if (polfd[i].revents & (POLLHUP | POLLERR | POLLNVAL)) {
err:					if (pfd->pf_err != NULL) {
						retval = pfd->pf_err(window, polfd[i].fd);
						goto end;
					}
				} else if (polfd[i].revents == POLLIN) {
					if (pfd->pf_in != NULL) {
						if ((retval = pfd->pf_in(window, polfd[i].fd)) < 0) {
							debugf(DSER, "pfd->in(window, polfd[%d].fd) < 0) Calling pfd->ierr if exists", i);
							goto err;
						}
						goto end;
					}
				}
			}
	}

end:	i = 0;
	s_thread_mutex_lock(window->timers->mut);
	while (!s_list_eol(window->timers->timers, i)) {
		tmr = (s_timer_t *) s_list_get(window->timers->timers, i);
		tmr->interval -= (tv[1] - tv[0]);
		i++;
	}
	i = 0;
	while (!s_list_eol(window->timers->timers, i)) {
		tmr = (s_timer_t *) s_list_get(window->timers->timers, i);
		if (tmr->interval <= 0) {
			s_event_t *e;
			s_event_init(&e);
			e->type = TIMER_EVENT;
			e->timer = tmr;
			s_eventq_add(window, e);
		}
		i++;
	}
	s_thread_mutex_unlock(window->timers->mut);

	s_free(polfd);
	return retval;
}

int s_socket_uninit (s_window_t *window, int s)
{
	s_thread_mutex_destroy(window->socket_mutex);
	s_socket_api_close(s);
	return 0;
}

int s_socket_in_f (s_window_t *window, int s)
{
	return s_socket_listen_parse(window, s);
}

int s_socket_ierr_f (s_window_t *window, int s)
{
	debugf(DCLI, "[%d] Server side closed the connection", window->client->id);
	return -1;
}

int s_socket_inw_f (s_window_t *window, int s)
{
        int w;
	s_pipe_api_read(s, &w, sizeof(int));
	return 0;
}

int s_socket_ierrw_f (s_window_t *window, int s)
{
	return -1;
}

int s_socket_api_closew_f (s_window_t *window, int s)
{
	s_pipe_api_close(s);
	s_pipe_api_close(window->wsoc);
	return 0;
}

int s_socket_init_uds (s_window_t *window)
{
#if defined(IPC_UDS)
	int fd;
        struct sockaddr_un server = {AF_UNIX, S_SERVER_SOC_NAME};

	fd = s_socket_api_socket(AF_UNIX, SOCK_STREAM, 0);
        if (fd < 0) {
		goto err0;
	}
	if (s_socket_api_connect(fd, (s_sockaddr_t *) &server, sizeof(server)) == -1) {
		goto err1;
	}
	debugf(DCLI, "Connected to server via UDS (%s)", S_SERVER_SOC_NAME);
	return fd;

err1:	s_socket_api_close(fd);
err0:	return -1;
#else
	return -1;
#endif
}

int s_socket_init_tcp (s_window_t *window)
{
#if defined(IPC_TCP)
        int fd;
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(S_SERVER_SOC_ADDR);
	server.sin_port = htons(S_SERVER_SOC_PORT);

        fd = s_socket_api_socket(AF_INET, SOCK_STREAM, 0);
        if (fd < 0) {
		goto err0;
	}
	if (s_socket_api_connect(fd, (s_sockaddr_t *) &server, sizeof(server)) == -1) {
		goto err1;
	}
	debugf(DCLI, "Connected to server via TCP (%s:%d)", S_SERVER_SOC_ADDR, S_SERVER_SOC_PORT);
	return fd;
err1:	s_socket_api_close(fd);
err0:	return -1;
#else
	return -1;
#endif
}

int s_socket_init_pipe (s_window_t *window)
{
#if defined(IPC_PIPE)
	int fd;
        s_sockaddr_t server = {AF_PIPE, S_SERVER_SOC_PIPE};

	fd = s_socket_api_socket(AF_PIPE, SOCK_STREAM, 0);
        if (fd < 0) {
		goto err0;
	}
	if (s_socket_api_connect(fd, (s_sockaddr_t *) &server, sizeof(server)) == -1) {
		goto err1;
	}
	debugf(DCLI, "Connected to server via PIPE (%s)", S_SERVER_SOC_PIPE);
	return fd;
err1:	s_socket_api_close(fd);
err0:	return -1;
#else
	return -1;
#endif
}

int s_socket_init_wakeup (s_window_t *window)
{
	int fd[2];
	s_pollfd_t *pfd;

	if (s_pollfd_init(&pfd)) {
		goto err0;
	}
	if (s_pipe_api_pipe(fd)) {
		goto err1;
	}

	window->wsoc = fd[1];
	pfd->fd = fd[0];
	pfd->pf_in = s_socket_inw_f;
	pfd->pf_err = s_socket_ierrw_f;
	pfd->pf_close = s_socket_api_closew_f;
	if (!s_pollfd_add(window, pfd)) {
		goto err2;
	}
	
	return 0;
err2:	s_pipe_api_close(fd[0]);
	s_pipe_api_close(fd[1]);
err1:	s_pollfd_uninit(pfd);
err0:	return -1;
}

int s_socket_init (s_window_t *window)
{
	int fd;
	s_pollfd_t *pfd;

	if (((fd = s_socket_init_uds(window)) < 0) &&
	    ((fd = s_socket_init_tcp(window)) < 0) &&
	    ((fd = s_socket_init_pipe(window)) < 0)) {
		goto err0;
	}
	if (s_socket_init_wakeup(window)) {
		goto err0;
	}
	if (s_pollfd_init(&pfd)) {
		goto err0;
	}

	window->soc = fd;
        pfd->fd = fd;
        pfd->pf_in = s_socket_in_f;
        pfd->pf_err = s_socket_ierr_f;
        pfd->pf_close = s_socket_uninit;

	if (!s_pollfd_add(window, pfd)) {
		goto err1;
	}

	if (s_thread_mutex_init(&(window->socket_mutex))) {
		goto err0;
	}

	return 0;

err1:	s_pollfd_uninit(pfd);
err0:	return -1;
}	
