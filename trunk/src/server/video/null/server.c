/***************************************************************************
    begin                : Wed Oct 8 2003
    copyright            : (C) 2003 - 2007 by Alper Akcan
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

#if defined(VIDEO_NULL)

#include "../../../lib/xynth_.h"
#include "server.h"
#include "null.h"

s_video_input_t s_video_null_input_keybd = {
	VIDEO_INPUT_KEYBD,
	s_video_null_kbd_init,
	s_video_null_kbd_update,
	s_video_null_kbd_uninit,
};

s_video_input_t s_video_null_input_mouse = {
	VIDEO_INPUT_MOUSE,
	s_video_null_mouse_init,
	s_video_null_mouse_update,
	s_video_null_mouse_uninit,
};

s_video_input_t *s_video_null_input[] = {
	&s_video_null_input_keybd,
	&s_video_null_input_mouse,
	NULL,
};

s_video_driver_t s_video_null = {
	"null",
	"/dev/null",
	s_video_null_input,
	s_video_null_server_init,
	s_video_null_server_uninit,
	s_video_null_server_goto_back,
	s_video_null_server_comefrom_back,
	s_video_null_server_restore,
	s_video_null_server_surface_update,
	s_video_null_server_fullscreen,
	NULL /* driver data */
};

void s_video_null_server_goto_back (void)
{
}

void s_video_null_server_comefrom_back (void)
{
}

void s_video_null_server_restore (void)
{
}

void s_video_null_server_uninit (void)
{
}

int s_video_null_server_init (s_server_conf_t *cfg)
{
	return -1;
}

void s_video_null_server_surface_update (s_rect_t *coor)
{
}

void s_video_null_server_fullscreen (void)
{
}

#endif /* VIDEO_NULL */
