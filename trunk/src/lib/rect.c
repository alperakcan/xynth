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

int s_rect_merge (s_rect_t *r1, s_rect_t *r2, s_rect_t *r)
{
	int x31;
	int x32;
	int y31;
	int y32;

	x31 = MIN(r1->x, r2->x);
	x32 = MAX(r1->x + r1->w - 1, r2->x + r2->w - 1);

	y31 = MIN(r1->y, r2->y);
	y32 = MAX(r1->y + r1->h - 1, r2->y + r2->h - 1);
	
	r->x = x31;
	r->y = y31;
	r->w = x32 - x31 + 1;
	r->h = y32 - y31 + 1;
	
	return 0;
}

int s_rect_intersect (s_rect_t *r1, s_rect_t *r2, s_rect_t *r)
{
	int x31;
	int x32;
	int y31;
	int y32;

	x31 = MAX(r1->x, r2->x);
	x32 = MIN(r1->x + r1->w - 1, r2->x + r2->w - 1);

	y31 = MAX(r1->y, r2->y);
	y32 = MIN(r1->y + r1->h - 1, r2->y + r2->h - 1);

	if ((x31 > x32) ||
	    (y31 > y32)) {
		return -1;
	}

	r->x = x31;
	r->y = y31;
	r->w = x32 - x31 + 1;
	r->h = y32 - y31 + 1;

	return 0;
}

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

	if (s_rect_intersect(&thip, &clip, coor)) {
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

	if (s_rect_intersect(&thip, &cliv, &that)) {
		return -1;
	}
	that.x += surface->buf->x;
	that.y += surface->buf->y;
	if (s_rect_intersect(&clir, &that, coor)) {
		return -1;
	}
	coor->x -= surface->buf->x;
	coor->y -= surface->buf->y;
	return 0;
}

int s_rect_difference_add (s_list_t *list, int x, int y, int w, int h)
{
	s_rect_t *rect;
	if (w <= 0 ||
	    h <= 0) {
		return -1;
	}
	rect = (s_rect_t *) s_malloc(sizeof(s_rect_t));
	rect->x = x;
	rect->y = y;
	rect->w = w;
	rect->h = h;
	s_list_add(list, rect, -1);
	return 0;
}

/* d = r1 - r0
*/
int s_rect_difference (s_rect_t *r1, s_rect_t *r0, s_list_t *list)
{
	/* big one */
	int x0;
	int y0;
	int w0;
	int h0;
	/* small one */
	int x1;
	int y1;
	int w1;
	int h1;
        s_rect_t r2;

	if (s_rect_intersect(r1, r0, &r2)) {
		/* xxxxxxxx
		   xxxxxxxx
		   xxxxxxxx  ........
		   xxxxxxxx  ........
		             ........
		             ........
		 */
		s_rect_difference_add(list, r1->x, r1->y, r1->w, r1->h);
		goto end;
	}

	x0 = r1->x;
	y0 = r1->y;
	w0 = r1->w;
	h0 = r1->h;

	x1 = r2.x;
	y1 = r2.y;
	w1 = r2.w;
	h1 = r2.h;

	/* xxxxxxxx
	   xx....xx
	   xx....xx
	   xxxxxxxx
	 */
	s_rect_difference_add(list, x0, y0, x1 - x0, h0);
	s_rect_difference_add(list, x1, y0, w1, y1 - y0);
	s_rect_difference_add(list, x1 + w1, y0, (x0 + w0) - (x1 + w1), h0);
	s_rect_difference_add(list, x1, y1 + h1, w1, (y0 + h0) - (y1 + h1));

end:	return list->nb_elt;
}
