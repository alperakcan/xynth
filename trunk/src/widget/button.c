/***************************************************************************
    begin                : Fri Sep 8 2006
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

#include "xynth_.h"
#include "widget.h"

int w_button_set_pressed (w_object_t *object, void (*pressed) (w_object_t *, int))
{
	w_button_t *button;
	button = (w_button_t *) object->data[OBJECT_BUTTON];
	button->pressed = pressed;
	return 0;
}

int w_button_set_released (w_object_t *object, void (*released) (w_object_t *, int))
{
	w_button_t *button;
	button = (w_button_t *) object->data[OBJECT_BUTTON];
	button->released = released;
	return 0;
}

int w_button_set_clicked (w_object_t *object, void (*clicked) (w_object_t *, int, int))
{
	w_button_t *button;
	button = (w_button_t *) object->data[OBJECT_BUTTON];
	button->clicked = clicked;
	return 0;
}

int w_button_set_image (w_object_t *object, unsigned int style, unsigned int rotation, unsigned int nimgs, char **imgs)
{
	return w_frame_set_image(object, style, rotation, nimgs, imgs);
}

int w_button_set_style (w_object_t *object, FRAME_SHAPE shape, FRAME_SHADOW shadow)
{
	return w_frame_set_style(object, shape, shadow);
}

void w_button_event (w_object_t *object, s_event_t *event)
{
	int i;
	w_button_t *button;
	button = (w_button_t *) object->data[OBJECT_BUTTON];
	if (event->type & MOUSE_EVENT) {
		event->mouse->x += object->window->window->surface->buf->x;
		event->mouse->y += object->window->window->surface->buf->y;
		event->mouse->px += object->window->window->surface->buf->x;
		event->mouse->py += object->window->window->surface->buf->y;
		s_thread_mutex_lock(object->window->window->handlers->mut);
		s_event_parse_handler_over(button->frame->object->window->window, event, button->handler_m);
		s_event_parse_handler_notover(object->window->window, event, button->handler_m);
		s_thread_mutex_unlock(object->window->window->handlers->mut);
	}
	if (event->type & KEYBD_EVENT) {
		s_thread_mutex_lock(object->window->window->handlers->mut);
		for (i = 0; i < 5; i++) {
			s_event_parse_keybd_handler(object->window->window, event, button->handler_k[i]);
		}
		s_thread_mutex_unlock(object->window->window->handlers->mut);
	}
	if (event->type & FOCUSOUT_EVENT) {
		button->state = 0;
		button->frame->style = (button->frame->style & FRAME_MSHAPE) | FRAME_RAISED;
		button->frame->style |= (button->frame->object->focused) ? FRAME_FOCUSED : 0;
		object->draw(object);
		w_object_update(object, object->surface->win);
	}
}

void w_button_draw (w_object_t *object)
{
	w_frame_draw(object);
}

void w_button_geometry (w_object_t *object)
{
	w_button_t *button;
	w_frame_geometry(object);
	button = (w_button_t *) object->data[OBJECT_BUTTON];
	button->handler_m->mouse.x = object->surface->win->x;
	button->handler_m->mouse.y = object->surface->win->y;
	button->handler_m->mouse.w = object->surface->win->w;
	button->handler_m->mouse.h = object->surface->win->h;
}

void w_button_cb_o (s_window_t *window, s_event_t *event, s_handler_t *handler)
{ /* over */
}

void w_button_cb_p (s_window_t *window, s_event_t *event, s_handler_t *handler)
{ /* pressed */
	w_button_t *button = (w_button_t *) handler->data;
	w_object_t *object = button->object;
	if ((button->state == 0) &&
	    (event->mouse->buttons == event->mouse->b)) {
		button->state |= event->mouse->b;
		button->frame->style = (button->frame->style & FRAME_MSHAPE) | FRAME_SUNKEN;
		button->frame->style |= (button->frame->object->focused) ? FRAME_FOCUSED : 0;
		object->draw(object);
		w_object_update(object, object->surface->win);
		if (button->pressed != NULL) {
			button->pressed(object, event->mouse->b);
		}
	}
}

void w_button_cb_c (s_window_t *window, s_event_t *event, s_handler_t *handler)
{ /* clicked */
	w_button_t *button = (w_button_t *) handler->data;
	w_object_t *object = button->object;
	if ((button->state != 0) &&
	    (button->state == event->mouse->b)) {
		button->state = 0;
		button->frame->style = (button->frame->style & FRAME_MSHAPE) | FRAME_RAISED;
		button->frame->style |= (button->frame->object->focused) ? FRAME_FOCUSED : 0;
		object->draw(object);
		w_object_update(object, object->surface->win);
		if (button->released != NULL) {
			button->released(object, event->mouse->b);
		}
		if (button->clicked != NULL) {
			button->clicked(object, event->mouse->b, event->mouse->clicks);
		}
	}
	
}

