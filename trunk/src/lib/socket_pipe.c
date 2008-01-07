/***************************************************************************
    begin                : Mon Aug 15 2005
    copyright            : (C) 2005 - 2008 by Alper Akcan
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

/* socket api emulation using pipe()
 */

#define MIN_SOCK_E	8192

typedef struct s_soce_s {
	int soc;
	int rsoc;
	int wsoc;
	char *bname;
	int connected;
	struct s_soce_s *conn;
} s_soce_t;

static struct {
	unsigned int ids;
	s_list_t *list;
	s_thread_mutex_t *lock;
} s_soce;

static int s_socket_pipe_find (int s, s_soce_t **soc)
{
	int pos = 0;
	s_soce_t *tmp;
	while (!s_list_eol(s_soce.list, pos)) {
		tmp = (s_soce_t *) s_list_get(s_soce.list, pos);
		if (s == tmp->soc) {
			*soc = tmp;
			return 0;
		}
		pos++;
	}
	return -1;
}

static int s_socket_pipe_poll (struct pollfd *ufds, nfds_t nfds, int timeout)
{
	int i;
	int ret;
	int tmpn;
	s_soce_t *soc;
	struct pollfd *tmp;
	struct pollfd **tbl;

	DEBUGF(0, "enter");

	s_thread_mutex_lock(s_soce.lock);
        tmpn = 0;
	tmp = (struct pollfd *) s_malloc(sizeof(struct pollfd) * nfds * 2);
	tbl = (struct pollfd **) s_malloc(sizeof(struct pollfd *) * nfds * 2);
        for (i = 0; i < nfds * 2; i++) {
		tmp[i].fd = -1;
		tmp[i].events = 0;
		tmp[i].revents = 0;
		tbl[i] = NULL;
	}
        for (i = 0; i < nfds; i++) {
		ufds[i].revents = 0;
	}
        for (i = 0; i < nfds; i++) {
		if (s_socket_pipe_find(ufds[i].fd, &soc)) {
			tmp[tmpn].fd = ufds[i].fd;
			tmp[tmpn].events = ufds[i].events;
			tbl[tmpn] = &ufds[i];
			tmpn++;
		} else {
			if (ufds[i].events & POLLIN) {
#if 0
				if (soc->connected && soc->conn == NULL) {
					/* connected end is closed
					 */
					ufds[i].revents = POLLHUP;
					s_thread_mutex_unlock(s_soce.lock);
					s_free(tmp);
					s_free(tbl);
					return 1;
				} else {
					tmp[tmpn].fd = soc->rsoc;
					tmp[tmpn].events |= POLLIN;
					tbl[tmpn] = &ufds[i];
					tmpn++;
				}
#else
					tmp[tmpn].fd = soc->rsoc;
					tmp[tmpn].events |= POLLIN;
					tbl[tmpn] = &ufds[i];
					tmpn++;
#endif
			}
			if (ufds[i].events & POLLOUT) {
				if (soc->connected && soc->conn != NULL) {
					tmp[tmpn].fd = soc->conn->wsoc;
					tmp[tmpn].events |= POLLOUT;
					tbl[tmpn] = &ufds[i];
					tmpn++;
				} else {
					/* connected end is closed
					 */
					ufds[i].revents = POLLHUP;
					s_thread_mutex_unlock(s_soce.lock);
					s_free(tmp);
					s_free(tbl);
					return 1;
				}
			}
		}
	}
	s_thread_mutex_unlock(s_soce.lock);

	ret = s_pipe_api_poll(tmp, tmpn, timeout);

	s_thread_mutex_lock(s_soce.lock);
        for (i = 0; i < tmpn; i++) {
		tbl[i]->revents |= tmp[i].revents;
	}
	s_free(tmp);
	s_free(tbl);
        s_thread_mutex_unlock(s_soce.lock);

	DEBUGF(0, "leave (ret = %d)", ret);
	return ret;
}

static int s_socket_pipe_recv (int s, void *read_buf, int total_size)
{
	int r;
	int sr;
	int r_size = 0;
	s_soce_t *soc;
	DEBUGF(0, "enter");
	s_thread_mutex_lock(s_soce.lock);
	if (s_socket_pipe_find(s, &soc)) {
		goto err0;
	}
	sr = soc->rsoc;
	s_thread_mutex_unlock(s_soce.lock);
	while (total_size != r_size) {
		r = s_pipe_api_read(sr, (void *) (((char *) read_buf) + r_size), total_size - r_size);
		if (r <= 0) {
			s_socket_api_close(sr);
			return -1;
		}
		r_size += r;
	}
	DEBUGF(0, "leave");
	return r_size;
err0:	s_thread_mutex_unlock(s_soce.lock);
	DEBUGF(0, "leave error");
	return -1;
}

