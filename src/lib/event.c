/***************************************************************************
    begin                : Thu Feb 20 2003
    copyright            : (C) 2003 - 2009 by Alper Akcan
    email                : alper.akcan@gmail.com
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

int s_event_mouse_handler_state (s_window_t *window, s_event_t *event, s_handler_mouse_t *mouse, int over)
{
        int x = mouse->x + window->surface->buf->x;
        int y = mouse->y + window->surface->buf->y;
        int w = mouse->w;
        int h = mouse->h;
        int b = mouse->button;

	int overbit = EVENT_TYPE_MOUSE_OVER << 16;
        int type = (event->type & EVENT_TYPE_MOUSE_CLICKED) ? event->type &= ~EVENT_TYPE_MOUSE_RELEASED : event->type;

	if ((type & (EVENT_TYPE_MOUSE_PRESSED | EVENT_TYPE_MOUSE_CLICKED | EVENT_TYPE_MOUSE_RELEASED)) &&
	    !(event->mouse->b & b)) {
		return -1;
	}
	if ((event->mouse->x >= x) && (event->mouse->x <= (x + w - 1)) &&
	    (event->mouse->y >= y) && (event->mouse->y <= (y + h - 1))) {
		if (over != 1) {
			goto end;
		}
		mouse->hstate |= overbit;
		if (type & (EVENT_TYPE_MOUSE_RELEASED | EVENT_TYPE_MOUSE_CLICKED)) {
			if (mouse->hstate & event->mouse->b) {
				mouse->hstate &= ~event->mouse->b;
				return (type & ~EVENT_TYPE_MOUSE);
			} else {
				return (type & ~EVENT_TYPE_MOUSE) | EVENT_TYPE_MOUSE_HINT1;
			}
		}
		mouse->hstate |= event->mouse->b;
		return (type & ~EVENT_TYPE_MOUSE);
	} else {
		if (over != 0) {
			goto end;
		}
		if (type & (EVENT_TYPE_MOUSE_RELEASED | EVENT_TYPE_MOUSE_CLICKED)) {
			if (mouse->hstate & event->mouse->b) {
				mouse->hstate &= ~event->mouse->b;
				return (type & ~EVENT_TYPE_MOUSE) | EVENT_TYPE_MOUSE_HINT2;
			}
		}
		if (mouse->hstate & overbit) {
			mouse->hstate &= ~overbit;
			return (type & ~EVENT_TYPE_MOUSE) | EVENT_TYPE_MOUSE_HINT2;
		}
	}
end:	return -1;
}

int s_event_parse_handler_over (s_window_t *window, s_event_t *event, s_handler_t *work)
{
	switch (s_event_mouse_handler_state(window, event, &(work->mouse), 1)) {
		case EVENT_TYPE_MOUSE_OVER:
			if (work->mouse.o != NULL) {
				s_thread_mutex_unlock(window->handlers->mut);
				work->mouse.o(window, event, work);
				s_thread_mutex_lock(window->handlers->mut);
			}
			goto not_over;
		case EVENT_TYPE_MOUSE_PRESSED:
			if (work->mouse.p != NULL) {
				s_thread_mutex_unlock(window->handlers->mut);
				work->mouse.p(window, event, work);
				s_thread_mutex_lock(window->handlers->mut);
			}
			goto not_over;
		case EVENT_TYPE_MOUSE_CLICKED:
			if (work->mouse.c != NULL) {
				s_thread_mutex_unlock(window->handlers->mut);
				work->mouse.c(window, event, work);
				s_thread_mutex_lock(window->handlers->mut);
				work = (s_handler_t *) s_list_get(window->handlers->list, s_list_get_pos(window->handlers->list, work));
				if (work == NULL) {
					goto not_over;
				}
			}
			/* no break */
		case EVENT_TYPE_MOUSE_RELEASED:
			if (work->mouse.r != NULL) {
				s_thread_mutex_unlock(window->handlers->mut);
				work->mouse.r(window, event, work);
				s_thread_mutex_lock(window->handlers->mut);
			}
			goto not_over;
		case (EVENT_TYPE_MOUSE_RELEASED | EVENT_TYPE_MOUSE_HINT1):
			/* mouse button released, but the prev. press was not on us */
			if (work->mouse.hr != NULL) {
				s_thread_mutex_unlock(window->handlers->mut);
				work->mouse.hr(window, event, work);
				s_thread_mutex_lock(window->handlers->mut);
			}
			goto not_over;
		case (EVENT_TYPE_MOUSE_OVER | EVENT_TYPE_MOUSE_HINT1):
			/* on over, but mouse button is still pressed */
			if (work->mouse.ho != NULL) {
				s_thread_mutex_unlock(window->handlers->mut);
				work->mouse.ho(window, event, work);
				s_thread_mutex_lock(window->handlers->mut);
			}
			goto not_over;
	}
	return -1;
