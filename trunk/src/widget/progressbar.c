/***************************************************************************
    begin                : Fri Sep 8 2006
    copyright            : (C) 2006 - 2007 by Alper Akcan
    email		 : distchx@yahoo.com, computationutku@yahoo.com
    file author          : Alper Akcan, Utku Bulkan
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

int w_progressbar_set_changed (w_object_t *object, void (*changed) (w_object_t *, int))
{
	w_progressbar_t *pb;
	pb = object->data[OBJECT_PROGRESSBAR];
	pb->changed = changed;
	return 0;
}

int w_progressbar_set_style (w_object_t *object, FRAME_SHAPE shape, FRAME_SHADOW shadow)
{
	return w_frame_set_style(object, shape, shadow);
}

int w_progressbar_set_image (w_object_t *object, unsigned int style, unsigned int rotation, unsigned int nimgs, char **imgs)
{
	return w_frame_set_image(object, style, rotation, nimgs, imgs);
}

int w_progressbar_set_boxstyle (w_object_t *object, FRAME_SHAPE shape, FRAME_SHADOW shadow)
{
	w_progressbar_t *pb;
	pb = object->data[OBJECT_PROGRESSBAR];
	return w_frame_set_style(pb->box->object, shape, shadow);
}

int w_progressbar_set_boximage (w_object_t *object, unsigned int style, unsigned int rotation, unsigned int nimgs, char **imgs)
{
	w_progressbar_t *pb;
	pb = object->data[OBJECT_PROGRESSBAR];
	return w_frame_set_image(pb->box->object, style, rotation, nimgs, imgs);
}

int w_progressbar_set_properties (w_object_t *object, PROGRESSBAR_PROPERTIES properties)
{
	w_progressbar_t *pb;
	pb = object->data[OBJECT_PROGRESSBAR];
	pb->properties = properties;
	if ((pb->properties & PROGRESSBAR_TEXT) == 0) {
		w_object_hide(pb->text->object);
	}
	return 0;
}

void w_progressbar_level (w_object_t *object, unsigned int level)
{
	char str[10];
	w_progressbar_t *pb;
	pb = object->data[OBJECT_PROGRESSBAR];
	pb->level = level;
	pb->level = MIN(100, pb->level);
	pb->level = MAX(0, pb->level);
	sprintf(str, "%% %d", pb->level);
	w_progressbar_geometry(object);
	w_textbox_set_str(pb->text->object, str);
	w_object_update(object, object->surface->win);
	if (pb->changed) {
		pb->changed(object, pb->level);
	}
}

void w_progressbar_geometry (w_object_t *object)
{
	int x;
	int w;
	w_progressbar_t *pb;
	pb = object->data[OBJECT_PROGRESSBAR];
	w_frame_geometry(object);
	if (pb->properties & PROGRESSBAR_SLIDE) {
		w = ((object->content->w - 4) * pb->level) / 100;
		w = MAX(w, 0);
		w_object_move(pb->box->object, 2, 2, w, object->content->h - 4);
	} else {
		x = ((object->content->w - object->content->h) * pb->level) / 100;
//		x = MAX(w, 0);
		w_object_move(pb->box->object, x + 2, 2, object->content->h - 4, object->content->h - 4);
	}
	if (pb->properties & PROGRESSBAR_TEXT) {
		w_object_move(pb->text->object, 2, 2, object->content->w - 4, object->content->h - 4);
	}
}

int w_progressbar_init (w_window_t *window, w_progressbar_t **progressbar, w_object_t *parent)
{
	w_progressbar_t *pb;
	pb = (w_progressbar_t *) s_malloc(sizeof(w_progressbar_t));
	pb->level = 0;
	w_frame_init(window, &(pb->frame), FRAME_PANEL | FRAME_PLAIN, parent);
	w_frame_init(window, &(pb->box), FRAME_PANEL | FRAME_PLAIN, pb->frame->object);
	w_textbox_init(window, &(pb->text), pb->frame->object);
	w_textbox_set_str(pb->text->object, "%0");
	pb->text->frame->style = FRAME_NOFRAME;
	pb->text->properties = TEXTBOX_HCENTER | TEXTBOX_VCENTER;
	pb->properties = PROGRESSBAR_SLIDE | PROGRESSBAR_TEXT;
	w_object_show(pb->box->object);
	w_object_show(pb->text->object);
	pb->changed = NULL;
	pb->object = pb->frame->object;
	pb->object->type = OBJECT_PROGRESSBAR;
	pb->object->geometry = w_progressbar_geometry;
	pb->object->destroy = w_progressbar_uninit;
	pb->object->data[OBJECT_PROGRESSBAR] = pb;
	*progressbar = pb;
	return 0;
}

void w_progressbar_uninit (w_object_t *object)
{
	w_progressbar_t *pb;
	pb = object->data[OBJECT_PROGRESSBAR];
	w_frame_uninit(pb->box->object);
	w_textbox_uninit(pb->text->object);
	w_frame_uninit(object);
	s_free(pb);
}
