/***************************************************************************
    begin                : Tue Oct 14 2003
    copyright            : (C) 2003 - 2007 by Alper Akcan
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

#ifndef XYNTH_H
#define XYNTH_H

#if defined(__cplusplus)
	extern "C" {
#endif

typedef struct s_font_ft_s s_font_ft_t;
typedef struct s_gettext_s s_gettext_t;

typedef struct s_window_s s_window_t;

typedef struct s_config_var_s s_config_var_t;
typedef struct s_config_cat_s s_config_cat_t;
typedef struct s_config_s s_config_t;
typedef struct s_handler_s s_handler_t;
typedef struct s_handlers_s s_handlers_t;
typedef struct s_handler_keybd_s s_handler_keybd_t;
typedef struct s_handler_mouse_s s_handler_mouse_t;
typedef struct s_hashtable_node_s s_hashtable_node_t;
typedef struct s_hashtable_table_s s_hashtable_table_t;
typedef struct s_hashtable_s s_hashtable_t;
typedef struct s_image_s s_image_t;
typedef struct s_list_s s_list_t;
typedef struct s_list_node_s s_list_node_t;
typedef struct s_pollfd_s s_pollfd_t;
typedef struct s_pollfds_s s_pollfds_t;
typedef struct s_rect_s s_rect_t;
typedef struct s_surface_s s_surface_t;
typedef struct s_thread_s s_thread_t;
typedef struct s_thread_sem_s s_thread_sem_t;
typedef struct s_thread_cond_s s_thread_cond_t;
typedef struct s_thread_mutex_s s_thread_mutex_t;
typedef struct s_timer_s s_timer_t;
typedef struct s_timers_s s_timers_t;
typedef struct s_xml_node_attr_s s_xml_node_attr_t;
typedef struct s_xml_node_s s_xml_node_t;

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
/* 199 */  S_KEYCODE_USER15,
/* 200 */  S_KEYCODE_USER16,
/* 201 */  S_KEYCODE_USER17,
/* 202 */  S_KEYCODE_USER18,
/* 203 */  S_KEYCODE_USER19,
/* 204 */  S_KEYCODE_USER20,
/* 205 */  S_KEYCODE_USER21,
/* 206 */  S_KEYCODE_USER22,
/* 207 */  S_KEYCODE_USER23,
/* 208 */  S_KEYCODE_USER24,
/* 209 */  S_KEYCODE_USER25,
/* 210 */  S_KEYCODE_USER26,
/* 211 */  S_KEYCODE_USER27,
/* 212 */  S_KEYCODE_NOCODE,
/* 213 */  S_KEYCODE_CODES
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
	NONE_EVENT     = 0x00000000,
	QUIT_EVENT     = 0x00000001,
	KEYBD_EVENT    = 0x00000002,
	KEYBD_RELEASED = 0x00000004,
	KEYBD_PRESSED  = 0x00000008,
	KEYBD_IRREXTEN = 0x00000010,
	MOUSE_EVENT    = 0x00000020,
	MOUSE_OVER     = 0x00000040,
	MOUSE_RELEASED = 0x00000080,
	MOUSE_PRESSED  = 0x00000100,
	MOUSE_CLICKED  = 0x00000200,
	MOUSE_ENTERED  = 0x00000400,
	MOUSE_EXITED   = 0x00000800,
	MOUSE_HINT     = 0x00001000,
	MOUSE_HINT2    = 0x00002000,
	EXPOSE_EVENT   = 0x00004000,
	CONFIG_EVENT   = 0x00008000,
	CONFIG_CHNGX   = 0x00010000,
	CONFIG_CHNGY   = 0x00020000,
	CONFIG_CHNGW   = 0x00040000,
	CONFIG_CHNGH   = 0x00080000,
	FOCUS_EVENT    = 0x00100000,
	FOCUSIN_EVENT  = 0x00200000, 
	FOCUSOUT_EVENT = 0x00400000,
	DESKTOP_EVENT  = 0x00800000,
	TIMER_EVENT    = 0x01000000,
	REMOTE_EVENT   = 0x02000000,
	SIGNAL_EVENT   = 0x04000000,
	KEYBD_MASK     = (KEYBD_EVENT | KEYBD_RELEASED | KEYBD_PRESSED | KEYBD_IRREXTEN),
	MOUSE_MASK     = (MOUSE_EVENT | MOUSE_OVER | MOUSE_RELEASED | MOUSE_PRESSED | MOUSE_CLICKED | MOUSE_HINT | MOUSE_HINT2),
	EVENT_MASK     = (QUIT_EVENT | KEYBD_EVENT | MOUSE_EVENT | EXPOSE_EVENT | CONFIG_EVENT | FOCUS_EVENT | DESKTOP_EVENT | TIMER_EVENT),
} S_EVENT;

typedef struct s_font_glyph_s {
	int yMin;
	int yMax;
	int height;
	int lineskip;
	int size;
	s_image_t *img;
} s_font_glyph_t;

typedef struct s_font_s {
	char *name;
	char *str;
	int size;
	int ascent;
	int descent;
	int height;
	int lineskip;
	int underline_offset;
	int underline_height;
	unsigned int rgb;
	s_font_glyph_t glyph;
	s_font_ft_t *ft;
} s_font_t;

typedef struct s_mouse_s {
	int x;
	int y;
	int x_old;
	int y_old;
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
	int scancode;
	S_KEYCODE_CODE keycode;
	int state[S_KEYCODE_CODES];
	long long time;
	S_KEYCODE_FLAG flag;
} s_keybd_t;

typedef struct s_expose_s {
	s_rect_t *rect;
} s_expose_t;

typedef struct s_desktop_client_s {
	int id;
	int pri;
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
	/** void event */
	void *data;
} s_event_t;

/** event queue */
typedef struct s_eventq_s {
	/** dispatcher thread`s id */
	s_thread_t *tid;
	/** queue list holds s_event_t */
	s_list_t *queue;
	/** conditional variable to suspend dispatcher thread */
        s_thread_cond_t *cond;
        /** queue mutex */
	s_thread_mutex_t *mut;
} s_eventq_t;

typedef struct s_childs_s {
	s_list_t *list;
	s_thread_mutex_t *mut;
} s_childs_t;

