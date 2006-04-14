/***************************************************************************
    begin                : Tue Oct 14 2003
    copyright            : (C) 2003 - 2006 by Alper Akcan
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

#ifndef SXYNTH_H
#define SXYNTH_H

#ifdef __cplusplus
	extern "C" {
#endif

typedef struct s_thread_s s_thread_t;
typedef struct s_thread_sem_s s_thread_sem_t;
typedef struct s_thread_cond_s s_thread_cond_t;
typedef struct s_thread_mutex_s s_thread_mutex_t;

typedef struct s_font_ft_s s_font_ft_t;

typedef struct s_handler_s s_handler_t;
typedef struct s_window_s s_window_t;

typedef struct s_image_s s_image_t;
typedef struct s_list_s s_list_t;
typedef struct s_list_node_s s_list_node_t;
typedef struct s_pollfd_s s_pollfd_t;
typedef struct s_pollfds_s s_pollfds_t;
typedef struct s_rect_s s_rect_t;
typedef struct s_timer_s s_timer_t;
typedef struct s_timers_s s_timers_t;

typedef enum {
	SOC_DATA_NOTHING   = 0x0,
	SOC_DATA_NEW       = 0x1,
	SOC_DATA_HIDE      = 0x2,
	SOC_DATA_SHOW      = 0x3,
	SOC_DATA_CLOSE     = 0x5,
	SOC_DATA_EVENT     = 0x6,
	SOC_DATA_EXPOSE    = 0x7,
	SOC_DATA_STREAM    = 0x8,
	SOC_DATA_DISPLAY   = 0x9,
	SOC_DATA_FORMDRAW  = 0xA,
	SOC_DATA_CONFIGURE = 0xB,
	SOC_DATA_DESKTOP   = 0xC,
	SOC_DATA_CURSOR    = 0xD
} S_SOC_DATA;

typedef enum {
        WINDOW_NEW        = 0x0001,
	WINDOW_COOR       = 0x0002,
	WINDOW_DRAW       = 0x0004,
	WINDOW_SHOW       = 0x0008,
	WINDOW_TITLE      = 0x0010,
	WINDOW_RESIZEABLE = 0x0020,

	WINDOW_NOFORM     = 0x0040,
	WINDOW_MAIN       = 0x0080,
	WINDOW_TEMP       = 0x0100,
	WINDOW_CHILD      = 0x0200,
	WINDOW_DESKTOP    = 0x0400
} S_WINDOW;

typedef enum {
	SURFACE_REAL       = 0x1,
	SURFACE_VIRTUAL    = 0x2,
	SURFACE_NEEDEXPOSE = 0x4,
	SURFACE_NEEDSTREAM = 0x8
} S_SURFACE_MODE;

typedef enum {
	MOUSE_CURSOR_WAIT    = 0x0,
	MOUSE_CURSOR_CROSS   = 0x1,
	MOUSE_CURSOR_IBEAM   = 0x2,
	MOUSE_CURSOR_SIZEV   = 0x3,
	MOUSE_CURSOR_SIZEH   = 0x4,
	MOUSE_CURSOR_SIZES   = 0x5,
	MOUSE_CURSOR_SIZEB   = 0x6,
	MOUSE_CURSOR_SIZEA   = 0x7,
	MOUSE_CURSOR_ARROW   = 0x8,
	MOUSE_CURSOR_POINT   = 0x9,
	MOUSE_CURSOR_SPLITV  = 0xA,
	MOUSE_CURSOR_SPLITH  = 0xB,
	MOUSE_CURSOR_FORBID  = 0xC,
	MOUSE_CURSOR_UPARROW = 0xD,
	MOUSE_CURSOR_NONE    = 0xE,
	MOUSE_CURSOR_MAX     = 0xF
} S_MOUSE_CURSOR;

typedef enum {
	KEYCODE_NMLCKF	= 0x01,
	KEYCODE_CPLCKF	= 0x02,
	KEYCODE_LCTRLF	= 0x04,
	KEYCODE_RCTRLF	= 0x08,
	KEYCODE_LSHIFTF	= 0x10,
	KEYCODE_RSHIFTF	= 0x20,
	KEYCODE_LALTF	= 0x40,
	KEYCODE_RALTF	= 0x80,
	KEYCODE_SHIFTF	= (KEYCODE_LSHIFTF | KEYCODE_RSHIFTF),
	KEYCODE_CTRLF	= (KEYCODE_LCTRLF | KEYCODE_RCTRLF),
	KEYCODE_ALTF	= (KEYCODE_LALTF | KEYCODE_LALTF),
	KEYCODE_ALTGRF	= KEYCODE_RALTF
} S_KEYCODE_FLAG;

typedef enum {
/* Plain */
/*   0 */  S_KEYCODE_ESCAPE,
/*   1 */  S_KEYCODE_ONE,
/*   2 */  S_KEYCODE_TWO,
/*   3 */  S_KEYCODE_THREE,
/*   4 */  S_KEYCODE_FOUR,
/*   5 */  S_KEYCODE_FIVE,
/*   6 */  S_KEYCODE_SIX,
/*   7 */  S_KEYCODE_SEVEN,
/*   8 */  S_KEYCODE_EIGHT,
/*   9 */  S_KEYCODE_NINE,
/*  10 */  S_KEYCODE_ZERO,
/*  11 */  S_KEYCODE_MINUS,
/*  12 */  S_KEYCODE_EQUAL,
/*  13 */  S_KEYCODE_DELETE,
/*  14 */  S_KEYCODE_TAB,
/*  15 */  S_KEYCODE_q,
/*  16 */  S_KEYCODE_w,
/*  17 */  S_KEYCODE_e,
/*  18 */  S_KEYCODE_r,
/*  19 */  S_KEYCODE_t,
/*  20 */  S_KEYCODE_y,
/*  21 */  S_KEYCODE_u,
/*  22 */  S_KEYCODE_i,
/*  23 */  S_KEYCODE_o,
/*  24 */  S_KEYCODE_p,
/*  25 */  S_KEYCODE_BRACKETLEFT,
/*  26 */  S_KEYCODE_BRACKETRIGHT,
/*  27 */  S_KEYCODE_RETURN,
/*  28 */  S_KEYCODE_LEFTCONTROL,
/*  29 */  S_KEYCODE_a,
/*  30 */  S_KEYCODE_s,
/*  31 */  S_KEYCODE_d,
/*  32 */  S_KEYCODE_f,
/*  33 */  S_KEYCODE_g,
/*  34 */  S_KEYCODE_h,
/*  35 */  S_KEYCODE_j,
/*  36 */  S_KEYCODE_k,
/*  37 */  S_KEYCODE_l,
/*  38 */  S_KEYCODE_SEMICOLON,
/*  39 */  S_KEYCODE_APOSTROPHE,
/*  40 */  S_KEYCODE_GRAVE,
/*  41 */  S_KEYCODE_LEFTSHIFT,
/*  42 */  S_KEYCODE_BACKSLASH,
/*  43 */  S_KEYCODE_z,
/*  44 */  S_KEYCODE_x,
/*  45 */  S_KEYCODE_c,
/*  46 */  S_KEYCODE_v,
/*  47 */  S_KEYCODE_b,
/*  48 */  S_KEYCODE_n,
/*  49 */  S_KEYCODE_m,
/*  50 */  S_KEYCODE_COMMA,
/*  51 */  S_KEYCODE_PERIOD,
/*  52 */  S_KEYCODE_SLASH,
/*  53 */  S_KEYCODE_RIGHTSHIFT,
/*  54 */  S_KEYCODE_KP_MULTIPLY,
/*  55 */  S_KEYCODE_ALT,
/*  56 */  S_KEYCODE_SPACE,
/*  57 */  S_KEYCODE_CAPS_LOCK,
/*  58 */  S_KEYCODE_F1,
/*  59 */  S_KEYCODE_F2,
/*  60 */  S_KEYCODE_F3,
/*  61 */  S_KEYCODE_F4,
/*  62 */  S_KEYCODE_F5,
/*  63 */  S_KEYCODE_F6,
/*  64 */  S_KEYCODE_F7,
/*  65 */  S_KEYCODE_F8,
/*  66 */  S_KEYCODE_F9,
/*  67 */  S_KEYCODE_F10,
/*  68 */  S_KEYCODE_NUM_LOCK,
/*  69 */  S_KEYCODE_SCROLL_LOCK,
/*  70 */  S_KEYCODE_KP_7,
/*  71 */  S_KEYCODE_KP_8,
/*  72 */  S_KEYCODE_KP_9,
/*  73 */  S_KEYCODE_KP_SUBTRACT,
/*  74 */  S_KEYCODE_KP_4,
/*  75 */  S_KEYCODE_KP_5,
/*  76 */  S_KEYCODE_KP_6,
/*  77 */  S_KEYCODE_KP_ADD,
/*  78 */  S_KEYCODE_KP_1,
/*  79 */  S_KEYCODE_KP_2,
/*  80 */  S_KEYCODE_KP_3,
/*  81 */  S_KEYCODE_KP_0,
/*  82 */  S_KEYCODE_KP_PERIOD,
/*  83 */  S_KEYCODE_LAST_CONSOLE,
/*  84 */  S_KEYCODE_LESS,
/*  85 */  S_KEYCODE_F11,
/*  86 */  S_KEYCODE_F12,
/*  87 */  S_KEYCODE_KP_ENTER,
/*  88 */  S_KEYCODE_RIGHTCONTROL,
/*  89 */  S_KEYCODE_KP_DIVIDE,
/*  90 */  S_KEYCODE_VOIDSYMBOL,
/*  91 */  S_KEYCODE_ALTGR,
/*  92 */  S_KEYCODE_BREAK,
/*  93 */  S_KEYCODE_HOME,
/*  94 */  S_KEYCODE_UP,
/*  95 */  S_KEYCODE_PAGEUP,
/*  96 */  S_KEYCODE_LEFT,
/*  97 */  S_KEYCODE_RIGHT,
/*  98 */  S_KEYCODE_END,
/*  99 */  S_KEYCODE_DOWN,
/* 100 */  S_KEYCODE_PAGEDOWN,
/* 101 */  S_KEYCODE_INSERT,
/* 102 */  S_KEYCODE_REMOVE,
/* 103 */  S_KEYCODE_PAUSE,
/* Shift */
/* 104 */  S_KEYCODE_EXCLAM,
/* 105 */  S_KEYCODE_AT,
/* 106 */  S_KEYCODE_NUMBERSIGN,
/* 107 */  S_KEYCODE_DOLLAR,
/* 108 */  S_KEYCODE_PERCENT,
/* 109 */  S_KEYCODE_ASCIICIRCUM,
/* 110 */  S_KEYCODE_AMPERSAND,
/* 111 */  S_KEYCODE_ASTERISK,
/* 112 */  S_KEYCODE_PARENLEFT,
/* 113 */  S_KEYCODE_PARENRIGHT,
/* 114 */  S_KEYCODE_UNDERSCORE,
/* 115 */  S_KEYCODE_PLUS,
/* 116 */  S_KEYCODE_Q,
/* 117 */  S_KEYCODE_W,
/* 118 */  S_KEYCODE_E,
/* 119 */  S_KEYCODE_R,
/* 120 */  S_KEYCODE_T,
/* 121 */  S_KEYCODE_Y,
/* 122 */  S_KEYCODE_U,
/* 123 */  S_KEYCODE_I,
/* 124 */  S_KEYCODE_O,
/* 125 */  S_KEYCODE_P,
/* 126 */  S_KEYCODE_BRACELEFT,
/* 127 */  S_KEYCODE_BRACERIGHT,
/* 128 */  S_KEYCODE_A,
/* 129 */  S_KEYCODE_S,
/* 130 */  S_KEYCODE_D,
/* 131 */  S_KEYCODE_F,
/* 132 */  S_KEYCODE_G,
/* 133 */  S_KEYCODE_H,
/* 134 */  S_KEYCODE_J,
/* 135 */  S_KEYCODE_K,
/* 136 */  S_KEYCODE_L,
/* 137 */  S_KEYCODE_COLON,
/* 138 */  S_KEYCODE_QUOTEDBL,
/* 139 */  S_KEYCODE_ASCIITILDE,
/* 140 */  S_KEYCODE_BAR,
/* 141 */  S_KEYCODE_Z,
/* 142 */  S_KEYCODE_X,
/* 143 */  S_KEYCODE_C,
/* 144 */  S_KEYCODE_V,
/* 145 */  S_KEYCODE_B,
/* 146 */  S_KEYCODE_N,
/* 147 */  S_KEYCODE_M,
/* 148 */  S_KEYCODE_GREATER,
/* 149 */  S_KEYCODE_QUESTION,
/* 150 */  S_KEYCODE_F13,
/* 151 */  S_KEYCODE_F14,
/* 152 */  S_KEYCODE_F15,
/* 153 */  S_KEYCODE_F16,
/* 154 */  S_KEYCODE_F17,
/* 155 */  S_KEYCODE_F18,
/* 156 */  S_KEYCODE_F19,
/* 157 */  S_KEYCODE_F20,
/* 158 */  S_KEYCODE_F21,
/* 159 */  S_KEYCODE_F22,
/* 160 */  S_KEYCODE_SHOW_MEMORY,
/* 161 */  S_KEYCODE_F23,
/* 162 */  S_KEYCODE_F24,
/* 163 */  S_KEYCODE_SCROLL_BACKWARD,
/* 164 */  S_KEYCODE_SCROLL_FORWARD,
/* ALTGR */
/* 165 */  S_KEYCODE_CURRENCY,
/* 166 */  S_KEYCODE_CENT,
/* 167 */  S_KEYCODE_HEX_C,
/* 168 */  S_KEYCODE_HEX_A,
/* 169 */  S_KEYCODE_SHOW_REGISTERS,
/* 170 */  S_KEYCODE_HEX_7,
/* 171 */  S_KEYCODE_HEX_8,
/* 172 */  S_KEYCODE_HEX_9,
/* 173 */  S_KEYCODE_HEX_D,
/* 174 */  S_KEYCODE_HEX_4,
/* 175 */  S_KEYCODE_HEX_5,
/* 176 */  S_KEYCODE_HEX_6,
/* 177 */  S_KEYCODE_HEX_E,
/* 178 */  S_KEYCODE_HEX_1,
/* 179 */  S_KEYCODE_HEX_2,
/* 180 */  S_KEYCODE_HEX_3,
/* 181 */  S_KEYCODE_HEX_0,
/* 182 */  S_KEYCODE_HEX_F,
/* 183 */  S_KEYCODE_HEX_B,
/* 184 */  S_KEYCODE_USER0,
/* 185 */  S_KEYCODE_USER1,
/* 186 */  S_KEYCODE_USER2,
/* 187 */  S_KEYCODE_USER3,
/* 188 */  S_KEYCODE_USER4,
/* 189 */  S_KEYCODE_USER5,
/* 190 */  S_KEYCODE_USER6,
/* 191 */  S_KEYCODE_USER7,
/* 192 */  S_KEYCODE_USER8,
/* 193 */  S_KEYCODE_USER9,
/* 194 */  S_KEYCODE_USER10,
/* 195 */  S_KEYCODE_USER11,
/* 196 */  S_KEYCODE_USER12,
/* 197 */  S_KEYCODE_USER13,
/* 198 */  S_KEYCODE_USER14,
/* 199 */  S_KEYCODE_NOCODE,
/* 200 */  S_KEYCODE_CODES
} S_KEYCODE_CODE;

