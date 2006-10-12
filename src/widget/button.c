/***************************************************************************
    begin                : Fri Sep 8 2006
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

void w_button_event (w_object_t *object, s_event_t *event)
{
	w_button_t *button;
	button = (w_button_t *) object->data[OBJECT_BUTTON];
	if (event->type & MOUSE_EVENT) {
		event->mouse->x += object->window->window->surface->buf->x;
		event->mouse->y += object->window->window->surface->buf->y;
		event->mouse->px += object->window->window->surface->buf->x;
		event->mouse->py += object->window->window->surface->buf->y;
		s_thread_mutex_lock(object->window->window->handlers->mut);
		s_event_parse_handler_over(button->frame->object->window->window, event, button->handler_m);
		s_event_parse_handler_notover(button->object->window->window, event, button->handler_m);
		s_thread_mutex_unlock(object->window->window->handlers->mut);
	}
	if (event->type & KEYBD_EVENT) {
		s_thread_mutex_lock(object->window->window->handlers->mut);
		s_event_parse_keybd_handler(object->window->window, event, button->handler_k);
		s_thread_mutex_unlock(object->window->window->handlers->mut);
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
	w_button_t *button;
	button = (w_button_t *) handler->data;
	if ((button->state == 0) &&
	    (event->mouse->buttons == event->mouse->b)) {
		button->state |= event->mouse->b;
		button->frame->style = (button->frame->style & FRAME_MSHAPE) | FRAME_SUNKEN;
		button->object->draw(button->object);
		w_object_update(button->object, button->object->surface->win);
		if (button->pressed != NULL) {
			button->pressed(button->object, event->mouse->b);
		}
	}
}

void w_button_cb_c (s_window_t *window, s_event_t *event, s_handler_t *handler)
{ /* clicked */
	w_button_t *button;
	button = (w_button_t *) handler->data;
	if ((button->state != 0) &&
	    (button->state == event->mouse->b)) {
		button->state = 0;
		button->frame->style = (button->frame->style & FRAME_MSHAPE) | FRAME_RAISED;
		button->object->draw(button->object);
		w_object_update(button->object, button->object->surface->win);
		if (button->clicked != NULL) {
			button->clicked(button->object, event->mouse->b, event->mouse->clicks);
		}
	}
	
}

void w_button_cb_r (s_window_t *window, s_event_t *event, s_handler_t *handler)
{ /* released */
	w_button_t *button;
	button = (w_button_t *) handler->data;
	if ((button->state != 0) &&
	    (button->state == event->mouse->b)) {
		button->state = 0;
		button->frame->style = (button->frame->style & FRAME_MSHAPE) | FRAME_RAISED;
		button->object->draw(button->object);
		w_object_update(button->object, button->object->surface->win);
		if (button->released != NULL) {
			button->released(button->object, event->mouse->b);
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
	w_button_t *button;
	button = (w_button_t *) handler->data;
	if (!(button->frame->style & FRAME_SUNKEN)) {
		if (button->state != 0) {
			button->frame->style = (button->frame->style & FRAME_MSHAPE) | FRAME_SUNKEN;
			button->object->draw(button->object);
			w_object_update(button->object, button->object->surface->win);
		}
	}
}

void w_button_cb_oh (s_window_t *window, s_event_t *event, s_handler_t *handler)
{ /* not on over, but was on over */
	w_button_t *button;
	button = (w_button_t *) handler->data;
	if (!(button->frame->style & FRAME_RAISED)) {
		button->frame->style = (button->frame->style & FRAME_MSHAPE) | FRAME_RAISED;
		button->object->draw(button->object);
		w_object_update(button->object, button->object->surface->win);
	}
}

void w_button_cb_hoh (s_window_t *window, s_event_t *event, s_handler_t *handler)
{ /* not on over, but was on over. and button is still pressed */
	w_button_t *button;
	button = (w_button_t *) handler->data;
	if (!(button->frame->style & FRAME_RAISED)) {
		button->frame->style = (button->frame->style & FRAME_MSHAPE) | FRAME_RAISED;
		button->object->draw(button->object);
		w_object_update(button->object, button->object->surface->win);
	}
}

void w_button_cb_kp (s_window_t *window, s_event_t *event, s_handler_t *handler)
{ /* pressed */
	w_button_t *button;
	button = (w_button_t *) handler->data;
	if (button->state == 0) {
		button->state |= MOUSE_LEFTBUTTON;
		button->frame->style = (button->frame->style & FRAME_MSHAPE) | FRAME_SUNKEN;
		button->object->draw(button->object);
		w_object_update(button->object, button->object->surface->win);
		if (button->pressed != NULL) {
			button->pressed(button->object, MOUSE_LEFTBUTTON);
		}
	}
}

void w_button_cb_kr (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	w_button_t *button;
	button = (w_button_t *) handler->data;
	if (button->state != 0) {
		button->state = 0;
		button->frame->style = (button->frame->style & FRAME_MSHAPE) | FRAME_RAISED;
		button->object->draw(button->object);
		w_object_update(button->object, button->object->surface->win);
		if (button->released != NULL) {
			button->released(button->object, MOUSE_LEFTBUTTON);
		}
	}
}

int w_button_init (w_window_t *window, w_button_t **button, w_object_t *parent)
{
	(*button) = (w_button_t *) s_malloc(sizeof(w_button_t));
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
	
	s_handler_init(&((*button)->handler_k));
	(*button)->handler_k->type = KEYBD_HANDLER;
	(*button)->handler_k->keybd.flag = 0;
	(*button)->handler_k->keybd.button = S_KEYCODE_SPACE;
	(*button)->handler_k->keybd.p = w_button_cb_kp;
	(*button)->handler_k->keybd.r = w_button_cb_kr;
	(*button)->handler_k->data = *button;	

	(*button)->object = (*button)->frame->object;
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
	w_button_t *button;
	button = (w_button_t *) object->data[OBJECT_BUTTON];
	w_frame_uninit(button->object);
	s_handler_uninit(button->handler_m);
	s_handler_uninit(button->handler_k);
	s_free(button);
}
