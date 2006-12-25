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

#ifndef W_SCROLLBUFFER_H_
#define W_SCROLLBUFFER_H_

struct w_scrollbufferbar_s {
	w_object_t *object;
	w_frame_t *frame;
	w_button_t *add;
	w_button_t *sub;
	w_frame_t *box;
};

struct w_scrollbuffer_s {
	w_object_t *object;
	w_frame_t *frame;
	w_scrollbufferbar_t *vertical;
	w_object_t *child;
	void (*slide) (w_object_t *, int, int, int *, int *);
};

/* scrollbuffer */
void w_scrollbuffer_set_slide (w_object_t *object, void (*slide) (w_object_t *, int, int, int *, int *));
void w_scrollbufferbar_add_pressed (w_object_t *object, int button);
void w_scrollbufferbar_sub_pressed (w_object_t *object, int button);
void w_scrollbufferbar_geometry (w_object_t *object);
void w_scrollbufferbar_uninit (w_object_t *object);
int w_scrollbufferbar_init (w_window_t *window, w_scrollbufferbar_t **scrollbufferbar, w_object_t *parent);
int w_scrollbuffer_set_boxstyle (w_object_t *object, FRAME_SHAPE shape, FRAME_SHADOW shadow);
int w_scrollbuffer_set_boximage (w_object_t *object, unsigned int style, unsigned int rotation, unsigned int nimgs, char **imgs);
int w_scrollbuffer_set_addstyle (w_object_t *object, FRAME_SHAPE shape, FRAME_SHADOW shadow);
int w_scrollbuffer_set_addimage (w_object_t *object, unsigned int style, unsigned int rotation, unsigned int nimgs, char **imgs);
int w_scrollbuffer_set_substyle (w_object_t *object, FRAME_SHAPE shape, FRAME_SHADOW shadow);
int w_scrollbuffer_set_subimage (w_object_t *object, unsigned int style, unsigned int rotation, unsigned int nimgs, char **imgs);
void w_scrollbuffer_slide (w_object_t *object, int vertical, int horizontal);
void w_scrollbuffer_set_child (w_object_t *object, w_object_t *child);
void w_scrollbuffer_geometry (w_object_t *object);
void w_scrollbuffer_uninit (w_object_t *object);
int w_scrollbuffer_init (w_window_t *window, w_scrollbuffer_t **scrollbuffer, w_object_t *parent);

#endif /* W_SCROLLBUFFER_H_ */
