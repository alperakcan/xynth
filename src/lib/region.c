/***************************************************************************
    begin                : Mon Jun 11 2007
    copyright            : (C) 2007 by Alper Akcan
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

int s_region_create (s_region_t **region)
{
	s_region_t *reg;
	reg = (s_region_t *) malloc(sizeof(s_region_t));
	if (reg == NULL) {
		goto err0;
	}
	memset(reg, 0, sizeof(s_region_t));
	*region = reg;
	return 0;
err0:	*region = NULL;
	return -1;
}

int s_region_destroy (s_region_t *region)
{
	if (region) {
		if (region->rects) {
			free(region->rects);
		}
		free(region);
	}
	return 0;
}

int s_region_extents_calculate (s_region_t *region)
{
	int n;
	s_rect_t *e;
	s_rect_t *r;
	
	r = region->rects;
	n = region->nrects;
	e = &region->extents;
	if (n == 0) {
		e->x = 0;
		e->y = 0;
		e->w = 0;
		e->h = 0;
		return 0;
	}
	*e = *r;

	while (n--) {
		int ex2 = e->x + e->w;
		int ey2 = e->y + e->h;
		int rx2 = r->x + r->w;
		int ry2 = r->y + r->h;

		e->x = MIN(e->x, r->x);
		e->y = MIN(e->y, r->y);
		e->w = MAX(ex2, rx2) - e->x;
		e->h = MAX(ey2, ry2) - e->y;

		r++;
	}
	
	return 0;
}

int s_region_delrect (s_region_t *region, s_rect_t *rect)
{
	int n;
	s_rect_t *r;
	s_rect_t *rs;
	if (region == NULL) {
		goto err0;
	}
	for (n = 0; n < region->nrects;) {
		r = &region->rects[n];
		if (r->x == rect->x &&
		    r->y == rect->y &&
		    r->w == rect->w &&
		    r->h == rect->h) {
			if (region->nrects == 1) {
				region->nrects = 0;
				free(region->rects);
				region->rects = NULL;
				break;
			}
			rs = (s_rect_t *) malloc(sizeof(s_rect_t) * (region->nrects - 1));
			if (rs == NULL) {
				goto err0;
			}
			region->nrects -= 1;
			if (n > 0) {
				memcpy(rs, region->rects, n * sizeof(s_rect_t));
			}
			if (n < region->nrects) {
				memcpy(rs + n, region->rects + n + 1, (region->nrects - n) * sizeof(s_rect_t));
			}
			free(region->rects);
			region->rects = rs;
			s_region_extents_calculate(region);
		} else {
			n++;
		}
	}
	return 0;
err0:	return -1;
}

int s_region_addrect (s_region_t *region, s_rect_t *rect)
{
	if (region == NULL) {
		goto err0;
	}
	if (rect->w <= 0 || rect->h <= 0) {
		return 0;
	}
	if (s_region_delrect(region, rect)) {
		return -1;
	}
	region->rects = (s_rect_t *) realloc(region->rects, sizeof(s_rect_t) * (region->nrects + 1));
	if (region->rects != NULL) {
		region->rects[region->nrects] = *rect;
		region->nrects += 1;
	} else {
		goto err0;
	}
	s_region_extents_calculate(region);
	return 0;
err0:	return -1;
}

int s_region_num_rectangles (s_region_t *region)
{
	if (region) {
		return region->nrects;
	}
	return 0;
}

s_rect_t * s_region_rectangles (s_region_t *region)
{
	if (region) {
		return region->rects;
	}
	return NULL;
}

int s_region_extents (s_region_t *region, s_rect_t *extents)
{
	if (region) {
		*extents = region->extents;
		return 0;
	}
	return -1;
}

int s_region_copy (s_region_t **region, s_region_t *src)
{
	s_region_t *r;
	r = (s_region_t *) malloc(sizeof(s_region_t));
	if (r == NULL) {
		goto err0;
	}
	memcpy(r, src, sizeof(s_region_t));
	r->rects = (s_rect_t *) malloc(sizeof(s_rect_t) * r->nrects);
	if (r->rects == NULL) {
		goto err1;
	}
	memcpy(r->rects, src->rects, sizeof(s_rect_t) * r->nrects);
	*region = r;
	return 0;
err1:	free(r);
err0:	*region = NULL;
	return -1;
}

int s_region_rect_intersect (s_rect_t *rect1, s_rect_t *rect2, s_rect_t *result)
{
	int x31;
	int x32;
	int y31;
	int y32;
	x31 = MAX(rect1->x, rect2->x);
	x32 = MIN(rect1->x + rect1->w - 1, rect2->x + rect2->w - 1);
	y31 = MAX(rect1->y, rect2->y);
	y32 = MIN(rect1->y + rect1->h - 1, rect2->y + rect2->h - 1);
	if ((x31 > x32) || (y31 > y32)) {
		return -1;
	}
	result->x = x31;
	result->y = y31;
	result->w = x32 - x31 + 1;
	result->h = y32 - y31 + 1;
	return 0;
}

int s_region_rect_substract (s_rect_t *rect1, s_rect_t *rect2, s_region_t *result)
{
	/* first one */
	int x0;
	int y0;
	int w0;
	int h0;
	/* second one */
	int x1;
	int y1;
	int w1;
	int h1;
        s_rect_t rtmp;
	if (s_region_rect_intersect(rect1, rect2, &rtmp) != 0) {
		/* xxxxxxxx
		   xxxxxxxx
		   xxxxxxxx  ........
		   xxxxxxxx  ........
		             ........
		             ........
		 */
		printf("adding rect: %d %d, %d %d\n", rect1->x, rect1->y, rect1->w, rect1->h);
		if (s_region_addrect(result, rect1)) {
			return -1;
		} else {
			return 0;
		}
	}
	x0 = rect1->x;
	y0 = rect1->y;
	w0 = rect1->w;
	h0 = rect1->h;
	x1 = rtmp.x;
	y1 = rtmp.y;
	w1 = rtmp.w;
	h1 = rtmp.h;
	/* xxxxxxxx
	   xx....xx
	   xx....xx
	   xxxxxxxx
	 */
	rtmp.x = x0;
	rtmp.y = y0;
	rtmp.w = x1 - x0;
	rtmp.h = h0;
	if (rtmp.w > 0 && rtmp.h > 0) printf("adding rect: %d %d, %d %d\n", rtmp.x, rtmp.y, rtmp.w, rtmp.h);
	if (s_region_addrect(result, &rtmp)) {
		return -1;
	}
	rtmp.x = x1;
	rtmp.y = y0;
	rtmp.w = w1;
	rtmp.h = y1 - y0;
	if (rtmp.w > 0 && rtmp.h > 0) printf("adding rect: %d %d, %d %d\n", rtmp.x, rtmp.y, rtmp.w, rtmp.h);
	if (s_region_addrect(result, &rtmp)) {
		return -1;
	}
	rtmp.x = x1 + w1;
	rtmp.y = y0;
	rtmp.w = (x0 + w0) - (x1 + w1);
	rtmp.h = h0;
	if (rtmp.w > 0 && rtmp.h > 0) printf("adding rect: %d %d, %d %d\n", rtmp.x, rtmp.y, rtmp.w, rtmp.h);
	if (s_region_addrect(result, &rtmp)) {
		return -1;
	}
	rtmp.x = x1;
	rtmp.y = y1 + h1;
	rtmp.w = w1;
	rtmp.h = (y0 + h0) - (y1 + h1);
	if (rtmp.w > 0 && rtmp.h > 0) printf("adding rect: %d %d, %d %d\n", rtmp.x, rtmp.y, rtmp.w, rtmp.h);
	if (s_region_addrect(result, &rtmp)) {
		return -1;
	}
	return 0;
}

