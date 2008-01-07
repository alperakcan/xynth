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

static int s_socket_bsd_init (void)
{
	return 0;
}

static int s_socket_bsd_uninit (void)
{
	return 0;
}

static int s_socket_bsd_poll (struct pollfd *ufds, nfds_t nfds, int timeout)
{
	return s_pipe_api_poll(ufds, nfds, timeout);
}

static int s_socket_bsd_recv (int s, void *read_buf, int total_size)
{
	int r = 0;
	int r_size = 0;
	while (total_size != r_size) {
		r = recv(s, read_buf + r_size, total_size - r_size, MSG_NOSIGNAL);
		if (r <= 0) {
			s_socket_api_close(s);
			return -1;
		}
		r_size += r;
	}
	return r_size;
}

static int s_socket_bsd_send (int s, void *write_buf, int total_size)
{
	int w = 0;
	int w_size = 0;
	while (total_size != w_size) {
		w = send(s, write_buf + w_size, total_size - w_size, MSG_NOSIGNAL);
		if (w <= 0) {
			s_socket_api_close(s);
			return -1;
		}
		w_size += w;
	}
	return w_size;
}

static int s_socket_bsd_socket (int domain, int type, int protocol)
{
	return socket(domain, type, protocol);
}

static int s_socket_bsd_bind (int sockfd, const s_sockaddr_t *my_addr, socklen_t addrlen)
{
	unsigned int yes = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
		debugf(DSER | DSYS, "Reusing ADDR failed");
		return -1;
	}
	return bind(sockfd, my_addr, addrlen);
}

static int s_socket_bsd_listen (int s, int backlog)
{
	return listen(s, backlog);
}

static int s_socket_bsd_accept (int s, s_sockaddr_t *addr, socklen_t *addrlen)
{
	return accept(s, addr, addrlen);
}

static int s_socket_bsd_connect (int sockfd, const s_sockaddr_t *serv_addr, socklen_t addrlen)
{
	return connect(sockfd, serv_addr, addrlen);
}

static int s_socket_bsd_close (int fd)
{
	shutdown(fd, SHUT_RDWR);
	close(fd);
	return 0;
}

static s_socket_api_t s_socket_api_bsd = {
	s_socket_bsd_init,
	s_socket_bsd_uninit,
	s_socket_bsd_poll,
	s_socket_bsd_recv,
	s_socket_bsd_send,
	s_socket_bsd_socket,
	s_socket_bsd_bind,
	s_socket_bsd_listen,
	s_socket_bsd_accept,
	s_socket_bsd_connect,
	s_socket_bsd_close
};
