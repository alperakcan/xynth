/***************************************************************************
    begin                : Fri Jan 31 2003
    copyright            : (C) 2003 - 2009 by Alper Akcan
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

#include "../lib/xynth_.h"
#include "server.h"
#include "cursor.h"

void s_server_cursor_uninit (void)
{
        int i;
	if (xynth_server->cursor.sw == 0) {
		return;
	}
	for (i = 0; i < CURSOR_TYPE_MAX; i++) {
		s_free(xynth_server->cursor.images[i].mat);
		s_free(xynth_server->cursor.images[i].buf);
	}
	xynth_server->cursor.sw = 0;
}

void s_server_cursor_init (void)
{
	int i;

	if (xynth_server->cursor.sw == 1) {
		return;
	}

	xynth_server->cursor.x = 0;
	xynth_server->cursor.y = 0;
	xynth_server->cursor.xyid = -1;
	xynth_server->cursor.xyid_old = -1;

	for (i = 0; i < CURSOR_TYPE_MAX; i++) {
		xynth_server->cursor.images[i].mat = NULL;
		xynth_server->cursor.images[i].buf = NULL;
	}

	xynth_server->cursor.sw = 1;

	s_server_cursor_image_set(CURSOR_TYPE_WAIT,    0xffffff, 0, s_video_helper_cursor_wait);
	s_server_cursor_image_set(CURSOR_TYPE_ARROW,   0xFFFFFF, 0, s_video_helper_cursor_arrow);
	s_server_cursor_image_set(CURSOR_TYPE_CROSS,   0xffffff, 0, s_video_helper_cursor_cross);
	s_server_cursor_image_set(CURSOR_TYPE_IBEAM,   0xffffff, 0, s_video_helper_cursor_ibeam);
	s_server_cursor_image_set(CURSOR_TYPE_SIZEV,   0xffffff, 0, s_video_helper_cursor_sizev);
	s_server_cursor_image_set(CURSOR_TYPE_SIZEH,   0xffffff, 0, s_video_helper_cursor_sizeh);
	s_server_cursor_image_set(CURSOR_TYPE_SIZES,   0xffffff, 0, s_video_helper_cursor_sizes);
	s_server_cursor_image_set(CURSOR_TYPE_SIZEB,   0xffffff, 0, s_video_helper_cursor_sizeb);
	s_server_cursor_image_set(CURSOR_TYPE_SIZEA,   0xffffff, 0, s_video_helper_cursor_sizea);
	s_server_cursor_image_set(CURSOR_TYPE_POINT,   0xffffff, 0, s_video_helper_cursor_point);
	s_server_cursor_image_set(CURSOR_TYPE_SPLITV,  0xffffff, 0, s_video_helper_cursor_splitv);
	s_server_cursor_image_set(CURSOR_TYPE_SPLITH,  0xffffff, 0, s_video_helper_cursor_splith);
	s_server_cursor_image_set(CURSOR_TYPE_FORBID,  0xffffff, 0, s_video_helper_cursor_forbid);
	s_server_cursor_image_set(CURSOR_TYPE_UPARROW, 0xffffff, 0, s_video_helper_cursor_uparrow);
	s_server_cursor_image_set(CURSOR_TYPE_NONE,    0xffffff, 0, s_video_helper_cursor_none);

	s_server_mouse_setcursor(CURSOR_TYPE_ARROW);
	s_server_cursor_position(s_mouse_getx(), s_mouse_gety());
}

void s_server_cursor_image_set (int which, int c0, int c1, unsigned int *c)
{
        int x;
        int y;
	s_surface_t *s;
        unsigned char *mat;
	unsigned int *cur = c;

        if (xynth_server->cursor.enabled == 0) {
        	return;
        }
	if (xynth_server->cursor.sw == 0) {
		return;
	}

	xynth_server->cursor.images[which].w = 32;
	xynth_server->cursor.images[which].h = 32;
	if (xynth_server->cursor.images[which].mat != NULL) {
		s_free(xynth_server->cursor.images[which].mat);
		xynth_server->cursor.images[which].mat = NULL;
	}
	if (xynth_server->cursor.images[which].buf != NULL) {
		s_free(xynth_server->cursor.images[which].buf);
		xynth_server->cursor.images[which].buf = NULL;
	}
	xynth_server->cursor.images[which].mat = (unsigned char *) s_malloc(sizeof(char) * 32 * 32);
	xynth_server->cursor.images[which].buf = (char *) s_malloc(xynth_server->window->surface->bytesperpixel * 32 * 32);

	mat = xynth_server->cursor.images[which].mat;
	for (y = 0; y < 32; y++) {
		for (x = 0; x < 32; x++) {
			if ((*(cur + 32) << x) & (1 << 31)) {
				*mat = 1;
			} else {
				*mat = 0;
			}
			mat++;
		}
		cur++;
	}

	if (s_surface_create_from_data(&s, xynth_server->cursor.images[which].w, xynth_server->cursor.images[which].h, xynth_server->window->surface->bitsperpixel, xynth_server->cursor.images[which].buf)) {
		return;
	}
        cur = c;
	for (y = 0; y < 32; y++) {
		for (x = 0; x < 32; x++) {
			if ((*cur << x) & (1 << 31)) {
				s_setpixel(s, x, y, c1);
			} else if ((*(cur + 32) << x) & (1 << 31)) {
				s_setpixel(s, x, y, c0);
			}
		}
		cur++;
	}
	s_surface_destroy(s);
}

void s_server_cursor_matrix_add (void)
{
        s_rect_t mcoor;

	if (xynth_server->cursor.sw == 0) {
		return;
	}

        if (xynth_server->cursor.enabled) {
        	mcoor.x = xynth_server->cursor.x;
        	mcoor.y = xynth_server->cursor.y;
        	mcoor.w = xynth_server->cursor.img->w;
        	mcoor.h = xynth_server->cursor.img->h;

        	xynth_server->cursor.xyid_old = xynth_server->cursor.xyid;
        	xynth_server->cursor.xyid = *(xynth_server->window->surface->matrix + (xynth_server->window->event->mouse->y * xynth_server->window->surface->width) + xynth_server->window->event->mouse->x);
        	s_server_surface_matrix_add_this(S_MATRIX_SWMOUSE, &mcoor, &mcoor, xynth_server->cursor.img->mat);
        } else {
        	xynth_server->cursor.xyid_old = S_MATRIX_FREE;
        	xynth_server->cursor.xyid = S_MATRIX_FREE;
        }
}

void s_server_cursor_draw (void)
{
        s_rect_t mcoor;

        if (xynth_server->cursor.enabled == 0) {
        	return;
        }
	if (xynth_server->cursor.sw == 0) {
		return;
	}

	mcoor.x = xynth_server->cursor.x;
	mcoor.y = xynth_server->cursor.y;
	mcoor.w = xynth_server->cursor.img->w;
	mcoor.h = xynth_server->cursor.img->h;

	s_server_putbox(xynth_server->window, S_MATRIX_SWMOUSE, &mcoor, xynth_server->cursor.x, xynth_server->cursor.y, xynth_server->cursor.img);
}

void s_server_cursor_select (s_cursor_type_t c)
{
        if (xynth_server->cursor.enabled == 0) {
        	return;
        }
	if (xynth_server->cursor.sw == 0) {
		return;
	}

	switch (c) {
		case CURSOR_TYPE_WAIT:   xynth_server->cursor.img = &xynth_server->cursor.images[CURSOR_TYPE_WAIT];   break;
		case CURSOR_TYPE_ARROW:  xynth_server->cursor.img = &xynth_server->cursor.images[CURSOR_TYPE_ARROW];  break;
		case CURSOR_TYPE_POINT:  xynth_server->cursor.img = &xynth_server->cursor.images[CURSOR_TYPE_POINT];  break;
		case CURSOR_TYPE_CROSS:  xynth_server->cursor.img = &xynth_server->cursor.images[CURSOR_TYPE_CROSS];  break;
		case CURSOR_TYPE_IBEAM:  xynth_server->cursor.img = &xynth_server->cursor.images[CURSOR_TYPE_IBEAM];  break;
		case CURSOR_TYPE_SIZES:  xynth_server->cursor.img = &xynth_server->cursor.images[CURSOR_TYPE_SIZES];  break;
		case CURSOR_TYPE_SIZEB:  xynth_server->cursor.img = &xynth_server->cursor.images[CURSOR_TYPE_SIZEB];  break;
		case CURSOR_TYPE_SIZEV:  xynth_server->cursor.img = &xynth_server->cursor.images[CURSOR_TYPE_SIZEV];  break;
		case CURSOR_TYPE_SIZEH:  xynth_server->cursor.img = &xynth_server->cursor.images[CURSOR_TYPE_SIZEH];  break;
		case CURSOR_TYPE_SIZEA:  xynth_server->cursor.img = &xynth_server->cursor.images[CURSOR_TYPE_SIZEA];  break;
		case CURSOR_TYPE_FORBID: xynth_server->cursor.img = &xynth_server->cursor.images[CURSOR_TYPE_FORBID]; break;
		case CURSOR_TYPE_SPLITH: xynth_server->cursor.img = &xynth_server->cursor.images[CURSOR_TYPE_SPLITH]; break;
		case CURSOR_TYPE_SPLITV: xynth_server->cursor.img = &xynth_server->cursor.images[CURSOR_TYPE_SPLITV]; break;
		case CURSOR_TYPE_UPARROW:xynth_server->cursor.img = &xynth_server->cursor.images[CURSOR_TYPE_UPARROW];break;
		case CURSOR_TYPE_NONE:   xynth_server->cursor.img = &xynth_server->cursor.images[CURSOR_TYPE_NONE];   break;
		default: break;
	}
}

void s_server_cursor_position (int x, int y)
{
	s_rect_t cold;
	s_rect_t cnew;
	s_rect_t cmer;
	s_rect_t clip;

        if (xynth_server->cursor.enabled == 0) {
        	return;
        }
	if (xynth_server->cursor.sw == 0) {
		return;
	}

	cold.x = xynth_server->cursor.x;
	cold.y = xynth_server->cursor.y;
	cold.w = xynth_server->cursor.img->w;
	cold.h = xynth_server->cursor.img->h;

	cnew.x = x;
	cnew.y = y;
	cnew.w = xynth_server->cursor.img->w;
	cnew.h = xynth_server->cursor.img->h;

	if (s_region_rect_union(&cold, &cnew, &cmer)) {
		return;
	}
	if (s_region_rect_intersect(xynth_server->window->surface->buf, &cmer, &clip)) {
		return;
	}

	xynth_server->cursor.x = x;
	xynth_server->cursor.y = y;
	s_server_pri_set(SURFACE_REDRAW, &clip);
}

int s_mouse_getx (void)
{
	return xynth_server->window->event->mouse->x;
}

int s_mouse_gety (void)
{
	return xynth_server->window->event->mouse->y;
}

void s_mouse_setxrange (s_window_t *window, int a, int b)
{
	int x0;
	int x1;

	x0 = MIN(a, b);
	x1 = MAX(a, b);
	x0 = MAX(x0, 0);
	x1 = MIN(x1, window->surface->width - 1);

	xynth_server->mouse_rangex[0] = x0;
	xynth_server->mouse_rangex[1] = x1;
}

void s_mouse_setyrange (s_window_t *window, int a, int b)
{
        int y0;
        int y1;

	y0 = MIN(a, b);
	y1 = MAX(a, b);
	y0 = MAX(y0, 0);
	y1 = MIN(y1, window->surface->height - 1);

	xynth_server->mouse_rangey[0] = y0;
	xynth_server->mouse_rangey[1] = y1;
}

void s_server_mouse_setcursor (s_cursor_type_t c)
{
	xynth_server->window->event->mouse->cursor = c;
	s_server_cursor_select(c);
	s_server_mouse_draw();
}

void s_server_mouse_draw (void)
{
	int x = s_mouse_getx();
	int y = s_mouse_gety();

	switch (xynth_server->window->event->mouse->cursor) {
		case CURSOR_TYPE_WAIT: 		x -= 7;		y -= 12;	break;
		case CURSOR_TYPE_NONE:						break;
		case CURSOR_TYPE_ARROW:						break;
		case CURSOR_TYPE_CROSS:		x -= 9;		y -= 9;		break;
		case CURSOR_TYPE_IBEAM:		x -= 5;		y -= 9;		break;
		case CURSOR_TYPE_SIZEV:		x -= 5;		y -= 10;	break;
		case CURSOR_TYPE_SIZEH:		x -= 10; 	y -= 5;		break;
		case CURSOR_TYPE_SIZES:		x -= 7;		y -= 7;		break;
		case CURSOR_TYPE_SIZEB:		x -= 7;		y -= 7;		break;
		case CURSOR_TYPE_SIZEA:		x -= 10; 	y -= 10;	break;
		case CURSOR_TYPE_POINT:		x -= 6;				break;
		case CURSOR_TYPE_SPLITV:	x -= 10; 	y -= 8;		break;
		case CURSOR_TYPE_SPLITH:	x -= 8;		y -= 10;	break;
		case CURSOR_TYPE_FORBID:	x -= 9;		y -= 9;		break;
		case CURSOR_TYPE_UPARROW:	x -= 5;				break;
		default:			break;
	}
	s_server_cursor_position(x, y);
}

int s_server_mouse_uninit (s_window_t *window, s_pollfd_t *pfd)
{
	s_video_input_t *mouse;
	mouse = (s_video_input_t *) pfd->data;
        if (mouse->uninit != NULL) {
		mouse->uninit();
	}
	return 0;
}

int s_server_mouse_update (s_window_t *window, s_pollfd_t *pfd)
{
	s_video_input_t *mouse;
	s_video_input_data_t mdata;

	mouse = (s_video_input_t *) pfd->data;
	xynth_server->window->event->type = 0;
	memset(&mdata, 0, sizeof(s_video_input_data_t));

	if (mouse->update != NULL) {
		if (mouse->update(&mdata)) {
			return 0;
		}
		mdata.mouse.x = MAX(mdata.mouse.x, xynth_server->mouse_rangex[0]);
		mdata.mouse.x = MIN(mdata.mouse.x, xynth_server->mouse_rangex[1]);
		mdata.mouse.y = MAX(mdata.mouse.y, xynth_server->mouse_rangey[0]);
		mdata.mouse.y = MIN(mdata.mouse.y, xynth_server->mouse_rangey[1]);

		if (s_server_event_parse_mouse(&(mdata.mouse))) {
			return 0;
		}
		s_server_event_changed();
	}

	return 0;
}

void s_server_mouse_init (s_server_conf_t *cfg, s_video_input_t *mouse)
{
	int fd = -1;
	s_pollfd_t *pfd;

        if (mouse->init != NULL) {
		if (strcasecmp(cfg->mouse.type, "MOUSE_NONE") != 0) {
			fd = mouse->init(cfg);
			if (fd < 0) {
				debugf(DSER, "mouse->init(cfg) failed");
			} else {
				xynth_server->cursor.enabled = cfg->mouse.cursor;
				s_server_cursor_init();
				xynth_server->window->event->mouse->x = xynth_server->window->surface->width / 2;
				xynth_server->window->event->mouse->y = xynth_server->window->surface->height / 2;
				s_mouse_setxrange(xynth_server->window, 0, xynth_server->window->surface->width);
				s_mouse_setyrange(xynth_server->window, 0, xynth_server->window->surface->height);
			}
		}
	}
	if (fd < 0) {
		debugf(DSER, "mouse support disabled");
	} else {
		s_pollfd_init(&pfd);
		pfd->fd = fd;
		pfd->pf_in = s_server_mouse_update;
		pfd->pf_close = s_server_mouse_uninit;
		pfd->data = mouse;
		s_pollfd_add(xynth_server->window, pfd);
	}
}
