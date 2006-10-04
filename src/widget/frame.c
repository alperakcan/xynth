/***************************************************************************
    begin                : Fri Sep 8 2006
    copyright            : (C) 2006 by Alper Akcan
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

#include "../lib/xynth_.h"
#include "widget.h"

int w_frame_image_init (w_frame_image_t **fimg)
{
	*fimg = s_malloc(sizeof(w_frame_image_t));
	memset(*fimg, 0, sizeof(w_frame_image_t));
	s_list_init(&((*fimg)->images));
	return 0;
}

int w_frame_image_uninit (w_frame_image_t *fimg)
{
	s_image_t *img;
	while (!s_list_eol(fimg->images, 0)) {
		img = (s_image_t *) s_list_get(fimg->images, 0);
		s_list_remove(fimg->images, 0);
		s_image_uninit(img);
	}
	s_free(fimg);
	return 0;
}

int w_frame_set_image (w_object_t *object, unsigned int style, unsigned int rotation, unsigned int nimgs, ...)
{
	int pos;
	char *file;
	va_list ap;
	s_image_t *img;
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
	va_start(ap, nimgs);
	w_frame_image_init(&fimg);
	fimg->style = style;
	fimg->rotation = rotation;
	for (; nimgs > 0; nimgs--) {
		file = va_arg(ap, char *);
		s_image_init(&img);
		s_image_img(file, img);
		s_image_get_mat(img);
		s_list_add(fimg->images, img, -1);
	}
	va_end(ap);
	s_list_add(frame->images, fimg, -1);
	return 0;
}

void w_frame_draw_image (w_object_t *object, w_frame_image_t *fimg)
{
	int i;
	s_image_t *img;
	img = (s_image_t *) s_list_get(fimg->images, 0);
	s_putmaskpart(object->surface->matrix, object->surface->width, object->surface->height,
	              0, 0, img->w, img->h, img->w, img->h, img->mat, 0, 0);
	s_putboxrgba(object->surface, 0, 0, img->w, img->h, img->rgba);
	i = img->h;
	img = (s_image_t *) s_list_get(fimg->images, 1);
	for (; i < object->surface->height - img->h; i++) {
		s_putmaskpart(object->surface->matrix, object->surface->width, object->surface->height,
		              0, i, img->w, img->h, img->w, img->h, img->mat, 0, 0);
		s_putboxrgba(object->surface, 0, i, img->w, img->h, img->rgba);
	}
	img = (s_image_t *) s_list_get(fimg->images, 2);
	s_putmaskpart(object->surface->matrix, object->surface->width, object->surface->height,
	              0, object->surface->height - img->h, img->w, img->h, img->w, img->h,
	              img->mat, 0, 0);
	s_putboxrgba(object->surface, 0, object->surface->height - img->h, img->w, img->h, img->rgba);				
}

void w_frame_draw (w_object_t *object)
{
        int i;
        int j;
        int c[4];
        w_frame_t *frame;
        w_frame_image_t *fimg;
        frame = (w_frame_t *) object->data[OBJECT_FRAME];
        
        for (i = 0; !s_list_eol(frame->images, i); i++) {
        	fimg = (w_frame_image_t *) s_list_get(frame->images, i);
        	if (frame->style == fimg->style) {
        		w_frame_draw_image(object, fimg);
        		return;
        	}
        }
        
        switch (frame->style & FRAME_MSHAPE) {
		case FRAME_NOFRAME:		return;
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
	w_object_uninit(frame->object);
	while (!s_list_eol(frame->images, 0)) {
		fimg = s_list_get(frame->images, 0);
		w_frame_image_uninit(fimg);
	}
	s_list_uninit(frame->images);
	s_free(frame);
}
