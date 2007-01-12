
#include "gdk-xynth.h"

static gpointer parent_class = NULL;

static GdkRegion * gdk_window_impl_xynth_get_visible_region (GdkDrawable *drawable)
{
	GdkWindowImplXynth *impl = GDK_WINDOW_IMPL_XYNTH(drawable);
	GdkRectangle result_rect;
	ENT();
	result_rect.x = 0;
	result_rect.y = 0;
	result_rect.width = impl->width;
	result_rect.height = impl->height;
	gdk_rectangle_intersect(&result_rect, &impl->clip, &result_rect);
	LEV();
	return gdk_region_rectangle (&result_rect);
}

static void gdk_window_impl_xynth_init (GdkWindowImplXynth *impl)
{
	ENT();
	impl->width = -1;
	impl->height = -1;
	LEV();
}

static void gdk_window_impl_xynth_finalize (GObject *object)
{
	GdkDrawableImplXynth *draw_impl;
	GdkWindowImplXynth *window_impl;
	ENT();
	g_return_if_fail(GDK_IS_WINDOW_IMPL_XYNTH(object));
	draw_impl = GDK_DRAWABLE_IMPL_XYNTH(object);
	window_impl = GDK_WINDOW_IMPL_XYNTH(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	LEV();
}

static void gdk_window_impl_xynth_class_init (GdkWindowImplXynthClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	GdkDrawableClass *drawable_class = GDK_DRAWABLE_CLASS(klass);
	ENT();
	parent_class = g_type_class_peek_parent(klass);
	object_class->finalize = gdk_window_impl_xynth_finalize;
	/* Visible and clip regions are the same */
	drawable_class->get_clip_region = gdk_window_impl_xynth_get_visible_region;
	drawable_class->get_visible_region = gdk_window_impl_xynth_get_visible_region;
	LEV();
}

GType gdk_window_impl_xynth_get_type (void)
{
	static GType object_type = 0;
	ENT();
	if (!object_type) {
		static const GTypeInfo object_info = {
			sizeof(GdkWindowImplXynthClass),
			(GBaseInitFunc) NULL,
			(GBaseFinalizeFunc) NULL,
			(GClassInitFunc) gdk_window_impl_xynth_class_init,
			NULL,           /* class_finalize */
			NULL,           /* class_data */
			sizeof(GdkWindowImplXynth),
			0,              /* n_preallocs */
			(GInstanceInitFunc) gdk_window_impl_xynth_init,
		};
		object_type = g_type_register_static(GDK_TYPE_DRAWABLE_IMPL_XYNTH, "GdkWindowImplXynth", &object_info, 0);
	}
	LEV();
	return object_type;
}

GType _gdk_window_impl_get_type (void)
{
	ENT();
	LEV();
	return gdk_window_impl_xynth_get_type ();
}

void _gdk_windowing_window_init (GdkDisplay *display)
{
	GdkWindowObject *private;
	GdkDisplayXynth *display_xynth;
	GdkWindowImplXynth *window_impl;
	GdkDrawableImplXynth *draw_impl;
	ENT();
	g_assert(_gdk_parent_root == NULL);
	_gdk_parent_root = g_object_new(GDK_TYPE_WINDOW, NULL);
	display_xynth = GDK_DISPLAY_XYNTH(display);
	private = (GdkWindowObject *)_gdk_parent_root;
	draw_impl = GDK_DRAWABLE_IMPL_XYNTH(private->impl);
	window_impl = GDK_WINDOW_IMPL_XYNTH(private->impl);
	private->window_type = GDK_WINDOW_ROOT;
	private->depth = gdk_visual_get_system()->depth;
	window_impl->width = display_xynth->window->window->surface->linear_buf_width;
	window_impl->height = display_xynth->window->window->surface->linear_buf_height; 
	LEV();
}
