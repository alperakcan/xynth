/***************************************************************************
    begin                : Thu Feb 17 2005
    copyright            : (C) 2005 - 2008 by Alper Akcan
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

#include "config.h"

int s_video_null_kbd_init (s_server_conf_t *cfg);
int s_video_null_kbd_update (s_video_input_data_t *keybd);
void s_video_null_kbd_uninit (void);

int s_video_null_mouse_init (s_server_conf_t *cfg);
int s_video_null_mouse_update (s_video_input_data_t *mouse);
void s_video_null_mouse_uninit (void);

void s_video_null_server_goto_back (void);
void s_video_null_server_comefrom_back (void);
void s_video_null_server_restore (void);
void s_video_null_server_uninit (void);
int s_video_null_server_init (s_server_conf_t *cfg);
void s_video_null_server_surface_update (s_rect_t *coor);
void s_video_null_server_fullscreen (void);
