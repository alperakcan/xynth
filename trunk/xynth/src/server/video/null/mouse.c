/***************************************************************************
    begin                : Wed Oct 8 2003
    copyright            : (C) 2003 - 2006 by Alper Akcan
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

#include "../../../lib/xynth_.h"
#include "../../server.h"
#include "null.h"

int s_video_null_mouse_init (s_server_conf_t *cfg)
{
	return -1;
}

void s_video_null_mouse_setcursor (S_MOUSE_CURSOR c)
{
}

void s_video_null_mouse_draw (void)
{
}

int s_video_null_mouse_update (s_mouse_driver_t *mouse)
{
	return 0;
}

void s_video_null_mouse_uninit (void)
{
}

void s_video_null_mouse_setxrange (int x1, int x2)
{
}

void s_video_null_mouse_setyrange (int y1, int y2)
{
}
