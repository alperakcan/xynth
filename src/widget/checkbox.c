/***************************************************************************
    begin                : Tue Oct 10 2006
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

void w_checkbox_geometry (w_object_t *object)
{
	s_rect_t coor;
	w_checkbox_t *cb;
	cb = object->data[OBJECT_CHECKBOX];
	w_button_geometry(object);
	coor.w = 16;
	coor.h = 16;
	coor.x = 0;
	coor.y = (object->surface->buf->h - coor.h) / 2;
	w_object_move(cb->box->object, coor.x, coor.y, coor.w, coor.h);
	coor.x = coor.w + 2;
	coor.y = 0;
	coor.w = object->surface->buf->w - coor.x;
	coor.h = object->surface->buf->h;
	w_object_move(cb->text->object, coor.x, coor.y, coor.w, coor.h);
}

void w_checkbox_draw (w_object_t *object)
{
	w_checkbox_t *cb;
	cb = object->data[OBJECT_CHECKBOX];
	w_button_draw(cb->button->object);
	w_textbox_draw(cb->text->object);
	w_frame_draw(cb->box->object);
	if (cb->state) {
		s_fillbox(cb->box->object->surface, 4, 4, 8, 8, 0);
	}
}

void w_checkbox_state (w_object_t *object, int state)
{
	w_checkbox_t *cb;
	cb = object->data[OBJECT_CHECKBOX];
	cb->state = state;
	w_checkbox_draw(object);
	w_object_update(object, object->surface->win);
	if (cb->changed != NULL) {
		cb->changed(object, cb->state);
	}
}

void w_checkbox_pressed (w_object_t *object, int btn)
{
	w_checkbox_t *cb;
	cb = object->data[OBJECT_CHECKBOX];
}

void w_checkbox_released (w_object_t *object, int btn)
{
	w_checkbox_t *cb;
	cb = object->data[OBJECT_CHECKBOX];
	w_checkbox_state(object, (cb->state + 1) % 2);
}

void w_checkbox_clicked (w_object_t *object, int btn, int cnt)
{
	w_checkbox_t *cb;
	cb = object->data[OBJECT_CHECKBOX];
	w_checkbox_state(object, (cb->state + 1) % 2);
}

void w_checkbox_uninit (w_object_t *object)
{
	w_checkbox_t *cb;
	cb = object->data[OBJECT_CHECKBOX];
	w_textbox_uninit(cb->text->object);
	w_frame_uninit(cb->box->object);
	w_button_uninit(cb->object);
	s_free(cb);
}

int w_checkbox_init (w_window_t *window, w_checkbox_t **checkbox, w_object_t *parent)
{
	w_checkbox_t *cb;
	cb = (w_checkbox_t *) s_malloc(sizeof(w_checkbox_t));
	cb->state = 0;

	w_button_init(window, &(cb->button), parent);
	cb->button->frame->style = FRAME_NOFRAME;
	
	w_textbox_init(window, &(cb->text), cb->button->object);
	cb->text->frame->style = FRAME_NOFRAME;
	w_object_show(cb->text->object);
	
	w_frame_init(window, &(cb->box), FRAME_PANEL | FRAME_SUNKEN, cb->button->object);
	w_object_show(cb->box->object);
	
	cb->object = cb->button->object;
	cb->object->data[OBJECT_CHECKBOX] = cb;
	cb->button->pressed = w_checkbox_pressed;
	cb->button->released = w_checkbox_released;
	cb->button->clicked = w_checkbox_clicked;
	cb->object->draw = w_checkbox_draw;
	cb->object->geometry = w_checkbox_geometry;
	cb->object->destroy = w_checkbox_uninit;
	cb->changed = NULL;
	*checkbox = cb;
	return 0;
}