typedef enum {
	MOUSE_RIGHTBUTTON    = 0x00000001,
	MOUSE_MIDDLEBUTTON   = 0x00000002,
	MOUSE_LEFTBUTTON     = 0x00000004,
	MOUSE_FOURTHBUTTON   = 0x00000008,
	MOUSE_FIFTHBUTTON    = 0x00000010,
	MOUSE_SIXTHBUTTON    = 0x00000020,
	MOUSE_RESETBUTTON    = 0x00000040,
	MOUSE_WHEELXBUTTON_U = 0x00000080,
	MOUSE_WHEELXBUTTON_D = 0x00000100,
	MOUSE_WHEELYBUTTON_U = 0x00000200,
	MOUSE_WHEELYBUTTON_D = 0x00000400,

	MOUSE_USER0BUTTON    = 0x00000800,
	MOUSE_USER1BUTTON    = 0x00001000,
	MOUSE_USER2BUTTON    = 0x00002000,
	MOUSE_USER3BUTTON    = 0x00004000,
	MOUSE_USER4BUTTON    = 0x00008000,
	MOUSE_USER5BUTTON    = 0x00010000,
	MOUSE_USER6BUTTON    = 0x00020000,
	MOUSE_USER7BUTTON    = 0x00040000,
	MOUSE_USER8BUTTON    = 0x00080000,
	MOUSE_USER9BUTTON    = 0x00100000,
	MOUSE_USER10BUTTON   = 0x00200000,

	MOUSE_NONEBUTTON     = 0x00400000
} S_MOUSECODE_CODE;

