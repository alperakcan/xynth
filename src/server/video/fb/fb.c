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

#if defined(VIDEO_FBDev)

#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "../../../lib/xynth_.h"
#include "server.h"
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

void fb_init (s_server_conf_t *cfg)
{
	if (strcmp(cfg->keyboard.keyboard, "console") == 0) {
		fb_console_fd = s_video_helper_console_init();
		if (fb_console_fd < 0) {
			debugf(DSER, "Unable to get console");
			return;
		}
	}
	fb_open();
}

#if 0
void fb_dump_vscr (struct fb_var_screeninfo *vscr)
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

#endif /* VIDEO_FBDev */