not_over:
	return 0;
}

int s_event_parse_handler_notover (s_window_t *window, s_event_t *event, s_handler_t *work)
{
	switch (s_event_mouse_handler_state(window, event, &(work->mouse), 0)) {
		case (EVENT_TYPE_MOUSE_OVER | EVENT_TYPE_MOUSE_HINT2):
			/* not on over, but was on over */
			if (work->mouse.oh != NULL) {
				s_thread_mutex_unlock(window->handlers->mut);
				work->mouse.oh(window, event, work);
				s_thread_mutex_lock(window->handlers->mut);
			}
			goto end;
		case (EVENT_TYPE_MOUSE_OVER | EVENT_TYPE_MOUSE_HINT1 | EVENT_TYPE_MOUSE_HINT2):
			/* not on over, but was on over. and button is still pressed */
			if (work->mouse.hoh != NULL) {
				s_thread_mutex_unlock(window->handlers->mut);
				work->mouse.hoh(window, event, work);
				s_thread_mutex_lock(window->handlers->mut);
			}
			goto end;
		case (EVENT_TYPE_MOUSE_RELEASED | EVENT_TYPE_MOUSE_HINT2):
			/* mouse button released outside, but the prev. press was on us */
			if (work->mouse.rh != NULL) {
				s_thread_mutex_unlock(window->handlers->mut);
				work->mouse.rh(window, event, work);
				s_thread_mutex_lock(window->handlers->mut);
			}
			goto end;
	}
	return -1;
end:	return 0;
}

void s_event_parse_mouse (s_window_t *window, s_event_t *event)
{
        int pos;
	s_handler_t *work;
        pos = 0;
	s_thread_mutex_lock(window->handlers->mut);
	while (!s_list_eol(window->handlers->list, pos)) {
		work = (s_handler_t *) s_list_get(window->handlers->list, pos++);
		if (work->type != MOUSE_HANDLER) {
			continue;
		}
		if (s_event_parse_handler_over(window, event, work) == 0) {
			break;
		}
	}
	pos = 0;
	while (!s_list_eol(window->handlers->list, pos)) {
		work = (s_handler_t *) s_list_get(window->handlers->list, pos++);
		if (work->type != MOUSE_HANDLER) {
			continue;
		}
		if (s_event_parse_handler_notover(window, event, work) == 0) {
			break;
		}
	}
	s_thread_mutex_unlock(window->handlers->mut);
	return;
}

int s_event_parse_keybd_handler (s_window_t *window, s_event_t *event, s_handler_t *work)
{
	if (((work->keybd.flag & event->keybd->flag) == (event->keybd->flag & ~KEYBD_MASL)) &&
	    !(work->keybd.flag & ~event->keybd->flag) &&
	     (event->keybd->button == work->keybd.button)) {
		if (event->type & EVENT_TYPE_KEYBOARD_PRESSED) {
			if (work->keybd.p != NULL) {
				s_thread_mutex_unlock(window->handlers->mut);
				work->keybd.p(window, event, work);
				s_thread_mutex_lock(window->handlers->mut);
			}
			return 0;
		}
		if (event->type & EVENT_TYPE_KEYBOARD_RELEASED) {
			if (work->keybd.r != NULL) {
				s_thread_mutex_unlock(window->handlers->mut);
				work->keybd.r(window, event, work);
				s_thread_mutex_lock(window->handlers->mut);
			}
			return 0;
		}
	}
	return -1;
}

int s_event_parse_keybd (s_window_t *window, s_event_t *event)
{
        int pos;
	int ret = 1;
	s_handler_t *work;

        pos = 0;
	s_thread_mutex_lock(window->handlers->mut);
	while (!s_list_eol(window->handlers->list, pos)) {
		work = (s_handler_t *) s_list_get(window->handlers->list, pos++);
		if (work->type != KEYBD_HANDLER) {
			continue;
		}
		if (s_event_parse_keybd_handler(window, event, work) == 0) {
			ret = 0;
			break;
		}
	}
	s_thread_mutex_unlock(window->handlers->mut);
	return ret;
}

int s_event_parse_expos (s_window_t *window, s_event_t *event)
{
	s_surface_changed(window, event->expose->rect, 0);
	return 0;
}

int s_event_parse_config (s_window_t *window, s_event_t *event)
{
	s_window_form_draw(window);
	return 0;
}

int s_event_parse_timer (s_window_t *window, s_event_t *event)
{
	if (event->timer->cb != NULL) {
		event->timer->cb(window, event->timer);
	}
	return 0;
}

