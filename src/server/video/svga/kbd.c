/***************************************************************************
    begin                : Mon Nov 3 2003
    copyright            : (C) 2003 - 2008 by Alper Akcan
    email                : alper.akcan@gmail.com
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
#include "server.h"
#include "svga.h"
#include "../helper/helper.h"

int s_video_svga_kbd_init (s_server_conf_t *cfg)
{
	s_video_svga_data_t *priv = (s_video_svga_data_t *) xynth_server->driver->driver_data;
	return s_video_helper_kbd_init(cfg, priv->console_fd);
}
