/***************************************************************************
    begin                : Thu Jan 30 2003
    copyright            : (C) 2003 - 2008 by Alper Akcan
    email                : alper.akcan@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#ifndef XYNTH__H
#define XYNTH__H

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
#include "config.h"
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
#define S_MATRIX_LOCKED  254
#define S_MATRIX_FREE	 255

#define S_FNAME_MAX      255
#define S_TITLE_MAX      255

#define DFAT	1
#define DSYS	2
#define DSER	4
#define DCLI	8

#define MOUSE_MASL	(MOUSE_BUTTON_WHEELX_UP|MOUSE_BUTTON_WHEELX_DOWN|MOUSE_BUTTON_WHEELY_UP|MOUSE_BUTTON_WHEELY_DOWN)
#define KEYBD_MASL	(KEYBOARD_FLAG_CAPSLOCK|KEYBOARD_FLAG_NUMLOCK)

#define MIN(a, b)	((a) < (b) ? (a) : (b))
#define MAX(a, b)	((a) > (b) ? (a) : (b))

#if defined(CONFIG_DEBUG)
	#define debugf(a, b...)	s_debug_debugf(a, __FILE__, __LINE__, (char *) __FUNCTION__, b);
#else
	#define debugf(a, b...) {\
		if ((a) & DFAT) {\
			s_debug_debugf(a, __FILE__, __LINE__, (char *) __FUNCTION__, b);\
		}\
	}
#endif

#if defined(CONFIG_PLATFORM_LINUX)
	#include <sys/time.h>
#elif defined(CONFIG_PLATFORM_GP2X)
	#include <sys/time.h>
#elif defined(CONFIG_PLATFORM_PSPDEV)
	#include <sys/time.h>
	unsigned int sleep (unsigned int sec);
	void usleep (unsigned long msec);
#elif defined(CONFIG_PLATFORM_MINGW)
	unsigned int sleep (unsigned int sec);
	void usleep (unsigned long msec);
#endif

#if defined(CONFIG_POLL_POLL)
	#include <sys/poll.h>
#else
	#include <fcntl.h>
	#include <sys/time.h>
#if !(defined(CONFIG_PLATFORM_MINGW))
	#include <sys/select.h>
#endif

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

#if defined(CONFIG_SOCKET_POSIX)
	#include <sys/types.h>
	#include <sys/socket.h>
#if defined(CONFIG_IPC_UDS)
	#include <sys/un.h>
#endif
	#include <netinet/in.h>
	#include <arpa/inet.h>
	typedef struct sockaddr s_sockaddr_t;
#elif defined(CONFIG_SOCKET_PIPE)
	#define AF_PIPE		0x0001
#if !defined(SOCK_STREAM)
	#define SOCK_STREAM	0x0001
	typedef unsigned int    socklen_t;
#endif
	struct s_sockaddr_s {
		short sa_family;
		char sa_addr[40];
	};
	typedef struct s_sockaddr_s s_sockaddr_t;
#else
	#error "Select one of the connection methods!"
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
	 /* 336 bytes */
	unsigned int bytesperpixel;
	unsigned int bitsperpixel;
	unsigned int colors;
	unsigned int blueoffset;
	unsigned int greenoffset;
	unsigned int redoffset;
	unsigned int bluelength;
	unsigned int greenlength;
	unsigned int redlength;
	unsigned int bluemask;
	unsigned int greenmask;
	unsigned int redmask;
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
	/* 64 bytes */
	int pri;
	s_rect_t buf;
	s_rect_t win;
	s_rect_t changed;
	int linear_buf_width;
	int linear_buf_pitch;
	int linear_buf_height;
} s_soc_data_expose_t;

typedef struct s_soc_data_event_s {
	/* 880 bytes */
	s_event_type_t type;
	s_mouse_t mouse;
	s_keybd_t keybd;
} s_soc_data_event_t;

typedef struct s_soc_data_configure_s {
	/* 288 bytes */
	s_window_type_t form;
	s_rect_t rnew;
	int resizeable;
	int alwaysontop;
	s_cursor_type_t cursor;
	char title[S_TITLE_MAX];
} s_soc_data_configure_t;

typedef struct s_soc_data_stream_s {
	/* 28 bytes */
	int bitspp;
	s_rect_t rect;
	unsigned int blen;
	char *buf;
} s_soc_data_stream_t;

typedef struct s_soc_data_desktop_s {
	/* 33004 bytes */
	int count;
	struct {
		int id;
		int pri;
		char title[S_TITLE_MAX];
	} client[S_CLIENTS_MAX];
} s_soc_data_desktop_t;

typedef struct s_soc_data_show_s {
	/* 8 bytes */
	int id;
	int show;
	char title[S_TITLE_MAX];
} s_soc_data_show_t;

#ifdef __cplusplus
}
#endif

#endif /* XYNTH__H */
