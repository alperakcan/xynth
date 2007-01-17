
#include "gdk-xynth.h"

static gpointer parent_class = NULL;

static void gdk_xynth_gc_set_dashes (GdkGC *gc, gint dash_offset, gint8 dash_list[], gint n)
{
	GdkGCXynth *private;
	ENT();
	private = GDK_GC_XYNTH(gc);
	private->dash_offset = dash_offset;
	private->dash_list_len = n;
	if (n) {
		private->dash_list = g_realloc(private->dash_list, n);
		memcpy(private->dash_list, dash_list, n);
	} else {
		g_free(private->dash_list);
		private->dash_list = NULL;
	}
	LEV();
}

static void gdk_xynth_gc_get_values (GdkGC *gc, GdkGCValues *values)
{
	ENT();
	*values = GDK_GC_XYNTH(gc)->values;
	LEV();
}

static void gdk_xynth_gc_set_values (GdkGC *gc, GdkGCValues *values, GdkGCValuesMask values_mask)
{
	GdkPixmap *oldpm;
	GdkFont *oldf;
	GdkGCXynth *private;
	ENT();
	private = GDK_GC_XYNTH(gc);
	if (values_mask & GDK_GC_FOREGROUND) {
		private->values.foreground = values->foreground;
		private->values_mask |= GDK_GC_FOREGROUND;
	}
	if (values_mask & GDK_GC_BACKGROUND) {
		private->values.background = values->background;
		private->values_mask |= GDK_GC_BACKGROUND;
	}
	if (values_mask & GDK_GC_FONT) {
		oldf = private->values.font;
		private->values.font = gdk_font_ref (values->font);
		private->values_mask |= GDK_GC_FONT;
		if (oldf) {
			gdk_font_unref(oldf);
		}
	}
	if (values_mask & GDK_GC_FUNCTION) {
		private->values.function = values->function;
		private->values_mask |= GDK_GC_FUNCTION;
	}
	if (values_mask & GDK_GC_FILL) {
		private->values.fill = values->fill;
		private->values_mask |= GDK_GC_FILL;
	}
	if (values_mask & GDK_GC_TILE) {
		oldpm = private->values.tile;
		private->values.tile = values->tile ? g_object_ref(values->tile) : NULL;
		private->values_mask |= GDK_GC_TILE;
		if (oldpm) {
			g_object_unref (oldpm);
		}
	}
	if (values_mask & GDK_GC_STIPPLE) {
		oldpm = private->values.stipple;
		private->values.stipple = values->stipple ? g_object_ref(values->stipple) : NULL;
		private->values_mask |= GDK_GC_STIPPLE;
		if (oldpm) {
			g_object_unref (oldpm);
		}
	}
	if (values_mask & GDK_GC_CLIP_MASK) {
		oldpm = private->values.clip_mask;
		private->values.clip_mask = values->clip_mask ? g_object_ref(values->clip_mask) : NULL;
		private->values_mask |= GDK_GC_CLIP_MASK;
		if (oldpm) {
			g_object_unref(oldpm);
		}
		if (private->clip_region) {
			gdk_region_destroy(private->clip_region);
			private->clip_region = NULL;
		}
	}
	if (values_mask & GDK_GC_SUBWINDOW) {
		private->values.subwindow_mode = values->subwindow_mode;
		private->values_mask |= GDK_GC_SUBWINDOW;
	}
	if (values_mask & GDK_GC_TS_X_ORIGIN) {
		private->values.ts_x_origin = values->ts_x_origin;
		private->values_mask |= GDK_GC_TS_X_ORIGIN;
	}
	if (values_mask & GDK_GC_TS_Y_ORIGIN) {
		private->values.ts_y_origin = values->ts_y_origin;
		private->values_mask |= GDK_GC_TS_Y_ORIGIN;
	}
	if (values_mask & GDK_GC_CLIP_X_ORIGIN) {
		private->values.clip_x_origin = gc->clip_x_origin = values->clip_x_origin;
		private->values_mask |= GDK_GC_CLIP_X_ORIGIN;
	}
	if (values_mask & GDK_GC_CLIP_Y_ORIGIN) {
		private->values.clip_y_origin = gc->clip_y_origin = values->clip_y_origin;
		private->values_mask |= GDK_GC_CLIP_Y_ORIGIN;
	}
	if (values_mask & GDK_GC_EXPOSURES) {
		private->values.graphics_exposures = values->graphics_exposures;
		private->values_mask |= GDK_GC_EXPOSURES;
	}
	if (values_mask & GDK_GC_LINE_WIDTH) {
		private->values.line_width = values->line_width;
		private->values_mask |= GDK_GC_LINE_WIDTH;
	}
	if (values_mask & GDK_GC_LINE_STYLE) {
		private->values.line_style = values->line_style;
		private->values_mask |= GDK_GC_LINE_STYLE;
	}
	if (values_mask & GDK_GC_CAP_STYLE) {
		private->values.cap_style = values->cap_style;
		private->values_mask |= GDK_GC_CAP_STYLE;
	}
	if (values_mask & GDK_GC_JOIN_STYLE) {
		private->values.join_style = values->join_style;
		private->values_mask |= GDK_GC_JOIN_STYLE;
	}
	LEV();
}

