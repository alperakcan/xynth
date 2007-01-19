/***************************************************************************
    begin                : Thu Feb 20 2003
    copyright            : (C) 2003 - 2007 by Alper Akcan
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

int s_event_mouse_handler_state (s_window_t *window, s_event_t *event, s_handler_mouse_t *mouse, int over)
{
        int x = mouse->x + window->surface->buf->x;
        int y = mouse->y + window->surface->buf->y;
        int w = mouse->w;
        int h = mouse->h;
        int b = mouse->button;
	
	int overbit = MOUSE_OVER << 16;
        int type = (event->type & MOUSE_CLICKED) ? event->type &= ~MOUSE_RELEASED : event->type;
        
	if ((type & (MOUSE_PRESSED | MOUSE_CLICKED | MOUSE_RELEASED)) &&
	    !(event->mouse->b & b)) {
		return -1;
	}
	if ((event->mouse->x >= x) && (event->mouse->x <= (x + w - 1)) &&
	    (event->mouse->y >= y) && (event->mouse->y <= (y + h - 1))) {
		if (over != 1) {
			goto end;
		}
		mouse->hstate |= overbit;
		if (type & (MOUSE_RELEASED | MOUSE_CLICKED)) {
			if (mouse->hstate & event->mouse->b) {
				mouse->hstate &= ~event->mouse->b;
				return (type & ~MOUSE_EVENT);
			} else {
				return (type & ~MOUSE_EVENT) | MOUSE_HINT;
			}
		}
		mouse->hstate |= event->mouse->b;
		return (type & ~MOUSE_EVENT);
	} else {
		if (over != 0) {
			goto end;
		}
		if (type & (MOUSE_RELEASED | MOUSE_CLICKED)) {
			if (mouse->hstate & event->mouse->b) {
				mouse->hstate &= ~event->mouse->b;
				return (type & ~MOUSE_EVENT) | MOUSE_HINT2;
			}
		}
		if (mouse->hstate & overbit) {
			mouse->hstate &= ~overbit;
			return (type & ~MOUSE_EVENT) | MOUSE_HINT2;
		}
	}
end:	return -1;
}

int s_event_parse_handler_over (s_window_t *window, s_event_t *event, s_handler_t *work)
{
	switch (s_event_mouse_handler_state(window, event, &(work->mouse), 1)) {
		case MOUSE_OVER:
			if (work->mouse.o != NULL) {
				s_thread_mutex_unlock(window->handlers->mut);
				work->mouse.o(window, event, work);
				s_thread_mutex_lock(window->handlers->mut);
			}
			goto not_over;
		case MOUSE_PRESSED:
			if (work->mouse.p != NULL) {
				s_thread_mutex_unlock(window->handlers->mut);
				work->mouse.p(window, event, work);
				s_thread_mutex_lock(window->handlers->mut);
			}
			goto not_over;
		case MOUSE_CLICKED:
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
		case MOUSE_RELEASED:
			if (work->mouse.r != NULL) {
				s_thread_mutex_unlock(window->handlers->mut);
				work->mouse.r(window, event, work);
				s_thread_mutex_lock(window->handlers->mut);
			}
			goto not_over;
		case (MOUSE_RELEASED | MOUSE_HINT):
			/* mouse button released, but the prev. press was not on us */
			if (work->mouse.hr != NULL) {
				s_thread_mutex_unlock(window->handlers->mut);
				work->mouse.hr(window, event, work);
				s_thread_mutex_lock(window->handlers->mut);
			}
			goto not_over;
		case (MOUSE_OVER | MOUSE_HINT):
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
		case (MOUSE_OVER | MOUSE_HINT2):
			/* not on over, but was on over */
			if (work->mouse.oh != NULL) {
				s_thread_mutex_unlock(window->handlers->mut);
				work->mouse.oh(window, event, work);
				s_thread_mutex_lock(window->handlers->mut);
			}
			goto end;
		case (MOUSE_OVER | MOUSE_HINT | MOUSE_HINT2):
			/* not on over, but was on over. and button is still pressed */
			if (work->mouse.hoh != NULL) {
				s_thread_mutex_unlock(window->handlers->mut);
				work->mouse.hoh(window, event, work);
				s_thread_mutex_lock(window->handlers->mut);
			}
			goto end;
		case (MOUSE_RELEASED | MOUSE_HINT2):
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
		if (event->type & KEYBD_PRESSED) {
			if (work->keybd.p != NULL) {
				s_thread_mutex_unlock(window->handlers->mut);
				work->keybd.p(window, event, work);
				s_thread_mutex_lock(window->handlers->mut);
			}
			return 0;
		}
		if (event->type & KEYBD_RELEASED) {
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
	s_surface_changed(window, event->expose->rect);
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
	memcpy(tmp->mouse, event->mouse, sizeof(s_mouse_t));
	memcpy(tmp->keybd, event->keybd, sizeof(s_keybd_t));
	memcpy(tmp->expose->rect, event->expose->rect, sizeof(s_rect_t));
	*nevent = tmp;
	return 0;
}

int s_event_changed (s_window_t *window)
{
	s_event_t *event;
	if (window->event->type & EVENT_MASK) {
		if (!s_event_copy(window->event, &event)) {
			s_eventq_add(window, event);
		}
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
