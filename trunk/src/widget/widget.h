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

typedef struct w_window_s {
	s_window_t *window;
	w_object_t *object;
} w_window_t;

typedef enum {
	OBJECT_FRAME   		= 0x0,
	OBJECT_BUTTON  		= 0x1,
	OBJECT_TEXTBOX 		= 0x2,
	OBJECT_EDITBOX 		= 0x3,
	OBJECT_LISTBOX 		= 0x4,
	OBJECT_PROGRESSBAR 	= 0x5,
	OBJECT_SCROLLBAR	= 0x6,
	OBJECT_COMBOBOX     = 0x7,
	OBJECT_RADIOBUTTON  = 0x8,
	OBJECT_OBJECTS 		= 0x9
} OBJECT;

struct w_object_s {
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

typedef struct w_frame_image_s {
	unsigned int style;
	unsigned int rotation;
	s_list_t *images;
} w_frame_image_t;

typedef struct w_frame_s {
	w_object_t *object;
	unsigned int style;
	unsigned int linewidth;
	unsigned int midlinewidth;
	s_list_t *images;
} w_frame_t;

typedef struct w_button_s {
	w_object_t *object;
	w_frame_t *frame;
	s_handler_t *handler;
	void (*pressed) (w_object_t *, int);
	void (*released) (w_object_t *, int);
	void (*clicked) (w_object_t *, int, int);
	int isimg;
	s_image_t *img_a;
	s_image_t *img_b;
	s_image_t *img_c;
	int state;
} w_button_t;

typedef enum {
	TEXTBOX_WRAP    = 0x1,
	TEXTBOX_NOWRAP  = 0x2,
	TEXTBOX_VCENTER = 0x4,
	TEXTBOX_HCENTER = 0x8
} TEXTBOX_PROPERTIES;

typedef struct w_textbox_s {
	w_object_t *object;
	w_frame_t *frame;
	s_font_t *font;
	s_image_t *img_left;
	s_image_t *img_middle;
	s_image_t *img_right;
	int isimg;
	TEXTBOX_PROPERTIES properties;
} w_textbox_t;

typedef struct w_progressbar_s {
	w_frame_t *frame;
	s_handler_t *handler;
	w_textbox_t *textbox;
	void (*pressed) (w_object_t *, int);
	void (*released) (w_object_t *, int);
	void (*clicked) (w_object_t *, int, int);
	int state;	
	int progressbarstatus;
} w_progressbar_t;

typedef struct w_scrollbar_s {
	w_frame_t *frame;
	s_handler_t *handler;
	void (*pressed) (w_object_t *, int);
	void (*released) (w_object_t *, int);
	void (*clicked) (w_object_t *, int, int);
	w_button_t *scrollup;
	w_button_t *scrolldown;
	w_button_t *scrollmiddle;
	w_button_t *background;
	int state;	
	int progressbarstatus;
	int activeitem;
	int maxactivescreenitem;	
	int maxactiveitem;
	int firstitemonscreen;
	int activeitemonscreen;
	w_object_t *friendobject;
} w_scrollbar_t;
	
typedef enum {
	LISTBOX_FRAME    = 0x1,
	LISTBOX_NOFRAME  = 0x2,
} LISTBOX_PROPERTIES;

typedef struct w_listbox_s {
	w_frame_t *object;
	w_frame_t *frame;
	s_list_t *list;
	s_handler_t *handler;
	int state;
	w_scrollbar_t *scrollbar;
	LISTBOX_PROPERTIES properties;
	int itemheight;
} w_listbox_t;

typedef struct w_editbox_s {
	w_object_t *object;
	w_textbox_t *textbox;
	s_handler_t *handler_mouse;
	s_handler_t *handler_keybd;
} w_editbox_t;

typedef struct w_radiobutton_s {
	w_object_t *object;
	w_frame_t *frame;
	w_button_t *button;
	w_editbox_t *editbox;
	int buttonwidth;
	int itemheight;
	int state;
} w_radiobutton_t;

typedef struct w_combobox_s {
	w_frame_t *frame;
	s_handler_t *handler;
	void (*pressed) (w_object_t *, int);
	void (*released) (w_object_t *, int);
	void (*clicked) (w_object_t *, int, int);
	int state;
	w_listbox_t *listbox;
	w_button_t *button;
	w_editbox_t *editbox;
	int islistboxvisible;
	int itemheight;
	int buttonlength;
} w_combobox_t;

typedef struct w_signal_s w_signal_t;
struct w_signal_s {
	w_object_t *from;
	w_object_t *to;
	void (*func) (w_signal_t *);
	void *arg;
};

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
int w_button_init (w_window_t *window, w_button_t **button, w_object_t *parent);
void w_button_uninit (w_object_t *object);
void w_button_loadimages(w_object_t *object,char *file_a,char *file_b,char *file_c,int isimg);
void w_button_loadstaticimage(w_object_t *object,char *file_normal,char *file_pressed,char *file_ontop);
void w_button_loadhorizantalimage(w_object_t *object,char *file_left,char *file_middle,char *file_right);
void w_button_loadverticalimage(w_object_t *object,char *file_up,char *file_middle,char *file_bottom);
void w_button_unloadimages(w_object_t *object);

/* editbox.c */
void w_editbox_draw (w_object_t *object);
void w_editbox_geometry (w_object_t *object);
int w_editbox_init (w_window_t *window, w_editbox_t **editbox, w_object_t *parent);
void w_editbox_uninit (w_object_t *object);

/* frame.c */
void w_frame_draw (w_object_t *object);
void w_frame_geometry (w_object_t *object);
int w_frame_init (w_window_t *window, w_frame_t **frame, unsigned int style, w_object_t *parent);
void w_frame_uninit (w_object_t *object);

/* listbox.c */
void w_listbox_draw (w_object_t *object);
void w_listbox_geometry(w_object_t *object);
int w_listbox_init (w_window_t *window, w_listbox_t **listbox, w_object_t *parent);
void w_listbox_uninit (w_object_t *object);
int w_listbox_add (w_listbox_t *listbox, char *item);

/* object.c */
int w_object_update_to_surface (w_object_t *object, s_surface_t *surface, s_rect_t *coor);
int w_object_update (w_object_t *object, s_rect_t *coor);
int w_object_set_content (w_object_t *object, int x, int y, int w, int h);
int w_object_move (w_object_t *object, int x, int y, int w, int h);
int w_object_hide (w_object_t *object);
int w_object_show (w_object_t *object);
int w_object_childatposition (w_object_t *object, int x, int y, w_object_t **child);
int w_object_atposition (w_object_t *root, int x, int y, w_object_t **object);
void w_object_signal (w_object_t *from, w_object_t *to, void (*func) (w_signal_t *), void *arg);
int w_object_init (w_window_t *window, w_object_t **object, void (*draw) (w_object_t *), w_object_t *parent);
void w_object_uninit (w_object_t *object);

/* textbox.c */
void w_textbox_draw (w_object_t *object);
int w_textbox_set_str (w_object_t *object, char *str);
void w_textbox_geometry (w_object_t *object);
int w_textbox_init (w_window_t *window, w_textbox_t **textbox, w_object_t *parent);
void w_textbox_uninit (w_object_t *object);
void w_textbox_loadimages(w_object_t *object,char *file_left,char *file_middle,char *file_right);

/*progressbar.c*/
void w_progressbar_uninit (w_object_t *object);
int w_progressbar_init (w_window_t *window, w_progressbar_t **progressbar, w_object_t *parent);
void w_progressbar_draw (w_object_t *object);
void w_progressbar_geometry (w_object_t *object);
void w_progressbar_setstatus (w_object_t *object,int progressbarstatus);
int w_progressbar_textbox_set_str (w_object_t *object, char *str,int barlength);

/* scrollbar.c*/
void w_scrollbar_geometry (w_object_t *object);
void w_scrollbar_draw (w_object_t *object);
int w_scrollbar_init (w_window_t *window, w_scrollbar_t **scrollbar, w_object_t *parent);
void w_scrollbar_uninit (w_object_t *object);
void w_buttonup_pressed (w_object_t *object, int buttonp);
void w_buttondown_pressed (w_object_t *object, int buttonp);
void w_scrollbar_loadimages (w_object_t *object,char *file_top,char *file_middle,char *file_bottom);

/*combobox.c*/
void w_combobox_geometry (w_object_t *object);
void w_combobox_draw (w_object_t *object);
int w_combobox_init (w_window_t *window, w_combobox_t **combobox, w_object_t *parent);
void w_combobox_uninit (w_object_t *object);

/*radiobutton.c*/
void w_radiobutton_geometry (w_object_t *object);
void w_radiobutton_draw (w_object_t *object);
int w_radiobutton_init (w_window_t *window, w_radiobutton_t **radiobutton, w_object_t *parent);
void w_radiobutton_uninit (w_object_t *object);

/* window.c */
void w_window_atevent (s_window_t *window, s_event_t *event);
int w_window_init (w_window_t **window, S_WINDOW type, w_window_t *parent);
int w_window_set_coor (w_window_t *window, int x, int y, int w, int h);
int w_window_uninit (w_window_t *window);

#endif
