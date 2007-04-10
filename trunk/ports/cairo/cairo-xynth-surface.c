
#include <xynth.h>

#include "cairoint.h"
#include "cairo-xynth.h"

#if 0
#define DEBUG(fmt...) {\
	printf(fmt);\
	printf(" [%s (%s:%d)]\n", __FUNCTION__, __FILE__, __LINE__);\
}
#else
#define DEBUG(fmt...) do { } while (0)
#endif

#define ENTER() DEBUG("Enter");
#define LEAVE() DEBUG("Leave");
#define NIY()   {\
	DEBUG("Not Implemented Yet");\
	exit(1);\
}
#define ASSERT() {\
	DEBUG("This should not happen");\
	assert(0);\
}

typedef struct cairo_xynth_surface_s {
	cairo_surface_t cairo;
	s_render_t *render;
} cairo_xynth_surface_t;

static S_RENDER_FORMAT _cairo_xynth_format_from_cairo_format (cairo_format_t cairo_format)
{
	switch (cairo_format) {
		case CAIRO_FORMAT_ARGB32:
			return S_RENDER_FORMAT_ARGB32;
		case CAIRO_FORMAT_RGB24:
			return S_RENDER_FORMAT_RGB24;
		case CAIRO_FORMAT_A8:
			return S_RENDER_FORMAT_A8;
		case CAIRO_FORMAT_A1:
			return S_RENDER_FORMAT_A1;
	}
	ASSERT();
	return S_RENDER_FORMAT_NONE;
}

static cairo_format_t _cairo_format_from_xynth_format (S_RENDER_FORMAT render_format)
{
	switch (render_format) {
		case S_RENDER_FORMAT_ARGB32:
			return CAIRO_FORMAT_ARGB32;
		case S_RENDER_FORMAT_RGB24:
			return CAIRO_FORMAT_RGB24;
		case S_RENDER_FORMAT_A8:
			return CAIRO_FORMAT_A8;
		case S_RENDER_FORMAT_A1:
			return CAIRO_FORMAT_A1;
	}
	ASSERT();
	return CAIRO_FORMAT_ARGB32;
}

static S_RENDER_OPERATOR _cairo_xynth_operator (cairo_operator_t operator)
{
	switch (operator) {
		case CAIRO_OPERATOR_CLEAR:
			return S_RENDER_OPERATOR_CLEAR;
		case CAIRO_OPERATOR_SOURCE:
			return S_RENDER_OPERATOR_SRC;
		case CAIRO_OPERATOR_OVER:
			return S_RENDER_OPERATOR_OVER;
		case CAIRO_OPERATOR_IN:
			return S_RENDER_OPERATOR_IN;
		case CAIRO_OPERATOR_OUT:
			return S_RENDER_OPERATOR_OUT;
		case CAIRO_OPERATOR_ATOP:
			return S_RENDER_OPERATOR_ATOP;
		case CAIRO_OPERATOR_DEST:
			return S_RENDER_OPERATOR_DST;
		case CAIRO_OPERATOR_DEST_OVER:
			return S_RENDER_OPERATOR_OVER_REVERSE;
		case CAIRO_OPERATOR_DEST_IN:
			return S_RENDER_OPERATOR_IN_REVERSE;
		case CAIRO_OPERATOR_DEST_OUT:
			return S_RENDER_OPERATOR_OUT_REVERSE;
		case CAIRO_OPERATOR_DEST_ATOP:
			return S_RENDER_OPERATOR_ATOP_REVERSE;
		case CAIRO_OPERATOR_XOR:
			return S_RENDER_OPERATOR_XOR;
		case CAIRO_OPERATOR_ADD:
			return S_RENDER_OPERATOR_ADD;
		case CAIRO_OPERATOR_SATURATE:
			return S_RENDER_OPERATOR_SATURATE;
		default:
			return S_RENDER_OPERATOR_OVER;
	}
}

static cairo_surface_t * _cairo_xynth_surface_create_similar (void *abstract_surface, cairo_content_t cairo_content, int width, int height)
{
	cairo_surface_t *surface;
	ENTER();
	if (!CAIRO_CONTENT_VALID(cairo_content)) {
		_cairo_error(CAIRO_STATUS_INVALID_CONTENT);
		surface = NULL;
	} else {
		surface = cairo_xynth_surface_create_with_content(cairo_content, width, height);
	}
	LEAVE();
	return surface;
}