typedef enum {
        QUIT_EVENT     = 0x00001,
	KEYBD_EVENT    = 0x00002,
	KEYBD_RELEASED = 0x00004,
	KEYBD_PRESSED  = 0x00008,
	MOUSE_EVENT    = 0x00010,
	MOUSE_OVER     = 0x00020,
	MOUSE_RELEASED = 0x00040,
	MOUSE_PRESSED  = 0x00080,
	MOUSE_CLICKED  = 0x00100,
	MOUSE_HINT     = 0x00200,
	MOUSE_HINT2    = 0x00400,
        EXPOSE_EVENT   = 0x00800,
        EXPOSE_CHNGX   = 0x01000,
        EXPOSE_CHNGY   = 0x02000,
        EXPOSE_CHNGW   = 0x04000,
        EXPOSE_CHNGH   = 0X08000,
        EXPOSE_CHNGF   = 0X10000,
        DESKTOP_EVENT  = 0x20000,
        TIMER_EVENT    = 0x40000,
	EVENT_MASK     = (QUIT_EVENT | EXPOSE_EVENT | KEYBD_EVENT | MOUSE_EVENT | DESKTOP_EVENT | TIMER_EVENT)
} S_EVENT;

typedef enum {
	MOUSE_HANDLER = 0x1,
	KEYBD_HANDLER = 0x2
} S_HANDLER;

typedef struct s_config_var_s {
	char *name;
	char *value;
} s_config_var_t;

typedef struct s_config_cat_s {
	char *name;
	s_list_t *variable;
} s_config_cat_t;

typedef struct s_config_s {
	s_list_t *category;
} s_config_t;

/** @addtogroup rect */
/*@{*/

/** rect struct
  */
struct s_rect_s {
	/** x coordinate */
	int x;
	/** y coordinate */
	int y;
	/** width */
	int w;
	/** height */
	int h;
};

/*@}*/

typedef struct s_font_s {
	int yMin;
	int yMax;
	char *str;
	int size;
	int ascender;
	int descender;
	int height;
	int max_advance;
	unsigned int rgb;
	s_image_t *img;
	s_font_ft_t *ft;
} s_font_t;

typedef struct s_mouse_s {
	int x;
	int y;
	S_MOUSECODE_CODE b;
	int clicks;		  /* click count	       */
	S_MOUSECODE_CODE buttons; /* buttons bitwise ORed      */
	long long time;		  /* event time	 	       */
	int px;			  /* prev. pressed coor.       */
	int py;
	int pb;			  /* prev. pressed button      */
	S_MOUSECODE_CODE pbuttons;/* prev. pressed buttons     */
	long long ctime;	  /* prev. clicked time	       */
	S_MOUSE_CURSOR cursor;
} s_mouse_t;

typedef struct s_keybd_s {
	int ascii;
	int button;
	int keycode;
	int scancode;
	int state[S_KEYCODE_CODES];
	S_KEYCODE_FLAG flag;
} s_keybd_t;

typedef struct s_expose_s {
	int change;
	s_rect_t *rect;
} s_expose_t;

typedef struct s_desktop_client_s {
	int id;
	int pri;
	int title_l;
	char *title;
} s_desktop_client_t;

typedef struct s_dekstop_s {
	s_list_t *clients;
} s_desktop_t;

/** event struct */
typedef struct s_event_s {
	/** event type */
	S_EVENT type;
	/** mouse event */
	s_mouse_t *mouse;
	/** keyboard event */
	s_keybd_t *keybd;
	/** expose event */
	s_expose_t *expose;
	/** desktop event */
	s_desktop_t *desktop;
	/** timer event */
	s_timer_t *timer;
} s_event_t;

/** event queue */
typedef struct s_eventq {
	/** dispatcher thread`s id */
	s_thread_t *tid;
	/** queue list holds s_event_t */
	s_list_t *queue;
	/** conditional variable to suspend dispatcher thread */
        s_thread_cond_t *cond;
        /** queue mutex */
	s_thread_mutex_t *mut;
} s_eventq_t;

typedef struct s_handler_keybd_s {
	int flag;
	int button;
	void (*p) (s_window_t *, s_event_t *, s_handler_t *);
	void (*r) (s_window_t *, s_event_t *, s_handler_t *);
} s_handler_keybd_t;

