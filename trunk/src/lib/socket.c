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
	pid = (window->type & (WINDOW_TEMP | WINDOW_CHILD)) ? window->parent->id : -1;
	s_socket_send(soc, &window->type, sizeof(S_WINDOW));
	s_socket_send(soc, &pid, sizeof(int));
	s_socket_send(soc, window->surface->buf, sizeof(s_rect_t));
	s_socket_recv(soc, window->surface->buf, sizeof(s_rect_t));
	s_socket_recv(soc, window->surface->win, sizeof(s_rect_t));
	return 0;
}

int s_socket_request_display (s_window_t *window, int soc)
{
	s_soc_data_display_t *data;
	data = (s_soc_data_display_t *) s_calloc(1, sizeof(s_soc_data_display_t));

	if (s_socket_api_recv(soc, data, sizeof(s_soc_data_display_t)) != sizeof(s_soc_data_display_t)) {
		s_free(data);
		return -1;
	}

	window->surface->bytesperpixel = data->bytesperpixel;
	window->surface->bitsperpixel = data->bitsperpixel;
	window->surface->colors = data->colors;
	window->surface->blueoffset = data->blueoffset;
	window->surface->greenoffset = data->greenoffset;
	window->surface->redoffset = data->redoffset;
	window->surface->bluelength = data->bluelength;
	window->surface->greenlength = data->greenlength;
	window->surface->redlength = data->redlength;

	window->surface->linear_buf = (unsigned char *) data->linear_buf;
	window->surface->matrix = (unsigned char *) data->matrix;

	window->surface->linear_mem_size = data->linear_mem_size;
	window->surface->linear_mem_base = data->linear_mem_base;
	window->surface->linear_buf_width = data->linear_buf_width;
	window->surface->linear_buf_pitch = data->linear_buf_pitch;
	window->surface->linear_buf_height = data->linear_buf_height;
	window->surface->shm_mid = data->shm_mid;
	window->id = data->id;

	window->surface->shm_sid = data->shm_sid;
	window->surface->need_expose = data->need_expose;

	window->surface->device = strdup(data->device);

	s_free(data);
	return 0;
}

int s_socket_request_configure (s_window_t *window, int soc, S_WINDOW form)
{
	s_soc_data_configure_t *data;
	data = (s_soc_data_configure_t *) s_calloc(1, sizeof(s_soc_data_configure_t));

	data->form = (form & WINDOW_NOFORM);
	data->rnew = *(window->surface->buf);
	data->resizeable = window->resizeable;
	data->alwaysontop = window->alwaysontop;
	data->cursor = window->cursor;
	if (window->title != NULL) {
		strncpy(data->title, window->title, S_TITLE_MAX);
	}
	data->title[S_TITLE_MAX - 1] = '\0';
	
	if (s_socket_api_send(soc, data, sizeof(s_soc_data_configure_t)) != sizeof(s_soc_data_configure_t)) {
		s_free(data);
		return -1;
	}

	s_free(data);
	return 0;
}

int s_socket_request_stream (s_window_t *window, int soc, s_rect_t *coor)
{
	s_soc_data_stream_t *data;
	data = (s_soc_data_stream_t *) s_calloc(1, sizeof(s_soc_data_stream_t));
	data->bitspp = window->surface->bitsperpixel;
	data->rect.x = coor->x + window->surface->buf->x;
	data->rect.y = coor->y + window->surface->buf->y;
	data->rect.w = coor->w;
	data->rect.h = coor->h;
	if (window->surface->need_expose & SURFACE_NEEDSTREAM) {
		data->blen = (unsigned int) (window->surface->bytesperpixel * coor->w * coor->h);
		data->buf = (char *) s_malloc(data->blen + 1);
		s_getbox(window->surface, coor->x, coor->y, coor->w, coor->h, data->buf);
	} else {
		data->blen = 0;
	}
	if (s_socket_api_send(soc, data, sizeof(s_soc_data_stream_t)) != sizeof(s_soc_data_stream_t)) {
		s_free(data->buf);
		s_free(data);
		return -1;
	}
	if (data->blen != 0) {
		if (s_socket_api_send(soc, data->buf, data->blen) != data->blen) {
			s_free(data->buf);
			s_free(data);
			return -1;
		}
		s_free(data->buf);
	}
	s_free(data);
	return 0;
}

