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
	for (n = 0; n < region->nrects; n++) {
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
		}
	}
	s_region_extents_calculate(region);
	return 0;
err0:	return -1;
}

int s_region_addrect (s_region_t *region, s_rect_t *rect)
{
	if (region == NULL) {
		goto err0;
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

int s_region_unify (s_region_t *region)
{
	s_region_extents_calculate(region);
	return 0;
}

int s_region_combine (s_region_t *region)
{
	s_region_extents_calculate(region);
	return 0;
}
