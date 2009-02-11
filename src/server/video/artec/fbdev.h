/***************************************************************************
    begin                : Thu Feb 17 2005
    copyright            : (C) 2005 - 2007 by Alper Akcan
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

#include <linux/fb.h>

struct {
	int fd;
	int bitsperpixel;
	int bytesperpixel;
	int colors;
	struct fb_fix_screeninfo f_scr;
	struct fb_var_screeninfo v_scr;
	struct fb_var_screeninfo v_scro;
} armfb;

void armfb_setpalette (int index, int red, int green, int blue);
void armfb_getvaro (void);
void armfb_putvaro (void);
void armfb_getfix (void);
void armfb_getvar (void);
void armfb_putvar (void);
void armfb_open (void);
void armfb_close (void);
void armfb_init (s_server_conf_t *cfg);

int armfb_keypad_init (s_server_conf_t *cfg);
int armfb_keypad_update (s_video_input_data_t *mouse);
void armfb_keypad_uninit (void);
int armfb_touch_init (s_server_conf_t *cfg);
int armfb_touch_update (s_video_input_data_t *mouse);
void armfb_touch_uninit (void);

void armfb_server_restore (void);
void armfb_server_goto_back (void);
void armfb_server_comefrom_back (void);
void armfb_server_uninit (void);
int armfb_server_init (s_server_conf_t *cfg);
void armfb_server_setmode (s_server_conf_t *cfg, s_video_helper_mode_info_t *gmode);
void armfb_server_surface_setrgbpalette (void);
void armfb_server_surface_init (s_video_helper_mode_info_t *gmode);
void armfb_server_surface_uninit (void);
