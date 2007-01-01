/***************************************************************************
    begin                : Thu Feb 2 2006
    copyright            : (C) 2006 - 2007 by Alper Akcan
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

#if defined(VIDEO_GP2X)

enum {
	GP2X_UP = 0x1,           GP2X_LEFT = 0x4,	    GP2X_DOWN = 0x10,   GP2X_RIGHT = 0x40,
        GP2X_START = (1 << 8),   GP2X_SELECT = (1 << 9),    GP2X_L = (1 << 10), GP2X_R = (1 << 11),
        GP2X_A = (1 << 12),      GP2X_B = (1 << 13),        GP2X_X = (1 << 14), GP2X_Y = (1 << 15),
        GP2X_VOL_UP = (1 << 23), GP2X_VOL_DOWN = (1 << 22), GP2X_PUSH = (1 << 27)
};

typedef struct s_video_gp2x_data_s {
	int mouse_fd[2];
	int keybd_fd[2];
	s_thread_t *event_tid;
	unsigned short *gp2x_io;
} s_video_gp2x_data_t;

int s_video_gp2x_kbd_init (s_server_conf_t *cfg);
int s_video_gp2x_kbd_update (s_video_input_data_t *keybd);
void s_video_gp2x_kbd_uninit (void);

int s_video_gp2x_mouse_init (s_server_conf_t *cfg);
void s_video_gp2x_mouse_setcursor (S_MOUSE_CURSOR c);
void s_video_gp2x_mouse_draw (void);
int s_video_gp2x_mouse_update (s_video_input_data_t *mouse);
void s_video_gp2x_mouse_uninit (void);

void s_video_gp2x_server_goto_back (void);
void s_video_gp2x_server_comefrom_back (void);
void s_video_gp2x_server_restore (void);
void s_video_gp2x_server_uninit (void);
int s_video_gp2x_server_init (s_server_conf_t *cfg);
void s_video_gp2x_server_surface_update (s_rect_t *coor);
void s_video_gp2x_server_fullscreen (void);

void * s_video_gp2x_event_parse (void *arg);

#endif /* VIDEO_GP2X */
