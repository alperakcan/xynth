/***************************************************************************
    begin                : Thu Jan 22 2004
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <xynth.h>

#include "mplayer.h"
#include "config.h"
#include "aspect.h"
#include "video_out.h"
#include "video_out_internal.h"
#include "osdep/keycodes.h"
#include "osdep/getch2.h"
#include "libmpcodecs/vf_scale.h"

#define XYNTH_DOUBLEBUFFER

static int running;
static s_thread_t *tid;
static s_window_t *window;

#define UNUSED(x) ((void)(x))

static void handler_set (void);
static void atexit_handler (s_window_t *w);
static void atexpose_handler (s_window_t *w);
static uint32_t draw_image (mp_image_t *mpi);
static int query_format (uint32_t format);
static void draw_alpha (int x0, int y0, int w, int h, unsigned char *src, unsigned char *srca, int stride);

static int image_x;
static int image_y;
static int image_x_full;
static int image_y_full;
static int image_width;
static int image_height;
static int image_width_full;
static int image_height_full;
static int image_aspect;
static int image_aspect_full;
static struct SwsContext *image_sws;
static uint8_t *image_scale_buf;
static int image_fullscreen;
static int image_format;

static vo_info_t info = {
	"Xynth Windowing System",
	"xynth",
	"Alper Akcan <distchx@yahoo.com>",
	""
};

LIBVO_EXTERN(xynth);

static int preinit(const char *arg)
{
	getch2_disable();
	
	if (s_window_init(&window)) {
		printf("%s:%d\n", __FILE__, __LINE__);
		exit(0);
	}
	
	s_window_atexit(window, atexit_handler);
	
	return 0;
}

static int control (uint32_t request, void *data, ...)
{
	switch (request) {
		case VOCTRL_QUERY_FORMAT:
			return query_format(*((uint32_t*) data));
		case VOCTRL_DRAW_IMAGE:
			return draw_image((mp_image_t *) data);
		case VOCTRL_PAUSE:
			window->surface->mode = SURFACE_REAL;
			return VO_TRUE;
		case VOCTRL_RESUME:
			window->surface->mode = SURFACE_VIRTUAL;
			return VO_TRUE;
		case VOCTRL_FULLSCREEN:
			image_fullscreen = (image_fullscreen + 1) % 2;
			if (image_fullscreen) {
				s_window_set_coor(window, WINDOW_NOFORM, 0, 0, window->surface->linear_buf_width, window->surface->linear_buf_height);
				s_window_set_alwaysontop(window, 1);
			} else {
				s_window_set_coor(window, WINDOW_NOFORM, image_x, image_y, image_width, image_height);
				s_window_set_alwaysontop(window, 0);
			}
			s_fillbox(window->surface, 0, 0, window->surface->width, window->surface->height, 0);
			return VO_TRUE;
		/*
		     VOCTRL_GET_IMAGE
		     VOCTRL_RESET
		     VOCTRL_GUISUPPORT
		     VOCTRL_SET_EQUALIZER
		     VOCTRL_GET_EQUALIZER
		     VOCTRL_ONTOP
		*/
	}

	return VO_NOTIMPL;
}

static int config (uint32_t width, uint32_t height, uint32_t d_width,
                   uint32_t d_height, uint32_t flags, char *title,
                   uint32_t format)
{
        int x;
        int y;
	int req_w = (int) width;
	int req_h = (int) height;
	int req_bpp;

	if (!IMGFMT_IS_RGB(format) && !IMGFMT_IS_BGR(format)) {
		assert(0);
		return -1;
	}
	req_bpp = IMGFMT_BGR_DEPTH(format);

	if (vo_dbpp != 0 && vo_dbpp != req_bpp) {
		assert(0);
		return -1;
	}

        if (window->surface->bitsperpixel != req_bpp) {
		exit(0);
	}

	image_width = req_w;
	image_height = req_h;

	image_fullscreen = 0;
	image_format = format;

	image_aspect = ((1 << 16) * image_width + image_height / 2) / image_height;
	image_width_full = window->surface->linear_buf_width;
	image_height_full = window->surface->linear_buf_height;

	image_aspect_full = (image_width_full * (1 << 16) + (image_height_full >> 1)) / image_height_full;
	if (image_aspect_full > image_aspect) {
		image_width_full = (image_height_full * image_aspect + (1 << 15)) >> 16;
	} else {
		image_height_full = ((image_width_full << 16) + (image_aspect >> 1)) / image_aspect;
	}

	image_x_full = (window->surface->linear_buf_width - image_width_full) / 2;
	image_y_full = (window->surface->linear_buf_height - image_height_full) / 2;

	image_scale_buf = (uint8_t *) malloc(image_width_full * image_height_full * window->surface->bytesperpixel);
        image_sws = sws_getContextFromCmdLine(image_width, image_height, image_format, image_width_full, image_height_full, image_format);

	if (vo_config_count <= 0) {
		handler_set();
		s_window_new(window, WINDOW_MAIN, NULL);
		s_window_set_title(window, "Mplayer - %s", filename);
		x = (window->surface->linear_buf_width - (int) image_width) / 2;
		if (x <= 5) x = window->surface->buf->x;
		y = (window->surface->linear_buf_height - (int) image_height) / 2;
		if (y <= 5) y = window->surface->buf->y;
		image_x = x;
		image_y = y;
		s_window_set_coor(window, WINDOW_NOFORM, image_x, image_y, image_width, image_height);
		s_window_set_resizeable(window, 0);
		s_window_show(window);
		s_free(window->surface->vbuf);
		window->surface->width = window->surface->linear_buf_width;
		window->surface->height = window->surface->linear_buf_height;
		window->surface->vbuf = (char *) s_malloc(window->surface->width *
		                                          window->surface->height *
		                                          window->surface->bytesperpixel);
		tid = s_thread_create(s_window_main, window);
		running = 1;

#if defined(XYNTH_DOUBLEBUFFER)
		/* tricky hack for double buffering */
		window->surface->mode = SURFACE_VIRTUAL;
#endif
	}

	return 0;
}

