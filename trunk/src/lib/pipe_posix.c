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

#if defined(CONFIG_PIPE_POSIX)

static int s_pipe_posix_init (void)
{
	return 0;
}

static int s_pipe_posix_uninit (void)
{
	return 0;
}

static int s_pipe_posix_pipe (int filedes[2])
{
	return pipe(filedes);
}

static int s_pipe_posix_poll (struct pollfd *ufds, nfds_t nfds, int timeout)
{
#if defined(CONFIG_POLL_POLL)

	return poll(ufds, nfds, timeout);

#else /* CONFIG_POLL_POLL */
	/* poll() emulation using select()
	 */
	nfds_t i;
	int rval;
	int rtmp;
	int highfd;
	fd_set read;
	fd_set write;
	fd_set except;
	struct timeval tv;

	FD_ZERO(&read);
	FD_ZERO(&write);
	FD_ZERO(&except);

	highfd = -1;
	for (i = 0; i < nfds; i++) {
		if (ufds[i].fd < 0) {
			continue;
		}
		if (ufds[i].fd >= FD_SETSIZE) {
			return -1;
		}
		if (ufds[i].fd > highfd) {
			highfd = ufds[i].fd;
		}
		if (ufds[i].events & POLLIN) {
			FD_SET(ufds[i].fd, &read);
		}
		if (ufds[i].events & POLLOUT) {
			FD_SET(ufds[i].fd, &write);
		}
		FD_SET(ufds[i].fd, &except);
	}

	tv.tv_sec = timeout / 1000;
	tv.tv_usec = (timeout % 1000) * 1000;

	rval = select(highfd + 1, &read, &write, &except, timeout == -1 ? NULL : &tv);
	if (rval <= 0) {
		/* just a hack, bad file descriptor check to get closed flag
		 */
		rtmp = 0;
		for (i = 0; i < nfds; i++) {
			int flag;
			ufds[i].revents = 0;
			if (fcntl(ufds[i].fd, F_GETFL, &flag) < 0) {
				ufds[i].revents |= (POLLERR | POLLHUP | POLLNVAL);
				rtmp++;
			}
		}
		return (rtmp > 0) ? rtmp : rval;
	}

	rval = 0;
	for (i = 0; i < nfds; i++) {
		ufds[i].revents = 0;
		if (FD_ISSET(ufds[i].fd, &read)) {
			ufds[i].revents |= POLLIN;
		}
		if (FD_ISSET(ufds[i].fd, &write)) {
			ufds[i].revents |= POLLOUT;
		}
		if (FD_ISSET(ufds[i].fd, &except)) {
			ufds[i].revents |= (POLLERR | POLLHUP | POLLNVAL);
		}
		if (ufds[i].revents != 0) {
			rval++;
		}
	}
	return rval;

#endif /* CONFIG_POLL_POLL */
}

static int s_pipe_posix_read (int fd, void *buf, unsigned int count)
{
	return read(fd, buf, count);
}

static int s_pipe_posix_write (int fd, void *buf, unsigned int count)
{
	return write(fd, buf, count);
}

static int s_pipe_posix_close (int fd)
{
	return close(fd);
}

static s_pipe_api_t s_pipe_posix = {
	s_pipe_posix_init,
	s_pipe_posix_uninit,
	s_pipe_posix_pipe,
	s_pipe_posix_poll,
	s_pipe_posix_read,
	s_pipe_posix_write,
	s_pipe_posix_close
};

#endif /* CONFIG_PIPE_POSIX */
