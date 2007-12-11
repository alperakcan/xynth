
#include "gdkxynth.h"

static gpointer parent_class = NULL;

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
