
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

gint gdk_colormap_alloc_colors (GdkColormap *colormap, GdkColor *colors, gint ncolors, gboolean writeable, gboolean best_match, gboolean *success)
{
	GdkVisual *visual;
	gint i;
	gint nremaining = 0;
	ENT();
	g_return_val_if_fail(colormap != NULL, FALSE);
	g_return_val_if_fail(colors != NULL, FALSE);
	for (i = 0; i < ncolors; i++) {
		success[i] = FALSE;
	}
	visual = colormap->visual;
	switch (visual->type) {
		case GDK_VISUAL_PSEUDO_COLOR:
		case GDK_VISUAL_GRAYSCALE:
		case GDK_VISUAL_STATIC_GRAY:
		case GDK_VISUAL_STATIC_COLOR:
		case GDK_VISUAL_DIRECT_COLOR:
		default:
			NIY();
			break;
		case GDK_VISUAL_TRUE_COLOR:
			for (i = 0; i < ncolors; i++) {
				colors[i].pixel = (((colors[i].red >> (16 - visual->red_prec)) << visual->red_shift) +
				                   ((colors[i].green >> (16 - visual->green_prec)) << visual->green_shift) +
				                   ((colors[i].blue >> (16 - visual->blue_prec)) << visual->blue_shift));
				success[i] = TRUE;
			}
			break;
	}
	LEV();
	return nremaining;
}

GdkScreen * gdk_colormap_get_screen (GdkColormap *cmap)
{
	ENT();
	g_return_val_if_fail(cmap != NULL, NULL);
	LEV();
	return gdk_screen_get_default();
}
