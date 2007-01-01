/***************************************************************************
    begin                : Mon Nov 3 2003
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

int s_video_null_kbd_init (s_server_conf_t *cfg)
{
	return -1;
}

int s_video_null_kbd_update (s_video_input_data_t *keybd)
{
	return 0;
}

void s_video_null_kbd_uninit (void)
{
}

#endif /* VIDEO_NULL */
