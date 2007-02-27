/***************************************************************************
    begin                : Tue Nov 28 2006
    copyright            : (C) 2006 - 2007 by Alper Akcan
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

void w_scrollbufferbar_add_pressed (w_object_t *object, int button)
{
	w_scrollbuffer_slide(object->parent->parent, -10, 0);
}

void w_scrollbufferbar_sub_pressed (w_object_t *object, int button)
{
	w_scrollbuffer_slide(object->parent->parent, 10, 0);
}

void w_scrollbufferbar_geometry (w_object_t *object)
{
	w_scrollbufferbar_t *sbb;
	sbb = object->data[OBJECT_SCROLLBUFFERBAR];
	w_frame_geometry(object);
	w_object_move(sbb->add->object, object->content->x, object->content->y, object->content->w, object->content->w);
	w_object_move(sbb->sub->object, object->content->x, object->content->y + object->content->h - object->content->w, object->content->w, object->content->w);
}

void w_scrollbufferbar_uninit (w_object_t *object)
{
	w_scrollbufferbar_t *sbb;
	sbb = object->data[OBJECT_SCROLLBUFFERBAR];
	w_frame_uninit(sbb->box->object);
	w_button_uninit(sbb->add->object);
	w_button_uninit(sbb->sub->object);
	w_frame_uninit(object);
	s_free(sbb);
}

int w_scrollbufferbar_init (w_window_t *window, w_scrollbufferbar_t **scrollbufferbar, w_object_t *parent)
{
	w_scrollbufferbar_t *sbb;
	sbb = (w_scrollbufferbar_t *) s_malloc(sizeof(w_scrollbufferbar_t));
	(*scrollbufferbar) = sbb;
	w_frame_init(window, &(sbb->frame), FRAME_EMPTY, parent);
	sbb->object = sbb->frame->object;
	sbb->object->type = OBJECT_SCROLLBUFFERBAR;
	sbb->object->data[OBJECT_SCROLLBUFFERBAR] = sbb;
	sbb->object->geometry = w_scrollbufferbar_geometry;
	sbb->object->destroy = w_scrollbufferbar_uninit;
	w_button_init(window, &(sbb->add), sbb->object);
	w_button_init(window, &(sbb->sub), sbb->object);
	w_button_set_pressed(sbb->add->object, w_scrollbufferbar_add_pressed);
	w_button_set_pressed(sbb->sub->object, w_scrollbufferbar_sub_pressed);
	w_frame_init(window, &(sbb->box), FRAME_PANEL | FRAME_RAISED, sbb->object);
	w_object_show(sbb->add->object);
	w_object_show(sbb->sub->object);
	w_object_show(sbb->box->object);
	return 0;
}

int w_scrollbuffer_set_border (w_object_t *object, int border)
{
	return w_frame_set_border(object, border);
}

int w_scrollbuffer_set_style (w_object_t *object, FRAME_SHAPE shape, FRAME_SHADOW shadow)
{
	w_frame_set_style(object, shape, shadow);
	return 0;
}

int w_scrollbuffer_set_image (w_object_t *object, unsigned int style, unsigned int rotation, unsigned int nimgs, char **imgs)
{
	return w_frame_set_image(object, style, rotation, nimgs, imgs);
}

int w_scrollbuffer_set_boxstyle (w_object_t *object, FRAME_SHAPE shape, FRAME_SHADOW shadow)
{
	w_scrollbuffer_t *sb;
	sb = object->data[OBJECT_SCROLLBUFFER];
	w_frame_set_style(sb->vertical->box->object, shape, shadow);
	w_object_draw(sb->vertical->box->object);
	return 0;
}

int w_scrollbuffer_set_boximage (w_object_t *object, unsigned int style, unsigned int rotation, unsigned int nimgs, char **imgs)
{
	w_scrollbuffer_t *sb;
	sb = object->data[OBJECT_SCROLLBUFFER];
	return w_frame_set_image(sb->vertical->box->object, style, rotation, nimgs, imgs);
}

int w_scrollbuffer_set_addstyle (w_object_t *object, FRAME_SHAPE shape, FRAME_SHADOW shadow)
{
	w_scrollbuffer_t *sb;
	sb = object->data[OBJECT_SCROLLBUFFER];
	w_frame_set_style(sb->vertical->add->object, shape, shadow);
	w_object_draw(sb->vertical->add->object);
	return 0;
}

int w_scrollbuffer_set_addimage (w_object_t *object, unsigned int style, unsigned int rotation, unsigned int nimgs, char **imgs)
{
	w_scrollbuffer_t *sb;
	sb = object->data[OBJECT_SCROLLBUFFER];
	return w_frame_set_image(sb->vertical->add->object, style, rotation, nimgs, imgs);
}

int w_scrollbuffer_set_substyle (w_object_t *object, FRAME_SHAPE shape, FRAME_SHADOW shadow)
{
	w_scrollbuffer_t *sb;
	sb = object->data[OBJECT_SCROLLBUFFER];
	w_frame_set_style(sb->vertical->sub->object, shape, shadow);
	w_object_draw(sb->vertical->sub->object);
	return 0;
}

int w_scrollbuffer_set_subimage (w_object_t *object, unsigned int style, unsigned int rotation, unsigned int nimgs, char **imgs)
{
	w_scrollbuffer_t *sb;
	sb = object->data[OBJECT_SCROLLBUFFER];
	return w_frame_set_image(sb->vertical->sub->object, style, rotation, nimgs, imgs);
}

void w_scrollbuffer_slide (w_object_t *object, int vertical, int horizontal)
{
	int x;
	int y;
	int w;
	int h;
	int ytotal;
	int yoffset;
	w_scrollbuffer_t *sb;
	sb = object->data[OBJECT_SCROLLBUFFER];
	sb->ytotal = 0;
	sb->yoffset = 0;
	if (sb->slide &&
	    sb->child) {
		sb->slide(sb->child, vertical, horizontal, &ytotal, &yoffset);
		if (ytotal <= 0) {
			return;
		}
		x = sb->vertical->object->content->x + 4;
		y = sb->vertical->object->content->y + 23;
		w = sb->vertical->object->content->w - 7;
		if (ytotal < sb->child->content->h) {
			h = sb->vertical->object->content->h - 49;
		} else {
			h = (sb->child->content->h * (sb->vertical->object->content->h - 46)) / ytotal;
		}
		y -= (yoffset * (sb->vertical->object->content->h - 49)) / ytotal;
		if ((ytotal + yoffset - sb->child->content->h) == 0) {
			y = sb->vertical->object->content->h - 26 - h;
		}
		sb->ytotal = ytotal;
		sb->yoffset = yoffset;
		w_object_move(sb->vertical->box->object, x, y, w, h);
	}
}

void w_scrollbuffer_set_child (w_object_t *object, w_object_t *child)
{
	w_scrollbuffer_t *sb;
	sb = object->data[OBJECT_SCROLLBUFFER];
	sb->child = child;
}

void w_scrollbuffer_set_slide (w_object_t *object, void (*slide) (w_object_t *, int, int, int *, int *))
{
	w_scrollbuffer_t *sb;
	sb = object->data[OBJECT_SCROLLBUFFER];
	sb->slide = slide;
}

void w_scrollbuffer_geometry (w_object_t *object)
{
	w_scrollbuffer_t *sb;
	sb = object->data[OBJECT_SCROLLBUFFER];
	w_frame_geometry(object);
	w_object_move(sb->vertical->object, object->content->x + object->content->w - 24,
	                                    object->content->y, 24, object->content->h);
	if (sb->child) {
		w_object_move(sb->child, object->content->x, object->content->y, object->content->w - 24, object->content->h);
	}
	w_scrollbuffer_slide(object, 0, 0);
}

void w_scrollbuffer_uninit (w_object_t *object)
{
	w_scrollbuffer_t *sb;
	sb = object->data[OBJECT_SCROLLBUFFER];
	w_scrollbufferbar_uninit(sb->vertical->object);
	w_frame_uninit(object);
	s_free(sb);
}

int w_scrollbuffer_init (w_window_t *window, w_scrollbuffer_t **scrollbuffer, w_object_t *parent)
{
	w_scrollbuffer_t *sb;
	sb = (w_scrollbuffer_t *) s_malloc(sizeof(w_scrollbuffer_t));
	(*scrollbuffer) = sb;
	w_frame_init(window, &(sb->frame), FRAME_EMPTY, parent);
	sb->child = NULL;
	sb->slide = NULL;
	sb->ytotal = 0;
	sb->yoffset = 0;
	sb->object = sb->frame->object;
	sb->object->type = OBJECT_SCROLLBUFFER;
	sb->object->geometry = w_scrollbuffer_geometry;
	sb->object->destroy = w_scrollbuffer_uninit;
	sb->object->data[OBJECT_SCROLLBUFFER] = sb;
	w_scrollbufferbar_init(window, &(sb->vertical), sb->object);
	w_object_show(sb->vertical->object);
	return 0;
}
