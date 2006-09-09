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

static s_frame_t *big[4];

static void object_draw_black (s_object_t *object)
{
	s_fillbox(object->surface, 0, 0, object->surface->width, object->surface->height, s_rgbcolor(object->surface, 0, 0, 0));
}
static void object_draw_red (s_object_t *object)
{
	s_fillbox(object->surface, 0, 0, object->surface->width, object->surface->height, s_rgbcolor(object->surface, 255, 0, 0));
}
static void object_draw_green (s_object_t *object)
{
	s_fillbox(object->surface, 0, 0, object->surface->width, object->surface->height, s_rgbcolor(object->surface, 0, 255, 0));
}
static void object_draw_blue (s_object_t *object)
{
	s_fillbox(object->surface, 0, 0, object->surface->width, object->surface->height, s_rgbcolor(object->surface, 0, 0, 255));
}
static void object_draw_white (s_object_t *object)
{
	s_fillbox(object->surface, 0, 0, object->surface->width, object->surface->height, s_rgbcolor(object->surface, 255, 255, 255));
}
typedef void (*object_draw) (s_object_t *);
object_draw object_draw_p[] = {
	&object_draw_red,
	&object_draw_green,
	&object_draw_blue,
	&object_draw_white,
	&object_draw_black
};

static void button0_pressed (s_object_t *object, int button)
{
	int i;
	int w;
	int h;
	w = object->window->surface->buf->w;
	h = object->window->surface->buf->h;
	for (i = 0; i < 4; i++) {
		s_object_move(big[i]->object, (rand() + 1) % (w - 10), (rand() + 1) % (h - 40),
		                              (rand() + 1) % (w - 10), (rand() + 1) % (h - 40));
	}
}
static void button1_pressed (s_object_t *object, int button)
{
}

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
	int i;
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
	s_frame_t *area;
	
	srand(time(NULL));

	s_client_init(&window);

	s_window_new(window, WINDOW_MAIN, NULL);
	mw = window->surface->width;
	mh = window->surface->height;
	w = MIN(mw, w);
	h = MIN(mh, h);

	s_window_set_title(window, "Demo - %s", argv[0]);
	s_window_set_coor(window, WINDOW_NOFORM, x, y, w, h);
	s_window_set_resizeable(window, 0);

	s_object_init(window, &root, w, h, NULL, NULL);
	s_client_atevent(window, object_atevent);
	window->client->user_data = root;
		
	s_frame_init(window, &frame, w, h, FRAME_PANEL | FRAME_RAISED, root);
	s_object_move(frame->object, 0, 0, w, h);

	s_button_init(window, &button[0], 20, 20, frame->object);
	button[0]->pressed = button0_pressed;
	s_object_move(button[0]->frame->object, 5, 5, 20, 20);

	s_button_init(window, &button[1], 20, 20, frame->object);
	button[1]->pressed = button1_pressed;
	s_object_move(button[1]->frame->object, 30, 5, 20, 20);
	
	s_frame_init(window, &area, w - 10, h - 40, FRAME_NOFRAME | FRAME_PLAIN, root);
	s_object_move(area->object, 5, 35, w - 10, h - 40);
	
	for (i = 0; i < 4; i++) {
		s_frame_init(window, &big[i], 100, 100, FRAME_NOFRAME | FRAME_PLAIN, area->object);
		big[i]->object->draw = object_draw_p[i];
		s_object_move(big[i]->object, (rand() + 1) % (w - 10), (rand() + 1) % (h - 40),
		                              (rand() + 1) % (w - 10), (rand() + 1) % (h - 40));
	}

	s_object_show(root);

	s_client_atevent(window, object_atevent);

	s_window_show(window);
	s_client_main(window);
	
	s_button_uninit(button[0]);
	s_button_uninit(button[1]);
	s_frame_uninit(frame);
	s_object_uninit(root);

	return 0;
}

#if defined(SINGLE_APP)
s_single_app_t single_object = {
	object_main,
	1,
	{"object"}
};
#endif
