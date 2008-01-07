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

/* pipe(), read(), write(), poll()
   emulation over memory using mutex and condition variables.
 */

#define MIN_PIPE_E	0
#define MAX_PIPE_E	4096
#define MAX_PIPE_LEN	65536

/* enable this to enable unlimeted size of data passing
 */
#define USE_PIPE_LIST	0

typedef enum {
	PIPET_NONE  = 0x2,
	PIPET_READ  = 0x0,
	PIPET_WRITE = 0x1
} PIPE_T;

typedef struct s_pipe_data_s {
	void *data;
	unsigned int dlen;
} s_pipe_data_t;

typedef struct s_pipe_s {
	int fd;
	PIPE_T type;
#if USE_PIPE_LIST
	s_list_t *data;
#else
	s_pipe_data_t *data;
#endif
	s_list_t *wait;
	struct s_pipe_s *conn;
} s_pipe_t;

static struct {
	unsigned int fds;
	s_list_t *list;
	s_thread_mutex_t *lock;
} s_pipe;

static int s_pipe_mem_init (void)
{
	DEBUGF(0, "enter");
	debugf(DSER, "Initializing Pipe API Emulation using Mutex and Condition variables");
	if (s_list_init(&(s_pipe.list))) {
		goto err0;
	}
	if (s_thread_mutex_init(&(s_pipe.lock))) {
		goto err1;
	}
	s_pipe.fds = MIN_PIPE_E;
	DEBUGF(0, "leave");
	return 0;
err1:	s_list_uninit(s_pipe.list);
err0:	s_pipe.list = NULL;
	DEBUGF(0, "leave error");
	return -1;
}

static int s_pipe_mem_uninit (void)
{
	DEBUGF(0, "enter");
	debugf(DSER, "UnInitializing Pipe API Emulation using Mutex and Condition variables");
	s_thread_mutex_lock(s_pipe.lock);
	while (!s_list_eol(s_pipe.list, 0)) {
		s_pipe_t *tmp = (s_pipe_t *) s_list_get(s_pipe.list, 0);
		s_list_remove(s_pipe.list, 0);
		s_free(tmp);
	}
	s_list_uninit(s_pipe.list);
	s_pipe.list = NULL;
	s_thread_mutex_unlock(s_pipe.lock);
	s_thread_mutex_destroy(s_pipe.lock);
	s_pipe.lock = NULL;
	DEBUGF(0, "leave");
	return 0;
}

static int s_pipe_mem_find (int fd, s_pipe_t **p)
{
	int pos = 0;
	s_pipe_t *tmp;
	while (!s_list_eol(s_pipe.list, pos)) {
		tmp = (s_pipe_t *) s_list_get(s_pipe.list, pos);
		if (fd == tmp->fd) {
			*p = tmp;
			return 0;
		}
		pos++;
	}
	return -1;
}

static int s_pipe_mem_wake (s_pipe_t *p)
{
	int pos = 0;
	while (!s_list_eol(p->wait, pos)) {
		s_thread_cond_t *c = (s_thread_cond_t *) s_list_get(p->wait, pos);
		s_thread_cond_signal(c);
		pos++;
	}
	return 0;
}

static int s_pipe_mem_del (s_pipe_t *p)
{
	while (!s_list_eol(p->wait, 0)) {
		s_list_remove(p->wait, 0);
	}
	s_free(p->wait);
	p->wait = NULL;
	s_free(p);
	return 0;
}

static int s_pipe_mem_new (s_pipe_t **p)
{
	(*p)= (s_pipe_t *) s_malloc(sizeof(s_pipe_t));
	(*p)->fd = -1;
	(*p)->type = PIPET_NONE;
	(*p)->conn = NULL;
	(*p)->data = NULL;
	if (s_list_init(&((*p)->wait))) {
		goto err0;
	}
	return 0;
err0:	s_free(*p);
	*p = NULL;
	return -1;
}

static int s_pipe_mem_close (int fd)
{
	s_pipe_t *p;
	DEBUGF(0, "enter");
	s_thread_mutex_lock(s_pipe.lock);
	if (s_pipe_mem_find(fd, &p)) {
		goto err0;
	}
	if (p->data != NULL) {
#if USE_PIPE_LIST
		while (!s_list_eol(p->data, 0)) {
			s_pipe_data_t *data = (s_pipe_data_t *) s_list_get(p->data, 0);
			s_list_remove(p->data, 0);
			s_free(data->data);
			s_free(data);
		}
		s_list_uninit(p->data);
#else
		s_free(p->data->data);
		s_free(p->data);
#endif
	}
	if (p->conn != NULL) {
		s_pipe_mem_wake(p->conn);
		p->conn->data = NULL;
		p->conn->conn = NULL;
	}
	s_list_remove(s_pipe.list, s_list_get_pos(s_pipe.list, p));
	s_pipe_mem_wake(p);
	s_pipe_mem_del(p);
	p = NULL;
	s_thread_mutex_unlock(s_pipe.lock);
	DEBUGF(0, "leave");
	return 0;
err0:	s_thread_mutex_unlock(s_pipe.lock);
	DEBUGF(0, "leave error");
	return -1;
}

