
#include "gdk-xynth.h"

static gpointer parent_class = NULL;

static void gdk_colormap_finalize (GObject *object)
{
	GdkColormap *colormap = GDK_COLORMAP(object);
	ENT();
	g_free(colormap->colors);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	LEV();
}

static void gdk_colormap_init (GdkColormap *colormap)
{
	ENT();
	colormap->size = 0;
	colormap->colors = NULL;
	LEV();
}

static void gdk_colormap_class_init (GdkColormapClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	ENT();
	parent_class = g_type_class_peek_parent(klass);
	object_class->finalize = gdk_colormap_finalize;
	LEV();
}

GType gdk_colormap_get_type (void)
{
	static GType object_type = 0;
	ENT();
	if (!object_type) {
		static const GTypeInfo object_info = {
			sizeof(GdkColormapClass),
			(GBaseInitFunc) NULL,
			(GBaseFinalizeFunc) NULL,
			(GClassInitFunc) gdk_colormap_class_init,
			NULL,           /* class_finalize */
			NULL,           /* class_data */
			sizeof(GdkColormap),
			0,              /* n_preallocs */
			(GInstanceInitFunc) gdk_colormap_init,
		};
		object_type = g_type_register_static(G_TYPE_OBJECT, "GdkColormap", &object_info, 0);
	}
	LEV();
	return object_type;
}

void gdk_colormap_change (GdkColormap *colormap, gint ncolors)
{
	ENT();
	LEV();
}