typedef struct s_handler_mouse_s {
	int x;
	int y;
	int w;
	int h;
	int button;
	void (*p) (s_window_t *, s_event_t *, s_handler_t *);	/* button pressed   		                                */
	void (*r) (s_window_t *, s_event_t *, s_handler_t *);	/* button released,          			                */
	void (*c) (s_window_t *, s_event_t *, s_handler_t *);	/* button clicked,                     			        */
	void (*o) (s_window_t *, s_event_t *, s_handler_t *);	/* on over 				                        */
	void (*ho) (s_window_t *, s_event_t *, s_handler_t *);	/* on over && hint,                                             */
								/* on over, but mouse button is still pressed		        */
	void (*hr) (s_window_t *, s_event_t *, s_handler_t *);	/* button realesed && hint	                                */
								/* mouse button released, but the prev. press was not on us	*/
	void (*oh) (s_window_t *, s_event_t *, s_handler_t *);	/* button over && hint2		                                */
								/* not on over, but was on over                                 */
	void (*hoh) (s_window_t *, s_event_t *, s_handler_t *); /* button over && hint && hint2	                                */
								/* not on over, but was on over. and button is still pressed    */
	void (*rh) (s_window_t *, s_event_t *, s_handler_t *);	/* button realesed && hint2	                                */
								/* mouse button released outside, but the prev. press was on us */
			                                        /* wheel buttons has no realesed event                          */
	S_EVENT hstate;
} s_handler_mouse_t;
                                     
struct s_handler_s {
	S_HANDLER type;
	s_handler_mouse_t mouse;
	s_handler_keybd_t keybd;
	void *user_data;
};

typedef struct s_handlers_s {
	s_list_t *list;
	s_thread_mutex_t *mut;
} s_handlers_t;

typedef struct s_childs_s {
	s_list_t *list;
	s_thread_mutex_t *mut;
} s_childs_t;

typedef struct s_surface_s {
       	S_SURFACE_MODE mode;

	int bytesperpixel;		/* We need to get them from server, to be able to draw right	*/
	int bitsperpixel;		/* thing on linear buffer.					*/

	int colors;			/* These are for future compability				*/
	int blueoffset;
	int greenoffset;
	int redoffset;
	int bluelength;
	int greenlength;
	int redlength;

	int width;			/* These are the real sizes of our buffer			*/
	int height;

        char *vbuf;			/* The buffer that holds clients window.			*/

	s_rect_t buf;			/* This is our bufs virtual part				*/
	s_rect_t win;

	char *linear_buf;		/* mapped shared buffer						*/
	int linear_buf_width;		/* See s_client_surface_linear()				*/
	int linear_buf_pitch;
	int linear_buf_height;
	unsigned int linear_mem_base;	/* We have to get the properties and the address of linear	*/
	unsigned int linear_mem_size;	/* buffer to be able to draw on it.				*/

        int *id;			/* ugly hack, window->surface->id = &(window->client->id);	*/
	int shm_mid;			/* shared screen matrix						*/
	unsigned char *matrix;

        int shm_sid;			/* shared linear buffer						*/
	int need_expose;
        s_window_t *window;		/* ugly hack, window->surface->window = window;			*/
} s_surface_t;

typedef struct s_object_s {
	s_list_t *childs;		/* object`s childs	*/
	s_thread_mutex_t *mut;		/* object`s root mutex	*/
	s_surface_t *surface;		/* object`s surface	*/
	struct s_object_s *parent;	/* object`s parent	*/
} s_object_t;

typedef struct s_client_s {
	int id;
	int pri;
	char *title;

	char *device;

	int resizeable;
	int alwaysontop;

	void (*atevent) (s_window_t *, s_event_t *);
	void (*atexit) (s_window_t *);

	void *user_data;
} s_client_t;

struct s_window_s {
	int running;
	int soc;
	int wsoc;
	S_WINDOW type;

	s_client_t *client;
	s_surface_t *surface;

	s_eventq_t *eventq;
	s_timers_t *timers;
	s_childs_t *childs;
	s_pollfds_t *pollfds;
	s_handlers_t *handlers;

	s_thread_t *tid;
	s_window_t *parent;

	s_event_t *event;
	s_thread_mutex_t *socket_mutex;
};

typedef struct s_single_app_s {
	int (*fonk) (int argc, char *argv[]);
	int argc;
	char *argv[10];
} s_single_app_t;

/* alloc.c */
void * s_malloc (unsigned int size);
void * s_calloc (unsigned int nmemb, unsigned int size);
void * s_realloc (void *ptr, unsigned int size);
void s_free (void *ptr);

/* child.c */
int s_child_add (s_window_t *window, s_window_t *child);
int s_child_del (s_window_t *window, s_window_t *child);
int s_childs_init (s_window_t *window);
int s_childs_uninit (s_window_t *window);

/* client.c */
int s_client_init (s_window_t **window);
void s_client_uninit (s_window_t *window);
void s_client_exit (s_window_t *window);
void s_client_quit (s_window_t *window);
int s_client_wakeup (s_window_t *window);
int s_client_child_find (s_window_t *parent, s_window_t *window, s_event_t *event);
void * s_client_loop_event (void *arg);
void * s_client_loop (void *arg);
void * s_client_main (void *arg);
void s_client_atevent (s_window_t *window, void (*f) (s_window_t *, s_event_t *));
void s_client_atexit (s_window_t *window, void (*f) (s_window_t *));

/* config.c */
char * s_config_strip (char *buf);
int s_config_parse (s_config_t *cfg, char *name);
int s_config_init (s_config_t **cfg);
int s_config_category_init (s_config_cat_t **cat, char *name);
int s_config_variable_init (s_config_var_t **var, char *name, char *value);
int s_config_variable_uninit (s_config_var_t *var);
int s_config_category_uninit (s_config_cat_t *cat);
int s_config_uninit (s_config_t *cfg);

/* debugf.c */
void s_debug_debugf (unsigned short flags, char *file, int line, char *func, char *fmt, ...);

/* driver.c */
void bpp_setpixel (s_surface_t *surface, int x, int y, int c);
void bpp_setpixel_o (s_surface_t *surface, int id, int x, int y, int c);
int bpp_getpixel (s_surface_t *surface, int x, int y);
int bpp_getpixel_o (s_surface_t *surface, int id,  int x, int y);
void bpp_hline (s_surface_t *surface, int x1, int y, int x2, int c);
void bpp_hline_o (s_surface_t *surface, int id,  int x1, int y, int x2, int c);
void bpp_vline (s_surface_t *surface, int x, int y1, int y2, int c);
void bpp_vline_o (s_surface_t *surface, int id,  int x, int y1, int y2, int c);
void bpp_fillbox (s_surface_t *surface, int x, int y, int w, int h, int c);
void bpp_fillbox_o (s_surface_t *surface, int id,  int x, int y, int w, int h, int c);
void bpp_putbox (s_surface_t *surface, int x, int y, int w, int h, char *sp, int bw);
void bpp_putbox_mask (s_surface_t *surface, int x, int y, int w, int h, char *sp, unsigned char *sm, int bw);
void bpp_getbox (s_surface_t *surface, int x, int y, int w, int h, char *dp);
void bpp_putbox_o (s_surface_t *surface, int id,  int x, int y, int w, int h, char *sp, int bw);
void bpp_putbox_mask_o (s_surface_t *surface, int id,  int x, int y, int w, int h, char *sp, unsigned char *sm, int bw);
void bpp_getbox_o (s_surface_t *surface, int id,  int x, int y, int w, int h, char *dp);

