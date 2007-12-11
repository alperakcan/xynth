
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
