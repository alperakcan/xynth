/***************************************************************************
    begin                : Tue Oct 14 2003
    copyright            : (C) 2003 - 2009 by Alper Akcan
    email                : alper.akcan@gmail.com
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
typedef struct s_region_s s_region_t;
typedef struct s_surface_s s_surface_t;
typedef struct s_thread_s s_thread_t;
typedef struct s_thread_sem_s s_thread_sem_t;
typedef struct s_thread_cond_s s_thread_cond_t;
typedef struct s_thread_mutex_s s_thread_mutex_t;
typedef struct s_timer_s s_timer_t;
typedef struct s_timers_s s_timers_t;
typedef struct s_window_s s_window_t;
typedef struct s_xml_node_attr_s s_xml_node_attr_t;
typedef struct s_xml_node_s s_xml_node_t;

typedef enum {
	CURSOR_TYPE_WAIT    = 0x0,
	CURSOR_TYPE_CROSS   = 0x1,
	CURSOR_TYPE_IBEAM   = 0x2,
	CURSOR_TYPE_SIZEV   = 0x3,
	CURSOR_TYPE_SIZEH   = 0x4,
	CURSOR_TYPE_SIZES   = 0x5,
	CURSOR_TYPE_SIZEB   = 0x6,
	CURSOR_TYPE_SIZEA   = 0x7,
	CURSOR_TYPE_ARROW   = 0x8,
	CURSOR_TYPE_POINT   = 0x9,
	CURSOR_TYPE_SPLITV  = 0xA,
	CURSOR_TYPE_SPLITH  = 0xB,
	CURSOR_TYPE_FORBID  = 0xC,
	CURSOR_TYPE_UPARROW = 0xD,
	CURSOR_TYPE_NONE    = 0xE,
	CURSOR_TYPE_MAX     = 0xF
} s_cursor_type_t;

typedef enum {
	KEYBOARD_FLAG_NONE	 = 0x00,
	KEYBOARD_FLAG_NUMLOCK	 = 0x01,
	KEYBOARD_FLAG_CAPSLOCK	 = 0x02,
	KEYBOARD_FLAG_LEFTCTRL	 = 0x04,
	KEYBOARD_FLAG_RIGHTCTRL	 = 0x08,
	KEYBOARD_FLAG_LEFTSHIFT	 = 0x10,
	KEYBOARD_FLAG_RIGHTSHIFT = 0x20,
	KEYBOARD_FLAG_LEFTALT	 = 0x40,
	KEYBOARD_FLAG_RIGHTALT	 = 0x80,
	KEYBOARD_FLAG_SHIFT	 = (KEYBOARD_FLAG_LEFTSHIFT | KEYBOARD_FLAG_RIGHTSHIFT),
	KEYBOARD_FLAG_CTRL	 = (KEYBOARD_FLAG_LEFTCTRL | KEYBOARD_FLAG_RIGHTCTRL),
	KEYBOARD_FLAG_ALT	 = (KEYBOARD_FLAG_LEFTALT | KEYBOARD_FLAG_LEFTALT),
	KEYBOARD_FLAG_ALTGR	 = KEYBOARD_FLAG_RIGHTALT
} s_keyboard_flag_t;

typedef enum {
/* Plain */
/*   0 */  KEYBOARD_BUTTON_ESCAPE,
/*   1 */  KEYBOARD_BUTTON_ONE,
/*   2 */  KEYBOARD_BUTTON_TWO,
/*   3 */  KEYBOARD_BUTTON_THREE,
/*   4 */  KEYBOARD_BUTTON_FOUR,
/*   5 */  KEYBOARD_BUTTON_FIVE,
/*   6 */  KEYBOARD_BUTTON_SIX,
/*   7 */  KEYBOARD_BUTTON_SEVEN,
/*   8 */  KEYBOARD_BUTTON_EIGHT,
/*   9 */  KEYBOARD_BUTTON_NINE,
/*  10 */  KEYBOARD_BUTTON_ZERO,
/*  11 */  KEYBOARD_BUTTON_MINUS,
/*  12 */  KEYBOARD_BUTTON_EQUAL,
/*  13 */  KEYBOARD_BUTTON_DELETE,
/*  14 */  KEYBOARD_BUTTON_TAB,
/*  15 */  KEYBOARD_BUTTON_q,
/*  16 */  KEYBOARD_BUTTON_w,
/*  17 */  KEYBOARD_BUTTON_e,
/*  18 */  KEYBOARD_BUTTON_r,
/*  19 */  KEYBOARD_BUTTON_t,
/*  20 */  KEYBOARD_BUTTON_y,
/*  21 */  KEYBOARD_BUTTON_u,
/*  22 */  KEYBOARD_BUTTON_i,
/*  23 */  KEYBOARD_BUTTON_o,
/*  24 */  KEYBOARD_BUTTON_p,
/*  25 */  KEYBOARD_BUTTON_BRACKETLEFT,
/*  26 */  KEYBOARD_BUTTON_BRACKETRIGHT,
/*  27 */  KEYBOARD_BUTTON_RETURN,
/*  28 */  KEYBOARD_BUTTON_LEFTCONTROL,
/*  29 */  KEYBOARD_BUTTON_a,
/*  30 */  KEYBOARD_BUTTON_s,
/*  31 */  KEYBOARD_BUTTON_d,
/*  32 */  KEYBOARD_BUTTON_f,
/*  33 */  KEYBOARD_BUTTON_g,
/*  34 */  KEYBOARD_BUTTON_h,
/*  35 */  KEYBOARD_BUTTON_j,
/*  36 */  KEYBOARD_BUTTON_k,
/*  37 */  KEYBOARD_BUTTON_l,
/*  38 */  KEYBOARD_BUTTON_SEMICOLON,
/*  39 */  KEYBOARD_BUTTON_APOSTROPHE,
/*  40 */  KEYBOARD_BUTTON_GRAVE,
/*  41 */  KEYBOARD_BUTTON_LEFTSHIFT,
/*  42 */  KEYBOARD_BUTTON_BACKSLASH,
/*  43 */  KEYBOARD_BUTTON_z,
/*  44 */  KEYBOARD_BUTTON_x,
/*  45 */  KEYBOARD_BUTTON_c,
/*  46 */  KEYBOARD_BUTTON_v,
/*  47 */  KEYBOARD_BUTTON_b,
/*  48 */  KEYBOARD_BUTTON_n,
/*  49 */  KEYBOARD_BUTTON_m,
/*  50 */  KEYBOARD_BUTTON_COMMA,
/*  51 */  KEYBOARD_BUTTON_PERIOD,
/*  52 */  KEYBOARD_BUTTON_SLASH,
/*  53 */  KEYBOARD_BUTTON_RIGHTSHIFT,
/*  54 */  KEYBOARD_BUTTON_KP_MULTIPLY,
/*  55 */  KEYBOARD_BUTTON_ALT,
/*  56 */  KEYBOARD_BUTTON_SPACE,
/*  57 */  KEYBOARD_BUTTON_CAPS_LOCK,
/*  58 */  KEYBOARD_BUTTON_F1,
/*  59 */  KEYBOARD_BUTTON_F2,
/*  60 */  KEYBOARD_BUTTON_F3,
/*  61 */  KEYBOARD_BUTTON_F4,
/*  62 */  KEYBOARD_BUTTON_F5,
/*  63 */  KEYBOARD_BUTTON_F6,
/*  64 */  KEYBOARD_BUTTON_F7,
/*  65 */  KEYBOARD_BUTTON_F8,
/*  66 */  KEYBOARD_BUTTON_F9,
/*  67 */  KEYBOARD_BUTTON_F10,
/*  68 */  KEYBOARD_BUTTON_NUM_LOCK,
/*  69 */  KEYBOARD_BUTTON_SCROLL_LOCK,
/*  70 */  KEYBOARD_BUTTON_KP_7,
/*  71 */  KEYBOARD_BUTTON_KP_8,
/*  72 */  KEYBOARD_BUTTON_KP_9,
/*  73 */  KEYBOARD_BUTTON_KP_SUBTRACT,
/*  74 */  KEYBOARD_BUTTON_KP_4,
/*  75 */  KEYBOARD_BUTTON_KP_5,
/*  76 */  KEYBOARD_BUTTON_KP_6,
/*  77 */  KEYBOARD_BUTTON_KP_ADD,
/*  78 */  KEYBOARD_BUTTON_KP_1,
/*  79 */  KEYBOARD_BUTTON_KP_2,
/*  80 */  KEYBOARD_BUTTON_KP_3,
/*  81 */  KEYBOARD_BUTTON_KP_0,
/*  82 */  KEYBOARD_BUTTON_KP_PERIOD,
/*  83 */  KEYBOARD_BUTTON_LAST_CONSOLE,
/*  84 */  KEYBOARD_BUTTON_LESS,
/*  85 */  KEYBOARD_BUTTON_F11,
/*  86 */  KEYBOARD_BUTTON_F12,
/*  87 */  KEYBOARD_BUTTON_KP_ENTER,
/*  88 */  KEYBOARD_BUTTON_RIGHTCONTROL,
/*  89 */  KEYBOARD_BUTTON_KP_DIVIDE,
/*  90 */  KEYBOARD_BUTTON_VOIDSYMBOL,
/*  91 */  KEYBOARD_BUTTON_ALTGR,
/*  92 */  KEYBOARD_BUTTON_BREAK,
/*  93 */  KEYBOARD_BUTTON_HOME,
/*  94 */  KEYBOARD_BUTTON_UP,
/*  95 */  KEYBOARD_BUTTON_PAGEUP,
/*  96 */  KEYBOARD_BUTTON_LEFT,
/*  97 */  KEYBOARD_BUTTON_RIGHT,
/*  98 */  KEYBOARD_BUTTON_END,
/*  99 */  KEYBOARD_BUTTON_DOWN,
/* 100 */  KEYBOARD_BUTTON_PAGEDOWN,
/* 101 */  KEYBOARD_BUTTON_INSERT,
/* 102 */  KEYBOARD_BUTTON_REMOVE,
/* 103 */  KEYBOARD_BUTTON_PAUSE,
/* Shift */
/* 104 */  KEYBOARD_BUTTON_EXCLAM,
/* 105 */  KEYBOARD_BUTTON_AT,
/* 106 */  KEYBOARD_BUTTON_NUMBERSIGN,
/* 107 */  KEYBOARD_BUTTON_DOLLAR,
/* 108 */  KEYBOARD_BUTTON_PERCENT,
/* 109 */  KEYBOARD_BUTTON_ASCIICIRCUM,
/* 110 */  KEYBOARD_BUTTON_AMPERSAND,
/* 111 */  KEYBOARD_BUTTON_ASTERISK,
/* 112 */  KEYBOARD_BUTTON_PARENLEFT,
/* 113 */  KEYBOARD_BUTTON_PARENRIGHT,
/* 114 */  KEYBOARD_BUTTON_UNDERSCORE,
/* 115 */  KEYBOARD_BUTTON_PLUS,
/* 116 */  KEYBOARD_BUTTON_Q,
/* 117 */  KEYBOARD_BUTTON_W,
/* 118 */  KEYBOARD_BUTTON_E,
/* 119 */  KEYBOARD_BUTTON_R,
/* 120 */  KEYBOARD_BUTTON_T,
/* 121 */  KEYBOARD_BUTTON_Y,
/* 122 */  KEYBOARD_BUTTON_U,
/* 123 */  KEYBOARD_BUTTON_I,
/* 124 */  KEYBOARD_BUTTON_O,
/* 125 */  KEYBOARD_BUTTON_P,
/* 126 */  KEYBOARD_BUTTON_BRACELEFT,
/* 127 */  KEYBOARD_BUTTON_BRACERIGHT,
/* 128 */  KEYBOARD_BUTTON_A,
/* 129 */  KEYBOARD_BUTTON_S,
/* 130 */  KEYBOARD_BUTTON_D,
/* 131 */  KEYBOARD_BUTTON_F,
/* 132 */  KEYBOARD_BUTTON_G,
/* 133 */  KEYBOARD_BUTTON_H,
/* 134 */  KEYBOARD_BUTTON_J,
/* 135 */  KEYBOARD_BUTTON_K,
/* 136 */  KEYBOARD_BUTTON_L,
/* 137 */  KEYBOARD_BUTTON_COLON,
/* 138 */  KEYBOARD_BUTTON_QUOTEDBL,
/* 139 */  KEYBOARD_BUTTON_ASCIITILDE,
/* 140 */  KEYBOARD_BUTTON_BAR,
/* 141 */  KEYBOARD_BUTTON_Z,
/* 142 */  KEYBOARD_BUTTON_X,
/* 143 */  KEYBOARD_BUTTON_C,
/* 144 */  KEYBOARD_BUTTON_V,
/* 145 */  KEYBOARD_BUTTON_B,
/* 146 */  KEYBOARD_BUTTON_N,
/* 147 */  KEYBOARD_BUTTON_M,
/* 148 */  KEYBOARD_BUTTON_GREATER,
/* 149 */  KEYBOARD_BUTTON_QUESTION,
/* 150 */  KEYBOARD_BUTTON_F13,
/* 151 */  KEYBOARD_BUTTON_F14,
/* 152 */  KEYBOARD_BUTTON_F15,
/* 153 */  KEYBOARD_BUTTON_F16,
/* 154 */  KEYBOARD_BUTTON_F17,
/* 155 */  KEYBOARD_BUTTON_F18,
/* 156 */  KEYBOARD_BUTTON_F19,
/* 157 */  KEYBOARD_BUTTON_F20,
/* 158 */  KEYBOARD_BUTTON_F21,
/* 159 */  KEYBOARD_BUTTON_F22,
/* 160 */  KEYBOARD_BUTTON_SHOW_MEMORY,
/* 161 */  KEYBOARD_BUTTON_F23,
/* 162 */  KEYBOARD_BUTTON_F24,
/* 163 */  KEYBOARD_BUTTON_SCROLL_BACKWARD,
/* 164 */  KEYBOARD_BUTTON_SCROLL_FORWARD,
/* ALTGR */
/* 165 */  KEYBOARD_BUTTON_CURRENCY,
/* 166 */  KEYBOARD_BUTTON_CENT,
/* 167 */  KEYBOARD_BUTTON_HEX_C,
/* 168 */  KEYBOARD_BUTTON_HEX_A,
/* 169 */  KEYBOARD_BUTTON_SHOW_REGISTERS,
/* 170 */  KEYBOARD_BUTTON_HEX_7,
/* 171 */  KEYBOARD_BUTTON_HEX_8,
/* 172 */  KEYBOARD_BUTTON_HEX_9,
/* 173 */  KEYBOARD_BUTTON_HEX_D,
/* 174 */  KEYBOARD_BUTTON_HEX_4,
/* 175 */  KEYBOARD_BUTTON_HEX_5,
/* 176 */  KEYBOARD_BUTTON_HEX_6,
/* 177 */  KEYBOARD_BUTTON_HEX_E,
/* 178 */  KEYBOARD_BUTTON_HEX_1,
/* 179 */  KEYBOARD_BUTTON_HEX_2,
/* 180 */  KEYBOARD_BUTTON_HEX_3,
/* 181 */  KEYBOARD_BUTTON_HEX_0,
/* 182 */  KEYBOARD_BUTTON_HEX_F,
/* 183 */  KEYBOARD_BUTTON_HEX_B,
/* User Defined */
/* 184 */  KEYBOARD_BUTTON_USER0,
/* 185 */  KEYBOARD_BUTTON_USER1,
/* 186 */  KEYBOARD_BUTTON_USER2,
/* 187 */  KEYBOARD_BUTTON_USER3,
/* 188 */  KEYBOARD_BUTTON_USER4,
/* 189 */  KEYBOARD_BUTTON_USER5,
/* 190 */  KEYBOARD_BUTTON_USER6,
/* 191 */  KEYBOARD_BUTTON_USER7,
/* 192 */  KEYBOARD_BUTTON_USER8,
/* 193 */  KEYBOARD_BUTTON_USER9,
/* 194 */  KEYBOARD_BUTTON_USER10,
/* 195 */  KEYBOARD_BUTTON_USER11,
/* 196 */  KEYBOARD_BUTTON_USER12,
/* 197 */  KEYBOARD_BUTTON_USER13,
/* 198 */  KEYBOARD_BUTTON_USER14,
/* 199 */  KEYBOARD_BUTTON_USER15,
/* 200 */  KEYBOARD_BUTTON_USER16,
/* 201 */  KEYBOARD_BUTTON_USER17,
/* 202 */  KEYBOARD_BUTTON_USER18,
/* 203 */  KEYBOARD_BUTTON_USER19,
/* 204 */  KEYBOARD_BUTTON_USER20,
/* 205 */  KEYBOARD_BUTTON_USER21,
/* 206 */  KEYBOARD_BUTTON_USER22,
/* 207 */  KEYBOARD_BUTTON_USER23,
/* 208 */  KEYBOARD_BUTTON_USER24,
/* 209 */  KEYBOARD_BUTTON_USER25,
/* 210 */  KEYBOARD_BUTTON_USER26,
/* 211 */  KEYBOARD_BUTTON_USER27,
/* 212 */  KEYBOARD_BUTTON_USER28,
/* 213 */  KEYBOARD_BUTTON_USER29,
/* 214 */  KEYBOARD_BUTTON_USER30,
/* 215 */  KEYBOARD_BUTTON_USER31,
/* 216 */  KEYBOARD_BUTTON_USER32,
/* 217 */  KEYBOARD_BUTTON_USER33,
/* 218 */  KEYBOARD_BUTTON_USER34,
/* 219 */  KEYBOARD_BUTTON_USER35,
/* 220 */  KEYBOARD_BUTTON_USER36,
/* 221 */  KEYBOARD_BUTTON_USER37,
/* 222 */  KEYBOARD_BUTTON_USER38,
/* 223 */  KEYBOARD_BUTTON_USER39,
/* 224 */  KEYBOARD_BUTTON_USER40,
/* 225 */  KEYBOARD_BUTTON_NOCODE,
/* 226 */  KEYBOARD_BUTTON_MAX
} s_keyboard_button_t;

