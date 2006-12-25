/***************************************************************************
    begin                : Mon Sep 20 2004
    copyright            : (C) 2004 - 2006 by Alper Akcan
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

#ifndef CHECKBOX_H_
#define CHECKBOX_H_

struct w_checkbox_s {
	w_object_t *object;
	w_button_t *button;
	w_frame_t *box;
	w_textbox_t *text;
	void (*changed) (w_object_t *, int);
	int state;
};

/* checkbox */
int w_checkbox_set_str (w_object_t *object, char *str);
int w_checkbox_set_changed (w_object_t *object, void (*changed) (w_object_t *, int));
int w_checkbox_set_style (w_object_t *object, FRAME_SHAPE shape, FRAME_SHADOW shadow);
int w_checkbox_set_image (w_object_t *object, unsigned int style, unsigned int rotation, unsigned int nimgs, char **imgs);
int w_checkbox_set_boxstyle (w_object_t *object, FRAME_SHAPE shape, FRAME_SHADOW shadow);
int w_checkbox_set_boximage (w_object_t *object, unsigned int style, unsigned int rotation, unsigned int nimgs, char **imgs);
int w_checkbox_set_rgb (w_object_t *object, int r, int g, int b);
int w_checkbox_set_size (w_object_t *object, int size);
int w_checkbox_set_properties (w_object_t *object, TEXTBOX_PROPERTIES properties);
void w_checkbox_geometry (w_object_t *object);
void w_checkbox_draw (w_object_t *object);
void w_checkbox_state (w_object_t *object, int state);
void w_checkbox_released (w_object_t *object, int btn);
void w_checkbox_uninit (w_object_t *object);
int w_checkbox_init (w_window_t *window, w_checkbox_t **checkbox, w_object_t *parent);

#endif /*CHECKBOX_H_*/
