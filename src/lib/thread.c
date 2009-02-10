/***************************************************************************
    begin                : Thu Mar 25 2004
    copyright            : (C) 2004 - 2008 by Alper Akcan
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

#include "xynth_.h"

typedef struct s_thread_arg_s {
	int flag;
	void *arg;
	void * (*r) (void *);
	void * (*f) (void *);
	s_thread_cond_t *cond;
	s_thread_mutex_t *mut;
} s_thread_arg_t;

typedef struct s_thread_api_s {
	int (*sem_create) (s_thread_sem_t *sem, int initial);
	int (*sem_destroy) (s_thread_sem_t *sem);
	int (*sem_wait) (s_thread_sem_t *sem);
	int (*sem_wait_timeout) (s_thread_sem_t *sem, int msec);
	int (*sem_post) (s_thread_sem_t *sem);

	int (*mutex_init) (s_thread_mutex_t *mut);
	int (*mutex_destroy) (s_thread_mutex_t *mut);
	int (*mutex_lock) (s_thread_mutex_t *mut);
	int (*mutex_trylock) (s_thread_mutex_t *mut);
	int (*mutex_unlock) (s_thread_mutex_t *mut);

	int (*cond_init) (s_thread_cond_t *cond);
	int (*cond_destroy) (s_thread_cond_t *cond);
	int (*cond_signal) (s_thread_cond_t *cond);
	int (*cond_broadcast) (s_thread_cond_t *cond);
	int (*cond_wait) (s_thread_cond_t *cond, s_thread_mutex_t *mut);
	int (*cond_timedwait) (s_thread_cond_t *cond, s_thread_mutex_t *mut, int msec);

	int (*thread_create) (s_thread_t *tid, s_thread_arg_t *targ);
	int (*thread_cancel) (s_thread_t *tid);
	int (*thread_join) (s_thread_t *tid, void **ret);
	int (*thread_detach) (s_thread_t *tid);
	int (*thread_self) (void);
	void (*thread_exit) (void *ret);
} s_thread_api_t;

#if defined(CONFIG_THREAD_PSPDEV)
	#include "thread_pspdev.c"
	static s_thread_api_t *s_thread_api = &s_thread_pspdev;
#elif defined(CONFIG_THREAD_PTHREADS)
	#include "thread_pthreads.c"
	static s_thread_api_t *s_thread_api = &s_thread_pthreads;
#elif defined(CONFIG_THREAD_WINDOWS)
	#include "thread_win32.c"
	static s_thread_api_t *s_thread_api = &s_thread_win32;
#else
	struct s_thread_s {
		char foo;
	};
	struct s_thread_sem_s {
		char foo;
	};
	struct s_thread_cond_s {
		char foo;
	};
	struct s_thread_mutex_s {
		char foo;
	};
	#error "I do need threads support"
	#error "Check Makefile.cfg"
#endif

int s_thread_sem_create (s_thread_sem_t **sem, int initial)
{
	if ((s_thread_api == NULL) ||
	    (s_thread_api->sem_create == NULL)) {
		return -1;
	}
        (*sem) = (s_thread_sem_t *) s_malloc(sizeof(s_thread_sem_t));
	return s_thread_api->sem_create(*sem, initial);
}

int s_thread_sem_destroy (s_thread_sem_t *sem)
{
	int ret;
	if ((s_thread_api == NULL) ||
	    (s_thread_api->sem_destroy == NULL)) {
		return -1;
	}
	ret = s_thread_api->sem_destroy(sem);
	s_free(sem);
	sem = NULL;
	return ret;
}

int s_thread_sem_wait (s_thread_sem_t *sem)
{
	if ((s_thread_api == NULL) ||
	    (s_thread_api->sem_wait == NULL)) {
		return -1;
	}
	return s_thread_api->sem_wait(sem);
}

int s_thread_sem_wait_timeout (s_thread_sem_t *sem, int msec)
{
	if ((s_thread_api == NULL) ||
	    (s_thread_api->sem_wait_timeout == NULL)) {
		return -1;
	}
	return s_thread_api->sem_wait_timeout(sem, msec);
}

int s_thread_sem_post (s_thread_sem_t *sem)
{
	if ((s_thread_api == NULL) ||
	    (s_thread_api->sem_post == NULL)) {
		return -1;
	}
	return s_thread_api->sem_post(sem);
}

int s_thread_mutex_init (s_thread_mutex_t **mut)
{
	if ((s_thread_api == NULL) ||
	    (s_thread_api->mutex_init == NULL)) {
		return -1;
	}
	(*mut) = (s_thread_mutex_t *) s_malloc(sizeof(s_thread_mutex_t));
	if (s_thread_api->mutex_init(*mut)) {
		goto err0;
	}
	return 0;
err0:	s_free(*mut);
	*mut = NULL;
	return -1;
}

int s_thread_mutex_destroy (s_thread_mutex_t *mut)
{
        int ret;
	if ((s_thread_api == NULL) ||
	    (s_thread_api->mutex_destroy == NULL)) {
		return -1;
	}
	if (mut == NULL) {
		return 1;
	}
	ret = s_thread_api->mutex_destroy(mut);
        s_free(mut);
        mut = NULL;
        return ret;
}

int s_thread_mutex_lock (s_thread_mutex_t *mut)
{
	if ((s_thread_api == NULL) ||
	    (s_thread_api->mutex_lock == NULL)) {
		return -1;
	}
	if (mut == NULL) {
		return -1;
	}
	return s_thread_api->mutex_lock(mut);
}

int s_thread_mutex_trylock (s_thread_mutex_t *mut)
{
	if ((s_thread_api == NULL) ||
	    (s_thread_api->mutex_trylock == NULL)) {
		return -1;
	}
	if (mut == NULL) {
		return -1;
	}
	return s_thread_api->mutex_trylock(mut);
}

int s_thread_mutex_unlock (s_thread_mutex_t *mut)
{
	if ((s_thread_api == NULL) ||
	    (s_thread_api->mutex_unlock == NULL)) {
		return -1;
	}
	if (mut == NULL) {
		return -1;
	}
	return s_thread_api->mutex_unlock(mut);
}

int s_thread_cond_init (s_thread_cond_t **cond)
{
	if ((s_thread_api == NULL) ||
	    (s_thread_api->cond_init == NULL)) {
		return -1;
	}
	(*cond) = (s_thread_cond_t *) s_malloc(sizeof(s_thread_cond_t));
	if (s_thread_api->cond_init(*cond)) {
		goto err0;
	}
	return 0;
err0:	s_free(*cond);
	*cond = NULL;
	return -1;
}

int s_thread_cond_destroy (s_thread_cond_t *cond)
{
	int ret;
	if ((s_thread_api == NULL) ||
	    (s_thread_api->cond_destroy == NULL)) {
		return -1;
	}
	if (cond == NULL) {
		return 1;
	}
	ret = s_thread_api->cond_destroy(cond);
	s_free(cond);
	cond = NULL;
	return ret;
}

int s_thread_cond_signal (s_thread_cond_t *cond)
{
	if ((s_thread_api == NULL) ||
	    (s_thread_api->cond_signal == NULL)) {
		return -1;
	}
	return s_thread_api->cond_signal(cond);
}

int s_thread_cond_broadcast (s_thread_cond_t *cond)
{
	if ((s_thread_api == NULL) ||
	    (s_thread_api->cond_broadcast == NULL)) {
		return -1;
	}
	return s_thread_api->cond_broadcast(cond);
}

int s_thread_cond_wait (s_thread_cond_t *cond, s_thread_mutex_t *mut)
{
	if ((s_thread_api == NULL) ||
	    (s_thread_api->cond_wait == NULL)) {
		return -1;
	}
	return s_thread_api->cond_wait(cond, mut);
}

int s_thread_cond_timedwait (s_thread_cond_t *cond, s_thread_mutex_t *mut, int msec)
{
	if ((s_thread_api == NULL) ||
	    (s_thread_api->cond_timedwait == NULL)) {
		return -1;
	}
	return s_thread_api->cond_timedwait(cond, mut, msec);
}

static void * s_thread_run (void *farg)
{
	s_thread_arg_t *arg = (s_thread_arg_t *) farg;
	void *p = arg->arg;
	void * (*f) (void *) = arg->f;

	s_thread_mutex_lock(arg->mut);
	arg->flag = 1;
	s_thread_cond_signal(arg->cond);
	s_thread_mutex_unlock(arg->mut);

	f(p);

	return NULL;
}

s_thread_t * s_thread_create (void * (*f) (void *), void *farg)
{
	s_thread_t *tid;
	s_thread_arg_t *arg;

	if ((s_thread_api == NULL) ||
	    (s_thread_api->thread_create == NULL)) {
		return NULL;
	}

	tid = (s_thread_t *) s_malloc(sizeof(s_thread_t));
	arg = (s_thread_arg_t *) s_malloc(sizeof(s_thread_arg_t));

	arg->r = &s_thread_run;
	arg->f = f;
	arg->arg = farg;
	s_thread_cond_init(&arg->cond);
	s_thread_mutex_init(&arg->mut);

	s_thread_mutex_lock(arg->mut);
	arg->flag = 0;
	s_thread_cond_signal(arg->cond);
	s_thread_mutex_unlock(arg->mut);

	s_thread_api->thread_create(tid, arg);

	s_thread_mutex_lock(arg->mut);
	while (arg->flag != 1) {
		if (s_thread_cond_wait(arg->cond, arg->mut)) {
			debugf(DSYS, "s_thread_cond_wait failed");
			return NULL;
		}
	}
	s_thread_mutex_unlock(arg->mut);

	s_thread_cond_destroy(arg->cond);
	s_thread_mutex_destroy(arg->mut);
	s_free(arg);
	arg = NULL;

	return tid;
}

int s_thread_cancel (s_thread_t *tid)
{
	if ((s_thread_api == NULL) ||
	    (s_thread_api->thread_cancel == NULL)) {
		return -1;
	}
	return s_thread_api->thread_cancel(tid);
}

int s_thread_join (s_thread_t *tid, void **ret)
{
	int r;
	if ((tid == NULL) ||
	    (s_thread_api == NULL) ||
	    (s_thread_api->thread_join == NULL)) {
		return -1;
	}
	r = s_thread_api->thread_join(tid, ret);
	s_free(tid);
	return r;
}

int s_thread_detach (s_thread_t *tid)
{
	int r;
	if ((tid == NULL) ||
	    (s_thread_api == NULL) ||
	    (s_thread_api->thread_detach == NULL)) {
		return -1;
	}
	r = s_thread_api->thread_detach(tid);
	s_free(tid);
	return r;
}

int s_thread_self (void)
{
	if ((s_thread_api == NULL) ||
	    (s_thread_api->thread_self == NULL)) {
		return -1;
	}
	return s_thread_api->thread_self();
}

void s_thread_exit (void *ret)
{
	if ((s_thread_api == NULL) ||
	    (s_thread_api->thread_exit == NULL)) {
		return;
	}
	s_thread_api->thread_exit(ret);
}
