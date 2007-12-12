
#include "gdkxynth.h"

static gpointer parent_class = NULL;

static void gdk_pixmap_impl_xynth_init (GdkPixmapImplXYNTH *impl)
{
	GdkDrawableImplXYNTH *draw_impl;
	ENTER();
	draw_impl = GDK_DRAWABLE_IMPL_XYNTH(impl);
	draw_impl->width  = 1;
	draw_impl->height = 1;
	LEAVE();
}

static void gdk_pixmap_impl_xynth_finalize (GObject *object)
{
	GdkDrawableImplXYNTH *impl;
	ENTER();
	impl = GDK_DRAWABLE_IMPL_XYNTH(object);
	if (G_OBJECT_CLASS(parent_class)->finalize) {
		G_OBJECT_CLASS(parent_class)->finalize(object);
	}
	LEAVE();
}

static void gdk_pixmap_impl_xynth_class_init (GdkPixmapImplXYNTHClass *klass)
{
	GObjectClass *object_class;
	ENTER();
	object_class = G_OBJECT_CLASS(klass);
	parent_class = g_type_class_peek_parent(klass);
	object_class->finalize = gdk_pixmap_impl_xynth_finalize;
	LEAVE();
}

GType gdk_pixmap_impl_xynth_get_type (void)
{
	static GType object_type = 0;
	ENTER();
	if (!object_type) {
		static const GTypeInfo object_info = {
			sizeof(GdkPixmapImplXYNTHClass),
			(GBaseInitFunc) NULL,
			(GBaseFinalizeFunc) NULL,
			(GClassInitFunc) gdk_pixmap_impl_xynth_class_init,
			NULL,           /* class_finalize */
			NULL,           /* class_data */
			sizeof (GdkPixmapImplXYNTH),
			0,              /* n_preallocs */
			(GInstanceInitFunc) gdk_pixmap_impl_xynth_init,
		};
		object_type = g_type_register_static(GDK_TYPE_DRAWABLE_IMPL_XYNTH, "GdkPixmapImplXYNTH", &object_info, 0);
	}
	LEAVE();
	return object_type;
}

GType _gdk_pixmap_impl_get_type (void)
{
	GType type;
	ENTER();
	type = gdk_pixmap_impl_xynth_get_type();
	LEAVE();
	return type;
}

GdkPixmap * gdk_pixmap_new (GdkDrawable *drawable, gint width, gint height, gint depth)
{
	GdkPixmap *pixmap;
	s_surface_t *surface;
	GdkDrawableImplXYNTH *draw_impl;
	
	g_return_val_if_fail(drawable == NULL || GDK_IS_DRAWABLE (drawable), NULL);
	g_return_val_if_fail(drawable != NULL || depth != -1, NULL);
	g_return_val_if_fail(width > 0 && height > 0, NULL);
	
	ENTER();
	
	if (!drawable) {
		drawable = _gdk_parent_root;
	}
	if (GDK_IS_WINDOW(drawable) && GDK_WINDOW_DESTROYED(drawable)) {
		LEAVE();
		return NULL;
	}
	GDK_NOTE(MISC, g_print("gdk_pixmap_new: %dx%dx%d\n", width, height, depth));

	if (depth == -1) {
		draw_impl = GDK_DRAWABLE_IMPL_XYNTH(GDK_WINDOW_OBJECT(drawable)->impl);
		if (draw_impl == NULL) {
			LEAVE();
			return NULL;
		}
		g_return_val_if_fail(draw_impl != NULL, NULL);
		depth = draw_impl->surface->bitsperpixel;
	} else {
		switch (depth) {
			case  8:
			case 15:
			case 16:
			case 24:
			case 32:
				break;
			default:
				g_message ("unimplemented %s for depth %d", G_GNUC_FUNCTION, depth);
				LEAVE();
				return NULL;
		}
	}

	if (!(surface = gdk_display_xynth_create_surface(_gdk_display, depth, width, height))) { 
		g_assert(surface != NULL);
		LEAVE();
		return NULL;
	}

	pixmap = g_object_new(gdk_pixmap_get_type(), NULL);
	draw_impl = GDK_DRAWABLE_IMPL_XYNTH(GDK_PIXMAP_OBJECT(pixmap)->impl);
	draw_impl->surface = surface;
	draw_impl->width = surface->width;
	draw_impl->height = surface->height;
	draw_impl->abs_x = 0;
	draw_impl->abs_y = 0;

	GDK_PIXMAP_OBJECT (pixmap)->depth = depth;

	LEAVE();
	return pixmap;
}
