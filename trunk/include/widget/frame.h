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

#ifndef FRAME_H_
#define FRAME_H_

typedef enum {
	FRAME_NOFRAME		= 0x00,
	FRAME_BOX		= 0x01,
	FRAME_PANEL		= 0x02,
	FRAME_WINPANEL		= 0x03,
	FRAME_HLINE		= 0x04,
	FRAME_VLINE		= 0x05,
	FRAME_STYLEDPANEL	= 0x06,
	FRAME_POPUPPANEL	= 0x07,
	FRAME_MENUBARPANEL	= 0x08,
	FRAME_TOOLBARPANEL	= 0x09,
	FRAME_LINEEDITPANEL	= 0x0a,
	FRAME_TABWIDGETPANEL	= 0x0b,
	FRAME_GROUPBOXPANEL	= 0x0c,
	FRAME_EMPTY             = 0x0d,
	FRAME_MSHAPE		= 0x0f
} FRAME_SHAPE;

typedef enum {
	FRAME_PLAIN		= 0x010,
	FRAME_RAISED		= 0x020,
	FRAME_SUNKEN		= 0x040,
	FRAME_FOCUSED           = 0x100,
	FRAME_MSHADOW		= 0x0f0
} FRAME_SHADOW;

typedef enum {
	FRAME_IMAGE_SOLID       = 0x0,
	FRAME_IMAGE_VERTICAL    = 0x1,
	FRAME_IMAGE_HORIZONTAL  = 0x2,
} FRAME_IMAGE_ROTATION;

struct w_frame_image_s {
	unsigned int style;
	unsigned int rotation;
	s_list_t *images;
	s_list_t *names;
};

struct w_frame_s {
	w_object_t *object;
	unsigned int style;
	unsigned int linewidth;
	unsigned int midlinewidth;
	s_list_t *images;
};

/* frame.c */
int w_frame_set_style (w_object_t *object, FRAME_SHAPE shape, FRAME_SHADOW shadow);
int w_frame_image_init (w_frame_image_t **fimg);
int w_frame_image_uninit (w_frame_image_t *fimg);
int w_frame_set_image (w_object_t *object, unsigned int style, unsigned int rotation, unsigned int nimgs, char **imgs);
void w_frame_draw_image (w_object_t *object, w_frame_image_t *fimg);
void w_frame_draw (w_object_t *object);
void w_frame_geometry (w_object_t *object);
int w_frame_init (w_window_t *window, w_frame_t **frame, unsigned int style, w_object_t *parent);
void w_frame_uninit (w_object_t *object);

#endif /*FRAME_H_*/
