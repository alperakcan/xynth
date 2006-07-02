/***************************************************************************
    begin                : Mon Feb 21 2005
    copyright            : (C) 2005 - 2006 by Alper Akcan
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

#include <SDL.h>

typedef struct s_video_sdl_data_s {
	int mouse_fd[2];
	int keybd_fd[2];
	int screen_shm_mid;
	SDL_Surface *screen;
	s_thread_t *event_tid;
	S_KEYCODE_CODE keymap[SDLK_LAST];
	int rx[2];
	int ry[2];
} s_video_sdl_data_t;

int SDL_PrivateQuit(void);

void * s_video_sdl_event_parse (void *);

int s_video_sdl_kbd_init (s_server_conf_t *cfg);
void s_video_sdl_kbd_update (s_keybd_driver_t *keybd);
void s_video_sdl_kbd_uninit (void);

int s_video_sdl_mouse_init (s_server_conf_t *cfg);
void s_video_sdl_mouse_setcursor (S_MOUSE_CURSOR c);
void s_video_sdl_mouse_draw (void);
int s_video_sdl_mouse_update (s_mouse_driver_t *mouse);
void s_video_sdl_mouse_uninit (void);
void s_video_sdl_mouse_setxrange (int x1, int x2);
void s_video_sdl_mouse_setyrange (int y1, int y2);

void s_video_sdl_server_uninit (void);
int s_video_sdl_server_init (s_server_conf_t *cfg);
void s_video_sdl_server_surface_update (s_rect_t *coor);
void s_video_sdl_server_fullscreen (void);