static cairo_status_t  _cairo_xynth_surface_finish (void *abstract_surface)
{
	cairo_xynth_surface_t *surface;
	ENTER();
	surface = (cairo_xynth_surface_t *) abstract_surface;
	s_render_uninit(surface->render);
	LEAVE();
	return CAIRO_STATUS_SUCCESS;
}

static cairo_status_t _cairo_xynth_surface_acquire_source_image (void *abstract_surface, cairo_image_surface_t **image_out, void **image_extra)
{
	cairo_xynth_surface_t *surface; 	 
	ENTER();
	surface = (cairo_xynth_surface_t *) abstract_surface;
	*image_out = (cairo_image_surface_t *) cairo_image_surface_create_for_data((void *) surface->render->data, _cairo_format_from_xynth_format(surface->render->format), surface->render->width, surface->render->height, surface->render->stride); 	 
	*image_extra = NULL; 	 
	DEBUG("(*image_out)->base.status: 0x%08x", (*image_out)->base.status); 	 
	LEAVE();
	return 0;
}

static void _cairo_xynth_surface_release_source_image (void *abstract_surface, cairo_image_surface_t *image, void *image_extra)
{
	ENTER();
	cairo_surface_destroy(&image->base);
	LEAVE();
}

static cairo_status_t _cairo_xynth_surface_acquire_dest_image (void *abstract_surface, cairo_rectangle_int16_t *interest_rect, cairo_image_surface_t **image_out, cairo_rectangle_int16_t *image_rect, void **image_extra)
{
	ENTER();
	NIY();
	LEAVE();
	return CAIRO_STATUS_SUCCESS;
}

static void _cairo_xynth_surface_release_dest_image (void *abstract_surface, cairo_rectangle_int16_t *interest_rect, cairo_image_surface_t *image, cairo_rectangle_int16_t *image_rect, void *image_extra)
{
	ENTER();
	NIY();
	LEAVE();
}

static cairo_status_t _cairo_xynth_surface_clone_similar (void *abstract_surface, cairo_surface_t *src, int src_x, int src_y, int width, int height, cairo_surface_t **clone_out)
{
	s_render_t *tmp;
	cairo_status_t status;
	cairo_xynth_surface_t *clone;
	cairo_xynth_surface_t *surface;
	ENTER();
	surface = (cairo_xynth_surface_t *) abstract_surface;
	if (src->backend == surface->cairo.backend) {
		*clone_out = cairo_surface_reference(src);
		status = CAIRO_STATUS_SUCCESS;
	} else if (_cairo_surface_is_image(src)) {
		cairo_image_surface_t *image_src = (cairo_image_surface_t *) src;
		if (!CAIRO_FORMAT_VALID(image_src->format)) {
			status = CAIRO_INT_STATUS_UNSUPPORTED;
			goto out;
		}
		clone = (cairo_xynth_surface_t *) cairo_xynth_surface_create(image_src->width, image_src->height, image_src->format);
		if (clone->cairo.status) {
			status = CAIRO_STATUS_NO_MEMORY;
			goto out;
		}
		s_render_init_for_data(&tmp, image_src->data, _cairo_xynth_format_from_cairo_format(image_src->format), image_src->width, image_src->height, image_src->depth, image_src->stride);
		s_render_composite(S_RENDER_OPERATOR_SRC, tmp, NULL, clone->render, src_x, src_y, 0, 0, src_x, src_y, width, height);
		s_render_uninit(tmp);
		*clone_out = &clone->cairo;
		status = CAIRO_STATUS_SUCCESS;
	} else {
		status = CAIRO_INT_STATUS_UNSUPPORTED;
	}
out:
	LEAVE();
	return status;
}

