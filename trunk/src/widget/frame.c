/***************************************************************************
    begin                : Fri Sep 8 2006
    copyright            : (C) 2006 - 2007 by Alper Akcan
    email                : distchx@yahoo.com
 **************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#include <stdarg.h>
#include <unistd.h>

#include "../lib/xynth_.h"
#include "widget.h"

int w_frame_set_border (w_object_t *object, int border)
{
	w_frame_t *frame;
	frame = (w_frame_t *) object->data[OBJECT_FRAME];
	frame->linewidth = border;
	return 0;
}

int w_frame_set_style (w_object_t *object, FRAME_SHAPE shape, FRAME_SHADOW shadow)
{
        w_frame_t *frame;
        frame = (w_frame_t *) object->data[OBJECT_FRAME];
        frame->style = shape | shadow;
        return 0;
}

int w_frame_image_init (w_frame_image_t **fimg)
{
	*fimg = s_malloc(sizeof(w_frame_image_t));
	memset(*fimg, 0, sizeof(w_frame_image_t));
	s_list_init(&((*fimg)->images));
	s_list_init(&((*fimg)->names));
	return 0;
}

int w_frame_image_uninit (w_frame_image_t *fimg)
{
	char *name;
	s_image_t *img;
	while (!s_list_eol(fimg->images, 0)) {
		img = (s_image_t *) s_list_get(fimg->images, 0);
		s_list_remove(fimg->images, 0);
		s_image_uninit(img);
	}
	s_list_uninit(fimg->images);
	while (!s_list_eol(fimg->names, 0)) {
		name = (char *) s_list_get(fimg->names, 0);
		s_list_remove(fimg->names, 0);
		s_free(name);
	}
	s_list_uninit(fimg->names);
	s_free(fimg);
	return 0;
}

int w_frame_set_image (w_object_t *object, unsigned int style, unsigned int rotation, unsigned int nimgs, char **imgs)
{
	int pos;
	char *file;
        w_frame_t *frame;
	w_frame_image_t *fimg;
        frame = (w_frame_t *) object->data[OBJECT_FRAME];
	for (pos = 0; !s_list_eol(frame->images, pos); pos++) {
		fimg = (w_frame_image_t *) s_list_get(frame->images, pos);
		if (fimg->style == style) {
			s_list_remove(frame->images, pos);
			w_frame_image_uninit(fimg);
			break;
		}
	}
	for (pos = 0; pos < nimgs; pos++) {
		if (access(imgs[pos], R_OK)) {
			return -1;
		}
	}
	w_frame_image_init(&fimg);
	fimg->style = style;
	fimg->rotation = rotation;
	for (pos = 0; pos < nimgs; pos++) {
		file = imgs[pos];
#if defined(WIDGET_OPTIMIZE_MEMORY) && WIDGET_OPTIMIZE_MEMORY >= 1
#else
		w_window_image_add(object->window, file);
#endif
		s_list_add(fimg->names, strdup(file), -1);
	}
	s_list_add(frame->images, fimg, -1);
	return 0;
}

void w_frame_draw_image (w_object_t *object, w_frame_image_t *fimg)
{
	int i;
	int j;
	switch (fimg->names->nb_elt) {
		case 1:
		{
			char *name;
			s_image_t *img;
			name = (char *) s_list_get(fimg->names, 0);
#if defined(WIDGET_OPTIMIZE_MEMORY) && WIDGET_OPTIMIZE_MEMORY >= 1
			s_image_init(&img);
			s_image_img(name, img);
#else
			img = w_window_image_get(object->window, name);
#endif
			s_image_get_mat(img);
			if (fimg->rotation == FRAME_IMAGE_SOLID) {
				s_putmaskpart(object->surface->matrix, object->surface->width, object->surface->height,
				              0, 0, img->w, img->h, img->w, img->h, img->mat, 0, 0);
				s_putboxrgb(object->surface, 0, 0, img->w, img->h, img->rgba);
			} else if (fimg->rotation == FRAME_IMAGE_VERTICAL) {
				for (i = 0; i < object->surface->height; i++) {
					s_putmaskpart(object->surface->matrix, object->surface->width, object->surface->height,
					              0, i, img->w, img->h, img->w, img->h, img->mat, 0, 0);
					s_putboxrgb(object->surface, 0, i, img->w, img->h, img->rgba);
				}
			}
			s_image_free_mat(img);
#if defined(WIDGET_OPTIMIZE_MEMORY) && WIDGET_OPTIMIZE_MEMORY >= 1
			s_image_uninit(img);
#endif
			break;	
		}
		case 9:
		{
			char *name[9];
			s_image_t *imgs[9];
			name[0] = (char *) s_list_get(fimg->names, 0);
			name[1] = (char *) s_list_get(fimg->names, 1);
			name[2] = (char *) s_list_get(fimg->names, 2);
			name[3] = (char *) s_list_get(fimg->names, 3);
			name[4] = (char *) s_list_get(fimg->names, 4);
			name[5] = (char *) s_list_get(fimg->names, 5);
			name[6] = (char *) s_list_get(fimg->names, 6);
			name[7] = (char *) s_list_get(fimg->names, 7);
			name[8] = (char *) s_list_get(fimg->names, 8);
#if defined(WIDGET_OPTIMIZE_MEMORY) && WIDGET_OPTIMIZE_MEMORY >= 1
			s_image_init(&imgs[0]);
			s_image_img(name[0], imgs[0]);
			s_image_init(&imgs[1]);
			s_image_img(name[1], imgs[1]);
			s_image_init(&imgs[2]);
			s_image_img(name[2], imgs[2]);
			s_image_init(&imgs[3]);
			s_image_img(name[3], imgs[3]);
			s_image_init(&imgs[4]);
			s_image_img(name[4], imgs[4]);
			s_image_init(&imgs[5]);
			s_image_img(name[5], imgs[5]);
			s_image_init(&imgs[6]);
			s_image_img(name[6], imgs[6]);
			s_image_init(&imgs[7]);
			s_image_img(name[7], imgs[7]);
			s_image_init(&imgs[8]);
			s_image_img(name[8], imgs[8]);
#else
			imgs[0] = w_window_image_get(object->window, name[0]);
			imgs[1] = w_window_image_get(object->window, name[1]);
			imgs[2] = w_window_image_get(object->window, name[2]);
			imgs[3] = w_window_image_get(object->window, name[3]);
			imgs[4] = w_window_image_get(object->window, name[4]);
			imgs[5] = w_window_image_get(object->window, name[5]);
			imgs[6] = w_window_image_get(object->window, name[6]);
			imgs[7] = w_window_image_get(object->window, name[7]);
			imgs[8] = w_window_image_get(object->window, name[8]);
#endif
			s_image_get_mat(imgs[0]);
			s_image_get_mat(imgs[1]);
			s_image_get_mat(imgs[2]);
			s_image_get_mat(imgs[3]);
			s_image_get_mat(imgs[4]);
			s_image_get_mat(imgs[5]);
			s_image_get_mat(imgs[6]);
			s_image_get_mat(imgs[7]);
			s_image_get_mat(imgs[8]);
			
			/* 0 1 2
			 * 3 4 5
			 * 6 7 8
			 */
			for (j = 0; j < object->surface->height; j+= imgs[4]->h) {
				for (i = 0; i < object->surface->width; i+= imgs[4]->w) {
					s_putmaskpart(object->surface->matrix, object->surface->width, object->surface->height,
					              i, j, imgs[4]->w, imgs[4]->h, imgs[4]->w, imgs[4]->h, imgs[4]->mat, 0, 0);
					s_putboxrgb(object->surface, i, j, imgs[4]->w, imgs[4]->h, imgs[4]->rgba);
				}
			}
			for (i = 0; i < object->surface->width; i += imgs[1]->w) {
				s_putmaskpart(object->surface->matrix, object->surface->width, object->surface->height,
				              i, 0, imgs[1]->w, imgs[1]->h, imgs[1]->w, imgs[1]->h, imgs[1]->mat, 0, 0);
				s_putboxrgb(object->surface, i, 0, imgs[1]->w, imgs[1]->h, imgs[1]->rgba);
			}
			for (i = 0; i < object->surface->width; i += imgs[7]->w) {
				s_putmaskpart(object->surface->matrix, object->surface->width, object->surface->height,
				              i, object->surface->height - imgs[7]->h, imgs[7]->w, imgs[7]->h, imgs[7]->w, imgs[7]->h, imgs[7]->mat, 0, 0);
				s_putboxrgb(object->surface, i, object->surface->height - imgs[7]->h, imgs[7]->w, imgs[7]->h, imgs[7]->rgba);
			}
			for (i = 0; i < object->surface->height; i += imgs[3]->h) {
				s_putmaskpart(object->surface->matrix, object->surface->width, object->surface->height,
				              0, i, imgs[3]->w, imgs[3]->h, imgs[3]->w, imgs[3]->h, imgs[3]->mat, 0, 0);
				s_putboxrgb(object->surface, 0, i, imgs[3]->w, imgs[3]->h, imgs[3]->rgba);
			}
			for (i = 0; i < object->surface->height; i += imgs[5]->h) {
				s_putmaskpart(object->surface->matrix, object->surface->width, object->surface->height,
				              object->surface->width - imgs[5]->w, i, imgs[5]->w, imgs[5]->h, imgs[5]->w, imgs[5]->h, imgs[5]->mat, 0, 0);
				s_putboxrgb(object->surface, object->surface->width - imgs[5]->w, i, imgs[5]->w, imgs[5]->h, imgs[5]->rgba);
			}
			s_putmaskpart(object->surface->matrix, object->surface->width, object->surface->height,
			              0, 0, imgs[0]->w, imgs[0]->h, imgs[0]->w, imgs[0]->h, imgs[0]->mat, 0, 0);
			s_putboxrgb(object->surface, 0, 0, imgs[0]->w, imgs[0]->h, imgs[0]->rgba);
			s_putmaskpart(object->surface->matrix, object->surface->width, object->surface->height,
			              object->surface->width - imgs[2]->w, 0, imgs[2]->w, imgs[2]->h, imgs[2]->w, imgs[2]->h, imgs[2]->mat, 0, 0);
			s_putboxrgb(object->surface, object->surface->width - imgs[2]->w, 0, imgs[2]->w, imgs[2]->h, imgs[2]->rgba);
			s_putmaskpart(object->surface->matrix, object->surface->width, object->surface->height,
			              0, object->surface->height - imgs[6]->h, imgs[6]->w, imgs[6]->h, imgs[6]->w, imgs[6]->h, imgs[6]->mat, 0, 0);
			s_putboxrgb(object->surface, 0, object->surface->height - imgs[6]->h, imgs[6]->w, imgs[6]->h, imgs[6]->rgba);
			s_putmaskpart(object->surface->matrix, object->surface->width, object->surface->height,
			              object->surface->width - imgs[8]->w, object->surface->height - imgs[8]->h, imgs[8]->w, imgs[8]->h, imgs[8]->w, imgs[8]->h, imgs[8]->mat, 0, 0);
			s_putboxrgb(object->surface, object->surface->width - imgs[8]->w, object->surface->height - imgs[8]->h, imgs[8]->w, imgs[8]->h, imgs[8]->rgba);

			s_image_free_mat(imgs[0]);
			s_image_free_mat(imgs[1]);
			s_image_free_mat(imgs[2]);
			s_image_free_mat(imgs[3]);
			s_image_free_mat(imgs[4]);
			s_image_free_mat(imgs[5]);
			s_image_free_mat(imgs[6]);
			s_image_free_mat(imgs[7]);
			s_image_free_mat(imgs[8]);
