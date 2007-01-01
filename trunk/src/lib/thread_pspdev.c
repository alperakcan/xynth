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

#if defined(THREAD_PSPDEV)

#include <pspkernel.h>
#include <pspkerneltypes.h>
#include <pspmoduleinfo.h>
#include <pspthreadman.h>
#include <pspkerror.h>

struct s_thread_sem_s {
	int sem_id;
};

#include "thread_mutex_emul.c"

static int s_thread_pspdev_sem_create (s_thread_sem_t *sem, int initial)
{
	if (sem == NULL) {
		goto err;
	}
	sem->sem_id = sceKernelCreateSema("Xynth Sema", 0, initial, 255, NULL);
	if (sem->sem_id < 0) {
		goto err;
	}
	return 0;
err:	return -1;
}

static int s_thread_pspdev_sem_destroy (s_thread_sem_t *sem)
{
	int ret;
	if ((sem == NULL) || (sem->sem_id < 0)) {
		goto err;
	}
	ret = sceKernelDeleteSema(sem->sem_id);
	if (ret < 0) {
	}
	sem->sem_id = -1;
	return 0;
err:	return -1;
}

static int s_thread_pspdev_sem_wait (s_thread_sem_t *sem)
{
	int ret;
	if ((sem == NULL) || (sem->sem_id < 0)) {
		goto err;
	}
	ret = sceKernelWaitSemaCB(sem->sem_id, 1, NULL);
	if (ret < 0) {
		goto err;
	}
	return 0;
err:	return -1;
}

static int s_thread_pspdev_sem_wait_timeout (s_thread_sem_t *sem, int msec)
{
	int ret;
	unsigned int psec;
	unsigned int *ptout;

	if ((sem == NULL) || (sem->sem_id < 0)) {
		return -1;
	}
	psec = (unsigned int) msec;
	psec *= 1000;
	ptout = &psec;
	ret = sceKernelWaitSemaCB(sem->sem_id, 1, ptout);
	switch (ret) {
		case SCE_KERNEL_ERROR_OK:
			return 0;
		case SCE_KERNEL_ERROR_WAIT_TIMEOUT:
			return 1;
		default:
			return -1;
	}
}

static int s_thread_pspdev_sem_post (s_thread_sem_t *sem)
{
	if ((sem == NULL) || (sem->sem_id < 0)) {
		goto err;
	}
	if (sceKernelSignalSema(sem->sem_id, 1)) {
		goto err;
	}
	return 0;
err:	return -1;
}

static int ThreadEntry (SceSize args, void *argp)
{
	s_thread_arg_t *targs;
	targs = (s_thread_arg_t *) (*(void **) argp);
	targs->r(*(void **) argp);
	return 0;
}

struct s_thread_s {
	SceUID tid;
};

static int s_thread_pspdev_thread_create (s_thread_t *tid, s_thread_arg_t *targs)
{
	int priority = 32;
	SceKernelThreadInfo status;

	status.size = sizeof(SceKernelThreadInfo);
	if (sceKernelReferThreadStatus(sceKernelGetThreadId(), &status) == 0) {
		priority = status.currentPriority;
	}
	tid->tid = sceKernelCreateThread("Xynth Thread", ThreadEntry, priority, 0x8000, PSP_THREAD_ATTR_USER, NULL);
	if (tid->tid < 0) {
		goto err;
	}
	sceKernelStartThread(tid->tid, 4, &targs);
	return 0;
err:	return -1;
}

static int s_thread_pspdev_thread_cancel (s_thread_t *tid)
{
	return -1;
}

static int s_thread_pspdev_thread_join (s_thread_t *tid, void **ret)
{
	sceKernelWaitThreadEnd(tid->tid, NULL);
	sceKernelDeleteThread(tid->tid);
	return 0;
}

static int s_thread_pspdev_thread_self (void)
{
	return (int) sceKernelGetThreadId();
}

static void s_thread_pspdev_thread_exit (void *ret)
{
	sceKernelExitThread(0);
}

static s_thread_api_t s_thread_pspdev = {
	s_thread_pspdev_sem_create,
	s_thread_pspdev_sem_destroy,
	s_thread_pspdev_sem_wait,
	s_thread_pspdev_sem_wait_timeout,
	s_thread_pspdev_sem_post,

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

	s_thread_pspdev_thread_create,
	s_thread_pspdev_thread_cancel,
	s_thread_pspdev_thread_join,
	s_thread_pspdev_thread_self,
	s_thread_pspdev_thread_exit,
};

#endif /* THREAD_PSPDEV */
