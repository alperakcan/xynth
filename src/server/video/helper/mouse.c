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

#if defined(VIDEO_HELPER_MOUSE)

#include "../../../lib/xynth_.h"
#include "server.h"
#include "mouse.h"
#include "mouse_1.c"
#include "helper.h"

typedef struct s_video_helper_mousetable_s {
	char *name;
	int number;
} s_video_helper_mousetable_t;

static s_video_helper_mousetable_t s_video_helper_mousetable[] = {
	{"MOUSE_MICROSOFT", MOUSE_MICROSOFT},
	{"MOUSE_MOUSESYSTEMS", MOUSE_MOUSESYSTEMS},
	{"MOUSE_MMSERIES", MOUSE_MMSERIES},
	{"MOUSE_LOGITECH", MOUSE_LOGITECH},
	{"MOUSE_BUSMOUSE", MOUSE_BUSMOUSE},
	{"MOUSE_PS2", MOUSE_PS2},
	{"MOUSE_LOGIMAN", MOUSE_LOGIMAN},
	{"MOUSE_GPM", MOUSE_GPM},
	{"MOUSE_SPACEBALL", MOUSE_SPACEBALL},
	{"MOUSE_NONE", MOUSE_NONE},
	{"MOUSE_INTELLIMOUSE", MOUSE_INTELLIMOUSE},
	{"MOUSE_IMPS2", MOUSE_IMPS2},
	{"MOUSE_PNP", MOUSE_PNP},
	{"MOUSE_WACOM_GRAPHIRE", MOUSE_WACOM_GRAPHIRE},
	{"MOUSE_DRMOUSE4DS", MOUSE_DRMOUSE4DS},
	{"MOUSE_EXPPS2", MOUSE_EXPPS2},
};

#define MAX_MOUSE (sizeof(s_video_helper_mousetable) / sizeof(s_video_helper_mousetable_t))

int s_video_helper_mouse_init (s_server_conf_t *cfg)
{
        int i;
	int fd = -1;

	for (i = 0; i < MAX_MOUSE; i++) {
		if (strcasecmp(cfg->mouse.type, s_video_helper_mousetable[i].name) == 0) {
			fd = mouse_init_return_fd(cfg->mouse.device, s_video_helper_mousetable[i].number, cfg->mouse.samplerate);
		}
	}

	if (fd == -1) {
		debugf(DSER, "Mouse %s cannot be found", cfg->mouse.type);
		return fd;
	}

	mouse_setscale(cfg->mouse.scale);
	mouse_setposition_6d(server->window->surface->width / 2, server->window->surface->height / 2, 0,
			     0, 0, 0,
			     MOUSE_6DIM);
	mouse_getposition_6d(&server->window->event->mouse->x,  &server->window->event->mouse->y,  NULL, NULL, NULL, NULL, NULL);
	mouse_setrange_6d(0, server->window->surface->width - 1,
	                  0, server->window->surface->height - 1,
	                  0, 0,
	                  -20, 20,
	                  -20, 20,
	                  0, 0,
	                  MOUSE_6DIM);
	
	return fd;
}

int s_video_helper_mouse_update (s_video_input_data_t *mouse)
{
        int rx = 0;
        int ry = 0;

	if (mouse_update() == 0) {
		return 1;
	}

	mouse_getposition_6d(&(mouse->mouse.x),  &(mouse->mouse.y),  NULL, &rx, &ry, NULL, &(mouse->mouse.buttons));

	if (rx != 0) {
		if (rx < 0) {
			mouse->mouse.buttons |= MOUSE_WHEELXBUTTON_U;
		} else {
			mouse->mouse.buttons |= MOUSE_WHEELXBUTTON_D;
		}
	}

	if (ry != 0) {
		if (ry < 0) {
			mouse->mouse.buttons |= MOUSE_WHEELYBUTTON_U;
		} else {
			mouse->mouse.buttons |= MOUSE_WHEELYBUTTON_D;
		}
	}

	/* reset wheels */
	mouse_setposition_6d(0, 0, 0, 0, 0, 0, MOUSE_RXDIM | MOUSE_RYDIM | MOUSE_RZDIM);

	return 0;
}

void s_video_helper_mouse_uninit (void)
{
	mouse_close();
	s_server_cursor_uninit();
}

void s_video_helper_mouse_setxrange (int x1, int x2)
{
	mouse_setrange_6d(x1, x2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, MOUSE_XDIM);
}

void s_video_helper_mouse_setyrange (int y1, int y2)
{
	mouse_setrange_6d(0, 0, y1, y2, 0, 0, 0, 0, 0, 0, 0, 0, MOUSE_YDIM);
}

#endif /* VIDEO_HELPER_MOUSE */
