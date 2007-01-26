/***************************************************************************
    begin                : Tue Oct 10 2006
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

#include "../lib/xynth_.h"
#include "widget.h"

int w_checkbox_set_str (w_object_t *object, char *str)
{
	w_checkbox_t *cb;
	cb = object->data[OBJECT_CHECKBOX];
	return w_textbox_set_str(cb->text->object, str);
}

int w_checkbox_set_changed (w_object_t *object, void (*changed) (w_object_t *, int))
{
	w_checkbox_t *cb;
	cb = object->data[OBJECT_CHECKBOX];
	cb->changed = changed;
	return 0;
}

int w_checkbox_set_style (w_object_t *object, FRAME_SHAPE shape, FRAME_SHADOW shadow)
{
	return w_frame_set_style(object, shape, shadow);
}

int w_checkbox_set_image (w_object_t *object, unsigned int style, unsigned int rotation, unsigned int nimgs, char **imgs)
{
	return w_frame_set_image(object, style, rotation, nimgs, imgs);
}

int w_checkbox_set_boxstyle (w_object_t *object, FRAME_SHAPE shape, FRAME_SHADOW shadow)
{
	w_checkbox_t *cb;
	cb = object->data[OBJECT_CHECKBOX];
	return w_frame_set_style(cb->box->object, shape, shadow);
}

int w_checkbox_set_boximage (w_object_t *object, unsigned int style, unsigned int rotation, unsigned int nimgs, char **imgs)
{
	w_checkbox_t *cb;
	cb = object->data[OBJECT_CHECKBOX];
	return w_frame_set_image(cb->box->object, style, rotation, nimgs, imgs);
}

int w_checkbox_set_rgb (w_object_t *object, int r, int g, int b)
{
	w_checkbox_t *cb;
	cb = object->data[OBJECT_CHECKBOX];
	return w_textbox_set_rgb(cb->text->object, r, g, b);
}

int w_checkbox_set_size (w_object_t *object, int size)
{
	w_checkbox_t *cb;
	cb = object->data[OBJECT_CHECKBOX];
	return w_textbox_set_size(cb->text->object, size);
}

int w_checkbox_set_properties (w_object_t *object, TEXTBOX_PROPERTIES properties)
{
	w_checkbox_t *cb;
	cb = object->data[OBJECT_CHECKBOX];
	return w_textbox_set_properties(cb->text->object, properties);
}

void w_checkbox_geometry (w_object_t *object)
{
	s_rect_t coor;
	w_checkbox_t *cb;
	cb = object->data[OBJECT_CHECKBOX];
	w_button_geometry(object);
	coor.w = object->surface->buf->h;
	coor.h = object->surface->buf->h;
	coor.x = 0;
	coor.y = (object->surface->buf->h - coor.h) / 2;
	w_object_move(cb->box->object, coor.x, coor.y, coor.w, coor.h);
	coor.x = coor.w + 4;
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
	cb->box->style &= ~FRAME_MSHADOW;
	cb->box->style |= (cb->state == 0) ? FRAME_RAISED : FRAME_SUNKEN;
	cb->box->object->focused = object->focused;
	w_frame_draw(cb->box->object);
}

void w_checkbox_state (w_object_t *object, int state)
{
	w_checkbox_t *cb;
	cb = object->data[OBJECT_CHECKBOX];
	cb->state = state;
	w_checkbox_draw(object);
	w_object_update(object, object->surface->win);
}

void w_checkbox_released (w_object_t *object, int btn)
{
	w_checkbox_t *cb;
	cb = object->data[OBJECT_CHECKBOX];
	w_checkbox_state(object, (cb->state + 1) % 2);
	if (cb->changed != NULL) {
		cb->changed(object, cb->state);
	}
}

void w_checkbox_event (w_object_t *object, s_event_t *event)
{
	w_checkbox_t *cb;
	cb = object->data[OBJECT_CHECKBOX];
	w_button_event(cb->button->object, event);
}

void w_checkbox_uninit (w_object_t *object)
{
	w_checkbox_t *cb;
	cb = object->data[OBJECT_CHECKBOX];
	w_textbox_uninit(cb->text->object);
	w_frame_uninit(cb->box->object);
	w_button_uninit(object);
	s_free(cb);
}

int w_checkbox_init (w_window_t *window, w_checkbox_t **checkbox, w_object_t *parent)
{
	w_checkbox_t *cb;
	cb = (w_checkbox_t *) s_malloc(sizeof(w_checkbox_t));
	cb->state = 0;

	w_button_init(window, &(cb->button), parent);
	cb->button->frame->style = FRAME_EMPTY;
	
	w_textbox_init(window, &(cb->text), cb->button->object);
	cb->text->frame->style = FRAME_NOFRAME;
	cb->text->properties = TEXTBOX_VCENTER;
	w_object_show(cb->text->object);
	
	w_frame_init(window, &(cb->box), FRAME_PANEL | FRAME_RAISED, cb->button->object);
	w_object_show(cb->box->object);
	
	cb->object = cb->button->object;
	cb->object->type = OBJECT_CHECKBOX;
	cb->object->data[OBJECT_CHECKBOX] = cb;
	cb->button->object->event = w_checkbox_event;
	cb->button->released = w_checkbox_released;
	cb->object->draw = w_checkbox_draw;
	cb->object->geometry = w_checkbox_geometry;
	cb->object->destroy = w_checkbox_uninit;
	cb->changed = NULL;
	*checkbox = cb;
	return 0;
}
