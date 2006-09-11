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

#ifndef SWIDGET_H
#define SWIDGET_H

typedef struct s_object_s s_object_t;

typedef enum {
	OBJECT_FRAME   = 0x0,
	OBJECT_BUTTON  = 0x1,
	OBJECT_OBJECTS = 0x2
} OBJECT;

struct s_object_s {
	/** child list */
	s_list_t *childs;
	/** shown list */
	s_list_t *shown;
	/** root mutex */
	s_thread_mutex_t *mut;
	/** the surface */
	s_surface_t *surface;
	/** the parent of the object */
	s_object_t *parent;
	/** content allowed rectangle */
	s_rect_t *content;
	/** geometry function */
	void (*geometry) (s_object_t *object);
	/** draw function */
	void (*draw) (s_object_t *object);
	/** event function */
	void (*event) (s_object_t *object, s_event_t *event);
	/** uninit function */
	void (*destroy) (s_object_t *object);
	/** window */
	s_window_t *window;
	/** user data */
	void *data[OBJECT_OBJECTS];
};

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
	FRAME_MSHAPE		= 0x0f
} FRAME_SHAPE;

typedef enum {
	FRAME_PLAIN		= 0x10,
	FRAME_RAISED		= 0x20,
	FRAME_SUNKEN		= 0x40,
	FRAME_MSHADOW		= 0xf0
} FRAME_SHADOW;

typedef struct s_frame_s {
	s_object_t *object;
	unsigned int style;
	unsigned int linewidth;
	unsigned int midlinewidth;
} s_frame_t;

typedef struct s_button_s {
	s_frame_t *frame;
	s_handler_t *handler;
	void (*pressed) (s_object_t *, int);
	void (*released) (s_object_t *, int);
	void (*clicked) (s_object_t *, int, int);
	int state;
} s_button_t;

/* button.c */
void s_button_event (s_object_t *object, s_event_t *event);
void s_button_draw (s_object_t *object);
void s_button_geometry (s_object_t *object);
void s_button_cb_o (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_button_cb_p (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_button_cb_c (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_button_cb_r (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_button_cb_hr (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_button_cb_rh (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_button_cb_ho (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_button_cb_oh (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_button_cb_hoh (s_window_t *window, s_event_t *event, s_handler_t *handler);
int s_button_init (s_window_t *window, s_button_t **button, s_object_t *parent);
void s_button_uninit (s_object_t *object);

/* frame.c */
void s_frame_content (s_frame_t *frame);
void s_frame_draw (s_object_t *object);
int s_frame_init (s_window_t *window, s_frame_t **frame, unsigned int style, s_object_t *parent);
void s_frame_uninit (s_object_t *object);

/* object.c */
int s_object_update_to_surface (s_object_t *object, s_surface_t *surface, s_rect_t *coor);
int s_object_update (s_object_t *object, s_rect_t *coor);
int s_object_set_content (s_object_t *object, int x, int y, int w, int h);
int s_object_move (s_object_t *object, int x, int y, int w, int h);
int s_object_hide (s_object_t *object);
int s_object_show (s_object_t *object);
int s_object_childatposition (s_object_t *object, int x, int y, s_object_t **child);
int s_object_atposition (s_object_t *root, int x, int y, s_object_t **object);
int s_object_init (s_window_t *window, s_object_t **object, void (*draw) (s_object_t *), s_object_t *parent);
void s_object_uninit (s_object_t *object);

#endif
