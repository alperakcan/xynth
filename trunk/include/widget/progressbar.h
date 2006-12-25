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

#ifndef W_PROGRESSBAR_H_
#define W_PROGRESSBAR_H_

struct w_progressbar_s {
	w_object_t *object;
	w_frame_t *frame;
	w_frame_t *box;
	w_textbox_t *text;
	unsigned int level;
	void (*changed) (w_object_t *, int);
};

/* progressbar.c */
int w_progressbar_set_changed (w_object_t *object, void (*changed) (w_object_t *, int));
int w_progressbar_set_style (w_object_t *object, FRAME_SHAPE shape, FRAME_SHADOW shadow);
int w_progressbar_set_image (w_object_t *object, unsigned int style, unsigned int rotation, unsigned int nimgs, char **imgs);
int w_progressbar_set_boxstyle (w_object_t *object, FRAME_SHAPE shape, FRAME_SHADOW shadow);
int w_progressbar_set_boximage (w_object_t *object, unsigned int style, unsigned int rotation, unsigned int nimgs, char **imgs);
void w_progressbar_level (w_object_t *object, unsigned int level);
void w_progressbar_geometry (w_object_t *object);
int w_progressbar_init (w_window_t *window, w_progressbar_t **progressbar, w_object_t *parent);
void w_progressbar_uninit (w_object_t *object);

#endif /* W_PROGRESSBAR_H_ */