void w_button_cb_r (s_window_t *window, s_event_t *event, s_handler_t *handler)
{ /* released */
	w_button_t *button = (w_button_t *) handler->data;
	w_object_t *object = button->object;
	if ((button->state != 0) &&
	    (button->state == event->mouse->b)) {
		button->state = 0;
		button->frame->style = (button->frame->style & FRAME_MSHAPE) | FRAME_RAISED;
		button->frame->style |= (button->frame->object->focused) ? FRAME_FOCUSED : 0;
		object->draw(object);
		w_object_update(object, object->surface->win);
		if (button->released != NULL) {
			button->released(object, event->mouse->b);
		}
	}
}

void w_button_cb_hr (s_window_t *window, s_event_t *event, s_handler_t *handler)
{ /* mouse button released, but the prev. press was not on us */
}

void w_button_cb_rh (s_window_t *window, s_event_t *event, s_handler_t *handler)
{ /* mouse button released outside, but the prev. press was on us */
}

void w_button_cb_ho (s_window_t *window, s_event_t *event, s_handler_t *handler)
{ /* on over, but mouse button is still pressed */
	w_button_t *button = (w_button_t *) handler->data;
	w_object_t *object = button->object;
	if (!(button->frame->style & FRAME_SUNKEN)) {
		if (button->state != 0) {
			button->frame->style = (button->frame->style & FRAME_MSHAPE) | FRAME_SUNKEN;
			button->frame->style |= (button->frame->object->focused) ? FRAME_FOCUSED : 0;
			object->draw(object);
			w_object_update(object, object->surface->win);
		}
	}
}

void w_button_cb_oh (s_window_t *window, s_event_t *event, s_handler_t *handler)
{ /* not on over, but was on over */
	w_button_t *button = (w_button_t *) handler->data;
	w_object_t *object = button->object;
	if (!(button->frame->style & FRAME_RAISED)) {
		button->frame->style = (button->frame->style & FRAME_MSHAPE) | FRAME_RAISED;
		button->frame->style |= (button->frame->object->focused) ? FRAME_FOCUSED : 0;
		object->draw(object);
		w_object_update(object, object->surface->win);
	}
}

void w_button_cb_hoh (s_window_t *window, s_event_t *event, s_handler_t *handler)
{ /* not on over, but was on over. and button is still pressed */
	w_button_t *button = (w_button_t *) handler->data;
	w_object_t *object = button->object;
	if (!(button->frame->style & FRAME_RAISED)) {
		button->frame->style = (button->frame->style & FRAME_MSHAPE) | FRAME_RAISED;
		button->frame->style |= (button->frame->object->focused) ? FRAME_FOCUSED : 0;
		object->draw(object);
		w_object_update(object, object->surface->win);
	}
}

void w_button_cb_kp (s_window_t *window, s_event_t *event, s_handler_t *handler)
{ /* pressed */
	w_button_t *button = (w_button_t *) handler->data;
	w_object_t *object = button->object;
	if (button->state == 0) {
		button->state |= MOUSE_LEFTBUTTON;
		button->frame->style = (button->frame->style & FRAME_MSHAPE) | FRAME_SUNKEN;
		button->frame->style |= (button->frame->object->focused) ? FRAME_FOCUSED : 0;
		object->draw(object);
		w_object_update(object, object->surface->win);
		if (button->pressed != NULL) {
			button->pressed(object, handler->keybd.button);
		}
	}
}

void w_button_cb_kr (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	w_button_t *button = (w_button_t *) handler->data;
	w_object_t *object = button->object;
	if (button->state != 0) {
		button->state = 0;
		button->frame->style = (button->frame->style & FRAME_MSHAPE) | FRAME_RAISED;
		button->frame->style |= (button->frame->object->focused) ? FRAME_FOCUSED : 0;
		object->draw(object);
		w_object_update(object, object->surface->win);
		if (button->released != NULL) {
			button->released(object, handler->keybd.button);
		}
	}
}

