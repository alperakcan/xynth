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

#include "../lib/xynth_.h"
#include "widget.h"

int w_object_update_to_surface (w_object_t *object, s_surface_t *surface, s_rect_t *coor)
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
		s_putboxpartalpha(surface, update.x, update.y, update.w, update.h,
		                           object->surface->width, object->surface->height,
	        	                   object->surface->vbuf, object->surface->matrix,
					   update.x - object->surface->win->x,
					   update.y - object->surface->win->y);
	}

	while (!(s_list_eol(object->shown, pos))) {
		w_object_t *obj = (w_object_t *) s_list_get(object->shown, pos);
		w_object_update_to_surface(obj, surface, coor);
		pos++;
	}

end:	return 0;
}

int w_object_update (w_object_t *object, s_rect_t *coor)
{
	s_rect_t clip;
	if (object == NULL) {
		goto end;
	}
	while (object->parent != NULL) {
		object = object->parent;
	}
	if (object->draw == NULL) {
		s_fillbox(object->surface, coor->x, coor->y, coor->w, coor->h, 0);
	} else {
		object->draw(object);
	}
	if (s_rect_intersect(coor, object->surface->win, &clip) == 0) {
		w_object_update_to_surface(object, object->surface, &clip);
		s_putboxpart(object->window->surface, clip.x, clip.y, clip.w, clip.h, object->surface->width, object->surface->height, object->surface->vbuf, clip.x, clip.y);
	}
end:	return 0;
}

int w_object_set_content (w_object_t *object, int x, int y, int w, int h)
{
	object->content->x = x;
	object->content->y = y;
	object->content->w = w;
	object->content->h = h;
	return 0;
}

static int w_object_move_correct (w_object_t *object)
{
	int pos = 0;
	while (!(s_list_eol(object->childs, pos))) {
		w_object_t *obj = (w_object_t *) s_list_get(object->childs, pos);
		obj->surface->win->x = object->surface->win->x + obj->surface->buf->x;
		obj->surface->win->y = object->surface->win->y + obj->surface->buf->y;
		w_object_move_correct(obj);
		pos++;	
	}
	return 0;
}

int w_object_move (w_object_t *object, int x, int y, int w, int h)
{
	s_rect_t old;
	s_rect_t new;
	s_rect_t *tmp;
	s_list_t *diff;
	w_object_t *root;

        s_thread_mutex_lock(object->mut);

	if (w < 0) { w = 0; }
	if (h < 0) { h = 0; }

	old = *(object->surface->win);

        /* coordinates in bounds of object`s parent */
        new.x = x;
        new.y = y;
        new.w = w;
        new.h = h;

        if (object->parent != NULL) {
        	if (s_rect_intersect(&new, object->parent->content, object->surface->buf)) {
	        	/* error, do not draw this child */
        		object->surface->buf->x = x;
        		object->surface->buf->y = y;
        		object->surface->buf->w = 0;
        		object->surface->buf->h = 0;
        	}
        	/* coordinates in bounds of object`s root */
		object->surface->win->x = object->parent->surface->win->x + object->surface->buf->x;
		object->surface->win->y = object->parent->surface->win->y + object->surface->buf->y;
		object->surface->win->w = object->surface->buf->w;
		object->surface->win->h = object->surface->buf->h;
        } else {
        	object->surface->buf->x = new.x;
        	object->surface->buf->y = new.y;
        	object->surface->buf->w = new.w;
        	object->surface->buf->h = new.h;
        	object->surface->win->x = new.x;
        	object->surface->win->y = new.y;
        	object->surface->win->w = new.w;
        	object->surface->win->h = new.h;
        }
	
	/* re set the content rectangle */
	object->content->x = 0;
	object->content->y = 0;
	object->content->w = object->surface->buf->w;
	object->content->h = object->surface->buf->h;
	
	root = object;
	while (root->parent) {
		root = root->parent;
	}
	w_object_move_correct(root);

	if (object->geometry != NULL) {
		object->geometry(object);
	}

	if ((old.w != object->surface->buf->w) ||
	    (old.h != object->surface->buf->h)) {
		/* re prepare matrix and surface buffer */
		s_free(object->surface->matrix);
		s_free(object->surface->vbuf);

		object->surface->width = object->surface->buf->w;
		object->surface->height = object->surface->buf->h;
		object->surface->matrix = (unsigned char *) s_malloc(sizeof(char) * object->surface->width * object->surface->height + 10);
		object->surface->vbuf = (char *) s_calloc(1, object->surface->width * object->surface->height * object->surface->bytesperpixel + 1);
		memset(object->surface->matrix, 0xff, sizeof(char) * object->surface->width * object->surface->height);

		if (object->draw != NULL) {
			object->draw(object);
		}
	}

	s_list_init(&diff);
	s_rect_difference(&old, object->surface->win, diff);
	while (!s_list_eol(diff, 0)) {
		tmp = (s_rect_t *) s_list_get(diff, 0);
		w_object_update(object, tmp);
		s_list_remove(diff, 0);
		s_free(tmp);
	}
	s_list_uninit(diff);
	w_object_update(object, object->surface->win);

        s_thread_mutex_unlock(object->mut);

	return 0;
}

int w_object_hide (w_object_t *object)
{
        s_thread_mutex_lock(object->mut);
        if (object->parent != NULL) {
		int pos = 0;
		while (!s_list_eol(object->parent->shown, pos)) {
			w_object_t *obj = (w_object_t *) s_list_get(object->parent->shown, pos);
			if (obj == object) {
				s_list_remove(object->parent->shown, pos);
				w_object_update(object, object->surface->win);
				break;
			}
			pos++;
		}
	}
        s_thread_mutex_unlock(object->mut);
	return 0;
}

