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

#ifndef W_WIDGET_H_
#define W_WIDGET_H_

/* reduce memory usage, this is a bit hacky
 * some speed consumptions ;|
 * 
 * levels;
 * 	0: no memory optimizations, memory hungary but faster
 * 	1: disables image caching, reduces memory usage, especially
 * 	   for the applications that have several images
 * 	2: will free hided object's and its childs' buffers (vbuf and matrix)
 * 	   slowers show() hide() processes, but cute.
 * 	3: much more memory optimization, too slower, draws everthing on the fly.
 */
#define WIDGET_OPTIMIZE_MEMORY 2

typedef struct w_object_s w_object_t;
typedef struct w_button_s w_button_t;
typedef struct w_checkbox_s w_checkbox_t;
typedef struct w_editbox_s w_editbox_t;
typedef struct w_frame_image_s w_frame_image_t;
typedef struct w_frame_s w_frame_t;
typedef struct w_progressbar_s w_progressbar_t;
typedef struct w_scrollbufferbar_s w_scrollbufferbar_t;
typedef struct w_scrollbuffer_s w_scrollbuffer_t;
typedef struct w_textbox_s w_textbox_t;
typedef struct w_window_s w_window_t;

typedef struct w_effect_s w_effect_t;
typedef struct w_signal_s w_signal_t;

/* object.c */
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
	OBJECT_WINDOW		= 0xc,
	OBJECT_OBJECTS 		= 0xd
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
	/** internal data */
	void *data[OBJECT_OBJECTS];
	/** user data */
	void *priv;
};

/*@}*/

void w_object_hide_ (w_object_t *object);
void w_object_show_ (w_object_t *object);
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
int w_object_move_silent (w_object_t *object, int x, int y, int w, int h);
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
int w_object_isshownchild (w_object_t *parent, w_object_t *child);
int w_object_ischild (w_object_t *parent, w_object_t *child);
int w_object_init (w_window_t *window, w_object_t **object, void (*draw) (w_object_t *), w_object_t *parent);
void w_object_uninit (w_object_t *object);

/* frame.c */
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

int w_frame_set_style (w_object_t *object, FRAME_SHAPE shape, FRAME_SHADOW shadow);
int w_frame_image_init (w_frame_image_t **fimg);
int w_frame_image_uninit (w_frame_image_t *fimg);
int w_frame_set_image (w_object_t *object, unsigned int style, unsigned int rotation, unsigned int nimgs, char **imgs);
void w_frame_draw_image (w_object_t *object, w_frame_image_t *fimg);
void w_frame_draw (w_object_t *object);
void w_frame_geometry (w_object_t *object);
int w_frame_init (w_window_t *window, w_frame_t **frame, unsigned int style, w_object_t *parent);
void w_frame_uninit (w_object_t *object);

/* button.c */
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

/* textbox.c */
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

/* editbox.c */
struct w_editbox_s {
	w_object_t *object;
	w_textbox_t *textbox;
	s_handler_t *handler_mouse;
	s_handler_t *handler_keybd;
};

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

/* checkbox.c */
struct w_checkbox_s {
	w_object_t *object;
	w_button_t *button;
	w_frame_t *box;
	w_textbox_t *text;
	void (*changed) (w_object_t *, int);
	int state;
};

int w_checkbox_set_str (w_object_t *object, char *str);
int w_checkbox_set_changed (w_object_t *object, void (*changed) (w_object_t *, int));
int w_checkbox_set_style (w_object_t *object, FRAME_SHAPE shape, FRAME_SHADOW shadow);
int w_checkbox_set_image (w_object_t *object, unsigned int style, unsigned int rotation, unsigned int nimgs, char **imgs);
int w_checkbox_set_boxstyle (w_object_t *object, FRAME_SHAPE shape, FRAME_SHADOW shadow);
int w_checkbox_set_boximage (w_object_t *object, unsigned int style, unsigned int rotation, unsigned int nimgs, char **imgs);
int w_checkbox_set_rgb (w_object_t *object, int r, int g, int b);
int w_checkbox_set_size (w_object_t *object, int size);
int w_checkbox_set_properties (w_object_t *object, TEXTBOX_PROPERTIES properties);
void w_checkbox_geometry (w_object_t *object);
void w_checkbox_draw (w_object_t *object);
void w_checkbox_state (w_object_t *object, int state);
void w_checkbox_released (w_object_t *object, int btn);
void w_checkbox_uninit (w_object_t *object);
int w_checkbox_init (w_window_t *window, w_checkbox_t **checkbox, w_object_t *parent);

/* progressbar.c */
struct w_progressbar_s {
	w_object_t *object;
	w_frame_t *frame;
	w_frame_t *box;
	w_textbox_t *text;
	unsigned int level;
	void (*changed) (w_object_t *, int);
};

int w_progressbar_set_changed (w_object_t *object, void (*changed) (w_object_t *, int));
int w_progressbar_set_style (w_object_t *object, FRAME_SHAPE shape, FRAME_SHADOW shadow);
int w_progressbar_set_image (w_object_t *object, unsigned int style, unsigned int rotation, unsigned int nimgs, char **imgs);
int w_progressbar_set_boxstyle (w_object_t *object, FRAME_SHAPE shape, FRAME_SHADOW shadow);
int w_progressbar_set_boximage (w_object_t *object, unsigned int style, unsigned int rotation, unsigned int nimgs, char **imgs);
void w_progressbar_level (w_object_t *object, unsigned int level);
void w_progressbar_geometry (w_object_t *object);
int w_progressbar_init (w_window_t *window, w_progressbar_t **progressbar, w_object_t *parent);
void w_progressbar_uninit (w_object_t *object);

/* scrollbar.c */
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

/* window.c */
struct w_window_s {
	s_window_t *window;
	w_object_t *object;
	w_object_t *focus;
	s_list_t *images;
	s_list_t *fonts;
};

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

#endif /* W_WIDGET_H_ */
