
#include "gdk-xynth.h"

static gpointer parent_class = NULL;

static void gdk_pixmap_impl_xynth_get_size (GdkDrawable *drawable, gint *width, gint *height)
{
	ENT();
	if (width) {
		*width = GDK_PIXMAP_IMPL_XYNTH(drawable)->width;
	}
	if (height) {
		*height = GDK_PIXMAP_IMPL_XYNTH(drawable)->height;
	}
	LEV();
}

static void gdk_pixmap_impl_xynth_finalize (GObject *object)
{
	GdkPixmapImplXynth *impl = GDK_PIXMAP_IMPL_XYNTH(object);
	ENT();
	G_OBJECT_CLASS(parent_class)->finalize(object);
	LEV();
}

static void gdk_pixmap_impl_xynth_class_init (GdkPixmapImplXynthClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	GdkDrawableClass *drawable_class = GDK_DRAWABLE_CLASS(klass);
	ENT();
	parent_class = g_type_class_peek_parent(klass);
	object_class->finalize = gdk_pixmap_impl_xynth_finalize;
	drawable_class->get_size = gdk_pixmap_impl_xynth_get_size;
	LEV();
}

static void gdk_pixmap_impl_xynth_init (GdkPixmapImplXynth *impl)
{
	impl->width = 1;
	impl->height = 1;
}

GType _gdk_pixmap_impl_get_type (void)
{
	static GType object_type = 0;
	ENT();
	if (!object_type) {
		static const GTypeInfo object_info = {
			sizeof(GdkPixmapImplXynthClass),
			(GBaseInitFunc) NULL,
			(GBaseFinalizeFunc) NULL,
			(GClassInitFunc) gdk_pixmap_impl_xynth_class_init,
			NULL,           /* class_finalize */
			NULL,           /* class_data */
			sizeof(GdkPixmapImplXynth),
			0,              /* n_preallocs */
			(GInstanceInitFunc) gdk_pixmap_impl_xynth_init,
		};
		object_type = g_type_register_static(GDK_TYPE_DRAWABLE_IMPL_XYNTH, "GdkPixmapImplXynth", &object_info, 0);
	}
	LEV();
	return object_type;
}

GdkPixmap * gdk_pixmap_new (GdkDrawable *drawable, gint width, gint height, gint depth)
{
	gint window_depth;
	GdkColormap *cmap;
	GdkPixmap *pixmap;
	GdkPixmapImplXynth *pix_impl;
	GdkDisplayXynth *display_xynth;
	GdkDrawableImplXynth *draw_impl;

	ENT();

	g_return_val_if_fail(drawable == NULL || GDK_IS_DRAWABLE(drawable), NULL);
	g_return_val_if_fail(drawable != NULL || depth != -1, NULL);
	g_return_val_if_fail(width > 0 && height > 0, NULL);

	if (!drawable) {
		drawable = gdk_screen_get_root_window(gdk_screen_get_default());
	}

	if (GDK_IS_WINDOW(drawable) && GDK_WINDOW_DESTROYED(drawable)) {
		return NULL;
	}

	window_depth = gdk_drawable_get_depth(GDK_DRAWABLE(drawable));
	if (depth == -1) {
		depth = window_depth;
	}

	pixmap = g_object_new(gdk_pixmap_get_type(), NULL);
	draw_impl = GDK_DRAWABLE_IMPL_XYNTH(GDK_PIXMAP_OBJECT(pixmap)->impl);
	pix_impl = GDK_PIXMAP_IMPL_XYNTH(GDK_PIXMAP_OBJECT(pixmap)->impl);
	draw_impl->wrapper = GDK_DRAWABLE(pixmap);
	
	if (draw_impl->object ||
	    depth != window_depth) {
		NIY();
	}

	display_xynth = GDK_DISPLAY_XYNTH(_gdk_display);
	w_object_init(display_xynth->window, &(draw_impl->object), NULL, NULL);
	DBG("width:%d, height: %d", width, height);
	w_object_move_silent(draw_impl->object, 0, 0, width, height);
	w_object_show(draw_impl->object);
	s_fillbox(draw_impl->object->surface, 0, 0, draw_impl->object->surface->width, draw_impl->object->surface->height, 0);

	pix_impl->is_foreign = FALSE;
	pix_impl->width = width;
	pix_impl->height = height;
	GDK_PIXMAP_OBJECT(pixmap)->depth = depth;
	
	if (depth == window_depth) {
		cmap = gdk_drawable_get_colormap(drawable);
		if (cmap) {
			gdk_drawable_set_colormap(pixmap, cmap);
		}
	}

	LEV();

	return pixmap;
}
