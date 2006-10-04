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

#ifndef SSERVER_H
#define SSERVER_H

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

typedef struct s_mouse_driver_s {
	int x;
	int y;
	int buttons;
} s_mouse_driver_t;

typedef struct s_keybd_driver_s {
	int state;
	int button;
	int keycode;
	int scancode;
	int ascii;
} s_keybd_driver_t;

typedef enum {
	VIDEO_INPUT_NONE,
	VIDEO_INPUT_MOUSE,
	VIDEO_INPUT_KEYBD,
} VIDEO_INPUT;

typedef union s_video_input_data_u {
	s_keybd_driver_t keybd;
	s_mouse_driver_t mouse;
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
	char *origin_vbuf;
	int rotate_shm_id;
} s_server_t;

s_server_t *server;

/* event.c */
void s_server_event_parse_keyboard (s_keybd_driver_t *keybd);
int s_server_event_parse_mouse (s_mouse_driver_t *mouse);
int s_event_changed_ (s_window_t *window);
void s_server_event_changed (void);

/* id.c */
int s_server_id_get (void);
int s_server_id_find (int soc);
void s_server_id_del (int id);

/* kbd.c */
void s_server_kbd_switch_handler (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_kbd_window_close_handler (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_kbd_server_quit_handler (s_window_t *window, s_event_t *event, s_handler_t *handler);
int s_server_kbd_update (s_window_t *window, s_pollfd_t *pfd);
void s_server_kbd_init (s_server_conf_t *cfg, s_video_input_t *keybd);
int s_server_kbd_uninit (s_window_t *window, s_pollfd_t *pfd);

/* mouse.c */
void s_server_cursor_uninit (void);
void s_server_cursor_init (void);
void s_server_cursor_image_set (int which, int c0, int c1, unsigned int *c);
void s_server_cursor_matrix_add (void);
void s_server_cursor_draw (void);
void s_server_cursor_select (S_MOUSE_CURSOR c);
void s_server_cursor_position (int x, int y);
int s_mouse_getx (void);
int s_mouse_gety (void);
void s_mouse_setxrange (s_window_t *window, int a, int b);
void s_mouse_setyrange (s_window_t *window, int a, int b);
void s_server_mouse_setcursor (S_MOUSE_CURSOR c);
void s_server_mouse_draw (void);
int s_server_mouse_uninit (s_window_t *window, s_pollfd_t *pfd);
int s_server_mouse_update (s_window_t *window, s_pollfd_t *pfd);
void s_server_mouse_init (s_server_conf_t *cfg, s_video_input_t *mouse);

/* priority.c */
void s_server_pri_set (S_SURFACE_CHNGF flag, ...);
void s_server_pri_set_ (S_SURFACE_CHNGF flag, int id, s_rect_t *c0, s_rect_t *c1);
int s_server_id_pri (int id);
int s_server_pri_id (int pri);
void s_server_pri_del (int id);

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

/* single.c */
#if defined(SINGLE_APP)
void * s_server_single_app_start (void *arg);
void s_server_single_start (void);
void s_server_single_stop (void);
#endif

/* socket.c */
int s_server_socket_listen_new (int id);
int s_server_socket_listen_display (int id);
int s_server_socket_listen_configure (int id);
int s_server_socket_listen_stream (int id);
int s_server_socket_listen_close (int id);
int s_server_socket_listen_show (int id);
int s_server_socket_listen_event (int id);
int s_server_socket_listen_window_close (int soc);
int s_server_socket_listen_parse (int soc);
int s_server_socket_client_in_f (s_window_t *window, s_pollfd_t *pfd);
int s_server_socket_client_ierr_f (s_window_t *window, s_pollfd_t *pfd);
int s_server_socket_listen_accept (int soc);
int s_server_socket_request_event (int id);
int s_server_socket_request_close (int id);
int s_server_socket_request_expose (int id, s_rect_t *changed);
int s_server_socket_request_desktop (int id);
int s_server_socket_request (S_SOC_DATA req, int id, ...);
int s_server_socket_uninit (s_window_t *window, s_pollfd_t *pfd);
int s_server_socket_in_f (s_window_t *window, s_pollfd_t *pfd);
int s_server_socket_ierr_f (s_window_t *window, s_pollfd_t *pfd);
int s_server_socket_init_uds (void);
int s_server_socket_init_tcp (void);
int s_server_socket_init_pipe (void);
void s_server_socket_init (void);

/* surface.c */
void s_server_surface_matrix_find (s_rect_t *coor, int *dm);
void s_server_surface_matrix_add_this (int id, s_rect_t *coor, s_rect_t *mcoor, unsigned char *mat);
void s_server_surface_matrix_add_id (int id, s_rect_t *coor);
void s_server_surface_matrix_add (int id, s_rect_t *coor);
void s_server_surface_matrix_del (int id);
void s_server_surface_matrix_del_coor (s_rect_t *coor);
void s_server_surface_clean (s_rect_t *coor);
void s_server_surface_background (s_rect_t *coor);
void s_server_surface_lock_real (void);
void s_server_surface_refresh (void);

/* window.c */
void s_server_window_new (int id);
void s_server_window_title (int id, char *title);
void s_server_putbox (s_window_t *window, int id, s_rect_t *coor, int x, int y, s_image_t *img);
void s_server_putmat (s_window_t *window, int id, s_rect_t *coor, int x, int y, s_image_t *img);
int s_server_window_form_mat_verbose (int id);
void s_server_window_form_mat (int v, int id, int mi, s_rect_t *coor, void (*func) (s_window_t *, int, s_rect_t *, int, int, s_image_t *));
void s_server_window_form (int id, s_rect_t *_coor_);
void s_server_window_matrix (int id, int mi, s_rect_t *_coor_);
void s_server_window_matrix_add (int id, s_rect_t *_coor_);
void s_server_window_matrix_del (int id, s_rect_t *_coor_);
void s_server_window_calculate (int id);
int s_server_window_is_parent_temp (int pid, int cid);
int s_server_window_temp_parent (int cid);
void s_server_window_close_temps (int id);
void s_server_window_close_id (int id);
void s_server_window_hide_id (int id);
void s_server_window_close (s_window_t *window);
void s_server_window_move_resize (int id, s_rect_t *new);
void s_server_window_maximize (s_window_t *window);

/* window_handler.c */
void s_server_window_btn_resize_oh (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_resize_u_o (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_resize_ur_o (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_resize_r_o (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_resize_dr_o (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_resize_d_o (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_resize_dl_o (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_resize_l_o (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_resize_ul_o (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_menu_p (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_menu_oh (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_menu_ho (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_menu_r (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_hide_p (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_hide_oh (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_hide_ho (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_hide_r (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_maximize_p (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_maximize_oh (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_maximize_ho (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_maximize_r (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_close_p (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_close_oh (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_close_ho (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_close_r (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_move (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_resize_up (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_resize_up_left (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_resize_up_right (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_resize_left (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_resize_right (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_resize_down (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_resize_down_left (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_resize_down_right (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_handlers_del_mouse (void);
void s_server_window_handlers_add_mouse (int id);

/* window_move_resize.c */
void s_server_window_lines_draw_ (s_rect_t *coor, s_rect_t *rect, int c);
void s_server_window_lines_draw (s_rect_t *lnew);
void s_server_window_lines_clear_ (s_rect_t *told, s_rect_t *tnew);
void s_server_window_lines_clear (s_rect_t *lold, s_rect_t *lnew);
void s_server_window_while (s_rect_t *move, int flag);
void s_server_window_finish (int id, s_rect_t *move);
void s_server_window_move (s_window_t *window);
void s_server_window_resize_up (s_window_t *window);
void s_server_window_resize_up_left (s_window_t *window);
void s_server_window_resize_left (s_window_t *window);
void s_server_window_resize_down_left (s_window_t *window);
void s_server_window_resize_down (s_window_t *window);
void s_server_window_resize_down_right (s_window_t *window);
void s_server_window_resize_right (s_window_t *window);
void s_server_window_resize_up_right (s_window_t *window);

/* theme.c */
void s_server_theme_init (void);
void s_server_theme_set (char *name);
void s_server_theme_uninit (void);

#endif
