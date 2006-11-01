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

int w_object_effect_stop (w_object_t *object)
{
	s_timer_del(object->window->window, object->effect->timer);
	object->effect->interval = 0;
	return 0;
}

void w_object_effect_timer_cb (s_window_t *window, s_timer_t *timer)
{
	w_object_t *object;
	object = (w_object_t *) timer->data;
	if (object->effect->interval <= 0) {
		s_timer_del(window, timer);
		if (!object->showed && object->effect->effect & EFFECT_HIDE) {
            		s_list_remove(object->parent->shown, s_list_get_pos(object->parent->shown, object));
		}
	}
	w_object_update(object, object->surface->win);
	object->effect->interval -= 1;
}

int w_object_effect_start (w_object_t *object)
{
	w_object_effect_stop(object);
	if (object->effect->level <= 0) {
		object->effect->level = 20;
	}
	if (object->effect->interval <= 0) {
		object->effect->interval = object->effect->level;
	}
	if (object->effect->timer->timeval <= 0) {
		object->effect->timer->timeval = 50;
	}
	object->effect->timer->cb = w_object_effect_timer_cb;
	object->effect->timer->data = object;
	s_timer_add(object->window->window, object->effect->timer);
	return 0;
}

int w_object_has_effect (w_object_t *effect, w_object_t *object)
{
	if (effect != NULL &&
	    effect->effect->effect != EFFECT_NONE &&
	    effect->effect->interval > 0 &&
	    (effect == object || w_object_isshownchild(effect, object) == 0)) {
	    	return 0;
	}
	return -1;
}

int w_object_effect_apply (s_surface_t *surface, s_rect_t *rect, w_object_t *effect, w_object_t *object)
{
	if (w_object_has_effect(effect, object)) {
		return -1;
	}
	
	if (effect->showed) {
		if (effect->effect->effect & EFFECT_FADEIN) {
			unsigned int i;
			unsigned char *mat;
			unsigned char *tmp;
			mat = (unsigned char *) s_malloc(sizeof(char) * object->surface->width * object->surface->height);
			memcpy(mat, object->surface->matrix, object->surface->width * object->surface->height);
			i = object->surface->width * object->surface->height;
			tmp = mat;
			while (i--) {
				*tmp = (*tmp * (effect->effect->level - effect->effect->interval)) / effect->effect->level;
				tmp++;
			}
			s_putboxpartalpha(surface, rect->x, rect->y, rect->w, rect->h,
			                           object->surface->width, object->surface->height,
			       	                   object->surface->vbuf, mat,
						   rect->x - object->surface->win->x,
						   rect->y - object->surface->win->y);
			s_free(mat);
			return 0;
		} else if (effect->effect->effect & EFFECT_POPIN) {
			s_rect_t new;
			s_surface_t *srf;
			unsigned char *buf;
			unsigned char *box;
			unsigned char *scl;
			if (effect == object) {
				srf = (s_surface_t *) s_malloc(sizeof(s_surface_t));
				buf = (unsigned char *) s_malloc(sizeof(char) * surface->width * surface->height * object->surface->bytesperpixel);
				s_getsurfacevirtual(srf, surface->width, surface->height, surface->bitsperpixel, buf);
				w_object_update_to_surface(object, srf, object->surface->win, effect, EFFECT_NONE);
				box = (unsigned char *) s_malloc(sizeof(char) * object->surface->width * object->surface->height * object->surface->bytesperpixel);
				s_getbox(srf, object->surface->win->x, object->surface->win->y, object->surface->win->w, object->surface->win->h, box);
				new.w = (object->surface->width * (effect->effect->level - effect->effect->interval)) / effect->effect->level;
				new.h = (object->surface->height * (effect->effect->level - effect->effect->interval)) / effect->effect->level;
				scl = (unsigned char *) s_malloc(sizeof(char) * new.w * new.h * object->surface->bytesperpixel);
				s_scalebox(object->surface, object->surface->width, object->surface->height, box, new.w, new.h, scl);
				s_putboxpart(surface, object->surface->win->x, object->surface->win->y, new.w, new.h, new.w, new.h, scl, 0, 0);
				s_free(scl);
				s_free(box);
				s_free(buf);
				s_free(srf);
			}
			return 0;
		}
	} else if (!effect->showed) {
		if (effect->effect->effect & EFFECT_FADEOUT) {
			unsigned int i;
			unsigned char *mat;
			unsigned char *tmp;
			mat = (unsigned char *) s_malloc(sizeof(char) * object->surface->width * object->surface->height);
			memcpy(mat, object->surface->matrix, object->surface->width * object->surface->height);
			i = object->surface->width * object->surface->height;
			tmp = mat;
			while (i--) {
				*tmp = (*tmp * (effect->effect->interval)) / effect->effect->level;
				tmp++;
			}
			s_putboxpartalpha(surface, rect->x, rect->y, rect->w, rect->h,
			                           object->surface->width, object->surface->height,
			       	                   object->surface->vbuf, mat,
						   rect->x - object->surface->win->x,
						   rect->y - object->surface->win->y);
			s_free(mat);
			return 0;
		} else if (effect->effect->effect & EFFECT_POPOUT) {
			s_rect_t new;
			s_surface_t *srf;
			unsigned char *buf;
			unsigned char *box;
			unsigned char *scl;
			if (effect == object) {
				srf = (s_surface_t *) s_malloc(sizeof(s_surface_t));
				buf = (unsigned char *) s_malloc(sizeof(char) * surface->width * surface->height * object->surface->bytesperpixel);
				s_getsurfacevirtual(srf, surface->width, surface->height, surface->bitsperpixel, buf);
				w_object_update_to_surface(object, srf, object->surface->win, effect, EFFECT_NONE);
				box = (unsigned char *) s_malloc(sizeof(char) * object->surface->width * object->surface->height * object->surface->bytesperpixel);
				s_getbox(srf, object->surface->win->x, object->surface->win->y, object->surface->win->w, object->surface->win->h, box);
				new.w = (object->surface->width * effect->effect->interval) / effect->effect->level;
				new.h = (object->surface->height * effect->effect->interval) / effect->effect->level;
				scl = (unsigned char *) s_malloc(sizeof(char) * new.w * new.h * object->surface->bytesperpixel);
				s_scalebox(object->surface, object->surface->width, object->surface->height, box, new.w, new.h, scl);
				s_putboxpart(surface, object->surface->win->x, object->surface->win->y, new.w, new.h, new.w, new.h, scl, 0, 0);
				s_free(scl);
				s_free(box);
				s_free(buf);
				s_free(srf);
			}
			return 0;
		}
	}

	return -1;
}

