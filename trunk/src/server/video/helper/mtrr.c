/***************************************************************************
    begin                : Sat Sep 27 2005
    copyright            : (C) 2005 - 2009 by Alper Akcan
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

#include "config.h"

#if defined(CONFIG_VIDEO_HELPER_MTRR)

#include <fcntl.h>
#include <sys/ioctl.h>
#include <asm/mtrr.h>

#include "../../../lib/xynth_.h"
#include "server.h"
#include "helper.h"

void s_video_helper_mtrr_add (unsigned int base, unsigned int size)
{
        int fd;
	struct mtrr_sentry sentry;
	
	sentry.base = base;
	sentry.size = size;
	sentry.type = MTRR_TYPE_WRCOMB;

	if ((fd = open("/proc/mtrr", O_WRONLY, 0)) == -1) {
		debugf(DSER | DSYS, "/proc/mtrr not found: not supported or you don't have a PPro?");
		return;
	}

	if (ioctl(fd, MTRRIOC_ADD_ENTRY, &sentry) == -1) {
		debugf(DSER | DSYS, "Error doing ioctl on /proc/mtrr");
	}
	
	close(fd);
}

#endif /* CONFIG_VIDEO_HELPER_MTRR */