static int s_pipe_mem_pipe (int filedes[2])
{
	s_pipe_t *pr;
	s_pipe_t *pw;
#if USE_PIPE_LIST
	s_list_t *pd;
#else
	s_pipe_data_t *pd;
#endif
	DEBUGF(0, "enter");
	if (s_pipe_mem_new(&pr)) {
		goto err0;
	}
	if (s_pipe_mem_new(&pw)) {
		goto err1;
	}

#if USE_PIPE_LIST
	if (s_list_init(&pd)) {
		goto err2;
	}
#else
	pd = (s_pipe_data_t *) s_malloc(sizeof(s_pipe_data_t));
	pd->dlen = 0;
	pd->data = (void *) s_malloc(sizeof(char) * MAX_PIPE_LEN);
#endif
	s_thread_mutex_lock(s_pipe.lock);
	if (s_pipe.fds >= MAX_PIPE_E) {
		debugf(DFAT, "Unhandled BUG");
	}
	pr->fd = s_pipe.fds++;
	pw->fd = s_pipe.fds++;
	pr->type = PIPET_READ;
	pw->type = PIPET_WRITE;
	pr->data = pd;
	pw->data = pd;
	pr->conn = pw;
	pw->conn = pr;
	filedes[0] = pr->fd;
	filedes[1] = pw->fd;
	s_list_add(s_pipe.list, pr, -1);
	s_list_add(s_pipe.list, pw, -1);
	s_thread_mutex_unlock(s_pipe.lock);
	DEBUGF(0, "leave");
	return 0;
#if USE_PIPE_LIST
err2:	s_pipe_mem_del(pw);
#endif
err1:	s_pipe_mem_del(pr);
err0:	DEBUGF(0, "leave error");
	return -1;
}

static int s_pipe_mem_write (int fd, void *buf, unsigned int count)
{
	s_pipe_t *p;
#if USE_PIPE_LIST
	s_pipe_data_t *d;
#endif
	DEBUGF(0, "enter");
	s_thread_mutex_lock(s_pipe.lock);
	if (s_pipe_mem_find(fd, &p)) {
		goto err0;
	}
	if ((p->type != PIPET_WRITE) ||
	    (p->conn == NULL)) {
		goto err0;
	}
#if USE_PIPE_LIST
	d = (s_pipe_data_t *) s_malloc(sizeof(s_pipe_data_t));
	d->dlen = count;
	d->data = (void *) s_malloc(d->dlen);
	memcpy(d->data, buf, d->dlen);
	s_list_add(p->data, d, -1);
#else
	if (p->data->dlen + count > MAX_PIPE_LEN) {
		goto err0;
	}
	memcpy(p->data->data + p->data->dlen, buf, count);
	p->data->dlen += count;
#endif
	s_pipe_mem_wake(p->conn);
	s_thread_mutex_unlock(s_pipe.lock);
	DEBUGF(0, "leave");
	return count;
err0:	s_thread_mutex_unlock(s_pipe.lock);
	DEBUGF(0, "leave error");
	return -1;
}

