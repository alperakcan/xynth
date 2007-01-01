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

#if defined(THREAD_MUTEX_EMULATION)

/* Emulation is heavily based on SDL Library www.libsdl.org
 */

struct s_thread_mutex_s {
	int recursive;
	int owner;
	s_thread_sem_t *sem;
};

struct s_thread_cond_s {
	s_thread_mutex_t *lock;
	int waiting;
	int signals;
	s_thread_sem_t *wait_sem;
	s_thread_sem_t *wait_done;
};

/* Implementation of mutexes using semaphores
 */

static int s_thread_emul_mutex_init (s_thread_mutex_t *mut)
{
	if (s_thread_sem_create(&(mut->sem), 1)) {
		goto err0;
	}
	mut->recursive = 0;
	mut->owner = 0;
	return 0;
err0:	s_free(mut->sem);
	mut->sem = NULL;
	return -1;
}

static int s_thread_emul_mutex_destroy (s_thread_mutex_t *mut)
{
	int ret = -1;
	if (mut->sem) {
		ret = s_thread_sem_destroy(mut->sem);
		mut->sem = NULL;
	}
	return ret;
}

static int s_thread_emul_mutex_lock (s_thread_mutex_t *mut)
{
	int ret = 0;
	int tid;

	if (mut->sem == NULL) {
		return -1;
	}

	tid = s_thread_self();
	if (mut->owner == tid) {
		++mut->recursive;
	} else {
		ret = s_thread_sem_wait(mut->sem);
		mut->owner = tid;
		mut->recursive = 0;
	}

	return ret;
}

static int s_thread_emul_mutex_trylock (s_thread_mutex_t *mut)
{
	return -1;
}

static int s_thread_emul_mutex_unlock (s_thread_mutex_t *mut)
{
        int ret = 0;
	if (mut->sem == NULL) {
		return -1;
	}
	if (s_thread_self() != mut->owner) {
		return -1;
	}
	if (mut->recursive) {
		--mut->recursive;
	} else {
		mut->owner = 0;
		ret = s_thread_sem_post(mut->sem);
	}
	return ret;
}

/* Implementation of condition variables using semaphores and mutexes
 */

static int s_thread_emul_cond_init (s_thread_cond_t *cond)
{
	if (s_thread_mutex_init(&(cond->lock))) {
		goto err0;
	}
	if (s_thread_sem_create(&(cond->wait_sem), 0)) {
		goto err1;
	}
	if (s_thread_sem_create(&(cond->wait_done), 0)) {
		goto err2;
	}
	cond->waiting = 0;
	cond->signals = 0;
	return 0;

err2:	cond->wait_done = NULL;
	s_thread_sem_destroy(cond->wait_sem);
err1:	cond->wait_sem = NULL;
	s_thread_mutex_destroy(cond->lock);
err0:	return -1;
}

static int s_thread_emul_cond_destroy (s_thread_cond_t *cond)
{
	int ret = 0;
	if (cond->wait_done) {
		ret |= s_thread_sem_destroy(cond->wait_done);
		cond->wait_done = NULL;
	}
	if (cond->wait_sem) {
		ret |= s_thread_sem_destroy(cond->wait_sem);
		cond->wait_sem = NULL;
	}
	if (cond->lock) {
		ret |= s_thread_mutex_destroy(cond->lock);
		cond->lock = NULL;
	}
	return ret;
}

static int s_thread_emul_cond_signal (s_thread_cond_t *cond)
{
	s_thread_mutex_lock(cond->lock);
	if (cond->waiting > cond->signals) {
		++cond->signals;
		s_thread_sem_post(cond->wait_sem);
		s_thread_mutex_unlock(cond->lock);
		s_thread_sem_wait(cond->wait_done);
	} else {
		s_thread_mutex_unlock(cond->lock);
	}

	return 0;
}

static int s_thread_emul_cond_broadcast (s_thread_cond_t *cond)
{
	s_thread_mutex_lock(cond->lock);
	if (cond->waiting > cond->signals) {
		int i;
		int num_waiting;

		num_waiting = (cond->waiting - cond->signals);
		cond->signals = cond->waiting;
		for (i = 0; i < num_waiting; ++i) {
			s_thread_sem_post(cond->wait_sem);
		}
		s_thread_mutex_unlock(cond->lock);
		for (i = 0; i < num_waiting; ++i) {
			s_thread_sem_wait(cond->wait_done);
		}
	} else {
		s_thread_mutex_unlock(cond->lock);
	}
	return 0;
}

static int s_thread_emul_cond_wait (s_thread_cond_t *cond, s_thread_mutex_t *mut)
{
        s_thread_mutex_lock(cond->lock);
        ++cond->waiting;
        s_thread_mutex_unlock(cond->lock);
        s_thread_mutex_unlock(mut);
        s_thread_sem_wait(cond->wait_sem);
        s_thread_mutex_lock(cond->lock);
        if (cond->signals > 0) {
		s_thread_sem_post(cond->wait_done);
		--cond->signals;
	}
	--cond->waiting;
	s_thread_mutex_unlock(cond->lock);
	s_thread_mutex_lock(mut);
	return 0;
}

static int s_thread_emul_cond_timedwait (s_thread_cond_t *cond, s_thread_mutex_t *mut, int msec)
{
	int ret = 0;
	s_thread_mutex_lock(cond->lock);
	++cond->waiting;
	s_thread_mutex_unlock(cond->lock);
	s_thread_mutex_unlock(mut);
	ret = s_thread_sem_wait_timeout(cond->wait_sem, msec);
	s_thread_mutex_lock(cond->lock);
	if (cond->signals > 0) {
		if (ret > 0) {
			s_thread_sem_wait(cond->wait_sem);
		}
		s_thread_sem_post(cond->wait_done);
		--cond->signals;
	}
	--cond->waiting;
	s_thread_mutex_unlock(cond->lock);
	s_thread_mutex_lock(mut);
	return ret;
}

#endif /* THREAD_MUTEX_EMULATION */