int w_object_show (w_object_t *object)
{
	int pos;
	int found;
	w_object_t *obj;
        s_thread_mutex_lock(object->mut);
        if (object->parent != NULL) {
		pos = 0;
		found = 0;
		while (!s_list_eol(object->parent->childs, pos)) {
			obj = (w_object_t *) s_list_get(object->parent->childs, pos);
			if (obj == object) {
				found = 1;
				break;
			}
			pos++;
		}
		if (found == 1) {
			pos = 0;
			found = 0;
			while (!s_list_eol(object->parent->shown, pos)) {
				obj = (w_object_t *) s_list_get(object->parent->shown, pos);
				if (obj == object) {
					if ((pos + 1) != object->parent->shown->nb_elt) {
						s_list_remove(object->parent->shown, pos);
					} else {
						found = 1;
					}
					break;
				}
				pos++;
			}
			if (!found) {
				s_list_add(object->parent->shown, object, -1);
			}
			w_object_update(object, object->surface->win);
		}
        } else {
        	w_object_update(object, object->surface->win);
        }
        s_thread_mutex_unlock(object->mut);
	return 0;
}

int w_object_childatposition (w_object_t *object, int x, int y, w_object_t **child)
{
	int pos;
	s_rect_t coor;
	s_rect_t rect;
	(*child) = NULL;
	coor.x = x;
	coor.y = y;
	coor.w = 1;
	coor.h = 1;
	pos = 0;
	while (!s_list_eol(object->shown, pos)) {
		w_object_t *obj = (w_object_t *) s_list_get(object->shown, pos);
		if (s_rect_intersect(obj->surface->buf, &coor, &rect)) {
		} else {
			(*child) = obj;
		}
		pos++;
	}
	return (*child) ? 0 : -1;
}

int w_object_atposition (w_object_t *root, int x, int y, w_object_t **object)
{
	w_object_t *tmp;
	tmp = root;
	while (tmp->parent) {
		tmp = tmp->parent;
	}
	do {
		*object = tmp;
		x -= (*object)->surface->buf->x;
		y -= (*object)->surface->buf->y;
	} while (w_object_childatposition(*object, x, y, &tmp) == 0);
	return 0;
}

int w_object_init (s_window_t *window, w_object_t **object, void (*draw) (w_object_t *), w_object_t *parent)
{
	(*object) = (w_object_t *) s_malloc(sizeof(w_object_t));
	(*object)->surface = (s_surface_t *) s_malloc(sizeof(s_surface_t));
	(*object)->surface->buf = (s_rect_t *) s_malloc(sizeof(s_rect_t));
	(*object)->surface->win = (s_rect_t *) s_malloc(sizeof(s_rect_t));
	(*object)->surface->matrix = NULL;
	s_getsurfacevirtual((*object)->surface, 0, 0, window->surface->bitsperpixel, NULL);

	s_list_init(&((*object)->shown));
	s_list_init(&((*object)->childs));
	(*object)->parent = parent;

 	(*object)->event = NULL;
 	memset((*object)->data, 0, sizeof(void *) * OBJECT_OBJECTS);
 	(*object)->geometry = NULL;
	(*object)->draw = draw;
	(*object)->destroy = w_object_uninit;
	(*object)->window = window;

	(*object)->content = (s_rect_t *) s_malloc(sizeof(s_rect_t));

	(*object)->surface->buf->x = 0;
	(*object)->surface->buf->y = 0;
	(*object)->surface->buf->w = 0;
	(*object)->surface->buf->h = 0;
	(*object)->surface->win->x = 0;
	(*object)->surface->win->y = 0;
	(*object)->surface->win->w = 0;
	(*object)->surface->win->h = 0;

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

	(*object)->content->x = (*object)->surface->buf->x;
	(*object)->content->y = (*object)->surface->buf->y;
	(*object)->content->w = (*object)->surface->buf->w;
	(*object)->content->h = (*object)->surface->buf->h;
	
	return 0;
err0:	s_free((*object)->surface->buf);
	s_free((*object)->surface->win);
	s_free((*object)->surface);
	s_list_uninit((*object)->shown);
	s_list_uninit((*object)->childs);
	s_free((*object)->content);
	s_free(*object);
	return 1;
}

void w_object_uninit (w_object_t *object)
{
	s_thread_mutex_lock(object->mut);
        if (object->parent != NULL) {
		int pos = 0;
		while (!s_list_eol(object->parent->childs, pos)) {
			w_object_t *obj = (w_object_t *) s_list_get(object->parent->childs, pos);
			if (obj == object) {
				s_list_remove(object->parent->childs, pos);
				break;
			}
			pos++;
		}
		pos = 0;
		while (!s_list_eol(object->parent->shown, pos)) {
			w_object_t *obj = (w_object_t *) s_list_get(object->parent->shown, pos);
			if (obj == object) {
				s_list_remove(object->parent->shown, pos);
				break;
			}
			pos++;
		}
	}
        while (!s_list_eol(object->childs, 0)) {
		w_object_t *obj = (w_object_t *) s_list_get(object->childs, 0);
		s_list_remove(object->childs, 0);
		s_thread_mutex_unlock(object->mut);
		if (obj->destroy != NULL) {
			obj->destroy(obj);
		} else {
			w_object_uninit(obj);
		}
		s_thread_mutex_lock(object->mut);
	}
	while (!s_list_eol(object->shown, 0)) {
		s_list_remove(object->shown, 0);
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
	s_free(object->content);
	s_list_uninit(object->shown);
	s_list_uninit(object->childs);
	s_free(object);
}