static int s_socket_pipe_send (int s, void *write_buf, int total_size)
{
	int w;
	int sw;
	int w_size = 0;
	s_soce_t *soc;
	DEBUGF(0, "enter");
	s_thread_mutex_lock(s_soce.lock);
	if (s_socket_pipe_find(s, &soc)) {
		goto err0;
	}
	sw = soc->conn->wsoc;
	s_thread_mutex_unlock(s_soce.lock);
	while (total_size != w_size) {
		w = s_pipe_api_write(sw, (void *) (((char *) write_buf) + w_size), total_size - w_size);
		if (w <= 0) {
			s_socket_api_close(sw);
			return -1;
		}
		w_size += w;
	}
	DEBUGF(0, "leave");
	return w_size;
err0:	s_thread_mutex_unlock(s_soce.lock);
	DEBUGF(0, "leave error");
	return -1;
}

static int s_socket_pipe_socket (int domain, int type, int protocol)
{
	int i;
	int pfd[2];
	s_soce_t *soc;
	s_soce_t *tmp;
	DEBUGF(0, "enter");
	s_thread_mutex_lock(s_soce.lock);
	soc = (s_soce_t *) s_malloc(sizeof(s_soce_t));
	if (s_pipe_api_pipe(pfd)) {
		goto err1;
	}
	for (i = s_soce.ids; i >= MIN_SOCK_E; i++) {
		if (s_socket_pipe_find(i, &tmp)) {
			goto found;
		}
	}
	goto err1;

found:	soc->soc = i;
	soc->rsoc = pfd[0];
	soc->wsoc = pfd[1];
	soc->bname = NULL;
	soc->conn = NULL;
	soc->connected = 0;

	/* Posibble Error :
	   if this function returns a newly closed id as new id (can happen,
	   and is ok) before it is processed as closed, this is a REAL BUG.
	   if there is any previous poll call waiting on that newly closed id
	   which will have to recieve POLLNVAL, or POLLHUP, but didnot recieve
	   yet, this newly created id will be recognized as old-but-closed id
	   altough it is new-created-id. the previous poll call will recieve
	   POLLIN, or POLLOUT, causing the server, or client to hangup/segfault.

	   here i used an ugly counting id bank to avoid this bug. hint is
	   unsigned int i++ will become "0" after sometime. pray for that "0"
	   is not newly closed ;)
	 */

	if ((i + 1) < MIN_SOCK_E) {
		s_soce.ids = MIN_SOCK_E;
	} else {
		s_soce.ids = i;
	}

	s_list_add(s_soce.list, soc, -1);
	s_thread_mutex_unlock(s_soce.lock);
	DEBUGF(0, "leave (id:%d)", i);
	return soc->soc;
err1:	s_free(soc);
	s_thread_mutex_unlock(s_soce.lock);
	DEBUGF(0, "leave error");
	return -1;
}

static int s_socket_pipe_bind (int sockfd, const s_sockaddr_t *my_addr, socklen_t addrlen)
{
	s_soce_t *soc;
	DEBUGF(0, "enter");
	s_thread_mutex_lock(s_soce.lock);
	if (s_socket_pipe_find(sockfd, &soc)) {
		goto err0;
	}
	soc->bname = strdup((char *) (my_addr->sa_addr));
	s_thread_mutex_unlock(s_soce.lock);
	DEBUGF(0, "leave");
	return 0;
err0:	s_thread_mutex_unlock(s_soce.lock);
	DEBUGF(0, "leave error");
	return -1;
}

static int s_socket_pipe_listen (int s, int backlog)
{
	DEBUGF(0, "enter");
	DEBUGF(0, "leave");
	return 0;
}

static int s_socket_pipe_accept (int s, s_sockaddr_t *addr, socklen_t *addrlen)
{
	int t;
	int cs;
	int ok = 1;
	s_soce_t *server;
	s_soce_t *client;
	s_soce_t *clconn;
	s_thread_mutex_lock(s_soce.lock);
	DEBUGF(0, "enter");
	if (s_socket_pipe_find(s, &server)) {
		goto err0;
	}
	if (s_pipe_api_read(server->rsoc, &cs, sizeof(int)) <= 0) {
		goto err0;
	}
	if (s_socket_pipe_find(cs, &client)) {
		goto err0;
	}
	s_thread_mutex_unlock(s_soce.lock);
	t = s_socket_api_socket(0, 0, 0);
	s_thread_mutex_lock(s_soce.lock);
	if (s_socket_pipe_find(t, &clconn)) {
		goto err0;
	}
	client->conn = clconn;
	client->connected = 1;
	clconn->conn = client;
	clconn->connected = 1;
	if (s_pipe_api_write(client->wsoc, &ok, sizeof(int)) <= 0) {
		s_pipe_api_close(t);
		goto err0;
	}
	s_thread_mutex_unlock(s_soce.lock);
	DEBUGF(0, "leave");
	return clconn->soc;
err0:	s_thread_mutex_unlock(s_soce.lock);
	DEBUGF(0, "leave error");
	return -1;
}

