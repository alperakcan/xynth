/***************************************************************************
    begin                : Thu Oct 9 2003
    copyright            : (C) 2003 - 2006 by Alper Akcan
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

#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "../../../lib/xynth_.h"
#include "../../server.h"
#include "../helper/helper.h"
#include "fbdev.h"

int fb_console_fd;

void fb_setpalette (int index, int red, int green, int blue)
{
	unsigned short int r;
	unsigned short int g;
	unsigned short int b;
	unsigned short int t;
	struct fb_cmap cmap;

	r = (red << 10) | (red << 4) | (red >> 2);
	g = (green << 10) | (green << 4) | (green >> 2);
	b = (blue << 10) | (blue << 4) | (blue >> 2);
	t = 0;

	cmap.start = index;
	cmap.len = 1;
	cmap.red = &r;
	cmap.green = &g;
	cmap.blue = &b;
	cmap.transp = &t;
	ioctl(fb.fd, FBIOPUTCMAP, &cmap);
}

void fb_getvaro (void)
{
	if (ioctl(fb.fd, FBIOGET_VSCREENINFO, &fb.v_scro) == -1) {
		debugf(DSER | DSYS | DFAT, "FBDEV : Unable to get variable screen info");
	}
}

void fb_putvaro (void)
{
        if (fb.fd == -1) {
		return;
	}
	if (ioctl(fb.fd, FBIOPUT_VSCREENINFO, &fb.v_scro) == -1) {
		debugf(DSER | DSYS | DFAT, "FBDEV : Unable to put variable screen info");
	}
	fb_getvaro();
}

void fb_getfix (void)
{
	if (ioctl(fb.fd, FBIOGET_FSCREENINFO, &fb.f_scr) == -1) {
		debugf(DSER | DSYS | DFAT, "FBDEV : Unable to get fixed screen info");
	}
}

void fb_getvar (void)
{
	if (ioctl(fb.fd, FBIOGET_VSCREENINFO, &fb.v_scr) == -1) {
		debugf(DSER | DSYS | DFAT, "FBDEV : Unable to get variable screen info");
	}
}

void fb_putvar (void)
{
	if (ioctl(fb.fd, FBIOPUT_VSCREENINFO, &fb.v_scr) == -1) {
		debugf(DSER | DSYS | DFAT, "FBDEV : Unable to put variable screen info");
	}
	fb_getfix();
	fb_getvar();
}

void fb_open (void)
{
	if ((fb.fd = open(server->driver->device, O_RDONLY | O_NONBLOCK)) == -1) {
		debugf(DSER | DSYS | DFAT, "FBDEV : Unable to open device (%s)", server->driver->device);
	}

        fb_getfix();
	fb_getvar();

	fb_getvaro();

	close(fb.fd);

	if ((fb.fd = open(server->driver->device, O_RDWR)) == -1) {
		debugf(DSER | DSYS | DFAT, "FBDEV : Unable to open device (%s)", server->driver->device);
	}

	fb_getfix();
	fb_getvar();
}

void fb_close (void)
{
	fb_putvaro();
	close(fb.fd);
	fb.fd = -1;
}

void fb_init (void)
{
	fb_console_fd = s_video_helper_console_init();
	if (fb_console_fd < 0) {
		debugf(DSER, "Unable to get console");
		return;
	}
	fb_open();
}