int s_socket_request_show (s_window_t *window, int soc, int id, int show)
{
	s_soc_data_show_t *data;
	data = (s_soc_data_show_t *) s_calloc(1, sizeof(s_soc_data_show_t));
	data->id = id;
	data->show = show;
	if (s_socket_api_send(soc, data, sizeof(s_soc_data_show_t)) != sizeof(s_soc_data_show_t)) {
		s_free(data);
		return -1;
	}
	s_free(data);
	return 0;
}

int s_socket_request_event (s_window_t *window, int soc, s_event_t *event)
{
	s_soc_data_event_t *data;
	data = (s_soc_data_event_t *) s_calloc(1, sizeof(s_soc_data_event_t));
	data->type = event->type;
	data->keybd.ascii = event->keybd->ascii;
	data->keybd.button = event->keybd->button;
	data->keybd.keycode = event->keybd->keycode;
	data->keybd.scancode = event->keybd->scancode;
	if (s_socket_api_send(soc, data, sizeof(s_soc_data_event_t)) != sizeof(s_soc_data_event_t)) {
		s_free(data);
		return -1;
	}
	s_free(data);
	return 0;
}

int s_socket_request (s_window_t *window, S_SOC_DATA req, ...)
{
	int id;
	int show;
	va_list ap;
	int ret = 0;
	S_WINDOW form;
	s_rect_t *coor;
	s_event_t *event;
	struct pollfd pollfd;

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
		debugf(DCLI, "[%d] Error occured when requesting (%d) from server", window->id, req);
		goto err0;
	}
	if (s_socket_api_send(pollfd.fd, &req, sizeof(req)) != sizeof(req)) {
		debugf(DCLI, "[%d] Error occured when requesting (%d) from server", window->id, req);
		goto err0;
	}

	switch (req) {
		case SOC_DATA_NEW:
			ret = s_socket_request_new(window, pollfd.fd);
			break;
		case SOC_DATA_DISPLAY:
			ret = s_socket_request_display(window, pollfd.fd);
			break;
		case SOC_DATA_CONFIGURE:
			va_start(ap, req);
			form = (S_WINDOW) va_arg(ap, S_WINDOW);
			ret = s_socket_request_configure(window, pollfd.fd, form);
			va_end(ap);
			break;
		case SOC_DATA_EXPOSE:
			va_start(ap, req);
			coor = (s_rect_t *) va_arg(ap, s_rect_t *);
			ret = s_socket_request_stream(window, pollfd.fd, coor);
			va_end(ap);
			break;
		case SOC_DATA_SHOW:
			va_start(ap, req);
			id = (int) va_arg(ap, int);
			show = (int) va_arg(ap, int);
			ret = s_socket_request_show(window, pollfd.fd, id, show);
			va_end(ap);
			break;
		case SOC_DATA_EVENT:
			va_start(ap, req);
			event = (s_event_t *) va_arg(ap, s_event_t *);
			ret = s_socket_request_event(window, pollfd.fd, event);
			va_end(ap);
			break;
		case SOC_DATA_CLOSE:
		case SOC_DATA_DESKTOP:
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
	S_EVENT dtype;
	s_soc_data_event_t *data;
	data = (s_soc_data_event_t *) s_calloc(1, sizeof(s_soc_data_event_t));

	if (s_socket_api_recv(soc, data, sizeof(s_soc_data_event_t)) != sizeof(s_soc_data_event_t)) {
		s_free(data);
		return -1;
	}
	
	dtype = data->type & (MOUSE_ENTERED | MOUSE_EXITED);
	if ((dtype == 0) ||
	    ((dtype != 0) &&
	     (dtype != window->mouse_entered))) {
		window->mouse_entered = dtype;
		window->event->type = data->type;
		*(window->event->mouse) = data->mouse;
		*(window->event->keybd) = data->keybd;
		s_event_changed(window);
	}

	s_free(data);
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
	S_EVENT change;
	s_rect_t r_old;
	s_soc_data_expose_t *data;
	data = (s_soc_data_expose_t *) s_calloc(1, sizeof(s_soc_data_expose_t));

	if (s_socket_api_recv(soc, data, sizeof(s_soc_data_expose_t)) != sizeof(s_soc_data_expose_t)) {
		s_free(data);
		return -1;
	}

	p_old = window->pri;
	r_old = *(window->surface->win);

	window->pri = data->pri;
	*(window->surface->buf) = data->buf;
	*(window->surface->win) = data->win;
	window->surface->linear_buf_width = data->linear_buf_width;
	window->surface->linear_buf_pitch = data->linear_buf_pitch;
	window->surface->linear_buf_height = data->linear_buf_height;
	
	/* configure event */
	change = 0;
	if (r_old.x != window->surface->win->x) { change |= CONFIG_CHNGX; }
	if (r_old.y != window->surface->win->y) { change |= CONFIG_CHNGY; }
	if (r_old.w != window->surface->win->w) { change |= CONFIG_CHNGW; }
	if (r_old.h != window->surface->win->h) { change |= CONFIG_CHNGH; }
	if (change != 0) {
		window->event->type = CONFIG_EVENT | change;
		window->event->expose->rect->x = window->surface->buf->x;
		window->event->expose->rect->y = window->surface->buf->y;
		window->event->expose->rect->w = window->surface->buf->w;
		window->event->expose->rect->h = window->surface->buf->h;
		s_event_changed(window);
	}
	/* focus event */
	if ((p_old != window->pri) &&
	    ((p_old == 0) || (window->pri == 0))) {
		window->event->type = FOCUS_EVENT;
		window->event->type |= (window->pri == 0) ? FOCUSIN_EVENT : FOCUSOUT_EVENT;
		window->event->expose->rect->x = window->surface->win->x;
		window->event->expose->rect->y = window->surface->win->y;
		window->event->expose->rect->w = window->surface->win->w;
		window->event->expose->rect->h = window->surface->win->h;
		s_event_changed(window);
	}
	/* expose event */
	window->event->type = EXPOSE_EVENT;
	window->event->expose->rect->x = data->changed.x;
	window->event->expose->rect->y = data->changed.y;
	window->event->expose->rect->w = data->changed.w;
	window->event->expose->rect->h = data->changed.h;
        s_event_changed(window);

        s_free(data);
	return 0;
}

