/***************************************************************************
    begin                : Wed Dec 14 2005
    copyright            : (C) 2005 - 2007 by Caglar Oral
    email                : caglaroral@yahoo.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#if defined(CONFIG_THREAD_WINDOWS)

#include <windows.h>

struct s_thread_sem_s {
	HANDLE sem_id;
};

#include "thread_mutex_emul.c"

static int s_thread_win32_sem_create (s_thread_sem_t *sem, int initial)
{
	if (sem == NULL) {
		goto err;
	}
	sem->sem_id = CreateSemaphore(NULL, initial, 32 * 1024, NULL);
	if (sem->sem_id == NULL) {
		goto err;
	}
	return 0;
err:	return -1;
}

static int s_thread_win32_sem_destroy (s_thread_sem_t *sem)
{
	if ((sem == NULL) || (sem->sem_id < 0)) {
		goto err;
	}
	CloseHandle(sem->sem_id);
	
	sem->sem_id = 0;
	return 0;
err:	return -1;
}

static int s_thread_win32_sem_wait_timeout (s_thread_sem_t *sem, int msec)
{
	int ret;
	DWORD dwMilliseconds;

	if ((sem == NULL) || (sem->sem_id < 0)) {
		return -1;
	}
	if (msec == (~(int) 0)) {
		dwMilliseconds = INFINITE;
	} else {
		dwMilliseconds = (DWORD) msec;
	}

	ret = WaitForSingleObject(sem->sem_id, dwMilliseconds);
	switch (ret) {
		case WAIT_OBJECT_0:
			return 0;
		case WAIT_TIMEOUT:
			return 1;
		default:
			return -1;
	}
}

static int s_thread_win32_sem_wait (s_thread_sem_t *sem)
{
	int ret;
	if ((sem == NULL) || (sem->sem_id < 0)) {
		goto err;
	}
	ret = s_thread_win32_sem_wait_timeout(sem, INFINITE);
	if (ret < 0) {
		goto err;
	}
	return 0;
err:	return -1;
}

static int s_thread_win32_sem_post (s_thread_sem_t *sem)
{
	if ((sem == NULL) || (sem->sem_id < 0)) {
		goto err;
	}
	if (ReleaseSemaphore(sem->sem_id, 1, NULL) == FALSE) {
		goto err;
	}
	return 0;
err:	return -1;
}

struct s_thread_s {
	HANDLE tid;
};

static DWORD WINAPI ThreadEntry (LPVOID argp)
{
	s_thread_arg_t *targs;
	targs = (s_thread_arg_t *) argp;
	trags->r(argp);
	return 0;
}

static int s_thread_win32_thread_create (s_thread_t *tid, s_thread_arg_t *targs)
{
	DWORD threadnum;
    
	tid->tid = CreateThread(NULL, 0, ThreadEntry, targs, 0, &threadnum);
	if (tid->tid == NULL) {
		goto err;
	}

	return 0;
err:	return -1;
}

static int s_thread_win32_thread_cancel (s_thread_t *tid)
{
	return -1;
}

static int s_thread_win32_thread_join (s_thread_t *tid, void **ret)
{
	WaitForSingleObject(tid->tid, INFINITE);
	CloseHandle(tid->tid);
	return 0;
}

static int s_thread_win32_thread_self (void)
{
	return (GetCurrentThreadId());
}

static void s_thread_win32_thread_exit (void *ret)
{
	ExitThread(0);
}

static s_thread_api_t s_thread_win32 = {
	s_thread_win32_sem_create,
	s_thread_win32_sem_destroy,
	s_thread_win32_sem_wait,
	s_thread_win32_sem_wait_timeout,
	s_thread_win32_sem_post,

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

	s_thread_win32_thread_create,
	s_thread_win32_thread_cancel,
	s_thread_win32_thread_join,
	s_thread_win32_thread_self,
	s_thread_win32_thread_exit,
};

#endif /* CONFIG_THREAD_WINDOWS */
