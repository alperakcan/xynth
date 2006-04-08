/***************************************************************************
    begin                : Thu Jan 30 2003
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

#ifndef SXYNTH__H
#define SXYNTH__H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "xynth.h"

#define S_SERVER_DIR		"/tmp/.Xynth"
#define S_SERVER_SOC_NAME	"/tmp/.Xynth/UDS-0"

#define S_SERVER_SOC_PORT	9000
#define S_SERVER_SOC_ADDR	"127.0.0.1"
#define S_SERVER_SOC_ALLOW	INADDR_LOOPBACK

#define S_SERVER_SOC_PIPE	"Emulated Sockets (PIPE)"

#define S_CLIENTS_MAX	 125
#define S_MATRIX_MRBOX	 252
#define S_MATRIX_SWMOUSE 253
#define S_MATRIX_DELETED 254
#define S_MATRIX_FREE	 255

#define S_FNAME_MAX       255

#define DFAT	1
#define DSYS	2
#define DSER	4
#define DCLI	8

#define MOUSE_MASK	(MOUSE_EVENT|MOUSE_OVER|MOUSE_RELEASED|MOUSE_PRESSED|MOUSE_CLICKED|MOUSE_HINT|MOUSE_HINT2)
#define MOUSE_MASL	(MOUSE_WHEELXBUTTON_U|MOUSE_WHEELXBUTTON_D|MOUSE_WHEELYBUTTON_U|MOUSE_WHEELYBUTTON_D)
#define KEYBD_MASK	(KEYBD_EVENT|KEYBD_RELEASED|KEYBD_PRESSED)
#define KEYBD_MASL	(KEYCODE_CPLCKF|KEYCODE_NMLCKF)

#define MIN(a, b)	((a) < (b) ? (a) : (b))
#define MAX(a, b)	((a) > (b) ? (a) : (b))

#if defined(DEBUG)
	#define debugf(a, b...)	s_debug_debugf(a, __FILE__, __LINE__, (char *) __FUNCTION__, b);
#else
	#define debugf(a, b...) {\
		if ((a) & DFAT) {\
			s_debug_debugf(a, __FILE__, __LINE__, (char *) __FUNCTION__, b);\
		}\
	}
#endif

#if defined(PLATFORM_LINUX)
	#include <sys/time.h>
#elif defined(PLAFORM_GP2X)
	#include <sys/time.h>
#elif defined(PLATFORM_PSPDEV)
	#include <sys/time.h>
	unsigned int sleep (unsigned int sec);
	unsigned int usleep (unsigned long msec);
#elif defined(PLATFORM_MINGW)
	unsigned int sleep (unsigned int sec);
	unsigned int usleep (unsigned long msec);
#endif

#if defined(SOCKET_BSD)
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <sys/un.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	typedef struct sockaddr s_sockaddr_t;
#elif defined(SOCKET_PIPE)
	#define AF_PIPE		0x0001
	#define SOCK_STREAM	0x0001
	#define socklen_t	int
	struct s_sockaddr_s {
		short sa_family;
		char sa_addr[40];
	};
	typedef struct s_sockaddr_s s_sockaddr_t;
#else
	#error "Select one of the connection methods!"
#endif

#if defined(HAVE_POLL)
	#include <sys/poll.h>
#else
	#define	POLLIN		0x0001
	#define	POLLPRI		0x0002
	#define	POLLOUT		0x0004
	#define	POLLERR		0x0008
	#define	POLLHUP		0x0010
	#define	POLLNVAL	0x0020
	struct pollfd {
		int fd;
		short events;
		short revents;
	};
	typedef unsigned int nfds_t;
#endif

/* pipe_api.c */
int s_pipe_api_init (void);
int s_pipe_api_uninit (void);
int s_pipe_api_pipe (int filedes[2]);
int s_pipe_api_poll (struct pollfd *ufds, nfds_t nfds, int timeout);
int s_pipe_api_write (int fd, void *buf, int count);
int s_pipe_api_read (int fd, void *buf, int count);
int s_pipe_api_close (int fd);

/* socket_api.c */
int s_socket_api_init (void);
int s_socket_api_uninit (void);
int s_socket_api_poll (struct pollfd *ufds, nfds_t nfds, int timeout);
int s_socket_api_recv (int s, void *read_buf, int total_size);
int s_socket_api_send (int s, void *write_buf, int total_size);
int s_socket_api_socket (int domain, int type, int protocol);
int s_socket_api_bind (int sockfd, const s_sockaddr_t *my_addr, socklen_t addrlen);
int s_socket_api_listen (int s, int backlog);
int s_socket_api_accept (int s, s_sockaddr_t *addr, socklen_t *addrlen);
int s_socket_api_connect (int sockfd, const s_sockaddr_t *serv_addr, socklen_t addrlen);
int s_socket_api_close (int fd);

typedef struct s_soc_data_display_s {
	int bytesperpixel;
	int bitsperpixel;
	int colors;
	int blueoffset;
	int greenoffset;
	int redoffset;
	int bluelength;
	int greenlength;
	int redlength;
	unsigned int linear_buf;
	unsigned int matrix;
	unsigned int linear_mem_size;
	unsigned int linear_mem_base;
	int linear_buf_width;
	int linear_buf_pitch;
	int linear_buf_height;
	int shm_mid;
	int id;
	int shm_sid;
	int need_expose;
	char device[S_FNAME_MAX];
} s_soc_data_display_t;

typedef struct s_soc_data_expose_s {
	int pri;
	s_rect_t buf;
	s_rect_t win;
	s_rect_t changed;
	int linear_buf_width;
	int linear_buf_pitch;
	int linear_buf_height;
} s_soc_data_expose_t;

typedef struct s_soc_data_event_s {
	S_EVENT type;
	s_mouse_t mouse;
	s_keybd_t keybd;
} s_soc_data_event_t;

#ifdef __cplusplus
}
#endif

#endif /* SXYNTH__H */