#if defined(WIDGET_OPTIMIZE_MEMORY) && WIDGET_OPTIMIZE_MEMORY >= 1
			s_image_uninit(imgs[0]);
			s_image_uninit(imgs[1]);
			s_image_uninit(imgs[2]);
			s_image_uninit(imgs[3]);
			s_image_uninit(imgs[4]);
			s_image_uninit(imgs[5]);
			s_image_uninit(imgs[6]);
			s_image_uninit(imgs[7]);
			s_image_uninit(imgs[8]);
#endif
			break;
		}
		case 3:
		{
			char *name[3];
			s_image_t *imgs[3];
			name[0] = (char *) s_list_get(fimg->names, 0);
			name[1] = (char *) s_list_get(fimg->names, 1);
			name[2] = (char *) s_list_get(fimg->names, 2);
#if defined(WIDGET_OPTIMIZE_MEMORY) && WIDGET_OPTIMIZE_MEMORY >= 1
			s_image_init(&imgs[0]);
			s_image_img(name[0], imgs[0]);
			s_image_init(&imgs[1]);
			s_image_img(name[1], imgs[1]);
			s_image_init(&imgs[2]);
			s_image_img(name[2], imgs[2]);
#else
			imgs[0] = w_window_image_get(object->window, name[0]);
			imgs[1] = w_window_image_get(object->window, name[1]);
			imgs[2] = w_window_image_get(object->window, name[2]);
#endif
			s_image_get_mat(imgs[0]);
			s_image_get_mat(imgs[1]);
			s_image_get_mat(imgs[2]);
			if (fimg->rotation == FRAME_IMAGE_VERTICAL) {
				s_putmaskpart(object->surface->matrix, object->surface->width, object->surface->height,
				              0, 0, imgs[0]->w, imgs[0]->h, imgs[0]->w, imgs[0]->h, imgs[0]->mat, 0, 0);
				s_putboxrgb(object->surface, 0, 0, imgs[0]->w, imgs[0]->h, imgs[0]->rgba);
				for (i = imgs[0]->h; i < object->surface->height - imgs[2]->h; i++) {
					s_putmaskpart(object->surface->matrix, object->surface->width, object->surface->height,
					              0, i, imgs[1]->w, imgs[1]->h, imgs[1]->w, imgs[1]->h, imgs[1]->mat, 0, 0);
					s_putboxrgb(object->surface, 0, i, imgs[1]->w, imgs[1]->h, imgs[1]->rgba);
				}
				s_putmaskpart(object->surface->matrix, object->surface->width, object->surface->height,
				              0, object->surface->height - imgs[2]->h, imgs[2]->w, imgs[2]->h, imgs[2]->w, imgs[2]->h,
				              imgs[2]->mat, 0, 0);
				s_putboxrgb(object->surface, 0, object->surface->height - imgs[2]->h, imgs[2]->w, imgs[2]->h, imgs[2]->rgba);
			} else if (fimg->rotation == FRAME_IMAGE_HORIZONTAL) {
				s_putmaskpart(object->surface->matrix, object->surface->width, object->surface->height,
				              0, 0, imgs[0]->w, imgs[0]->h, imgs[0]->w, imgs[0]->h, imgs[0]->mat, 0, 0);
				s_putboxrgb(object->surface, 0, 0, imgs[0]->w, imgs[0]->h, imgs[0]->rgba);
				for (i = imgs[0]->w; i < object->surface->width - imgs[2]->w; i++) {
					s_putmaskpart(object->surface->matrix, object->surface->width, object->surface->height,
					              i, 0, imgs[1]->w, imgs[1]->h, imgs[1]->w, imgs[1]->h, imgs[1]->mat, 0, 0);
					s_putboxrgb(object->surface, i, 0, imgs[1]->w, imgs[1]->h, imgs[1]->rgba);
				}
				s_putmaskpart(object->surface->matrix, object->surface->width, object->surface->height,
				              object->surface->width - imgs[2]->w, 0, imgs[2]->w, imgs[2]->h, imgs[2]->w, imgs[2]->h,
				              imgs[2]->mat, 0, 0);
				s_putboxrgb(object->surface, object->surface->width - imgs[2]->w, 0, imgs[2]->w, imgs[2]->h, imgs[2]->rgba);
			}
			s_image_free_mat(imgs[0]);
			s_image_free_mat(imgs[1]);
			s_image_free_mat(imgs[2]);
#if defined(WIDGET_OPTIMIZE_MEMORY) && WIDGET_OPTIMIZE_MEMORY >= 1
			s_image_uninit(imgs[0]);
			s_image_uninit(imgs[1]);
			s_image_uninit(imgs[2]);
#endif
			break;
		}
	}				
}