int w_object_update_to_surface (w_object_t *object, s_surface_t *surface, s_rect_t *coor, w_object_t *effect, int do_effect)
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
	if (object->parent && s_rect_intersect(&bound, object->parent->surface->win, &update)) {
		goto end;
	}

#if defined(WIDGET_OPTIMIZE_MEMORY)
	if (object->parent != NULL) {
		object->surface->matrix = (unsigned char *) s_malloc(sizeof(char) * object->surface->width * object->surface->height + 10);
		object->surface->vbuf = (char *) s_calloc(1, object->surface->width * object->surface->height * object->surface->bytesperpixel + 1);
		memset(object->surface->matrix, 0xff, sizeof(char) * object->surface->width * object->surface->height);
		if (object->draw != NULL) {
			object->draw(object);
		}
	}
#endif

        if (object->parent != NULL &&
            object->surface->matrix != NULL &&
            object->surface->vbuf != NULL) {
		if (do_effect == EFFECT_NONE || w_object_effect_apply(surface, &update, effect, object)) {
			s_putboxpartalpha(surface, update.x, update.y, update.w, update.h,
			                           object->surface->width, object->surface->height,
			       	                   object->surface->vbuf, object->surface->matrix,
						   update.x - object->surface->win->x,
						   update.y - object->surface->win->y);
		}
	}

#if defined(WIDGET_OPTIMIZE_MEMORY)
	if (object->parent != NULL) {
		s_free(object->surface->vbuf);
		s_free(object->surface->matrix);
		object->surface->vbuf = NULL;
		object->surface->matrix = NULL;
	}
#endif

	while (!(s_list_eol(object->shown, pos))) {
		w_object_t *obj = (w_object_t *) s_list_get(object->shown, pos);
		w_object_update_to_surface(obj, surface, coor, effect, do_effect);
		pos++;
	}

end:	return 0;
}

