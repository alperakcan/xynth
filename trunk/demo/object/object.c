/***************************************************************************
    begin                : Wed Mar 9 2005
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "xynth_.h"

static void object_atevent (s_window_t *window, s_event_t *event)
{
	s_object_t *root;
	s_object_t *objectn;
	s_object_t *objectp;
	root = (s_object_t *) window->client->user_data;
	if (event->type & MOUSE_EVENT) {
		event->mouse->x -= window->surface->buf->x;
		event->mouse->y -= window->surface->buf->y;
		event->mouse->px -= window->surface->buf->x;
		event->mouse->py -= window->surface->buf->y;
		s_object_atposition(root, event->mouse->x, event->mouse->y, &objectn);
		s_object_atposition(root, event->mouse->px, event->mouse->py, &objectp);
		if ((objectn != objectp) && (objectp->event)) {
			objectp->event(objectp, event);
		}
		if (objectn->event) {
			objectn->event(objectn, event);
		}
	}
}

int main (int argc, char *argv[])
{
	int x = 60;
	int y = 60;
	int w = 400;
	int h = 300;
	int mw = 1000;
	int mh = 1000;
	s_window_t *window;
	s_object_t *root;
	s_frame_t *frame;
	s_button_t *button[2];

	srand(time(NULL));

	s_client_init(&window);

	s_window_new(window, WINDOW_MAIN, NULL);
	mw = window->surface->width;
	mh = window->surface->height;
	w = MIN(mw, w);
	h = MIN(mh, h);

	s_window_set_title(window, "Demo - %s", argv[0]);
	s_window_set_coor(window, WINDOW_NOFORM, x, y, w, h);

	s_object_init(window, &root, w, h, NULL, NULL);
	s_client_atevent(window, object_atevent);
	window->client->user_data = root;
		
	s_frame_init(window, &frame, w, h, FRAME_PANEL | FRAME_RAISED, root);
	s_object_move(frame->object, 0, 0, w, h);
	s_button_init(window, &button[0], 20, 20, frame->object);
	s_object_move(button[0]->frame->object, 5, 5, 20, 20);
	s_button_init(window, &button[1], 20, 20, frame->object);
	s_object_move(button[1]->frame->object, 30, 5, 20, 20);

	s_object_show(root);

	s_client_atevent(window, object_atevent);

	s_window_show(window);
	s_client_main(window);

	return 0;
}

#if defined(SINGLE_APP)
s_single_app_t single_object = {
	object_main,
	1,
	{"object"}
};
#endif
