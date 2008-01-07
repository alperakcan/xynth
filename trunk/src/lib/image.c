/***************************************************************************
    begin                : Thu Jun 3 2004
    copyright            : (C) 2004 - 2008 by Alper Akcan
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

#include "xynth_.h"

int s_image_hex2int (char *str)
{
    int i = 0;
    int r = 0;

    for (i = 0; i < 2; i++) {
	switch (str[i]) {
	    case '1':	r += 1;		break;
	    case '2':	r += 2;		break;
	    case '3':	r += 3;		break;
	    case '4':	r += 4;		break;
	    case '5':	r += 5;		break;
	    case '6':	r += 6;		break;
	    case '7':	r += 7;		break;
	    case '8':	r += 8;		break;
	    case '9':	r += 9;		break;
	    case 'a':
	    case 'A':	r += 10;	break;
	    case 'b':
	    case 'B':	r += 11;	break;
	    case 'c':
	    case 'C':	r += 12;	break;
	    case 'd':
	    case 'D':	r += 13;	break;
	    case 'e':
	    case 'E':	r += 14;	break;
	    case 'f':
	    case 'F':	r += 15;	break;
	}
	if (!i) {
	    r *= 16;
	}
    }

    return r;
}

int s_image_get_mat (s_image_t *img)
{
        int i;
        unsigned int *d;
        unsigned char *m;

        if (img->mat != NULL) {
		s_image_free_mat(img);
	}
	
        d = img->rgba;
        i = img->w * img->h;
        
	img->mat = (unsigned char *) s_calloc(i, sizeof(char));

	m = img->mat;
	while (i--) {
		*m++ = (~*d++ & 0xFF);
	}

	return 0;
}

int s_image_get_buf (s_surface_t *surface, s_image_t *img)
{
        s_surface_t *s;
        if (img->buf != NULL) {
		s_image_free_buf(img);
	}
	img->buf = (char *) s_malloc(img->w * img->h * surface->bytesperpixel + 1);
	s_surface_create_from_data(&s, img->w, img->h, surface->bitsperpixel, img->buf);
	s_putboxrgb(s, 0, 0, img->w, img->h, img->rgba);
	s_surface_destroy(s);
	return 0;
}

void s_image_get_handler (s_image_t *img)
{
	int i;
	int j;
	int x;
	int y;
	unsigned int *rgba_tmp;

        if (img->handler != NULL) {
		s_image_free_handler(img);
	}

	img->handler = (s_rect_t *) s_malloc(sizeof(s_rect_t));
	img->handler->x = 0;
	img->handler->y = 0;
	img->handler->w = 0;
	img->handler->h = 0;

	j = 1;
	i = img->w;
	rgba_tmp = img->rgba;

	for (y = 0; y < img->h; y++) {
		for (x = 0; x < img->w; x++) {
			if (j && (~*rgba_tmp & 0xFF)) {
				img->handler->y = y;
				j = 0;
			}
			if (~*rgba_tmp & 0xFF) {
				if (x < i) {
					i = x;
				}
			}
			rgba_tmp++;
		}
	}
	img->handler->x = i;

        j = 1;
        i = img->w;
	rgba_tmp = img->rgba + (img->h * img->w - 1);
	for (y = 0; y < img->h; y++) {
		for (x = 0; x < img->w; x++) {
			if (j && (~*rgba_tmp & 0xFF)) {
				img->handler->h = (img->h - y) - img->handler->y;
				j = 0;
			}
			if (~*rgba_tmp & 0xFF) {
				if (x < i) {
					i = x;
				}
			}
			rgba_tmp--;
		}
	}
	img->handler->w = (img->w - i) - img->handler->x;
}

int s_image_init (s_image_t **img)
{
	(*img) = (s_image_t *) s_calloc(1, sizeof(s_image_t));
	s_image_layers_init(*img);
	return 0;
}

void s_image_free_buf (s_image_t *img)
{
	s_free(img->buf);
	img->buf = NULL;
}

void s_image_free_mat (s_image_t *img)
{
	s_free(img->mat);
	img->mat = NULL;
}

void s_image_free_rgba (s_image_t *img)
{
	s_free(img->rgba);
	img->rgba = NULL;
}

void s_image_free_handler (s_image_t *img)
{
	s_free(img->handler);
	img->handler = NULL;
}

int s_image_uninit (s_image_t *img)
{
	if (img == NULL) {
		return 0;
	}
	s_image_free_buf(img);
	s_image_free_mat(img);
	s_image_free_rgba(img);
	s_image_free_handler(img);
	s_image_layers_uninit(img);
	s_free(img->name);
	s_free(img);
	img = NULL;
	return 0;
}

int s_image_layer_init (s_image_t **lyr)
{
	return s_image_init(lyr);
}

int s_image_layer_uninit (s_image_t *lyr)
{
	return s_image_uninit(lyr);
}

int s_image_layers_init (s_image_t *img)
{
	return s_list_init(&(img->layers));
}

int s_image_layers_uninit (s_image_t *img)
{
	s_image_t *img_;

	while (!s_list_eol(img->layers, 0)) {
		img_ = (s_image_t *) s_list_get(img->layers, 0);
		s_list_remove(img->layers, 0);
		s_image_layer_uninit(img_);
	}
	s_list_uninit(img->layers);
	img->layers = NULL;

	return 0;
}

int s_image_img (char *file, s_image_t *img)
{
	img->name = strdup(file);
	if (!s_image_png_is(file)) {
		return s_image_png(file, img);
	} else if (!s_image_xpm_is(file)) {
		return s_image_xpm(file, img);
	} else if (!s_image_gif_is(file)) {
		return s_image_gif(file, img);
	} else {
		debugf(DFAT, "Unknown image format (%s)", file);
	}
	return -1;
}
