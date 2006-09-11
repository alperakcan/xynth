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

void s_button_event (s_object_t *object, s_event_t *event)
{
	s_button_t *button;
	button = (s_button_t *) object->data[OBJECT_BUTTON];
	event->mouse->x += object->window->surface->buf->x;
	event->mouse->y += object->window->surface->buf->y;
	event->mouse->px += object->window->surface->buf->x;
	event->mouse->py += object->window->surface->buf->y;
	s_thread_mutex_lock(object->window->handlers->mut);
	s_event_parse_handler_over(button->frame->object->window, event, button->handler);
	s_event_parse_handler_notover(button->frame->object->window, event, button->handler);
	s_thread_mutex_unlock(object->window->handlers->mut);
}

void s_button_draw (s_object_t *object)
{
	s_button_t *button;
	button = (s_button_t *) object->data[OBJECT_BUTTON];
	s_frame_draw(object);
}

void s_button_geometry (s_object_t *object)
{
	s_button_t *button;
	button = (s_button_t *) object->data[OBJECT_BUTTON];
	button->handler->mouse.x = object->surface->win->x;
	button->handler->mouse.y = object->surface->win->y;
	button->handler->mouse.w = object->surface->win->w;
	button->handler->mouse.h = object->surface->win->h;
}

void s_button_cb_o (s_window_t *window, s_event_t *event, s_handler_t *handler)
{ /* over */
}

void s_button_cb_p (s_window_t *window, s_event_t *event, s_handler_t *handler)
{ /* pressed */
	s_button_t *button;
	button = (s_button_t *) handler->user_data;
	if ((button->state == 0) &&
	    (event->mouse->buttons == event->mouse->b)) {
		button->state |= event->mouse->b;
		button->frame->style = (button->frame->style & FRAME_MSHAPE) | FRAME_SUNKEN;
		button->frame->object->draw(button->frame->object);
		s_object_update(button->frame->object, button->frame->object->surface->win);
		if (button->pressed != NULL) {
			button->pressed(button->frame->object, event->mouse->b);
		}
	}
}

void s_button_cb_c (s_window_t *window, s_event_t *event, s_handler_t *handler)
{ /* clicked */
	s_button_t *button;
	button = (s_button_t *) handler->user_data;
	if ((button->state != 0) &&
	    (button->state == event->mouse->b)) {
		button->state = 0;
		button->frame->style = (button->frame->style & FRAME_MSHAPE) | FRAME_RAISED;
		button->frame->object->draw(button->frame->object);
		s_object_update(button->frame->object, button->frame->object->surface->win);
	}
}

void s_button_cb_r (s_window_t *window, s_event_t *event, s_handler_t *handler)
{ /* released */
	s_button_t *button;
	button = (s_button_t *) handler->user_data;
	if ((button->state != 0) &&
	    (button->state == event->mouse->b)) {
		button->state = 0;
		button->frame->style = (button->frame->style & FRAME_MSHAPE) | FRAME_RAISED;
		button->frame->object->draw(button->frame->object);
		s_object_update(button->frame->object, button->frame->object->surface->win);
	}
}

void s_button_cb_hr (s_window_t *window, s_event_t *event, s_handler_t *handler)
{ /* mouse button released, but the prev. press was not on us */
}

void s_button_cb_rh (s_window_t *window, s_event_t *event, s_handler_t *handler)
{ /* mouse button released outside, but the prev. press was on us */
}

void s_button_cb_ho (s_window_t *window, s_event_t *event, s_handler_t *handler)
{ /* on over, but mouse button is still pressed */
	s_button_t *button;
	button = (s_button_t *) handler->user_data;
	if (!(button->frame->style & FRAME_SUNKEN)) {
		if (button->state != 0) {
			button->frame->style = (button->frame->style & FRAME_MSHAPE) | FRAME_SUNKEN;
			button->frame->object->draw(button->frame->object);
			s_object_update(button->frame->object, button->frame->object->surface->win);
		}
	}
}

void s_button_cb_oh (s_window_t *window, s_event_t *event, s_handler_t *handler)
{ /* not on over, but was on over */
	s_button_t *button;
	button = (s_button_t *) handler->user_data;
	if (!(button->frame->style & FRAME_RAISED)) {
		button->frame->style = (button->frame->style & FRAME_MSHAPE) | FRAME_RAISED;
		button->frame->object->draw(button->frame->object);
		s_object_update(button->frame->object, button->frame->object->surface->win);
	}
}

void s_button_cb_hoh (s_window_t *window, s_event_t *event, s_handler_t *handler)
{ /* not on over, but was on over. and button is still pressed */
	s_button_t *button;
	button = (s_button_t *) handler->user_data;
	if (!(button->frame->style & FRAME_RAISED)) {
		button->frame->style = (button->frame->style & FRAME_MSHAPE) | FRAME_RAISED;
		button->frame->object->draw(button->frame->object);
		s_object_update(button->frame->object, button->frame->object->surface->win);
	}
}

int s_button_init (s_window_t *window, s_button_t **button, s_object_t *parent)
{
	(*button) = (s_button_t *) s_malloc(sizeof(s_button_t));
	if (s_frame_init(window, &((*button)->frame), FRAME_PANEL | FRAME_RAISED, parent)) {
		goto err0;
	}

	s_handler_init(&((*button)->handler));
	(*button)->handler->type = MOUSE_HANDLER;
	(*button)->handler->mouse.x = 0;
	(*button)->handler->mouse.y = 0;
	(*button)->handler->mouse.w = 0;
	(*button)->handler->mouse.h = 0;
	(*button)->handler->mouse.o = s_button_cb_o;
	(*button)->handler->mouse.p = s_button_cb_p;
	(*button)->handler->mouse.c = s_button_cb_c;
	(*button)->handler->mouse.r = s_button_cb_r;
	(*button)->handler->mouse.hr = s_button_cb_hr;
	(*button)->handler->mouse.rh = s_button_cb_rh;
	(*button)->handler->mouse.ho = s_button_cb_ho;
	(*button)->handler->mouse.oh = s_button_cb_oh;
	(*button)->handler->mouse.hoh = s_button_cb_hoh;
	(*button)->handler->mouse.button = ~MOUSE_NONEBUTTON;
	(*button)->handler->user_data = *button;	
	(*button)->state = 0;

	(*button)->frame->object->data[OBJECT_BUTTON] = *button;
	(*button)->frame->object->draw = s_button_draw;
	(*button)->frame->object->event = s_button_event;
	(*button)->frame->object->geometry = s_button_geometry;
	(*button)->frame->object->destroy = s_button_uninit;
	
	(*button)->pressed = NULL;
	(*button)->clicked = NULL;
	(*button)->released = NULL;
	
	return 0;
err0:	s_free(*button);
	return -1;
}

void s_button_uninit (s_object_t *object)
{
	s_button_t *button;
	button = (s_button_t *) object->data[OBJECT_BUTTON];
	s_frame_uninit(button->frame->object);
	s_handler_uninit(button->handler);
	s_free(button);
}