static int s_socket_pipe_connect (int sockfd, const s_sockaddr_t *serv_addr, socklen_t addrlen)
{
	int ok;
	int pos = 0;
	s_soce_t *client;
	s_soce_t *server;
	s_thread_mutex_lock(s_soce.lock);
	DEBUGF(0, "enter");
	if (s_socket_pipe_find(sockfd, &client)) {
		goto err1;
	}
	while (!s_list_eol(s_soce.list, pos)) {
		server = (s_soce_t *) s_list_get(s_soce.list, pos);
		if ((server->bname != NULL) &&
		    (strcmp(server->bname, serv_addr->sa_addr) == 0)) {
			goto found;
		}
	}
	goto err1;
found:	s_thread_mutex_unlock(s_soce.lock);
	if (s_pipe_api_write(server->wsoc, &sockfd, sizeof(int)) <= 0) {
		goto err0;
	}
	if (s_pipe_api_read(client->rsoc, &ok, sizeof(int)) <= 0) {
		goto err0;
	}
	DEBUGF(0, "leave");
	return 0;
err1:	s_thread_mutex_unlock(s_soce.lock);
err0:	DEBUGF(0, "leave error");
	return -1;
}

static int s_socket_pipe_close (int fd)
{
	s_soce_t *soc;
	DEBUGF(0, "enter");
	s_thread_mutex_lock(s_soce.lock);
	if (s_socket_pipe_find(fd, &soc)) {
		goto err0;
	}
	s_list_remove(s_soce.list, s_list_get_pos(s_soce.list, soc));
	s_pipe_api_close(soc->rsoc);
	s_pipe_api_close(soc->wsoc);
	if (soc->conn != NULL) {
		s_pipe_api_close(soc->conn->rsoc);
		s_pipe_api_close(soc->conn->wsoc);
		soc->conn->conn = NULL;
	}
	s_free(soc->bname);
	s_free(soc);
	s_thread_mutex_unlock(s_soce.lock);
	DEBUGF(0, "leave");
	return 0;
err0:	s_thread_mutex_unlock(s_soce.lock);
	DEBUGF(0, "leave error");
	return -1;
}

static int s_socket_pipe_init (void)
{
	DEBUGF(0, "enter");
	if (s_pipe_api_init()) {
		goto err0;
	}
	debugf(DSER, "Initializing Socket API Emulation using Pipes");
	s_soce.ids = MIN_SOCK_E;
	if (s_list_init(&(s_soce.list))) {
		goto err1;
	}
	if (s_thread_mutex_init(&(s_soce.lock))) {
		goto err2;
	}
	DEBUGF(0, "leave");
	return 0;
err2:	s_list_uninit(s_soce.list);
	s_soce.list = NULL;
err1:	s_pipe_api_uninit();
err0:	DEBUGF(0, "leave error");
	return -1;
}

static int s_socket_pipe_uninit (void)
{
	DEBUGF(0, "enter");
	debugf(DSER, "UnInitializing Socket API Emulation using Pipes");
	s_thread_mutex_lock(s_soce.lock);
	while (!s_list_eol(s_soce.list, 0)) {
		s_soce_t *tmp = (s_soce_t *) s_list_get(s_soce.list, 0);
		s_list_remove(s_soce.list, 0);
		s_free(tmp);
	}
	s_list_uninit(s_soce.list);
	s_soce.list = NULL;
	s_pipe_api_uninit();
	s_thread_mutex_unlock(s_soce.lock);
	s_thread_mutex_destroy(s_soce.lock);
	s_soce.lock = NULL;
	DEBUGF(0, "leave");
	return 0;
}

static s_socket_api_t s_socket_api_pipe = {
	s_socket_pipe_init,
	s_socket_pipe_uninit,
	s_socket_pipe_poll,
	s_socket_pipe_recv,
	s_socket_pipe_send,
	s_socket_pipe_socket,
	s_socket_pipe_bind,
	s_socket_pipe_listen,
	s_socket_pipe_accept,
	s_socket_pipe_connect,
	s_socket_pipe_close
};
