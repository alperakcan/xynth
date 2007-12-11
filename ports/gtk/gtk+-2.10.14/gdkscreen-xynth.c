
#include "gdkxynth.h"

static GdkColormap *default_colormap = NULL;

void gdk_screen_set_default_colormap (GdkScreen *screen, GdkColormap *colormap)
{
	GdkColormap *old_colormap;
	ENTER();
	g_return_if_fail(GDK_IS_SCREEN(screen));
	g_return_if_fail(GDK_IS_COLORMAP(colormap));
	old_colormap = default_colormap;
	default_colormap = g_object_ref(colormap);
	if (old_colormap) {
		g_object_unref(old_colormap);
	}
	LEAVE();
}

gint gdk_screen_get_number (GdkScreen *screen)
{
	ENTER();
	if (GDK_IS_SCREEN(screen) == 0) {
		LEAVE();
		return 0;
	}
	LEAVE();
	return 0;
}

GdkDisplay * gdk_screen_get_display (GdkScreen *screen)
{
	ENTER();
	LEAVE();
	return GDK_DISPLAY_OBJECT(_gdk_display);
}

GdkColormap * gdk_screen_get_default_colormap (GdkScreen *screen)
{
	ENTER();
	LEAVE();
	return default_colormap;
}

GdkWindow * gdk_screen_get_root_window (GdkScreen *screen)
{
	ENTER();
	LEAVE();
	return _gdk_parent_root;
}
