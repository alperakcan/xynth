/***************************************************************************
    begin                : Thu Oct 9 2003
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

#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "../../../lib/xynth_.h"
#include "server.h"
#include "../helper/helper.h"
#include "fbdev.h"

void armfb_setpalette (int index, int red, int green, int blue)
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
	ioctl(armfb.fd, FBIOPUTCMAP, &cmap);
}

void armfb_getvaro (void)
{
	if (ioctl(armfb.fd, FBIOGET_VSCREENINFO, &armfb.v_scro) == -1) {
		debugf(DSER | DSYS | DFAT, "FBDEV : Unable to get variable screen info");
	}
}

void armfb_putvaro (void)
{
        if (armfb.fd == -1) {
		return;
	}
	if (ioctl(armfb.fd, FBIOPUT_VSCREENINFO, &armfb.v_scro) == -1) {
		debugf(DSER | DSYS | DFAT, "FBDEV : Unable to put variable screen info");
	}
	armfb_getvaro();
}

void armfb_getfix (void)
{
	if (ioctl(armfb.fd, FBIOGET_FSCREENINFO, &armfb.f_scr) == -1) {
		debugf(DSER | DSYS | DFAT, "FBDEV : Unable to get fixed screen info");
	}
}

void armfb_getvar (void)
{
	if (ioctl(armfb.fd, FBIOGET_VSCREENINFO, &armfb.v_scr) == -1) {
		debugf(DSER | DSYS | DFAT, "FBDEV : Unable to get variable screen info");
	}
}

void armfb_putvar (void)
{
	if (ioctl(armfb.fd, FBIOPUT_VSCREENINFO, &armfb.v_scr) == -1) {
		debugf(DSER | DSYS | DFAT, "FBDEV : Unable to put variable screen info");
	}
	armfb_getfix();
	armfb_getvar();
}

void armfb_open (void)
{
	if ((armfb.fd = open(xynth_server->driver->device, O_RDONLY | O_NONBLOCK)) == -1) {
		debugf(DSER | DSYS | DFAT, "FBDEV : Unable to open device (%s)", xynth_server->driver->device);
	}

	armfb_getfix();
	armfb_getvar();

	armfb_getvaro();

	close(armfb.fd);

	if ((armfb.fd = open(xynth_server->driver->device, O_RDWR)) == -1) {
		debugf(DSER | DSYS | DFAT, "FBDEV : Unable to open device (%s)", xynth_server->driver->device);
	}

	armfb_getfix();
	armfb_getvar();
}

void armfb_close (void)
{
	armfb_putvaro();
	close(armfb.fd);
	armfb.fd = -1;
}

void armfb_init (s_server_conf_t *cfg)
{
	armfb_open();
}

#ifdef DEBUG
void armfb_dump_vscr (struct fb_var_screeninfo *vscr)
{
	printf("xres: %u\n", vscr->xres);
	printf("yres: %u\n", vscr->yres);
	printf("xres_virtual: %u\n", vscr->xres_virtual);
	printf("yres_virtual: %u\n", vscr->yres_virtual);
	printf("xoffset: %u\n", vscr->xoffset);
	printf("yoffset: %u\n", vscr->yoffset);
	printf("bits_per_pixel: %u\n", vscr->bits_per_pixel);
	printf("grayscale: %u\n", vscr->grayscale);
	printf("nonstd: %u\n", vscr->nonstd);
	printf("red.offset: %u\n", vscr->red.offset);
	printf("red.length: %u\n", vscr->red.length);
	printf("red.msb_right: %u\n", vscr->red.msb_right);
	printf("green.offset: %u\n", vscr->green.offset);
	printf("green.length: %u\n", vscr->green.length);
	printf("green.msb_right: %u\n", vscr->green.msb_right);
	printf("blue.offset: %u\n", vscr->blue.offset);
	printf("blue.length: %u\n", vscr->blue.length);
	printf("blue.msb_right: %u\n", vscr->blue.msb_right);
	printf("transp.offset: %u\n", vscr->transp.offset);
	printf("transp.length: %u\n", vscr->transp.length);
	printf("transp.msb_right: %u\n", vscr->transp.msb_right);
	printf("activate: %u\n", vscr->activate);
	printf("height: %u\n", vscr->height);
	printf("width: %u\n", vscr->width);
	printf("accel_flags: %u\n", vscr->accel_flags);
	printf("pixclock: %u\n", vscr->pixclock);
	printf("left_margin: %u\n", vscr->left_margin);
	printf("right_margin: %u\n", vscr->right_margin);
	printf("upper_margin: %u\n", vscr->upper_margin);
	printf("lower_margin: %u\n", vscr->lower_margin);
	printf("hsync_len: %u\n", vscr->hsync_len);
	printf("vsync_len: %u\n", vscr->vsync_len);
	printf("sync: %u\n", vscr->sync);
	printf("vmode: %u\n", vscr->vmode);
}
#endif
