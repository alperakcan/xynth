/***************************************************************************
    begin                : Tue Sep 12 2006
    copyright            : (C) 2006 by Alper Akcan
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

#include "../lib/xynth_.h"
#include "widget.h"

void w_textbox_draw (w_object_t *object)
{
	int x;
	int y;
	int w;
	int h;
	w_textbox_t *textbox;
	textbox = (w_textbox_t *) object->data[OBJECT_TEXTBOX];

	w = MIN(textbox->frame->object->content->w, textbox->font->img->w);
	h = MIN(textbox->frame->object->content->h, textbox->font->img->h);
	if (textbox->frame->object->content->w == w) { x = 0;
	} else { x = (textbox->frame->object->content->w - w) / 2; }
	if (textbox->frame->object->content->h == h) { y = 0;
	} else { y = (textbox->frame->object->content->h - h) / 2; }
	x += textbox->frame->object->content->x;
	y += textbox->frame->object->content->y;

	s_image_get_mat(textbox->font->img);
	if ((textbox->frame->style & FRAME_MSHAPE) == FRAME_NOFRAME) {
		memset(textbox->frame->object->surface->matrix,
		       0,
		       textbox->frame->object->surface->width * textbox->frame->object->surface->height);
		s_putmaskpart(textbox->frame->object->surface->matrix,
		              textbox->frame->object->surface->width,
		              textbox->frame->object->surface->height,
		              x,
		              y,
		              w,
		              h,
		              textbox->font->img->w,
		              textbox->font->img->h,
		              textbox->font->img->mat,
		              0,
		              0);
	} else {
		w_frame_draw(textbox->frame->object);
	}
	
	s_putboxpartrgba(textbox->frame->object->surface,
	                 x,
	                 y,
	                 w,
	                 h,
	                 textbox->font->img->w,
	                 textbox->font->img->h,
	                 textbox->font->img->rgba,
	                 0,
	                 0);
}

int w_textbox_set_str (w_object_t *object, char *str)
{
	w_textbox_t *textbox;
	textbox = (w_textbox_t *) object->data[OBJECT_TEXTBOX];
	s_font_set_str(textbox->font, str);
	s_font_get_glyph(textbox->font);
	w_textbox_draw(object);
	return 0;
}

int w_textbox_init (s_window_t *window, w_textbox_t **textbox, w_object_t *parent)
{
	(*textbox) = (w_textbox_t *) s_malloc(sizeof(w_textbox_t));
	(*textbox)->properties = TEXTBOX_NOWRAP;

	if (s_font_init(&((*textbox)->font), "arial.ttf")) {
		goto err0;
	}
	if (w_frame_init(window, &((*textbox)->frame), FRAME_PANEL | FRAME_RAISED, parent)) {
		goto err1;
	}

	(*textbox)->frame->object->draw = w_textbox_draw;
	(*textbox)->frame->object->destroy = w_textbox_uninit;
	(*textbox)->frame->object->data[OBJECT_TEXTBOX] = *textbox;

	s_font_set_size((*textbox)->font, 10);
	s_font_set_rgb((*textbox)->font, 0, 0, 0);
	w_textbox_set_str((*textbox)->frame->object, "");

	return 0;
err1:	s_font_uninit((*textbox)->font);
err0:	return -1;
}

void w_textbox_uninit (w_object_t *object)
{
	w_textbox_t *textbox;
	textbox = (w_textbox_t *) object->data[OBJECT_TEXTBOX];
	w_frame_uninit(textbox->frame->object);
	s_font_uninit(textbox->font);
	s_free(textbox);
}