static void _cairo_matrix_to_render_matrix (const cairo_matrix_t *matrix, s_render_matrix_t *render_matrix)
{
	static const s_render_matrix_t render_identity_matrix = {{
		{1 << 16,        0,       0},
		{       0, 1 << 16,       0},
		{       0,       0, 1 << 16}
	}};
	if (_cairo_matrix_is_identity(matrix)) {
		*render_matrix = render_identity_matrix;
	} else {
		render_matrix->matrix[0][0] = _cairo_fixed_from_double(matrix->xx);
		render_matrix->matrix[0][1] = _cairo_fixed_from_double(matrix->xy);
		render_matrix->matrix[0][2] = _cairo_fixed_from_double(matrix->x0);
		render_matrix->matrix[1][0] = _cairo_fixed_from_double(matrix->yx);
		render_matrix->matrix[1][1] = _cairo_fixed_from_double(matrix->yy);
		render_matrix->matrix[1][2] = _cairo_fixed_from_double(matrix->y0);
		render_matrix->matrix[2][0] = 0;
		render_matrix->matrix[2][1] = 0;
		render_matrix->matrix[2][2] = 1 << 16;
	}
}

static cairo_status_t _cairo_xynth_surface_set_matrix (cairo_xynth_surface_t *surface, const cairo_matrix_t *matrix)
{
	s_render_matrix_t render_matrix;
	_cairo_matrix_to_render_matrix(matrix, &render_matrix);
	s_render_set_transform_matrix(surface->render, &render_matrix);
	return CAIRO_STATUS_SUCCESS;
}

static cairo_status_t _cairo_xynth_surface_set_filter (cairo_xynth_surface_t *surface, cairo_filter_t filter)
{
	S_RENDER_FILTER render_filter;
	switch (filter) {
		case CAIRO_FILTER_FAST:
			render_filter = S_RENDER_FILTER_FAST;
			break;
		case CAIRO_FILTER_GOOD:
			render_filter = S_RENDER_FILTER_GOOD;
			break;
		case CAIRO_FILTER_BEST:
			render_filter = S_RENDER_FILTER_BEST;
			break;
		case CAIRO_FILTER_NEAREST:
			render_filter = S_RENDER_FILTER_NEAREST;
			break;
		case CAIRO_FILTER_BILINEAR:
			render_filter = S_RENDER_FILTER_BILINEAR;
			break;
		case CAIRO_FILTER_GAUSSIAN:
		default:
			render_filter = S_RENDER_FILTER_BEST;
	}
	s_render_set_filter(surface->render, render_filter);
	return CAIRO_STATUS_SUCCESS;
}

static cairo_int_status_t _cairo_xynth_surface_set_attributes (cairo_xynth_surface_t *surface, cairo_surface_attributes_t *attributes)
{
	cairo_int_status_t status;
	status = _cairo_xynth_surface_set_matrix(surface, &attributes->matrix);
	if (status) {
		return status;
	}
	switch (attributes->extend) {
		case CAIRO_EXTEND_NONE:
			s_render_set_repeat(surface->render, S_RENDER_REPEAT_NONE);
			break;
		case CAIRO_EXTEND_REPEAT:
			s_render_set_repeat(surface->render, S_RENDER_REPEAT_NORMAL);
			break;
		case CAIRO_EXTEND_REFLECT:
			s_render_set_repeat(surface->render, S_RENDER_REPEAT_REFLECT);
			break;
		case CAIRO_EXTEND_PAD:
			s_render_set_repeat(surface->render, S_RENDER_REPEAT_PAD);
			break;
	}
	status = _cairo_xynth_surface_set_filter(surface, attributes->filter);
	return status;
}

