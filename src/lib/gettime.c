/***************************************************************************
    begin                : Thu Mar 25 2004
    copyright            : (C) 2004 - 2006 by Alper Akcan
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

unsigned int usleep (unsigned long msec)
{
	unsigned long delay;
	delay = MIN(msec, 0xffffffffUL);
	sceKernelDelayThreadCB(delay);
	return 0;
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

struct timezone {
	int tz_minuteswest;
	int tz_dsttime;
};

unsigned int usleep (unsigned long msec)
{
	Sleep(msec);
	return 0;
}

unsigned int sleep (unsigned int sec)
{
	usleep(sec * 1000 * 1000);
	return 0;
}

int gettimeofday (struct timeval *tv, struct timezone *tz)
{
	FILETIME ft;
	LARGE_INTEGER li;
	__int64 t;
	static int tzflag;
	
	if (tv) {
		GetSystemTimeAsFileTime(&ft);
		li.LowPart = ft.dwLowDateTime;
		li.HighPart = ft.dwHighDateTime;
		t = li.QuadPart;
		t -= EPOCHFILETIME;
		t /= 10;
		tv->tv_sec = (long) (t / 1000000);
		tv->tv_usec = (long) (t % 1000000);
	}
	if (tz) {
		if (!tzflag) {
			_tzset();
			tzflag++;
		}
		tz->tz_minuteswest = _timezone / 60;
		tz->tz_dsttime = _daylight;
	}
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
