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

#include "xynth_.h"

#if defined(PLATFORM_PSPDEV)

#include <pspkernel.h>
#include <pspthreadman.h>
#include <pspmoduleinfo.h>

void usleep (unsigned long msec)
{
	unsigned long delay;
	delay = MIN(msec, 0xffffffffUL);
	sceKernelDelayThreadCB(delay);
}

unsigned int sleep (unsigned int sec)
{
	usleep(sec * 1000 * 1000);
	return 0;
}

#elif defined(PLATFORM_MINGW)

#include <windows.h>
#include <time.h>

#define WIN32_LEAN_AND_MEAN

#ifndef __GNUC__
#define EPOCHFILETIME (116444736000000000i64)
#else
#define EPOCHFILETIME (116444736000000000LL)
#endif

void usleep (unsigned long msec)
{
	Sleep(msec);
}

unsigned int sleep (unsigned int sec)
{
	usleep(sec * 1000 * 1000);
	return 0;
}

#endif

long long s_gettimeofday (void)
{
	long long tsec;
	long long tusec;
	struct timeval tv;
	if (gettimeofday(&tv, NULL) < 0) {
		return -1;
	}
	tsec = ((long long) tv.tv_sec) * 1000;
	tusec = ((long long) tv.tv_usec) / 1000;
	return tsec + tusec;
}