typedef enum {
	MOUSE_BUTTON_RIGHT       = 0x00000001,
	MOUSE_BUTTON_MIDDLE      = 0x00000002,
	MOUSE_BUTTON_LEFT        = 0x00000004,
	MOUSE_BUTTON_FOURTH      = 0x00000008,
	MOUSE_BUTTON_FIFTH       = 0x00000010,
	MOUSE_BUTTON_SIXTH       = 0x00000020,
	MOUSE_BUTTON_RESET       = 0x00000040,
	MOUSE_BUTTON_WHEELX_UP   = 0x00000080,
	MOUSE_BUTTON_WHEELX_DOWN = 0x00000100,
	MOUSE_BUTTON_WHEELY_UP   = 0x00000200,
	MOUSE_BUTTON_WHEELY_DOWN = 0x00000400,

	MOUSE_BUTTON_USER0       = 0x00000800,
	MOUSE_BUTTON_USER1       = 0x00001000,
	MOUSE_BUTTON_USER2       = 0x00002000,
	MOUSE_BUTTON_USER3       = 0x00004000,
	MOUSE_BUTTON_USER4       = 0x00008000,
	MOUSE_BUTTON_USER5       = 0x00010000,
	MOUSE_BUTTON_USER6       = 0x00020000,
	MOUSE_BUTTON_USER7       = 0x00040000,
	MOUSE_BUTTON_USER8       = 0x00080000,
	MOUSE_BUTTON_USER9       = 0x00100000,
	MOUSE_BUTTON_USER10      = 0x00200000,

	MOUSE_BUTTON_NONE        = 0x00400000
} s_mouse_button_t;

