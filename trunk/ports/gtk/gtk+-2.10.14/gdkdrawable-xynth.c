
#include "gdkxynth.h"

static gpointer parent_class = NULL;

static inline void gdk_xynth_set_color (GdkDrawableImplXYNTH *impl, GdkColor *color)
{
	ENTER();
	impl->color = *color;
	DEBUG("color %d %d %d", color->red >> 8, color->green >> 8, color->blue  >> 8);
	LEAVE();
}

static gboolean gdk_xynth_setup_for_drawing (GdkDrawableImplXYNTH *impl, GdkGCXYNTH *gc_private)
{
	GdkColor color = { 0, 0, 0, 0 };
	ENTER();
	if (!impl->surface) {
		return FALSE;
	}
	if (gc_private && gc_private->values_mask & GDK_GC_FOREGROUND) {
		color = gc_private->values.foreground;
	}
	if (gc_private && gc_private->values_mask & GDK_GC_FUNCTION) {
		switch (gc_private->values.function) {
			case GDK_COPY:
				ASSERT();
				break;
			case GDK_INVERT:
				color.red = color.green = color.blue = 0xFFFF;
				ASSERT();
				break;
			case GDK_XOR:
				ASSERT();
				break;
			case GDK_CLEAR:
				color.red = color.green = color.blue = 0x0;
				ASSERT();
				break;
			case GDK_NOOP:
				ASSERT();
				LEAVE();
				return FALSE;
			case GDK_SET:
				color.red = color.green = color.blue = 0xFFFF;
				ASSERT();
				break;
			default:
				g_message ("unsupported GC function %d", gc_private->values.function);
				ASSERT();
				break;
		}
	}
	gdk_xynth_set_color(impl, &color);
	LEAVE();
	return TRUE;
}

static GdkRegion * gdk_xynth_clip_region (GdkDrawable *drawable, GdkGC *gc, GdkRectangle *draw_rect)
{
	GdkRectangle rect;
	GdkRegion *tmpreg;
	GdkRegion *clip_region;
	GdkDrawableImplXYNTH *private;
	
	ENTER();
	if (GDK_IS_DRAWABLE(drawable) == 0) {
		LEAVE();
		return NULL;
	}
	if (GDK_IS_DRAWABLE_IMPL_XYNTH(drawable) == 0) {
		LEAVE();
		return NULL;
	}
	private = GDK_DRAWABLE_IMPL_XYNTH(drawable);
	if (!draw_rect) {
		rect.x      = 0;
		rect.y      = 0;
		rect.width  = private->width;
		rect.height = private->height;
		draw_rect = &rect;
	}
	clip_region = gdk_region_rectangle(draw_rect);
	if (private->buffered && private->paint_region) {
		gdk_region_intersect (clip_region, private->paint_region);
	}
	if (gc) {
		GdkGCXYNTH *gc_private = GDK_GC_XYNTH(gc);
		GdkRegion *region = gc_private->clip_region;
		if (region) {
			if (gc->clip_x_origin || gc->clip_y_origin) {
				tmpreg = gdk_region_copy(region);
				gdk_region_offset(tmpreg, gc->clip_x_origin, gc->clip_y_origin);
				gdk_region_intersect(clip_region, tmpreg);
				gdk_region_destroy(tmpreg);
			} else {
				gdk_region_intersect(clip_region, region);
			}
		}
		if (gc_private->values_mask & GDK_GC_SUBWINDOW && gc_private->values.subwindow_mode == GDK_INCLUDE_INFERIORS) {
			LEAVE();
			return clip_region;
		}
	}
	if (GDK_IS_WINDOW (private->wrapper) &&
	    GDK_WINDOW_IS_MAPPED (private->wrapper) &&
	    !GDK_WINDOW_OBJECT (private->wrapper)->input_only) {
		GList *cur;
		for (cur = GDK_WINDOW_OBJECT (private->wrapper)->children; cur; cur = cur->next) {
			GdkWindowObject *cur_private;
			GdkDrawableImplXYNTH *cur_impl;
			cur_private = GDK_WINDOW_OBJECT(cur->data);
			if (!GDK_WINDOW_IS_MAPPED(cur_private) || cur_private->input_only) {
				continue;
			}
			cur_impl = GDK_DRAWABLE_IMPL_XYNTH(cur_private->impl);
			rect.x      = cur_private->x;
			rect.y      = cur_private->y;
			rect.width  = cur_impl->width;
			rect.height = cur_impl->height;
			tmpreg = gdk_region_rectangle(&rect);
			gdk_region_subtract(clip_region, tmpreg);
			gdk_region_destroy(tmpreg);
		}
	}
	LEAVE();
	return clip_region;
}

