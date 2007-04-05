
#include <xynth.h>

#include "cairoint.h"
#include "cairo-xynth.h"

#if 1
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

typedef struct cairo_xynth_surface_s {
	cairo_surface_t cairo;
	s_render_t *render;
} cairo_xynth_surface_t;

static cairo_surface_t * _cairo_xynth_surface_create_similar (void *abstract_surface, cairo_content_t cairo_content, int width, int height)
{
	ENTER();
	NIY();
	LEAVE();
	return NULL;
}

static cairo_status_t  _cairo_xynth_surface_finish (void *abstract_surface)
{
	ENTER();
	NIY();
	LEAVE();
	return CAIRO_STATUS_SUCCESS;
}

static cairo_status_t _cairo_xynth_surface_acquire_source_image (void *abstract_surface, cairo_image_surface_t **image_out, void **image_extra)
{
	ENTER();
	NIY();
	LEAVE();
	return 0;
}

static void _cairo_xynth_surface_release_source_image (void *abstract_surface, cairo_image_surface_t *image, void *image_extra)
{
	ENTER();
	NIY();
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
	ENTER();
	NIY();
	LEAVE();
	return CAIRO_STATUS_SUCCESS;
}

static cairo_int_status_t _cairo_xynth_surface_composite (cairo_operator_t cairo_operator, cairo_pattern_t *src_pattern, cairo_pattern_t *mask_pattern, void *abstract_surface, int src_x, int src_y, int mask_x, int mask_y, int dst_x, int dst_y, unsigned int width, unsigned int height)
{
	ENTER();
	NIY();
	LEAVE();
	return CAIRO_STATUS_SUCCESS;
}

static cairo_int_status_t _cairo_xynth_surface_fill_rectangles (void *abstract_surface, cairo_operator_t cairo_operator, const cairo_color_t *cairo_color, cairo_rectangle_int16_t *cairo_rects, int num_rects)
{
	ENTER();
	NIY();
	LEAVE();
	return CAIRO_STATUS_SUCCESS;
}

static cairo_int_status_t _cairo_xynth_surface_composite_trapezoids (cairo_operator_t operator, cairo_pattern_t *source, void *abstract_surface, cairo_antialias_t antialias, int src_x, int src_y, int dst_x, int dst_y, unsigned int width, unsigned int height, cairo_trapezoid_t *traps, int num_traps)
{
	ENTER();
	NIY();
	LEAVE();
	return 0;
}

static cairo_int_status_t _cairo_xynth_surface_set_clip_region (void *abstract_surface, pixman_region16_t *region)
{
	ENTER();
	NIY();
	LEAVE();
	return CAIRO_STATUS_SUCCESS;
}

static cairo_int_status_t _cairo_xynth_surface_get_extents (void *abstract_surface, cairo_rectangle_int16_t *rectangle)
{
	ENTER();
	NIY();
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

cairo_surface_t * cairo_xynth_surface_create (unsigned int width, unsigned int height, cairo_content_t content)
{
	ENTER();
	NIY();
	LEAVE();
	return NULL;
}

cairo_bool_t _cairo_surface_is_xynth (const cairo_surface_t *surface)
{
	return surface->backend == &cairo_xynth_surface_backend;
}