static cairo_int_status_t _cairo_xynth_surface_composite (cairo_operator_t cairo_operator, cairo_pattern_t *src_pattern, cairo_pattern_t *mask_pattern, void *abstract_surface, int src_x, int src_y, int mask_x, int mask_y, int dst_x, int dst_y, unsigned int width, unsigned int height)
{
	cairo_int_status_t status;
	cairo_xynth_surface_t *dst;
	cairo_xynth_surface_t *src;
	cairo_xynth_surface_t *mask;
	cairo_surface_attributes_t src_attr;
	cairo_surface_attributes_t mask_attr;
	ENTER();
	dst = (cairo_xynth_surface_t *) abstract_surface;
	status = _cairo_pattern_acquire_surfaces(src_pattern, mask_pattern, &dst->cairo, src_x, src_y, mask_x, mask_y, width, height, (cairo_surface_t **) &src, (cairo_surface_t **) &mask, &src_attr, &mask_attr);
	if (status) {
		return status;
	}
	status = _cairo_xynth_surface_set_attributes(src, &src_attr);
	if (status) {
		goto out;
	}
	if (mask) {
		status = _cairo_xynth_surface_set_attributes(mask, &mask_attr);
		if (status) {
			goto out;
		}
		s_render_composite(_cairo_xynth_operator(cairo_operator),
	                           src->render,
	                           mask->render,
	                           dst->render,
	                           src_x + src_attr.x_offset,
	                           src_y + src_attr.y_offset,
	                           mask_x + mask_attr.x_offset,
	                           mask_y + mask_attr.y_offset,
	                           dst_x, dst_y,
	                           width, height);
	} else {
		s_render_composite(_cairo_xynth_operator(cairo_operator),
		                   src->render,
		                   NULL,
		                   dst->render,
		                   src_x + src_attr.x_offset,
		                   src_y + src_attr.y_offset,
		                   0, 0,
		                   dst_x, dst_y,
		                   width, height);
	}
	if (!_cairo_operator_bounded_by_source(cairo_operator)) {
		status = _cairo_surface_composite_fixup_unbounded(&dst->cairo,
		                                                  &src_attr, src->render->width, src->render->height,
		                                                  mask ? &mask_attr : NULL,
		                                                  mask ? mask->render->width : 0,
		                                                  mask ? mask->render->height : 0,
		                                                  src_x, src_y,
		                                                  mask_x, mask_y,
		                                                  dst_x, dst_y,
		                                                  width, height);
	}
 out: 	if (mask) {
 		_cairo_pattern_release_surface(mask_pattern, &mask->cairo, &mask_attr);
 	}
 	_cairo_pattern_release_surface(src_pattern, &src->cairo, &src_attr);
	LEAVE();
	return CAIRO_STATUS_SUCCESS;
}

static cairo_int_status_t _cairo_xynth_surface_fill_rectangles (void *abstract_surface, cairo_operator_t cairo_operator, const cairo_color_t *cairo_color, cairo_rectangle_int16_t *cairo_rects, int num_rects)
{
	int n;
	s_rect_t *render_rects;
	s_render_color_t render_color;
	cairo_xynth_surface_t *surface;
	ENTER();
	surface = (cairo_xynth_surface_t *) abstract_surface;
	render_color.red = cairo_color->red_short;
	render_color.green = cairo_color->green_short;
	render_color.blue = cairo_color->blue_short;
	render_color.alpha = cairo_color->alpha_short;
	render_rects = (s_rect_t *) malloc(sizeof(s_rect_t) * num_rects);
	if (render_rects == NULL) {
		_cairo_error(CAIRO_STATUS_NO_MEMORY);
		return CAIRO_STATUS_NO_MEMORY;
	}
	for (n = 0; n < num_rects; n++) {
		render_rects[n].x = cairo_rects[n].x;
		render_rects[n].y = cairo_rects[n].y;
		render_rects[n].w = cairo_rects[n].width;
		render_rects[n].h = cairo_rects[n].height;
	}
	s_render_fill_rectangles(_cairo_xynth_operator(cairo_operator), surface->render, &render_color, num_rects, render_rects);
	free(render_rects);		 
	LEAVE();
	return CAIRO_STATUS_SUCCESS;
}

