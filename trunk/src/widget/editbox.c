/***************************************************************************
    begin                : Tue Sep 14 2006
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

int w_editbox_set_str (w_object_t *object, char *str)
{
	return w_textbox_set_str(object, str);
}

int w_editbox_set_style (w_object_t *object, FRAME_SHAPE shape, FRAME_SHADOW shadow)
{
	return w_textbox_set_style(object, shape, shadow);
}

int w_editbox_set_image (w_object_t *object, unsigned int style, unsigned int rotation, unsigned int nimgs, char **imgs)
{
	return w_textbox_set_image(object, style, rotation, nimgs, imgs);
}

int w_editbox_set_size (w_object_t *object, int size)
{
	return w_textbox_set_size(object, size);
}

int w_editbox_set_rgb (w_object_t *object, int r, int g, int b)
{
	return w_textbox_set_rgb(object, r, g, b);
}

int w_editbox_set_properties (w_object_t *object, TEXTBOX_PROPERTIES properties)
{
	return w_textbox_set_properties(object, properties);
}

void w_editbox_event (w_object_t *object, s_event_t *event)
{
	char *str;
	char *ptr;
	w_editbox_t *editbox;
	editbox = object->data[OBJECT_EDITBOX];
	if (event->type & KEYBD_EVENT) {
		if (event->type & KEYBD_PRESSED) {
			if (event->keybd->keycode == S_KEYCODE_DELETE) {
					str = strdup(editbox->textbox->str);
					if (strlen(str) > 0) {
						*(str + strlen(str) - 1) = '\0';
					}
					w_textbox_set_str(object, str);
					w_object_update(object, object->surface->win);
					s_free(str);
			} else {
				if (event->keybd->ascii) {
					if (editbox->textbox->str == NULL) {
						str = strdup("");
					} else {
						str = strdup(editbox->textbox->str);
					}
					ptr = (char *) s_malloc(sizeof(char *) * (strlen(str) + 1 + 1));
					sprintf(ptr, "%s%c", str, event->keybd->ascii);
					w_textbox_set_str(object, ptr);
					w_object_update(object, object->surface->win);
					s_free(str);
					s_free(ptr);
				}
			}
		}
	}
}

void w_editbox_draw (w_object_t *object)
{
	w_editbox_t *editbox;
	editbox = object->data[OBJECT_EDITBOX];
	w_textbox_draw(object);
}

void w_editbox_geometry (w_object_t *object)
{
	w_textbox_geometry(object);
}

int w_editbox_init (w_window_t *window, w_editbox_t **editbox, w_object_t *parent)
{
	(*editbox) = (w_editbox_t *) s_malloc(sizeof(w_editbox_t));
	w_textbox_init(window, &((*editbox)->textbox), parent);
	s_handler_init(&((*editbox)->handler_mouse));
	s_handler_init(&((*editbox)->handler_keybd));
	(*editbox)->textbox->properties = TEXTBOX_VCENTER;
	(*editbox)->textbox->frame->style = FRAME_LINEEDITPANEL | FRAME_SUNKEN;
	(*editbox)->object = (*editbox)->textbox->frame->object;
	(*editbox)->object->type = OBJECT_EDITBOX;
	(*editbox)->object->data[OBJECT_EDITBOX] = *editbox;
	(*editbox)->object->geometry = w_editbox_geometry;
	(*editbox)->object->draw = w_editbox_draw;
	(*editbox)->object->event = w_editbox_event;
	(*editbox)->object->destroy = w_editbox_uninit;
	return 0;
}

void w_editbox_uninit (w_object_t *object)
{
	w_editbox_t *editbox;
	editbox = (w_editbox_t *) object->data[OBJECT_EDITBOX];
	w_textbox_uninit(object);
	s_handler_uninit(editbox->handler_mouse);
	s_handler_uninit(editbox->handler_keybd);
	s_free(editbox);
}
