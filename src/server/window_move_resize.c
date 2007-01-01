/***************************************************************************
    begin                : Mon Aug 9 2004
    copyright            : (C) 2004 - 2007 by Alper Akcan
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

#include "../lib/xynth_.h"
#include "server.h"

void s_server_window_lines_draw_ (s_rect_t *coor, s_rect_t *rect, int c)
{
	if (!s_rect_clip_virtual(server->window->surface, coor->x, coor->y, coor->w, coor->h, rect)) {
		s_server_surface_matrix_add_id(S_MATRIX_MRBOX, rect);
		bpp_fillbox_o(server->window->surface, S_MATRIX_MRBOX, rect->x, rect->y, rect->w, rect->h, c);
		s_server_surface_update(rect);
	}
}

void s_server_window_lines_draw (s_rect_t *bnew)
{
	int color;
	s_rect_t rect;
	s_rect_t coor;

	color = s_rgbcolor(server->window->surface, 222, 222, 222);

	coor.x = bnew->x;
	coor.y = bnew->y;
	coor.w = bnew->w;
	coor.h = 1;
	s_server_window_lines_draw_(&coor, &rect, color);

	coor.x = bnew->x;
	coor.y = bnew->y;
	coor.w = 1;
	coor.h = bnew->h;
	s_server_window_lines_draw_(&coor, &rect, color);

	coor.x = bnew->x;
	coor.y = bnew->y + bnew->h - 1;
	coor.w = bnew->w;
	coor.h = 1;
	s_server_window_lines_draw_(&coor, &rect, color);

	coor.x = bnew->x + bnew->w - 1;
	coor.y = bnew->y;
	coor.w = 1;
	coor.h = bnew->h;
	s_server_window_lines_draw_(&coor, &rect, color);
}

void s_server_window_lines_clear_ (s_rect_t *told, s_rect_t *tnew)
{
	s_rect_t *rtmp;
	s_list_t *diff;
	s_list_init(&diff);
	s_rect_difference(told, tnew, diff);
	while (!s_list_eol(diff, 0)) {
		rtmp = (s_rect_t *) s_list_get(diff, 0);
		s_server_pri_set(SURFACE_REDRAW, rtmp);
		s_list_remove(diff, 0);
		s_free(rtmp);
	}
	s_list_uninit(diff);
}
	
void s_server_window_lines_clear (s_rect_t *bold, s_rect_t *bnew)
{
	s_rect_t told;
	s_rect_t tnew;
	
	told.x = bold->x;
	told.y = bold->y;
	told.w = bold->w;
	told.h = 1;
	tnew.x = bnew->x;
	tnew.y = bnew->y;
	tnew.w = bnew->w;
	tnew.h = 1;
	s_server_window_lines_clear_(&told, &tnew);

	told.x = bold->x;
	told.y = bold->y;
	told.w = 1;
	told.h = bold->h;
	tnew.x = bnew->x;
	tnew.y = bnew->y;
	tnew.w = 1;
	tnew.h = bnew->h;
	s_server_window_lines_clear_(&told, &tnew);

	told.x = bold->x;
	told.y = bold->y + bold->h - 1;
	told.w = bold->w;
	told.h = 1;
	tnew.x = bnew->x;
	tnew.y = bnew->y + bnew->h - 1;
	tnew.w = bnew->w;
	tnew.h = 1;
	s_server_window_lines_clear_(&told, &tnew);

	told.x = bold->x + bold->w - 1;
	told.y = bold->y;
	told.w = 1;
	told.h = bold->h;
	tnew.x = bnew->x + bnew->w - 1;
	tnew.y = bnew->y;
	tnew.w = 1;
	tnew.h = bnew->h;
	s_server_window_lines_clear_(&told, &tnew);
}

#define s_server_window_prepare(cursor)\
	int id;\
	int mx;\
	int my;\
	int mw;\
	int mh;\
	int myr;\
	int mxr;\
	s_rect_t move;\
	id = s_server_pri_id(0);\
	if (id < 0) {\
		return;\
	}\
	move.x = server->client[id].win.x;\
	move.y = server->client[id].win.y;\
	move.w = server->client[id].win.w;\
	move.h = server->client[id].win.h;\
	myr = 0;\
	mxr = 0;\
	mx = s_mouse_getx();\
	my = s_mouse_gety();\
	mw = server->theme.form_min.w_;\
	mh = server->theme.form_min.h;\
	server->mh = 1;\
	s_server_mouse_setcursor(cursor);\
	s_server_window_lines_draw(&move);

void s_server_window_while (s_rect_t *move, int flag)
{
	int mx = s_mouse_getx();
	int my = s_mouse_gety();
	s_rect_t tmp;
	
	while ((server->window->running * server->window->event->mouse->buttons * server->mh) > 0) {
		if (s_socket_listen_wait(server->window, -1)) {
			server->window->running = 0;
			break;
		}
		if (server->mh == 0) {
			continue;
		}
		if ((mx == s_mouse_getx()) && (my == s_mouse_gety())) {
			continue;
		}
		tmp = *move;

		if (flag & 0x01) { move->x += (s_mouse_getx() - mx); }
		if (flag & 0x02) { move->x -= (s_mouse_getx() - mx); }

		if (flag & 0x04) { move->y += (s_mouse_gety() - my); }
		if (flag & 0x08) { move->y -= (s_mouse_gety() - my); }

		if (flag & 0x10) { move->w += (s_mouse_getx() - mx); }
		if (flag & 0x20) { move->w -= (s_mouse_getx() - mx); }

		if (flag & 0x40) { move->h += (s_mouse_gety() - my); }
		if (flag & 0x80) { move->h -= (s_mouse_gety() - my); }
		
		mx = s_mouse_getx();
		my = s_mouse_gety();
		
		s_server_window_lines_clear(&tmp, move);
		s_server_window_lines_draw(move);
	}
}

void s_server_window_finish (int id, s_rect_t *move)
{
	s_rect_t tmp = {0, 0, 0, 0};
	s_server_window_move_resize(id, move);
	s_server_window_lines_clear(move, &tmp);
	s_mouse_setxrange(server->window, 0, server->window->surface->width - 1);
	s_mouse_setyrange(server->window, 0, server->window->surface->height - 1);
	server->mh = 0;
}

void s_server_window_move (s_window_t *window)
{
	s_server_window_prepare(MOUSE_CURSOR_SIZEA);
	s_server_window_while(&move, 0x01 | 0x04);
	s_server_window_finish(id, &move);
}

void s_server_window_resize_up (s_window_t *window)
{
	s_server_window_prepare(MOUSE_CURSOR_SIZEV);

	myr = 0;
	if (myr < my - move.y) {
		myr = my - move.y;
	}
	s_mouse_setyrange(window, myr, move.y + move.h - mh + (my - move.y));
	s_mouse_setxrange(window, mx, mx);

	s_server_window_while(&move, 0x04 | 0x80);
	s_server_window_finish(id, &move);
}

void s_server_window_resize_up_left (s_window_t *window)
{
	s_server_window_prepare(MOUSE_CURSOR_SIZEB);

	myr = 0;
	mxr = 0;
	if (myr < my - move.y) {
		myr = my - move.y;
	}
	s_mouse_setyrange(window, myr, move.y + move.h - mh + (my - move.y));
	if (mxr < mx - move.x) {
		mxr = mx - move.x;
	}
	s_mouse_setxrange(window, mxr, move.x + move.w - mw + (mx - move.x));

	s_server_window_while(&move, 0x01 | 0x04 | 0x20 | 0x80);
	s_server_window_finish(id, &move);
}

void s_server_window_resize_left (s_window_t *window)
{
	s_server_window_prepare(MOUSE_CURSOR_SIZEH);

	mxr = 0;
	if (mxr < mx - move.x) {
		mxr = mx - move.x;
	}
	s_mouse_setxrange(window, mxr, move.x + move.w - mw + (mx - move.x));
	s_mouse_setyrange(window, my, my);

	s_server_window_while(&move, 0x01 | 0x20);
	s_server_window_finish(id, &move);
}

void s_server_window_resize_down_left (s_window_t *window)
{
	s_server_window_prepare(MOUSE_CURSOR_SIZES);

	mxr = 0;
	myr = window->surface->height;
	if (mxr < mx - move.x) {
		mxr = mx - move.x;
	}
	s_mouse_setxrange(window, mxr, move.x + move.w - mw + (mx - move.x));
	if ((myr + move.y + move.h - my) > window->surface->height) {
		myr = window->surface->height - (move.y + move.h - my);
	}
	s_mouse_setyrange(window, move.y + mh - (move.y + move.h - my), myr);

	s_server_window_while(&move, 0x01 | 0x20 | 0x40);
	s_server_window_finish(id, &move);
}

void s_server_window_resize_down (s_window_t *window)
{
	s_server_window_prepare(MOUSE_CURSOR_SIZEV);

	myr = window->surface->height;
	if ((myr + move.y + move.h - my) > window->surface->height) {
		myr = window->surface->height - (move.y + move.h - my);
	}
	s_mouse_setxrange(window, mx, mx);
	s_mouse_setyrange(window, move.y + mh - (move.y + move.h - my), myr);

	s_server_window_while(&move, 0x40);
	s_server_window_finish(id, &move);
}

void s_server_window_resize_down_right (s_window_t *window)
{
	s_server_window_prepare(MOUSE_CURSOR_SIZEB);

	myr = window->surface->height;
	mxr = window->surface->width;
	if ((mxr + move.x + move.w - mx) > window->surface->width) {
		mxr = window->surface->width - (move.x + move.w - mx);
	}
	s_mouse_setxrange(window, move.x + mw - (move.x + move.w - mx), mxr);
	if ((myr + move.y + move.h - my) > window->surface->height) {
		myr = window->surface->height - (move.y + move.h - my);
	}
	s_mouse_setyrange(window, move.y + mh - (move.y + move.h - my), myr);

	s_server_window_while(&move, 0x10 | 0x40);
	s_server_window_finish(id, &move);
}

void s_server_window_resize_right (s_window_t *window)
{
	s_server_window_prepare(MOUSE_CURSOR_SIZEH);

	mxr = window->surface->width;
	if ((mxr + move.x + move.w - mx) > window->surface->width) {
		mxr = window->surface->width - (move.x + move.w - mx);
	}
	s_mouse_setxrange(window, move.x + mw - (move.x + move.w - mx), mxr);
	s_mouse_setyrange(window, my, my);

	s_server_window_while(&move, 0x10);
	s_server_window_finish(id, &move);
}

void s_server_window_resize_up_right (s_window_t *window)
{
	s_server_window_prepare(MOUSE_CURSOR_SIZES);

	myr = 0;
	mxr = window->surface->width;
	if (myr < my - move.y) {
		myr = my - move.y;
	}
	s_mouse_setyrange(window, myr, move.y + move.h - mh + (my - move.y));
	if ((mxr + move.x + move.w - mx) > window->surface->width) {
		mxr = window->surface->width - (move.x + move.w - mx);
	}
	s_mouse_setxrange(window, move.x + mw - (move.x + move.w - mx), mxr);

	s_server_window_while(&move, 0x04 | 0x10 | 0x80);
	s_server_window_finish(id, &move);
}
