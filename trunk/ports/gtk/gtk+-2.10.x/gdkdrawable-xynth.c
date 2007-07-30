
#include <config.h>
#include "gdk.h"
#include "gdkwindow.h"

#include "gdkregion-generic.h"

#include "gdkinternals.h"
#include "gdkalias.h"

#include "gdkxynth.h"
#include "gdkprivate-xynth.h"

static gpointer parent_class = NULL;

static GdkRegion * gdk_xynth_clip_region (GdkDrawable *drawable, GdkGC *gc, GdkRectangle *draw_rect)
{
	GdkDrawableImplXYNTH *private;
	GdkRegion            *clip_region;
	GdkRegion            *tmpreg;
	GdkRectangle         rect;
	g_return_val_if_fail(GDK_IS_DRAWABLE(drawable), NULL);
	g_return_val_if_fail(GDK_IS_DRAWABLE_IMPL_XYNTH(drawable), NULL);
	ENTER();
	private = GDK_DRAWABLE_IMPL_XYNTH(drawable);
	if (!draw_rect) {
		rect.x      = 0;
		rect.y      = 0;
		rect.width  = private->width;
		rect.height = private->height;
		draw_rect = &rect;
	}
	DEBUG("buffered: %d", private->buffered);
	clip_region = gdk_region_rectangle(draw_rect);
	if (private->buffered && private->paint_region) {
		gdk_region_intersect(clip_region, private->paint_region);
	}
	if (gc) {
		GdkGCXYNTH *gc_private = GDK_GC_XYNTH(gc);
		GdkRegion  *region     = gc_private->clip_region;
		if (region) {
			if (gc->clip_x_origin || gc->clip_y_origin) {
				tmpreg = gdk_region_copy(region);
				gdk_region_offset(tmpreg, gc->clip_x_origin, gc->clip_y_origin);
				gdk_region_intersect(clip_region, tmpreg);
				gdk_region_destroy (tmpreg);
			} else {
				gdk_region_intersect(clip_region, region);
			}
		}
		if (gc_private->values_mask & GDK_GC_SUBWINDOW &&
		    gc_private->values.subwindow_mode == GDK_INCLUDE_INFERIORS) {
			LEAVE();
			return clip_region;
		}
	}
	if (GDK_IS_WINDOW(private->wrapper) &&
	    GDK_WINDOW_IS_MAPPED(private->wrapper) &&
	    !GDK_WINDOW_OBJECT(private->wrapper)->input_only) {
		GList *cur;
		for (cur = GDK_WINDOW_OBJECT(private->wrapper)->children; cur; cur = cur->next) {
			GdkWindowObject      *cur_private;
			GdkDrawableImplXYNTH *cur_impl;
			cur_private = GDK_WINDOW_OBJECT(cur->data);
			if (!GDK_WINDOW_IS_MAPPED (cur_private) || cur_private->input_only) {
				continue;
			}
			cur_impl = GDK_DRAWABLE_IMPL_XYNTH(cur_private->impl);
			rect.x      = cur_private->x;
			rect.y      = cur_private->y;
			rect.width  = cur_impl->width;
			rect.height = cur_impl->height;
			tmpreg = gdk_region_rectangle(&rect);
			gdk_region_subtract(clip_region, tmpreg);
			gdk_region_destroy (tmpreg);
		}
	}
	LEAVE();
	return clip_region;
}

static inline void gdk_xynth_set_color (GdkDrawableImplXYNTH *impl, GdkColor *color, guchar alpha)
{
	ENTER();
	DEBUG("color: %d, %d, %d, %d", color->red >> 8, color->green >> 8, color->blue >> 8, alpha);
	LEAVE();
}

static gboolean gdk_xynth_setup_for_drawing (GdkDrawableImplXYNTH *impl, GdkGCXYNTH *gc_private)
{
	GdkColor color = {0, 0, 0, 0};
	guchar alpha = 0xFF;
	ENTER();
	if (!impl->xynth_surface) {
		LEAVE();
		return FALSE;
	}
	if (gc_private && gc_private->values_mask & GDK_GC_FOREGROUND) {
		color = gc_private->values.foreground;
	}
	if (gc_private && gc_private->values_mask & GDK_GC_FUNCTION) {
		switch (gc_private->values.function) {
			case GDK_COPY:
				DEBUG("copy");
				break;
			case GDK_INVERT:
				DEBUG("invert");
				color.red = color.green = color.blue = 0xFFFF;
				alpha = 0x0;
				break;
			case GDK_XOR:
				DEBUG("xor");
				alpha = 0x0;
				break;
			case GDK_CLEAR:
				DEBUG("clear");
				color.red = color.green = color.blue = 0x0;
				break;
			case GDK_NOOP:
				DEBUG("noop");
				return FALSE;
			case GDK_SET:
				DEBUG("set");
				color.red = color.green = color.blue = 0xFFFF;
				break;
			default:
				NIY();
				ASSERT();
				break;
		}
		NIY();
		ASSERT();
	}
	gdk_xynth_set_color(impl, &color, alpha);
	LEAVE();
	return TRUE;
}

