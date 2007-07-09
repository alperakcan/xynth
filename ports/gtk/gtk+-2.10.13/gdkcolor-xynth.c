
#include "config.h"
#include "gdk.h"

#include "gdkinternals.h"

#include "gdkcolor.h"
#include "gdkalias.h"

#include "gdkxynth.h"

gint gdk_colormap_alloc_colors (GdkColormap *colormap, GdkColor *colors, gint ncolors, gboolean writeable, gboolean best_match, gboolean *success)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return -1;
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
	NIY();
	ASSERT();
	LEAVE();
	return NULL;
}

GType gdk_colormap_get_type (void)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

GdkColormap * gdk_colormap_new (GdkVisual *visual, gboolean private_cmap)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return NULL;
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
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return NULL;
}