typedef enum {
	EVENT_TYPE_UNKNOWN                = 0x00000000,
	EVENT_TYPE_QUIT                   = 0x00000001,
	EVENT_TYPE_KEYBOARD               = 0x00000002,
	EVENT_TYPE_KEYBOARD_RELEASED      = 0x00000004,
	EVENT_TYPE_KEYBOARD_PRESSED       = 0x00000008,
	EVENT_TYPE_KEYBOARD_EXTENSION_IRR = 0x00000010,
	EVENT_TYPE_MOUSE                  = 0x00000020,
	EVENT_TYPE_MOUSE_OVER             = 0x00000040,
	EVENT_TYPE_MOUSE_RELEASED         = 0x00000080,
	EVENT_TYPE_MOUSE_PRESSED          = 0x00000100,
	EVENT_TYPE_MOUSE_CLICKED          = 0x00000200,
	EVENT_TYPE_MOUSE_ENTER            = 0x00000400,
	EVENT_TYPE_MOUSE_EXIT             = 0x00000800,
	EVENT_TYPE_MOUSE_HINT1            = 0x00001000,
	EVENT_TYPE_MOUSE_HINT2            = 0x00002000,
	EVENT_TYPE_EXPOSE                 = 0x00004000,
	EVENT_TYPE_CONFIG                 = 0x00008000,
	EVENT_TYPE_CONFIG_X               = 0x00010000,
	EVENT_TYPE_CONFIG_Y               = 0x00020000,
	EVENT_TYPE_CONFIG_W               = 0x00040000,
	EVENT_TYPE_CONFIG_H               = 0x00080000,
	EVENT_TYPE_FOCUS                  = 0x00100000,
	EVENT_TYPE_FOCUS_IN               = 0x00200000,
	EVENT_TYPE_FOCUS_OUT              = 0x00400000,
	EVENT_TYPE_DESKTOP                = 0x00800000,
	EVENT_TYPE_TIMER                  = 0x01000000,
	EVENT_TYPE_REMOTE                 = 0x02000000,
	EVENT_TYPE_SIGNAL                 = 0x04000000,
	EVENT_TYPE_KEYBOARD_MASK          = (EVENT_TYPE_KEYBOARD | EVENT_TYPE_KEYBOARD_RELEASED | EVENT_TYPE_KEYBOARD_PRESSED | EVENT_TYPE_KEYBOARD_EXTENSION_IRR),
	EVENT_TYPE_MOUSE_MASK             = (EVENT_TYPE_MOUSE | EVENT_TYPE_MOUSE_OVER | EVENT_TYPE_MOUSE_RELEASED | EVENT_TYPE_MOUSE_PRESSED | EVENT_TYPE_MOUSE_CLICKED | EVENT_TYPE_MOUSE_HINT1 | EVENT_TYPE_MOUSE_HINT2),
	EVENT_TYPE_MASK                   = (EVENT_TYPE_QUIT | EVENT_TYPE_KEYBOARD | EVENT_TYPE_MOUSE | EVENT_TYPE_EXPOSE | EVENT_TYPE_CONFIG | EVENT_TYPE_FOCUS | EVENT_TYPE_DESKTOP | EVENT_TYPE_TIMER),
} s_event_type_t;

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
	s_mouse_button_t b;
	int clicks;		  /* click count	       */
	s_mouse_button_t buttons; /* buttons bitwise ORed      */
	long long time;		  /* event time	 	       */
	int px;			  /* prev. pressed coor.       */
	int py;
	int pb;			  /* prev. pressed button      */
	s_mouse_button_t pbuttons;/* prev. pressed buttons     */
	long long ctime;	  /* prev. clicked time	       */
	s_cursor_type_t cursor;
} s_mouse_t;