void _gdk_xynth_draw_rectangle (GdkDrawable *drawable, GdkGC *gc, gint filled, gint x, gint y, gint width, gint height)
{
	GdkDrawableImplXYNTH *impl;
	GdkRegion            *clip;
	GdkGCXYNTH           *gc_private = NULL;
	s_surface_t *xynth_surface = NULL;
	gint  i;
	g_return_if_fail(GDK_IS_DRAWABLE(drawable));
	ENTER();
	impl = GDK_DRAWABLE_IMPL_XYNTH(drawable);
	if (!impl->xynth_surface) {
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
		gdk_xynth_set_color(impl, &win->bg_color, 0xFF);
	}
	if (filled) {
		GdkRectangle rect = {x, y, width, height};
		clip = gdk_xynth_clip_region(drawable, gc, &rect);
		if (gc_private && gc_private->values_mask & GDK_GC_FILL) {
#if 0
			if (gc_private->values.fill == GDK_STIPPLED  &&
			    gc_private->values_mask & GDK_GC_STIPPLE &&
			    gc_private->values.stipple) {
				xynth_surface = GDK_DRAWABLE_IMPL_XYNTH(GDK_PIXMAP_OBJECT(gc_private->values.stipple)->impl)->xynth_surface;
				if (surface) {
					impl->surface->SetBlittingFlags(impl->surface, (DSBLIT_BLEND_ALPHACHANNEL | DSBLIT_COLORIZE));
				}
			} else if (gc_private->values.fill == GDK_TILED  &&
				   gc_private->values_mask & GDK_GC_TILE &&
				   gc_private->values.tile) {
				surface = GDK_DRAWABLE_IMPL_XYNTH(GDK_PIXMAP_OBJECT(gc_private->values.tile)->impl)->surface;
			}
#else
		ASSERT();
#endif
		}
		if (xynth_surface) {
#if 0
			if (gc_private->values_mask & GDK_GC_TS_X_ORIGIN) {
				x = gc_private->values.ts_x_origin;
			}
			if (gc_private->values_mask & GDK_GC_TS_Y_ORIGIN) {
				y = gc_private->values.ts_y_origin;
			}
			for (i = 0; i < clip->numRects; i++) {
				Region reg = {
					clip->rects[i].x1,
					clip->rects[i].y1,
					clip->rects[i].x2,
					clip->rects[i].y2
				};
				impl->surface->SetClip(impl->surface, &reg);
				impl->surface->TileBlit(impl->surface, surface, NULL, x, y);
			}
			impl->surface->SetBlittingFlags(impl->surface, DSBLIT_NOFX);
			impl->surface->SetClip(impl->surface, NULL);
#else
		ASSERT();
#endif
		} else {
			for (i = 0; i < clip->numRects; i++) {
				DEBUG("rect: %d %d, %d %d",
				      clip->rects[i].x1,
				      clip->rects[i].y1,
				      clip->rects[i].x2 - clip->rects[i].x1,
				      clip->rects[i].y2 - clip->rects[i].y1);
			}
		}
		gdk_region_destroy (clip);
	} else {
#if 0
		Region region = {
			x,
			y,
			x + width,
			y + height
		};
		impl->surface->SetClip(impl->surface, &region);
		/* Xyhnth does not draw rectangles the X way. Using Xynth,
		 * a filled Rectangle has the same size as a drawn one, while
		 * X draws the rectangle one pixel taller and wider.
		 */
		impl->surface->DrawRectangle(impl->surface, x, y, width, height);
		impl->surface->SetClip (impl->surface, NULL);
#else
		ASSERT();
#endif
	}
	LEAVE();
}

static void gdk_xynth_draw_arc (GdkDrawable *drawable, GdkGC *gc, gint filled, gint x, gint y, gint width, gint height, gint angle1, gint angle2)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

static void gdk_xynth_draw_polygon (GdkDrawable *drawable, GdkGC *gc, gint filled, GdkPoint *points, gint npoints)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