/* event.c */
int s_event_mouse_state (s_window_t *window, s_event_t *event, s_handler_mouse_t *mouse, int over);
void s_event_parse_mouse (s_window_t *window, s_event_t *event);
int s_event_parse_keybd (s_window_t *window, s_event_t *event);
int s_event_parse_expos (s_window_t *window, s_event_t *event);
int s_event_parse_timer (s_window_t *window, s_event_t *event);
int s_event_changed (s_window_t *window);
int s_event_init (s_event_t **event);
int s_event_uninit (s_event_t *event);

/* eventq.c */
int s_eventq_init (s_window_t *window);
int s_eventq_uninit (s_window_t *window);
int s_eventq_add (s_window_t *window, s_event_t *event);
int s_eventq_get (s_window_t *window, s_event_t **event);
int s_eventq_wait (s_window_t *window, s_event_t **event);

/* font.c */
int s_font_init (s_font_t **font, char *name);
int s_font_uninit (s_font_t *font);
int s_font_set_size (s_font_t *font, int size);
int s_font_set_str (s_font_t *font, char *str);
int s_font_set_rgb (s_font_t *font, int r, int g, int b);
int s_font_get_glyph (s_font_t *font);

/* gettime.c */
long long s_gettimeofday (void);

/* grlib.c */
int s_rgbcolor (s_surface_t *surface, int r, int g, int b);
void s_colorrgb (s_surface_t *surface, int c, int *r, int *g, int *b);
void s_setpixel (s_surface_t *surface, int x, int y, int c);
void s_setpixelrgb (s_surface_t *surface, int x, int y, int r, int g, int b);
void s_setpixelrgba (s_surface_t *surface, int x, int y, int r, int g, int b, int a);
int s_getpixel (s_surface_t *surface, int x, int y);
void s_getpixelrgb (s_surface_t *surface, int x, int y, int *r, int *g, int *b);
void s_hline (s_surface_t *surface, int x1, int y, int x2, int c);
void s_vline (s_surface_t *surface, int x, int y1, int y2, int c);
void s_fillbox (s_surface_t *surface, int x, int y, int w, int h, int c);
void s_putbox (s_surface_t *surface, int x, int y, int w, int h, char *sp);
void s_putboxmask (s_surface_t *surface, int x, int y, int w, int h, char *sp, unsigned char *sm);
void s_putboxalpha (s_surface_t *surface, int x, int y, int w, int h, char *sp, unsigned char *sm);
void s_putboxrgba (s_surface_t *surfacem, int x, int y, int w, int h, unsigned int *rgba);
void s_getbox (s_surface_t *surface, int x, int y, int w, int h, char *dp);
void s_putboxpart (s_surface_t *surface, int x, int y, int w, int h, int bw, int bh, char *sp, int xo, int yo);
void s_putboxpartmask (s_surface_t *surface, int x, int y, int w, int h, int bw, int bh, char *sp, unsigned char *sm, int xo, int yo);
void s_putboxpartrgba (s_surface_t *surface, int x, int y, int w, int h, int bw, int bh, unsigned int *rgba, int xo, int yo);
void s_putboxpartalpha (s_surface_t *surface, int x, int y, int w, int h, int bw, int bh, char *sp, unsigned char *sm, int xo, int yo);
void s_copybox (s_surface_t *surface, int x1, int y1, int w, int h, int x2, int y2);
void s_getsurfacevirtual (s_surface_t *s, int w, int h, int bitspp, char *vbuf);
int s_copybuffer (char *sb, int sbitspp, char **db, int dbitspp, int w, int h);
void s_scalebox (s_surface_t *surface, int w1, int h1, void *_dp1, int w2, int h2, void *_dp2);

/* handler.c */
int s_handler_init (s_handler_t **handler);
int s_handler_uninit (s_handler_t *handler);
int s_handler_add (s_window_t *window, s_handler_t *handler);
int s_handler_del (s_window_t *window, s_handler_t *handler);
int s_handlers_init (s_window_t *window);
int s_handlers_uninit (s_window_t *window);

/** @defgroup image image struct, api
  * @brief detailed description
  *
  * @example
  *
  * for further information look in demo/img/
  *
  * @code
  * int x;
  * int y;
  * s_image_t *img;
  * s_surface_t *surface;
  * char *file = "file.img";
  *
  * // initialize img struct
  * s_image_init(&img);
  *
  * // parse and load file to img struct
  * s_image_img(file, img);
  *
  * // paste img to the surface
  * s_putboxrgba(surface, x + img->x, y + img->y, img->w, img->h, img->rgba);
  *
  * // or
  * s_image_get_buf(surface, img);
  * s_putbox(surface, x + img->x, y + img->y, img->w, img->h, img->buf);
  *
  * // or
  * s_image_get_buf(surface, img);
  * s_image_get_mat(surface, img);
  * s_putboxmask(surface, x + img->x, y + img->y, img->w, img->h, img->buf, img->mat);
  *
  * // or
  * s_image_get_buf(surface, img);
  * s_image_get_mat(surface, img);
  * s_putboxalpha(surface, x + img->x, y + img->y, img->w, img->h, img->buf, img->mat);
  *
  * // when finished, uninitialize img struct
  * s_image_uninit(img);
  * @endcode
  */

/** @addtogroup image */
/*@{*/

/** image struct
  */
struct s_image_s {
	/** layer x */
	int x;
	/** layer y */
	int y;
	/** layer w */
	int w;
	/** layer h */
	int h;
	/** image buffer */
	char *buf;
	/** holds only anti-alias (alpha) values */
	unsigned char *mat;
	/** image handler area */
	s_rect_t *handler;
	/** show delay (microseconds) of the layer. */
	unsigned int delay;
	/** rgba data of image RED | GREEN | BLUE | ALPHA*/
	unsigned int *rgba;
	/** layers list holds s_image_s */
	s_list_t *layers;
};

/* image.c */

/** @brief convert a hex str to int
  *
  * @param *str - pointer to a buffer of chars with at least length 2
  * @returns the value.
  */
int s_image_hex2int (char *str);

/** @brief generates alpha values for given image
  *
  * @param *img - the image
  * @returns 0 on success, 1 on error.
  */
int s_image_get_mat (s_image_t *img);

/** @brief generates image buffer using surface, for given image
  *
  * @param *surface - pointer to a valid surface.
  * @param *img     - the image
  * @returns 0 on success, 1 on error.
  */
int s_image_get_buf (s_surface_t *surface, s_image_t *img);

/** @brief calculates handler for given image
  *
  * @param *img - the image
  * @returns 0 on success, 1 on error.
  */
void s_image_get_handler (s_image_t *img);

/** @brief initializes image struct
  *
  * @param **img - address of the pointer to image struct
  * @returns 0 on success, 1 on error.
  */
