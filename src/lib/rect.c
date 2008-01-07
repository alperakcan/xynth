/***************************************************************************
    begin                : Wed May 26 2004
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

#include "xynth_.h"

int s_rect_clip_virtual (s_surface_t *surface, int x, int y, int w, int h, s_rect_t *coor)
{
	s_rect_t thip;
	s_rect_t clip;

	thip.x = x;
	thip.y = y;
	thip.w = w;
	thip.h = h;

	clip.x = 0;
	clip.y = 0;
	clip.w = surface->width;
	clip.h = surface->height;

	if (s_region_rect_intersect(&thip, &clip, coor)) {
		return -1;
	}
	return 0;
}

int s_rect_clip_real (s_surface_t *surface, int x, int y, int w, int h, s_rect_t *coor)
{
        s_rect_t that;
	s_rect_t thip;
	s_rect_t cliv;
	s_rect_t clir;
	
        thip.x = x;
	thip.y = y;
	thip.w = w;
	thip.h = h;

	cliv.x = 0;
	cliv.y = 0;
	cliv.w = MIN(surface->buf->w, surface->width);
	cliv.h = MIN(surface->buf->h, surface->height);

	clir.x = 0;
	clir.y = 0;
	clir.w = surface->linear_buf_width;
	clir.h = surface->linear_buf_height;

	if (s_region_rect_intersect(&thip, &cliv, &that)) {
		return -1;
	}
	that.x += surface->buf->x;
	that.y += surface->buf->y;
	if (s_region_rect_intersect(&clir, &that, coor)) {
		return -1;
	}
	coor->x -= surface->buf->x;
	coor->y -= surface->buf->y;
	return 0;
}