int s_event_copy (s_event_t *event, s_event_t **nevent)
{
	s_event_t *tmp;
	if (s_event_init(&tmp)) {
		return -1;
	}
	tmp->type = event->type;
	tmp->window = event->window;
	memcpy(tmp->mouse, event->mouse, sizeof(s_mouse_t));
	memcpy(tmp->keybd, event->keybd, sizeof(s_keybd_t));
	memcpy(tmp->expose->rect, event->expose->rect, sizeof(s_rect_t));
	*nevent = tmp;
	return 0;
}

int s_event_changed (s_window_t *window)
{
	s_event_t *event;
	if (window->event->type & EVENT_TYPE_MASK) {
		if (!s_event_copy(window->event, &event)) {
			s_eventq_add(window, event);
		}
	}
	return 0;
}

int s_event_dump (s_event_t *event)
{
	s_window_t *window;
	window = event->window;
	if (event->type & EVENT_TYPE_QUIT) {
		debugf(0, "EVENT_TYPE_QUIT");
	}
        if (event->type & EVENT_TYPE_KEYBOARD) {
		debugf(0, "EVENT_TYPE_KEYBOARD");
	}
	if (event->type & EVENT_TYPE_MOUSE) {
		debugf(0, "EVENT_TYPE_MOUSE");
	}
	if (event->type & EVENT_TYPE_EXPOSE) {
		debugf(0,
		       "EVENT_TYPE_EXPOSE:\n"
		       "    expose->rect->x: %d\n"
		       "    expose->rect->y: %d\n"
		       "    expose->rect->w: %d\n"
		       "    expose->rect->h: %d\n",
		       event->expose->rect->x,
		       event->expose->rect->y,
		       event->expose->rect->w,
		       event->expose->rect->h);
	}
	if (event->type & EVENT_TYPE_CONFIG) {
		debugf(0,
		       "EVENT_TYPE_CONFIG:\n"
		       "    win : (%d, %d) (%d x %d)\n"
		       "    buf : (%d, %d) (%d x %d)\n"
		       "    x changed : %s\n"
		       "    y changed : %s\n"
		       "    w changed : %s\n"
		       "    h changed : %s\n",
		       window->surface->win->x,  window->surface->win->y,  window->surface->win->w,  window->surface->win->h,
		       window->surface->buf->x,  window->surface->buf->y,  window->surface->buf->w,  window->surface->buf->h,
		       (event->type & EVENT_TYPE_CONFIG_X) ? "yes" : "no",
		       (event->type & EVENT_TYPE_CONFIG_Y) ? "yes" : "no",
		       (event->type & EVENT_TYPE_CONFIG_W) ? "yes" : "no",
		       (event->type & EVENT_TYPE_CONFIG_H) ? "yes" : "no");
	}
	if (event->type & EVENT_TYPE_FOCUS) {
		debugf(0,
		       "EVENT_TYPE_FOCUS:\n"
		       "    focus type : %s\n",
		       (event->type & EVENT_TYPE_FOCUS_IN) ? "in" :
		       (event->type & EVENT_TYPE_FOCUS_OUT) ? "out" :
		       "unknown");
	}
	if (event->type & EVENT_TYPE_DESKTOP) {
		debugf(0, "EVENT_TYPE_DESKTOP");
	}
	if (event->type & EVENT_TYPE_TIMER) {
		debugf(0, "EVENT_TYPE_TIMER");
	}
	return 0;
}

int s_event_init (s_event_t **event)
{
	(*event) = (s_event_t *) s_calloc(1, sizeof(s_event_t));
	(*event)->mouse = (s_mouse_t *) s_calloc(1, sizeof(s_mouse_t));
	(*event)->keybd = (s_keybd_t *) s_calloc(1, sizeof(s_keybd_t));
	(*event)->expose = (s_expose_t *) s_calloc(1, sizeof(s_expose_t));
	(*event)->expose->rect = (s_rect_t *) s_calloc(1, sizeof(s_rect_t));
	(*event)->desktop = (s_desktop_t *) s_calloc(1, sizeof(s_desktop_t));
	if (s_list_init(&((*event)->desktop->clients))) {
		goto err0;
	}
	return 0;
err0:	s_free((*event)->desktop);
	s_free((*event)->expose);
	s_free((*event)->keybd);
	s_free((*event)->mouse);
	s_free(*event);
	return -1;
}

int s_event_uninit (s_event_t *event)
{
	s_desktop_client_t *desktopc;

	while (!s_list_eol(event->desktop->clients, 0)) {
		desktopc = (s_desktop_client_t *) s_list_get(event->desktop->clients, 0);
		s_list_remove(event->desktop->clients, 0);
		s_free(desktopc->title);
		s_free(desktopc);
	}
	s_list_uninit(event->desktop->clients);
	s_free(event->desktop);
	s_free(event->expose->rect);
	s_free(event->expose);
	s_free(event->mouse);
	s_free(event->keybd);
	s_free(event);
	return 0;
}
