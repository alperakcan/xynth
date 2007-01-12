
#include "gdk-xynth.h"

GdkColormap * gdk_screen_get_system_colormap (GdkScreen *screen)
{
	static GdkColormap *colormap = NULL;
	ENT();
	if (!colormap) {
		GdkVisual *visual = gdk_visual_get_system();
		colormap = g_object_new(gdk_colormap_get_type(), NULL);
		colormap->visual = visual;
		colormap->size = visual->colormap_size;
		colormap->colors = NULL;
		gdk_colormap_change (colormap, colormap->size);
	}
	LEV();
	return colormap;
}

GdkVisual * gdk_screen_get_system_visual (GdkScreen *screen)
{
	ENT();
	LEV();
	return _gdk_visual;
}

void gdk_screen_set_default_colormap (GdkScreen *screen, GdkColormap *colormap)
{
	GdkColormap *old_colormap;
	ENT();
	g_return_if_fail(GDK_IS_SCREEN(screen));
	g_return_if_fail(GDK_IS_COLORMAP(colormap));
	old_colormap = _gdk_colormap;
	_gdk_colormap = g_object_ref(colormap);
	if (old_colormap) {
		g_object_unref(old_colormap);
	}
	LEV();
}
