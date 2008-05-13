/***************************************************************************
    begin                : Mon Feb 21 2005
    copyright            : (C) 2005 - 2008 by Alper Akcan
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

#include "config.h"

#include <SDL.h>

typedef struct s_video_sdl_data_s {
	int mouse_fd[2];
	int keybd_fd[2];
	int screen_shm_mid;
	SDL_Surface *screen;
	s_thread_t *event_tid;
	s_keyboard_button_t keymap[SDLK_LAST];
} s_video_sdl_data_t;

int SDL_PrivateQuit(void);

void * s_video_sdl_event_parse (void *);

int s_video_sdl_kbd_init (s_server_conf_t *cfg);
int s_video_sdl_kbd_update (s_video_input_data_t *keybd);
void s_video_sdl_kbd_uninit (void);

int s_video_sdl_mouse_init (s_server_conf_t *cfg);
int s_video_sdl_mouse_update (s_video_input_data_t *mouse);
void s_video_sdl_mouse_uninit (void);

void s_video_sdl_server_uninit (void);
int s_video_sdl_server_init (s_server_conf_t *cfg);
void s_video_sdl_server_surface_update (s_rect_t *coor);
void s_video_sdl_server_fullscreen (void);
