/***************************************************************************
    begin                : Wed Oct 8 2003
    copyright            : (C) 2003 - 2009 by Alper Akcan
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

#include "../../../lib/xynth_.h"
#include "server.h"
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

s_video_input_t s_video_fb_input_irr = {
	VIDEO_INPUT_IRR,
	s_video_helper_irr_init,
	s_video_helper_irr_update,
	s_video_helper_irr_uninit,
};

#if defined(CONFIG_VIDEO_HELPER_TSCREEN)
s_video_input_t s_video_fb_input_tscreen = {
	VIDEO_INPUT_MOUSE,
	s_video_helper_touchscreen_init,
	s_video_helper_touchscreen_update,
	s_video_helper_touchscreen_uninit,
};
#endif

s_video_input_t *s_video_fb_input[] = {
	&s_video_fb_input_irr,
	&s_video_fb_input_keybd,
	&s_video_fb_input_mouse,
#if defined(CONFIG_VIDEO_HELPER_TSCREEN)
	&s_video_fb_input_tscreen,
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
	s_server_surface_lock();
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
        xynth_server->window->surface->width = fb.v_scr.xres;
        xynth_server->window->surface->height = fb.v_scr.yres;
        xynth_server->window->surface->bitsperpixel = fb.bitsperpixel;
        xynth_server->window->surface->bytesperpixel = fb.bytesperpixel;
	xynth_server->window->surface->blueoffset = fb.v_scr.blue.offset;
	xynth_server->window->surface->greenoffset = fb.v_scr.green.offset;
	xynth_server->window->surface->redoffset = fb.v_scr.red.offset;
	xynth_server->window->surface->bluelength = fb.v_scr.blue.length;
	xynth_server->window->surface->greenlength = fb.v_scr.green.length;
	xynth_server->window->surface->redlength = fb.v_scr.red.length;
	xynth_server->window->surface->colors = fb.colors;

        switch (xynth_server->window->surface->colors) {
		default:
		case 256:
			xynth_server->window->surface->bitsperpixel = 8;
			xynth_server->window->surface->blueoffset = 0;
			xynth_server->window->surface->greenoffset = 3;
			xynth_server->window->surface->redoffset = 6;
			xynth_server->window->surface->bluelength = 3;
			xynth_server->window->surface->greenlength = 3;
			xynth_server->window->surface->redlength = 2;
			break;
		case 32768:
			xynth_server->window->surface->bitsperpixel = 15;
			xynth_server->window->surface->blueoffset = 0;
			xynth_server->window->surface->greenoffset = 5;
			xynth_server->window->surface->redoffset = 10;
			xynth_server->window->surface->bluelength = 5;
			xynth_server->window->surface->greenlength = 5;
			xynth_server->window->surface->redlength = 5;
			break;
		case 65536:
			xynth_server->window->surface->bitsperpixel = 16;
			xynth_server->window->surface->blueoffset = 0;
			xynth_server->window->surface->greenoffset = 5;
			xynth_server->window->surface->redoffset = 11;
			xynth_server->window->surface->bluelength = 5;
			xynth_server->window->surface->greenlength = 6;
			xynth_server->window->surface->redlength = 5;
			break;
		case (1 << 18):
                        xynth_server->window->surface->bitsperpixel = 18;
                        xynth_server->window->surface->blueoffset = 0;
                        xynth_server->window->surface->greenoffset = 6;
                        xynth_server->window->surface->redoffset = 12;
                        xynth_server->window->surface->bluelength = 6;
                        xynth_server->window->surface->greenlength = 6;
                        xynth_server->window->surface->redlength = 6;
                        break;
		case 256 * 65536:
			xynth_server->window->surface->bitsperpixel = xynth_server->window->surface->bytesperpixel * 8;
			xynth_server->window->surface->blueoffset = 0;
			xynth_server->window->surface->greenoffset = 8;
			xynth_server->window->surface->redoffset = 16;
			xynth_server->window->surface->bluelength = 8;
			xynth_server->window->surface->greenlength = 8;
			xynth_server->window->surface->redlength = 8;
			break;
	}

	xynth_server->window->surface->linear_mem_base = 0;
	xynth_server->window->surface->linear_mem_size = fb.f_scr.smem_len;

	if (xynth_server->window->surface->linear_mem_size){
		xynth_server->window->surface->linear_mem_base = (unsigned int) mmap((caddr_t) 0, xynth_server->window->surface->linear_mem_size,
						                               PROT_WRITE | PROT_READ,
						                               MAP_SHARED,
						                               fb.fd,
						                               (off_t) xynth_server->window->surface->linear_mem_base);
		if ((int) xynth_server->window->surface->linear_mem_base == -1) {
			debugf(DSER | DSYS | DFAT, "FBDEV : mmap failed");
		}
	} else {
		debugf(DSER | DFAT, "FBDEV : mmap failed");
	}

	xynth_server->window->surface->vbuf = (unsigned char *) xynth_server->window->surface->linear_mem_base;
	xynth_server->window->surface->linear_buf = xynth_server->window->surface->vbuf;
	xynth_server->window->surface->linear_mem_base = 0;

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
	munmap((void *) xynth_server->window->surface->linear_mem_base, xynth_server->window->surface->linear_mem_size);
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
