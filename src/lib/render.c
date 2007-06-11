/***************************************************************************
    begin                : Mon Mar 9 2007
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
#include "pixman.h"

struct s_render_pixman_s {
	pixman_image_t *pixman_image;
};

static pixman_operator_t _s_render_pixman_operator (S_RENDER_OPERATOR operator)
{
	switch (operator) {
		case S_RENDER_OPERATOR_CLEAR:
			return PIXMAN_OPERATOR_CLEAR;
		case S_RENDER_OPERATOR_SRC:
			return PIXMAN_OPERATOR_SRC;
		case S_RENDER_OPERATOR_OVER:
			return PIXMAN_OPERATOR_OVER;
		case S_RENDER_OPERATOR_IN:
			return PIXMAN_OPERATOR_IN;
		case S_RENDER_OPERATOR_OUT:
			return PIXMAN_OPERATOR_OUT;
		case S_RENDER_OPERATOR_ATOP:
			return PIXMAN_OPERATOR_ATOP;
		case S_RENDER_OPERATOR_DST:
			return PIXMAN_OPERATOR_DST;
		case S_RENDER_OPERATOR_OVER_REVERSE:
			return PIXMAN_OPERATOR_OVER_REVERSE;
		case S_RENDER_OPERATOR_IN_REVERSE:
			return PIXMAN_OPERATOR_IN_REVERSE;
		case S_RENDER_OPERATOR_OUT_REVERSE:
			return PIXMAN_OPERATOR_OUT_REVERSE;
		case S_RENDER_OPERATOR_ATOP_REVERSE:
			return PIXMAN_OPERATOR_ATOP_REVERSE;
		case S_RENDER_OPERATOR_XOR:
			return PIXMAN_OPERATOR_XOR;
		case S_RENDER_OPERATOR_ADD:
			return PIXMAN_OPERATOR_ADD;
		case S_RENDER_OPERATOR_SATURATE:
			return PIXMAN_OPERATOR_SATURATE;
		default:
			return PIXMAN_OPERATOR_OVER;
	}
}

static pixman_format_t * _s_render_create_pixman_format (S_RENDER_FORMAT render_format)
{
	switch (render_format) {
		case S_RENDER_FORMAT_A1:
			return s_render_pixman_format_create(PIXMAN_FORMAT_NAME_A1);
		case S_RENDER_FORMAT_A8:
			return s_render_pixman_format_create(PIXMAN_FORMAT_NAME_A8);
		case S_RENDER_FORMAT_RGB16:
			return s_render_pixman_format_create(PIXMAN_FORMAT_NAME_RGB16_565);
		case S_RENDER_FORMAT_RGB24:
			return s_render_pixman_format_create(PIXMAN_FORMAT_NAME_RGB24);
		case S_RENDER_FORMAT_ARGB32:
			return s_render_pixman_format_create(PIXMAN_FORMAT_NAME_ARGB32);
		default:
			return NULL;
	}
}

int s_render_uninit (s_render_t *render)
{
	s_render_pixman_image_destroy(render->pixman->pixman_image);
	free(render->pixman);
	free(render);
	return 0;
}

int s_render_init (s_render_t **render, S_RENDER_FORMAT render_format, int width, int height)
{
	s_render_t *rnd;
	pixman_format_t *pixman_format;
	
	rnd = (s_render_t *) malloc(sizeof(s_render_t));
	if (rnd == NULL) {
		goto err0;
	}
	memset(rnd, 0, sizeof(s_render_t));

	rnd->pixman = (s_render_pixman_t *) malloc(sizeof(s_render_pixman_t));
	if (rnd->pixman == NULL) {
		goto err1;
	}
	memset(rnd->pixman, 0, sizeof(s_render_pixman_t));
	
	pixman_format = _s_render_create_pixman_format(render_format);
	if (pixman_format == NULL) {
		goto err2;
	}
	rnd->pixman->pixman_image = s_render_pixman_image_create(pixman_format, width, height);
	s_render_pixman_format_destroy(pixman_format);
	if (rnd->pixman->pixman_image == NULL) {
		goto err3;
	}
	rnd->format = render_format;
	rnd->has_clip = 0;
	rnd->data = (unsigned char *) s_render_pixman_image_get_data(rnd->pixman->pixman_image);
	rnd->width = s_render_pixman_image_get_width(rnd->pixman->pixman_image);
	rnd->height = s_render_pixman_image_get_height(rnd->pixman->pixman_image);
	rnd->stride = s_render_pixman_image_get_stride(rnd->pixman->pixman_image);
	rnd->depth = s_render_pixman_image_get_depth(rnd->pixman->pixman_image);
	
	*render = rnd;
	return 0;
err3:
err2:	free(rnd->pixman);	
err1:	free(rnd);
err0:	*render = NULL;
	return -1;
}

int s_render_init_for_data (s_render_t **render, unsigned char *data, S_RENDER_FORMAT render_format, int width, int height, int bpp, int stride)
{
	s_render_t *rnd;
	pixman_format_t *pixman_format;
	
	rnd = (s_render_t *) malloc(sizeof(s_render_t));
	if (rnd == NULL) {
		goto err0;
	}
	memset(rnd, 0, sizeof(s_render_t));

	rnd->pixman = (s_render_pixman_t *) malloc(sizeof(s_render_pixman_t));
	if (rnd->pixman == NULL) {
		goto err1;
	}
	memset(rnd->pixman, 0, sizeof(s_render_pixman_t));
	
	pixman_format = _s_render_create_pixman_format(render_format);
	if (pixman_format == NULL) {
		goto err2;
	}
	rnd->pixman->pixman_image = s_render_pixman_image_create_for_data((pixman_bits_t *) data, pixman_format, width, height, bpp, stride);
	s_render_pixman_format_destroy(pixman_format);
	if (rnd->pixman->pixman_image == NULL) {
		goto err3;
	}
	rnd->format = render_format;
	rnd->has_clip = 0;
	rnd->data = (unsigned char *) s_render_pixman_image_get_data(rnd->pixman->pixman_image);
	rnd->width = s_render_pixman_image_get_width(rnd->pixman->pixman_image);
	rnd->height = s_render_pixman_image_get_height(rnd->pixman->pixman_image);
	rnd->stride = s_render_pixman_image_get_stride(rnd->pixman->pixman_image);
	rnd->depth = s_render_pixman_image_get_depth(rnd->pixman->pixman_image);
	
	*render = rnd;
	return 0;
err3:
err2:	free(rnd->pixman);	
err1:	free(rnd);
err0:	*render = NULL;
	return -1;
}

int s_render_set_filter (s_render_t *render, S_RENDER_FILTER filter)
{
	pixman_filter_t pixman_filter;
	switch (filter) {
		case S_RENDER_FILTER_FAST:
			pixman_filter = PIXMAN_FILTER_FAST;
			break;
		case S_RENDER_FILTER_GOOD:
			pixman_filter = PIXMAN_FILTER_GOOD;
			break;
		case S_RENDER_FILTER_BEST:
			pixman_filter = PIXMAN_FILTER_BEST;
			break;
		case S_RENDER_FILTER_NEAREST:
			pixman_filter = PIXMAN_FILTER_NEAREST;
			break;
		case S_RENDER_FILTER_BILINEAR:
			pixman_filter = PIXMAN_FILTER_BILINEAR;
			break;
		default:
			pixman_filter = PIXMAN_FILTER_BEST;
	}
	s_render_pixman_image_set_filter(render->pixman->pixman_image, pixman_filter);
	return 0;
}

int s_render_set_repeat (s_render_t *render, S_RENDER_REPEAT repeat)
{
	switch (repeat) {
		case S_RENDER_REPEAT_NONE:
			s_render_pixman_image_set_repeat(render->pixman->pixman_image, PIXMAN_REPEAT_NONE);
			break;
		case S_RENDER_REPEAT_NORMAL:
			s_render_pixman_image_set_repeat(render->pixman->pixman_image, PIXMAN_REPEAT_NORMAL);
			break;
		case S_RENDER_REPEAT_REFLECT:
			s_render_pixman_image_set_repeat(render->pixman->pixman_image, PIXMAN_REPEAT_REFLECT);
			break;
		case S_RENDER_REPEAT_PAD:
			s_render_pixman_image_set_repeat(render->pixman->pixman_image, PIXMAN_REPEAT_PAD);
			break;
	}
	return 0;
}

int s_render_set_transform_matrix (s_render_t *render, s_render_matrix_t *matrix)
{
	int i;
	int j;
	pixman_transform_t pixman_transform;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			pixman_transform.matrix[i][j] = matrix->matrix[i][j];
		}
	}
	s_render_pixman_image_set_transform(render->pixman->pixman_image, &pixman_transform);
	return 0;
}

int s_render_add_trapezoid (s_render_t *render, int x_off, int y_off, int ntraps, s_render_trap_t *traps)
{
	int i;
	pixman_trapezoid_t *pixman_traps;
	if (ntraps > 0) {
		pixman_traps = (pixman_trapezoid_t *) malloc(sizeof(pixman_trapezoid_t) * ntraps);
		if (pixman_traps == NULL) {
			return -1;
		}
	} else {
		ntraps = 0;
		pixman_traps = NULL;
	}
	for (i = 0; i < ntraps; i++) {
		pixman_traps[i].top = traps[i].top;
		pixman_traps[i].bottom = traps[i].bottom;
		pixman_traps[i].left.p1.x = traps[i].left1x;
		pixman_traps[i].left.p1.y = traps[i].left1y;
		pixman_traps[i].left.p2.x = traps[i].left2x;
		pixman_traps[i].left.p2.y = traps[i].left2y;
		pixman_traps[i].right.p1.x = traps[i].right1x;
		pixman_traps[i].right.p1.y = traps[i].right1y;
		pixman_traps[i].right.p2.x = traps[i].right2x;
		pixman_traps[i].right.p2.y = traps[i].right2y;
	}
	s_render_pixman_add_trapezoids(render->pixman->pixman_image, x_off, y_off, pixman_traps, ntraps);
	return 0;
}

pixman_region_status_t pixman_region_addrect (pixman_region16_t *region, int x1, int y1, int x2, int y2);
int s_render_set_clip (s_render_t *render, int nrects, s_rect_t *rects)
{
	int i;
	pixman_region16_t *pixman_region;
	pixman_region = s_render_pixman_region_create();
	if (pixman_region == NULL) {
		return -1;
	}
	for (i = 0; i < nrects; i++) {
		pixman_region_addrect(pixman_region, rects[i].x, rects[i].y, rects[i].w + rects[i].x, rects[i].h + rects[i].y);
	}
	s_render_pixman_image_set_clip_region(render->pixman->pixman_image, pixman_region);
	s_render_pixman_region_destroy(pixman_region);
	render->has_clip = (nrects > 0) ? 1 : 0;
	return 0;
}

int s_render_fill_rectangles (S_RENDER_OPERATOR operator, s_render_t *render, s_render_color_t *color, int nrects, s_rect_t *rects)
{
	int n;
	pixman_color_t pixman_color;
	pixman_rectangle_t *pixman_rects;
	pixman_color.red = color->red;
	pixman_color.green = color->green;
	pixman_color.blue = color->blue;
	pixman_color.alpha = color->alpha;
	pixman_rects = (pixman_rectangle_t *) malloc(sizeof(pixman_rectangle_t) * nrects);
	if (pixman_rects == NULL) {
		return -1;
	}
	for (n = 0; n < nrects; n++) {
		pixman_rects[n].x = rects[n].x;
		pixman_rects[n].y = rects[n].y;
		pixman_rects[n].width = rects[n].w;
		pixman_rects[n].height = rects[n].h;
	}
	s_render_pixman_fill_rectangles(_s_render_pixman_operator(operator), render->pixman->pixman_image, &pixman_color, pixman_rects, n);
	free(pixman_rects);
	return 0;
}

int s_render_composite (S_RENDER_OPERATOR operator, s_render_t *source, s_render_t *mask, s_render_t *dest, int src_x, int src_y, int mask_x, int mask_y, int dest_x, int dest_y, int width, int height)
{
	s_render_pixman_composite(_s_render_pixman_operator(operator),
	                          source->pixman->pixman_image,
	                          (mask) ? mask->pixman->pixman_image : NULL,
	                          dest->pixman->pixman_image,
	                          src_x, src_y,
	                          mask_x, mask_y,
	                          dest_x, dest_y,
	                          width, height);
	return 0;
}