int s_image_init (s_image_t **img);

/** @brief frees memory used for buf, and sets it to NULL.
  *
  * @param *img - the image
  * @returns 0 on success, 1 on error.
  */
void s_image_free_buf (s_image_t *img);

/** @brief frees memory used for mat, and sets it to NULL.
  *
  * @param *img - the image
  * @returns 0 on success, 1 on error.
  */
void s_image_free_mat (s_image_t *img);

/** @brief frees memory used for rgb, and sets it to NULL.
  *
  * @param *img - the image
  * @returns 0 on success, 1 on error.
  */
void s_image_free_rgba (s_image_t *img);

/** @brief frees memory used for handler, and sets it to NULL.
  *
  * @param *img - the image
  * @returns 0 on success, 1 on error.
  */
void s_image_free_handler (s_image_t *img);

/** @brief uninitializes image struct
  *
  * @param *img - the image
  * @returns 0 on success, 1 on error.
  */
int s_image_uninit (s_image_t *img);

/** @brief initializes image struct
  *
  * @param **lyr - address of the pointer to image struct
  * @returns 0 on success, 1 on error.
  */
int s_image_layer_init (s_image_t **lyr);

/** @brief uninitializes image struct
  *
  * @param *lyr - the layer
  * @returns 0 on success, 1 on error.
  */
int s_image_layer_uninit (s_image_t *lyr);

/** @brief initializes layer list for given image
  *
  * @param *img - the image
  * @returns 0 on success, 1 on error.
  */
int s_image_layers_init (s_image_t *img);

/** @brief uninitializes layer list for given image
  *
  * @param *img - the image
  * @returns 0 on success, 1 on error.
  */
int s_image_layers_uninit (s_image_t *img);

/** @brief fills image struct by calling the approtiated function,
  *        if the file is a supported format
  *
  * @param *file - exact path to the file
  * @param *img  - the image
  * @returns 0 on success, 1 on error.
  */
int s_image_img (char *file, s_image_t *img);

/* image_gif.c */

/** @brief checks if the file is a gif
  *
  * @param *file - exact path to the file
  * @returns 0 on success, 1 on error.
  */
int s_image_gif_is (char *file);

/** @brief fills the image struct
  *
  * @param *file - exact path to the file
  * @param *img  - the image
  * @returns 0 on success, 1 on error.
  */
int s_image_gif (char *file, s_image_t *img);

/* image_png.c */

/** @brief checks if the file is a png
  *
  * @param *file - exact path to the file
  * @returns 0 on success, 1 on error.
  */
int s_image_png_is (char *file);

/** @brief fills the image struct
  *
  * @param *file - exact path to the file
  * @param *img  - the image
  * @returns 0 on success, 1 on error.
  */
int s_image_png (char *file, s_image_t *img);

/* image_xpm.c */

/** @brief checks if the file is a xpm
  *
  * @param *file - exact path to the file
  * @returns 0 on success, 1 on error.
  */
int s_image_xpm_is (char *file);

/** @brief fills the image struct
  *
  * @param *file - exact path to the file
  * @param *img  - the image
  * @returns 0 on success, 1 on error.
  */
int s_image_xpm (char *file, s_image_t *img);

/*@}*/

/** @defgroup list list struct, api
  * @brief detailed description
  *
  * @example
  * typical usage of list api;
  *
  * @code
  * int compare_function (void *p1, void *p2)
  * {
  * 	int v1 = (int) p1;
  * 	int v2 = (int) p2;
  *	if (v1 < v2) { return -1; }
  * 	if (v1 > v2) { return  1; }
  * 	return 0;
  * }
  *
  * int pos;
  * void *item;
  * s_list_t *list;
  *
  * // initialize the list struct
  * s_list_init(&list);
  *
  * // add an item to the end
  * item = (void *) 0;
  * s_list_add(list, item, -1);
  *
  * // add an item to the beginning
  * item = (void *) 1;
  * s_list_add(list, item, 0);
  *
  * // add an item to position 1
  * item = (void *) 2;
  * s_list_add(list, item, 1);
  *
  * // add an item to the end
  * item = (void *) 3;
  * s_list_add(list, item, -1);
  *
  * // find the the item with compare function
  * item = (void *) 2;
  * item = s_list_find(list, item, compare_function);
  *
  * // get the items, and print them
  * // output: 1 2 0 3
  * for (pos = 0; !s_list_eol(list, pos); pos++) {
  *	item = (void *) s_list_get(list, pos);
  * 	printf("%d ", (int) item;
  * }
  *
  * // remove items
  * while (!s_list_eol(list, 0)) {
  *	s_list_remove(list, 0);
  * }
  *
  * // uninitialize the list
  * s_list_uninit(list);
  * @endcode
  */

/** @addtogroup list */
/*@{*/

/** list node struct
  */
struct s_list_node_s {
	/** next element */
        void *next;
        /** element */
        void *element;
};

/** list struct
  */
struct s_list_s {
	/** number of elements */
        int nb_elt;
        /** list node */
        s_list_node_t *node;
};

/* list.c */

/** @brief initializes the list struct
  *
  * @param **li - address of the pointer to list struct
  * @returns 0 on success, 1 on error
  */
int s_list_init (s_list_t **li);

/** @brief uninitializes the list struct
  *
  * @param *li - the list
  * @returns 0 on success, 1 on error
  */
int s_list_uninit (s_list_t *li);

/** @brief query if i'nth element exists
  *
  * @param *li - the list
  * @param i   - position
  * @returns 0 on success, 1 on error
  */
int s_list_eol (s_list_t *li, int i);

/** @brief return the element at position
  *
  * @param *li - the list
  * @param pos - position
  * @returns pointer to element on success, NULL on error.
  */
void * s_list_get (s_list_t *li, int pos);

/** @brief removes the element at position
  *
  * @param *li - the list
  * @param pos - position
  * @returns - on success, 1 on error
  */
int s_list_remove (s_list_t *li, int pos);

/** @brief adds the element at position
  *
  * @param *li - the list
  * @param *el - element
  * @param pos - position (-1 means the end)
  * @returns - on success, 1 on error
  */
int s_list_add (s_list_t *li, void *el, int pos);

/** @brief search the node at list, with the given compare function
  *
  * @param *list    - the list
  * @param *node    - node to be matched
  * @param cmp_func - compare function. compare function must return -1, 0, 1
                      for less than, equal to, and greater than
  * @returns - on success, 1 on error
  */
void * s_list_find (s_list_t *list, void *node, int (*cmp_func) (void *, void *));

/** @brief return the position of node
  *
  * @param *list - the list
  * @param *node - element
  * @returns - position on success, -1 on error
  */
int s_list_get_pos (s_list_t *list, void *node);

/*@}*/

/* object.c */
int s_object_update_to_surface (s_object_t *object, s_surface_t *surface, s_rect_t *coor);
int s_object_update (s_object_t *object, s_rect_t *coor);
int s_object_move (s_object_t *object, int x, int y, int w, int h);
int s_object_hide (s_object_t *object);
int s_object_show (s_object_t *object);
int s_object_init (s_window_t *window, s_object_t **object, int w, int h, s_object_t *parent);
int s_object_uninit (s_object_t *object);

