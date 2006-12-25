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

#ifndef EDITBOX_H_
#define EDITBOX_H_

struct w_editbox_s {
	w_object_t *object;
	w_textbox_t *textbox;
	s_handler_t *handler_mouse;
	s_handler_t *handler_keybd;
};

/* editbox.c */
int w_editbox_set_str (w_object_t *object, char *str);
int w_editbox_set_style (w_object_t *object, FRAME_SHAPE shape, FRAME_SHADOW shadow);
int w_editbox_set_image (w_object_t *object, unsigned int style, unsigned int rotation, unsigned int nimgs, char **imgs);
int w_editbox_set_size (w_object_t *object, int size);
int w_editbox_set_rgb (w_object_t *object, int r, int g, int b);
int w_editbox_set_properties (w_object_t *object, TEXTBOX_PROPERTIES properties);
void w_editbox_draw (w_object_t *object);
void w_editbox_geometry (w_object_t *object);
int w_editbox_init (w_window_t *window, w_editbox_t **editbox, w_object_t *parent);
void w_editbox_uninit (w_object_t *object);

#endif /*EDITBOX_H_*/
