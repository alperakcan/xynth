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

typedef struct w_object_s w_object_t;

typedef enum {
	OBJECT_FRAME   = 0x0,
	OBJECT_BUTTON  = 0x1,
	OBJECT_TEXTBOX = 0x2,
	OBJECT_OBJECTS = 0x3
} OBJECT;

struct w_object_s {
	/** child list */
	s_list_t *childs;
	/** shown list */
	s_list_t *shown;
	/** root mutex */
	s_thread_mutex_t *mut;
	/** the surface */
	s_surface_t *surface;
	/** the parent of the object */
	w_object_t *parent;
	/** content allowed rectangle */
	s_rect_t *content;
	/** geometry function */
	void (*geometry) (w_object_t *object);
	/** draw function */
	void (*draw) (w_object_t *object);
	/** event function */
	void (*event) (w_object_t *object, s_event_t *event);
	/** uninit function */
	void (*destroy) (w_object_t *object);
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

typedef struct w_frame_s {
	w_object_t *object;
	unsigned int style;
	unsigned int linewidth;
	unsigned int midlinewidth;
} w_frame_t;

typedef struct w_button_s {
	w_frame_t *frame;
	s_handler_t *handler;
	void (*pressed) (w_object_t *, int);
	void (*released) (w_object_t *, int);
	void (*clicked) (w_object_t *, int, int);
	int state;
} w_button_t;

typedef enum {
	TEXTBOX_WRAP,
	TEXTBOX_NOWRAP
} TEXTBOX_PROPERTIES;

typedef struct w_textbox_s {
	w_frame_t *frame;
	s_font_t *font;
	TEXTBOX_PROPERTIES properties;
} w_textbox_t;

typedef struct w_window_s {
	s_window_t *window;
	w_object_t *object;
} w_window_t;

/* button.c */
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
int w_button_init (s_window_t *window, w_button_t **button, w_object_t *parent);
void w_button_uninit (w_object_t *object);

/* frame.c */
void w_frame_content (w_frame_t *frame);
void w_frame_draw (w_object_t *object);
int w_frame_init (s_window_t *window, w_frame_t **frame, unsigned int style, w_object_t *parent);
void w_frame_uninit (w_object_t *object);

/* object.c */
int w_object_update_to_surface (w_object_t *object, s_surface_t *surface, s_rect_t *coor);
int w_object_update (w_object_t *object, s_rect_t *coor);
int w_object_set_content (w_object_t *object, int x, int y, int w, int h);
int w_object_move (w_object_t *object, int x, int y, int w, int h);
int w_object_hide (w_object_t *object);
int w_object_show (w_object_t *object);
int w_object_childatposition (w_object_t *object, int x, int y, w_object_t **child);
int w_object_atposition (w_object_t *root, int x, int y, w_object_t **object);
int w_object_init (s_window_t *window, w_object_t **object, void (*draw) (w_object_t *), w_object_t *parent);
void w_object_uninit (w_object_t *object);

/* textbox.c */
void w_textbox_draw (w_object_t *object);
int w_textbox_init (s_window_t *window, w_textbox_t **textbox, w_object_t *parent);
void w_textbox_uninit (w_object_t *object);

/* window.c */
void w_window_atevent (s_window_t *window, s_event_t *event);
int w_window_init (w_window_t **window, S_WINDOW type, w_window_t *parent);
int w_window_set_coor (w_window_t *window, int x, int y, int w, int h);
int w_window_uninit (w_window_t *window);

#endif
