/***************************************************************************
    begin                : Wed Feb 19 2003
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

#include "../lib/xynth_.h"
#include "server.h"

void s_server_surface_matrix_find (s_rect_t *coor, int *dm)
{
	int i;
	int h;
	int w;
	int w_;
	s_rect_t clip;
	s_rect_t inter;
	unsigned char *tmp;

	clip.x = 0;
	clip.y = 0;
	clip.w = server->window->surface->width;
	clip.h = server->window->surface->height;
	tmp = server->window->surface->matrix;

	if (s_rect_intersect(&clip, coor, &inter)) {
		return;
	}

        h = inter.h;
        w = inter.w;
        w_ = clip.w - inter.w;
        tmp += ((inter.y * clip.w) + inter.x);

	while (h--) {
		w = inter.w;
		while (w--) {
			i = *tmp;
			if (i < S_CLIENTS_MAX) {
				dm[i] = 1;
			}
			tmp++;
		}
		tmp += w_;
	}
}

void s_server_surface_matrix_add (int id, s_rect_t *coor)
{
	int h;
	int w;
	int w_;
	s_rect_t clip;
	s_rect_t intr_;
	s_rect_t inter;
	unsigned char *tmp;

	if (s_rect_intersect(coor, &server->client[id].buf, &intr_)) {
		return;
	}

	clip.x = 0;
	clip.y = 0;
	clip.w = server->window->surface->width;
	clip.h = server->window->surface->height;

	if (s_rect_intersect(&intr_, &clip, &inter)) {
		return;
	}

        h = inter.h;
        w = inter.w;
        w_ = clip.w - inter.w;
        tmp = (server->window->surface->matrix + (inter.y * clip.w) + inter.x);

	while (h--) {
		w = inter.w;
		while (w--) {
			*tmp++ = id;
		}
		tmp += w_;
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
	clip.w = server->window->surface->width;
	clip.h = server->window->surface->height;
	tmp = server->window->surface->matrix;

	if (s_rect_intersect(coor, &clip, &inter)) {
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

	if (s_rect_intersect(coor, mcoor, &intr_)) {
		return;
	}

	clip.x = 0;
	clip.y = 0;
	clip.w = server->window->surface->width;
	clip.h = server->window->surface->height;

	if (s_rect_intersect(&intr_, &clip, &inter)) {
		return;
	}

        h = inter.h;
        w = inter.w;
        w_ = clip.w - w;
        m_ = mcoor->w - w;
        tmp = (server->window->surface->matrix + (inter.y * clip.w) + inter.x);
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

void s_server_surface_matrix_del (int id)
{
	s_server_surface_matrix_add_id(S_MATRIX_DELETED, &server->client[id].buf);
}

void s_server_surface_matrix_del_coor (s_rect_t *coor)
{
	s_server_surface_matrix_add_id(S_MATRIX_DELETED, coor);
}

void s_server_surface_clean (s_rect_t *coor)
{
	s_rect_t intersect;

	if (s_rect_clip_virtual(server->window->surface, coor->x, coor->y, coor->w, coor->h, &intersect)) {
		return;
	}

	bpp_fillbox_o(server->window->surface, S_MATRIX_FREE, intersect.x, intersect.y, intersect.w, intersect.h, 0);
}

void s_server_surface_background (s_rect_t *coor)
{
	int h;
	int w;
	int w_;
	s_rect_t intersect;
	unsigned char *tmp;

	tmp = server->window->surface->matrix;

	if (s_rect_clip_virtual(server->window->surface, coor->x, coor->y, coor->w, coor->h, &intersect)) {
		return;
	}
	bpp_fillbox_o(server->window->surface, S_MATRIX_DELETED, intersect.x, intersect.y, intersect.w, intersect.h, 0);
	
	h = intersect.h;
        w = intersect.w;
        w_ = server->window->surface->width - w;
        tmp += ((intersect.y * server->window->surface->width) + intersect.x);

	while (h--) {
		w = intersect.w;
		while (w--) {
			if (*tmp == S_MATRIX_DELETED) {
				*tmp = S_MATRIX_FREE;
			}
			tmp++;
		}
		tmp += w_;
	}
}

void s_server_surface_lock_real (void)
{
	int i;
	s_rect_t coor;
	unsigned char *mat;
	coor.x = 0;
	coor.y = 0;
	coor.w = server->window->surface->width;
	coor.h = server->window->surface->height;
	i = coor.w * coor.h;
	mat = server->window->surface->matrix;
	while (i--) {
		*mat++ = S_MATRIX_DELETED;
	}
}

void s_server_surface_refresh (void)
{
	s_rect_t coor;
	coor.x = 0;
	coor.y = 0;
	coor.w = server->window->surface->width;
	coor.h = server->window->surface->height;
        s_server_surface_lock_real();
	s_server_pri_set(SURFACE_REFRESH, &coor);
}
