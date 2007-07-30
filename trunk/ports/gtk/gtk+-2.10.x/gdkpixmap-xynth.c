
#include <config.h>
#include "gdk.h"

#include "gdkinternals.h"

#include "gdkpixmap.h"
#include "gdkalias.h"

#include "gdkprivate-xynth.h"
#include "gdkxynth.h"

static gpointer parent_class = NULL;

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

static void gdk_pixmap_impl_xynth_init (GdkPixmapImplXYNTH *impl)
{
	GdkDrawableImplXYNTH *draw_impl;
	ENTER();
	draw_impl = GDK_DRAWABLE_IMPL_XYNTH(impl);
	draw_impl->width  = 1;
	draw_impl->height = 1;
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

GType _gdk_pixmap_impl_get_type (void)
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
			sizeof(GdkPixmapImplXYNTH),
			0,              /* n_preallocs */
			(GInstanceInitFunc) gdk_pixmap_impl_xynth_init,
		};
		object_type = g_type_register_static(GDK_TYPE_DRAWABLE_IMPL_XYNTH, "GdkPixmapImplXYNTH", &object_info, 0);
	}
	LEAVE();
	return object_type;
}

GdkPixmap * gdk_bitmap_create_from_data (GdkDrawable *drawable, const gchar *data, gint width, gint height)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return NULL;
}

GdkPixmap * gdk_pixmap_new (GdkDrawable *drawable, gint width, gint height, gint depth)
{
	GdkPixmap *pixmap;
	GdkDrawableImplXYNTH *draw_impl;
	g_return_val_if_fail(drawable == NULL || GDK_IS_DRAWABLE(drawable), NULL);
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
	DEBUG("Pixmap width:%d, height:%d, depth:%d", width, height, depth);
	if (depth == -1) {
		draw_impl = GDK_DRAWABLE_IMPL_XYNTH(GDK_WINDOW_OBJECT(drawable)->impl);
		if (draw_impl == NULL) {
			LEAVE();
			return NULL;
		}
		depth = draw_impl->xynth_surface->bitsperpixel;
	}
	{
		s_surface_t *xynth_surface;
		if (s_surface_create(&xynth_surface, width, height, depth)) {
			LEAVE();
			return NULL;
		}
		pixmap = g_object_new(gdk_pixmap_get_type(), NULL);
		draw_impl = GDK_DRAWABLE_IMPL_XYNTH(GDK_PIXMAP_OBJECT(pixmap)->impl);
		draw_impl->xynth_surface = xynth_surface;
		draw_impl->width = width;
		draw_impl->height = height;
		draw_impl->abs_x = 0;
		draw_impl->abs_y = 0;
		GDK_PIXMAP_OBJECT(pixmap)->depth = depth;
	}
	LEAVE();
	return pixmap;
}
