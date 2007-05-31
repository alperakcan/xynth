/***************************************************************************
    begin                : Tue Oct 14 2003
    copyright            : (C) 2003 - 2007 by Alper Akcan
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
	#include <pspdebug.h>
	/* for psplink
	#define printf	pspDebugScreenPrintf
	*/
#endif

void s_debug_debugf (unsigned short flags, char *file, int line, char *func, char *fmt, ...)
{
	int n;
	int s;
	char *p;
	va_list args;

#if !defined(DEBUG)
	if ((flags & DFAT) == 0) {
		return;
	}
#endif
	printf("[0x%08X] ", s_thread_self());

	if (flags & DFAT) { printf("FATAL : ");   }
	if (flags & DSYS) { printf("SYSERR : ");  }
	if (flags & DSER) { printf("SERVER :: "); }
	if (flags & DCLI) { printf("CLIENT :: "); }
	
	s = 100;
	if ((p = s_malloc(sizeof(char) * s)) == NULL) {
		goto err;
	}

	while (1) {
		va_start(args, fmt);
		n = vsnprintf(p, s, fmt, args);
		va_end(args);
		if (n > -1 && n < s) {
			break;
		}
		if (n > -1) {
			s = n + 1;
		} else {
			s *= 2;
		}
		if ((p = s_realloc(p, s))  == NULL) {
			goto err;
		}
	}

	printf(p);
	s_free(p);

	if (flags & DSYS) {
		printf(" : %s", strerror(errno));
	}
	printf(" [%s (%s:%d)]\n", func, file, line);
	if (flags & DFAT) {
		goto err;
	}
	return;
err:	exit(1);
}
