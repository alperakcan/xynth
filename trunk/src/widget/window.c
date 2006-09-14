/***************************************************************************
    begin                : Mon Sep 11 2006
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

void w_window_atevent (s_window_t *window, s_event_t *event)
{
	w_object_t *objectn;
	w_object_t *objectp;
	w_window_t *windoww;
	windoww = (w_window_t *) window->client->user_data;
	if (event->type & (MOUSE_EVENT | KEYBD_EVENT)) {
		event->mouse->x -= window->surface->buf->x;
		event->mouse->y -= window->surface->buf->y;
		event->mouse->px -= window->surface->buf->x;
		event->mouse->py -= window->surface->buf->y;
		w_object_atposition(windoww->object, event->mouse->x, event->mouse->y, &objectn);
		w_object_atposition(windoww->object, event->mouse->px, event->mouse->py, &objectp);
		while (objectp && objectp->event == NULL) {
			objectp = objectp->parent;
		}
		while (objectn && objectn->event == NULL) {
			objectn = objectn->parent;
		}
		if (objectp && (objectn != objectp) && (objectp->event)) {
			objectp->event(objectp, event);
		}
		if (objectn && objectn->event) {
			objectn->event(objectn, event);
		}
	}
}

int w_window_init (w_window_t **window, S_WINDOW type, w_window_t *parent)
{
	(*window) = (w_window_t *) s_malloc(sizeof(w_window_t));
	s_client_init(&((*window)->window));
	s_window_new((*window)->window, type, NULL);
	s_window_set_resizeable((*window)->window, 0);
	w_object_init((*window)->window, &((*window)->object), NULL, NULL);
	s_client_atevent((*window)->window, w_window_atevent);
	(*window)->window->client->user_data = (*window);
	return 0;
}

int w_window_set_coor (w_window_t *window, int x, int y, int w, int h)
{
	s_window_set_coor(window->window, WINDOW_NOFORM, x, y, w, h);
	w_object_move(window->object, 0, 0, w, h);
	return 0;
}

int w_window_uninit (w_window_t *window)
{
	w_object_uninit(window->object);
	s_free(window);
	return 0;
}