static void gdk_xynth_gc_finalize (GObject *obj)
{
	GdkGC *gc = obj;
	GdkGCXynth *private;
	ENT();
	private = GDK_GC_XYNTH(gc);
	if (private->clip_region)
		gdk_region_destroy(private->clip_region);
	if (private->values.clip_mask)
		g_object_unref(private->values.clip_mask);
	if (private->values.stipple)
		g_object_unref(private->values.stipple);
	if (private->values.tile)
		g_object_unref(private->values.tile);
	g_free(private->dash_list);
	G_OBJECT_CLASS(parent_class)->finalize(obj);
	LEV();
}

static void gdk_gc_xynth_class_init (GdkGCXynthClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	GdkGCClass *gc_class = GDK_GC_CLASS (klass);
	ENT();
	parent_class = g_type_class_peek_parent (klass);
	object_class->finalize = gdk_xynth_gc_finalize;
	gc_class->get_values = gdk_xynth_gc_get_values;
	gc_class->set_values = gdk_xynth_gc_set_values;
	gc_class->set_dashes = gdk_xynth_gc_set_dashes;
	LEV();
}

GType gdk_gc_xynth_get_type (void)
{
	static GType object_type = 0;
	ENT();
	if (!object_type) {
		static const GTypeInfo object_info = {
			sizeof(GdkGCXynthClass),
			(GBaseInitFunc) NULL,
			(GBaseFinalizeFunc) NULL,
			(GClassInitFunc) gdk_gc_xynth_class_init,
			NULL,           /* class_finalize */
			NULL,           /* class_data */
			sizeof(GdkGCXynth),
			0,              /* n_preallocs */
			(GInstanceInitFunc) NULL,
		};
		object_type = g_type_register_static(GDK_TYPE_GC, "GdkGCXynth", &object_info, 0);
	}
	LEV();
	return object_type;
}

GdkGC * gdk_xynth_gc_new (GdkDrawable *drawable, GdkGCValues *values, GdkGCValuesMask values_mask)
{
	GdkGC *gc;
	GdkGCXynth *private;
	ENT();
	g_return_val_if_fail (GDK_IS_DRAWABLE_IMPL_XYNTH(drawable), NULL);
	gc = g_object_new(gdk_gc_xynth_get_type(), NULL);
	private = GDK_GC_XYNTH(gc);
	private->depth = gdk_drawable_get_depth(drawable);
	private->values.foreground.pixel = 255;
	private->values.foreground.red =
	private->values.foreground.green =
	private->values.foreground.blue = 0;
	private->values.cap_style = GDK_CAP_BUTT;
	gdk_xynth_gc_set_values(gc, values, values_mask);
	LEV();
	return gc;
}

void gdk_gc_set_clip_region (GdkGC *gc, GdkRegion *region)
{
	GdkGCXynth *xgc;
	gboolean had_region = FALSE;
	ENT();
	g_return_if_fail(GDK_IS_GC(gc));
	xgc = GDK_GC_XYNTH(gc);
	if (xgc->clip_region) {
		had_region = TRUE;
		gdk_region_destroy(xgc->clip_region);
	}
	if (region) {
		xgc->clip_region = gdk_region_copy(region);
	} else {
		xgc->clip_region = NULL;
	}
	gc->clip_x_origin = 0;
	gc->clip_y_origin = 0;
	LEV();
}

void gdk_gc_set_clip_rectangle (GdkGC *gc, GdkRectangle *rectangle)
{
	GdkGCXynth *xgc;
	gboolean had_region = FALSE;
	ENT();
	g_return_if_fail(GDK_IS_GC(gc));
	xgc = GDK_GC_XYNTH(gc);
	if (xgc->clip_region) {
		had_region = TRUE;
		gdk_region_destroy(xgc->clip_region);
	}
	if (rectangle) {
		xgc->clip_region = gdk_region_rectangle(rectangle);
	} else {
		xgc->clip_region = NULL;
	}
	gc->clip_x_origin = 0;
	gc->clip_y_origin = 0;
	LEV();
}