static void gdk_xynth_draw_text (GdkDrawable *drawable, GdkFont *font, GdkGC *gc, gint x, gint y, const gchar *text, gint text_length)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

static void gdk_xynth_draw_text_wc (GdkDrawable *drawable, GdkFont *font, GdkGC *gc, gint x, gint y, const GdkWChar *text, gint text_length)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

static void gdk_xynth_draw_drawable (GdkDrawable *drawable, GdkGC *gc, GdkDrawable *src, gint xsrc, gint ysrc, gint xdest, gint ydest, gint width, gint height)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

static void gdk_xynth_draw_points (GdkDrawable *drawable, GdkGC *gc, GdkPoint *points, gint npoints)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

static void gdk_xynth_draw_segments (GdkDrawable *drawable, GdkGC *gc, GdkSegment *segs, gint nsegs)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

static void gdk_xynth_draw_lines (GdkDrawable *drawable, GdkGC *gc, GdkPoint *points, gint npoints)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

static void gdk_xynth_draw_image (GdkDrawable *drawable, GdkGC *gc, GdkImage *image, gint xsrc, gint ysrc, gint xdest, gint ydest, gint width, gint height)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

static cairo_surface_t * gdk_xynth_ref_cairo_surface (GdkDrawable *drawable)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

static gint gdk_xynth_get_depth (GdkDrawable *drawable)
{
	gint depth;
	GdkDrawableImplXYNTH *impl;	
	ENTER();
	impl = GDK_DRAWABLE_IMPL_XYNTH(drawable);
	depth = impl->xynth_surface->bitsperpixel;
	LEAVE();
	return depth;
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

static GdkVisual * gdk_xynth_get_visual (GdkDrawable *drawable)
{
	GdkVisual *visual;
	ENTER();
	visual = gdk_visual_get_system();
	LEAVE();
	return visual;
}

static GdkScreen * gdk_xynth_get_screen (GdkDrawable *drawable)
{
	GdkScreen *screen;
	ENTER();
	screen = gdk_screen_get_default();
	LEAVE();
	return screen;
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

static void gdk_drawable_impl_xynth_finalize (GObject *object)
{
	GdkDrawableImplXYNTH *impl;
	ENTER();
	impl = GDK_DRAWABLE_IMPL_XYNTH(object);
	gdk_xynth_set_colormap(GDK_DRAWABLE(object), NULL);
	if (impl->xynth_surface) {
		s_surface_destroy(impl->xynth_surface);
	}
	G_OBJECT_CLASS(parent_class)->finalize(object);
	LEAVE();
}

static void gdk_drawable_impl_xynth_class_init (GdkDrawableImplXYNTHClass *klass)
{
	GObjectClass *object_class   = G_OBJECT_CLASS(klass);
	GdkDrawableClass *drawable_class = GDK_DRAWABLE_CLASS(klass);
	ENTER();
	parent_class = g_type_class_peek_parent(klass);
	object_class->finalize = gdk_drawable_impl_xynth_finalize;
	drawable_class->create_gc      = _gdk_xynth_gc_new;
	drawable_class->draw_rectangle = _gdk_xynth_draw_rectangle;
	drawable_class->draw_arc       = gdk_xynth_draw_arc;
	drawable_class->draw_polygon   = gdk_xynth_draw_polygon;
	drawable_class->draw_text      = gdk_xynth_draw_text;
	drawable_class->draw_text_wc   = gdk_xynth_draw_text_wc;
	drawable_class->draw_drawable  = gdk_xynth_draw_drawable;
	drawable_class->draw_points    = gdk_xynth_draw_points;
	drawable_class->draw_segments  = gdk_xynth_draw_segments;
	drawable_class->draw_lines     = gdk_xynth_draw_lines;
	drawable_class->draw_image     = gdk_xynth_draw_image;
	drawable_class->ref_cairo_surface = gdk_xynth_ref_cairo_surface;
	drawable_class->set_colormap   = gdk_xynth_set_colormap;
	drawable_class->get_colormap   = gdk_xynth_get_colormap;
	drawable_class->get_depth      = gdk_xynth_get_depth;
	drawable_class->get_visual     = gdk_xynth_get_visual;
	drawable_class->get_size       = gdk_xynth_get_size;
	drawable_class->_copy_to_image = _gdk_xynth_copy_to_image;
	drawable_class->get_screen     = gdk_xynth_get_screen;
	LEAVE();
}

GType gdk_drawable_impl_xynth_get_type (void)
{
	static GType object_type = 0;
	ENTER();
	if (!object_type) {
		static const GTypeInfo object_info = {
			sizeof (GdkDrawableImplXYNTHClass),
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
