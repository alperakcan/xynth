/***************************************************************************
    begin                : Mon Sep 11 2006
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

void w_window_focus_change_notify (s_window_t *window, w_object_t *focus)
{
	s_event_t *event;
	w_window_t *windoww;
	windoww = (w_window_t *) window->data;
	if (windoww->focus != focus) {
		s_event_init(&event);
		event->type = FOCUS_EVENT | FOCUSOUT_EVENT;
		if (windoww->focus && windoww->focus->event) {
			windoww->focus->focused = 0;
			windoww->focus->event(windoww->focus, event);
			if (windoww->focus->draw) {
				windoww->focus->draw(windoww->focus);
				w_object_update(windoww->focus, windoww->focus->surface->win);
			}
		}
		s_event_uninit(event);
		windoww->focus = focus;
		s_event_init(&event);
		event->type = FOCUS_EVENT | FOCUSIN_EVENT;
		if (windoww->focus && windoww->focus->event) {
			windoww->focus->focused = 1;
			windoww->focus->event(windoww->focus, event);
			if (windoww->focus->draw) {
				windoww->focus->draw(windoww->focus);
				w_object_update(windoww->focus, windoww->focus->surface->win);
			}
		}
		s_event_uninit(event);
	}
}

void w_window_change_keybd_focus (s_window_t *window, int type)
{
	int i;
	int l;
	int ls;
	w_object_t *temp;
	w_object_t *root;
	w_window_t *windoww;

	windoww = (w_window_t *) window->data;
	
	if (type == 0 || windoww->focus == NULL) {
		/* focus next object */
		root = windoww->object;
	} else if (type == 1) {
		/* focus next child oject of the same parent */
		root = windoww->focus->parent;
	} else if (type == 2) {
		/* focus next sister object of the same grand parent */
		root = windoww->object;
	} else {
		return;
	}

	w_object_level_find(root, windoww->focus, &l);
	w_object_level_count(root, &ls);
	for (i = 0; i < ls; i++) {
		l++;
		if (l > ls) {
			l = 0;
		}
		w_object_level_get(root, &temp, l);
		if (temp && temp->event) {
			if (type == 2 &&
			    windoww->focus &&
			    windoww->focus->parent &&
			    w_object_isshownchild(windoww->focus->parent, temp) == 0) {
			} else {
				w_window_focus_change_notify(window, temp);
				break;
			}
		}
	}
}

void w_window_atevent (s_window_t *window, s_event_t *event)
{
	int flag;
	w_object_t *objectn;
	w_object_t *objectp;
	w_window_t *windoww;
	windoww = (w_window_t *) window->data;
	if (event->type & (CONFIG_CHNGW | CONFIG_CHNGH)) {
		window->surface->width = window->surface->buf->w;
		window->surface->height = window->surface->buf->h;
		s_free(window->surface->vbuf);
		window->surface->vbuf = (unsigned char *) s_malloc(sizeof(char) * window->surface->bytesperpixel * window->surface->buf->w * window->surface->buf->h);
		windoww->object->surface->vbuf = window->surface->vbuf;
		w_object_update(windoww->object, windoww->object->surface->win);
	}
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
		if (event->type & MOUSE_PRESSED) {
			if (objectn && objectn->event) {
				w_window_focus_change_notify(window, objectn);
			}
		}
		if (objectp && (objectn != objectp) && (objectp->event)) {
			objectp->event(objectp, event);
		}
		if (objectn && objectn->event) {
			objectn->event(objectn, event);
		}
	}
	if (event->type & KEYBD_EVENT) {
		if (event->type & KEYBD_PRESSED &&
		    event->keybd->keycode == S_KEYCODE_TAB) {
		    	flag = (event->keybd->flag & KEYCODE_LSHIFTF) ? 1 : (event->keybd->flag & KEYCODE_RSHIFTF) ? 2 : 0;
		    	w_window_change_keybd_focus(window, flag);
		}
		if (windoww->focus && windoww->focus->event) {
			windoww->focus->event(windoww->focus, event);
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

int w_window_set_coor (w_window_t *window, int x, int y, int w, int h)
{
	s_window_set_coor(window->window, WINDOW_NOFORM, x, y, w, h);
	w_object_move(window->object, 0, 0, w, h);
	s_free(window->object->surface->vbuf);
	s_free(window->object->surface->matrix);
	window->object->surface->vbuf = window->window->surface->vbuf;
	window->object->surface->matrix = NULL;
	return 0;
}

s_image_t * w_window_image_get (w_window_t *window, char *image)
{
	int p;
	s_image_t *img;
	for (p = 0; !s_list_eol(window->images, p); p++) {
		img = (s_image_t *) s_list_get(window->images, p);
		if (strcmp(img->name, image) == 0) {
			return img;
		}
	}
	return NULL;
}

int w_window_image_add (w_window_t *window, char *image)
{
	s_image_t *img;
	if ((img = w_window_image_get(window, image)) == NULL) {
		s_image_init(&img);
		s_image_img(image, img);
		s_list_add(window->images, img, -1);
	}
	return 0;
}

s_font_t * w_window_font_get (w_window_t *window, char *font)
{
	int p;
	s_font_t *fnt;
	for (p = 0; !s_list_eol(window->fonts, p); p++) {
		fnt = (s_font_t *) s_list_get(window->fonts, p);
		if (strcmp(fnt->name, font) == 0) {
			return fnt;
		}
	}
	return NULL;
}

int w_window_font_add (w_window_t *window, char *font)
{
	s_font_t *fnt;
	if ((fnt = w_window_font_get(window, font)) == NULL) {
		s_font_init(&fnt, font);
		s_list_add(window->fonts, fnt, -1);
	}
	return 0;
}

int w_window_uninit (w_window_t *window)
{
	s_image_t *img;
	s_font_t *font;
	window->object->surface->vbuf = NULL;
	window->object->surface->matrix = NULL;
	w_object_uninit(window->object);
	while (!s_list_eol(window->images, 0)) {
		img = (s_image_t *) s_list_get(window->images, 0);
		s_list_remove(window->images, 0);
		s_image_uninit(img);
	}
	s_list_uninit(window->images);
	while (!s_list_eol(window->fonts, 0)) {
		font = (s_font_t *) s_list_get(window->fonts, 0);
		s_list_remove(window->fonts, 0);
		s_font_uninit(font);
	}
	s_list_uninit(window->fonts);
	s_free(window);
	return 0;
}

int w_window_init (w_window_t **window, S_WINDOW type, w_window_t *parent)
{
	(*window) = (w_window_t *) s_malloc(sizeof(w_window_t));
	s_window_init(&((*window)->window));
	s_window_new((*window)->window, type, NULL);
	s_window_set_resizeable((*window)->window, 0);
	w_object_init(*window, &((*window)->object), NULL, NULL);
	s_list_init(&((*window)->images));
	s_list_init(&((*window)->fonts));
	(*window)->focus = NULL;
	s_window_atevent((*window)->window, w_window_atevent);
	(*window)->window->data = (*window);
	(*window)->object->type = OBJECT_WINDOW;
	(*window)->object->window = *window;
	(*window)->object->data[OBJECT_WINDOW] = *window;
	return 0;
}