int w_button_init (w_window_t *window, w_button_t **button, w_object_t *parent)
{
	(*button) = (w_button_t *) s_malloc(sizeof(w_button_t));
	memset(*button, 0, sizeof(w_button_t));
	if (w_frame_init(window, &((*button)->frame), FRAME_PANEL | FRAME_RAISED, parent)) {
		goto err0;
	}

	s_handler_init(&((*button)->handler_m));
	(*button)->handler_m->type = MOUSE_HANDLER;
	(*button)->handler_m->mouse.x = 0;
	(*button)->handler_m->mouse.y = 0;
	(*button)->handler_m->mouse.w = 0;
	(*button)->handler_m->mouse.h = 0;
	(*button)->handler_m->mouse.o = w_button_cb_o;
	(*button)->handler_m->mouse.p = w_button_cb_p;
	(*button)->handler_m->mouse.c = w_button_cb_c;
	(*button)->handler_m->mouse.r = w_button_cb_r;
	(*button)->handler_m->mouse.hr = w_button_cb_hr;
	(*button)->handler_m->mouse.rh = w_button_cb_rh;
	(*button)->handler_m->mouse.ho = w_button_cb_ho;
	(*button)->handler_m->mouse.oh = w_button_cb_oh;
	(*button)->handler_m->mouse.hoh = w_button_cb_hoh;
	(*button)->handler_m->mouse.button = ~MOUSE_NONEBUTTON;
	(*button)->handler_m->data = *button;	
	(*button)->state = 0;
	
	s_handler_init(&((*button)->handler_k[0]));
	(*button)->handler_k[0]->type = KEYBD_HANDLER;
	(*button)->handler_k[0]->keybd.flag = 0;
	(*button)->handler_k[0]->keybd.button = S_KEYCODE_SPACE;
	(*button)->handler_k[0]->keybd.p = w_button_cb_kp;
	(*button)->handler_k[0]->keybd.r = w_button_cb_kr;
	(*button)->handler_k[0]->data = *button;	

	s_handler_init(&((*button)->handler_k[1]));
	(*button)->handler_k[1]->type = KEYBD_HANDLER;
	(*button)->handler_k[1]->keybd.flag = 0;
	(*button)->handler_k[1]->keybd.button = S_KEYCODE_LEFT;
	(*button)->handler_k[1]->keybd.p = w_button_cb_kp;
	(*button)->handler_k[1]->keybd.r = w_button_cb_kr;
	(*button)->handler_k[1]->data = *button;	

	s_handler_init(&((*button)->handler_k[2]));
	(*button)->handler_k[2]->type = KEYBD_HANDLER;
	(*button)->handler_k[2]->keybd.flag = 0;
	(*button)->handler_k[2]->keybd.button = S_KEYCODE_UP;
	(*button)->handler_k[2]->keybd.p = w_button_cb_kp;
	(*button)->handler_k[2]->keybd.r = w_button_cb_kr;
	(*button)->handler_k[2]->data = *button;	

	s_handler_init(&((*button)->handler_k[3]));
	(*button)->handler_k[3]->type = KEYBD_HANDLER;
	(*button)->handler_k[3]->keybd.flag = 0;
	(*button)->handler_k[3]->keybd.button = S_KEYCODE_RIGHT;
	(*button)->handler_k[3]->keybd.p = w_button_cb_kp;
	(*button)->handler_k[3]->keybd.r = w_button_cb_kr;
	(*button)->handler_k[3]->data = *button;	

	s_handler_init(&((*button)->handler_k[4]));
	(*button)->handler_k[4]->type = KEYBD_HANDLER;
	(*button)->handler_k[4]->keybd.flag = 0;
	(*button)->handler_k[4]->keybd.button = S_KEYCODE_DOWN;
	(*button)->handler_k[4]->keybd.p = w_button_cb_kp;
	(*button)->handler_k[4]->keybd.r = w_button_cb_kr;
	(*button)->handler_k[4]->data = *button;	

	(*button)->object = (*button)->frame->object;
	(*button)->object->type = OBJECT_BUTTON;
	(*button)->object->data[OBJECT_BUTTON] = *button;
	(*button)->object->draw = w_button_draw;
	(*button)->object->event = w_button_event;
	(*button)->object->geometry = w_button_geometry;
	(*button)->object->destroy = w_button_uninit;
	
	(*button)->pressed = NULL;
	(*button)->clicked = NULL;
	(*button)->released = NULL;
	
	return 0;
err0:	s_free(*button);
	return -1;
}

void w_button_uninit (w_object_t *object)
{
	int i;
	w_button_t *button;
	button = (w_button_t *) object->data[OBJECT_BUTTON];
	w_frame_uninit(object);
	s_handler_uninit(button->handler_m);
	for (i = 0; i < 5; i++) {
		s_handler_uninit(button->handler_k[i]);
	}
	s_free(button);
}
