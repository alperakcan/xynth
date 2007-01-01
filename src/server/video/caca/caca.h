/***************************************************************************
    begin                : Tue Dec 26 2006
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

#if defined(VIDEO_CACA)

#include <caca.h>

typedef struct s_video_caca_data_s {
	int mouse_fd[2];
	int keybd_fd[2];
	int caca_width;
	int caca_height;
	int screen_shm_mid;
	s_thread_t *event_tid;
} s_video_caca_data_t;

void * s_video_caca_event_parse (void *arg);

int s_video_caca_kbd_init (s_server_conf_t *cfg);
int s_video_caca_kbd_update (s_video_input_data_t *keybd);
void s_video_caca_kbd_uninit (void);

int s_video_caca_mouse_init (s_server_conf_t *cfg);
int s_video_caca_mouse_update (s_video_input_data_t *mouse);
void s_video_caca_mouse_uninit (void);

void s_video_caca_server_goto_back (void);
void s_video_caca_server_comefrom_back (void);
void s_video_caca_server_restore (void);
void s_video_caca_server_uninit (void);
int s_video_caca_server_init (s_server_conf_t *cfg);
void s_video_caca_server_surface_update (s_rect_t *coor);
void s_video_caca_server_fullscreen (void);

#endif /* VIDEO_CACA */
