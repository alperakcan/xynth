/***************************************************************************
    begin                : Thu Mar 25 2004
    copyright            : (C) 2004 - 2007 by Alper Akcan
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

#if defined(THREAD_PTHREADS)

#include <pthread.h>

#if defined(PLATFORM_MINGW)
#include <windows.h>
#endif

#if defined(THREAD_MUTEX_EMULATION)

#include <semaphore.h>
struct s_thread_sem_s {
	sem_t *sem;
	sem_t sem_data;
};

#include "thread_mutex_emul.c"

static int s_thread_pthreads_sem_create (s_thread_sem_t *sem, int initial)
{
	if (sem == NULL) {
		goto err;
	}
	if (sem_init(&sem->sem_data, 0, initial) < 0) {
		goto err;
	}
	sem->sem = &sem->sem_data;
	return 0;
err:	return -1;
}

static int s_thread_pthreads_sem_destroy (s_thread_sem_t *sem)
{
	if (sem == NULL) {
		goto err;
	}
	sem_destroy(sem->sem);
	return 0;
err:	return -1;
}

static int s_thread_pthreads_sem_wait (s_thread_sem_t *sem)
{
	if (sem == NULL) {
		goto err;
	}
	if (sem_wait(sem->sem) < 0) {
		goto err;
	}
	return 0;
err:	return -1;
}

static int s_thread_pthreads_sem_wait_timeout (s_thread_sem_t *sem, int msec)
{
	unsigned int psec;
	signed int *ptout;

	if (sem == NULL) {
		return -1;
	}
	psec = (unsigned int) msec;
	ptout = &psec;

	do {
		if (sem_trywait(sem->sem) == 0) {
			break;
		}
		usleep(100);
		*ptout -= 100;
	} while (*ptout > 0);
	if (*ptout < 0) {
		return 1;
	}
	return 0;
}

static int s_thread_pthreads_sem_post (s_thread_sem_t *sem)
{
	if (sem == NULL) {
		goto err;
	}
	if (sem_post(sem->sem) < 0) {
		goto err;
	}
	return 0;
err:	return -1;
}

#else /* THREAD_MUTEX_EMULATION */

struct s_thread_sem_s {
	char foo;
};

struct s_thread_cond_s {
	pthread_cond_t cond;
};

struct s_thread_mutex_s {
	pthread_mutex_t mut;
};

static int s_thread_pthreads_mutex_init (s_thread_mutex_t *mut)
{
#if 1
	return pthread_mutex_init(&(mut->mut), NULL);
#else
	pthread_mutexattr_t mattr = { PTHREAD_MUTEX_RECURSIVE_NP };
	return pthread_mutex_init(&(mut->mut), &mattr);
#endif
}

static int s_thread_pthreads_mutex_destroy (s_thread_mutex_t *mut)
{
	return pthread_mutex_destroy(&(mut->mut));
}

static int s_thread_pthreads_mutex_lock (s_thread_mutex_t *mut)
{
	return pthread_mutex_lock(&(mut->mut));
}

static int s_thread_pthreads_mutex_trylock (s_thread_mutex_t *mut)
{
	int ret;
	ret = pthread_mutex_trylock(&(mut->mut));
	if (ret == EBUSY) {
		ret = 1;
	}
	return ret;
}

static int s_thread_pthreads_mutex_unlock (s_thread_mutex_t *mut)
{
	return pthread_mutex_unlock(&(mut->mut));
}

static int s_thread_pthreads_cond_init (s_thread_cond_t *cond)
{
	return pthread_cond_init(&(cond->cond), NULL);
}

static int s_thread_pthreads_cond_destroy (s_thread_cond_t *cond)
{
	return pthread_cond_destroy(&(cond->cond));
}

static int s_thread_pthreads_cond_signal (s_thread_cond_t *cond)
{
	return pthread_cond_signal(&(cond->cond));
}

