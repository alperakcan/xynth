/***************************************************************************
    begin                : Thu Jan 30 2003
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "xynth_.h"

typedef struct img_data_s {
	int imgf;       /* full screen */
	int imgp;       /* image pos   */
	int imgs;       /* scale image */
	s_list_t *imgl; /* images list */
} img_data_t;

static void img_show (s_window_t *window)
{
	char *name;
	char *sbuf;
	s_image_t *img;
	s_surface_t *srf;
	img_data_t *imgd;

	imgd = window->data;

	name = (char *) s_list_get(imgd->imgl, imgd->imgp);
	if (name == NULL) {
		return;
	}
	
	s_fillbox(window->surface, 0, 0, window->surface->width, window->surface->height, 0);

	s_image_init(&img);
	s_image_img(name, img);

	sbuf = (char *) s_calloc(1, sizeof(char) * window->surface->width * window->surface->height * window->surface->bytesperpixel);
	if (s_surface_create(&srf, img->w, img->h, window->surface->bitsperpixel)) {
		goto out;
	}
	s_putboxrgba(srf, 0, 0, img->w, img->h, img->rgba);

	if (imgd->imgs) {
		s_scalebox(srf, img->w, img->h, srf->vbuf, window->surface->buf->w, window->surface->buf->h, sbuf);
	}

	window->surface->mode = SURFACE_VIRTUAL;
	s_fillbox(window->surface, 0, 0, window->surface->width, window->surface->height, 0);
	window->surface->mode |= SURFACE_REAL;

	if (imgd->imgs) {
		s_putbox(window->surface, 0, 0, window->surface->buf->w, window->surface->buf->h, sbuf);
	} else {
		s_putbox(window->surface, img->x, img->y, img->w, img->h, srf->vbuf);
	}

	s_surface_destroy(srf);
out:	s_image_uninit(img);
        s_free(sbuf);
}

static void img_atevent (s_window_t *window, s_event_t *event)
{
	img_data_t *imgd;
	imgd = window->data;

	if (event->type & CONFIG_EVENT) {
		if (event->type & (CONFIG_CHNGW |  CONFIG_CHNGH)) {
			img_show(window);
		}
	}
	if ((event->type & KEYBD_EVENT) &&
	    (event->type & KEYBD_PRESSED)) {
		switch (event->keybd->button) {
			case S_KEYCODE_q:
			case S_KEYCODE_Q:
				s_window_quit(window);
				break;
			case S_KEYCODE_r:
			case S_KEYCODE_R:
				img_show(window);
				break;
			case S_KEYCODE_s:
			case S_KEYCODE_S:
				imgd->imgs = (imgd->imgs + 1) % 2;
				img_show(window);
				break;
			case S_KEYCODE_f:
			case S_KEYCODE_F:
				imgd->imgf = (imgd->imgf + 1) % 2;
				s_window_set_alwaysontop(window, imgd->imgf);
				if (imgd->imgf) {
					s_window_set_coor(window, WINDOW_NOFORM, 0, 0, window->surface->width, window->surface->height);
				} else {
				        s_window_set_coor(window, WINDOW_NOFORM, window->surface->width / 4,
                                	                                         window->surface->height / 4,
                                                                          	 window->surface->width / 2,
                                                                                 window->surface->height / 2);
				}
				img_show(window);
				break;
			case S_KEYCODE_UP:
			case S_KEYCODE_PAGEUP:
			case S_KEYCODE_RETURN:
				if (imgd->imgl->nb_elt <= (imgd->imgp + 1)) {
					break;
				}
				imgd->imgp++;
				img_show(window);
				break;
			case S_KEYCODE_DOWN:
			case S_KEYCODE_DELETE:
			case S_KEYCODE_PAGEDOWN:
				if (imgd->imgp <= 0) {
					break;
				}
				imgd->imgp--;
				img_show(window);
				break;
		}
	}
}

int main (int argc, char *argv[])
{
	int i;
	img_data_t *imgd;
	s_window_t *window;

        if (argc <= 1) {
		debugf(DCLI, "usage: %s image_file", argv[0]);
		return 1;
	}

	imgd = (img_data_t *) s_malloc(sizeof(img_data_t));

	s_window_init(&window);
	s_window_new(window, WINDOW_MAIN, NULL);
        s_window_set_title(window, "Demo - %s ", argv[0]);

        window->data = imgd;

        imgd->imgf = 0;
        imgd->imgp = 0;
        imgd->imgs = 0;
        s_list_init(&(imgd->imgl));

        for (i = 1; i < argc; i++) {
		s_list_add(imgd->imgl, strdup(argv[i]), -1);
	}

        s_window_set_coor(window, WINDOW_NOFORM, window->surface->width / 4,
                                                 window->surface->height / 4,
                                                 window->surface->width / 2,
                                                 window->surface->height / 2);
	img_show(window);
	
	s_window_atevent(window, img_atevent);
	
	s_window_show(window);
        s_window_main(window);

        while (!s_list_eol(imgd->imgl, 0)) {
		char *tmp = (char *) s_list_get(imgd->imgl, 0);
		s_list_remove(imgd->imgl, 0);
		s_free(tmp);
	}
	s_free(imgd->imgl);
	s_free(imgd);
	
	return 0;
}
