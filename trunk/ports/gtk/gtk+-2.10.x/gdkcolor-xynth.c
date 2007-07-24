
#include "config.h"
#include "gdk.h"

#include "gdkinternals.h"

#include "gdkcolor.h"
#include "gdkalias.h"

#include "gdkprivate-xynth.h"
#include "gdkxynth.h"

static GObjectClass *parent_class = NULL;

gint gdk_colormap_alloc_colors (GdkColormap *colormap, GdkColor *colors, gint ncolors, gboolean writeable, gboolean best_match, gboolean *success)
{
	gint i;
	GdkVisual *visual;
	g_return_val_if_fail(GDK_IS_COLORMAP (colormap), 0);
	g_return_val_if_fail(colors != NULL, 0);
	g_return_val_if_fail(success != NULL, 0);
	ENTER();
	switch (colormap->visual->type) {
		case GDK_VISUAL_TRUE_COLOR:
			visual = colormap->visual;
			for (i = 0; i < ncolors; i++) {
				colors[i].pixel = (((colors[i].red   >> (16 - visual->red_prec))   << visual->red_shift) +
						   ((colors[i].green >> (16 - visual->green_prec)) << visual->green_shift) +
						   ((colors[i].blue  >> (16 - visual->blue_prec))  << visual->blue_shift));
				success[i] = TRUE;
			}
			break;
		case GDK_VISUAL_STATIC_COLOR:
			for (i = 0; i < ncolors; i++) {
				colors[i].pixel = (((colors[i].red   & 0xE000) >> 8)  |
						   ((colors[i].green & 0xE000) >> 11) |
						   ((colors[i].blue  & 0xC000) >> 14));
				success[i] = TRUE;
			}
			break;
		default:
			for (i = 0; i < ncolors; i++) {
				success[i] = FALSE;
			}
			NIY();
			break;
	}
	LEAVE();
	return 0;
}

void gdk_colormap_change (GdkColormap *colormap, gint ncolors)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_colormap_free_colors (GdkColormap *colormap, GdkColor *colors, gint ncolors)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

GdkScreen * gdk_colormap_get_screen (GdkColormap *cmap)
{
	ENTER();
	LEAVE();
	return _gdk_screen;
}

static void gdk_colormap_init (GdkColormap *colormap)
{
	ENTER();
	colormap->size           = 0;
	colormap->colors         = NULL;
	colormap->windowing_data = NULL;
	LEAVE();
}

static void gdk_colormap_finalize (GObject *object)
{
	GdkColormap *colormap;
	ENTER();
	colormap = GDK_COLORMAP(object);
	g_free(colormap->colors);
	colormap->colors = NULL;
	G_OBJECT_CLASS(parent_class)->finalize(object);
	LEAVE();
}

static void gdk_colormap_class_init (GdkColormapClass *klass)
{
	GObjectClass *object_class;
	ENTER();
	object_class = G_OBJECT_CLASS(klass);
	parent_class = g_type_class_peek_parent(klass);
	object_class->finalize = gdk_colormap_finalize;
	LEAVE();
}

GType gdk_colormap_get_type (void)
{
	ENTER();
	static GType object_type = 0;
	if (!object_type) {
		static const GTypeInfo object_info = {
			sizeof(GdkColormapClass),
			(GBaseInitFunc) NULL,
			(GBaseFinalizeFunc) NULL,
			(GClassInitFunc) gdk_colormap_class_init,
			NULL,           /* class_finalize */
			NULL,           /* class_data */
			sizeof (GdkColormap),
			0,              /* n_preallocs */
			(GInstanceInitFunc) gdk_colormap_init,
		};
		object_type = g_type_register_static(G_TYPE_OBJECT, "GdkColormap", &object_info, 0);
	}
	LEAVE();
	return object_type;
}

GdkColormap * gdk_colormap_new (GdkVisual *visual, gboolean private_cmap)
{
	GdkColormap *colormap;
	ENTER();
	g_return_val_if_fail(visual != NULL, NULL);
	g_return_val_if_fail(visual->type == GDK_VISUAL_TRUE_COLOR, NULL);
	colormap = g_object_new(gdk_colormap_get_type(), NULL);
	colormap->visual = visual;
	colormap->size   = visual->colormap_size;
	LEAVE();
	return colormap;
}

void gdk_colormap_query_color (GdkColormap *colormap, gulong pixel, GdkColor *result)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

GdkColormap * gdk_screen_get_system_colormap (GdkScreen *screen)
{
	static GdkColormap *colormap = NULL;
	ENTER();
	if (!colormap) {
		GdkVisual *visual = gdk_visual_get_system();
		colormap = gdk_colormap_new(visual, FALSE);
	}
	LEAVE();
	return colormap;
}