static int s_thread_pthreads_cond_broadcast (s_thread_cond_t *cond)
{
	return pthread_cond_broadcast(&(cond->cond));
}

static int s_thread_pthreads_cond_wait (s_thread_cond_t *cond, s_thread_mutex_t *mut)
{
	return pthread_cond_wait(&(cond->cond), &(mut->mut));
}

static int s_thread_pthreads_cond_timedwait (s_thread_cond_t *cond, s_thread_mutex_t *mut, int msec)
{
	int ret;
	long long tval;
	struct timespec tspec;

	tval = s_gettimeofday();
	tspec.tv_sec = (tval / 1000) + (msec / 1000);
	tspec.tv_nsec = ((tval % 1000) + (msec % 1000)) * 1000 * 1000;

        if (tspec.tv_nsec > 1000000000) {
		tspec.tv_sec += 1;
		tspec.tv_nsec -= 1000000000;
	}

again:	ret = pthread_cond_timedwait(&(cond->cond), &(mut->mut), &tspec);
	switch (ret) {
		case EINTR:
			goto again;
			break;
		case ETIMEDOUT:
			ret = 1;
			break;
		case 0:
			break;
		default:
			ret = -1;
			break;
	}
	return ret;
}

#endif

struct s_thread_s {
	pthread_t tid;
};

static int s_thread_pthreads_thread_create (s_thread_t *tid, s_thread_arg_t *targs)
{
        int ret;

#ifdef __UCLIBC__
	ret = pthread_create(&(tid->tid),  NULL, targs->r, targs);
#else
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	ret = pthread_create(&(tid->tid), &attr, targs->r, targs);
	pthread_attr_destroy(&attr);
#endif

	return ret;
}

static int s_thread_pthreads_thread_cancel (s_thread_t *tid)
{
	return pthread_cancel(tid->tid);
}

static int s_thread_pthreads_thread_join (s_thread_t *tid, void **ret)
{
	return pthread_join(tid->tid, ret);
}

static int s_thread_pthreads_thread_self (void)
{
#if defined(PLATFORM_MINGW)
	return *(int *) pthread_self().p;
#else
	return (int) pthread_self();
#endif
}

static void s_thread_pthreads_thread_exit (void *ret)
{
	pthread_exit(ret);
}

static s_thread_api_t s_thread_pthreads = {
#if defined(THREAD_MUTEX_EMULATION)
	s_thread_pthreads_sem_create,
	s_thread_pthreads_sem_destroy,
	s_thread_pthreads_sem_wait,
	s_thread_pthreads_sem_wait_timeout,
	s_thread_pthreads_sem_post,

	s_thread_emul_mutex_init,
	s_thread_emul_mutex_destroy,
	s_thread_emul_mutex_lock,
	s_thread_emul_mutex_trylock,
	s_thread_emul_mutex_unlock,

	s_thread_emul_cond_init,
	s_thread_emul_cond_destroy,
	s_thread_emul_cond_signal,
	s_thread_emul_cond_broadcast,
	s_thread_emul_cond_wait,
	s_thread_emul_cond_timedwait,
#else
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,

	s_thread_pthreads_mutex_init,
	s_thread_pthreads_mutex_destroy,
	s_thread_pthreads_mutex_lock,
	s_thread_pthreads_mutex_trylock,
	s_thread_pthreads_mutex_unlock,

	s_thread_pthreads_cond_init,
	s_thread_pthreads_cond_destroy,
	s_thread_pthreads_cond_signal,
	s_thread_pthreads_cond_broadcast,
	s_thread_pthreads_cond_wait,
	s_thread_pthreads_cond_timedwait,
#endif

	s_thread_pthreads_thread_create,
	s_thread_pthreads_thread_cancel,
	s_thread_pthreads_thread_join,
	s_thread_pthreads_thread_self,
	s_thread_pthreads_thread_exit,
};

#endif /* THREAD_PTHREADS */
