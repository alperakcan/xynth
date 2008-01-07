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
#include <linux/fb.h>

struct {
	int fd;
	int bitsperpixel;
	int bytesperpixel;
	int colors;
	struct fb_fix_screeninfo f_scr;
	struct fb_var_screeninfo v_scr;
	struct fb_var_screeninfo v_scro;
} fb;

void fb_setpalette (int index, int red, int green, int blue);
void fb_getvaro (void);
void fb_putvaro (void);
void fb_getfix (void);
void fb_getvar (void);
void fb_putvar (void);
void fb_open (void);
void fb_close (void);
void fb_init (s_server_conf_t *cfg);

int fb_kbd_init (s_server_conf_t *cfg);

void fb_server_restore (void);
void fb_server_goto_back (void);
void fb_server_comefrom_back (void);
void fb_server_uninit (void);
int fb_server_init (s_server_conf_t *cfg);
void fb_server_setmode (s_server_conf_t *cfg, s_video_helper_mode_info_t *gmode);
void fb_server_surface_setrgbpalette (void);
void fb_server_surface_init (s_video_helper_mode_info_t *gmode);
void fb_server_surface_uninit (void);

void fb_timing_set (s_server_conf_t *cfg, struct fb_var_screeninfo *v_scr);