int w_object_update (w_object_t *object, s_rect_t *coor)
{
	s_rect_t clip;
	w_object_t *effect;
	if (object == NULL) {
		goto end;
	}
	effect = object;
	while (object->parent != NULL) {
		object = object->parent;
	}
	if (object->draw == NULL) {
		s_fillbox(object->surface, coor->x, coor->y, coor->w, coor->h, 0);
	} else {
		object->draw(object);
	}
	if (s_rect_intersect(coor, object->surface->win, &clip) == 0) {
		w_object_update_to_surface(object, object->surface, &clip, effect, EFFECT_SHOW | EFFECT_HIDE);
		s_putboxpart(object->window->window->surface, clip.x, clip.y, clip.w, clip.h, object->surface->width, object->surface->height, object->surface->vbuf, clip.x, clip.y);
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

int w_object_move_correct (w_object_t *object)
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
#if defined(WIDGET_OPTIMIZE_MEMORY)
#else
		object->surface->matrix = (unsigned char *) s_malloc(sizeof(char) * object->surface->width * object->surface->height + 10);
		object->surface->vbuf = (char *) s_calloc(1, object->surface->width * object->surface->height * object->surface->bytesperpixel + 1);
		memset(object->surface->matrix, 0xff, sizeof(char) * object->surface->width * object->surface->height);
		if (object->draw != NULL) {
			object->draw(object);
		}
#endif
	}
	
	if (object->parent != NULL &&
	    object->showed == 1) {
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
	}

	return 0;
}

int w_object_hide (w_object_t *object)
{
	if (object == object->window->focus) {
		w_window_change_keybd_focus(object->window->window, 0);
	}
	object->showed = 0;
	if (object->parent != NULL) {
		if ((object->effect->effect & EFFECT_HIDE) == 0) {
			s_list_remove(object->parent->shown, s_list_get_pos(object->parent->shown, object));
		}
	}
	w_object_effect_stop(object);
	if (object->effect->effect & EFFECT_HIDE) {
		w_object_effect_start(object);
	} else {
		w_object_update(object, object->surface->win);
	}
	return 0;
}

