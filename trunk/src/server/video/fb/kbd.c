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

#if defined(VIDEO_FBDev)

#include <signal.h>
#include <termios.h>
#include <linux/vt.h>
#include <linux/kd.h>
#include <linux/keyboard.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "../../../lib/xynth_.h"
#include "server.h"
#include "../helper/helper.h"
#include "fbdev.h"

extern int fb_console_fd;

int fb_kbd_init (s_server_conf_t *cfg)
{
	if (strcmp(cfg->keyboard.keyboard, "console") == 0) {
		return s_video_helper_kbd_init(cfg, fb_console_fd);
	} else {
		return -1;
	}
}

#endif /* VIDEO_FBDev */
