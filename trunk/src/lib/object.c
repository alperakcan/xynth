/***************************************************************************
    begin                : Thu Mar 10 2005
    copyright            : (C) 2005 - 2006 by Alper Akcan
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

int s_object_update_to_surface (s_object_t *object, s_surface_t *surface, s_rect_t *coor)
{
        int pos = 0;
	s_rect_t bound;
	s_rect_t update;
        
	if (object == NULL) {
		goto end;
	}
	if (s_rect_intersect(coor, object->surface->win, &bound)) {
		goto end;
	}
	
        if (object->parent != NULL) {
		if (s_rect_intersect(&bound, object->parent->surface->win, &update)) {
			goto end;
		}
		s_putboxpartmask(surface, update.x, update.y, update.w, update.h,
		                          object->surface->width, object->surface->height,
	        	                  object->surface->vbuf, object->surface->matrix,
					  update.x - object->surface->win->x,
					  update.y - object->surface->win->y);
	}

	while (!(s_list_eol(object->childs, pos))) {
		s_object_t *obj = (s_object_t *) s_list_get(object->childs, pos);
		s_object_update_to_surface(obj, surface, coor);
		pos++;
	}

end:	return 0;
}

int s_object_update (s_object_t *object, s_rect_t *coor)
{
        s_object_t *parent;

	if (object == NULL) {
		goto end;
	}
	if (object->parent == NULL) {
		s_fillbox(object->surface, coor->x, coor->y, coor->w, coor->h, 0);
		s_object_update_to_surface(object, object->surface, coor);
		goto end;
	}
	parent = object;
	while (1) {
		if (parent->parent != NULL) {
			parent = parent->parent;
		} else {
			s_fillbox(parent->surface, coor->x, coor->y, coor->w, coor->h, 0);
			s_object_update_to_surface(parent, parent->surface, coor);
			goto end;
		}
	}

end:	return 0;
}

int s_object_move (s_object_t *object, int x, int y, int w, int h)
{
        int pos = 0;
        s_rect_t old;
	s_rect_t *tmp;
	s_list_t *diff;

        s_thread_mutex_lock(object->mut);

	if (w < 0) { w = 0; }
	if (h < 0) { h = 0; }

	old = *(object->surface->win);

        /* coordinates in bounds of object`s parent */
	object->surface->buf->x = x;
	object->surface->buf->y = y;
	object->surface->buf->w = w;
	object->surface->buf->h = h;

        /* coordinates in bounds of object`s root */
	object->surface->win->x = object->parent->surface->win->x + object->surface->buf->x;
	object->surface->win->y = object->parent->surface->win->y + object->surface->buf->y;
	object->surface->win->w = w;
	object->surface->win->h = h;
	
	if ((old.w != w) || (old.h != h)) {
		/* re prepare matrix and surface buffer */
		s_free(object->surface->matrix);
		s_free(object->surface->vbuf);
		object->surface->matrix = (unsigned char *) s_malloc(sizeof(char) * w * h + 1);
		memset(object->surface->matrix, 1, sizeof(char) * w * h);
		object->surface->vbuf = (char *) s_calloc(1, w * h * object->surface->bytesperpixel + 1);
		object->surface->width = w;
		object->surface->height = h;
		if (object->draw != NULL) {
			object->draw(object);
		}
	}

	while (!(s_list_eol(object->childs, pos))) {
		s_object_t *obj = (s_object_t *) s_list_get(object->childs, pos);
		obj->surface->win->x = object->surface->win->x + obj->surface->buf->x;
		obj->surface->win->y = object->surface->win->x + obj->surface->buf->y;
		pos++;
	}

	s_list_init(&diff);
	s_rect_difference(&old, object->surface->win, diff);
	while (!s_list_eol(diff, 0)) {
		tmp = (s_rect_t *) s_list_get(diff, 0);
		s_object_update(object, tmp);
		s_list_remove(diff, 0);
		s_free(tmp);
	}
	s_list_uninit(diff);
	s_object_update(object, object->surface->win);

        s_thread_mutex_unlock(object->mut);

	return 0;
}

int s_object_hide (s_object_t *object)
{
        s_thread_mutex_lock(object->mut);
        if (object->parent != NULL) {
		int pos = 0;
		while (!s_list_eol(object->parent->childs, pos)) {
			s_object_t *obj = (s_object_t *) s_list_get(object->parent->childs, pos);
			if (obj == object) {
				s_list_remove(object->parent->childs, pos);
				break;
			}
			pos++;
		}
		s_object_update(object, object->surface->win);
	}
        s_thread_mutex_unlock(object->mut);
	return 0;
}

