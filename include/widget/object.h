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

#ifndef OBJECT_H_
#define OBJECT_H_

typedef enum {
	OBJECT_OBJECT		= 0x0,
	OBJECT_FRAME   		= 0x1,
	OBJECT_BUTTON  		= 0x2,
	OBJECT_TEXTBOX 		= 0x3,
	OBJECT_EDITBOX 		= 0x4,
	OBJECT_LISTBOX 		= 0x5,
	OBJECT_PROGRESSBAR 	= 0x6,
	OBJECT_SCROLLBAR	= 0x7,
	OBJECT_COMBOBOX		= 0x8,
	OBJECT_CHECKBOX		= 0x9,
	OBJECT_SCROLLBUFFER	= 0xa,
	OBJECT_SCROLLBUFFERBAR	= 0xb,
	OBJECT_OBJECTS 		= 0xc
} OBJECT;

typedef enum {
	EFFECT_NONE    = 0x0,
	EFFECT_FADEIN  = 0x1,
	EFFECT_FADEOUT = 0x2,
	EFFECT_POPIN   = 0x4,
	EFFECT_POPOUT  = 0x8,
	EFFECT_SHOW    = (EFFECT_FADEIN | EFFECT_POPIN),
	EFFECT_HIDE    = (EFFECT_FADEOUT | EFFECT_POPOUT)
} EFFECT;

struct w_effect_s {
	EFFECT effect;
	int level;
	int interval;
	s_timer_t *timer;
};

struct w_signal_s {
	w_object_t *from;
	w_object_t *to;
	void (*func) (w_signal_t *);
	void *arg;
};

/** @defgroup widget_object Widget Library - Object API
  * @brief
  * 
  * @example
  *
  * for further information look in demo/ directory
  *
  * @code
  * // simple example will be in here
  * @endcode
  */

/** @addtogroup widget_object */
/*@{*/

/** object struct
 */
struct w_object_s {
	/** object type */
	OBJECT type;
	/** child list */
	s_list_t *childs;
	/** shown list */
	s_list_t *shown;
	/** the surface */
	s_surface_t *surface;
	/** the parent of the object */
	w_object_t *parent;
	/** content allowed rectangle */
	s_rect_t *content;
	/** is shown */
	int showed;
	/** is focused */
	int focused;
	/** effect */
	w_effect_t *effect;
	/** geometry function */
	void (*geometry) (w_object_t *object);
	/** draw function */
	void (*draw) (w_object_t *object);
	/** event function */
	void (*event) (w_object_t *object, s_event_t *event);
	/** uninit function */
	void (*destroy) (w_object_t *object);
	/** window */
	w_window_t *window;
	/** user data */
	void *data[OBJECT_OBJECTS];
	void *priv;
};

/*@}*/

/* object.c */
int w_object_effect_stop (w_object_t *object);
void w_object_effect_timer_cb (s_window_t *window, s_timer_t *timer);
int w_object_effect_start (w_object_t *object);
int w_object_has_effect (w_object_t *effect, w_object_t *object);
int w_object_effect_apply (s_surface_t *surface, s_rect_t *rect, w_object_t *effect, w_object_t *object);
int w_object_update_to_surface (w_object_t *object, s_surface_t *surface, s_rect_t *coor, w_object_t *effect, int do_effect);
int w_object_update (w_object_t *object, s_rect_t *coor);
int w_object_draw (w_object_t *object);
int w_object_refresh (w_object_t *object);
int w_object_set_content (w_object_t *object, int x, int y, int w, int h);
int w_object_move_correct (w_object_t *object);
int w_object_move (w_object_t *object, int x, int y, int w, int h);
int w_object_hide (w_object_t *object);
int w_object_show (w_object_t *object);
int w_object_childatposition (w_object_t *object, int x, int y, w_object_t **child);
int w_object_atposition (w_object_t *root, int x, int y, w_object_t **object);
void w_object_signal (w_object_t *from, w_object_t *to, void (*func) (w_signal_t *), void *arg);
int w_object_level_get_ (w_object_t *parent, w_object_t **object, int *level);
int w_object_level_get (w_object_t *parent, w_object_t **object, int level);
int w_object_level_count_ (w_object_t *parent, int *level);
int w_object_level_count (w_object_t *parent, int *level);
int w_object_level_find_ (w_object_t *parent, w_object_t *object, int *level);
int w_object_level_find (w_object_t *parent, w_object_t *object, int *level);
int w_object_ischild (w_object_t *parent, w_object_t *child);
int w_object_isshownchild (w_object_t *parent, w_object_t *child);
int w_object_init (w_window_t *window, w_object_t **object, void (*draw) (w_object_t *), w_object_t *parent);
void w_object_uninit (w_object_t *object);

#endif /*OBJECT_H_*/
