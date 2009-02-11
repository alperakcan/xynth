/***************************************************************************
    begin                : Tue Sep 22 2007
    copyright            : (C) 2007 - 2009 by Alper Akcan
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

#include "../lib/xynth_.h"
#include "widget.h"

int w_pushbutton_set_text (w_object_t *object, char *text)
{
	w_pushbutton_t *pb;
	pb = (w_pushbutton_t *) object->data[OBJECT_PUSHBUTTON];
	w_textbox_set_str(pb->textbox->object, text);
	return 0;
}

int w_pushbutton_set_clicked (w_object_t *object, void (*clicked) (w_pushbutton_t *))
{
	w_pushbutton_t *pb;
	pb = (w_pushbutton_t *) object->data[OBJECT_PUSHBUTTON];
	pb->clicked = clicked;
	return 0;
}

void w_pushbutton_draw (w_object_t *object)
{
	w_pushbutton_t *pb;
	pb = (w_pushbutton_t *) object->data[OBJECT_PUSHBUTTON];
	w_button_draw(object);
	w_textbox_draw(pb->textbox->object);
}

void w_pushbutton_geometry (w_object_t *object)
{
	w_pushbutton_t *pb;
	pb = (w_pushbutton_t *) object->data[OBJECT_PUSHBUTTON];
	w_button_geometry(object);
	w_object_move(pb->textbox->object, 0, 0, object->surface->buf->w, object->surface->buf->h);

}

static void w_pushbutton_slot (w_signal_t *signal)
{
	w_pushbutton_t *pb;
	pb = (w_pushbutton_t *) ((w_object_t *) signal->arg)->data[OBJECT_PUSHBUTTON];
	if (pb->clicked != NULL) {
		pb->clicked(pb);
	}
}

static void w_pushbutton_released (w_object_t *object, int button)
{
	w_pushbutton_t *pb;
	pb = (w_pushbutton_t *) object->data[OBJECT_PUSHBUTTON];
	if (pb->clicked != NULL) {
		w_signal_send(object, object, w_pushbutton_slot, object);
	}
}

int w_pushbutton_init (w_window_t *window, w_pushbutton_t **pbutton, w_object_t *parent)
{
	w_pushbutton_t *pb;
	
	pb = (w_pushbutton_t *) s_malloc(sizeof(w_pushbutton_t));
	memset(pb, 0, sizeof(w_pushbutton_t));
	if (w_button_init(window, &pb->button, parent)) {
		goto err0;
	}
	if (w_textbox_init(window, &pb->textbox, pb->button->object)) {
		goto err1;
	}
	w_button_set_released(pb->button->object, w_pushbutton_released);
	w_textbox_set_properties(pb->textbox->object, TEXTBOX_VCENTER | TEXTBOX_HCENTER);
	w_textbox_set_style(pb->textbox->object, FRAME_NOFRAME, 0);
	w_object_show(pb->textbox->object);
	pb->object = pb->button->object;
	pb->object->type = OBJECT_PUSHBUTTON;
	pb->object->data[OBJECT_PUSHBUTTON] = pb;
	pb->object->draw = w_pushbutton_draw;
	pb->object->geometry = w_pushbutton_geometry;
	pb->object->destroy = w_pushbutton_uninit;
	*pbutton = pb;
	return 0;
err1:	w_button_uninit(pb->button->object);
err0:	*pbutton = NULL;
	return -1;
}

void w_pushbutton_uninit (w_object_t *object)
{
	w_pushbutton_t *pb;
	pb = (w_pushbutton_t *) object->data[OBJECT_PUSHBUTTON];
	w_button_uninit(object);
	s_free(pb);
}