typedef struct s_single_app_s {
	int (*fonk) (int argc, char *argv[]);
	int argc;
	char *argv[10];
	s_thread_t *tid;
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

/** @defgroup client_config Client Library - Config API
  * @brief detailed description
  *
  * @example
  *
  * basic.cfg file;
  * @code
  * ; this is a basic configuration file
  * [category0]
  * variable0 = value0 ; this is a comment
  * variable1 = value1
  *
  * [category1]
  * variable0 = value2
  * variable2 = value3
  * @endcode
  *
  * the code;
  * @code
  * int i;
  * int j;
  * s_config_t *cfg;
  * s_config_cat_t *cat;
  * s_config_var_t *var;
  *
  * // initialize cfg struct
  * s_config_init(&cfg);
  *
  * // parse the cfg file
  * s_config_parse(cfg, "basic.cfg");
  *
  * // walk through the categories
  * i = 0;
  * while (!s_list_eol(cfg->category, i)) {
  *	// get category and print
  * 	cat = s_list_get(cfg->category, i++);
  *	printf("category: %s\n", cat->name);
  *
  *	// walk though the variables in category
  *	j = 0;
  *	while (!s_list_eol(cat->variable, j)) {
  *		// get variable and print
  *		var = s_list_get(cat->variable, j++);
  *		printf("\t%s = %s\n", var->name, var->value);
  *	}
  * }
  *
  * // uninitialize the cfg struct
  * s_config_uninit(cfg);
  * @endcode
  *
  * print out will be;
  * @code
  * category : category0
  *	variable0 = value0
  *	variable1 = value1
  * category : category1
  *	variable0 = value2
  *	variable2 = value3
  * @endcode
  */

/** @addtogroup client_config */
/*@{*/

/** config variable struct
  */
struct s_config_var_s {
	/** variable name */
	char *name;
	/** variable value */
	char *value;
};

/** config category struct
  */
struct s_config_cat_s {
	/** category name */
	char *name;
	/** list of variables in category */
	s_list_t *variable;
};

/** config struct
  */
struct s_config_s {
	/** list of categories in config */
	s_list_t *category;
};

/* config.c */

/** @brief strips leading and trailing white spaces by modifing buf
  *
  * @param buf - buffer to strip white spaces from
  * @returns a valid pointer
  */
char * s_config_strip (char *buf);

/** @brief parses the given configuration file
  *
  * @param cfg  - the config struct pointer
  * @param name - file name to parse
  * @returns 0 on success, 1 on error.
  */
int s_config_parse (s_config_t *cfg, char *name);

/** @brief initialize the config struct
  *
  * @param cfg - pointer to the config struct pointer
  * @returns 0 on success, 1 on error.
  */
int s_config_init (s_config_t **cfg);

/** @brief initialize the category struct
  *
  * @param cat  - pointer to the category struct pointer
  * @param name - category name
  * @returns 0 on success, 1 on error.
  */
int s_config_category_init (s_config_cat_t **cat, char *name);

/** @brief initialize the variable struct
  *
  * @param var   - pointer to the variable struct pointer
  * @param name  - variable name
  * @param value - variable value
  * @returns 0 on success, 1 on error.
  */
int s_config_variable_init (s_config_var_t **var, char *name, char *value);

/** @brief uninitialize the variable struct
  *
  * @param var - the variable struct pointer
  * @returns 0 on success, 1 on error.
  */
int s_config_variable_uninit (s_config_var_t *var);

/** @brief uninitialize the category struct
  *
  * @param cat - the category struct pointer
  * @returns 0 on success, 1 on error.
  */
int s_config_category_uninit (s_config_cat_t *cat);

/** @brief uninitialize the config struct
  *
  * @param cfg - the config struct pointer
  * @returns 0 on success, 1 on error.
  */
int s_config_uninit (s_config_t *cfg);

/*@}*/

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
void bpp_putbox (s_surface_t *surface, int x, int y, int w, int h, void *sp, int bw);
void bpp_putbox_mask (s_surface_t *surface, int x, int y, int w, int h, void *sp, unsigned char *sm, int bw);
void bpp_putbox_alpha (s_surface_t *surface, int x, int y, int w, int h, void *sp, unsigned char *sm, int bw);
void bpp_putbox_rgb (s_surface_t *surface, int x, int y, int w, int h, unsigned int *rgb, int bw);
void bpp_putbox_rgba (s_surface_t *surface, int x, int y, int w, int h, unsigned int *rgba, int bw);
void bpp_getbox (s_surface_t *surface, int x, int y, int w, int h, void *dp);
void bpp_putbox_o (s_surface_t *surface, int id,  int x, int y, int w, int h, void *sp, int bw);
void bpp_putbox_mask_o (s_surface_t *surface, int id,  int x, int y, int w, int h, void *sp, unsigned char *sm, int bw);
void bpp_putbox_alpha_o (s_surface_t *surface, int id,  int x, int y, int w, int h, void *sp, unsigned char *sm, int bw);
void bpp_putbox_rgb_o (s_surface_t *surface, int id,  int x, int y, int w, int h, unsigned int *rgb, int bw);
void bpp_putbox_rgba_o (s_surface_t *surface, int id,  int x, int y, int w, int h, unsigned int *rgba, int bw);
void bpp_getbox_o (s_surface_t *surface, int id,  int x, int y, int w, int h, void *dp);

/* event.c */
int s_event_mouse_handler_state (s_window_t *window, s_event_t *event, s_handler_mouse_t *mouse, int over);
int s_event_parse_handler_over (s_window_t *window, s_event_t *event, s_handler_t *work);
int s_event_parse_handler_notover (s_window_t *window, s_event_t *event, s_handler_t *work);
void s_event_parse_mouse (s_window_t *window, s_event_t *event);
int s_event_parse_keybd_handler (s_window_t *window, s_event_t *event, s_handler_t *work);
int s_event_parse_keybd (s_window_t *window, s_event_t *event);
int s_event_parse_expos (s_window_t *window, s_event_t *event);
int s_event_parse_config (s_window_t *window, s_event_t *event);
int s_event_parse_timer (s_window_t *window, s_event_t *event);
int s_event_copy (s_event_t *event, s_event_t **nevent);
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
int s_font_get_width (s_font_t *font, char *str);

/* gettext.c */
char * s_setlocale(s_window_t *window, int category, const char *locale);
char * s_bindtextdomain (s_window_t *window, const char *domainname, const char *dirname);
char * s_textdomain (s_window_t *window, const char *domainname);
char * s_gettext (s_window_t *window, const char *str);
void s_gettext_uninit (s_window_t *window);
int s_gettext_init (s_window_t *window);

/** @defgroup client_time Client Library - Time API
  * @brief this is a layer to operating systems time api.
  */

/** @addtogroup client_time */
/*@{*/

/* gettime.c */

/** @brief resolution is miliseconds, and is enough for any gui related work.
  *        if you need a high presicion you should not use this function,
  *        consider looking to your system specs.
  * 
  * @returns the time value in miliseconds
  */
long long s_gettimeofday (void);

/*@}*/

/** @defgroup client_grlib Client Library - Graphics API
  * @brief these are simple low-level graphics api functions, for further information
  *        you may look in demo, and the server source codes.
  *
  * @example
  * @code
  * // simple example code will be in here
  * @endcode
  */

/** @addtogroup client_grlib */
/*@{*/

/* grlib.c */

/** @brief returns pixel value used in the current surface that corresponds with the given color components.
  *
  * @param *surface - the surface
  * @param r        - red
  * @param g        - green
  * @param b        - blue
  * @returns the calulated color value
  */
int s_rgbcolor (s_surface_t *surface, int r, int g, int b);

/** @brief return the red, green, and blue values for the given color
  *
  * @param *surface - the surface
  * @param c        - the color
  * @param *r       - red (output)
  * @param *g       - green (output)
  * @param *b       - blue (output)
  * @returns no return
  */
void s_colorrgb (s_surface_t *surface, int c, int *r, int *g, int *b);

/** @brief puts one pixel color to the x, y coordinate on the surface
  *
  * @param *surface - the surface
  * @param x        - x coordinate
  * @param y        - y coordinate
  * @param c        - the color
  * @returns no return
  */
void s_setpixel (s_surface_t *surface, int x, int y, int c);

/** @brief puts one pixel color to the x, y coordinate on the surface
  *
  * @param *surface - the surface
  * @param x        - x coordinate
  * @param y        - y coordinate
  * @param r        - red value
  * @param g        - green value
  * @param b        - blue value
  * @returns no return
  */
void s_setpixelrgb (s_surface_t *surface, int x, int y, int r, int g, int b);

/** @brief puts one pixel color to the x, y coordinate on the surface (with alpha)
  *
  * @param *surface - the surface
  * @param x        - x coordinate
  * @param y        - y coordinate
  * @param r        - red value
  * @param g        - green value
  * @param b        - blue value
  * @param a        - alpha value
  * @returns no return
  */
void s_setpixelrgba (s_surface_t *surface, int x, int y, int r, int g, int b, int a);

/** @brief gets the color value of the pixel form x, y coordinates of the surface
  *
  * @param *surface - the surface
  * @param x        - x coordinate
  * @param y        - y coordinate
  * @returns the color value
  */
int s_getpixel (s_surface_t *surface, int x, int y);

/** @brief gets the red, green, blue values of the pixel form x, y coordinates of the surface
  *
  * @param *surface - the surface
  * @param x        - x coordinate
  * @param y        - y coordinate
  * @param *r       - red value (output)
  * @param *g       - green value (output)
  * @param *b       - blue value (output)
  * @returns no return
  */
void s_getpixelrgb (s_surface_t *surface, int x, int y, int *r, int *g, int *b);

/** @brief draw a horizontal line from point (x1, y) to (x2, y) in color c on to the surface
  *
  * @param *surface - the surface
  * @param x1       - start coordinate
  * @param y        - y coordinate
  * @param x2       - end coordinate
  * @param c        - color value
  * @returns no return
  */
void s_hline (s_surface_t *surface, int x1, int y, int x2, int c);

/** @brief draw a vertical line from point (x, y1) to (x, y2) in color c on to the surface
  *
  * @param *surface - the surface
  * @param x        - x coordinate
  * @param y1       - start coordinate
  * @param y2       - end coordinate
  * @param c        - color value
  * @returns no return
  */
void s_vline (s_surface_t *surface, int x, int y1, int y2, int c);

/** @brief draw a line from point (x1, y1) to (x2, y2) inclusively in color c
  *
  * @param *surface - the surface
  * @param x1       - x start point
  * @param x2       - x end point
  * @param y1       - y start point
  * @param y2       - y end point
  * @param c        - color value
  * @returns no return
  */
void s_line (s_surface_t *surface, int x1, int y1, int x2, int y2, int c);

/** @brief fill a rectangular area at position (x, y) with size (w, h) of the screen with a
  *        single color c on to the surface
  *
  * @param *surface - the surface
  * @param x        - x coordinate
  * @param y        - y coordinate
  * @param w        - width
  * @param h        - height
  * @param c        - color value
  * @returns no return
  */
void s_fillbox (s_surface_t *surface, int x, int y, int w, int h, int c);

/** @brief copy the contents of a memory buffer sp to a rectangular bitmap at position (x, y) with
  *        size (w, h) on to the surface. Pixmaps are in row-major order. the source pixmap memory
  *        has the size w * h * surface->bytesperpixel
  *
  * @param *surface - the surface
  * @param x        - x coordinate
  * @param y        - y coordinate
  * @param w        - width
  * @param h        - height
  * @param *sp      - source pixmap
  * @returns no return
  */
void s_putbox (s_surface_t *surface, int x, int y, int w, int h, void *sp);

/** @brief copy the contents of a memory buffer sp to a rectangular bitmap at position (x, y) with
  *        size (w, h) on to the surface. pixmaps are in row-major order. the source pixmap memory
  *        has the size w * h * surface->bytesperpixel. this function works like s_putbox, but uses
  *        sm for mask operations and does not write pixmap pixels if sm[y * w + x] == 0
  *
  * @param *surface - the surface
  * @param x        - x coordinate
  * @param y        - y coordinate
  * @param w        - width
  * @param h        - height
  * @param *sp      - source pixmap
  * @param *sm      - source mask
  * @returns no return
  */
void s_putboxmask (s_surface_t *surface, int x, int y, int w, int h, char *sp, unsigned char *sm);

/** @brief copy the contents of a memory buffer sp to a rectangular bitmap at position (x, y) with
  *        size (w, h) on to the surface. pixmaps are in row-major order. the source pixmap memory
  *        has the size w * h * surface->bytesperpixel. this function works like s_putbox, but uses
  *        sm for alpha mask operations, sm[y * w + x] is the alpha value for corresponding pixel.
  *
  * @param *surface - the surface
  * @param x        - x coordinate
  * @param y        - y coordinate
  * @param w        - width
  * @param h        - height
  * @param *sp      - source pixmap
  * @param *sm      - source mask
  * @returns no return
  */
void s_putboxalpha (s_surface_t *surface, int x, int y, int w, int h, char *sp, unsigned char *sm);

/** @brief copy the contents of a rgba buffer rgba to a rectangular bitmap at position (x, y) with
  *        size (w, h) on to the surface.
  *
  * @param *surface - the surface
  * @param x        - x coordinate
  * @param y        - y coordinate
  * @param w        - width
  * @param h        - height
  * @param *rgba    - source pixmap rgba values
  * @returns no return
  */
void s_putboxrgba (s_surface_t *surface, int x, int y, int w, int h, unsigned int *rgba);

/** @brief copy the contents of a rgb buffer rgb to a rectangular bitmap at position (x, y) with
  *        size (w, h) on to the surface.
  *
  * @param *surface - the surface
  * @param x        - x coordinate
  * @param y        - y coordinate
  * @param w        - width
  * @param h        - height
  * @param *rgb    - source pixmap rgb values
  * @returns no return
  */
void s_putboxrgb (s_surface_t *surface, int x, int y, int w, int h, unsigned int *rgb);

/** @brief copy a rectangular bitmap at position (x, y) with size (w, h) from the surface to a buffer
  *        dp. pixmaps are in row-major order. the destination pixmap has the size w * h * surface->bytesperpixel
  *
  * @param *surface - the surface
  * @param x        - x coordinate
  * @param y        - y coordinate
  * @param w        - width
  * @param h        - height
  * @param *dp      - destination pixmap
  * @returns no return
  */
void s_getbox (s_surface_t *surface, int x, int y, int w, int h, void *dp);

/** @brief copy the contents of a memory buffer sp to a rectangular bitmap at position (x, y) with
  *        size (w, h) on to the surface. however, only a part of size (w, h) starting at offset
  *        (xo, yo) in the pixmap is copied. pixmaps are in row-major order. the source pixmap memory
  *        has the size bw * bh * surface->bytesperpixel
  *
  * @param *surface - the surface
  * @param x        - x coordinate
  * @param y        - y coordinate
  * @param w        - width
  * @param h        - height
  * @param bw       - bitmap width
  * @param bh       - bitmap heihgt
  * @param *sp      - source pixmap
  * @param xo       - x offset
  * @param yo       - y offset
  * @returns no return
  */
void s_putboxpart (s_surface_t *surface, int x, int y, int w, int h, int bw, int bh, void *sp, int xo, int yo);

/** @brief copy the contents of a memory buffer sp to a rectangular bitmap at position (x, y) with
  *        size (w, h) on to the surface. however, only a part of size (bw, bh) starting at offset
  *        (xo, yo) in the pixmap is copied. pixmaps are in row-major order. the source pixmap memory
  *        has the size w * h * surface->bytesperpixel. this function works like s_putbox, but uses
  *        sm for alpha mask operations, sm[y * w + x] is the alpha value for corresponding pixel.
  *
  * @param *surface - the surface
  * @param x        - x coordinate
  * @param y        - y coordinate
  * @param w        - width
  * @param h        - height
  * @param bw       - bitmap width
  * @param bh       - bitmap heihgt
  * @param *sp      - source pixmap
  * @param *sm      - source mask
  * @param xo       - x offset
  * @param yo       - y offset
  * @returns no return
  */
void s_putboxpartmask (s_surface_t *surface, int x, int y, int w, int h, int bw, int bh, char *sp, unsigned char *sm, int xo, int yo);

/** @brief copy the contents of a rgb buffer rgb to a rectangular bitmap at position (x, y) with
  *        size (w, h) on to the surface. however, only a part of size (bw, bh) starting at offset
  *        (xo, yo) in the pixmap is copied.
  *
  * @param *surface - the surface
  * @param x        - x coordinate
  * @param y        - y coordinate
  * @param w        - width
  * @param h        - height
  * @param bw       - bitmap width
  * @param bh       - bitmap heihgt
  * @param *rgb    - source pixmap rgb values
  * @param xo       - x offset
  * @param yo       - y offset
  * @returns no return
  */
void s_putboxpartrgb (s_surface_t *surface, int x, int y, int w, int h, int bw, int bh, unsigned int *rgb, int xo, int yo);

/** @brief copy the contents of a rgba buffer rgba to a rectangular bitmap at position (x, y) with
  *        size (w, h) on to the surface. however, only a part of size (bw, bh) starting at offset
  *        (xo, yo) in the pixmap is copied.
  *
  * @param *surface - the surface
  * @param x        - x coordinate
  * @param y        - y coordinate
  * @param w        - width
  * @param h        - height
  * @param bw       - bitmap width
  * @param bh       - bitmap heihgt
  * @param *rgba    - source pixmap rgba values
  * @param xo       - x offset
  * @param yo       - y offset
  * @returns no return
  */
void s_putboxpartrgba (s_surface_t *surface, int x, int y, int w, int h, int bw, int bh, unsigned int *rgba, int xo, int yo);

/** @brief copy the contents of a memory buffer sp to a rectangular bitmap at position (x, y) with
  *        size (w, h) on to the surface. however, only a part of size (bw, bh) starting at offset
  *        (xo, yo) in the pixmap is copied. pixmaps are in row-major order. the source pixmap memory
  *        has the size w * h * surface->bytesperpixel. this function works like s_putbox, but uses
  *        sm for alpha mask operations, sm[y * w + x] is the alpha value for corresponding pixel.
  *
  * @param *surface - the surface
  * @param x        - x coordinate
  * @param y        - y coordinate
  * @param w        - width
  * @param h        - height
  * @param bw       - bitmap width
  * @param bh       - bitmap heihgt
  * @param *sp      - source pixmap
  * @param *sm      - source mask
  * @param xo       - x offset
  * @param yo       - y offset
  * @returns no return
  */
void s_putboxpartalpha (s_surface_t *surface, int x, int y, int w, int h, int bw, int bh, void *sp, unsigned char *sm, int xo, int yo);

/** @brief copy the rectangular area at (x1, y1) of size (w, h), to (x2, y2) (surface copy).
  *
  * @param *surface - the surface
  * @param x1       - source x coordinate
  * @param y1       - source y coordinate
  * @param w        - width
  * @param h        - height
  * @param x2       - destination x coordinate
  * @param y2       - destination y coordinate
  * @returns no return
  */
void s_copybox (s_surface_t *surface, int x1, int y1, int w, int h, int x2, int y2);

/** @brief creates a surface, as virtual with the given bitsperpixel, width,
  *        and height. surface buffer will be allocated internal with the size
  *        of width * height * bytesperpixel.
  *
  * @param **susrface   - the surface
  * @param width        - width
  * @param height       - height
  * @param bitsperpixel - bitsperpixel
  * @return 0 on success.
  */
int s_surface_create (s_surface_t **surface, int width, int height, int bitsperpixel);

/** @brief creates a surface, as virtual with the given bitsperpixel, width,
  *        and height. vbuf will be the virtual buffer of the surface. the
  *        source pixmap memory (vbuf) has the size w * h * bytesperpixel.
  *
  * @param **susrface   - the surface
  * @param width        - width
  * @param height       - height
  * @param bitsperpixel - bitsperpixel
  * @param *vbuf        - given virtual buffer
  * @return 0 on success.
  */
int s_surface_create_from_data (s_surface_t **surface, int width, int height, int bitsperpixel, void *vbuf);

/** @brief destroys given virtual surface, vbuf will not be freed unless it was
  *        allocated internaly.
  * 
  * @param *surface - the surface
  * @return 0 on success.
  */
int s_surface_destroy (s_surface_t *surface);

/** @brief copies and converts the source buffer (sb) which is sbitspp bitsperpixel, to destination
  *        buffer (db) which is dbitspp bitsperpixel. db will be alocated internally.
  *
  * @param *sb     - source buffer
  * @param sbitspp - source bitsperpixel
  * @param **db    - destination buffer
  * @param dbitspp - destination bitsperpixel
  * @param w       - width
  * @param h       - height
  * @returns 0 on success
  */
int s_copybuffer (char *sb, int sbitspp, char **db, int dbitspp, int w, int h);

/** @brief rotate the bitmap of size (srect) at sbuf to size (drect) and store it at dbuf, which must be
  *        a large enough buffer, with the angle of rotate.
  *
  * @param *surface - the surface
  * @param *srect   - source rectangle
  * @param *sbuf    - source buffer
  * @param *drect   - destination rectangle
  * @param *dbuf    - destination buffer
  * @param rotate   - rotate angle
  * @returns no return
  */
void s_rotatebox (s_surface_t *surface, s_rect_t *srect, void *sbuf, s_rect_t *drect, void *dbuf, int rotate);

/** @brief scale the bitmap of size (w1, h1) at _dp1 to size (w2, h2) and store it at _dp2, which must be
  *        a large enough buffer.
  *
  * @param *surface - the surface
  * @param w1       - source width
  * @param h1       - source height
  * @param *_dp1    - source pixmap
  * @param w2       - destination width
  * @param h2       - destination height
  * @param *_dp2    - destination pixmap
  * @returns no return
  */
void s_scalebox (s_surface_t *surface, int w1, int h1, void *_dp1, int w2, int h2, void *_dp2);

/** @brief copy the contents of a memory buffer sp to a rectangular bitmap at position (x, y) with
  *        size (w, h) on to the memory buffer dp. however, only a part of size (w, h) starting at offset
  *        (xo, yo) in the pixmap is copied. pixmaps are in row-major order. the source pixmap memory
  *        has the size bw * bh * 1. this is a special copy function to use internally.
  *
  * @param *dp - destination pixmap
  * @param *dw - destination width
  * @param *dh - destination height
  * @param x   - x coordinate
  * @param y   - y coordinate
  * @param w   - width
  * @param h   - height
  * @param bw  - bitmap width
  * @param bh  - bitmap heihgt
  * @param *sp - source pixmap
  * @param xo  - x offset
  * @param yo  - y offset
  * @returns no return
  */
int s_putmaskpart (unsigned char *dp, int dw, int dh, int x, int y, int w, int h, int bw, int bh, unsigned char *sp, int xo, int yo);

/*@}*/

/** @defgroup client_handler Client Library - Handlers API
  * @brief this api is used for setting and processing handlers, and callback for input events.
  *
  * @example
  * 
  * for further information look in demo/ directory
  * 
  * @code
  * void simpe_handle_onover (s_window_t *window, s_event_t *event, s_handler_t *handler)
  * {
  * }
  * void simpe_handle_pressed (s_window_t *window, s_event_t *event, s_handler_t *handler)
  * {
  * }
  * void simpe_handle_released (s_window_t *window, s_event_t *event, s_handler_t *handler)
  * {
  * }
  * void simpe_handle_clicked (s_window_t *window, s_event_t *event, s_handler_t *handler)
  * {
  * }
  * {
  * 	s_handler_t *hndl;
  * 	s_handler_init(&hndl);
  * 	hndl->type = MOUSE_HANDLER;
  * 	hndl->mouse.x = 0;
  * 	hndl->mouse.y = 0;
  * 	hndl->mouse.w = 20;
  * 	hndl->mouse.h = 20;
  * 	hndl->mouse.o = simpe_handle_onover;
  * 	hndl->mouse.c = simpe_handle_clicked;
  * 	hndl->mouse.p = simpe_handle_pressed;
  * 	hndl->mouse.r = simpe_handle_released;
  * 	hndl->mouse.button = MOUSE_LEFTBUTTON;
  * 	s_handler_add(window, hndl);
  * }
  * @endcode
  */

/** @addtogroup client_handler */
/*@{*/

/** handler type enumarator
  */
typedef enum S_HANDLER {
	/** handler is a mouse handler */
	MOUSE_HANDLER = 0x1,
	/** handler is a keyboard handler */
	KEYBD_HANDLER = 0x2
} S_HANDLER;

/** keyboard handler struct
  */
struct s_handler_keybd_s {
	/** handler flags bitwise ored */
	int flag;
	/** handler button */
	int button;
	/** key pressed callback function */
	void (*p) (s_window_t *, s_event_t *, s_handler_t *);
	/** key released callback function */
	void (*r) (s_window_t *, s_event_t *, s_handler_t *);
};

/** mouse handler struct
  */
struct s_handler_mouse_s {
	/** mouse handler x */
	int x;
	/** mouse handler y */
	int y;
	/** mouse handler width */
	int w;
	/** mouse handler height */
	int h;
	/** mouse handler button */
	int button;
	/** button pressed callback function */
	void (*p) (s_window_t *, s_event_t *, s_handler_t *);
	/** button released callback function */
	void (*r) (s_window_t *, s_event_t *, s_handler_t *);
	/** button clicked callback function */
	void (*c) (s_window_t *, s_event_t *, s_handler_t *);
	/** pointer is on over callback function */
	void (*o) (s_window_t *, s_event_t *, s_handler_t *);
	/** pointer is on over and button is still pressed callback function */
	void (*ho) (s_window_t *, s_event_t *, s_handler_t *);
	/** mouse button released, but the previous press was not on us */
	void (*hr) (s_window_t *, s_event_t *, s_handler_t *);
	/** pointer is not on over, but was on over */
	void (*oh) (s_window_t *, s_event_t *, s_handler_t *);
	/** pointer is not on over, but was on over and button is still pressed */
	void (*hoh) (s_window_t *, s_event_t *, s_handler_t *);
	/** mouse button is released outside, but the previous press was on us */
	void (*rh) (s_window_t *, s_event_t *, s_handler_t *);
	/** internal private value */
	S_EVENT hstate;
};

/** handler struct
  */
struct s_handler_s {
	/** handler type */
	S_HANDLER type;
	/** mouse handler */
	s_handler_mouse_t mouse;
	/** keyboard handler */
	s_handler_keybd_t keybd;
	/** user data which will be passed through callback functions */
	void *data;
};

/** handlers struct
  */
struct s_handlers_s {
	/** handlers list */
	s_list_t *list;
	/** handlers list mutex */
	s_thread_mutex_t *mut;
};

/* handler.c */

/** @brief initialize the thandler struct.
  *
  * @param **handler - address of the handler pointer.
  * @returns 0 on success, 1 on error.
  *
  */
int s_handler_init (s_handler_t **handler);

/** @brief uninitialize the thandler struct.
  *
  * @param *handler - poniter to the handler, that was returned from s_handler_init.
  * @returns 0 on success, 1 on error.
  *
  */
int s_handler_uninit (s_handler_t *handler);

/** @brief adds a handler to windows` handlers list
  *
  * @param *window - window to attach the timer
  * @param *handler  - the handler
  * @returns 0 on success, 1 on error.
  */
int s_handler_add (s_window_t *window, s_handler_t *handler);

/** @brief deletes a handler from windows` handlers list
  *
  * @param *window - window that holds the handler
  * @param *handler  - handler
  * @returns 0 on success, 1 on error.
  */
int s_handler_del (s_window_t *window, s_handler_t *handler);

/** @brief initialize the handlers struct for given window
  *
  * @param *window - window
  * @returns 0 on success, 1 on error.
  */
int s_handlers_init (s_window_t *window);

/** @brief uninitialize the handlers struct for given window
  *
  * @param *window - window
  * @returns 0 on success, 1 on error.
  */
int s_handlers_uninit (s_window_t *window);

/*@}*/

/** @defgroup client_hashtable Client Library - Hashtable API
  * @brief this api is used for simple hashtable support.
  *
  * @example
  * 
  * for further information look in demo/ directory
  * 
  * @code
  * {
  * 	s_hashtable_t *htable;
  * 	s_hashtable_init(&htable, S_HASHTABLE_MASK, S_HASHTABLE_DEPTH);
  * 	s_hashtable_add(htable, "name-0", (void *) "value-0");
  *	s_hashtable_add(htable, "name-1", (void *) "value-1");
  * 	printf("name-0: %s\n", s_hashtable_get_data(htable, "name-0")); 
  * 	printf("name-1: %s\n", s_hashtable_get_data(htable, "name-1")); 
  * 	s_hashtable_uninit(&htable);
  * }
  * @endcode
  */

/** @addtogroup client_hashtable */
/*@{*/

/* hashtable.c */

/** hashtable default values
  */
typedef enum S_HASHTABLE {
	/** default table mask */
	S_HASHTABLE_MASK = 0xf,
	/** default table depth */
	S_HASHTABLE_DEPTH = 0x8,
} S_HASHTABLE;

/** hashtable node struct
  */ 
struct s_hashtable_node_s {
	/** node hash key (32 bit) */
	unsigned long int key;
	/** node name */
	char *name;
	/** next node with the same hash key */
	s_hashtable_node_t *next;
	/** prev node with the same hash key */
	s_hashtable_node_t *prev;
	/** table that node is attached to */
	s_hashtable_table_t *table;
	/** private data, for user */
	void *data;
};

/** hashtable table struct
  */ 
struct s_hashtable_table_s {
	/** tree tables */
	s_hashtable_table_t *table;
	/** tree nodes */
	s_hashtable_node_t *node;
};

/** hashtable struct
  */ 
struct s_hashtable_s {
	/** hashtable size for allocation */
	unsigned int size;
	/** hashtable hashkey bitwise mask */
	unsigned int mask;
	/** hashtable tables depth */
	unsigned int depth;
	/** the root table */
	s_hashtable_table_t *table;
};

/** @brief returns the node which was added with the name "name".
  *
  * @param *htable - hashtable
  * @param *name   - name to search
  * @returns the node, or NULL if fails.
  */
s_hashtable_node_t * s_hashtable_get_node (s_hashtable_t *htable, char *name);

/** @brief adds a node with the name "name", and data "data" to the hashtable
  *
  * @param *htable - hashtable
  * @param *name   - key name
  * @param *data   - key data
  * @returns 0 on success, 1 on error
  */
int s_hashtable_add (s_hashtable_t *htable, char *name, void *data);

/** @brief deletes the node which was added with the name "name".
  *
  * @param *htable - hashtable
  * @param *name   - name to search
  * @returns 0 on success, 1 on error
  */
int s_hashtable_del (s_hashtable_t *htable, char *name);

/** @brief returns the private data of the node which was added with the name "name".
  *
  * @param *htable - hashtable
  * @param *name   - name to search
  * @returns the data, or NULL if fails.
  */
void * s_hashtable_get_data (s_hashtable_t *htable, char *name);

/** @brief initialize the hashtable
  *
  * @param **htable - hashtable
  * @param *mask    - mask value to use for each table
  * @param *depth   - maximum depth for the tables
  * @returns 0 on success, 1 on error
  */
int s_hashtable_init (s_hashtable_t **htable, unsigned int mask, unsigned int depth);

/** @brief initialize the hashtable table
  *
  * @param *table - table array
  * @param *size  - array size
  * @returns 0 on success, 1 on error
  */
int s_hashtable_uninit_table (s_hashtable_table_t *table, unsigned int size);

/** @brief uninitialize the hashtable
  *
  * @param *htable - hashtable
  * @returns 0 on success, 1 on error
  */
int s_hashtable_uninit (s_hashtable_t *htable);

/*@}*/

/** @defgroup client_image Client Library - Image API
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

/** @addtogroup client_image */
/*@{*/

/** image struct
  */
struct s_image_s {
	/** image name */
	char *name;
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

/** @defgroup client_list Client Library - List API
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

/** @addtogroup client_list */
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

/** @defgroup client_pollfd Client Library - Pollfd API
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

/** @addtogroup client_pollfd */
/*@{*/

/** polfd struct
  */
struct s_pollfd_s {
	/** fd to poll */
	int fd;
	/** in data call back function */
	int (*pf_in) (s_window_t *, s_pollfd_t *);
	/** error call back function */
	int (*pf_err) (s_window_t *, s_pollfd_t *);
	/** closing call back function */
	int (*pf_close) (s_window_t *, s_pollfd_t *);
	/** user data */
	void *data;
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

/** @defgroup client_rect Client Library - Rect API
  * @brief s_rect_* api is designed for rectangular operations.
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

/** @addtogroup client_rect */
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

/* rect.c */

/** @brief calculates the merging area of r1 and r2.
  *
  * @param *r1 - pointer to a rect
  * @param *r2 - pointer to a rect
  * @param *r  - on success r will be the merging area
  * @returns 0 on success, 1 on error.
  */
int s_rect_merge (s_rect_t *r1, s_rect_t *r2, s_rect_t *r);

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

/* region.c */
typedef struct s_region_s {
	s_rect_t extents;
	int nrects;
	s_rect_t *rects;
} s_region_t;

int s_region_create (s_region_t **region);
int s_region_destroy (s_region_t *region);
int s_region_addrect (s_region_t *region, s_rect_t *rect);
int s_region_num_rectangles (s_region_t *region);
s_rect_t * s_region_rectangles (s_region_t *region);

/* render.c */
typedef struct s_render_s s_render_t;
typedef struct s_render_trap_s s_render_trap_t;
typedef struct s_render_color_s s_render_color_t;
typedef struct s_render_matrix_s s_render_matrix_t;
typedef struct s_render_pixman_s s_render_pixman_t;

typedef enum S_RENDER_FORMAT {
	S_RENDER_FORMAT_NONE,
	S_RENDER_FORMAT_ARGB32,
	S_RENDER_FORMAT_RGB16,
	S_RENDER_FORMAT_RGB24,
	S_RENDER_FORMAT_A8,
	S_RENDER_FORMAT_A1
} S_RENDER_FORMAT;

typedef enum S_RENDER_OPERATOR {
	S_RENDER_OPERATOR_NONE,
	S_RENDER_OPERATOR_CLEAR,
	S_RENDER_OPERATOR_SRC,
	S_RENDER_OPERATOR_OVER,
	S_RENDER_OPERATOR_IN,
	S_RENDER_OPERATOR_OUT,
	S_RENDER_OPERATOR_ATOP,
	S_RENDER_OPERATOR_DST,
	S_RENDER_OPERATOR_OVER_REVERSE,
	S_RENDER_OPERATOR_IN_REVERSE,
	S_RENDER_OPERATOR_OUT_REVERSE,
	S_RENDER_OPERATOR_ATOP_REVERSE,
	S_RENDER_OPERATOR_XOR,
	S_RENDER_OPERATOR_ADD,
	S_RENDER_OPERATOR_SATURATE,
} S_RENDER_OPERATOR;

typedef enum S_RENDER_REPEAT {
	S_RENDER_REPEAT_NONE,
	S_RENDER_REPEAT_NORMAL,
	S_RENDER_REPEAT_REFLECT,
	S_RENDER_REPEAT_PAD,
} S_RENDER_REPEAT;

typedef enum S_RENDER_FILTER {
	S_RENDER_FILTER_NONE,
	S_RENDER_FILTER_FAST,
	S_RENDER_FILTER_GOOD,
	S_RENDER_FILTER_BEST,
	S_RENDER_FILTER_NEAREST,
	S_RENDER_FILTER_BILINEAR,
} S_RENDER_FILTER;

struct s_render_color_s {
	unsigned short red;
	unsigned short green;
	unsigned short blue;
	unsigned short alpha;
};

struct s_render_matrix_s {
	unsigned int matrix[3][3];
};

struct s_render_trap_s {
	int top;
	int bottom;
	int left1x;
	int left1y;
	int left2x;
	int left2y;
	int right1x;
	int right1y;
	int right2x;
	int right2y;
};

struct s_render_s {
	S_RENDER_FORMAT format;
	int width;
	int height;
	int stride;
	int depth;
	int has_clip;
	unsigned char *data;
	s_render_pixman_t *pixman;
};

int s_render_uninit (s_render_t *render);
int s_render_init (s_render_t **render, S_RENDER_FORMAT render_format, int width, int height);
int s_render_init_for_data (s_render_t **render, unsigned char *data, S_RENDER_FORMAT render_format, int width, int height, int bpp, int stride);
int s_render_set_filter (s_render_t *render, S_RENDER_FILTER filter);
int s_render_set_repeat (s_render_t *render, S_RENDER_REPEAT repeat);
int s_render_set_transform_matrix (s_render_t *render, s_render_matrix_t *matrix);
int s_render_add_trapezoid (s_render_t *render, int x_off, int y_off, int ntraps, s_render_trap_t *traps);
int s_render_set_clip (s_render_t *render, int nrects, s_rect_t *rects);
int s_render_fill_rectangles (S_RENDER_OPERATOR op, s_render_t *render, s_render_color_t *color, int nrects, s_rect_t *rects);
int s_render_composite (S_RENDER_OPERATOR op, s_render_t *source, s_render_t *mask, s_render_t *dest, int src_x, int src_y, int mask_x, int mask_y, int dest_x, int dest_y, int width, int height);

/** @defgroup client_socket Client Library - Socket API
  * @brief s_socket_* api is very internal, and is used for server / client
  *        interaction. every message type has its own function both for
  *        sending and receiving. programmers do not need to use these
  *        functions unless they do not want to modify or expand the inter
  *        messaging sub system. additionaly, the comminication initializations
  *        is alsa done by this api calls.
  */

/** @addtogroup client_socket */
/*@{*/

/** socket data types
  */  
typedef enum {
	/** nothing ;) */
	SOC_DATA_NOTHING      = 0x0,
	/** new window */
	SOC_DATA_NEW          = 0x1,
	/** show, hide */
	SOC_DATA_SHOW         = 0x2,
	/** close me request */
	SOC_DATA_CLOSE        = 0x3,
	/** close server request */
	SOC_DATA_CLOSE_SERVER = 0x4,
	/** event */
	SOC_DATA_EVENT        = 0x5,
	/** expose */
	SOC_DATA_EXPOSE       = 0x6,
	/** display properties */
	SOC_DATA_DISPLAY      = 0x7,
	/** request to re draw window form */
	SOC_DATA_FORMDRAW     = 0x8,
	/** configure */
	SOC_DATA_CONFIGURE    = 0x9,
	/** special data for desktop windows' */
	SOC_DATA_DESKTOP      = 0xa,
} S_SOC_DATA;

/* socket.c */

/** @brief send new window request to server
  *
  * @param *window - the window
  * @param soc     - window socket
  * @return 0 on success
  */
int s_socket_request_new (s_window_t *window, int soc);

/** @brief send display request to server
  *
  * @param *window - the window
  * @param soc     - window socket
  * @return 0 on success
  */
int s_socket_request_display (s_window_t *window, int soc);

/** @brief configure request to server
  *
  * @param *window - the window
  * @param soc     - window socket
  * @param form    - is form area will be calculated
  * @return 0 on success
  */
int s_socket_request_configure (s_window_t *window, int soc, int form);

/** @brief stream request to server
  *
  * @param *window - the window
  * @param soc     - window socket
  * @param coor    - coordinates to be updated
  * @return 0 on success
  */
int s_socket_request_stream (s_window_t *window, int soc, s_rect_t *coor);

/** @brief show request to server
  *
  * @param *window - the window
  * @param soc     - window socket
  * @param id      - requested window id
  * @param *title  - requested window title
  * @param show    - 1: show, 0: hide
  * @return 0 on success
  */
int s_socket_request_show (s_window_t *window, int soc, int id, char *title, int show);

/** @brief send event request to server
  *
  * @param *window - the window
  * @param soc     - window socket
  * @param *event  - event data to send
  * @return 0 on success
  */
int s_socket_request_event (s_window_t *window, int soc, s_event_t *event);

/** @brief send request to server
  *
  * @param *window - the window
  * @param req     - reuest type, followed with request params -if any-.
  * @return 0 on success
  */
int s_socket_request (s_window_t *window, S_SOC_DATA req, ...);

/** @brief recv, parse end process event data
  *
  * @param *window - the window
  * @param soc     - window socket
  * @returns 0 on success
  */
int s_socket_listen_event (s_window_t *window, int soc);

/** @brief recv, parse end process expose data
  *
  * @param *window - the window
  * @param soc     - window socket
  * @returns 0 on success
  */
int s_socket_listen_expose (s_window_t *window, int soc);

/** @brief recv, parse end process desktop data
  *
  * @param *window - the window
  * @param soc     - window socket
  * @returns 0 on success
  */
int s_socket_listen_desktop (s_window_t *window, int soc);

/** @brief recv, and call the processing function
  *
  * @param *window - the window
  * @param soc     - window socket
  * @returns 0 on success
  */
int s_socket_listen_parse (s_window_t *window, int soc);

/** @brief waits until any event occurs on socket, or timeout.
  *
  * @param *window - the window
  * @param timeout - timeout value in mili seconds
  * @returns 0 on success
  */
int s_socket_listen_wait (s_window_t *window, int timeout);

/** @brief uninitialize window socket
  *
  * @param *window - the window
  * @param *pfd    - pollfd handler for window socket
  * @returns 0 on success
  */
int s_socket_uninit (s_window_t *window, s_pollfd_t *pfd);

/** @brief pollfd in data call back function for window socket
  *
  * @param *window - the window
  * @param *pfd    - pollfd handler for window socket
  * @returns 0 on success
  */
int s_socket_in_f (s_window_t *window, s_pollfd_t *pfd);

/** @brief pollfd error call back function for window socket
  *
  * @param *window - the window
  * @param *pfd    - pollfd handler for window socket
  * @returns 0 on success
  */
int s_socket_ierr_f (s_window_t *window, s_pollfd_t *pfd);

/** @brief pollfd in data call back function for window wakeup socket
  *
  * @param *window - the window
  * @param *pfd    - pollfd handler for window socket
  * @returns 0 on success
  */
int s_socket_inw_f (s_window_t *window, s_pollfd_t *pfd);

/** @brief pollfd error call back function for window wakeup socket
  *
  * @param *window - the window
  * @param *pfd    - pollfd handler for window socket
  * @returns 0 on success
  */
int s_socket_ierrw_f (s_window_t *window, s_pollfd_t *pfd);

/** @brief pollfd close call back function for window wakeup socket
  *
  * @param *window - the window
  * @param *pfd    - pollfd handler for window socket
  * @returns 0 on success
  */
int s_socket_closew_f (s_window_t *window, s_pollfd_t *pfd);	

/** @brief initialize uds socket connection between server and window
  *
  * @param *window - the window
  * @returns 0 on success
  */
int s_socket_init_uds (s_window_t *window);

/** @brief initialize tcp socket connection between server and window
  *
  * @param *window - the window
  * @returns 0 on success
  */
int s_socket_init_tcp (s_window_t *window);

/** @brief initialize wakeup socket for window
  *
  * @param *window - the window
  * @returns 0 on success
  */
int s_socket_init_wakeup (s_window_t *window);

/** @brief initialize socket connection between server and window
  *
  * @param *window - the window
  * @returns 0 on success
  */
int s_socket_init (s_window_t *window);

/*@}*/

/** @defgroup client_surface Client Library - Surface API
  * @brief s_surface_* api is very internal, and is used for initializing the video buffer,
  *        that will print on. the initialization method is choosen by the server - linear
  *        video buffer, linear shadow buffer, streaming buffer -. programmers do not need
  *        to use these functions unless they do not want to change the behaviour of server/
  *        client interraction.
  */

/** @addtogroup client_surface */
/*@{*/

/** surface modes
  */
typedef enum {
	/** graphics will be drawn on real buffer */
	SURFACE_REAL       = 0x1,
	/** graphics will be drawn on virtual buffer */
	SURFACE_VIRTUAL    = 0x2,
	/** window will send expose event (just the rectangular area) to the server */
	SURFACE_NEEDEXPOSE = 0x4,
	/** window will send expose event (rectanglar area, and the buffer) to the server */
	SURFACE_NEEDSTREAM = 0x8
} S_SURFACE_MODE;

/** surface struct
  */
struct s_surface_s {
	/** bitwise ored SURFACE_REAL, SURFACE_VIRTUAL */
	S_SURFACE_MODE mode;
	/** surface buytes per pixel */
	unsigned int bytesperpixel;
	/** surface bits per pixel */
	unsigned int bitsperpixel;
	/** number of colors */
	unsigned int colors;
	/** blue color offset */
	unsigned int blueoffset;
	/** green color offset */
	unsigned int greenoffset;
	/** red color offset */
	unsigned int redoffset;
	/** blue color length */
	unsigned int bluelength;
	/** green color length */
	unsigned int greenlength;
	/** red color length */
	unsigned int redlength;
	/** blue color mask */
	unsigned int bluemask;
	/** green color mask */
	unsigned int greenmask;
	/** red color mask */
	unsigned int redmask;
	/** surface virtual buffer width */
	int width;
	/** surface virtual buffer height */
	int height;
	/** vbuf is external */
	unsigned int evbuf;
	/** surface's virtual buffer */
        unsigned char *vbuf;
	/** virtual buffers' seen part on screen */
	s_rect_t *buf;
	/** window coordinated that hold surface on screen (if any) */
	s_rect_t *win;
	/** memory mapped shared buffer, this is the readl video buffer (usually) */
	unsigned char *linear_buf;
	/** video buffer width */
	int linear_buf_width;
	/** video buffer pitch */
	int linear_buf_pitch;
	/** video buffer height */
	int linear_buf_height;
	/** video buffer linear mem base */
	unsigned int linear_mem_base;
	/** video buffer linear mem size */
	unsigned int linear_mem_size;
	/** shared memory id for screen matrix */
	int shm_mid;
	/** shared memory buffer for screen matrix */
	unsigned char *matrix;
	/** shared video memory id */
        int shm_sid;
	/** this is either, 0, SURFACE_NEEDSTREAM, or SURFACE_NEEDEXPOSE */
	S_SURFACE_MODE need_expose;
	/** device name */
	char *device;
	/** ugly hack for overlay operations, window->surface->id = &(window->client->id) */
        int *id;
	/** ugly hack, window->surface->window = window */
        s_window_t *window;
};

/* surface.c */

/** @brief initialize the main video buffer surface struct
  *
  * @param *window - the window
  * @returns 0 on success 
  */
int s_surface_init (s_window_t *window);

/** @brief create and attach the device buffer to surface
  *
  * @param *window - the window
  * @returns no return
  */
void s_surface_attach (s_window_t *window);

/** @brief attach to the matrix buffer
  *
  * @param *window - the window
  * @returns no return
  */
void s_surface_shm_attach (s_window_t *window);

/** @brief attach to the video buffer
  *
  * @param *window - the window
  * @returns no return
  */
void s_surface_linear (s_window_t *window);

/** @brief uninitialize the surface struct
  *
  * @param *window - the window
  * @returns no return
  */
void s_surface_uninit (s_window_t *window);

/** @brief expose event handler
  *
  * @param *window  - the window
  * @param *changed - the rectangular area that will be updated 
  * @returns no return
  */
void s_surface_changed (s_window_t *window, s_rect_t *changed);

/*@}*/

/** @defgroup client_thread Client Library - Thread API
  * @brief s_thread_* api is an abstract layer for system calls.
  *        programmer should not use s_thread_sem_* calls, becouse
  *        they are designed to be used internally - especially for
  *        mutex and condition variable emulation -, and not supported
  *        for every operating system.
  *
  * @example
  *
  * @code
  *
  * s_thread_mutex_t *mut;
  * s_thread_cond_t *cond;
  *
  * void * thread_function (void *arg)
  * {
  *	int quit = 0;
  *	int *val = (int *) arg;
  *	while (!quit) {
  *		s_thread_mutex_lock(mut);
  *		if (*val == 0) {
  *			quit = 1;
  *		} else {
  *			*val = *val + 1;
  *		}
  *		s_thread_cond_signal(cond);
  *		s_thread_mutex_unlock(mut);
  *		sleep(3);
  *	}
  *	return NULL;
  * }
  *
  * {
  *	int arg;
  *	int barg;
  *	int quit = 0;
  *	s_thread_t *tid;
  *
  *	arg = 1;
  *	if (s_thread_mutex_init(&mut)) {
  *		// error
  *	}
  *	if (s_thread_cond_init(&cond)) {
  *		// error
  *	}
  *	if (s_thread_mutex_lock(mut)) {
  *		// error
  *	}
  *	tid = s_thread_create(&thread_function, &arg);
  *	if (tid == NULL) {
  *		// error
  *	}
  *	barg = arg;
  *	if (s_thread_mutex_unlock(mut)) {
  *		// error
  *	}
  *	while (!quit) {
  *		if (s_thread_mutex_lock(mut)) {
  *			// error
  *		}
  *		while (barg == arg) {
  *			if (s_thread_cond_wait(cond, mut)) {
  *				// error
  *			}
  *		}
  *		if (arg == 10) {
  *			arg = 0;
  *			quit = 1;
  *		}
  *		barg = arg;
  *		if (s_thread_mutex_unlock(mut)) {
  *			// error
  *		}
  *	}
  *	if (s_thread_mutex_destroy(mut)) {
  *		// error
  *	}
  *	if (s_thread_cond_destroy(cond)) {
  *		// error
  *	}
  * }
  * @endcode
  */

/** @addtogroup client_thread */
/*@{*/

/* thread.c */

/** @brief initialize the semaphore struct with an initial value
  *
  * @param **sem   - address of the semaphore pointer.
  * @param initial - initial value.
  * @returns 0 on success, 1 on error.
  */
int s_thread_sem_create (s_thread_sem_t **sem, int initial);

/** @brief destroys the given semaphore
  *
  * @param *sem - address of the semaphore pointer.
  * @returns 0 on success, 1 on error.
  */
int s_thread_sem_destroy (s_thread_sem_t *sem);

/** @brief waits on given semaphore
  *
  * @param *sem - address of the semaphore pointer.
  * @returns 0 on success, 1 on error.
  */
int s_thread_sem_wait (s_thread_sem_t *sem);

/** @brief waits on given semaphore with timeout
  *
  * @param *sem - address of the semaphore pointer.
  * @param msec - timeout in mili seconds.
  * @returns 0 on success, 1 on timeout, -1 on error.
  */
int s_thread_sem_wait_timeout (s_thread_sem_t *sem, int msec);

/** @brief unlocks given semaphore
  *
  * @param *sem - address of the semaphore pointer.
  * @returns 0 on success, 1 on error.
  */
int s_thread_sem_post (s_thread_sem_t *sem);

/** @brief initialize the mutex struct
  *
  * @param **mut - address of the mutex pointer.
  * @returns 0 on success, 1 on error.
  */
int s_thread_mutex_init (s_thread_mutex_t **mut);

/** @brief destroys the given mutex
  *
  * @param *mut - address of the mutex pointer.
  * @returns 0 on success, 1 on error.
  */
int s_thread_mutex_destroy (s_thread_mutex_t *mut);

/** @brief locks the given mutex
  *
  * @param *mut - address of the mutex pointer.
  * @returns 0 on success, 1 on error.
  */
int s_thread_mutex_lock (s_thread_mutex_t *mut);

/** @brief tries to lock the given mutex
  *
  * @param *mut - address of the mutex pointer.
  * @returns 0 on success, 1 on error.
  */
int s_thread_mutex_trylock (s_thread_mutex_t *mut);

/** @brief unlocks the given mutex
  *
  * @param *mut - address of the mutex pointer.
  * @returns 0 on success, 1 on error.
  */
int s_thread_mutex_unlock (s_thread_mutex_t *mut);

/** @brief initialize the condition variable struct
  *
  * @param **cond - address of the condition variable pointer.
  * @returns 0 on success, 1 on error.
  */
int s_thread_cond_init (s_thread_cond_t **cond);

/** @brief destroys the given condition variable
  *
  * @param *cond - address of the condition variable pointer.
  * @returns 0 on success, 1 on error.
  */
int s_thread_cond_destroy (s_thread_cond_t *cond);

/** @brief signals waiter on given condition variable
  *
  * @param *cond - address of the condition variable pointer.
  * @returns 0 on success, 1 on error.
  */
int s_thread_cond_signal (s_thread_cond_t *cond);

/** @brief signals all waiters on given condition variable
  *
  * @param *cond - address of the condition variable pointer.
  * @returns 0 on success, 1 on error.
  */
int s_thread_cond_broadcast (s_thread_cond_t *cond);

/** @brief waits on condition variable
  *
  * @param *cond - address of the condition variable pointer.
  * @param *mut  - address of the mutex pointer.
  * @returns 0 on success, 1 on error.
  */
int s_thread_cond_wait (s_thread_cond_t *cond, s_thread_mutex_t *mut);

/** @brief waits on condition variable, with timeout
  *
  * @param *cond - address of the condition variable pointer.
  * @param *mut  - address of the mutex pointer.
  * @param *msec - timeout value in mili seconds.
  * @returns 0 on success, 1 on error.
  */
int s_thread_cond_timedwait (s_thread_cond_t *cond, s_thread_mutex_t *mut, int msec);

/** @brief creates a new thread of control that executes concurrently
  *        with the calling thread. The new thread applies the function
  *        passing it as first argument. 
  *
  * @param *f   - pointer to the function.
  * @param *farg - argument to pass to function.
  * @returns NULL on error, otherwise address of the thread id.
  */
s_thread_t * s_thread_create (void * (*f) (void *), void *farg);

/** @brief cancels the thread.
  *
  * @param *tid - thread id of the thread to cancel.
  * @returns 0 on success, 1 on error.
  */
int s_thread_cancel (s_thread_t *tid);

/** @brief suspends the execution of the calling thread until the
  *        thread identified by tid terminates, either by calling
  *        s_thread_exit or by being cancelled.
  *
  * @param *tid  - thread id of the thread to cancel.
  * @param **ret - return value of the thread.
  * @returns 0 on success, 1 on error.
  */
int s_thread_join (s_thread_t *tid, void **ret);

/** @brief returns the thread identifier for the calling thread.
  *
  * @returns th thread id.
  */
int s_thread_self (void);

/** @brief terminates the execution of the calling thread.
  */
void s_thread_exit (void *ret);

/*@}*/

/** @defgroup client_timer Client Library - Timer API
  * @brief s_timer_* api calls is designed for setting timers,
  *        and timer callbacks for any window.
  *        programmers should use these timers rather than using signals
  *        or looping threads to be thread safe. and do not forget that
  *        these timers are not real-time.
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

/** @addtogroup client_timer */
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
	void *data;
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

/** @defgroup client_window Client Library - Window API
  * @brief these are simple low-level windows api functions,
  *        for further information you may look in demo directory.
  *
  * @example
  *
  * for further information look in demo/ directory
  *
  * @code
  * void simple_atevent (s_window_t *window, s_event_t *event)
  * {
  * 	if (event->type & KEYBD_EVENT) {
  * 		debugf(DCLI, "[%d] event: 0x%08X, f: 0x%08X, b: %d, a:%c", window->id, event->type, event->keybd->flag, event->keybd->button, event->keybd->ascii);
  * 	}
  * 	if (event->type & MOUSE_EVENT) {
  * 		debugf(DCLI, "[%d] event->type : 0x%08X, button : 0x%08X, buttons : 0x%08X", window->id, event->type, event->mouse->b, event->mouse->buttons);
  * 	}
  * }
  * 
  * void simple_atexit (s_window_t *window)
  * {
  * 	debugf(DCLI, "[%d] exited", window->id);
  * }
  * 
  * int main (int argc, char *argv[])
  * {
  * 	int x = 60;
  * 	int y = 60;
  * 	int w = 400;
  * 	int h = 300;
  * 	int mw = 1000;
  * 	int mh = 1000;
  * 	s_window_t *window;
  * 	s_window_init(&window);
  * 	s_window_new(window, WINDOW_MAIN, NULL);
  * 	mw = window->surface->width;
  * 	mh = window->surface->height;
  * 	s_window_set_title(window, "Demo - %s", argv[0]);
  * 	s_window_set_coor(window, 0, x, y, MIN(mw, w), MIN(mh, h));
  *   	s_window_atevent(window, simple_atevent);
  * 	s_window_atexit(window, simple_atexit);
  * 	s_window_show(window);
  *   	s_window_main(window);
  * 	return 0;
  * }
  * @endcode
  */

/** @addtogroup client_window */
/*@{*/

/** window types
  */
typedef enum {
	/** dont calculate form area, for requested action */
	WINDOW_NOFORM     = 0x0040,
	/** window main */
	WINDOW_MAIN       = 0x0080,
	/** window temp */
	WINDOW_TEMP       = 0x0100,
	/** window child */
	WINDOW_CHILD      = 0x0200,
	/** bitwise or`ed window types */
	WINDOW_TYPES      = WINDOW_MAIN | WINDOW_TEMP | WINDOW_CHILD,
	/** window desktop, this is a bitwise or'ed property */
	WINDOW_DESKTOP    = 0x0400,
	/** window input, this is a bitwise or`ed property
	  * this type of window will receive all input events,
	  * even if not shown */
	WINDOW_INPUT      = 0x0800,
} S_WINDOW;

/** window struct
  */
struct s_window_s {
	/** is window running */
	int running;
	/** window's connection socket to the server */
	int soc;
	/** self wake up socket */
	int wsoc;
	/** bitwise or'ed window type */
	S_WINDOW type;

	/** window id */
	int id;
	/** window priority */
	int pri;
	/** window title */
	char *title;

	/** window resizeable property */
	int resizeable;
	/** window always ontop property, 0: normal, 1: ontop, -1:on buttom */
	int alwaysontop;
	/** window cursor */
	S_MOUSE_CURSOR cursor;
	/** is mouse over */
	int mouse_entered;

	/** surface attached to the window */
	s_surface_t *surface;

	/** event queue of the window */
	s_eventq_t *eventq;
	/** timers list of the window */
	s_timers_t *timers;
	/** childs list of the window */
	s_childs_t *childs;
	/** pollfds list of the window */
	s_pollfds_t *pollfds;
	/** handlers list of the window */
	s_handlers_t *handlers;

	/** thread id of the event dispatcher thread */
	s_thread_t *tid;
	/** points the window's parent window */
	s_window_t *parent;

	/** most recent event received from the server */
	s_event_t *event;
	/** socket request and listen mutex */
	s_thread_mutex_t *socket_mutex;
	
	/** used for self implementation gettext */
	s_gettext_t *gettext;

	/** atevent event callback */
	void (*atevent) (s_window_t *, s_event_t *);
	/** atexit callback */
	void (*atexit) (s_window_t *);

	/** user data */
	void *data;
};

/* window.c */

/** @brief sets the cursor for desired window
  *
  * @param *window - the window
  * @param cursor  - the cursor type
  * @returns no returns
  */
void s_window_set_cursor (s_window_t *window, S_MOUSE_CURSOR cursor);

/** @brief sets the title for desired window
  *
  * @param *window - the window
  * @param fmt     - the title
  * @returns no returns
  */
void s_window_set_title (s_window_t *window, char *fmt, ...);

/** @brief redraws the form of the desired window
  *
  * @param *window - the window
  * @returns no returns
  */
void s_window_form_draw (s_window_t *window);

/** @brief hides the given window
  *
  * @param *window - the window
  * @returns no returns
  */
void s_window_hide (s_window_t *window);

/** @brief shows the given window
  *
  * @param *window - the window
  * @returns no returns
  */
void s_window_show (s_window_t *window);

/** @brief moves the given window to the desired coordinates
  *
  * @param *window - the window
  * @param form    - is window form included in given coordinates
  * @param x       - position x
  * @param y       - position y
  * @param w       - width
  * @param h       - height
  * @returns no returns
  */
void s_window_set_coor (s_window_t *window, int form, int x, int y, int w, int h);

/** @brief sets the resizable property ot the given window
  *
  * @param *window    - the window
  * @param resizeable - 0 (enable) or 1 (disable)
  * @returns no returns
  */
void s_window_set_resizeable (s_window_t *window, int resizeable);

/** @brief sets the always on top property ot the given window
  *
  * @param *window     - the window
  * @param alwaysontop - 0 (regular), 1 (always on top), -1 (always on bottom)
  * @returns no returns
  */
void s_window_set_alwaysontop (s_window_t *window, int alwaysontop);

/** @brief creates a new window with the given type, attached to the window parent.
  *
  * @param *window - the window
  * @param type    - window type
  * @param parent  - parent window for the given window
  * @returns no returns
  */
int s_window_new (s_window_t *window, S_WINDOW type, s_window_t *parent);

/** @brief initiliazes the window struct, creates a connection to the server.
  *
  * @param **window - pointer to window pointer
  * @returns 0 on success
  */ 
int s_window_init (s_window_t **window);

/** @brief uninitializes the window struct.
  *
  * @param *window - pointer to window struct
  * @returns no return
  */ 
void s_window_uninit (s_window_t *window);

/** @brief exits the client window
  *
  * @param *window - pointer to window struct
  * @returns no return
  */ 
void s_window_exit (s_window_t *window);

/** @brief quits the client window
  *
  * @param *window - pointer to window struct
  * @returns no return
  */ 
void s_window_quit (s_window_t *window);

/** @brief wakes up the window
  *
  * @param *window - pointer to window struct
  * @returns 0 on success
  */ 
int s_window_wakeup (s_window_t *window);

/** @brief event loop thread function.
  *
  * @param *arg - thread argument
  * @returns no return
  */
void * s_window_loop_event (void *arg);

/** @brief main loop thread function, starts the even tool thread
  *
  * @param *arg - thread argument
  * @returns no return
  */
void * s_window_loop (void *arg);

/** @brief main functions that creates or tarts the loop function.
  *
  * @param *arg - thread argument
  * @returns no return
  */
void * s_window_main (void *arg);

/** @brief sets the atevent callback function for given window
  *
  * @param *window - the window
  * @param *f      - callback function
  * @returns no return
  */
void s_window_atevent (s_window_t *window, void (*f) (s_window_t *, s_event_t *));

/** @brief sets the atexit callback function for given window
  *
  * @param *window - the window
  * @param *f      - callback function
  * @returns no return
  */
void s_window_atexit (s_window_t *window, void (*f) (s_window_t *));

/*@}*/

/** @defgroup client_xml Client Library - XML API
  * @brief XML parse and handling api.
  *
  * @example
  *
  * for further information look in demo/ directory
  *
  * @code
  * <xml>
  * 	<a>
  * 		<b>b</b>
  * 		<c c0="c0" c1="c1">
  * 			<d>d</d>
  * 		</c>
  * 	</a>
  * 	<e>e</e>
  * </xml>
  * 
  * {
  * 	s_xml_node_t *xml;
  * 	s_xml_node_t *xml_e;
  * 	s_xml_node_t *xml_d;
  * 	s_xml_node_t *xml_c;
  * 	s_xml_node_attr_t *xml_c_attr;
  * 
  * 	s_xml_parse_file(&xml, &fle);
  * 
  * 	xml_d = s_xml_node_get_path(xml, "xml/a/c/d");
  * 	printf("xml/a/c/d value: %s\n", s_xml_node_get_value(xml_d);
  * 	xml_e = s_xml_node_get_path(xml, "xml/e");
  * 	printf("xml/e value: %s\n", s_xml_node_get_value(xml_e);
  * 
  * 	xml_c = s_xml_node_get_path(xml, "xml/a/c");
  * 	xml_c_attr = s_xml_node_get_attr(xml_c, "c0");
  * 	printf("xml/c attr c0 value: %s\n", s_xml_node_get_attr_value(xml_c, "c0"));
  * 
  * 	s_xml_node_uninit(xml);
  * }
  * @endcode
  */

/** @addtogroup client_xml */
/*@{*/

/* xml.c */

/** xml node attributes struct
  */
struct s_xml_node_attr_s {
	/** attr name */
	char *name;
	/** attr value */
	char *value;
	/** internal dontparse flag */
	int dontparse;
};

/** xml node struct
  */
struct s_xml_node_s {
	/** node name */
	char *name;
	/** node value */
	char *value;
	/** node attributes */
	s_list_t *attrs;
	/** child nodes */
	s_list_t *nodes;
	/** parent node */
	s_xml_node_t *parent;
	/** internal dontparse flag */
	int dontparse;
};

/** @brief return the node for given path, and head node
  *
  * @param *node - head node to start
  * @param *path - the requested path
  * @returns node for given path, or NULL if not found.
  */
s_xml_node_t * s_xml_node_get_path (s_xml_node_t *node, char *path);

/** @brief returns the value for given node
  *
  * @param *node - the node
  * @returns value for given node, or NULL if no value.
  */
char * s_xml_node_get_value (s_xml_node_t *node);

/** @brief return the node value for given path, and head node
  *
  * @param *node - head node to start
  * @param *path - the requested path
  * @returns value for given path, or NULL if not found.
  */
char * s_xml_node_get_path_value (s_xml_node_t *node, char *path);

/** @brief returns the attribute "attr", for given node
  *
  * @param *node - the node
  * @param *attr - requested attribute
  * @return the attribute, or NULL if not found
  */
s_xml_node_attr_t * s_xml_node_get_attr (s_xml_node_t *node, char *attr);

/** @brief returns the value for given node, and attribute
  *
  * @param *node - the noe
  * @param *attr - the attribute
  * @returns value for given node, or NULL if no value.
  */
char * s_xml_node_get_attr_value (s_xml_node_t *node, char *attr);

/** @brief dublicates the given node
  *
  * @param *node - node to dublicate
  * @param **dub - dublicated value
  * @returns 0 on success
  */
int s_xml_node_dublicate (s_xml_node_t *node, s_xml_node_t **dub);

/** @brief dublicates the given attribute
  *
  * @param *attr - node to dublicate
  * @param **dub - dublicated value
  * @returns 0 on success
  */
int s_xml_node_attr_dublicate (s_xml_node_attr_t *attr, s_xml_node_attr_t **dub);

/** @brief return the parent of node with the name "name"
  *
  * @param *node - the node
  * @param *name - name of the parent
  * @returns the parent, or NULL if not found
  */
s_xml_node_t * s_xml_node_get_parent (s_xml_node_t *node, char *name);

/** @brief initializes the attribute struct
  *
  * @param **attr - out value
  * @returns 0 on success
  */
int s_xml_node_attr_init (s_xml_node_attr_t **attr);

/** @brief uninitializes the attribute node
  *
  * @param *attr - the attribute
  * @returns 0 on success
  */
int s_xml_node_attr_uninit (s_xml_node_attr_t *attr);

/** @brief initializes the node struct
  *
  * @param **node - out value
  * @returns 0 on success
  */
int s_xml_node_init (s_xml_node_t **node);

/** @brief uninitializes the node
  *
  * @param *node - the node
  * @returns 0 on success
  */
int s_xml_node_uninit (s_xml_node_t *node);

/** @brief parses the given buffer
  *
  * @param **node  - out value
  * @param *buffer - buffer to parse
  * @param len     - buffer length
  * @returns 0 on success
  */
int s_xml_parse_buffer (s_xml_node_t **node, char *buffer, unsigned int len);

/** @brief parses the given file
  *
  * @param **node - out value
  * @param *file  - file to parse
  * @returns 0 on success
  */
int s_xml_parse_file (s_xml_node_t **node, char *file);

/*@}*/

#ifdef __cplusplus
	}
#endif

#endif /* XYNTH_H */
