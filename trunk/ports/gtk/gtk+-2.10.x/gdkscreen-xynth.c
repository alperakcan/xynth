
#include "config.h"
#include "gdk.h"
#include "gdkscreen.h"

#include "gdkinternals.h"
#include "gdkalias.h"

#include "gdkprivate-xynth.h"
#include "gdkxynth.h"

static GdkColormap *default_colormap = NULL;

gchar * _gdk_windowing_substitute_screen_number (const gchar *display_name, int screen_number)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return NULL;
}

GdkColormap * gdk_screen_get_default_colormap (GdkScreen *screen)
{
	ENTER();
	LEAVE();
	return default_colormap;
}

GdkDisplay * gdk_screen_get_display (GdkScreen *screen)
{
	ENTER();
	LEAVE();
	return GDK_DISPLAY_OBJECT(_gdk_display);
}

gint gdk_screen_get_height (GdkScreen *screen)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

gint gdk_screen_get_height_mm (GdkScreen *screen)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_screen_get_monitor_geometry (GdkScreen *screen, gint num_monitor, GdkRectangle *dest)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

gint gdk_screen_get_n_monitors (GdkScreen *screen)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

gint gdk_screen_get_number (GdkScreen *screen)
{
	ENTER();
	LEAVE();
	g_return_val_if_fail(GDK_IS_SCREEN (screen), 0);
	return 0;
}

GdkWindow * gdk_screen_get_root_window (GdkScreen *screen)
{
	ENTER();
	LEAVE();
	return _gdk_parent_root;
}

gint gdk_screen_get_width (GdkScreen *screen)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

gint gdk_screen_get_width_mm (GdkScreen *screen)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

gboolean gdk_screen_is_composited (GdkScreen *screen)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

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

GdkColormap * gdk_screen_get_rgba_colormap (GdkScreen *screen)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return NULL;
}