int w_object_show (w_object_t *object)
{
	object->showed = 1;
	if (object->parent != NULL) {
		s_list_remove(object->parent->shown, s_list_get_pos(object->parent->shown, object));
            	s_list_add(object->parent->shown, object, -1);
	}
	w_object_effect_stop(object);
	if (object->effect->effect & EFFECT_SHOW) {
		w_object_effect_start(object);
	} else {
		w_object_update(object, object->surface->win);
	}
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

void w_object_signal (w_object_t *from, w_object_t *to, void (*func) (w_signal_t *), void *arg)
{
	s_event_t *event;
	w_signal_t *signal;
	s_event_init(&event);
	event->type = SIGNAL_EVENT;
	signal = (w_signal_t *) s_malloc(sizeof(w_signal_t));
	signal->from = from;
	signal->to  = to;
	signal->func = func;
	signal->arg = arg;
	event->data = (void *) signal;
	s_eventq_add(to->window->window, event);
}

int w_object_level_get_ (w_object_t *parent, w_object_t **object, int *level)
{
	int pos = 0;
	w_object_t *temp = NULL;
	if (*level == 0) {
		*object = parent;
		return 0;
	}
	while (!s_list_eol(parent->shown, pos)) {
		temp = (w_object_t *) s_list_get(parent->shown, pos);
		pos++;
		(*level)--;
		if ((*level) < 0) {
			*object = NULL;
			return -1;
		}
		if ((*level) == 0) {
			*object = temp;
			return 0;
		}
		if (w_object_level_get_(temp, object, level) == 0) {
			return 0;
		}
	}
	return -1;
}

int w_object_level_get (w_object_t *parent, w_object_t **object, int level)
{
	int levels;
	int l = level;
	w_object_level_count(parent, &levels);
	if (levels < l) {
		*object = NULL;
		return -1;
	}
	return w_object_level_get_(parent, object, &l);
}

int w_object_level_count_ (w_object_t *parent, int *level)
{
	int pos = 0;
	w_object_t *temp;
	if (parent == NULL) {
		return -1;
	}
	while (!s_list_eol(parent->shown, pos)) {
		temp = (w_object_t *) s_list_get(parent->shown, pos);
		pos++;
		(*level)++;
		w_object_level_count_(temp, level);
	}
	return 0;
}

int w_object_level_count (w_object_t *parent, int *level)
{
	*level = 0;
	return w_object_level_count_(parent, level);
}

int w_object_level_find_ (w_object_t *parent, w_object_t *object, int *level)
{
	int pos = 0;
	w_object_t *temp;
	if (object == NULL) {
		return -1;
	}
	if (parent == object) {
		return 0;
	}
	while (!s_list_eol(parent->shown, pos)) {
		temp = (w_object_t *) s_list_get(parent->shown, pos);
		pos++;
		(*level)++;
		if (temp == object) {
			return 0;
		}
		if (w_object_level_find_(temp, object, level) == 0) {
			return 0;
		}
	}
	return -1;
}

int w_object_level_find (w_object_t *parent, w_object_t *object, int *level)
{
	*level = 0;
	return w_object_level_find_(parent, object, level);
}

int w_object_isshownchild (w_object_t *parent, w_object_t *child)
{
	int pos = 0;
	w_object_t *tmp;
	while (!s_list_eol(parent->shown, pos)) {
		tmp = (w_object_t *) s_list_get(parent->shown, pos);
		if (tmp == child) {
			return 0;
		}
		if (w_object_isshownchild(tmp, child) == 0) {
			return 0;
		}
		pos++;
	}
	return -1;
}

int w_object_ischild (w_object_t *parent, w_object_t *child)
{
	int pos = 0;
	w_object_t *tmp;
	while (!s_list_eol(parent->childs, pos)) {
		tmp = (w_object_t *) s_list_get(parent->childs, pos);
		if (tmp == child) {
			return 0;
		}
		if (w_object_ischild(tmp, child) == 0) {
			return 0;
		}
		pos++;
	}
	return -1;
}

int w_object_init (w_window_t *window, w_object_t **object, void (*draw) (w_object_t *), w_object_t *parent)
{
	(*object) = (w_object_t *) s_malloc(sizeof(w_object_t));
	(*object)->surface = (s_surface_t *) s_malloc(sizeof(s_surface_t));
	(*object)->surface->buf = (s_rect_t *) s_malloc(sizeof(s_rect_t));
	(*object)->surface->win = (s_rect_t *) s_malloc(sizeof(s_rect_t));
	(*object)->surface->matrix = NULL;
	s_getsurfacevirtual((*object)->surface, 0, 0, window->window->surface->bitsperpixel, NULL);

	s_list_init(&((*object)->shown));
	s_list_init(&((*object)->childs));
	(*object)->parent = parent;
	
	(*object)->focused = 0;
	(*object)->showed = 0;
	
	(*object)->effect = (w_effect_t *) s_malloc(sizeof(w_effect_t));
	(*object)->effect->effect = EFFECT_NONE;
	(*object)->effect->level = 0;
	(*object)->effect->interval = 0;
	s_timer_init(&((*object)->effect->timer));

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
		s_list_add(parent->childs, *object, -1);
	} else {
		(*object)->surface->buf->w = (*object)->surface->width;
		(*object)->surface->buf->h = (*object)->surface->height;
		(*object)->surface->win->w = (*object)->surface->width;
		(*object)->surface->win->h = (*object)->surface->height;
	}

	(*object)->content->x = (*object)->surface->buf->x;
	(*object)->content->y = (*object)->surface->buf->y;
	(*object)->content->w = (*object)->surface->buf->w;
	(*object)->content->h = (*object)->surface->buf->h;
	
	return 0;
}

void w_object_uninit (w_object_t *object)
{
	int pos;
	s_event_t *event;
	w_signal_t *signal;
        s_thread_mutex_lock(object->window->window->eventq->mut);
	pos = 0;
	while (!s_list_eol(object->window->window->eventq->queue, pos)) {
		event = (s_event_t *) s_list_get(object->window->window->eventq->queue, pos);
		signal = (w_signal_t *) event->data;
		if ((event->type == SIGNAL_EVENT) &&
		    ((signal->from == object) || (signal->to == object))) {
			s_list_remove(object->window->window->eventq->queue, pos);
			s_free(signal);
			s_event_uninit(event);
		} else {
			pos++;
		}
	}
	s_thread_mutex_unlock(object->window->window->eventq->mut);
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
		if (obj->destroy != NULL) {
			obj->destroy(obj);
		} else {
			w_object_uninit(obj);
		}
	}
	while (!s_list_eol(object->shown, 0)) {
		s_list_remove(object->shown, 0);
	}
	s_timer_uninit(object->effect->timer);
	s_free(object->effect);
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
