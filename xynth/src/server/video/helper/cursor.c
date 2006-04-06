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

#if defined(VIDEO_HELPER_CURSOR)

#include "../../../lib/xynth_.h"
#include "../../server.h"
#include "cursor.h"
#include "helper.h"

void s_video_helper_mouse_cursor_init (void)
{
	s_server_cursor_init();
	s_server_cursor_image_set(MOUSE_CURSOR_WAIT,    0xffffff, 0, s_video_helper_cursor_wait);
	s_server_cursor_image_set(MOUSE_CURSOR_ARROW,   0xFFFFFF, 0, s_video_helper_cursor_arrow);
	s_server_cursor_image_set(MOUSE_CURSOR_CROSS,   0xffffff, 0, s_video_helper_cursor_cross);
	s_server_cursor_image_set(MOUSE_CURSOR_IBEAM,   0xffffff, 0, s_video_helper_cursor_ibeam);
	s_server_cursor_image_set(MOUSE_CURSOR_SIZEV,   0xffffff, 0, s_video_helper_cursor_sizev);
	s_server_cursor_image_set(MOUSE_CURSOR_SIZEH,   0xffffff, 0, s_video_helper_cursor_sizeh);
	s_server_cursor_image_set(MOUSE_CURSOR_SIZES,   0xffffff, 0, s_video_helper_cursor_sizes);
	s_server_cursor_image_set(MOUSE_CURSOR_SIZEB,   0xffffff, 0, s_video_helper_cursor_sizeb);
	s_server_cursor_image_set(MOUSE_CURSOR_SIZEA,   0xffffff, 0, s_video_helper_cursor_sizea);
	s_server_cursor_image_set(MOUSE_CURSOR_POINT,   0xffffff, 0, s_video_helper_cursor_point);
	s_server_cursor_image_set(MOUSE_CURSOR_SPLITV,  0xffffff, 0, s_video_helper_cursor_splitv);
	s_server_cursor_image_set(MOUSE_CURSOR_SPLITH,  0xffffff, 0, s_video_helper_cursor_splith);
	s_server_cursor_image_set(MOUSE_CURSOR_FORBID,  0xffffff, 0, s_video_helper_cursor_forbid);
	s_server_cursor_image_set(MOUSE_CURSOR_UPARROW, 0xffffff, 0, s_video_helper_cursor_uparrow);
	s_server_cursor_image_set(MOUSE_CURSOR_NONE,    0xffffff, 0, s_video_helper_cursor_none);

	s_server_mouse_setcursor(MOUSE_CURSOR_ARROW);
	s_server_cursor_position(s_mouse_getx(), s_mouse_gety());
}

void s_video_helper_mouse_setcursor (S_MOUSE_CURSOR c)
{
	server->window->event->mouse->cursor = c;
	s_server_cursor_select(c);
	s_server_mouse_draw();
}

void s_video_helper_mouse_draw (void)
{
	int x = s_mouse_getx();
	int y = s_mouse_gety();

	switch (server->window->event->mouse->cursor) {
		case MOUSE_CURSOR_WAIT: 	x -= 7;		y -= 12;	break;
		case MOUSE_CURSOR_NONE:
		case MOUSE_CURSOR_ARROW:					break;
		case MOUSE_CURSOR_CROSS:	x -= 9;		y -= 9;		break;
		case MOUSE_CURSOR_IBEAM:	x -= 5;		y -= 9;		break;
		case MOUSE_CURSOR_SIZEV:	x -= 5;		y -= 10;	break;
		case MOUSE_CURSOR_SIZEH:	x -= 10; 	y -= 5;		break;
		case MOUSE_CURSOR_SIZES:	x -= 7;		y -= 7;		break;
		case MOUSE_CURSOR_SIZEB:	x -= 7;		y -= 7;		break;
		case MOUSE_CURSOR_SIZEA:	x -= 10; 	y -= 10;	break;
		case MOUSE_CURSOR_POINT:	x -= 6;				break;
		case MOUSE_CURSOR_SPLITV:	x -= 10; 	y -= 8;		break;
		case MOUSE_CURSOR_SPLITH:	x -= 8;		y -= 10;	break;
		case MOUSE_CURSOR_FORBID:	x -= 9;		y -= 9;		break;
		case MOUSE_CURSOR_UPARROW:	x -= 5;				break;
		default:			break;
	}
	s_server_cursor_position(x, y);
}

#endif /* VIDEO_HELPER_CURSOR */
