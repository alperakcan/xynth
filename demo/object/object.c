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
#include "widget.h"

static int area_hide = 0;
static w_frame_t *area;
static w_button_t *button[3];
static w_textbox_t *textbox[3];
static w_frame_t *big[4];

static void object_draw_black (w_object_t *object)
{
	s_fillbox(object->surface, 0, 0, object->surface->width, object->surface->height, s_rgbcolor(object->surface, 0, 0, 0));
}

static void object_draw_red (w_object_t *object)
{
	s_fillbox(object->surface, 0, 0, object->surface->width, object->surface->height, s_rgbcolor(object->surface, 255, 0, 0));
}

static void object_draw_green (w_object_t *object)
{
	s_fillbox(object->surface, 0, 0, object->surface->width, object->surface->height, s_rgbcolor(object->surface, 0, 255, 0));
}

static void object_draw_blue (w_object_t *object)
{
	s_fillbox(object->surface, 0, 0, object->surface->width, object->surface->height, s_rgbcolor(object->surface, 0, 0, 255));
}

static void object_draw_white (w_object_t *object)
{
	s_fillbox(object->surface, 0, 0, object->surface->width, object->surface->height, s_rgbcolor(object->surface, 255, 255, 255));
}

typedef void (*object_draw) (w_object_t *);
static object_draw object_draw_p[] = {
	&object_draw_red,
	&object_draw_green,
	&object_draw_blue,
	&object_draw_white,
	&object_draw_black
};

static void button0_pressed (w_object_t *object, int buttonp)
{
	area_hide = (area_hide + 1) % 2;
	if (area_hide) {
		w_object_hide(area->object);
		w_object_hide(button[1]->frame->object);
		w_object_hide(button[2]->frame->object);
	} else {
		w_object_show(area->object);
		w_object_show(button[1]->frame->object);
		w_object_show(button[2]->frame->object);
	}
}

static void button0_draw (w_object_t *object)
{
	if (area_hide) {
		w_textbox_set_str(textbox[0]->frame->object, "show area");
	} else {
		w_textbox_set_str(textbox[0]->frame->object, "hide area");
	}
	w_button_draw(object);
}

static void button0_destroy (w_object_t *object)
{
	w_button_uninit(object);
}

static void button1_pressed (w_object_t *object, int button)
{
	int i;
	int w;
	int h;
	w = object->window->surface->buf->w;
	h = object->window->surface->buf->h;
	for (i = 0; i < 4; i++) {
		w_object_move(big[i]->object, (rand() + 1) % (w - 10), (rand() + 1) % (h - 40),
		                              (rand() + 1) % (w - 10), (rand() + 1) % (h - 40));
	}
}

static void button1_destroy (w_object_t *object)
{
	w_button_uninit(object);
}

static void button2_pressed (w_object_t *object, int button)
{
	int r;
	int show;
	r = rand();
	if (r) r++;
	show = r % 4;
	w_object_show(big[show]->object);
}

static void button2_destroy (w_object_t *object)
{
	w_button_uninit(object);
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
	w_window_t *window;
	w_frame_t *frame;

	srand(time(NULL));
	
	w_window_init(&window, WINDOW_MAIN, NULL);

	mw = window->window->surface->width;
	mh = window->window->surface->height;
	w = MIN(mw, w);
	h = MIN(mh, h);
	
	s_window_set_title(window->window, "Demo - %s", argv[0]);
	w_window_set_coor(window, x, y, w, h);

	w_frame_init(window->window, &frame, FRAME_PANEL | FRAME_RAISED, window->object);
	w_object_move(frame->object, 0, 0, w, h);
	w_object_show(frame->object);

	w_button_init(window->window, &button[0], frame->object);
	button[0]->pressed = button0_pressed;
	button[0]->frame->object->destroy = button0_destroy;
	w_object_move(button[0]->frame->object, 5, 5, 70, 20);
	w_object_show(button[0]->frame->object);

	w_textbox_init(window->window, &textbox[0], button[0]->frame->object);
	textbox[0]->frame->style = FRAME_NOFRAME;
	w_textbox_set_str(textbox[0]->frame->object, "hide area");
	button[0]->frame->object->draw = button0_draw;
	w_object_move(textbox[0]->frame->object, 0, 0, 70, 20);
	w_object_show(textbox[0]->frame->object);

	w_button_init(window->window, &button[1], frame->object);
	button[1]->pressed = button1_pressed;
	button[1]->frame->object->destroy = button1_destroy;
	w_object_move(button[1]->frame->object, 80, 5, 55, 20);
	w_object_show(button[1]->frame->object);

	w_textbox_init(window->window, &textbox[1], button[1]->frame->object);
	textbox[1]->frame->style = FRAME_NOFRAME;
	w_textbox_set_str(textbox[1]->frame->object, "blender");
	w_object_move(textbox[1]->frame->object, 0, 0, 55, 20);
	w_object_show(textbox[1]->frame->object);

	w_button_init(window->window, &button[2], frame->object);
	button[2]->pressed = button2_pressed;
	button[2]->frame->object->destroy = button2_destroy;
	w_object_move(button[2]->frame->object, 140, 5, 85, 20);
	w_object_show(button[2]->frame->object);

	w_textbox_init(window->window, &textbox[2], button[2]->frame->object);
	textbox[2]->frame->style = FRAME_NOFRAME;
	w_textbox_set_str(textbox[2]->frame->object, "random focus");
	w_object_move(textbox[2]->frame->object, 0, 0, 85, 20);
	w_object_show(textbox[2]->frame->object);

	w_frame_init(window->window, &area, FRAME_NOFRAME | FRAME_PLAIN, frame->object);
	w_object_move(area->object, 5, 30, w - 10, h - 35);
	w_object_show(area->object);

	for (i = 0; i < 4; i++) {
		w_frame_init(window->window, &big[i], FRAME_NOFRAME | FRAME_PLAIN, area->object);
		big[i]->object->draw = object_draw_p[i];
		w_object_move(big[i]->object, (rand() + 1) % (w - 10), ((rand() + 1) % (h - 35)) + 1,
		                              (rand() + 1) % (w - 10), ((rand() + 1) % (h - 35)) + 1);
		w_object_show(big[i]->object);
	}

	w_object_show(window->object);

	s_window_show(window->window);
	s_client_main(window->window);

	w_window_uninit(window);

	return 0;
}

#if defined(SINGLE_APP)
s_single_app_t single_object = {
	object_main,
	1,
	{"object"}
};
#endif
