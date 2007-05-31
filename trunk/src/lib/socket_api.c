/***************************************************************************
    begin                : Mon Aug 15 2005
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

//#define DEBUGF debugf
#define DEBUGF(...)

#include "xynth_.h"

typedef struct s_socket_api_s {
	int (*init) (void);
	int (*uninit) (void);
	int (*poll) (struct pollfd *ufds, nfds_t nfds, int timeout);
	int (*recv) (int s, void *read_buf, int total_size);
	int (*send) (int s, void *write_buf, int total_size);
	int (*socket) (int domain, int type, int protocol);
	int (*bind) (int sockfd, const s_sockaddr_t *my_addr, socklen_t addrlen);
	int (*listen) (int s, int backlog);
	int (*accept) (int s, s_sockaddr_t *addr, socklen_t *addrlen);
	int (*connect) (int sockfd, const s_sockaddr_t *serv_addr, socklen_t addrlen);
	int (*close) (int fd);
} s_socket_api_t;

#if defined(SOCKET_BSD)
	#include "socket_bsd.c"
	static s_socket_api_t *s_socket_api = &s_socket_api_bsd;
#elif defined(SOCKET_PIPE)
	#include "socket_pipe.c"
	static s_socket_api_t *s_socket_api = &s_socket_api_pipe;
#else
	#error "Select one of the connection methods!"
#endif

int s_socket_api_init (void)
{
	if ((s_socket_api != NULL) &&
	    (s_socket_api->init != NULL)) {
		return s_socket_api->init();
	}
	return -1;
}

int s_socket_api_uninit (void)
{
	if ((s_socket_api != NULL) &&
	    (s_socket_api->uninit != NULL)) {
		return s_socket_api->uninit();
	}
	return -1;
}

int s_socket_api_poll (struct pollfd *ufds, nfds_t nfds, int timeout)
{
	if ((s_socket_api != NULL) &&
	    (s_socket_api->poll != NULL)) {
		return s_socket_api->poll(ufds, nfds, timeout);
	}
	return -1;
}

int s_socket_api_recv (int s, void *read_buf, int total_size)
{
	int ret;
	int read = 0;
	if ((s_socket_api != NULL) && (s_socket_api->recv != NULL)) {
		while (total_size > 0) {
			ret = s_socket_api->recv(s, read_buf + read, total_size);
			if (ret < 0) {
				goto err;
			}
			read += ret;
			total_size -= ret;
		}
		return read;
	}
err:	return -1;
}

int s_socket_api_send (int s, void *write_buf, int total_size)
{
	int ret;
	int sent = 0;
	if ((s_socket_api != NULL) && (s_socket_api->send != NULL)) {
		while (total_size > 0) {
			ret = s_socket_api->send(s, write_buf + sent, total_size);
			if (ret < 0) {
				goto err;
			}
			sent += ret;
			total_size -= ret;
		}
		return sent;
	}
err:	return -1;
}

int s_socket_api_socket (int domain, int type, int protocol)
{
	if ((s_socket_api != NULL) &&
	    (s_socket_api->socket != NULL)) {
		return s_socket_api->socket(domain, type, protocol);
	}
	return -1;
}

int s_socket_api_bind (int sockfd, const s_sockaddr_t *my_addr, socklen_t addrlen)
{
	if ((s_socket_api != NULL) &&
	    (s_socket_api->bind != NULL)) {
		return s_socket_api->bind(sockfd, my_addr, addrlen);
	}
	return -1;
}

int s_socket_api_listen (int s, int backlog)
{
	if ((s_socket_api != NULL) &&
	    (s_socket_api->listen != NULL)) {
		return s_socket_api->listen(s, backlog);
	}
	return -1;
}

int s_socket_api_accept (int s, s_sockaddr_t *addr, socklen_t *addrlen)
{
	if ((s_socket_api != NULL) &&
	    (s_socket_api->accept != NULL)) {
		return s_socket_api->accept(s, addr, addrlen);
	}
	return -1;
}

int s_socket_api_connect (int sockfd, const s_sockaddr_t *serv_addr, socklen_t addrlen)
{
	if ((s_socket_api != NULL) &&
	    (s_socket_api->connect != NULL)) {
		return s_socket_api->connect(sockfd, serv_addr, addrlen);
	}
	return -1;
}

int s_socket_api_close (int fd)
{
	if ((s_socket_api != NULL) &&
	    (s_socket_api->close != NULL)) {
		return s_socket_api->close(fd);
	}
	return -1;
}
