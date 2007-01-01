/***************************************************************************
    begin                : Wed Mar 9 2005
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "xynth_.h"
#include "widget.h"

static int area_hide = 0;
static w_frame_t *area;

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
	int i;
	w_object_t *obj;
	area_hide = (area_hide + 1) % 2;
	if (area_hide) {
		for (i = 0; !s_list_eol(object->parent->childs, i); i++) {
			obj = (w_object_t *) s_list_get(object->parent->childs, i);
			if (obj != object) {
				w_object_hide(obj);
			}
		}
	} else {
		for (i = 0; !s_list_eol(object->parent->childs, i); i++) {
			obj = (w_object_t *) s_list_get(object->parent->childs, i);
			if (obj != object) {
				w_object_show(obj);
			}
		}
	}
}

static void button0_draw (w_object_t *object)
{
	w_object_t *obj;
	w_button_draw(object);
	obj = (w_object_t *) s_list_get(object->childs, 0);
	if (obj == NULL) {
		return;
	}
	if (area_hide) {
		w_textbox_set_str(obj, "show area");
	} else {
		w_textbox_set_str(obj, "hide area");
	}
}

static void button0_destroy (w_object_t *object)
{
	w_button_uninit(object);
}

static void button1_signal_func (w_signal_t *signal)
{
	printf("button1_signal_func\n");
}

static void button1_pressed (w_object_t *object, int button)
{
	int i;
	int w;
	int h;
	w_object_t *obj;
	w = object->window->window->surface->buf->w;
	h = object->window->window->surface->buf->h;
	for (i = 0; !s_list_eol(area->object->childs, i); i++) {
		obj = (w_object_t *) s_list_get(area->object->childs, i);
		w_object_move(obj, (rand() + 1) % (w - 10), (rand() + 1) % (h - 100),
		                   (rand() + 1) % (w - 10), (rand() + 1) % (h - 100));
	}
	w_signal_send(object, object, button1_signal_func, NULL);
}

static void button1_destroy (w_object_t *object)
{
	w_button_uninit(object);
}

static void button2_pressed (w_object_t *object, int button)
{
	int r;
	int show;
	w_object_t *obj;
	r = rand();
	if (r == 0) {
		r++;
	}
	show = r % area->object->childs->nb_elt;
	obj = (w_object_t *) s_list_get(area->object->childs, show);
	w_object_show(obj);
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
	int w = 500;
	int h = 400;
	w_window_t *window;
	w_frame_t *frame;
	w_frame_t *square;
	w_button_t *button;
	w_textbox_t *textbox;
	w_checkbox_t *checkbox;

	srand(time(NULL));
	
	w_window_init(&window, WINDOW_MAIN, NULL);

	w = MIN(window->window->surface->width, w);
	h = MIN(window->window->surface->height, h);
	
	s_window_set_title(window->window, "Demo - %s", argv[0]);
	w_window_set_coor(window, x, y, w, h);

	w_frame_init(window, &frame, FRAME_PANEL | FRAME_RAISED, window->object);
	w_object_move(frame->object, 0, 0, w, h);
	w_object_show(frame->object);
	
	w_button_init(window, &button, frame->object);
	button->pressed = button0_pressed;
	button->object->draw = button0_draw;
	button->object->destroy = button0_destroy;
	w_object_move(button->object, 5, 5, 70, 20);
	w_object_show(button->object);

	w_textbox_init(window, &textbox, button->object);
	textbox->frame->style = FRAME_NOFRAME;
	w_textbox_set_str(textbox->object, "hide area");
	w_object_move(textbox->object, 0, 0, 70, 20);
	w_object_show(textbox->object);

	w_button_init(window, &button, frame->object);
	button->object->effect->effect = EFFECT_POPIN | EFFECT_POPOUT;
	button->pressed = button1_pressed;
	button->object->destroy = button1_destroy;
	w_object_move(button->object, 80, 5, 55, 20);
	w_object_show(button->object);

	w_textbox_init(window, &textbox, button->object);
	textbox->frame->style = FRAME_NOFRAME;
	w_textbox_set_str(textbox->object, "blender");
	w_object_move(textbox->object, 0, 0, 55, 20);
	w_object_show(textbox->object);

	w_button_init(window, &button, frame->object);
	button->object->effect->effect = EFFECT_FADEIN | EFFECT_FADEOUT;
	button->pressed = button2_pressed;
	button->object->destroy = button2_destroy;
	w_object_move(button->object, 140, 5, 85, 20);
	w_object_show(button->object);

	w_textbox_init(window, &textbox, button->object);
	textbox->frame->style = FRAME_NOFRAME;
	w_textbox_set_str(textbox->object, "random focus");
	w_object_move(textbox->object, 0, 0, 85, 20);
	w_object_show(textbox->object);

	w_checkbox_init(window, &checkbox, frame->object);
	w_textbox_set_str(checkbox->text->object, "checkbox");
	w_object_move(checkbox->object, 300, 5, 85, 20);
	w_object_show(checkbox->object);
	
	w_frame_init(window, &area, FRAME_PANEL | FRAME_SUNKEN, frame->object);
	area->object->effect->effect = EFFECT_FADEIN;
	w_object_move(area->object, 5, 60, w - 10, h - 130);
	w_object_show(area->object);

	for (i = 0; i < 4; i++) {
		w_frame_init(window, &square, FRAME_NOFRAME | FRAME_PLAIN, area->object);
		square->object->draw = object_draw_p[i];
		w_object_move(square->object, (rand() + 1) % (w - 10), ((rand() + 1) % (h - 70)) + 1,
		                              (rand() + 1) % (w - 10), ((rand() + 1) % (h - 70)) + 1);
		w_object_show(square->object);
	}

	w_textbox_init(window, &textbox, frame->object);
	textbox->frame->style = FRAME_PANEL | FRAME_PLAIN;
	w_textbox_set_str(textbox->object, "this is a text box !");
	w_object_move(textbox->object, 5, 30 + (h - 100) + 5, w - 10, 100 - 30 - 5 - 30);
	w_object_show(textbox->object);
	{
		w_editbox_t *editbox;
		w_editbox_init(window, &editbox, frame->object);
		w_object_move(editbox->object, 5, 30 + (h - 60) + 5, w - 10, 20);
		w_textbox_set_str(editbox->object, "this is an editable box !");
		w_object_show(editbox->object);
	}

	w_frame_init(window, &frame, FRAME_NOFRAME, frame->object);
	w_object_move(frame->object, 5, 30, w - 10, 25);
	w_object_show(frame->object);

	w_button_init(window, &button, frame->object);
	w_object_move(button->object, 5, 2, 70, 20);
	w_object_show(button->object);

	w_textbox_init(window, &textbox, button->object);
	textbox->frame->style = FRAME_NOFRAME;
	w_textbox_set_str(textbox->object, "test 0");
	w_object_move(textbox->object, 0, 0, 70, 20);
	w_object_show(textbox->object);

	w_button_init(window, &button, frame->object);
	w_object_move(button->object, 80, 2, 55, 20);
	w_object_show(button->object);

	w_textbox_init(window, &textbox, button->object);
	textbox->frame->style = FRAME_NOFRAME;
	w_textbox_set_str(textbox->object, "test 1");
	w_object_move(textbox->object, 0, 0, 55, 20);
	w_object_show(textbox->object);

	w_button_init(window, &button, frame->object);
	w_object_move(button->object, 140, 2, 85, 20);
	w_object_show(button->object);

	w_textbox_init(window, &textbox, button->object);
	textbox->frame->style = FRAME_NOFRAME;
	w_textbox_set_str(textbox->object, "test 2");
	w_object_move(textbox->object, 0, 0, 85, 20);
	w_object_show(textbox->object);

	w_checkbox_init(window, &checkbox, frame->object);
	w_textbox_set_str(checkbox->text->object, "test 3");
	w_object_move(checkbox->object, 300, 2, 85, 20);
	w_object_show(checkbox->object);
	
	w_object_show(window->object);

	s_window_show(window->window);
	s_window_main(window->window);

	return 0;
}

#if defined(SINGLE_APP)
s_single_app_t single_object = {
	object_main,
	1,
	{"object"}
};
#endif