void _gdk_xynth_draw_rectangle (GdkDrawable *drawable, GdkGC *gc, gint filled, gint x, gint y, gint width, gint height)
{
	gint  i;
	GdkRegion *clip;
	GdkGCXYNTH *gc_private = NULL;
	GdkDrawableImplXYNTH *impl;
	s_surface_t *surface = NULL;
	
	ENTER();
	if (GDK_IS_DRAWABLE(drawable) == 0) {
		LEAVE();
		return;
	}
	impl = GDK_DRAWABLE_IMPL_XYNTH(drawable);
	if (!impl->surface) {
		LEAVE();
		return;
	}
	if (gc) {
		gc_private = GDK_GC_XYNTH(gc);
	}
	if (gc_private) {
		if (!gdk_xynth_setup_for_drawing(impl, gc_private)) {
			LEAVE();
			return;
		}
	} else {
		GdkWindowObject *win = GDK_WINDOW_OBJECT(impl->wrapper);
		gdk_xynth_set_color(impl, &win->bg_color);
	}
	if (filled) {
		GdkRectangle rect = { x, y, width, height };
		clip = gdk_xynth_clip_region(drawable, gc, &rect);
		if (gc_private && gc_private->values_mask & GDK_GC_FILL) {
			if (gc_private->values.fill == GDK_STIPPLED  &&
	                    gc_private->values_mask & GDK_GC_STIPPLE &&
	                    gc_private->values.stipple) {
				surface = GDK_DRAWABLE_IMPL_XYNTH(GDK_PIXMAP_OBJECT(gc_private->values.stipple)->impl)->surface;
				if (surface) {
					ASSERT();
				}
			} else if (gc_private->values.fill == GDK_TILED  && gc_private->values_mask & GDK_GC_TILE && gc_private->values.tile) {
				surface = GDK_DRAWABLE_IMPL_XYNTH(GDK_PIXMAP_OBJECT(gc_private->values.tile)->impl)->surface;
			}
		}
		if (surface) {
			if (gc_private->values_mask & GDK_GC_TS_X_ORIGIN) {
				x = gc_private->values.ts_x_origin;
			}
			if (gc_private->values_mask & GDK_GC_TS_Y_ORIGIN) {
				y = gc_private->values.ts_y_origin;
			}
			for (i = 0; i < clip->numRects; i++) {
				ASSERT();
			}
			ASSERT();
		} else {
			for (i = 0; i < clip->numRects; i++) {
				DEBUG("fillbox %d %d, %d %d", clip->rects[i].x1,
						              clip->rects[i].y1,
						              clip->rects[i].x2 - clip->rects[i].x1,
						              clip->rects[i].y2 - clip->rects[i].y1);
				DEBUG("impl(%p)->surface(%p), color:%d", impl, impl->surface, s_rgbcolor(impl->surface, impl->color.red >> 8, impl->color.green >> 8, impl->color.blue >> 8));
				s_fillbox(impl->surface, clip->rects[i].x1, clip->rects[i].y1,
						         clip->rects[i].x2 - clip->rects[i].x1, clip->rects[i].y2 - clip->rects[i].y1,
						         s_rgbcolor(impl->surface, impl->color.red >> 8, impl->color.green >> 8, impl->color.blue >> 8));
				DEBUG("filled box");
			}
		}
		gdk_region_destroy(clip);
	} else {
		ASSERT();
	}
	LEAVE();
}

static void gdk_xynth_draw_arc (GdkDrawable *drawable, GdkGC *gc, gint filled, gint x, gint y, gint width, gint height, gint angle1, gint angle2)
{
	ENTER();
	ASSERT();
	LEAVE();
}

static void gdk_xynth_draw_polygon (GdkDrawable *drawable, GdkGC *gc, gint filled, GdkPoint *points, gint npoints)
{
	ENTER();
	ASSERT();
	LEAVE();
}

static void gdk_xynth_draw_text (GdkDrawable *drawable, GdkFont *font, GdkGC *gc, gint x, gint y, const gchar *text, gint text_length)
{
	ENTER();
	ASSERT();
	LEAVE();
}

static void gdk_xynth_draw_text_wc (GdkDrawable *drawable, GdkFont *font, GdkGC *gc, gint x, gint y, const GdkWChar *text, gint text_length)
{
	ENTER();
	ASSERT();
	LEAVE();
}