int s_object_show (s_object_t *object)
{
        s_thread_mutex_lock(object->mut);
        if (object->parent != NULL) {
		int pos = 0;
		int found = 0;
		while (!s_list_eol(object->parent->childs, pos)) {
			s_object_t *obj = (s_object_t *) s_list_get(object->parent->childs, pos);
			if (obj == object) {
				if ((pos + 1) != object->parent->childs->nb_elt) {
					s_list_remove(object->parent->childs, pos);
				} else {
					found = 1;
				}
				break;
			}
			pos++;
		}
		if (!found) {
			s_list_add(object->parent->childs, object, -1);
		}
		s_object_update(object, object->surface->win);
	}
        s_thread_mutex_unlock(object->mut);
	return 0;
}

int s_object_init (s_window_t *window, s_object_t **object, int w, int h, void (*draw) (s_object_t *), s_object_t *parent)
{
        char *vbuf;

	(*object) = (s_object_t *) s_malloc(sizeof(s_object_t));
	(*object)->surface = (s_surface_t *) s_malloc(sizeof(s_surface_t));
	(*object)->surface->buf = (s_rect_t *) s_malloc(sizeof(s_rect_t));
	(*object)->surface->win = (s_rect_t *) s_malloc(sizeof(s_rect_t));
	(*object)->surface->matrix = (unsigned char *) s_malloc(sizeof(char) * w * h + 1);
	memset((*object)->surface->matrix, 1, sizeof(char) * w * h);
	s_list_init(&((*object)->childs));
        vbuf = (char *) s_calloc(1, w * h * window->surface->bytesperpixel + 1);
	s_getsurfacevirtual((*object)->surface, w, h, window->surface->bitsperpixel, vbuf);
	(*object)->parent = parent;

	(*object)->surface->buf->x = 0;
	(*object)->surface->buf->y = 0;
	(*object)->surface->buf->w = 0;
	(*object)->surface->buf->h = 0;
	(*object)->surface->win->x = 0;
	(*object)->surface->win->y = 0;
	(*object)->surface->win->w = 0;
	(*object)->surface->win->h = 0;
	(*object)->draw = draw;

	if (parent != NULL) {
		s_thread_mutex_lock(parent->mut);
		s_list_add(parent->childs, *object, -1);
		(*object)->mut = parent->mut;
		s_thread_mutex_unlock(parent->mut);
	} else {
		(*object)->surface->buf->w = (*object)->surface->width;
		(*object)->surface->buf->h = (*object)->surface->height;
		(*object)->surface->win->w = (*object)->surface->width;
		(*object)->surface->win->h = (*object)->surface->height;
		if (s_thread_mutex_init(&((*object)->mut))) {
			goto err0;
		}
	}

	return 0;
err0:	s_free(vbuf);
	s_free((*object)->surface->matrix);
	s_free((*object)->surface->buf);
	s_free((*object)->surface->win);
	s_free((*object)->surface);
	s_free((*object)->childs);
	s_free(*object);
	return 1;
}

int s_object_uninit (s_object_t *object)
{
	s_thread_mutex_lock(object->mut);
        if (object->parent != NULL) {
		int pos = 0;
		while (!s_list_eol(object->parent->childs, pos)) {
			s_object_t *obj = (s_object_t *) s_list_get(object->parent->childs, pos);
			if (obj == object) {
				s_list_remove(object->parent->childs, pos);
				break;
			}
			pos++;
		}
	}
        while (!s_list_eol(object->childs, 0)) {
		s_object_t *obj = (s_object_t *) s_list_get(object->childs, 0);
		s_list_remove(object->childs, 0);
		s_thread_mutex_unlock(object->mut);
		s_object_uninit(obj);
		s_thread_mutex_lock(object->mut);
	}
	s_thread_mutex_unlock(object->mut);
	if (object->parent == NULL) {
		s_thread_mutex_destroy(object->mut);
	}
	s_free(object->surface->buf);
	s_free(object->surface->win);
	s_free(object->surface->vbuf);
	s_free(object->surface->matrix);
	s_free(object->surface);
	s_list_uninit(object->childs);
	s_free(object);
	return 0;
}
