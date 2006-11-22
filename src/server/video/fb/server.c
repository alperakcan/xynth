/***************************************************************************
    begin                : Wed Oct 8 2003
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

#if defined(VIDEO_FBDev)

#include "../../../lib/xynth_.h"
#include "../../server.h"
#include "../helper/helper.h"
#include "fbdev.h"

#include <sys/mman.h>

s_video_input_t s_video_fb_input_keybd = {
	VIDEO_INPUT_KEYBD,
	fb_kbd_init,
	s_video_helper_kbd_update,
	s_video_helper_kbd_uninit,
};

s_video_input_t s_video_fb_input_mouse = {
	VIDEO_INPUT_MOUSE,
	s_video_helper_mouse_init,
	s_video_helper_mouse_update,
	s_video_helper_mouse_uninit,
};

#if defined(VIDEO_HELPER_TSCREEN)
s_video_input_t s_video_fb_input_tscreen = {
	VIDEO_INPUT_MOUSE,
	s_video_helper_touchscreen_init,
	s_video_helper_touchscreen_update,
	s_video_helper_touchscreen_uninit,
};
#endif

#if defined(VIDEO_HELPER_IRSERIAL)
#include <termios.h>

typedef struct irserial_codes_s {
	char *code;
	s_video_input_data_keybd_t keybd;
} irserial_codes_t;

static irserial_codes_t irserial_codes_sonytv[] = {
	{"\r\n0094", {KEYBD_PRESSED, 0, 0, 0, 0}}, /* muting */
	{"\r\n00b6", {KEYBD_PRESSED, 0, 0, 0, 0}}, /* sleep */
	{"\r\n0095", {KEYBD_PRESSED, 0, 0, 0, 0}}, /* power */
	{"\r\n00ba", {KEYBD_PRESSED, 0, 0, 0, 0}}, /* display */
	{"\r\n00a5", {KEYBD_PRESSED, 0, 0, 0, 0}}, /* tv/video */
	{"\r\n0080", {KEYBD_PRESSED, 0, 0, 0, 0}}, /* 1 */
	{"\r\n0081", {KEYBD_PRESSED, 0, 0, 0, 0}}, /* 2 */
	{"\r\n0082", {KEYBD_PRESSED, 0, 0, 0, 0}}, /* 3 */
	{"\r\n0083", {KEYBD_PRESSED, 0, 0, 0, 0}}, /* 4 */
	{"\r\n0084", {KEYBD_PRESSED, 0, 0, 0, 0}}, /* 5 */
	{"\r\n0085", {KEYBD_PRESSED, 0, 0, 0, 0}}, /* 6 */
	{"\r\n0086", {KEYBD_PRESSED, 0, 0, 0, 0}}, /* 7 */
	{"\r\n0087", {KEYBD_PRESSED, 0, 0, 0, 0}}, /* 8 */
	{"\r\n0088", {KEYBD_PRESSED, 0, 0, 0, 0}}, /* 9 */
	{"\r\n00bb", {KEYBD_PRESSED, 0, 0, 0, 0}}, /* jump */
	{"\r\n0089", {KEYBD_PRESSED, 0, 0, 0, 0}}, /* 0 */
	{"\r\n008b", {KEYBD_PRESSED, 0, 0, 0, 0}}, /* enter */
	{"\r\n0092", {KEYBD_PRESSED, 0, 0, 0, 0}}, /* vol + */
	{"\r\n0090", {KEYBD_PRESSED, 0, 0, 0, 0}}, /* chan + */
	{"\r\n0093", {KEYBD_PRESSED, 0, 0, 0, 0}}, /* vol - */
	{"\r\n0091", {KEYBD_PRESSED, 0, 0, 0, 0}}, /* chan - */
	{"\r\n0096", {KEYBD_PRESSED, 0, 0, 0, 0}}, /* reset */
	{"\r\n00e0", {KEYBD_PRESSED, 0, 0, 0, 0}}, /* menu */
	{"\r\n00f4", {KEYBD_PRESSED, 0, 0, 0, 0}}, /* up */
	{"\r\n00b4", {KEYBD_PRESSED, S_KEYCODE_TAB, S_KEYCODE_TAB, 0, 0}}, /* left */
	{"\r\n00e5", {KEYBD_PRESSED, S_KEYCODE_SPACE, S_KEYCODE_SPACE, 0, 0}}, /* ok */
	{"\r\n00b3", {KEYBD_PRESSED, 0, 0, 0, 0}}, /* right */
	{"\r\n00f5", {KEYBD_PRESSED, 0, 0, 0, 0}}, /* down */
	{NULL, {0, 0, 0, 0, 0}},
};

int s_video_fbdev_irserial_init (s_server_conf_t *cfg)
{
	int fd;
	fd = irserial_init("/dev/ttyS2", B2400);
	return fd;
}

