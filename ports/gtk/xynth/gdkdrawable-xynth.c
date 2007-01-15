
#include "gdk-xynth.h"

static gpointer parent_class = NULL;

static void gdk_xynth_draw_rectangle (GdkDrawable *drawable, GdkGC *gc, gboolean filled, gint x, gint y, gint width, gint height)
{
	NIY();
}

static void gdk_xynth_draw_arc (GdkDrawable *drawable, GdkGC *gc, gboolean filled, gint x, gint y, gint width, gint height, gint angle1, gint angle2)
{
	NIY();
}

static void gdk_xynth_draw_polygon (GdkDrawable *drawable, GdkGC *gc, gboolean filled, GdkPoint *points, gint npoints)
{
	NIY();
}

static void gdk_xynth_draw_text (GdkDrawable *drawable, GdkFont *font, GdkGC *gc, gint x, gint y, const gchar *text, gint text_length)
{
	NIY();
}

static void gdk_xynth_draw_text_wc (GdkDrawable *drawable, GdkFont *font, GdkGC *gc, gint x, gint y, const GdkWChar *text, gint text_length)
{
	NIY();
}

static void gdk_xynth_draw_glyphs (GdkDrawable *drawable, GdkGC *gc, PangoFont *font, gint x, gint y, PangoGlyphString *glyphs)
{
	NIY();
}

static void gdk_xynth_draw_drawable (GdkDrawable *drawable, GdkGC *gc, GdkPixmap *src, gint xsrc, gint ysrc, gint xdest, gint ydest, gint width, gint height)
{
	NIY();
}

static void gdk_xynth_draw_image (GdkDrawable *drawable, GdkGC *gc, GdkImage *image, gint xsrc, gint ysrc, gint xdest, gint ydest, gint width, gint height)
{
	NIY();
}

static void gdk_xynth_draw_points (GdkDrawable *drawable, GdkGC *gc, GdkPoint *points, gint npoints)
{
	NIY();
}

static void gdk_xynth_draw_segments (GdkDrawable *drawable, GdkGC *gc, GdkSegment *segs, gint nsegs)
{
	NIY();
}

static void gdk_xynth_draw_lines (GdkDrawable *drawable, GdkGC *gc, GdkPoint *points, gint npoints)
{
	NIY();
}

static GdkColormap * gdk_xynth_get_colormap (GdkDrawable *drawable)
{
	GdkColormap *retval;
	GdkDrawableImplXynth *draw_impl;
	ENT();
	draw_impl = GDK_DRAWABLE_IMPL_XYNTH(drawable);
	retval = draw_impl->colormap;
	if (!retval) {
		retval = gdk_colormap_get_system();
	}
	LEV();
	return retval;
}

static void gdk_xynth_set_colormap (GdkDrawable *drawable, GdkColormap *colormap)
{
	NIY();
}

static gint gdk_xynth_get_depth (GdkDrawable *drawable)
{
	NIY();
}

static GdkScreen * gdk_xynth_get_screen (GdkDrawable *drawable)
{
	NIY();
}

static GdkVisual * gdk_xynth_get_visual (GdkDrawable *drawable)
{
	NIY();
}

static void gdk_xynth_get_size (GdkDrawable *d, gint *width, gint *height)
{
	NIY();
}

static GdkImage * gdk_xynth_copy_to_image (GdkDrawable *drawable, GdkImage *image, gint src_x, gint src_y, gint dest_x, gint dest_y, gint width, gint height)
{
	NIY();
}

static GdkGC * gdk_xynth_gc_new (GdkDrawable *drawable, GdkGCValues *values, GdkGCValuesMask values_mask)
{
	NIY();
}

static void gdk_xynth_drawable_finalize (GObject *object)
{
	ENT();
	gdk_drawable_set_colormap(GDK_DRAWABLE(object), NULL);
	G_OBJECT_CLASS (parent_class)->finalize (object);
	LEV();
}

static void gdk_drawable_impl_xynth_class_init (GdkDrawableImplXynthClass *klass)
{
	GdkDrawableClass *drawable_class = GDK_DRAWABLE_CLASS(klass);
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	ENT();
	parent_class = g_type_class_peek_parent(klass);
	object_class->finalize = gdk_xynth_drawable_finalize;
	drawable_class->create_gc = gdk_xynth_gc_new;
	drawable_class->draw_rectangle = gdk_xynth_draw_rectangle;
	drawable_class->draw_arc = gdk_xynth_draw_arc;
	drawable_class->draw_polygon = gdk_xynth_draw_polygon;
	drawable_class->draw_text = gdk_xynth_draw_text;
	drawable_class->draw_text_wc = gdk_xynth_draw_text_wc;
	drawable_class->draw_drawable = gdk_xynth_draw_drawable;
	drawable_class->draw_points = gdk_xynth_draw_points;
	drawable_class->draw_segments = gdk_xynth_draw_segments;
	drawable_class->draw_lines = gdk_xynth_draw_lines;
	drawable_class->draw_glyphs = gdk_xynth_draw_glyphs;
	drawable_class->draw_image = gdk_xynth_draw_image;
	drawable_class->set_colormap = gdk_xynth_set_colormap;
	drawable_class->get_colormap = gdk_xynth_get_colormap;
	drawable_class->get_size = gdk_xynth_get_size;
	drawable_class->get_depth = gdk_xynth_get_depth;
	drawable_class->get_screen = gdk_xynth_get_screen;
	drawable_class->get_visual = gdk_xynth_get_visual;
	drawable_class->_copy_to_image = gdk_xynth_copy_to_image;
	LEV();
}

GType _gdk_drawable_impl_xynth_get_type (void)
{
	static GType object_type = 0;
	ENT();
	if (!object_type) {
		static const GTypeInfo object_info = {
			sizeof(GdkDrawableImplXynthClass),
			(GBaseInitFunc) NULL,
			(GBaseFinalizeFunc) NULL,
			(GClassInitFunc) gdk_drawable_impl_xynth_class_init,
			NULL,           /* class_finalize */
			NULL,           /* class_data */
			sizeof(GdkDrawableImplXynth),
			0,              /* n_preallocs */
			(GInstanceInitFunc) NULL,
		};
		object_type = g_type_register_static(GDK_TYPE_DRAWABLE, "GdkDrawableImplXynth", &object_info, 0);
	}
	LEV();
	return object_type;
}
