/***************************************************************************
    begin                : Thu Mar 10 2005
    copyright            : (C) 2005 - 2007 by Alper Akcan
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

int w_effect_stop (w_object_t *object)
{
	s_timer_del(object->window->window, object->effect->timer);
	object->effect->interval = 0;
	return 0;
}

void w_effect_timer_cb (s_window_t *window, s_timer_t *timer)
{
	w_object_t *object;
	object = (w_object_t *) timer->data;
	if (object->effect->interval <= 0) {
		s_timer_del(window, timer);
		if (!object->showed && object->effect->effect & EFFECT_HIDE) {
            		s_list_remove(object->parent->shown, s_list_get_pos(object->parent->shown, object));
			{
				w_object_hide_(object);
			}
		}
	}
	w_object_update(object, object->surface->win);
	object->effect->interval -= 1;
}

int w_effect_start (w_object_t *object)
{
	w_effect_stop(object);
	if (object->effect->level <= 0) {
		object->effect->level = 20;
	}
	if (object->effect->interval <= 0) {
		object->effect->interval = object->effect->level;
	}
	if (object->effect->timer->timeval <= 0) {
		object->effect->timer->timeval = 50;
	}
	object->effect->timer->cb = w_effect_timer_cb;
	object->effect->timer->data = object;
	s_timer_add(object->window->window, object->effect->timer);
	return 0;
}

int w_effect_apply_ (w_object_t *effect, w_object_t *object)
{
	if (effect != NULL &&
	    effect->effect->effect != EFFECT_NONE &&
	    effect->effect->interval > 0 &&
	    (effect == object || w_object_isshownchild(effect, object) == 0)) {
	    	return 0;
	}
	return -1;
}

int w_effect_apply (s_surface_t *surface, s_rect_t *rect, w_object_t *effect, w_object_t *object)
{
	if (w_effect_apply_(effect, object)) {
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
			unsigned char *box;
			unsigned char *scl;
			if (effect == object) {
				if (s_surface_create(&srf, surface->width, surface->height, surface->bitsperpixel)) {
					return -1;
				}
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
				s_surface_destroy(srf);
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
			unsigned char *box;
			unsigned char *scl;
			if (effect == object) {
				if (s_surface_create(&srf, surface->width, surface->height, surface->bitsperpixel)) {
					return -1;
				}
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
				s_surface_destroy(srf);
			}
			return 0;
		}
	}

	return -1;
}