static void gdk_xynth_draw_drawable (GdkDrawable *drawable, GdkGC *gc, GdkDrawable *src, gint xsrc, gint ysrc, gint xdest, gint ydest, gint width, gint height)
{
	ENTER();
	ASSERT();
	LEAVE();
}

static void gdk_xynth_draw_points (GdkDrawable *drawable, GdkGC *gc, GdkPoint *points, gint npoints)
{
	ENTER();
	ASSERT();
	LEAVE();
}

static void gdk_xynth_draw_segments (GdkDrawable *drawable, GdkGC *gc, GdkSegment *segs, gint nsegs)
{
	gint i;
	s_rect_t rect;
	s_region_t *region;
	GdkRegion *clip;
	GdkDrawableImplXYNTH *impl;
	ENTER();
	if (nsegs < 1) {
		LEAVE();
		return;
	}
	impl = GDK_DRAWABLE_IMPL_XYNTH(drawable);
	if (!gdk_xynth_setup_for_drawing(impl, GDK_GC_XYNTH(gc))) {
		LEAVE();
		return;
	}
	clip = gdk_xynth_clip_region(drawable, gc, NULL);
	if (s_region_create(&region)) {
		ASSERT();
	}
	for (i = 0; i < clip->numRects; i++) {
		rect.x = clip->rects[i].x1;
		rect.y = clip->rects[i].y1,
		rect.w = clip->rects[i].x2 - clip->rects[i].x1;
		rect.h = clip->rects[i].y2 - clip->rects[i].y1;
		s_region_addrect(region, &rect);
	}
	if (s_surface_set_clip_region(impl->surface, region)) {
		ASSERT();
	}
	for (i = 0; i < nsegs; i++) {
		s_line(impl->surface, segs[i].x1, segs[i].y1, segs[i].x2, segs[i].y2, s_rgbcolor(impl->surface, impl->color.red >> 8, impl->color.green >> 8, impl->color.blue >> 8));
	}
	if (s_surface_set_clip_region(impl->surface, NULL)) {
		ASSERT();
	}
	if (s_region_destroy(region)) {
		ASSERT();
	}
	gdk_region_destroy (clip);

	/* everything below can be omitted if the drawing is buffered */
	if (impl->buffered) {
		LEAVE();
		return;
	}
	NIY();
	LEAVE();
}

static void gdk_xynth_draw_lines (GdkDrawable *drawable, GdkGC *gc, GdkPoint *points, gint npoints)
{
	ENTER();
	ASSERT();
	LEAVE();
}

static void gdk_xynth_draw_image (GdkDrawable *drawable, GdkGC *gc, GdkImage *image, gint xsrc, gint ysrc, gint xdest, gint ydest, gint width, gint height)
{
	ENTER();
	ASSERT();
	LEAVE();
}

static cairo_surface_t * gdk_xynth_ref_cairo_surface (GdkDrawable *drawable)
{
	GdkDrawableImplXYNTH *impl;
	ENTER();
	impl = GDK_DRAWABLE_IMPL_XYNTH(drawable);
	if (GDK_IS_DRAWABLE(drawable) == 0) {
		LEAVE();
		return NULL;
	}
	if (GDK_IS_DRAWABLE_IMPL_XYNTH(drawable) == 0) {
		LEAVE();
		return NULL;
	}
	if (impl->cairo_surface == NULL) {
		ASSERT();
	} else {
		cairo_surface_reference(impl->cairo_surface);
	}
	g_assert(impl->cairo_surface != NULL);
	LEAVE();
	return impl->cairo_surface;
}

static void gdk_xynth_set_colormap (GdkDrawable *drawable, GdkColormap *colormap)
{
	GdkDrawableImplXYNTH *impl;
	ENTER();
	impl = GDK_DRAWABLE_IMPL_XYNTH(drawable);
	if (impl->colormap == colormap) {
		LEAVE();
		return;
	}
	if (impl->colormap) {
		g_object_unref(impl->colormap);
	}
	impl->colormap = colormap;
	if (colormap) {
		g_object_ref(colormap);
	}
	LEAVE();
}

static GdkColormap * gdk_xynth_get_colormap (GdkDrawable *drawable)
{
	GdkColormap *retval;
	ENTER();
	retval = GDK_DRAWABLE_IMPL_XYNTH(drawable)->colormap;
	if (!retval) {
		retval = gdk_colormap_get_system();
		gdk_xynth_set_colormap(drawable, retval);
	}
	LEAVE();
	return retval;
}

