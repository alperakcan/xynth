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

static int area_hide = 0;
static s_frame_t *area;
static s_button_t *button[3];
static s_font_t *font[3];
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

static void button0_pressed (s_object_t *object, int buttonp)
{
	area_hide = (area_hide + 1) % 2;
	if (area_hide) {
		s_object_hide(area->object);
		s_object_hide(button[1]->frame->object);
		s_object_hide(button[2]->frame->object);
	} else {
		s_object_show(area->object);
		s_object_show(button[1]->frame->object);
		s_object_show(button[2]->frame->object);
	}
}

static void button0_draw (s_object_t *object)
{
	int x;
	int y;
	if (area_hide) {
		s_font_set_str(font[0], "show area");
	} else {
		s_font_set_str(font[0], "hide area");
	}
	s_font_get_glyph(font[0]);
	x = MAX((object->content->w - font[0]->img->w) / 2, 0);
	y = MAX((object->content->h - font[0]->img->h) / 2, 0);
	s_button_draw(object);
	s_putboxrgba(object->surface, x, y, font[0]->img->w, font[0]->img->h, font[0]->img->rgba);
}

static void button0_destroy (s_object_t *object)
{
	s_button_uninit(object);
	s_font_uninit(font[0]);
}

static void button1_pressed (s_object_t *object, int button)
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

static void button1_draw (s_object_t *object)
{
	int x;
	int y;
	s_font_set_str(font[1], "blender");
	s_font_get_glyph(font[1]);
	x = MAX((object->content->w - font[1]->img->w) / 2, 0);
	y = MAX((object->content->h - font[1]->img->h) / 2, 0);
	s_button_draw(object);
	s_putboxrgba(object->surface, x, y, font[1]->img->w, font[1]->img->h, font[1]->img->rgba);
}

static void button1_destroy (s_object_t *object)
{
	s_button_uninit(object);
	s_font_uninit(font[1]);
}

static void button2_pressed (s_object_t *object, int button)
{
	int r;
	int show;
	r = rand();
	if (r) r++;
	show = r % 4;
	s_object_show(big[show]->object);
}

static void button2_draw (s_object_t *object)
{
	int x;
	int y;
	s_font_set_str(font[2], "random focus");
	s_font_get_glyph(font[2]);
	x = MAX((object->content->w - font[2]->img->w) / 2, 0);
	y = MAX((object->content->h - font[2]->img->h) / 2, 0);
	s_button_draw(object);
	s_putboxrgba(object->surface, x, y, font[2]->img->w, font[2]->img->h, font[2]->img->rgba);
}

static void button2_destroy (s_object_t *object)
{
	s_button_uninit(object);
	s_font_uninit(font[2]);
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

	s_object_init(window, &root, NULL, NULL);
	s_object_move(root, 0, 0, w, h);
	s_client_atevent(window, object_atevent);
	window->client->user_data = root;

	s_frame_init(window, &frame, FRAME_PANEL | FRAME_RAISED, root);
	s_object_move(frame->object, 0, 0, w, h);
	s_object_show(frame->object);

	s_button_init(window, &button[0], frame->object);
	button[0]->pressed = button0_pressed;
	button[0]->frame->object->draw = button0_draw;
	button[0]->frame->object->destroy = button0_destroy;
	s_font_init(&font[0], "arial.ttf");
	s_font_set_size(font[0], 10);
	s_object_move(button[0]->frame->object, 5, 5, 70, 20);
	s_object_show(button[0]->frame->object);

	s_button_init(window, &button[1], frame->object);
	button[1]->pressed = button1_pressed;
	button[1]->frame->object->draw = button1_draw;
	button[1]->frame->object->destroy = button1_destroy;
	s_font_init(&font[1], "arial.ttf");
	s_font_set_size(font[1], 10);
	s_object_move(button[1]->frame->object, 80, 5, 55, 20);
	s_object_show(button[1]->frame->object);

	s_button_init(window, &button[2], frame->object);
	button[2]->pressed = button2_pressed;
	button[2]->frame->object->draw = button2_draw;
	button[2]->frame->object->destroy = button2_destroy;
	s_font_init(&font[2], "arial.ttf");
	s_font_set_size(font[2], 10);
	s_object_move(button[2]->frame->object, 140, 5, 85, 20);
	s_object_show(button[2]->frame->object);

	s_frame_init(window, &area, FRAME_NOFRAME | FRAME_PLAIN, frame->object);
	s_object_move(area->object, 5, 30, w - 10, h - 35);
	s_object_show(area->object);

	for (i = 0; i < 4; i++) {
		s_frame_init(window, &big[i], FRAME_NOFRAME | FRAME_PLAIN, area->object);
		big[i]->object->draw = object_draw_p[i];
		s_object_move(big[i]->object, (rand() + 1) % (w - 10), ((rand() + 1) % (h - 35)) + 1,
		                              (rand() + 1) % (w - 10), ((rand() + 1) % (h - 35)) + 1);
		s_object_show(big[i]->object);
	}
	s_object_show(root);

	s_client_atevent(window, object_atevent);

	s_window_show(window);
	s_client_main(window);

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
