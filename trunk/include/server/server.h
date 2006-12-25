/***************************************************************************
    begin                : Sun Feb 16 2003
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

#ifndef SERVER_H_
#define SERVER_H_

typedef enum {
	TOP_L	= 0x0,
	TOP_1,
	TOP_2,
	TOP_3,
	TOP_4,
	TOP_5,
	TOP_R,
	LEFT,
	RIGHT,
	BTM_L,
	BTM,
	BTM_R,
	FORM_MAX
} THEME_FORM;

typedef enum {
	CLOSE	= 0x0,
	MAXIMIZE,
	HIDE,
	MENU,
	BTNS_MAX
} THEME_BTN;

typedef enum {
	INACTIVE,
	ACTIVE,
	PRESSED
} THEME_STATE;

typedef struct s_theme_s {
	int title_full;
	int text_color[2];
	int text_v_off[2];
	s_image_t form[2][FORM_MAX];
	s_image_t button[3][BTNS_MAX];
	struct {
		int h;
		int w;
		int w_;
	} form_min;
	s_font_t *font[2];
} s_theme_t;

typedef enum {
	SURFACE_CLOSE	 = 0x1,
	SURFACE_FOCUS	 = 0x2,
	SURFACE_REDRAW	 = 0x3,
	SURFACE_REFRESH	 = 0x4,
	SURFACE_CHANGED	 = 0x5
} S_SURFACE_CHNGF;

typedef struct s_clients_s {
	int soc;
	int pid; /* parent id */
	int resizeable;
	int alwaysontop;
	S_MOUSE_CURSOR cursor;
	S_WINDOW type;
	s_rect_t buf;
	s_rect_t win;
	/* theme related */
	struct {
		char *str;
		int hy[2];
		int hh[2];
		s_image_t img[2];
	} title;
	s_rect_t form[FORM_MAX];
	s_rect_t button[BTNS_MAX];
} s_clients_t;

typedef struct s_cursor_s {
	int sw;
	int x;
	int y;
	int xyid;
	int xyid_old;
	s_image_t *img;
	s_image_t images[MOUSE_CURSOR_MAX];
} s_cursor_t;

typedef struct s_server_conf_s {
	struct {
		char *driver;
		char *mode;
		int rotate;
	} general;
	struct {
		char *type;
		char *device;
		int samplerate;
		int scale;
	} mouse;
	struct {
		char *keyboard;
	} keyboard;
	struct {
		char *type;
		char *device;
		char *brate;
	} irr;
	struct {
		float horizsync_min;
		float horizsync_max;
		float vertrefresh_min;
		float vertrefresh_max;
		unsigned int pixclock;
		unsigned int left_margin;
		unsigned int right_margin;
		unsigned int upper_margin;
		unsigned int lower_margin;
		unsigned int hsync_len;
		unsigned int vsync_len;
		unsigned int vmode;
	} monitor;
} s_server_conf_t;

typedef struct s_video_input_data_mouse_s {
	int x;
	int y;
	int buttons;
} s_video_input_data_mouse_t;

typedef struct s_video_input_data_keybd_s {
	int state;
	int button;
	int keycode;
	int scancode;
	int ascii;
} s_video_input_data_keybd_t;

typedef enum {
	VIDEO_INPUT_NONE,
	VIDEO_INPUT_MOUSE,
	VIDEO_INPUT_KEYBD,
	VIDEO_INPUT_IRR,
} VIDEO_INPUT;

typedef union s_video_input_data_u {
	s_video_input_data_mouse_t mouse;
	s_video_input_data_keybd_t keybd;
	s_video_input_data_keybd_t irr;
} s_video_input_data_t;

typedef struct s_video_input_s {
	VIDEO_INPUT type;
	int (*init) (s_server_conf_t *cfg);
	int (*update) (s_video_input_data_t *idata);
	void (*uninit) (void);
} s_video_input_t;

typedef struct s_video_driver_s {
	char *driver;
	char *device;

	s_video_input_t **input;

	int (*server_init) (s_server_conf_t *cfg);
	void (*server_uninit) (void);
	void (*server_goto_back) (void);
	void (*server_comefrom_back) (void);
	void (*server_restore) (void);
	void (*server_surface_update) (s_rect_t *coor);
	void (*server_fullscreen) (void);

	void *driver_data;
} s_video_driver_t;

typedef struct s_server_s {
	int mode;
	int id[S_CLIENTS_MAX];
	int pri[S_CLIENTS_MAX];
	s_clients_t client[S_CLIENTS_MAX];
	s_cursor_t cursor;
        s_theme_t theme;
        s_window_t *window;
	s_video_driver_t *driver;

	int mouse_rangex[2];
	int mouse_rangey[2];

	s_handler_t *whndl[20]; /* window move & resize handlers        */
	int mh;                 /* mouse hint (search for server->mh)   */
	int ph;			/* priority hint (search for server->ph */
	int rotate;
	int origin_w;
	int origin_h;
	unsigned char *origin_vbuf;
	int rotate_shm_id;
} s_server_t;

s_server_t *server;

#include "server/event.h"
#include "server/id.h"
#include "server/irr.h"
#include "server/kbd.h"
#include "server/mouse.h"
#include "server/priority.h"
#include "server/single.h"
#include "server/socket.h"
#include "server/surface.h"
#include "server/window.h"
#include "server/window_handler.h"
#include "server/window_move_resize.h"
#include "server/theme.h"

/* server.c */
int s_server_cfg_check_digit (char *ptr, char *digits);
char * s_server_cfg_token (char **ptr);
int s_server_cfg (s_server_conf_t *config);
int s_server_init (void);
void s_server_quit (s_window_t *window);
void s_server_uninit (void);
void s_server_goto_back (void);
void s_server_comefrom_back (void);
void s_server_restore (void);
void s_server_fullscreen (void);
void s_server_surface_update (s_rect_t *coor);

#endif /*SERVER_H_*/