static gint gdk_xynth_get_depth (GdkDrawable *drawable)
{
	int bpp;
	GdkDrawableImplXYNTH *impl;
	ENTER();
	impl = GDK_DRAWABLE_IMPL_XYNTH(drawable);
	bpp = impl->surface->bitsperpixel;
	LEAVE();
	return bpp;
}

static GdkVisual * gdk_xynth_get_visual (GdkDrawable *drawable)
{
	ENTER();
	ASSERT();
	LEAVE();
	return NULL;
}

static void gdk_xynth_get_size (GdkDrawable *drawable, gint *width, gint *height)
{
	GdkDrawableImplXYNTH *impl;
	ENTER();
	impl = GDK_DRAWABLE_IMPL_XYNTH(drawable);
	if (width) {
		*width = impl->width;
	}
	if (height) {
		*height = impl->height;
	}
	LEAVE();
}

static GdkScreen * gdk_xynth_get_screen (GdkDrawable *drawable)
{
	GdkScreen *screen;
	ENTER();
	screen = gdk_screen_get_default();
	LEAVE();
	return screen;
}

static void gdk_drawable_impl_xynth_finalize (GObject *object)
{
	GdkDrawableImplXYNTH *impl;
	ENTER();
	impl = GDK_DRAWABLE_IMPL_XYNTH(object);
	gdk_xynth_set_colormap(GDK_DRAWABLE (object), NULL);
	if (impl->cairo_surface) {
		cairo_surface_finish(impl->cairo_surface);
	}
	if (impl->surface) {
		DEBUG("uninit surface");
		ERROR("must uninit surface");
	}
	G_OBJECT_CLASS(parent_class)->finalize(object);
	LEAVE();
}

static void gdk_drawable_impl_xynth_class_init (GdkDrawableImplXYNTHClass *klass)
{
	GObjectClass     *object_class;
	GdkDrawableClass *drawable_class;

	ENTER();
	
	drawable_class = GDK_DRAWABLE_CLASS(klass);
	object_class   = G_OBJECT_CLASS(klass);
	
	parent_class = g_type_class_peek_parent(klass);
	
	object_class->finalize = gdk_drawable_impl_xynth_finalize;
	
	drawable_class->create_gc         = _gdk_xynth_gc_new;
	drawable_class->draw_rectangle    = _gdk_xynth_draw_rectangle;
	drawable_class->draw_arc          = gdk_xynth_draw_arc;
	drawable_class->draw_polygon      = gdk_xynth_draw_polygon;
	drawable_class->draw_text         = gdk_xynth_draw_text;
	drawable_class->draw_text_wc      = gdk_xynth_draw_text_wc;
	drawable_class->draw_drawable     = gdk_xynth_draw_drawable;
	drawable_class->draw_points       = gdk_xynth_draw_points;
	drawable_class->draw_segments     = gdk_xynth_draw_segments;
	drawable_class->draw_lines        = gdk_xynth_draw_lines;
	drawable_class->draw_image        = gdk_xynth_draw_image;
	drawable_class->ref_cairo_surface = gdk_xynth_ref_cairo_surface;
	drawable_class->set_colormap      = gdk_xynth_set_colormap;
	drawable_class->get_colormap      = gdk_xynth_get_colormap;
	drawable_class->get_depth         = gdk_xynth_get_depth;
	drawable_class->get_visual        = gdk_xynth_get_visual;
	drawable_class->get_size          = gdk_xynth_get_size;
	drawable_class->_copy_to_image    = _gdk_xynth_copy_to_image;
	drawable_class->get_screen        = gdk_xynth_get_screen;
	
	LEAVE();
}

GType gdk_drawable_impl_xynth_get_type (void)
{
	static GType object_type = 0;
	ENTER();
	if (!object_type) {
		static const GTypeInfo object_info = {
			sizeof(GdkDrawableImplXYNTHClass),
			(GBaseInitFunc) NULL,
			(GBaseFinalizeFunc) NULL,
			(GClassInitFunc) gdk_drawable_impl_xynth_class_init,
			NULL,           /* class_finalize */
			NULL,           /* class_data */
			sizeof (GdkDrawableImplXYNTH),
			0,              /* n_preallocs */
			(GInstanceInitFunc) NULL,
		};
		object_type = g_type_register_static(GDK_TYPE_DRAWABLE, "GdkDrawableImplXYNTH", &object_info, 0);
	}
	LEAVE();
	return object_type;
}