int s_socket_listen_desktop (s_window_t *window, int soc)
{
	int i;
        s_event_t *event;
	s_soc_data_desktop_t *data;
        s_desktop_client_t *desktopc;

	data = (s_soc_data_desktop_t *) s_calloc(1, sizeof(s_soc_data_desktop_t));
	if (s_socket_api_recv(soc, data, sizeof(s_soc_data_desktop_t)) != sizeof(s_soc_data_desktop_t)) {
		s_free(data);
		return -1;
	}

        s_event_init(&event);
	event->type = DESKTOP_EVENT;

	for (i = 0; i < data->count; i++) {
		desktopc = (s_desktop_client_t *) s_malloc(sizeof(s_desktop_client_t));
		desktopc->id = data->client[i].id;
		desktopc->pri = data->client[i].pri;
		desktopc->title = strdup(data->client[i].title);
		s_list_add(event->desktop->clients, desktopc, -1);
	}
	s_eventq_add(window, event);
	free(data);

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
		case SOC_DATA_SHOW:
		case SOC_DATA_NOTHING:
		case SOC_DATA_DISPLAY:
		case SOC_DATA_FORMDRAW:
		case SOC_DATA_CONFIGURE:
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
				if (pfd == NULL) {
					continue;
				}
				if (polfd[i].revents & (POLLHUP | POLLERR | POLLNVAL)) {
err:					if (pfd->pf_err != NULL) {
						retval = pfd->pf_err(window, pfd);
						goto end;
					}
				} else if (polfd[i].revents == POLLIN) {
					if (pfd->pf_in != NULL) {
						if ((retval = pfd->pf_in(window, pfd)) < 0) {
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

int s_socket_uninit (s_window_t *window, s_pollfd_t *pfd)
{
	s_thread_mutex_destroy(window->socket_mutex);
	s_socket_api_close(pfd->fd);
	return 0;
}

int s_socket_in_f (s_window_t *window, s_pollfd_t *pfd)
{
	return s_socket_listen_parse(window, pfd->fd);
}

int s_socket_ierr_f (s_window_t *window, s_pollfd_t *pfd)
{
	debugf(DCLI, "[%d] Server side closed the connection", window->id);
	return -1;
}

int s_socket_inw_f (s_window_t *window, s_pollfd_t *pfd)
{
        int w;
	s_pipe_api_read(pfd->fd, &w, sizeof(int));
	return 0;
}

int s_socket_ierrw_f (s_window_t *window, s_pollfd_t *pfd)
{
	return -1;
}

int s_socket_api_closew_f (s_window_t *window, s_pollfd_t *pfd)
{
	s_pipe_api_close(pfd->fd);
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