static cairo_int_status_t _cairo_xynth_surface_composite_trapezoids (cairo_operator_t cairo_operator, cairo_pattern_t *pattern, void *abstract_surface, cairo_antialias_t antialias, int src_x, int src_y, int dst_x, int dst_y, unsigned int width, unsigned int height, cairo_trapezoid_t *traps, int num_traps)
{
	int i;
	int mask_bpp;
	int mask_stride;
	s_render_t *mask;
	unsigned char *mask_data;
	cairo_int_status_t status;
	cairo_xynth_surface_t *src;
	cairo_xynth_surface_t *dst;
	s_render_trap_t *render_traps;
	S_RENDER_FORMAT render_format;
	cairo_surface_attributes_t attributes;
	ENTER();
	dst = (cairo_xynth_surface_t *) abstract_surface;
	if (num_traps > 0) {
		render_traps = (s_render_trap_t *) malloc(sizeof(s_render_trap_t) * num_traps);
		if (render_traps == NULL) {
			LEAVE();
			status = CAIRO_STATUS_NO_MEMORY;
			goto out0;
		} 
	} else {
		num_traps = 0;
		render_traps = NULL;
	}
	for (i = 0; i < num_traps; i++) {
		render_traps[i].top = traps[i].top;
		render_traps[i].bottom = traps[i].bottom;
		render_traps[i].left1x = traps[i].left.p1.x;
		render_traps[i].left2x = traps[i].left.p2.x;
		render_traps[i].right1x = traps[i].right.p1.x;
		render_traps[i].right2x = traps[i].right.p2.x;
		render_traps[i].left1y = traps[i].left.p1.y;
		render_traps[i].left2y = traps[i].left.p2.y;
		render_traps[i].right1y = traps[i].right.p1.y;
		render_traps[i].right2y = traps[i].right.p2.y;
	}
	if (cairo_operator == CAIRO_OPERATOR_ADD &&
	    _cairo_pattern_is_opaque_solid (pattern) &&
	    dst->cairo.content == CAIRO_CONTENT_ALPHA &&
	    !dst->render->has_clip &&
	    antialias != CAIRO_ANTIALIAS_NONE) {
		s_render_add_trapezoid(dst->render, 0, 0, num_traps, render_traps);
	    	free(render_traps);
	    	return CAIRO_STATUS_SUCCESS;
	}
	status = _cairo_pattern_acquire_surface(pattern, &dst->cairo, src_x, src_y, width, height, (cairo_surface_t **) &src, &attributes);
	if (status) {
		goto out1;
	}
	status = _cairo_xynth_surface_set_attributes(src, &attributes);
	if (status) {
		goto out2;
	}
	switch (antialias) {
		case CAIRO_ANTIALIAS_NONE:
			render_format = S_RENDER_FORMAT_A1;
			mask_stride = (width + 31) / 8;
			mask_bpp = 1;
			break;
		case CAIRO_ANTIALIAS_GRAY:
		case CAIRO_ANTIALIAS_SUBPIXEL:
		case CAIRO_ANTIALIAS_DEFAULT:
		default:
			render_format = S_RENDER_FORMAT_A8;
			mask_stride = (width + 3) & ~3;
			mask_bpp = 8;
			break;
	}
	mask_data = calloc(1, mask_stride * height);
	if (!mask_data) {
		status = CAIRO_STATUS_NO_MEMORY;
		goto out3;
	}
	s_render_init_for_data(&mask, mask_data, render_format, width, height, mask_bpp, mask_stride);
	s_render_add_trapezoid(mask, - dst_x, - dst_y, num_traps, render_traps);
	s_render_composite(_cairo_xynth_operator(cairo_operator),
	                   src->render,
	                   mask,
	                   dst->render,
	                   src_x + attributes.x_offset,
	                   src_y + attributes.y_offset,
	                   0, 0,
	                   dst_x, dst_y,
	                   width, height);
	if (!_cairo_operator_bounded_by_mask(cairo_operator)) {
		status = _cairo_surface_composite_shape_fixup_unbounded(&dst->cairo,
		                                                        &attributes, src->render->width, src->render->height,
		                                                        width, height,
		                                                        src_x, src_y,
		                                                        0, 0,
		                                                        dst_x, dst_y, width, height);
	}
	s_render_uninit(mask);
out3:	free(mask_data);
out2:	_cairo_pattern_release_surface(pattern, &src->cairo, &attributes);
out1:	free(render_traps);
out0:	LEAVE();
	return 0;
}

static cairo_int_status_t _cairo_xynth_surface_set_clip_region (void *abstract_surface, pixman_region16_t *region)
{
	int i;
	int nboxes;
	pixman_box16_t *boxes;
	s_rect_t *render_rects;
	cairo_xynth_surface_t *surface;
	ENTER();
	surface = (cairo_xynth_surface_t *) abstract_surface;
	nboxes = pixman_region_num_rects(region);
	if (nboxes > 0) {
		render_rects = (s_rect_t *) malloc(sizeof(s_rect_t) * nboxes);
		if (render_rects == NULL) {
			LEAVE();
			return CAIRO_STATUS_NO_MEMORY;
		}
	} else {
		nboxes = 0;
		render_rects = NULL;
	}
	boxes = pixman_region_rects(region);
	for (i = 0; i < nboxes; i++) {
		render_rects[i].x = boxes[i].x1;
		render_rects[i].y = boxes[i].y1;
		render_rects[i].w = boxes[i].x2 - boxes[i].x1;
		render_rects[i].h = boxes[i].y2 - boxes[i].y1;
	}
	s_render_set_clip(surface->render, nboxes, render_rects);
	free(render_rects);
	LEAVE();
	return CAIRO_STATUS_SUCCESS;
}

