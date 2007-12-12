
#include "gdkxynth.h"

static gpointer parent_class = NULL;

static void gdk_xynth_gc_set_dashes (GdkGC *gc, gint dash_offset, gint8 dash_list[], gint n)
{
	ENTER();
	LEAVE();
}

static void gdk_xynth_gc_get_values (GdkGC *gc, GdkGCValues *values)
{
	ENTER();
	*values = GDK_GC_XYNTH(gc)->values;
	LEAVE();
}

static void gdk_xynth_gc_set_values (GdkGC *gc, GdkGCValues *values, GdkGCValuesMask  values_mask)
{
	GdkGCXYNTH *private;
	ENTER();
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
		GdkFont *oldf = private->values.font;
		private->values.font = gdk_font_ref (values->font);
		private->values_mask |= GDK_GC_FONT;
		if (oldf) {
			gdk_font_unref (oldf);
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
		GdkPixmap *oldpm = private->values.tile;
		if (values->tile) {
			g_assert (GDK_PIXMAP_OBJECT (values->tile)->depth > 1);
		}
		private->values.tile = values->tile ? g_object_ref (values->tile) : NULL;
		private->values_mask |= GDK_GC_TILE;
		if (oldpm) {
			g_object_unref (oldpm);
		}
	}
	if (values_mask & GDK_GC_STIPPLE) {
		GdkPixmap *oldpm = private->values.stipple;
		if (values->stipple) {
			g_assert (GDK_PIXMAP_OBJECT (values->stipple)->depth == 1);
		}
		private->values.stipple = (values->stipple ? g_object_ref (values->stipple) : NULL);
		private->values_mask |= GDK_GC_STIPPLE;
		if (oldpm) {
			g_object_unref (oldpm);
		}
	}
	if (values_mask & GDK_GC_CLIP_MASK) {
		GdkPixmap *oldpm = private->values.clip_mask;
		private->values.clip_mask = (values->clip_mask ? g_object_ref (values->clip_mask) : NULL);
		private->values_mask |= GDK_GC_CLIP_MASK;
		if (oldpm) {
			g_object_unref (oldpm);
		}
		if (private->clip_region) {
			gdk_region_destroy (private->clip_region);
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
		private->values.clip_x_origin = GDK_GC (gc)->clip_x_origin = values->clip_x_origin;
		private->values_mask |= GDK_GC_CLIP_X_ORIGIN;
	}
	if (values_mask & GDK_GC_CLIP_Y_ORIGIN) {
		private->values.clip_y_origin = GDK_GC (gc)->clip_y_origin = values->clip_y_origin;
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
	LEAVE();
}

static void gdk_gc_xynth_finalize (GObject *object)
{
	GdkGC         *gc;
	GdkGCXYNTH *private;
	ENTER();
	gc      = GDK_GC(object);
	private = GDK_GC_XYNTH(gc);
	if (private->clip_region) {
		gdk_region_destroy(private->clip_region);
	}
	if (private->values.clip_mask) {
		g_object_unref(private->values.clip_mask);
	}
	if (private->values.stipple) {
		g_object_unref(private->values.stipple);
	}
	if (private->values.tile) {
		g_object_unref(private->values.tile);
	}
	if (G_OBJECT_CLASS(parent_class)->finalize) {
		G_OBJECT_CLASS(parent_class)->finalize(object);
	}
	LEAVE();
}

static void gdk_gc_xynth_class_init (GdkGCXYNTHClass *klass)
{
	GdkGCClass   *gc_class;
	GObjectClass *object_class;
	ENTER();
	gc_class     = GDK_GC_CLASS(klass);
	object_class = G_OBJECT_CLASS(klass);
	parent_class = g_type_class_peek_parent(klass);
	object_class->finalize = gdk_gc_xynth_finalize;
	gc_class->get_values = gdk_xynth_gc_get_values;
	gc_class->set_values = gdk_xynth_gc_set_values;
	gc_class->set_dashes = gdk_xynth_gc_set_dashes;
	LEAVE();
}

GType gdk_gc_xynth_get_type (void)
{
	static GType object_type = 0;
	ENTER();
	if (!object_type) {
		static const GTypeInfo object_info = {
			sizeof(GdkGCXYNTHClass),
			(GBaseInitFunc) NULL,
			(GBaseFinalizeFunc) NULL,
			(GClassInitFunc) gdk_gc_xynth_class_init,
			NULL,           /* class_finalize */
			NULL,           /* class_data */
			sizeof(GdkGCXYNTH),
			0,              /* n_preallocs */
			(GInstanceInitFunc) NULL,
		};
		object_type = g_type_register_static(GDK_TYPE_GC, "GdkGCXYNTH", &object_info, 0);
	}
	LEAVE();
	return object_type;
}

GdkGC * _gdk_xynth_gc_new (GdkDrawable *drawable, GdkGCValues *values, GdkGCValuesMask values_mask)
{
	GdkGC *gc;
	GdkGCXYNTH *private;
	
	ENTER();
	
	if (GDK_IS_DRAWABLE_IMPL_XYNTH(drawable) == 0) {
		LEAVE();
		return NULL;
	}
	
	gc = GDK_GC(g_object_new(gdk_gc_xynth_get_type(), NULL));
	_gdk_gc_init(gc, drawable, values, values_mask);
	
	private = GDK_GC_XYNTH(gc);
	private->values.cap_style = GDK_CAP_BUTT;
	
	gdk_xynth_gc_set_values(gc, values, values_mask);
	
	LEAVE();
	return gc;
}

static void gc_unset_clip_mask (GdkGC *gc)
{
	GdkGCXYNTH *data;
	ENTER();
	data = GDK_GC_XYNTH(gc);
	if (data->values.clip_mask) {
		g_object_unref(data->values.clip_mask);
		data->values.clip_mask = NULL;
		data->values_mask &= ~GDK_GC_CLIP_MASK;
	}
	LEAVE();
}

void _gdk_windowing_gc_set_clip_region (GdkGC *gc, GdkRegion *region)
{
	GdkGCXYNTH *data;
	ENTER();
	if (gc == NULL) {
		LEAVE();
		return;
	}
	data = GDK_GC_XYNTH(gc);
	if (region == data->clip_region) {
		LEAVE();
		return;
	}
	if (data->clip_region) {
		gdk_region_destroy(data->clip_region);
		data->clip_region = NULL;
	}
	if (region) {
		data->clip_region = gdk_region_copy(region);
	}
	gc->clip_x_origin = 0;
	gc->clip_y_origin = 0;
	data->values.clip_x_origin = 0;
	data->values.clip_y_origin = 0;
	gc_unset_clip_mask(gc);
	LEAVE();
}
