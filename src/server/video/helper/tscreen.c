/***************************************************************************
    begin                : Sun Oct 1 2006
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

/* Touch screen support with tslib patch came from Ali Utku Selen
 */

#if defined(VIDEO_HELPER_TSCREEN)

#include "../../../lib/xynth_.h"
#include "server.h"
#include <tslib.h>
 
static struct tsdev *ts = NULL;

int s_video_helper_touchscreen_init (s_server_conf_t *cfg)
{

	debugf(DSER, "Touchscreen initializing");	

	ts = ts_open("/dev/touchscreen/ucb1x00", 0);

	if (!ts) {
		debugf(DSER, "Touchscreen cannot be found");
		return -1;
	}

	if (ts_config(ts)) {
		debugf(DSER, "Touchscreen configuration error");
	}

	debugf(DSER, "Touchscreen initialized");

	return ts_fd(ts);
}

int s_video_helper_touchscreen_update (s_video_input_data_t *mouse)
{
	struct ts_sample samp;
	int ret;

	debugf(DSER, "Touchscreen updating");

	if (ts == NULL) {
		return -1;
	}

	ret = ts_read(ts, &samp, 1);

	if (ret < 0) {
		debugf(DSER, "Touchscreen read error");
	}

	if (ret != 1) {
		return 0;
	}

	mouse->mouse.x = samp.x;
	mouse->mouse.y = samp.y;

	debugf(DSER, "Touchscreen update (%d,%d)", mouse->mouse.x, mouse->mouse.y);
	
        return 0;
}

void s_video_helper_touchscreen_uninit (void)
{
	ts_close(ts);
	debugf(DSER, "Touchscreen uninitialized");
}

#endif /* VIDEO_HELPER_TSCREEN */
