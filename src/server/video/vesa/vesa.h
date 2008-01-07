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

typedef struct vbe_s {
	struct vbe_info_block *info;
	struct vbe_mode_info_block *mode;
	void *addr;
	void *state;
	int mode_n;
} vbe_t;

typedef struct s_video_vesa_data_s {
	int console_fd;
	vbe_t vbe;
} s_video_vesa_data_t;

int s_video_vesa_kbd_init (s_server_conf_t *cfg);

void s_video_vesa_server_goto_back (void);
void s_video_vesa_server_comefrom_back (void);
void s_video_vesa_server_restore (void);
void s_video_vesa_server_uninit (void);
void s_video_vesa_server_mtrr_init (void);
int s_video_vesa_server_init (s_server_conf_t *cfg);
void s_video_vesa_surface_uninit (void);
void s_video_vesa_server_surface_setrgbpalette (void);
void s_video_vesa_server_surface_init (int mode);
