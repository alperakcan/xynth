/***************************************************************************
    begin                : Fri Dec 30 2005
    copyright            : (C) 2005 - 2007 by Caglar Oral
    email                : caglaroral@yahoo.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#if defined(VIDEO_GDI)

#include "../../../lib/xynth_.h"
#include "server.h"
#include "../helper/helper.h"
#include "gdi.h"

s_video_input_t s_video_gdi_input_keybd = {
	VIDEO_INPUT_KEYBD,
	s_video_gdi_kbd_init,
	s_video_gdi_kbd_update,
	s_video_gdi_kbd_uninit,
};

s_video_input_t s_video_gdi_input_mouse = {
	VIDEO_INPUT_MOUSE,
	s_video_gdi_mouse_init,
	s_video_gdi_mouse_update,
	s_video_gdi_mouse_uninit,
};

s_video_input_t *s_video_gdi_input[] = {
	&s_video_gdi_input_keybd,
	&s_video_gdi_input_mouse,
	NULL,
};

s_video_driver_t s_video_gdi = {
	"gdi",
	"/dev/gdi",
	s_video_gdi_input,
	s_video_gdi_server_init,
	s_video_gdi_server_uninit,
	NULL,
	NULL,
	NULL,
	s_video_gdi_server_surface_update,
	s_video_gdi_server_fullscreen,
	NULL
};

void s_video_gdi_server_uninit (void)
{
	PostQuitMessage(0);
}

int s_video_gdi_server_init (s_server_conf_t *cfg)
{
	void *addr;

	s_video_helper_mode_info_t *gmode;
	s_video_gdi_data_t *priv;

	priv = (s_video_gdi_data_t *) s_calloc(1, sizeof(s_video_gdi_data_t));
	server->driver->driver_data = (void *) priv;

	if (s_video_helper_mode_find(cfg, &gmode)) {
		debugf(DSER, "Couldn't find mode: %s", cfg->general.mode);
		goto err0;
	}else {
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
	}

	addr = (void *) s_malloc(sizeof(char) * server->window->surface->width * server->window->surface->height * server->window->surface->bytesperpixel);

	server->window->surface->linear_mem_base = (unsigned int) addr;
	server->window->surface->linear_mem_size = (unsigned int) (sizeof(char) * server->window->surface->width * server->window->surface->height * server->window->surface->bytesperpixel);

	server->window->surface->vbuf = (char *) addr;
	server->window->surface->linear_buf = (char *) addr;

	server->window->surface->need_expose = SURFACE_NEEDEXPOSE;

	/* We need seperate thread for events so event loop and the
	   window creation are in the same thread.

	   Applications with multiple threads can include a
	   message loop in each thread that creates a window.

	   http://msdn.microsoft.com/library/default.asp?url=/library/en-us/winui/winui/
	   windowsuserinterface/windowing/messagesandmessagequeues/
	   aboutmessagesandmessagequeues.asp
	*/
       	priv->event_tid = s_thread_create(&s_video_gdi_create_window, (void *) NULL);

	return gmode->number;

err0:	PostQuitMessage(0);
	return -1;
}

void s_video_gdi_server_surface_update (s_rect_t *coor)
{
	char *ptr;
	char *tmp;
	BITMAP bm;
	HDC memoryDC;
	HBITMAP hBtmp;
	HDC mainwindow;
	s_rect_t inter;
	
	s_rect_t clip = {0, 0, server->window->surface->width, server->window->surface->height};
	s_video_gdi_data_t *priv = (s_video_gdi_data_t *) server->driver->driver_data;

        if (priv->bpp_windows == 0) {
                return;
        }
	if (s_rect_intersect(&clip, coor, &inter)) {
		return;
	}

	ptr = (char *) s_malloc(inter.w * inter.h * server->window->surface->bytesperpixel);
	s_getbox(server->window->surface, inter.x, inter.y, inter.w, inter.h, ptr);

	mainwindow = GetDC(priv->hwndMain);
	if (mainwindow == NULL) {
		debugf(DSER, "GetDC(priv->hwndMain) == NULL");
		goto err0;
	}
	if (priv->bpp_windows != server->window->surface->bitsperpixel) {
                s_copybuffer(ptr, server->window->surface->bitsperpixel, &tmp, priv->bpp_windows, inter.w, inter.h);
	} else {
		tmp = ptr;
        }
	
	hBtmp = CreateBitmap(inter.w, inter.h, 1, priv->bpp_windows, tmp);
	if (hBtmp == NULL) {
		debugf(DSER, "CreateBitmap(inter.w, inter.h, 1, priv->bpp_windows, tmp) == NULL");
		goto err1;
	}
	memoryDC = CreateCompatibleDC(mainwindow);
	if (memoryDC == NULL) {
		debugf(DSER, "CreateCompatibleDC(mainwindow) == NULL");
		goto err2;
        }
	if (GetObject(hBtmp, sizeof(BITMAP), (LPVOID) &bm) == 0) {
		debugf(DSER, "GetObject(hBtmp, sizeof(BITMAP), (LPVOID) &bm) == 0");
		goto err3;
	}
	if (SelectObject(memoryDC, hBtmp) == NULL) {
		debugf(DSER, "SelectObject(memoryDC, hBtmp) == NULL");
		goto err4;
	}
	BitBlt(mainwindow, inter.x, inter.y, inter.w, inter.h, memoryDC, 0, 0, SRCCOPY);
	DeleteDC(memoryDC);
	DeleteObject(hBtmp);
	ReleaseDC(priv->hwndMain, mainwindow);

	s_free(ptr);
	if (priv->bpp_windows != server->window->surface->bitsperpixel) {
		s_free(tmp);
        }
        return;

err4:
err3:   DeleteDC(memoryDC);
err2:	DeleteObject(hBtmp);
err1:	ReleaseDC(priv->hwndMain, mainwindow);
       	if (priv->bpp_windows != server->window->surface->bitsperpixel) {
		s_free(tmp);
        }
err0:   free(ptr);
}

#endif /* VIDEO_GDI */
