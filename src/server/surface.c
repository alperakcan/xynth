/***************************************************************************
    begin                : Wed Feb 19 2003
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

#include "../lib/xynth_.h"
#include "server.h"

void s_server_surface_matrix_add_this (int id, s_rect_t *coor, s_rect_t *mcoor, unsigned char *mat)
{
	int h;
	int w;
	int w_;
	int m_;
	s_rect_t clip;
	s_rect_t intr_;
	s_rect_t inter;
	unsigned char *tmp;

	if (s_region_rect_intersect(coor, mcoor, &intr_)) {
		return;
	}

	clip.x = 0;
	clip.y = 0;
	clip.w = xynth_server->window->surface->width;
	clip.h = xynth_server->window->surface->height;

	if (s_region_rect_intersect(&intr_, &clip, &inter)) {
		return;
	}

        h = inter.h;
        w = inter.w;
        w_ = clip.w - w;
        m_ = mcoor->w - w;
        tmp = (xynth_server->window->surface->matrix + (inter.y * clip.w) + inter.x);
        mat += ((inter.y - mcoor->y) * mcoor->w) + (inter.x - mcoor->x);

	while (h--) {
		w = inter.w;
		while (w--) {
			if (*mat) {
				*tmp = id;
			}
			mat++;
			tmp++;
		}
		tmp += w_;
		mat += m_;
	}
}

void s_server_surface_matrix_add_id (int id, s_rect_t *coor)
{
	int h;
	int w;
	int w_;
	s_rect_t clip;
	s_rect_t inter;
	unsigned char *tmp;

	clip.x = 0;
	clip.y = 0;
	clip.w = xynth_server->window->surface->width;
	clip.h = xynth_server->window->surface->height;
	tmp = xynth_server->window->surface->matrix;

	if (s_region_rect_intersect(coor, &clip, &inter)) {
		return;
	}
	
        h = inter.h;
        w = inter.w;
        w_ = clip.w - w;
        tmp += ((inter.y * clip.w) + inter.x);

	while (h--) {
		w = inter.w;
		while (w--) {
			*tmp++ = id;
		}
		tmp += w_;
	}
}

void s_server_surface_matrix_add (int id, s_rect_t *coor)
{
	s_rect_t inter;
	if (s_region_rect_intersect(coor, &xynth_server->client[id].buf, &inter)) {
		return;
	}
	s_server_surface_matrix_add_id(id, &inter);
}

void s_server_surface_matrix_del_coor (s_rect_t *coor)
{
	s_server_surface_matrix_add_id(S_MATRIX_FREE, coor);
}

void s_server_surface_clean (s_rect_t *coor)
{
	s_rect_t clip;
	if (s_region_rect_intersect(xynth_server->window->surface->buf, coor, &clip)) {
		return;
	}
	bpp_fillbox_o(xynth_server->window->surface, S_MATRIX_FREE, clip.x, clip.y, clip.w, clip.h, 0);
}

void s_server_surface_lock (void)
{
	s_rect_t coor;
	coor.x = 0;
	coor.y = 0;
	coor.w = xynth_server->window->surface->width;
	coor.h = xynth_server->window->surface->height;
	s_server_surface_matrix_add_id(S_MATRIX_LOCKED, &coor);
}

void s_server_surface_refresh (void)
{
	s_rect_t coor;
	coor.x = 0;
	coor.y = 0;
	coor.w = xynth_server->window->surface->width;
	coor.h = xynth_server->window->surface->height;
	s_server_pri_set(SURFACE_REFRESH, &coor);
}
