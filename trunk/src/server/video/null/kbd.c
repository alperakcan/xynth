/***************************************************************************
    begin                : Mon Nov 3 2003
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

#if defined(VIDEO_NULL)

#include "../../../lib/xynth_.h"
#include "../../server.h"
#include "null.h"

int s_video_null_kbd_init (s_server_conf_t *cfg)
{
	return -1;
}

void s_video_null_kbd_update (s_keybd_driver_t *keybd)
{
}

void s_video_null_kbd_uninit (void)
{
}

void s_video_null_kbd_switch (int vt)
{
}

#endif /* VIDEO_NULL */
