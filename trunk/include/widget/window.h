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

#ifndef WINDOW_H_
#define WINDOW_H_

struct w_window_s {
	s_window_t *window;
	w_object_t *object;
	w_object_t *focus;
	s_list_t *images;
	s_list_t *fonts;
};

/* window.c */
void w_window_focus_change_notify (s_window_t *window, w_object_t *focus);
void w_window_change_keybd_focus (s_window_t *window, int type);
void w_window_atevent (s_window_t *window, s_event_t *event);
int w_window_init (w_window_t **window, S_WINDOW type, w_window_t *parent);
int w_window_set_coor (w_window_t *window, int x, int y, int w, int h);
s_image_t * w_window_image_get (w_window_t *window, char *image);
int w_window_image_add (w_window_t *window, char *image);
s_font_t * w_window_font_get (w_window_t *window, char *font);
int w_window_font_add (w_window_t *window, char *font);
int w_window_uninit (w_window_t *window);

#endif /*WINDOW_H_*/
