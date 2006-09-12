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
	w_textbox_t *textbox;
	textbox = (w_textbox_t *) object->data[OBJECT_TEXTBOX];
	w_frame_draw(textbox->frame->object);
	s_font_set_str(textbox->font, "try");
	s_font_set_size(textbox->font, 10);
	s_font_get_glyph(textbox->font);
	x = 0;
	y = 0;
	s_putboxrgba(textbox->frame->object->surface, x, y, textbox->font->img->w, textbox->font->img->h, textbox->font->img->rgba);

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
	return 0;
}
