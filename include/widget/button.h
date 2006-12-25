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

#ifndef BUTTON_H_
#define BUTTON_H_

struct w_button_s {
	w_object_t *object;
	w_frame_t *frame;
	s_handler_t *handler_m;
	s_handler_t *handler_k;
	void (*pressed) (w_object_t *, int);
	void (*released) (w_object_t *, int);
	void (*clicked) (w_object_t *, int, int);
	int state;
};

/* button.c */
int w_button_set_pressed (w_object_t *object, void (*pressed) (w_object_t *, int));
int w_button_set_released (w_object_t *object, void (*released) (w_object_t *, int));
int w_button_set_clicked (w_object_t *object, void (*clicked) (w_object_t *, int, int));
int w_button_set_image (w_object_t *object, unsigned int style, unsigned int rotation, unsigned int nimgs, char **imgs);
int w_button_set_style (w_object_t *object, FRAME_SHAPE shape, FRAME_SHADOW shadow);
void w_button_event (w_object_t *object, s_event_t *event);
void w_button_draw (w_object_t *object);
void w_button_geometry (w_object_t *object);
void w_button_cb_o (s_window_t *window, s_event_t *event, s_handler_t *handler);
void w_button_cb_p (s_window_t *window, s_event_t *event, s_handler_t *handler);
void w_button_cb_c (s_window_t *window, s_event_t *event, s_handler_t *handler);
void w_button_cb_r (s_window_t *window, s_event_t *event, s_handler_t *handler);
void w_button_cb_hr (s_window_t *window, s_event_t *event, s_handler_t *handler);
void w_button_cb_rh (s_window_t *window, s_event_t *event, s_handler_t *handler);
void w_button_cb_ho (s_window_t *window, s_event_t *event, s_handler_t *handler);
void w_button_cb_oh (s_window_t *window, s_event_t *event, s_handler_t *handler);
void w_button_cb_hoh (s_window_t *window, s_event_t *event, s_handler_t *handler);
int w_button_init (w_window_t *window, w_button_t **button, w_object_t *parent);
void w_button_uninit (w_object_t *object);

#endif /*BUTTON_H_*/