void w_frame_draw (w_object_t *object)
{
	int i;
	int j;
	int c[4];
	w_frame_t *frame;
	w_frame_image_t *fimg;
	frame = (w_frame_t *) object->data[OBJECT_FRAME];

	if (object->surface->vbuf == NULL) {
		return;
	}

	if (object->focused) {
		frame->style |=  FRAME_FOCUSED;
	} else {
		frame->style &=  ~FRAME_FOCUSED;
	}

        for (i = 0; !s_list_eol(frame->images, i); i++) {
        	fimg = (w_frame_image_t *) s_list_get(frame->images, i);
        	if (frame->style == fimg->style) {
        		memset(object->surface->matrix, 0x00, sizeof(char) * object->surface->width * object->surface->height);
        		w_frame_draw_image(object, fimg);
        		return;
        	}
        }
        
	memset(object->surface->matrix, 0xff, sizeof(char) * object->surface->width * object->surface->height);
        switch (frame->style & FRAME_MSHAPE) {
        	case FRAME_EMPTY:
        		s_free(object->surface->matrix);
        		s_free(object->surface->vbuf);
        		object->surface->matrix = NULL;
        		object->surface->vbuf = NULL;
        		return;
		case FRAME_NOFRAME:
			memset(object->surface->matrix, 0x00, sizeof(char) * object->surface->width * object->surface->height);
			return;
		case FRAME_BOX:
		case FRAME_GROUPBOXPANEL:
			switch (frame->style & FRAME_MSHADOW) {
				case FRAME_PLAIN:
sbox_plain:				s_fillbox(object->surface, 0, 0, object->surface->buf->w, object->surface->buf->h, s_rgbcolor(object->surface, 0, 0, 0));
					break;
				case FRAME_RAISED:
					c[0] = s_rgbcolor(object->surface, 96, 96, 96);
					c[1] = s_rgbcolor(object->surface, 128, 128, 128);
					c[2] = s_rgbcolor(object->surface, 255, 255, 255);
sbox_raised:				for (i = 0; i < frame->linewidth; i++) {
						s_fillbox(object->surface, i, i, object->surface->buf->w - (i * 2), object->surface->buf->h - (i * 2), c[2]);
						s_fillbox(object->surface, i + 1, i + 1, object->surface->buf->w - (i * 2) - 1, object->surface->buf->h - (i * 2) - 1, c[0]);
					}
					s_fillbox(object->surface, i, i, object->surface->buf->w - (i * 2), object->surface->buf->h - (i * 2), c[1]);
					for (j = 0; j < frame->linewidth; j++) {
						s_fillbox(object->surface, j + i + frame->midlinewidth, j + i + frame->midlinewidth, object->surface->buf->w - ((j + i + frame->midlinewidth) * 2), object->surface->buf->h - ((j + i + frame->midlinewidth) * 2), c[0]);
						s_fillbox(object->surface, j + 1 + i + frame->midlinewidth, j + 1 + i + frame->midlinewidth, object->surface->buf->w - ((j + i + frame->midlinewidth) * 2) - 1, object->surface->buf->h - ((j + i + frame->midlinewidth) * 2) - 1, c[2]);
					}
					break;
				case FRAME_SUNKEN:
					c[2] = s_rgbcolor(object->surface, 96, 96, 96);
					c[1] = s_rgbcolor(object->surface, 128, 128, 128);
					c[0] = s_rgbcolor(object->surface, 255, 255, 255);
					goto sbox_raised;
					break;
			}
			break;
		case FRAME_PANEL:
		case FRAME_STYLEDPANEL:
			switch (frame->style & FRAME_MSHADOW) {
				case FRAME_PLAIN:
					goto sbox_plain;
					break;
				case FRAME_RAISED:
					c[0] = s_rgbcolor(object->surface, 96, 96, 96);
					c[1] = s_rgbcolor(object->surface, 255, 255, 255);
spanel_raised:				for (i = 0; i < frame->linewidth; i++) {
						s_fillbox(object->surface, i, i, object->surface->buf->w - (i * 2), object->surface->buf->h - (i * 2), c[0]);
						s_fillbox(object->surface, i, i, object->surface->buf->w - (i * 2) - 1, object->surface->buf->h - (i * 2) - 1, c[1]);
					}
					break;
				case FRAME_SUNKEN:
					c[1] = s_rgbcolor(object->surface, 96, 96, 96);
					c[0] = s_rgbcolor(object->surface, 255, 255, 255);
					goto spanel_raised;
					break;
			}
			break;
		case FRAME_WINPANEL:
		case FRAME_POPUPPANEL:
			switch (frame->style & FRAME_MSHADOW) {
				case FRAME_PLAIN:
					goto sbox_plain;
					break;
				case FRAME_RAISED:
swinpanel_raised:			c[0] = s_rgbcolor(object->surface, 0, 0, 0);
					c[1] = s_rgbcolor(object->surface, 96, 96, 96);
					c[2] = s_rgbcolor(object->surface, 220, 220, 220);
					c[3] = s_rgbcolor(object->surface, 255, 255, 255);
					s_fillbox(object->surface, 0, 0, object->surface->buf->w, object->surface->buf->h, c[0]);
					s_fillbox(object->surface, 0, 0, object->surface->buf->w - 1, object->surface->buf->h - 1, c[2]);
					s_fillbox(object->surface, 1, 1, object->surface->buf->w - 2, object->surface->buf->h - 2, c[1]);
					s_fillbox(object->surface, 1, 1, object->surface->buf->w - 3, object->surface->buf->h - 3, c[3]);
					break;
swinpanel_sunken:		case FRAME_SUNKEN:
					c[0] = s_rgbcolor(object->surface, 0, 0, 0);
					c[1] = s_rgbcolor(object->surface, 96, 96, 96);
					c[2] = s_rgbcolor(object->surface, 220, 220, 220);
					c[3] = s_rgbcolor(object->surface, 255, 255, 255);
					s_fillbox(object->surface, 0, 0, object->surface->buf->w, object->surface->buf->h, c[1]);
					s_fillbox(object->surface, 1, 1, object->surface->buf->w - 1, object->surface->buf->h - 1, c[2]);
					s_fillbox(object->surface, 1, 1, object->surface->buf->w - 2, object->surface->buf->h - 2, c[0]);
					s_fillbox(object->surface, 2, 2, object->surface->buf->w - 3, object->surface->buf->h - 3, c[3]);
					break;
			}
			break;
		case FRAME_MENUBARPANEL:
			s_fillbox(object->surface, 0, 0, object->surface->buf->w, object->surface->buf->h, s_rgbcolor(object->surface, 220, 220, 220));
			break;
		case FRAME_TOOLBARPANEL:
			c[0] = s_rgbcolor(object->surface, 96, 96, 96);
			c[1] = s_rgbcolor(object->surface, 255, 255, 255);
			s_fillbox(object->surface, 0, 0, object->surface->buf->w, object->surface->buf->h, c[0]);
			s_fillbox(object->surface, 0, 0, object->surface->buf->w - 1, object->surface->buf->h - 1, c[1]);
			break;
		case FRAME_LINEEDITPANEL:
		case FRAME_TABWIDGETPANEL:
			switch (frame->style & FRAME_MSHADOW) {
				case FRAME_PLAIN:
				case FRAME_RAISED:
					goto swinpanel_raised;
					break;
				case FRAME_SUNKEN:
					goto swinpanel_sunken;
					break;
			}
			break;
		case FRAME_HLINE:
		case FRAME_VLINE:
			break;
	}

	s_fillbox(object->surface, object->content->x, 
	                           object->content->y,
	                           object->content->w,
	                           object->content->h,
	                           s_rgbcolor(object->surface, 220, 220, 220));
}