/** @defgroup pollfd pollfd struct, api
  * @brief detailed description
  *
  * @example
  * typical usage of pollfd api;
  *
  * @code
  * int pollfd_in_cb (s_window_t *window, s_pollfd_t *pfd)
  * {
  * 	// do what ever you want with your fd
  * 	// return -1 on error, 0 on success
  *	return 0;
  * }
  *
  * int pollfd_in_err (s_window_t *window, s_pollfd_t *pfd)
  * {
  * 	// will be called if something bad happens on poll call
  *  	// return -1 to stop client, otherwise return 0
  *	return 0;
  * }
  *
  * int pollfd_in_close (s_window_t *window, s_pollfd_t *pfd)
  * {
  * 	// will be called while uniting the window
  *	return 0;
  * }
  *
  * s_pollfd_t *pfd;
  *
  * // initialize pfd struct
  * s_pollfd_init(&pfd);
  *
  * // set fd to poll, and callback functions
  * pfd->fd = fd;
  * pfd->pf_in = pollfd_in_cb;
  * pfd->pf_err = pollfd_err_cb;
  * pfd->pf_close = pollfd_close_cb;
  *
  * // add pfd to the window
  * s_pollfd_add(window, pfd);
  *
  * // del pfd from window
  * s_pollfd_del(window, pfd);
  *
  * // uninitialize pfd struct
  * s_pollfd_uninit(pfd);
  * @endcode
  */

/** @addtogroup pollfd */
/*@{*/

/** polfd struct
  */
struct s_pollfd_s {
	/** fd to poll */
	int fd;
	/** in data call back function */
	int (*pf_in) (s_window_t *, int);
	/** error call back function */
	int (*pf_err) (s_window_t *, int);
	/** closing call back function */
	int (*pf_close) (s_window_t *, int);
};

/** polfds struct
  */
struct s_pollfds_s {
	/** list of fds */
	s_list_t *list;
	/** list mutex */
	s_thread_mutex_t *mut;
};

/* pollfd.c */

/** @brief initialize the pollfd struct.
  *
  * @param **pfd - address of the pollfd pointer.
  * @returns 0 on success, 1 on error.
  *
  */
int s_pollfd_init (s_pollfd_t **pfd);

/** @brief uninitialize the pollfd struct
  *
  * @param *pfd - the pollfd
  * @returns 0 on success, 1 on error.
  */
int s_pollfd_uninit (s_pollfd_t *pfd);

/** @brief finds the pollfd struct that is associated with fd
  *
  * @param window - the window that holds pollfd
  * @param fd     - file descriptor to find
  * @returns pointer to pollfd on success, NULL on error.
  */
s_pollfd_t * s_pollfd_find (s_window_t *window, int fd);

/** @brief adds a pollfd to windows` pollfds list
  *
  * @param *window - window to attach the pollfd
  * @param *pfd    - the pollfd
  * @returns 0 on success, 1 on error.
  */
int s_pollfd_add (s_window_t *window, s_pollfd_t *pfd);

/** @brief deletes a pollfd from windows` pollfds list
  *
  * @param *window - window that holds the pollfd
  * @param *pfd    - the pollfd
  * @returns 0 on success, 1 on error.
  */
int s_pollfd_del (s_window_t *window, s_pollfd_t *pfd);

/** @brief initialize the pollfds struct for given window
  *
  * @param *window - window
  * @returns 0 on success, 1 on error.
  */
int s_pollfds_init (s_window_t *window);

/** @brief uninitialize the pollfds struct for given window
  *
  * @param *window - window
  * @returns 0 on success, 1 on error.
  */
int s_pollfds_uninit (s_window_t *window);

/*@}*/

/** @defgroup rect rect struct, api
  * @brief detailed description
  *
  * @example
  * typical usage of rect api;
  *
  * @code
  *
  * {
  *	s_rect_t r
  *	s_rect_t rect r1 = (s_rect_t) {10, 10, 20, 20};
  *	s_rect_t rect r2 = (s_rect_t) {15, 15, 20, 20};
  *	if (s_rect_intersect(&r1, &r2, &r)) {
  *		// no intersection
  *	}
  *	// intersectig region is r
  * }
  *
  * {
  *	s_list_t *diff;
  *	s_rect_t rect r1 = (s_rect_t) {10, 10, 20, 20};
  *	s_rect_t rect r2 = (s_rect_t) {15, 15, 20, 20};
  *
  *	s_list_init(&diff);
  *	if (s_rect_difference(&r1, &r2, diff) < 0) {
  *		// will not happen
  *	}
  *
  *	// walk through the difference list
  *	for (pos = 0; !s_list_eol(list, pos); pos++) {
  *		s_rect_t *rtmp = (s_rect_t *) s_list_get(list, pos);
  *		// do what ever you want with the rectangle
  *		s_list_remove(list, pos);
  *		s_free(rtmp);
  *	}
  *
  *	s_list_uninit(list);
  * }
  *
  * @endcode
  */

/** @addtogroup rect */
/*@{*/

/* rect.c */

/** @brief calculates the intersection area of r1 and r2.
  *
  * @param *r1 - pointer to a rect
  * @param *r2 - pointer to a rect
  * @param *r  - on success r will be the intersection area
  * @returns 0 on success, 1 on error.
  */
int s_rect_intersect (s_rect_t *r1, s_rect_t *r2, s_rect_t *r);

/** @brief performs clip on rectangle (x, y, w, h).
  *        clip region is the surface's virtual buffer area
  *
  * @param *surface - surface
  * @param x        - x
  * @param y        - y
  * @param w        - w
  * @param h        - h
  * @param *coor    - on success coor will be the clipped area
  * @returns 0 on success, 1 on error.
  */
int s_rect_clip_virtual (s_surface_t *surface, int x, int y, int w, int h, s_rect_t *coor);

/** @brief performs clip on rectangle (x, y, w, h).
  *        clip region is the surface's virtual buffer area
  *
  * @param *surface - surface
  * @param x        - x
  * @param y        - y
  * @param w        - w
  * @param h        - h
  * @param *coor    - on success coor will be the clipped area
  * @returns 0 on success, 1 on error.
  */
int s_rect_clip_real (s_surface_t *surface, int x, int y, int w, int h, s_rect_t *coor);

/** @brief adds the rectangle (x, y, w, h) to the list
  *
  * @param *list - list to add
  * @param x     - x
  * @param y     - y
  * @param w     - w
  * @param h     - h
  * @returns 0 on success, 1 on error.
  */
int s_rect_difference_add (s_list_t *list, int x, int y, int w, int h);

/** @brief adds the difference of r1 form r0 to the list
  *
  * @param *r1   - rectangle
  * @param *r0   - rectangle
  * @param *list - list
  * @returns number of differences on success, -1 on error.
  */
int s_rect_difference (s_rect_t *r1, s_rect_t *r0, s_list_t *list);

/*@}*/