typedef struct s_keybd_s {
	int ascii;
	int button;
	int scancode;
	s_keyboard_button_t keycode;
	int state[KEYBOARD_BUTTON_MAX];
	long long time;
	s_keyboard_flag_t flag;
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
	s_event_type_t type;
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
	/** event window */
	s_window_t *window;
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
void s_trace_start (void);
void s_trace_stop (void);

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
int s_font_init (s_font_t **font, const char *name);
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
  * @param **surface    - the surface
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
  * @param **surface    - the surface
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

/** @brief set a clip region for surface. region is dublicated so caller must
  *        destroy region, not to allow memory leaks. a NULL region indicates
  *        that no clipping required while drawing on surface.
  *
  * @param *surface - the surface
  * @param *region - the region
  * @return 0 on succuess.
  */
int s_surface_set_clip_region (s_surface_t *surface, s_region_t *region);

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
  * 	hndl->mouse.button = MOUSE_BUTTON_LEFT;
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
	s_event_type_t hstate;
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

/* region.c */

struct s_rect_s {
	int x;
	int y;
	int w;
	int h;
};

struct s_region_s {
	s_rect_t extents;
	int nrects;
	s_rect_t *rects;
};

int s_region_create (s_region_t **region);
int s_region_destroy (s_region_t *region);
int s_region_clear (s_region_t *region);
int s_region_extents_calculate (s_region_t *region);
int s_region_intrect (s_region_t *region, s_rect_t *rect, s_region_t *result);
int s_region_subrect (s_region_t *region, s_rect_t *rect);
int s_region_delrect (s_region_t *region, s_rect_t *rect);
int s_region_addrect (s_region_t *region, s_rect_t *rect);
int s_region_intregion (s_region_t *region, s_region_t *intersect, s_region_t *result);
int s_region_subregion (s_region_t *region, s_region_t *sub);
int s_region_delregion (s_region_t *region, s_region_t *del);
int s_region_addregion (s_region_t *region, s_region_t *add);
int s_region_num_rectangles (s_region_t *region);
s_rect_t * s_region_rectangles (s_region_t *region);
int s_region_extents (s_region_t *region, s_rect_t *extents);
int s_region_copy (s_region_t **region, s_region_t *src);
int s_region_unify (s_region_t *region);
int s_region_reverse (s_region_t *region, s_region_t **result);
int s_region_combine (s_region_t *region);
int s_region_rect_union (s_rect_t *rect1, s_rect_t *rect2, s_rect_t *result);
int s_region_rect_intersect (s_rect_t *rect1, s_rect_t *rect2, s_rect_t *result);
int s_region_rect_substract (s_rect_t *rect1, s_rect_t *rect2, s_region_t *result);

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

/** @brief get name of data
  *
  * @param sdata - the data
  * @return pointer to name of data
  */
const char * s_socket_data_to_name (S_SOC_DATA sdata);

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
	/** surface clip region */
	s_region_t *clip;
	/** vbuf is external */
	unsigned int evbuf;
	/** surface's virtual buffer */
	unsigned char *vbuf;
	/** virtual buffers' seen part on screen */
	s_rect_t *buf;
	/** window coordinate that hold surface on screen (if any) */
	s_rect_t *win;
	/** memory mapped shared buffer, this is the real video buffer (usually) */
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
	/** paint mutex */
	s_thread_mutex_t *paint_mutex;
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
  * @returns 0 on success
  */
int s_surface_attach (s_window_t *window);

/** @brief attach to the matrix buffer
  *
  * @param *window - the window
  * @returns no return
  */
void s_surface_attach_matrix (s_window_t *window);

/** @brief attach to the video buffer
  *
  * @param *window - the window
  * @returns no return
  */
void s_surface_attach_buffer (s_window_t *window);

/** @brief uninitialize the surface struct
  *
  * @param *window - the window
  * @returns no return
  */
void s_surface_uninit (s_window_t *window);

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
int s_surface_clip_virtual (s_surface_t *surface, int x, int y, int w, int h, s_rect_t *coor);

/** @brief performs clip on rectangle (x, y, w, h).
  *        clip region is the surface's real buffer area
  *
  * @param *surface - surface
  * @param x        - x
  * @param y        - y
  * @param w        - w
  * @param h        - h
  * @param *coor    - on success coor will be the clipped area
  * @returns 0 on success, 1 on error.
  */
int s_surface_clip_real (s_surface_t *surface, int x, int y, int w, int h, s_rect_t *coor);

/** @brief expose event handler
  *
  * @param *window  - the window
  * @param *changed - the rectangular area that will be updated
  * @param all      - do not do matrix check
  * @returns no return
  */
void s_surface_changed (s_window_t *window, s_rect_t *changed, int all);

/** @brief set window surface coordinates
  *
  * @param *window  - the window
  * @param x - x position
  * @param y - y position
  * @param w - width for surface
  * @param h - height for surface
  * @returns no return
  */
void s_surface_set_coor (s_window_t *window, int x, int y, int w, int h);

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

/** @brief detachs the given thread
  *
  * @param *tid  - thread id of the thread to detach.
  * @returns 0 on success, 1 on error.
  */
int s_thread_detach (s_thread_t *tid);

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
  * 	if (event->type & EVENT_TYPE_KEYBOARD) {
  * 		debugf(DCLI, "[%d] event: 0x%08X, f: 0x%08X, b: %d, a:%c", window->id, event->type, event->keybd->flag, event->keybd->button, event->keybd->ascii);
  * 	}
  * 	if (event->type & EVENT_TYPE_MOUSE) {
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
  * 	s_window_new(window, WINDOW_TYPE_MAIN, NULL);
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
	WINDOW_TYPE_NOFORM  = 0x0040,
	/** window main */
	WINDOW_TYPE_MAIN    = 0x0080,
	/** window temp */
	WINDOW_TYPE_TEMP    = 0x0100,
	/** window child */
	WINDOW_TYPE_CHILD   = 0x0200,
	/** window popup */
	WINDOW_TYPE_POPUP   = 0x0400,
	/** bitwise or`ed window types */
	WINDOW_TYPES        = WINDOW_TYPE_MAIN | WINDOW_TYPE_TEMP | WINDOW_TYPE_CHILD | WINDOW_TYPE_POPUP,
	/** window desktop, this is a bitwise or'ed property */
	WINDOW_TYPE_DESKTOP = 0x0800,
	/** window input, this is a bitwise or`ed property
	  * this type of window will receive all input events,
	  * even if not shown */
	WINDOW_TYPE_INPUT   = 0x1000,
} s_window_type_t;

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
	s_window_type_t type;

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
	s_cursor_type_t cursor;
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
void s_window_set_cursor (s_window_t *window, s_cursor_type_t cursor);

/** @brief sets the title for desired window
  *
  * @param *window - the window
  * @param fmt     - the title
  * @returns no returns
  */
void s_window_set_title (s_window_t *window, const char *fmt, ...);

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
int s_window_new (s_window_t *window, s_window_type_t type, s_window_t *parent);

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
s_xml_node_t * s_xml_node_get_path (s_xml_node_t *node, const char *path);

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
char * s_xml_node_get_path_value (s_xml_node_t *node, const char *path);

/** @brief returns the attribute "attr", for given node
  *
  * @param *node - the node
  * @param *attr - requested attribute
  * @return the attribute, or NULL if not found
  */
s_xml_node_attr_t * s_xml_node_get_attr (s_xml_node_t *node, const char *attr);

/** @brief returns the value for given node, and attribute
  *
  * @param *node - the noe
  * @param *attr - the attribute
  * @returns value for given node, or NULL if no value.
  */
char * s_xml_node_get_attr_value (s_xml_node_t *node, const char *attr);

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
