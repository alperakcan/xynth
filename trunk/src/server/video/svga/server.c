/***************************************************************************
    begin                : Wed Oct 8 2003
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

#if defined(VIDEO_SVGALib)

#include "../../../lib/xynth_.h"
#include "server.h"
#include "svga.h"
#include "../helper/helper.h"

#include <vga.h>
#include <fcntl.h>
#include <sys/mman.h>

#if SVGALIB_VER < 0x010920
extern unsigned int __svgalib_linear_mem_base, __svgalib_linear_mem_size;
#endif

static struct {
	int snumber;
	int vnumber;
} s_video_svga_modetable[MAX_MODES] = {
	{M360x480x256, G360x480x256},
	{M640x480x2, G640x480x2},
	{M640x480x256, G640x480x256},
	{M800x600x256, G800x600x256},
	{M1024x768x256, G1024x768x256},
	{M1280x1024x256, G1280x1024x256},
	{M320x200x32K, G320x200x32K},
	{M320x200x64K, G320x200x64K},
	{M320x200x16M, G320x200x16M},
	{M640x480x32K, G640x480x32K},
	{M640x480x64K, G640x480x64K},
	{M640x480x16M, G640x480x16M},
	{M800x600x32K, G800x600x32K},
	{M800x600x64K, G800x600x64K},
	{M800x600x16M, G800x600x16M},
	{M1024x768x32K, G1024x768x32K},
	{M1024x768x64K, G1024x768x64K},
	{M1024x768x16M, G1024x768x16M},
	{M1280x1024x32K, G1280x1024x32K},
	{M1280x1024x64K, G1280x1024x64K},
	{M1280x1024x16M, G1280x1024x16M},
	{M800x600x16, G800x600x16},
	{M1024x768x16, G1024x768x16},
	{M1280x1024x16, G1280x1024x16},
	{M720x348x2, G720x348x2},
	{M320x200x16M32, G320x200x16M32},
	{M640x480x16M32, G640x480x16M32},
	{M800x600x16M32, G800x600x16M32},
	{M1024x768x16M32, G1024x768x16M32},
	{M1280x1024x16M32, G1280x1024x16M32},
	{M1152x864x16, G1152x864x16},
	{M1152x864x256, G1152x864x256},
	{M1152x864x32K, G1152x864x32K},
	{M1152x864x64K, G1152x864x64K},
	{M1152x864x16M, G1152x864x16M},
	{M1152x864x16M32, G1152x864x16M32},
	{M1600x1200x16, G1600x1200x16},
	{M1600x1200x256, G1600x1200x256},
	{M1600x1200x32K, G1600x1200x32K},
	{M1600x1200x64K, G1600x1200x64K},
	{M1600x1200x16M, G1600x1200x16M},
	{M1600x1200x16M32, G1600x1200x16M32},
	{M320x240x256V, G320x240x256V},
	{M320x240x32K, G320x240x32K},
	{M320x240x64K, G320x240x64K},
	{M320x240x16M, G320x240x16M},
	{M320x240x16M32, G320x240x16M32},
	{M400x300x256, G400x300x256},
	{M400x300x32K, G400x300x32K},
	{M400x300x64K, G400x300x64K},
	{M400x300x16M, G400x300x16M},
	{M400x300x16M32, G400x300x16M32},
	{M512x384x256, G512x384x256},
	{M512x384x32K, G512x384x32K},
	{M512x384x64K, G512x384x64K},
	{M512x384x16M, G512x384x16M},
	{M512x384x16M32, G512x384x16M32},
	{M960x720x256, G960x720x256},
	{M960x720x32K, G960x720x32K},
	{M960x720x64K, G960x720x64K},
	{M960x720x16M, G960x720x16M},
	{M960x720x16M32, G960x720x16M32},
	{M1920x1440x256, G1920x1440x256},
	{M1920x1440x32K, G1920x1440x32K},
	{M1920x1440x64K, G1920x1440x64K},
	{M1920x1440x16M, G1920x1440x16M},
	{M1920x1440x16M32, G1920x1440x16M32},
	{M320x400x256V, G320x400x256V},
	{M320x400x32K, G320x400x32K},
	{M320x400x64K, G320x400x64K},
	{M320x400x16M, G320x400x16M},
	{M320x400x16M32, G320x400x16M32},
	{M640x400x256, G640x400x256},
	{M640x400x32K, G640x400x32K},
	{M640x400x64K, G640x400x64K},
	{M640x400x16M, G640x400x16M},
	{M640x400x16M32, G640x400x16M32},
	{M320x480x256, G320x480x256},
	{M320x480x32K, G320x480x32K},
	{M320x480x64K, G320x480x64K},
	{M320x480x16M, G320x480x16M},
	{M320x480x16M32, G320x480x16M32},
	{M720x540x256, G720x540x256},
	{M720x540x32K, G720x540x32K},
	{M720x540x64K, G720x540x64K},
	{M720x540x16M, G720x540x16M},
	{M720x540x16M32, G720x540x16M32},
	{M848x480x256, G848x480x256},
	{M848x480x32K, G848x480x32K},
	{M848x480x64K, G848x480x64K},
	{M848x480x16M, G848x480x16M},
	{M848x480x16M32, G848x480x16M32},
	{M1072x600x256, G1072x600x256},
	{M1072x600x32K, G1072x600x32K},
	{M1072x600x64K, G1072x600x64K},
	{M1072x600x16M, G1072x600x16M},
	{M1072x600x16M32, G1072x600x16M32},
	{M1280x720x256, G1280x720x256},
	{M1280x720x32K, G1280x720x32K},
	{M1280x720x64K, G1280x720x64K},
	{M1280x720x16M, G1280x720x16M},
	{M1280x720x16M32, G1280x720x16M32},
	{M1360x768x256, G1360x768x256},
	{M1360x768x32K, G1360x768x32K},
	{M1360x768x64K, G1360x768x64K},
	{M1360x768x16M, G1360x768x16M},
	{M1360x768x16M32, G1360x768x16M32},
	{M1800x1012x256, G1800x1012x256},
	{M1800x1012x32K, G1800x1012x32K},
	{M1800x1012x64K, G1800x1012x64K},
	{M1800x1012x16M, G1800x1012x16M},
	{M1800x1012x16M32, G1800x1012x16M32},
	{M1920x1080x256, G1920x1080x256},
	{M1920x1080x32K, G1920x1080x32K},
	{M1920x1080x64K, G1920x1080x64K},
	{M1920x1080x16M, G1920x1080x16M},
	{M1920x1080x16M32, G1920x1080x16M32},
	{M2048x1152x256, G2048x1152x256},
	{M2048x1152x32K, G2048x1152x32K},
	{M2048x1152x64K, G2048x1152x64K},
	{M2048x1152x16M, G2048x1152x16M},
	{M2048x1152x16M32, G2048x1152x16M32},
	{M2048x1536x256, G2048x1536x256},
	{M2048x1536x32K, G2048x1536x32K},
	{M2048x1536x64K, G2048x1536x64K},
	{M2048x1536x16M, G2048x1536x16M},
	{M2048x1536x16M32, G2048x1536x16M32},
	{M512x480x256, G512x480x256},
	{M512x480x32K, G512x480x32K},
	{M512x480x64K, G512x480x64K},
	{M512x480x16M, G512x480x16M},
	{M512x480x16M32, G512x480x16M32},
	{M400x600x256, G400x600x256},
	{M400x600x32K, G400x600x32K},
	{M400x600x64K, G400x600x64K},
	{M400x600x16M, G400x600x16M},
	{M400x600x16M32, G400x600x16M32},
	{M400x300x256X, G400x300x256X},
	{M320x200x256V, G320x200x256V}
};

s_video_input_t s_video_svga_input_keybd = {
	.keybd = {
		VIDEO_INPUT_KEYBD,
		s_video_svga_kbd_init,
		s_video_helper_kbd_update,
		s_video_helper_kbd_uninit,
	}
};

s_video_input_t s_video_svga_input_mouse = {
	.mouse = {
		VIDEO_INPUT_MOUSE,
		s_video_helper_mouse_update,
		s_video_helper_mouse_uninit,
		s_video_helper_mouse_init,
	}
};

s_video_input_t *s_video_svga_input[] = {
	&s_video_svga_input_keybd,
	&s_video_svga_input_mouse,
	NULL,
};

s_video_driver_t s_video_svga = {
	"svgalib",
	"/dev/mem",
	s_video_svga_input,
	s_video_svga_server_init,
	s_video_svga_server_uninit,
	s_video_svga_server_goto_back,
	s_video_svga_server_comefrom_back,
	NULL,
	NULL,
	NULL
};

void s_video_svga_server_goto_back (void)
{
	s_server_surface_lock_real();
}

void s_video_svga_server_comefrom_back (void)
{
	s_server_surface_refresh();
	s_video_helper_kbd_set_attr();
}

void s_video_svga_surface_uninit (void)
{
	s_video_svga_data_t *priv = (s_video_svga_data_t *) server->driver->driver_data;
	munmap((void *) server->window->surface->linear_mem_base, server->window->surface->linear_mem_size);
	s_free(priv);
}

void s_video_svga_server_uninit (void)
{
        s_video_svga_surface_uninit();
	vga_setmode(TEXT);
}

void s_video_svga_server_surface_setrgbpalette (void)
{
	int i;
	int r;
	int g;
	int b;

	for (i = 0; i < 256; i++) {
		b = (i & 7) * (64 / 8);		/* 3 bits */
		g = ((i & 56) >> 3) * (64 / 8);	/* 3 bits */
		r = ((i & 192) >> 6) * (64 / 4);/* 2 bits */
		vga_setpalette(i, r, g, b);
	}
}