int s_video_fbdev_irserial_update (s_video_input_data_t *keybd)
{
	char *code;
	code = irserial_getcode();
	if (code) {
		irserial_codes_t *codes;
		codes = &irserial_codes_sonytv[0];
		while (codes->code != NULL) {
			if (strcmp(codes->code, code) == 0) {
				keybd->keybd.state = codes->keybd.state;
				keybd->keybd.button = codes->keybd.button;
				keybd->keybd.keycode = codes->keybd.keycode;
				return 1;
			}
			codes++;
		}
	}
	return 0;
}

void s_video_fbdev_irserial_uninit (void)
{
	irserial_uninit();
}

s_video_input_t s_video_fb_input_irserial = {
	VIDEO_INPUT_KEYBD,
	s_video_fbdev_irserial_init,
	s_video_fbdev_irserial_update,
	s_video_fbdev_irserial_uninit,
};
#endif


s_video_input_t *s_video_fb_input[] = {
	&s_video_fb_input_keybd,
	&s_video_fb_input_mouse,
#if defined(VIDEO_HELPER_TSCREEN)
	&s_video_fb_input_tscreen,
#endif
#if defined(VIDEO_HELPER_IRSERIAL)
	&s_video_fb_input_irserial,
#endif
	NULL,
};

s_video_driver_t s_video_fbdev = {
	"fbdev",
	"/dev/fb0",
	s_video_fb_input,
	fb_server_init,
	fb_server_uninit,
	fb_server_goto_back,
	fb_server_comefrom_back,
	fb_server_restore,
	NULL,
	NULL,
	NULL
};

void fb_server_restore (void)
{
	fb_close();
}

void fb_server_goto_back (void)
{
	s_server_surface_lock_real();
	fb_putvaro();
}

void fb_server_comefrom_back (void)
{
	fb_putvar();
	s_server_surface_refresh();
	s_video_helper_kbd_set_attr();
}

void fb_server_mtrr_init (void)
{
}

void fb_server_surface_setrgbpalette (void)
{
	int i;
	int r;
	int g;
	int b;

	for (i = 0; i < 256; i++) {
		b = (i & 7) * (64 / 8);		/* 3 bits */
		g = ((i & 56) >> 3) * (64 / 8);	/* 3 bits */
		r = ((i & 192) >> 6) * (64 / 4);/* 2 bits */
		fb_setpalette(i, r, g, b);
	}
}

void fb_server_surface_init (s_video_helper_mode_info_t *gmode)
{
        server->window->surface->width = fb.v_scr.xres;
        server->window->surface->height = fb.v_scr.yres;
        server->window->surface->bitsperpixel = fb.bitsperpixel;
        server->window->surface->bytesperpixel = fb.bytesperpixel;
	server->window->surface->blueoffset = fb.v_scr.blue.offset;
	server->window->surface->greenoffset = fb.v_scr.green.offset;
	server->window->surface->redoffset = fb.v_scr.red.offset;
	server->window->surface->bluelength = fb.v_scr.blue.length;
	server->window->surface->greenlength = fb.v_scr.green.length;
	server->window->surface->redlength = fb.v_scr.red.length;
	server->window->surface->colors = fb.colors;

        switch (server->window->surface->colors) {
		default:
		case 256:
			server->window->surface->bitsperpixel = 8;
			server->window->surface->blueoffset = 0;
			server->window->surface->greenoffset = 3;
			server->window->surface->redoffset = 6;
			server->window->surface->bluelength = 3;
			server->window->surface->greenlength = 3;
			server->window->surface->redlength = 2;
			break;
		case 32768:
			server->window->surface->bitsperpixel = 15;
			server->window->surface->blueoffset = 0;
			server->window->surface->greenoffset = 5;
			server->window->surface->redoffset = 10;
			server->window->surface->bluelength = 5;
			server->window->surface->greenlength = 5;
			server->window->surface->redlength = 5;
			break;
		case 65536:
			server->window->surface->bitsperpixel = 16;
			server->window->surface->blueoffset = 0;
			server->window->surface->greenoffset = 5;
			server->window->surface->redoffset = 11;
			server->window->surface->bluelength = 5;
			server->window->surface->greenlength = 6;
			server->window->surface->redlength = 5;
			break;
		case (1 << 18):
                        server->window->surface->bitsperpixel = 18;
                        server->window->surface->blueoffset = 0;
                        server->window->surface->greenoffset = 6;
                        server->window->surface->redoffset = 12;
                        server->window->surface->bluelength = 6;
                        server->window->surface->greenlength = 6;
                        server->window->surface->redlength = 6;
                        break;
		case 256 * 65536:
			server->window->surface->bitsperpixel = server->window->surface->bytesperpixel * 8;
			server->window->surface->blueoffset = 0;
			server->window->surface->greenoffset = 8;
			server->window->surface->redoffset = 16;
			server->window->surface->bluelength = 8;
			server->window->surface->greenlength = 8;
			server->window->surface->redlength = 8;
			break;
	}

	server->window->surface->linear_mem_base = 0;
	server->window->surface->linear_mem_size = fb.f_scr.smem_len;

	if (server->window->surface->linear_mem_size){
		server->window->surface->linear_mem_base = (unsigned int) mmap((caddr_t) 0, server->window->surface->linear_mem_size,
						                               PROT_WRITE | PROT_READ,
						                               MAP_SHARED,
						                               fb.fd,
						                               (off_t) server->window->surface->linear_mem_base);
		if ((int) server->window->surface->linear_mem_base == -1) {
			debugf(DSER | DSYS | DFAT, "FBDEV : mmap failed");
		}
	} else {
		debugf(DSER | DFAT, "FBDEV : mmap failed");
	}

	server->window->surface->vbuf = (char *) server->window->surface->linear_mem_base;
	server->window->surface->linear_buf = server->window->surface->vbuf;
	server->window->surface->linear_mem_base = 0;

        fb_server_surface_setrgbpalette();
}

