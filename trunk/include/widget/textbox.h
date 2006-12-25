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

#ifndef W_TEXTBOX_H_
#define W_TEXTBOX_H_

typedef enum {
	TEXTBOX_WRAP    = 0x1,
	TEXTBOX_VCENTER = 0x2,
	TEXTBOX_HCENTER = 0x4
} TEXTBOX_PROPERTIES;

struct w_textbox_s {
	w_object_t *object;
	w_frame_t *frame;
	s_list_t *lines;
	char *str;
	char *font;
	unsigned int size;
	unsigned int rgb;
	TEXTBOX_PROPERTIES properties;
	int height;
	int yoffset;
};

/* textbox.c */
void w_textbox_slide (w_object_t *object, int vertical, int horizontal, int *ytotal, int *yoffset);
int w_textbox_set_properties (w_object_t *object, TEXTBOX_PROPERTIES properties);
int w_textbox_set_image (w_object_t *object, unsigned int style, unsigned int rotation, unsigned int nimgs, char **imgs);
int w_textbox_set_style (w_object_t *object, FRAME_SHAPE shape, FRAME_SHADOW shadow);
void w_textbox_draw (w_object_t *object);
int w_textbox_set_rgb (w_object_t *object, int r, int g, int b);
int w_textbox_set_size (w_object_t *object, int size);
int w_textbox_set_str (w_object_t *object, char *str);
void w_textbox_geometry (w_object_t *object);
int w_textbox_init (w_window_t *window, w_textbox_t **textbox, w_object_t *parent);
void w_textbox_uninit (w_object_t *object);
void w_textbox_loadimages(w_object_t *object,char *file_left,char *file_middle,char *file_right);

#endif /* W_TEXTBOX_H_ */