static int draw_slice (uint8_t *src[], int stride[], int w, int h, int x, int y)
{
	int src_stride[3];
	int dst_stride[3];
	uint8_t *src_rgb[3];
	uint8_t *dst_rgb[3];
	struct SwsContext *sws;

	if (!running) {
		return VO_TRUE;
	}

	if (image_fullscreen == 0) {
		return VO_ERROR;
	}

	if (x != 0 ||
	    y != 0 ||
	    w != image_width ||
	    h != image_height) {
		printf("ERROR: Is this really happens? (%s:%d)\n", __FILE__, __LINE__);
		return VO_ERROR;
	}

        src_rgb[1] = NULL;
	src_rgb[2] = NULL;
	dst_rgb[1] = NULL;
	dst_rgb[2] = NULL;

        src_stride[1] = 0;
	src_stride[2] = 0;
	dst_stride[1] = 0;
	dst_stride[2] = 0;

	src_rgb[0] = src[0];
	src_stride[0] = window->surface->bytesperpixel * image_width;

	dst_rgb[0] = image_scale_buf;
	dst_stride[0] = window->surface->bytesperpixel * image_width_full;

	sws_scale(image_sws, src_rgb, src_stride, 0, image_height, dst_rgb, dst_stride);

        s_putbox(window->surface, image_x_full, image_y_full, image_width_full, image_height_full, dst_rgb[0]);
        
	return VO_TRUE;
}

static int draw_frame (uint8_t *src[])
{
	assert(0);
	UNUSED(src);
	return VO_ERROR;
}

static void draw_osd (void)
{
	vo_draw_text(image_width, image_height, draw_alpha);
}

static void flip_page (void)
{
	if (!running) {
		return;
	}

#if defined(XYNTH_DOUBLEBUFFER)
	window->surface->mode = SURFACE_REAL;
	s_putbox(window->surface, 0, 0, window->surface->width, window->surface->height, window->surface->vbuf);
	window->surface->mode = SURFACE_VIRTUAL;
#endif
}

static void check_events (void)
{
}

static void uninit (void)
{
	if (running) {
		s_window_quit(window);
		s_thread_join(tid, NULL);
	}
	running = 0;
}

/*****************************************************************************/

static int query_format (uint32_t format)
{
	int32_t req_bpp;
	int32_t flags;

	// only RGB modes supported
	if((!IMGFMT_IS_RGB(format)) && (!IMGFMT_IS_BGR(format)))
		return 0;
	// Reject different endian
#ifdef WORDS_BIGENDIAN
	if (IMGFMT_IS_BGR(format))
		return 0;
#else
	if (IMGFMT_IS_RGB(format))
		return 0;
#endif
	if ((format == IMGFMT_BGR4) || (format == IMGFMT_RGB4))
		return 0;
	req_bpp = IMGFMT_RGB_DEPTH(format);
	if ((vo_dbpp > 0) && (vo_dbpp != req_bpp))
		return 0;

	if (req_bpp == window->surface->bitsperpixel) {
		flags = VFCAP_CSP_SUPPORTED |
			VFCAP_CSP_SUPPORTED_BY_HW |
			VFCAP_ACCEPT_STRIDE |
			VFCAP_SWSCALE;
		if (req_bpp > 8)
			flags |= VFCAP_OSD;
		return flags;
	}

        return 0;
}