/* socket.c */
int s_socket_request_new (s_window_t *window, int soc);
int s_socket_request_display (s_window_t *window, int soc);
int s_socket_request_configure (s_window_t *window, int soc, S_WINDOW form);
int s_socket_request_desktop (s_window_t *window, int soc, int id);
int s_socket_request_expose (s_window_t *window, int soc, s_rect_t *coor);
int s_socket_request_stream (s_window_t *window, int soc, s_rect_t *coor);
int s_socket_request_cursor (s_window_t *window, int soc, S_MOUSE_CURSOR cursor);
int s_socket_request (s_window_t *window, S_SOC_DATA req, ...);
int s_socket_listen_event (s_window_t *window, int soc);
int s_socket_listen_expose (s_window_t *window, int soc);
int s_socket_listen_desktop (s_window_t *window, int soc);
int s_socket_listen_parse (s_window_t *window, int soc);
int s_socket_listen_wait (s_window_t *window, int timeout);
int s_socket_uninit (s_window_t *window, int s);
int s_socket_in_f (s_window_t *window, int s);
int s_socket_ierr_f (s_window_t *window, int s);
int s_socket_inw_f (s_window_t *window, int s);
int s_socket_ierrw_f (s_window_t *window, int s);
int s_socket_closew_f (s_window_t *window, int s);	
int s_socket_init_uds (s_window_t *window);
int s_socket_init_tcp (s_window_t *window);
int s_socket_init_wakeup (s_window_t *window);
int s_socket_init (s_window_t *window);

/* surface.c */
int s_surface_init (s_window_t *window);
void s_surface_create (s_window_t *window);
void s_surface_shm_attach (s_window_t *window);
void s_surface_linear (s_window_t *window);
void s_surface_uninit (s_window_t *window);
void s_surface_changed (s_window_t *window, s_rect_t *changed);

/* thread.c */
int s_thread_sem_create (s_thread_sem_t *sem, int initial);
int s_thread_sem_destroy (s_thread_sem_t *sem);
int s_thread_sem_wait (s_thread_sem_t *sem);
int s_thread_sem_wait_timeout (s_thread_sem_t *sem, int msec);
int s_thread_sem_post (s_thread_sem_t *sem);
int s_thread_mutex_init (s_thread_mutex_t **mut);
int s_thread_mutex_destroy (s_thread_mutex_t *mut);
int s_thread_mutex_lock (s_thread_mutex_t *mut);
int s_thread_mutex_trylock (s_thread_mutex_t *mut);
int s_thread_mutex_unlock (s_thread_mutex_t *mut);
int s_thread_cond_init (s_thread_cond_t **cond);
int s_thread_cond_destroy (s_thread_cond_t *cond);
int s_thread_cond_signal (s_thread_cond_t *cond);
int s_thread_cond_broadcast (s_thread_cond_t *cond);
int s_thread_cond_wait (s_thread_cond_t *cond, s_thread_mutex_t *mut);
int s_thread_cond_timedwait (s_thread_cond_t *cond, s_thread_mutex_t *mut, int msec);
void * s_thread_run (void *farg);
s_thread_t * s_thread_create (void * (*f) (void *), void *farg);
int s_thread_cancel (s_thread_t *tid);
int s_thread_join (s_thread_t *tid, void **ret);
int s_thread_self (void);
void s_thread_exit (void *ret);

/** @defgroup timer timer struct, api
  * @brief detailed description
  *
  * @example
  *
  * for further information look in demo/timer/
  *
  * @code
  * // this is the timer callback
  * void timer_cb (s_window_t *window, s_timer_t *timer)
  * {
  *	// do what ever you want
  *
  *	// use s_timer_* functions to change anything on timer,
  *	s_timer_timeval(window, timeval, new_timeval);
  *	s_timer_del(window, timer);
  *	s_timer_uninit(timer);
  *
  *	// or to work directly on time struct
  *	s_thread_mutex_lock(window->timers->mut);
  *	// do anything on window->timers->*, or timer->*
  *	s_thread_mutex_unlock(window->timers->mut);
  * }
  *
  * s_timer_t *tmr;
  * s_window_t *window;
  *
  * // initialize a timer, and set a callback function
  * s_timer_init(&tmr);
  * timer->timeval = 2000;
  * timer->cb = timer_cb;
  * s_timer_add(window, timer);
  *
  * // timeval timer can be changed any time
  * s_timer_timeval(window, timer, 3000);
  *
  * // delete the timer from window, and uninitialize
  * s_timer_del(window, timer);
  * s_timer_uninit(tmr);
  * @endcode
  */

/** @addtogroup timer */
/*@{*/

/** timer struct
  */
struct s_timer_s {
	/** timer value in microseconds */
	long long timeval;
	/** remaining timer value in microseconds,
	    if this is <= 0 then it is time to call the callback */
	long long interval;
	/** call back function */
	void (*cb) (s_window_t *, struct s_timer_s *);
	/** pointer to hold user specified data */
	void *user_data;
};

/** timers struct
  */
struct s_timers_s {
	/** list of timers */
	s_list_t *timers;
	/** list mutex */
	s_thread_mutex_t *mut;
};

/* timer.c */

/** @brief initialize the timer struct.
  *
  * @param **timer - address of the timer pointer.
  * @returns 0 on success, 1 on error.
  *
  */
int s_timer_init (s_timer_t **timer);

/** @brief updates/sets the timeval for the given timer
  *
  * @param *window - window that holds the timer
  * @param *timer  - the timer
  * @param timeval - timeval to set the timer
  * @returns 0 on success, 1 on error.
  */
int s_timer_timeval (s_window_t *window, s_timer_t *timer, int timeval);

/** @brief uninitialize the timer struct
  *
  * @param *timer - the timer
  * @returns 0 on success, 1 on error.
  */
int s_timer_uninit (s_timer_t *timer);

/** @brief deletes a timer from windows` timers list
  *
  * @param *window - window that holds the timer
  * @param *timer  - timer
  * @returns 0 on success, 1 on error.
  */
int s_timer_del (s_window_t *window, s_timer_t *timer);

/** @brief adds a timer to windows` timers list
  *
  * @param *window - window to attach the timer
  * @param *timer  - the timer
  * @returns 0 on success, 1 on error.
  */
int s_timer_add (s_window_t *window, s_timer_t *timer);

/** @brief initialize the timers struct for given window
  *
  * @param *window - window
  * @returns 0 on success, 1 on error.
  */
int s_timers_init (s_window_t *window);

/** @brief uninitialize the timers struct for given window
  *
  * @param *window - window
  * @returns 0 on success, 1 on error.
  */
int s_timers_uninit (s_window_t *window);
/*@}*/

/* window.c */
void s_window_set_cursor (s_window_t *window, S_MOUSE_CURSOR cursor);
void s_window_set_title (s_window_t *window, char *fmt, ...);
void s_window_form_draw (s_window_t *window);
void s_window_hide (s_window_t *window);
void s_window_show (s_window_t *window);
void s_window_set_coor (s_window_t *window, int form, int x, int y, int w, int h);
void s_window_set_resizeable (s_window_t *window, int resizeable);
void s_window_set_alwaysontop (s_window_t *window, int alwaysontop);
int s_window_new (s_window_t *window, S_WINDOW type, s_window_t *parent);

#ifdef __cplusplus
	}
#endif

#endif
