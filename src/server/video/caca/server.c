/***************************************************************************
    begin                : Tue Dec 26 2006
    copyright            : (C) 2006 - 2007 by Alper Akcan
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

#if defined(VIDEO_CACA)

#include "../../../lib/xynth_.h"
#include "server.h"
#include "../helper/helper.h"
#include "caca.h"
#include <sys/shm.h>

s_video_input_t s_video_caca_input_keybd = {
	VIDEO_INPUT_KEYBD,
	s_video_caca_kbd_init,
	s_video_caca_kbd_update,
	s_video_caca_kbd_uninit,
};

s_video_input_t s_video_caca_input_mouse = {
	VIDEO_INPUT_MOUSE,
	s_video_caca_mouse_init,
	s_video_caca_mouse_update,
	s_video_caca_mouse_uninit,
};

s_video_input_t *s_video_caca_input[] = {
	&s_video_caca_input_keybd,
	&s_video_caca_input_mouse,
	NULL,
};

s_video_driver_t s_video_caca = {
	"caca",
	"/dev/caca",
	s_video_caca_input,
	s_video_caca_server_init,
	s_video_caca_server_uninit,
	s_video_caca_server_goto_back,
	s_video_caca_server_comefrom_back,
	s_video_caca_server_restore,
	s_video_caca_server_surface_update,
	s_video_caca_server_fullscreen,
	NULL /* driver data */
};

void s_video_caca_server_goto_back (void)
{
}

void s_video_caca_server_comefrom_back (void)
{
}

void s_video_caca_server_restore (void)
{
}

void s_video_caca_server_uninit (void)
{
}

int s_video_caca_server_init (s_server_conf_t *cfg)
{
        void *addr;
	s_video_caca_data_t *priv;
	s_video_helper_mode_info_t *gmode;
	
	priv = (s_video_caca_data_t *) s_malloc(sizeof(s_video_caca_data_t));
	server->driver->driver_data = (void *) priv;
	
        priv->mouse_fd[0] = -1;
        priv->mouse_fd[1] = -1;
        priv->keybd_fd[0] = -1;
        priv->keybd_fd[1] = -1;
	
	if (caca_init()) {
		debugf(DSER, "Couldn't initialize libcaca");
		goto err0;
	}
	if (s_video_helper_mode_find(cfg, &gmode)) {
		debugf(DSER, "Couldn't find mode: %s", cfg->general.mode);
		goto err0;
	}
	priv->caca_width = caca_get_width();
	priv->caca_height = caca_get_height();

	server->window->surface->width = gmode->xdim;
	server->window->surface->height = gmode->ydim;
	server->window->surface->bytesperpixel = gmode->bytesperpixel;
	server->window->surface->bitsperpixel = gmode->bytesperpixel * 8;
        server->window->surface->blueoffset = 0;
        server->window->surface->greenoffset = 0;
        server->window->surface->redoffset = 0;
        server->window->surface->bluelength = 0;
        server->window->surface->greenlength = 0;
        server->window->surface->redlength = 0;
        server->window->surface->colors = 0;
        switch (server->window->surface->bitsperpixel) {
		case 8:
			server->window->surface->colors = 256;
			server->window->surface->bitsperpixel = 8;
			server->window->surface->blueoffset = 0;
			server->window->surface->greenoffset = 3;
			server->window->surface->redoffset = 6;
			server->window->surface->bluelength = 3;
			server->window->surface->greenlength = 3;
			server->window->surface->redlength = 2;
			break;
		case 15:
			server->window->surface->colors = 32768;
			server->window->surface->bitsperpixel = 15;
			server->window->surface->blueoffset = 0;
			server->window->surface->greenoffset = 5;
			server->window->surface->redoffset = 10;
			server->window->surface->bluelength = 5;
			server->window->surface->greenlength = 5;
			server->window->surface->redlength = 5;
			break;
		case 16:
			server->window->surface->colors = 65536;
			server->window->surface->bitsperpixel = 16;
			server->window->surface->blueoffset = 0;
			server->window->surface->greenoffset = 5;
			server->window->surface->redoffset = 11;
			server->window->surface->bluelength = 5;
			server->window->surface->greenlength = 6;
			server->window->surface->redlength = 5;
			break;
		case 24:
		case 32:
			server->window->surface->colors = 256 * 65536;
			server->window->surface->bitsperpixel = server->window->surface->bytesperpixel * 8;
			server->window->surface->blueoffset = 0;
			server->window->surface->greenoffset = 8;
			server->window->surface->redoffset = 16;
			server->window->surface->bluelength = 8;
			server->window->surface->greenlength = 8;
			server->window->surface->redlength = 8;
			break;
	}
	if ((priv->screen_shm_mid = shmget(IPC_PRIVATE, sizeof(char) * server->window->surface->width * server->window->surface->height * server->window->surface->bytesperpixel, IPC_CREAT | 0644)) < 0) {
		debugf(DSER | DSYS, "Can not get id for shared memory");
		goto err1;
	}
	if ((addr = (void *) shmat(priv->screen_shm_mid, NULL, 0)) < 0) {
		debugf(DSER | DSYS, "Can not attach the shared memory");
		goto err1;
	}
	server->window->surface->shm_sid = priv->screen_shm_mid;
        shmctl(priv->screen_shm_mid, IPC_RMID, 0);

	server->window->surface->linear_mem_base = (unsigned int) addr;
	server->window->surface->linear_mem_size = (unsigned int) (sizeof(char) * server->window->surface->width * server->window->surface->height * server->window->surface->bytesperpixel);
	server->window->surface->vbuf = (char *) addr;
	server->window->surface->linear_buf = (char *) addr;
	server->window->surface->need_expose = SURFACE_NEEDEXPOSE;
	
	caca_set_window_title("Xynth Windowing System (video driver = caca)");

	priv->event_tid = s_thread_create(&s_video_caca_event_parse, (void *) NULL);
	return gmode->number;

err1:
err0:	s_free(priv);
	return -1;
}

void s_video_caca_server_surface_update (s_rect_t *coor)
{
	unsigned int red;
	unsigned int green;
	unsigned int blue;
	struct caca_bitmap *bitmap;
	s_video_caca_data_t *priv = server->driver->driver_data;
	priv->caca_width = caca_get_width();
	priv->caca_height = caca_get_height();
	red = ((0xff >> (8 - server->window->surface->redlength)) << server->window->surface->redoffset);
	green = ((0xff >> (8 - server->window->surface->greenlength)) << server->window->surface->greenoffset);
	blue = ((0xff >> (8 - server->window->surface->bluelength)) << server->window->surface->blueoffset);
	bitmap = caca_create_bitmap(server->window->surface->bitsperpixel,
				    server->window->surface->width,
				    server->window->surface->height,
				    server->window->surface->width * server->window->surface->bytesperpixel,
				    red,
				    green,
				    blue,
				    0x00000000);
	caca_draw_bitmap(0, 0, priv->caca_width, priv->caca_height, bitmap, server->window->surface->vbuf);
	caca_refresh();
	caca_free_bitmap(bitmap);
}

void s_video_caca_server_fullscreen (void)
{
}

#endif /* VIDEO_CACA */
