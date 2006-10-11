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

void w_window_change_keybd_focus (s_window_t *window)
{
	int i;
	int l;
	int ls;
	w_object_t *temp;
	w_object_t *root;
	w_window_t *windoww;

	windoww = (w_window_t *) window->client->data;

	root = windoww->object;
	
	ls = 0;
	w_object_level_find(root, windoww->event, &l);
	w_object_level_count(root, &ls);
	for (i = 0; i < ls; i++) {
		l++;
		if (l > ls) {
			l = 0;
		}
		w_object_level_get(root, &temp, l);
		if (temp && temp->event) {
			windoww->event = temp;
			break;
		}
	}
#if 0
	w_object_level_find(root, windoww->event, &l);
	printf("level %d [%d]\n", l, ls);
#endif
}

void w_window_atevent (s_window_t *window, s_event_t *event)
{
	s_event_t *evnt;
	w_object_t *objectn;
	w_object_t *objectp;
	w_window_t *windoww;
	windoww = (w_window_t *) window->client->data;
	if (event->type & (MOUSE_EVENT | KEYBD_EVENT)) {
		event->mouse->x -= window->surface->buf->x;
		event->mouse->y -= window->surface->buf->y;
		event->mouse->x_old -= window->surface->buf->x;
		event->mouse->y_old -= window->surface->buf->y;
		event->mouse->px -= window->surface->buf->x;
		event->mouse->py -= window->surface->buf->y;
		w_object_atposition(windoww->object, event->mouse->x, event->mouse->y, &objectn);
		w_object_atposition(windoww->object, event->mouse->x_old, event->mouse->y_old, &objectp);
	}
	if (event->type & MOUSE_EVENT) {
		while (objectp && objectp->event == NULL) {
			objectp = objectp->parent;
		}
		while (objectn && objectn->event == NULL) {
			objectn = objectn->parent;
		}
		if (windoww->focus != objectn) {
			s_event_init(&evnt);
			evnt->type = FOCUS_EVENT | FOCUSIN_EVENT;
			if (objectn && objectn->event) {
				objectn->event(objectn, evnt);
			}
			s_event_uninit(evnt);
			s_event_init(&evnt);
			evnt->type = FOCUS_EVENT | FOCUSOUT_EVENT;
			if (windoww->focus && windoww->focus->event) {
				windoww->focus->event(windoww->focus, evnt);
			}
			s_event_uninit(evnt);
			windoww->focus = objectn;
		}
		if (objectp && (objectn != objectp) && (objectp->event)) {
			objectp->event(objectp, event);
		}
		if (objectn && objectn->event) {
			objectn->event(objectn, event);
		}
		if (event->type & MOUSE_PRESSED) {
			if (objectn && objectn->event) {
				windoww->event = objectn;
			}
		}
	}
	if (event->type & KEYBD_EVENT) {
		if (event->type & KEYBD_PRESSED &&
		    event->keybd->keycode == S_KEYCODE_TAB) {
			w_window_change_keybd_focus(window);
		}
		if (windoww->event && windoww->event->event) {
			windoww->event->event(windoww->event, event);
		}
	}
	if (event->type & SIGNAL_EVENT) {
		w_signal_t *signal;
		signal = (w_signal_t *) event->data;
		signal->func(signal);
		s_free(signal);
	}
	if (event->type & QUIT_EVENT) {
		w_window_uninit(windoww);
	}
}

int w_window_init (w_window_t **window, S_WINDOW type, w_window_t *parent)
{
	(*window) = (w_window_t *) s_malloc(sizeof(w_window_t));
	s_client_init(&((*window)->window));
	s_window_new((*window)->window, type, NULL);
	s_window_set_resizeable((*window)->window, 0);
	w_object_init(*window, &((*window)->object), NULL, NULL);
	(*window)->focus = NULL;
	(*window)->event = NULL;
	s_client_atevent((*window)->window, w_window_atevent);
	(*window)->window->client->data = (*window);
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