static int s_pipe_mem_read (int fd, void *buf, unsigned int count)
{
	s_pipe_t *p;
#if USE_PIPE_LIST
	s_pipe_data_t *d;
#endif
	s_thread_cond_t *c = NULL;
	DEBUGF(0, "enter");
	s_thread_mutex_lock(s_pipe.lock);
	if (s_pipe_mem_find(fd, &p)) {
		goto err0;
	}
	if ((p->type != PIPET_READ) ||
	    (p->conn == NULL)) {
		goto err0;
	}

#if USE_PIPE_LIST
	if (p->data->nb_elt <= 0) {
		if (s_thread_cond_init(&c)) {
			goto err0;
		}
		s_list_add(p->wait, c, -1);
	}
	while (p->data->nb_elt <= 0) {
		s_thread_cond_wait(c, s_pipe.lock);
	}
#else
	if (p->data->dlen <= 0) {
		if (s_thread_cond_init(&c)) {
			goto err0;
		}
		s_list_add(p->wait, c, -1);
	}
	while (p->data->dlen <= 0) {
		s_thread_cond_wait(c, s_pipe.lock);
	}
#endif
	if (c != NULL) {
		s_list_remove(p->wait, s_list_get_pos(p->wait, c));
		s_thread_cond_destroy(c);
	}

#if USE_PIPE_LIST
	d = (s_pipe_data_t *) s_list_get(p->data, 0);
	if (d->dlen != count) {
		goto err0;
	}
	memcpy(buf, d->data, d->dlen);
	s_list_remove(p->data, 0);
	s_free(d->data);
	s_free(d);
#else
	if (p->data->dlen < count) {
		goto err0;
	}
	memcpy(buf, p->data->data, count);
	p->data->dlen -= count;
	memmove(p->data->data, p->data->data + count, p->data->dlen);
#endif
	s_pipe_mem_wake(p->conn);
	s_thread_mutex_unlock(s_pipe.lock);
	DEBUGF(0, "leave");
	return count;
err0:	s_thread_mutex_unlock(s_pipe.lock);
	DEBUGF(0, "leave error");
	return -1;
}

static int s_pipe_mem_poll (struct pollfd *ufds, nfds_t nfds, int timeout)
{
	int i;
	int ret;
	int tim;
	s_pipe_t *p;
	s_thread_cond_t *c;
	DEBUGF(0, "enter");
	s_thread_mutex_lock(s_pipe.lock);
	if (s_thread_cond_init(&c)) {
		ret = -1;
		goto err0;
	}
	for (i = 0; i < nfds; i++) {
		if (s_pipe_mem_find(ufds[i].fd, &p) == 0) {
			s_list_add(p->wait, c, -1);
		}
	}
	tim = timeout;
again:	ret = 0;
	for (i = 0; i < nfds; i++) {
		if (s_pipe_mem_find(ufds[i].fd, &p)) {
error:			ufds[i].revents = POLLNVAL | POLLHUP | POLLERR;
			ret++;
		} else {
			if (p->conn == NULL) {
				goto error;
			}
			if (ufds[i].events & POLLIN) {
				if (p->type == PIPET_READ) {
#if USE_PIPE_LIST
					if (p->data->nb_elt > 0) {
#else
					if (p->data->dlen > 0) {
#endif
						ufds[i].revents = POLLIN;
						ret++;
					}
				} else {
					goto error;
				}
			}
			if (ufds[i].events & POLLOUT) {
				if (p->type == PIPET_WRITE) {
#if USE_PIPE_LIST
					if (p->data->nb_elt == 0) {
#else
					if (p->data->dlen <= 0) {
#endif
						ufds[i].revents = POLLOUT;
						ret++;
					}
				} else {
					goto error;
				}
			}
		}
		DEBUGF(0, "fd: %d, e: 0x%x, r: 0x%x", ufds[i].fd, ufds[i].events, ufds[i].revents);
	}
	if ((tim != 0) && (ret == 0)) {
		if (tim < 0) {
			s_thread_cond_wait(c, s_pipe.lock);
		} else {
			int cret;
			long long tv[2];
			tv[0] = s_gettimeofday();
			cret = s_thread_cond_timedwait(c, s_pipe.lock, tim);
			if (cret < 0) {
				ret = -1;
				goto err0;
			} else if (cret == 0) {
				tv[1] = s_gettimeofday();
				tim -= (tv[1] - tv[0]);
				if (tim <= 0) {
					tim = 0;
				}
			} else if (cret > 0) {
				ret = 0;
				goto tout;
			}
		}
		goto again;
	}
tout:	for (i = 0; i < nfds; i++) {
		if (s_pipe_mem_find(ufds[i].fd, &p) == 0) {
			s_list_remove(p->wait, s_list_get_pos(p->wait, c));
		}
	}
	s_thread_cond_destroy(c);
	s_thread_mutex_unlock(s_pipe.lock);
	DEBUGF(0, "leave ret: %d", ret);
	return ret;
err0:	s_thread_mutex_unlock(s_pipe.lock);
	DEBUGF(0, "leave ret: %d", ret);
	return ret;
}

static s_pipe_api_t s_pipe_mem = {
	s_pipe_mem_init,
	s_pipe_mem_uninit,
	s_pipe_mem_pipe,
	s_pipe_mem_poll,
	s_pipe_mem_read,
	s_pipe_mem_write,
	s_pipe_mem_close
};