void s_video_svga_server_surface_init (int mode)
{
	vga_cardinfo *cardinfo;
	vga_modeinfo *modeinfo;

       	vga_setlinearaddressing();
        cardinfo = vga_getcardinfo();
        modeinfo = vga_getmodeinfo(mode);

        server->window->surface->width = modeinfo->width;
        server->window->surface->height = modeinfo->height;
        server->window->surface->bytesperpixel = modeinfo->bytesperpixel;
        server->window->surface->bitsperpixel = server->window->surface->bytesperpixel * 8;
	server->window->surface->blueoffset = 0;
	server->window->surface->greenoffset = 0;
	server->window->surface->redoffset = 0;
	server->window->surface->bluelength = 0;
	server->window->surface->greenlength = 0;
	server->window->surface->redlength = 0;
	server->window->surface->colors = modeinfo->colors;

        switch (modeinfo->colors) {
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

#if SVGALIB_VER < 0x010920
	server->window->surface->vbuf = vga_getgraphmem();
	server->window->surface->linear_buf = server->window->surface->vbuf;
        server->window->surface->linear_mem_base = (unsigned int) __svgalib_linear_mem_base;
	server->window->surface->linear_mem_size = (unsigned int) __svgalib_linear_mem_size;
#else
	server->window->surface->vbuf = cardinfo->linearmem;
	server->window->surface->linear_buf = cardinfo->linearmem;
	server->window->surface->linear_mem_base = (unsigned int) cardinfo->physmem;
	server->window->surface->linear_mem_size = (unsigned int) cardinfo->physmemsize;
#endif
	
	s_video_svga_server_surface_setrgbpalette();
}

int s_video_svga_server_init (s_server_conf_t *cfg)
{
	int i;
	s_video_svga_data_t *priv;
	s_video_helper_mode_info_t *gmode;
	
	priv = (s_video_svga_data_t *) s_malloc(sizeof(s_video_svga_data_t));
	server->driver->driver_data = (void *) priv;
	
	priv->console_fd = s_video_helper_console_init();
	if (priv->console_fd < 0) {
		debugf(DSER, "Unable to get console");
		goto err0;
	}
	if (s_video_helper_mode_find(cfg, &gmode)) {
		debugf(DSER, "Couldn't find mode: %s", cfg->general.mode);
		goto err0;
	}

/*	vga_disabledriverreport();
*/
	vga_init();
	vga_runinbackground(VGA_GOTOBACK, s_server_goto_back);
	vga_runinbackground(VGA_COMEFROMBACK, s_server_comefrom_back);
	
	for (i = 0; i < MAX_MODES; i++) {
		if (s_video_svga_modetable[i].snumber == gmode->number) {
			if ((vga_hasmode(s_video_svga_modetable[i].vnumber) == 0) || (vga_setmode(s_video_svga_modetable[i].vnumber) != 0)) {
				debugf(DSER, "Mode %s(%d:%d) is not supported", cfg->general.mode, s_video_svga_modetable[i].snumber, s_video_svga_modetable[i].vnumber);
				goto err0;
			} else {
				s_video_svga_server_surface_init(s_video_svga_modetable[i].vnumber);
				s_video_helper_mtrr_add(server->window->surface->linear_mem_base, server->window->surface->linear_mem_size);
				return s_video_svga_modetable[i].vnumber;
			}
		}
	}

	debugf(DSER, "Mode %s cannot be found", cfg->general.mode);
err0:	s_free(priv);
	return -1;
}

#endif /* VIDEO_SVGALib */
