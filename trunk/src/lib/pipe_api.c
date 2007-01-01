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

typedef struct s_pipe_api_s {
	int (*init) (void);
	int (*uninit) (void);
	int (*pipe) (int filedes[2]);
	int (*poll) (struct pollfd *ufds, nfds_t nfds, int timeout);
	int (*read) (int fd, void *buf, unsigned int count);
	int (*write) (int fd, void *buf, unsigned int count);
	int (*close) (int fd);
} s_pipe_api_t;

#if defined(PIPE_POSIX)
	#include "pipe_posix.c"
	static s_pipe_api_t *s_pipe_api = &s_pipe_posix;
#elif defined(PIPE_MEM)
	#include "pipe_mem.c"
	static s_pipe_api_t *s_pipe_api = &s_pipe_mem;
#endif

int s_pipe_api_init (void)
{
	if ((s_pipe_api != NULL) &&
	    (s_pipe_api->init != NULL)) {
		return s_pipe_api->init();
	}
	return -1;
}

int s_pipe_api_uninit (void)
{
	if ((s_pipe_api != NULL) &&
	    (s_pipe_api->uninit != NULL)) {
		return s_pipe_api->uninit();
	}
	return -1;
}

int s_pipe_api_pipe (int filedes[2])
{
	if ((s_pipe_api != NULL) &&
	    (s_pipe_api->pipe != NULL)) {
		return s_pipe_api->pipe(filedes);
	}
	return -1;
}

int s_pipe_api_poll (struct pollfd *ufds, nfds_t nfds, int timeout)
{
	if ((s_pipe_api != NULL) &&
	    (s_pipe_api->poll != NULL)) {
		return s_pipe_api->poll(ufds, nfds, timeout);
	}
	return -1;
}

int s_pipe_api_write (int fd, void *buf, int count)
{
	if ((s_pipe_api != NULL) &&
	    (s_pipe_api->write != NULL)) {
		return s_pipe_api->write(fd, buf, count);
	}
	return -1;
}

int s_pipe_api_read (int fd, void *buf, int count)
{
	if ((s_pipe_api != NULL) &&
	    (s_pipe_api->read != NULL)) {
		return s_pipe_api->read(fd, buf, count);
	}
	return -1;
}

int s_pipe_api_close (int fd)
{
	if ((s_pipe_api != NULL) &&
	    (s_pipe_api->close != NULL)) {
		return s_pipe_api->close(fd);
	}
	return -1;
}