static void draw_alpha (int x0, int y0, int w, int h, unsigned char *src, unsigned char *srca, int stride)
{
        int bpp; 
	char *base;
	int image_w;

	if (image_fullscreen == 1) {
		x0 += image_x_full;
		y0 += image_y_full;
	}

	image_w = window->surface->width;
	bpp = window->surface->bytesperpixel;
	base = window->surface->vbuf + ((y0 * image_w) + x0) * bpp;

	switch (window->surface->bitsperpixel) {
		case 32:
			vo_draw_alpha_rgb32(w, h, src, srca, stride, base, image_w * bpp);
			break;
		case 24:
			vo_draw_alpha_rgb24(w, h, src, srca, stride, base, image_w * bpp);
			break;
		case 16:
			vo_draw_alpha_rgb16(w, h, src, srca, stride, base, image_w * bpp);
			break;
		case 15:
			vo_draw_alpha_rgb15(w, h, src, srca, stride, base, image_w * bpp);
			break;
	}

#if !defined(XYNTH_DOUBLEBUFFER)
	s_putboxpart(window->surface, x0, y0, w, h, window->surface->width, window->surface->height, window->surface->vbuf, x0, y0);
#endif
}

static uint32_t draw_image (mp_image_t *mpi)
{
	int x;
	int y;
	int w;
	int h;
	uint8_t *rgbplane;

	if (!running) {
		return VO_TRUE;
	}
	
	if (mpi->flags & MP_IMGFLAG_DIRECT) {
		assert((uint32_t) mpi->priv >= 0);
		return VO_TRUE; // it's already done
	}

	if (image_fullscreen == 1) {
		return VO_FALSE;
	}

	x = mpi->x;
	y = mpi->y;
	w = mpi->w;
	h = mpi->h;
	rgbplane = mpi->planes[0] + y * mpi->stride[0] + (x * mpi->bpp) / 8;

	s_putbox(window->surface, x, y, w, h, rgbplane);

	return VO_TRUE;
}

static void atexit_handler (s_window_t *w)
{
	if (running) {
		exit_player(NULL);
	}
}
static void keybd_handler_left		(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key(KEY_LEFT); }	/* seek +10		*/
static void keybd_handler_right		(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key(KEY_RIGHT); }	/* seek -10		*/
static void keybd_handler_down		(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key(KEY_DOWN); }	/* seek +60		*/
static void keybd_handler_up		(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key(KEY_UP); }	/* seek -60		*/
static void keybd_handler_pagedown	(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key(KEY_PAGE_DOWN); }	/* seek +600	*/
static void keybd_handler_pageup	(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key(KEY_PAGE_UP); }/* seek -600		*/
static void keybd_handler_minus		(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key('-'); }	/* audio delay +1.000	*/
static void keybd_handler_add		(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key('+'); }	/* audio delay -1.000	*/
static void keybd_handler_q		(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key('q'); }	/* quit			*/
static void keybd_handler_esc		(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key(KEY_ESC); }	/* quit			*/
static void keybd_handler_p		(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key(' '); }	/* pause		*/
static void keybd_handler_space		(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key(' '); }	/* pause		*/
static void keybd_handler_o		(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key('o'); }	/* osd			*/
static void keybd_handler_z		(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key('z'); }	/* sub delay -0.1	*/
static void keybd_handler_x		(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key('x'); }	/* sub delay +0.1	*/
static void keybd_handler_g		(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key('g'); }	/* sub step -1		*/
static void keybd_handler_y		(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key('y'); }	/* sub step +1		*/
static void keybd_handler_9		(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key('9'); }	/* volume -1		*/
static void keybd_handler_divide	(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key('/'); }	/* volume -1		*/
static void keybd_handler_0		(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key('0'); }	/* volume +1		*/
static void keybd_handler_multiply	(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key('*'); }	/* volume +1		*/
static void keybd_handler_m		(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key('m'); }	/* mute			*/
static void keybd_handler_1		(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key('1'); }	/* contrast -1		*/
static void keybd_handler_2		(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key('2'); }	/* contrast +1		*/
static void keybd_handler_3		(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key('3'); }	/* brightness -1	*/
static void keybd_handler_4		(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key('4'); }	/* brightness +1	*/
static void keybd_handler_5		(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key('5'); }	/* hue -1		*/
static void keybd_handler_6		(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key('6'); }	/* hue +1		*/
static void keybd_handler_7		(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key('7'); }	/* saturation -1	*/
static void keybd_handler_8		(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key('8'); }	/* saturation +1	*/
static void keybd_handler_d		(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key('d'); }	/* frame drop		*/
static void keybd_handler_r		(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key('r'); }	/* sub pos -1		*/
static void keybd_handler_t		(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key('t'); }	/* sub pos +1		*/
static void keybd_handler_a		(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key('a'); }	/* sub alignment	*/
static void keybd_handler_v		(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key('v'); }	/* sub visibility	*/
static void keybd_handler_j		(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key('j'); }	/* vobsub lang		*/
static void keybd_handler_s		(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key('s'); }	/* screen shot		*/
static void keybd_handler_w		(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key('w'); }	/* panscan -0.1		*/
static void keybd_handler_e		(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key('e'); }	/* panscan +0.1		*/
static void keybd_handler_f		(s_window_t *w, s_event_t *e, s_handler_t *h) { mplayer_put_key('f'); }	/* vo fullscreen	*/