static cairo_int_status_t _cairo_xynth_surface_get_extents (void *abstract_surface, cairo_rectangle_int16_t *rectangle)
{
	cairo_xynth_surface_t *surface;
	ENTER();
	surface = (cairo_xynth_surface_t *) abstract_surface;
	rectangle->x = 0;
	rectangle->y = 0;
	rectangle->width = surface->render->width;
	rectangle->height = surface->render->height;
	LEAVE();
	return CAIRO_STATUS_SUCCESS;
}

static cairo_surface_backend_t cairo_xynth_surface_backend = {
	CAIRO_SURFACE_TYPE_XYNTH,
	_cairo_xynth_surface_create_similar,
	_cairo_xynth_surface_finish,
	_cairo_xynth_surface_acquire_source_image,
	_cairo_xynth_surface_release_source_image,
	_cairo_xynth_surface_acquire_dest_image,
	_cairo_xynth_surface_release_dest_image,
	_cairo_xynth_surface_clone_similar,
	_cairo_xynth_surface_composite,
	_cairo_xynth_surface_fill_rectangles,
	_cairo_xynth_surface_composite_trapezoids,
	NULL, //_cairo_xynth_surface_copy_page,
	NULL, //_cairo_xynth_surface_show_page,
	_cairo_xynth_surface_set_clip_region,
	NULL, //_cairo_xynth_surface_intersect_clip_path,
	_cairo_xynth_surface_get_extents,
	NULL, //_cairo_xynth_surface_old_show_glyphs,
	NULL, //_cairo_xynth_surface_get_font_options,
	NULL, //_cairo_xynth_surface_flush,
	NULL, //_cairo_xynth_surface_mark_dirty_rectangle,
	NULL, //_cairo_xynth_surface_scaled_font_fini,
	NULL, //_cairo_xynth_surface_scaled_glyph_fini,
	NULL, //_cairo_xynth_surface_paint,
	NULL, //_cairo_xynth_surface_mask,
	NULL, //_cairo_xynth_surface_stroke,
	NULL, //_cairo_xynth_surface_fill,
	NULL, //_cairo_xynth_surface_show_glyphs,
	NULL, //_cairo_xynth_surface_snapshot,
};

cairo_surface_t * cairo_xynth_surface_create (unsigned int width, unsigned int height, cairo_format_t cairo_format)
{
	S_RENDER_FORMAT render_format;
	cairo_xynth_surface_t *surface;
	ENTER();
	if (!CAIRO_FORMAT_VALID(cairo_format)) {
		_cairo_error(CAIRO_STATUS_INVALID_FORMAT);
		return NULL;
	}
	render_format = _cairo_xynth_format_from_cairo_format(cairo_format);
	if (render_format == S_RENDER_FORMAT_NONE) {
		_cairo_error(CAIRO_STATUS_INVALID_FORMAT);
		return NULL;
	}
	
	surface = (cairo_xynth_surface_t *) malloc(sizeof(cairo_xynth_surface_t));
	if (surface == NULL) {
		_cairo_error(CAIRO_STATUS_NO_MEMORY);
		return NULL;
	}
	memset(surface, 0, sizeof(cairo_xynth_surface_t));
	
	if (s_render_init(&surface->render, render_format, width, height)) {
		free(surface);
		_cairo_error(CAIRO_STATUS_NO_MEMORY);
		return NULL;
	}
	_cairo_surface_init(&surface->cairo, &cairo_xynth_surface_backend, _cairo_content_from_format(cairo_format));
	LEAVE();
	return &surface->cairo;
}

cairo_surface_t * cairo_xynth_surface_create_with_content (cairo_content_t content, int width, int height)
{
	cairo_surface_t *surface;
	ENTER();
	if (!CAIRO_CONTENT_VALID(content)) {
		surface = NULL;
	} else {
		surface = cairo_xynth_surface_create( width, height, _cairo_format_from_content(content));
	}
	LEAVE();
	return surface;
}

cairo_bool_t _cairo_surface_is_xynth (const cairo_surface_t *surface)
{
	return surface->backend == &cairo_xynth_surface_backend;
}