void w_frame_geometry (w_object_t *object)
{
        w_frame_t *frame;
        frame = (w_frame_t *) object->data[OBJECT_FRAME];

	switch (frame->style & FRAME_MSHAPE) {
		case FRAME_NOFRAME:
		case FRAME_MENUBARPANEL:
		case FRAME_TOOLBARPANEL:
			frame->linewidth = 1;
			frame->midlinewidth = 0;
			break;
		case FRAME_PANEL:
		case FRAME_STYLEDPANEL:
			frame->midlinewidth = 0;
			break;
		case FRAME_WINPANEL:
		case FRAME_POPUPPANEL:
		case FRAME_LINEEDITPANEL:
		case FRAME_TABWIDGETPANEL:
			frame->linewidth = 2;
			frame->midlinewidth = 0;
			break;
		case FRAME_BOX:
		case FRAME_HLINE:
		case FRAME_VLINE:
		case FRAME_GROUPBOXPANEL:
			if ((frame->style & FRAME_MSHADOW) == FRAME_PLAIN) {
				frame->midlinewidth = 0;
			}
			break;
	}

	switch (frame->style & FRAME_MSHAPE) {
		case FRAME_NOFRAME:
			w_object_set_content(object, 0, 0, object->surface->buf->w, object->surface->buf->h);
			return;
		case FRAME_BOX:
		case FRAME_HLINE:
		case FRAME_VLINE:
		case FRAME_GROUPBOXPANEL:
			switch (frame->style & FRAME_MSHADOW) {
				case FRAME_PLAIN:
sbox_plain:				w_object_set_content(object, frame->linewidth, frame->linewidth,
                                                                     object->surface->buf->w - (frame->linewidth * 2),
                                                                     object->surface->buf->h - (frame->linewidth * 2));
					break;
				case FRAME_RAISED:
				case FRAME_SUNKEN:
					w_object_set_content(object, (frame->linewidth * 2) + (frame->midlinewidth),
					                             (frame->linewidth * 2) + (frame->midlinewidth),
					                             object->surface->buf->w - (frame->linewidth * 4) - (frame->midlinewidth * 2),
					                             object->surface->buf->h - (frame->linewidth * 4) - (frame->midlinewidth * 2));
					break;
			}
			break;
		case FRAME_PANEL:
		case FRAME_WINPANEL:
		case FRAME_POPUPPANEL:
		case FRAME_STYLEDPANEL:
		case FRAME_MENUBARPANEL:
		case FRAME_TOOLBARPANEL:
		case FRAME_LINEEDITPANEL:
		case FRAME_TABWIDGETPANEL:
			goto sbox_plain;
			break;
	}
}

int w_frame_init (w_window_t *window, w_frame_t **frame, unsigned int style, w_object_t *parent)
{
	(*frame) = (w_frame_t *) s_malloc(sizeof(w_frame_t));
	(*frame)->style = style;
	(*frame)->linewidth = 1;
	(*frame)->midlinewidth = 0;
	if (w_object_init(window, &((*frame)->object), w_frame_draw, parent)) {
		goto err0;
	}
	(*frame)->object->type = OBJECT_FRAME;
	(*frame)->object->data[OBJECT_FRAME] = *frame;
	(*frame)->object->geometry = w_frame_geometry;
	(*frame)->object->destroy = w_frame_uninit;
	s_list_init(&((*frame)->images));
	return 0;
err0:	s_free(*frame);
	return -1;
}

void w_frame_uninit (w_object_t *object)
{
	w_frame_t *frame;
	w_frame_image_t *fimg;
	frame = (w_frame_t *) object->data[OBJECT_FRAME];
	w_object_uninit(object);
	while (!s_list_eol(frame->images, 0)) {
		fimg = s_list_get(frame->images, 0);
		s_list_remove(frame->images, 0);
		w_frame_image_uninit(fimg);
	}
	s_list_uninit(frame->images);
	s_free(frame);
}
