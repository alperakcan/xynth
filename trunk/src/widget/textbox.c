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
	int d;
	w_textbox_t *textbox;
	
	textbox = (w_textbox_t *) object->data[OBJECT_TEXTBOX];
	
	w = MIN(textbox->object->content->w, textbox->font->img->w);
	h = MIN(textbox->object->content->h, textbox->font->height);
	if (!(textbox->properties & TEXTBOX_HCENTER) || textbox->object->content->w == w) { x = 0;
	} else { x = (textbox->object->content->w - w) / 2; }
	if (!(textbox->properties & TEXTBOX_VCENTER) || textbox->object->content->h == h) { y = 0;
	} else { y = (textbox->object->content->h - h) / 2; }
	x += textbox->object->content->x;
	y += textbox->object->content->y;
	
	d = textbox->font->img->w - textbox->object->content->w;
	if (d > 0) { x -= d; w += d; }

	s_image_get_mat(textbox->font->img);
	if ((textbox->frame->style & FRAME_MSHAPE) == FRAME_NOFRAME) {
		memset(textbox->object->surface->matrix, 0, textbox->object->surface->width * textbox->object->surface->height);
		s_putmaskpart(textbox->object->surface->matrix, textbox->object->surface->width, textbox->object->surface->height,
		              x, y + textbox->font->size - textbox->font->yMax, w, h, textbox->font->img->w, textbox->font->img->h,
		              textbox->font->img->mat, 0, 0);
		s_putboxpartrgb(textbox->object->surface, x, y + textbox->font->size - textbox->font->yMax, w, h,
		                textbox->font->img->w, textbox->font->img->h, textbox->font->img->rgba, 0, 0);
	} else {
		w_frame_draw(textbox->object);
		s_putboxpartrgba(textbox->object->surface, x, y + textbox->font->size - textbox->font->yMax, w, h,
		                 textbox->font->img->w, textbox->font->img->h, textbox->font->img->rgba, 0, 0);
	}
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

void w_textbox_geometry (w_object_t *object)
{
	w_frame_geometry(object);
}

void w_textbox_loadimages(w_object_t *object,char *file_left,char *file_middle,char *file_right)
{
	w_textbox_t *textbox;
	
	textbox = (w_textbox_t*) object->data[OBJECT_TEXTBOX];
	
	s_image_init(&(textbox->img_left));	
	s_image_init(&(textbox->img_middle));	
	s_image_init(&(textbox->img_right));	
	
	if( (s_image_img(file_left,textbox->img_left)!=-1) &&
		(s_image_img(file_middle,textbox->img_middle)!=-1) &&
		(s_image_img(file_right,textbox->img_right)!=-1) )
	{
		textbox->isimg=1;
	}
	else
	{
		textbox->isimg=0;
		s_image_uninit(textbox->img_left);	
		s_image_uninit(textbox->img_middle);	
		s_image_uninit(textbox->img_right);	
	}	
}

int w_textbox_init (w_window_t *window, w_textbox_t **textbox, w_object_t *parent)
{
	(*textbox) = (w_textbox_t *) s_malloc(sizeof(w_textbox_t));
	(*textbox)->properties = TEXTBOX_NOWRAP;

	if (s_font_init(&((*textbox)->font), "arial.ttf")) {
		goto err0;
	}
	if (w_frame_init(window, &((*textbox)->frame), FRAME_PANEL | FRAME_RAISED, parent)) {
		goto err1;
	}
	
	(*textbox)->isimg=0;
	
	(*textbox)->properties = TEXTBOX_VCENTER | TEXTBOX_HCENTER;

	(*textbox)->object = (*textbox)->frame->object;
	(*textbox)->object->draw = w_textbox_draw;
	(*textbox)->object->geometry = w_textbox_geometry;
	(*textbox)->object->destroy = w_textbox_uninit;
	(*textbox)->object->data[OBJECT_TEXTBOX] = *textbox;

	s_font_set_size((*textbox)->font, 10);
	s_font_set_rgb((*textbox)->font, 0, 0, 0);
	w_textbox_set_str((*textbox)->object, "");

	return 0;
err1:	s_font_uninit((*textbox)->font);
err0:	return -1;
}

void w_textbox_uninit (w_object_t *object)
{
	w_textbox_t *textbox;
	textbox = (w_textbox_t *) object->data[OBJECT_TEXTBOX];
	
	if(textbox->isimg==1)
	{
		s_image_uninit(textbox->img_left);	
		s_image_uninit(textbox->img_middle);	
		s_image_uninit(textbox->img_right);	
	}
	
	w_frame_uninit(textbox->object);
	s_font_uninit(textbox->font);
	s_free(textbox);
}
