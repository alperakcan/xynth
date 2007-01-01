/***************************************************************************
    begin                : Tue Dec 26 2006
    copyright            : (C) 2006 - 2007 by Alper Akcan
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

#if defined(VIDEO_CACA)

#include "../../../lib/xynth_.h"
#include "server.h"
#include "caca.h"

int s_video_caca_mouse_init (s_server_conf_t *cfg)
{
	return -1;
}

int s_video_caca_mouse_update (s_video_input_data_t *mouse)
{
	return 0;
}

void s_video_caca_mouse_uninit (void)
{
}

#endif /* VIDEO_CACA */
