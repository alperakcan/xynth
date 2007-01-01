/***************************************************************************
    begin                : Thu Feb 2 2006
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

#if defined(VIDEO_GP2X)

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "../../../lib/xynth_.h"
#include "server.h"
#include "../helper/helper.h"
#include "gp2x.h"

s_video_input_t s_video_gp2x_input_keybd = {
	VIDEO_INPUT_KEYBD,
	s_video_gp2x_kbd_init,
	s_video_gp2x_kbd_update,
	s_video_gp2x_kbd_uninit,
};

s_video_input_t s_video_gp2x_input_mouse = {
	VIDEO_INPUT_MOUSE,
	s_video_gp2x_mouse_init,
	s_video_gp2x_mouse_update,
	s_video_gp2x_mouse_uninit,
};

s_video_input_t *s_video_gp2x_input[] = {
	&s_video_gp2x_input_keybd,
	&s_video_gp2x_input_mouse,
	NULL,
};

s_video_driver_t s_video_gp2x = {
	"gp2x",
	"/dev/mem",
	s_video_gp2x_input,
	s_video_gp2x_server_init,
	s_video_gp2x_server_uninit,
	s_video_gp2x_server_goto_back,
	s_video_gp2x_server_comefrom_back,
	s_video_gp2x_server_restore,
	s_video_gp2x_server_surface_update,
	s_video_gp2x_server_fullscreen,
	NULL /* driver data */
};

void s_video_gp2x_server_goto_back (void)
{
}

void s_video_gp2x_server_comefrom_back (void)
{
}

void s_video_gp2x_server_restore (void)
{
}

void s_video_gp2x_server_uninit (void)
{
}

int s_video_gp2x_server_init (s_server_conf_t *cfg)
{
	int fb_fd;
	void *m_addr;
	void *v_addr;
	s_video_gp2x_data_t *priv;

	priv = (s_video_gp2x_data_t *) s_malloc(sizeof(s_video_gp2x_data_t));
	server->driver->driver_data = (void *) priv;

        priv->mouse_fd[0] = -1;
        priv->mouse_fd[1] = -1;
        priv->keybd_fd[0] = -1;
        priv->keybd_fd[1] = -1;

	server->window->surface->width = 320;
	server->window->surface->height = 240;
	server->window->surface->bytesperpixel = 2;
	server->window->surface->bitsperpixel = 16;
	server->window->surface->blueoffset = 0;
	server->window->surface->greenoffset = 0;
	server->window->surface->redoffset = 0;
	server->window->surface->bluelength = 0;
	server->window->surface->greenlength = 0;
	server->window->surface->redlength = 0;
	server->window->surface->colors = 0;

	switch (server->window->surface->bitsperpixel) {
		case 16:
			server->window->surface->colors = 65536;
			server->window->surface->bytesperpixel = 2;
			server->window->surface->bitsperpixel = 16;
			server->window->surface->blueoffset = 0;
			server->window->surface->greenoffset = 5;
			server->window->surface->redoffset = 11;
			server->window->surface->bluelength = 5;
			server->window->surface->greenlength = 6;
			server->window->surface->redlength = 5;
			break;
		default:
			debugf(DSER, "requested bitsperpixel(%d) is not supported", server->window->surface->bitsperpixel);
			goto err0;
	}

	fb_fd = open("/dev/mem", O_RDWR);
	v_addr = (void *) mmap(NULL, 5 * 1024 * 1024, PROT_WRITE | PROT_READ, MAP_SHARED, fb_fd, 0x03101000);
	m_addr = (void *) mmap(NULL, 0x10000, PROT_WRITE | PROT_READ, MAP_SHARED, fb_fd, 0xc0000000);
	priv->gp2x_io = (unsigned short *) m_addr;

	server->window->surface->linear_buf_pitch = server->window->surface->width;

	server->window->surface->linear_mem_base = (unsigned int) 0x03101000;
	server->window->surface->linear_mem_size = (unsigned int) (5 * 1024 * 1024);
	server->window->surface->vbuf = (char *) v_addr;
	server->window->surface->linear_buf = (char *) v_addr;

	priv->event_tid = s_thread_create(&s_video_gp2x_event_parse, (void *) NULL);

	return 0;

err0:	s_free(priv);
	server->driver->driver_data = NULL;
	return -1;
}

void s_video_gp2x_server_surface_update (s_rect_t *coor)
{
}

void s_video_gp2x_server_fullscreen (void)
{
}

#endif /* VIDEO_GP2X */
