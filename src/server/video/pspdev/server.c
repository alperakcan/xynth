/***************************************************************************
    begin                : Thu Sep 22 2005
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

#include "../../../lib/xynth_.h"
#include "server.h"
#include "../helper/helper.h"
#include "pspdev.h"

#if !defined(PSP_DIRECTACCESS)
	#include "../../../lib/mem.h"
#endif

#if !defined(PSP_BITSPERPIXEL)
	#define PSP_BITSPERPIXEL 32
#endif

s_video_input_t s_video_pspdev_input_keybd = {
	VIDEO_INPUT_KEYBD,
	s_video_pspdev_kbd_init,
	s_video_pspdev_kbd_update,
	s_video_pspdev_kbd_uninit,
};

s_video_input_t s_video_pspdev_input_mouse = {
	VIDEO_INPUT_MOUSE,
	s_video_pspdev_mouse_init,
	s_video_pspdev_mouse_update,
	s_video_pspdev_mouse_uninit,
};

s_video_input_t *s_video_pspdev_input[] = {
	&s_video_pspdev_input_keybd,
	&s_video_pspdev_input_mouse,
	NULL,
};

s_video_driver_t s_video_pspdev = {
	"pspdev",
	"/dev/pspdev",
	s_video_pspdev_input,
	s_video_pspdev_server_init,
	s_video_pspdev_server_uninit,
	NULL,
	NULL,
	NULL,
	s_video_pspdev_server_surface_update,
	NULL,
	NULL
};

void s_video_pspdev_server_uninit (void)
{
	s_video_pspdev_data_t *priv = (s_video_pspdev_data_t *) xynth_server->driver->driver_data;
	s_thread_join(priv->event_tid, NULL);
#if defined(PSP_DIRECTACCESS)
#else
	s_free((void *) xynth_server->window->surface->linear_mem_base);
#endif
	xynth_server->driver->driver_data = NULL;
	s_free(priv);
}

int s_video_pspdev_server_init (s_server_conf_t *cfg)
{
	void *addr;
	int pixel_format;
	s_video_pspdev_data_t *priv;

	priv = (s_video_pspdev_data_t *) s_malloc(sizeof(s_video_pspdev_data_t));
	xynth_server->driver->driver_data = (void *) priv;

        priv->mouse_fd[0] = -1;
        priv->mouse_fd[1] = -1;
        priv->keybd_fd[0] = -1;
        priv->keybd_fd[1] = -1;

	xynth_server->window->surface->width = 480;
	xynth_server->window->surface->height = 272;
	xynth_server->window->surface->bytesperpixel = 4;
	xynth_server->window->surface->bitsperpixel = 32;
	xynth_server->window->surface->blueoffset = 0;
	xynth_server->window->surface->greenoffset = 0;
	xynth_server->window->surface->redoffset = 0;
	xynth_server->window->surface->bluelength = 0;
	xynth_server->window->surface->greenlength = 0;
	xynth_server->window->surface->redlength = 0;
	xynth_server->window->surface->colors = 0;
	
	switch (PSP_BITSPERPIXEL) {
		case 16:
			xynth_server->window->surface->colors = 65536;
			xynth_server->window->surface->bytesperpixel = 2;
			xynth_server->window->surface->bitsperpixel = 16;
			xynth_server->window->surface->blueoffset = 11;
			xynth_server->window->surface->greenoffset = 5;
			xynth_server->window->surface->redoffset = 0;
			xynth_server->window->surface->bluelength = 5;
			xynth_server->window->surface->greenlength = 6;
			xynth_server->window->surface->redlength = 5;
			pixel_format = PSP_DISPLAY_PIXEL_FORMAT_565;
			break;
		case 32:
			xynth_server->window->surface->colors = 65536 * 256;
			xynth_server->window->surface->bytesperpixel = 4;
			xynth_server->window->surface->bitsperpixel = 32;
			xynth_server->window->surface->blueoffset = 16;
			xynth_server->window->surface->greenoffset = 8;
			xynth_server->window->surface->redoffset = 0;
			xynth_server->window->surface->bluelength = 8;
			xynth_server->window->surface->greenlength = 8;
			xynth_server->window->surface->redlength = 8;
			pixel_format = PSP_DISPLAY_PIXEL_FORMAT_8888;
			break;
		default:
			debugf(DSER, "requested bitsperpixel(%d) is not supported", PSP_BITSPERPIXEL);
			goto err0;
	}
	
	priv->vram_base = (void *) (0x40000000 | (u32) sceGeEdramGetAddr());

	sceDisplaySetMode(0, 480, 272);
	sceDisplaySetFrameBuf(priv->vram_base, 512, pixel_format, PSP_DISPLAY_SETBUF_NEXTFRAME);

#if defined(PSP_DIRECTACCESS)
	addr = priv->vram_base;
	xynth_server->window->surface->linear_buf_pitch = 512;
#else
	addr = (void *) s_malloc(sizeof(char) * xynth_server->window->surface->width * xynth_server->window->surface->height * xynth_server->window->surface->bytesperpixel);
	xynth_server->window->surface->need_expose = SURFACE_NEEDEXPOSE;
	xynth_server->window->surface->linear_buf_pitch = xynth_server->window->surface->width;
#endif
	xynth_server->window->surface->linear_mem_base = (unsigned int) addr;
	xynth_server->window->surface->linear_mem_size = (unsigned int) (sizeof(char) * xynth_server->window->surface->width * xynth_server->window->surface->height * xynth_server->window->surface->bytesperpixel);
	xynth_server->window->surface->vbuf = (unsigned char *) addr;
	xynth_server->window->surface->linear_buf = (unsigned char *) addr;

	priv->event_tid = s_thread_create(&s_video_pspdev_event_parse, (void *) NULL);

	return 0;

err0:	s_free(priv);
	xynth_server->driver->driver_data = NULL;
	return -1;
}

void s_video_pspdev_server_surface_update (s_rect_t *coor)
{
#if defined(PSP_DIRECTACCESS)
	return;
#else
	int y;
	s_rect_t clip;
	s_rect_t inter;
	s_video_pspdev_data_t *priv = (s_video_pspdev_data_t *) xynth_server->driver->driver_data;
	char *dst = (char *) priv->vram_base;
	char *src = (char *) xynth_server->window->surface->linear_mem_base;

	clip.x = 0;
	clip.y = 0;
	clip.w = xynth_server->window->surface->width;
	clip.h = xynth_server->window->surface->height;
	if (s_region_rect_intersect(&clip, coor, &inter)) {
		return;
	}

	src += ((inter.x + inter.y * xynth_server->window->surface->width) * xynth_server->window->surface->bytesperpixel);
	dst += ((inter.x + inter.y * 512) * xynth_server->window->surface->bytesperpixel);

	for (y = 0; y < inter.h; y++) {
		switch (xynth_server->window->surface->bytesperpixel) {
			case 1: s_memcpy1(dst, src, inter.w); break;
			case 2: s_memcpy2(dst, src, inter.w); break;
			case 4: s_memcpy4(dst, src, inter.w); break;
		}
		src += (xynth_server->window->surface->width * xynth_server->window->surface->bytesperpixel);
		dst += (512 * xynth_server->window->surface->bytesperpixel);
	}
#endif
}