int s_region_unify (s_region_t *region)
{
	int i;
	int j;
	s_rect_t rtmp;
	s_rect_t rdel;
	printf("nrects: %d\n", region->nrects);
	for (i = 0; i < s_region_num_rectangles(region); i++) {
		for (j = i + 1; j < s_region_num_rectangles(region);) {
			if (s_region_rect_intersect(&region->rects[i], &region->rects[j], &rtmp) == 0) {
				rdel.x = region->rects[j].x;
				rdel.y = region->rects[j].y;
				rdel.w = region->rects[j].w;
				rdel.h = region->rects[j].h;
				printf("deleted rect: %d %d, %d %d, "
				       "intersected with: %d %d, %d %d, "
				       "intersection: %d %d, %d %d\n",
				       rdel.x, rdel.y, rdel.w, rdel.h,
				       region->rects[i].x, region->rects[i].y, region->rects[i].w, region->rects[i].h,
				       rtmp.x, rtmp.y, rtmp.w, rtmp.h);
				if (s_region_delrect(region, &rdel)) {
					goto err0;
				}
				if (s_region_rect_substract(&rdel, &rtmp, region)) {
					goto err0;
				}
			} else {
				j++;
			}
		}
	}
	s_region_extents_calculate(region);
	printf("nrects: %d\n", region->nrects);
	return 0;
err0:	return -1;
}

int s_region_combine (s_region_t *region)
{
	s_region_extents_calculate(region);
	return 0;
}
