/***************************************************************************
    begin                : Thu Sep 22 2005
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

#if defined(VIDEO_PSPDEV)

#include "../../../lib/xynth_.h"
#include "server.h"
#include "pspdev.h"

int s_video_pspdev_kbd_init (s_server_conf_t *cfg)
{
	return -1;
}

int s_video_pspdev_kbd_update (s_video_input_data_t *keybd)
{
	return 0;
}

void s_video_pspdev_kbd_uninit (void)
{
}

#endif /* VIDEO_PSPDEV */