#define keybd_handler_set_(a, b)\
	{\
		s_handler_init(&handler);\
		handler->type = KEYBD_HANDLER;\
		handler->keybd.button = a;\
		handler->keybd.p = b;\
		s_handler_add(window, handler);\
	}

static void keybd_handler_set (void)
{
	s_handler_t *handler;
	
	keybd_handler_set_(S_KEYCODE_RIGHT, keybd_handler_right);
	keybd_handler_set_(S_KEYCODE_LEFT, keybd_handler_left);
	keybd_handler_set_(S_KEYCODE_UP, keybd_handler_up);
	keybd_handler_set_(S_KEYCODE_DOWN, keybd_handler_down);
	keybd_handler_set_(S_KEYCODE_PAGEUP, keybd_handler_pageup);
	keybd_handler_set_(S_KEYCODE_PAGEDOWN, keybd_handler_pagedown);
	keybd_handler_set_(S_KEYCODE_KP_SUBTRACT, keybd_handler_minus);
	keybd_handler_set_(S_KEYCODE_KP_ADD, keybd_handler_add);
	keybd_handler_set_(S_KEYCODE_q, keybd_handler_q);
	keybd_handler_set_(S_KEYCODE_ESCAPE, keybd_handler_esc);
	keybd_handler_set_(S_KEYCODE_p, keybd_handler_p);
	keybd_handler_set_(S_KEYCODE_SPACE, keybd_handler_space);
	keybd_handler_set_(S_KEYCODE_o, keybd_handler_o);
	keybd_handler_set_(S_KEYCODE_z, keybd_handler_z);
	keybd_handler_set_(S_KEYCODE_x, keybd_handler_x);
	keybd_handler_set_(S_KEYCODE_g, keybd_handler_g);
	keybd_handler_set_(S_KEYCODE_y, keybd_handler_y);
	keybd_handler_set_(S_KEYCODE_NINE, keybd_handler_9);
	keybd_handler_set_(S_KEYCODE_KP_DIVIDE, keybd_handler_divide);
	keybd_handler_set_(S_KEYCODE_ZERO, keybd_handler_0);
	keybd_handler_set_(S_KEYCODE_KP_MULTIPLY, keybd_handler_multiply);
	keybd_handler_set_(S_KEYCODE_m, keybd_handler_m);
	keybd_handler_set_(S_KEYCODE_ONE, keybd_handler_1);
	keybd_handler_set_(S_KEYCODE_TWO, keybd_handler_2);
	keybd_handler_set_(S_KEYCODE_THREE, keybd_handler_3);
	keybd_handler_set_(S_KEYCODE_FOUR, keybd_handler_4);
	keybd_handler_set_(S_KEYCODE_FIVE, keybd_handler_5);
	keybd_handler_set_(S_KEYCODE_SIX, keybd_handler_6);
	keybd_handler_set_(S_KEYCODE_SEVEN, keybd_handler_7);
	keybd_handler_set_(S_KEYCODE_EIGHT, keybd_handler_8);
	keybd_handler_set_(S_KEYCODE_d, keybd_handler_d);
	keybd_handler_set_(S_KEYCODE_r, keybd_handler_r);
	keybd_handler_set_(S_KEYCODE_t, keybd_handler_t);
	keybd_handler_set_(S_KEYCODE_a, keybd_handler_a);
	keybd_handler_set_(S_KEYCODE_v, keybd_handler_v);
	keybd_handler_set_(S_KEYCODE_j, keybd_handler_j);
	keybd_handler_set_(S_KEYCODE_f, keybd_handler_f);
	keybd_handler_set_(S_KEYCODE_s, keybd_handler_s);
	keybd_handler_set_(S_KEYCODE_w, keybd_handler_w);
	keybd_handler_set_(S_KEYCODE_e, keybd_handler_e);
}
static void handler_set (void)
{
	keybd_handler_set();
}