int fb_server_init (s_server_conf_t *cfg)
{
	s_video_helper_mode_info_t *gmode;
	
	if (s_video_helper_mode_find(cfg, &gmode)) {
		debugf(DSER, "Mode %s cannot be found", cfg->general.mode);
		goto err0;
	}
        
	fb_init(cfg);

	fb_server_setmode(cfg, gmode);
	fb_server_surface_init(gmode);
	fb_server_mtrr_init();
	return gmode->number;

err0:	return -1;
}

void fb_server_surface_uninit (void)
{
	munmap((void *) server->window->surface->linear_mem_base, server->window->surface->linear_mem_size);
}

void fb_server_uninit (void)
{
	fb_server_surface_uninit();
//	fb_setmode(0);
}

void fb_server_setmode (s_server_conf_t *cfg, s_video_helper_mode_info_t *gmode)
{
	fb.v_scr.xres = gmode->xdim;
	fb.v_scr.yres = gmode->ydim;
	fb.v_scr.xres_virtual = gmode->xdim;
	fb.v_scr.yres_virtual = gmode->ydim;
	fb.v_scr.xoffset = 0;
	fb.v_scr.yoffset = 0;
	fb.v_scr.bits_per_pixel = gmode->bytesperpixel * 8;
	fb.v_scr.grayscale = 0;
	fb.v_scr.nonstd = 0;

	switch (gmode->colors) {
		default:
		case 256:
			fb.bitsperpixel = 8;
			fb.v_scr.blue.offset = 0;
			fb.v_scr.green.offset = 3;
			fb.v_scr.red.offset = 6;
			fb.v_scr.blue.length = 3;
			fb.v_scr.green.length = 3;
			fb.v_scr.red.length = 2;
			break;
		case 32768:
			fb.bitsperpixel = 15;
			fb.v_scr.blue.offset = 0;
			fb.v_scr.green.offset = 5;
			fb.v_scr.red.offset = 10;
			fb.v_scr.blue.length = 5;
			fb.v_scr.green.length = 5;
			fb.v_scr.red.length = 5;
			break;
		case 65536:
			fb.bitsperpixel = 16;
			fb.v_scr.blue.offset = 0;
			fb.v_scr.green.offset = 5;
			fb.v_scr.red.offset = 11;
			fb.v_scr.blue.length = 5;
			fb.v_scr.green.length = 6;
			fb.v_scr.red.length = 5;
			break;
		case  (1 << 18):
			fb.bitsperpixel = 18;
			fb.v_scr.bits_per_pixel = 18;
			fb.v_scr.blue.offset = 0;
			fb.v_scr.green.offset = 6;
			fb.v_scr.red.offset = 12;
			fb.v_scr.blue.length = 6;
			fb.v_scr.green.length = 6;
			fb.v_scr.red.length = 6;
			break;
		case 256 * 65536:
			fb.bitsperpixel = 24;
			fb.v_scr.blue.offset = 0;
			fb.v_scr.green.offset = 8;
			fb.v_scr.red.offset = 16;
			fb.v_scr.blue.length = 8;
			fb.v_scr.green.length = 8;
			fb.v_scr.red.length = 8;
			break;
	}
	fb.bytesperpixel = gmode->bytesperpixel;
	fb.colors = gmode->colors;

	fb.v_scr.red.msb_right = 0;
	fb.v_scr.green.msb_right = 0;
	fb.v_scr.blue.msb_right = 0;

	fb_timing_set(cfg, &(fb.v_scr));

#if 0
	void fb_dump_vscr (struct fb_var_screeninfo *vscr);
	fb_dump_vscr(&(fb.v_scr));
	fb_dump_vscr(&(fb.v_scro));
#endif

	fb_putvar();

	switch (fb.f_scr.type) {
		case FB_TYPE_PACKED_PIXELS:
			break;
		default :
			debugf(DSER | DSYS | DFAT, "FBDEV : Unsupported type(%d) requested", fb.f_scr.type);
	}

	switch (fb.f_scr.visual) {
		case FB_VISUAL_TRUECOLOR:
		case FB_VISUAL_DIRECTCOLOR:
			break;
		case FB_VISUAL_PSEUDOCOLOR:
			break;
		default :
			debugf(DSER | DSYS | DFAT, "FBDEV : Unsupported visual(%d) requested", fb.f_scr.visual);
	}
}

#endif /* VIDEO_FBDev */
