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

typedef struct s_video_svga_data_s {
	int console_fd;
} s_video_svga_data_t;

int s_video_svga_kbd_init (s_server_conf_t *cfg);

void s_video_svga_server_goto_back (void);
void s_video_svga_server_comefrom_back (void);
void s_video_svga_server_uninit (void);
void s_video_svga_server_mtrr_init (void);
int s_video_svga_server_init (s_server_conf_t *cfg);
void s_video_svga_surface_uninit (void);
void s_video_svga_server_surface_setrgbpalette (void);
void s_video_svga_server_surface_init (int mode);
