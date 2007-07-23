
#include <config.h>

#include <glib.h>
#include "gdk.h"
#include "gdkscreen.h"
#include "gdkdisplaymanager.h"
#include "gdkintl.h"
#include "gdkalias.h"

#include "gdkprivate-xynth.h"
#include "gdkxynth.h"

extern void _gdk_visual_init (void);
extern void _gdk_windowing_window_init (void);
extern void _gdk_windowing_image_init (void);
extern void _gdk_input_init (void);
extern void _gdk_dnd_init (void);
extern void _gdk_events_init (void);

const GOptionEntry _gdk_windowing_args[] = {
	{ NULL }
};

void _gdk_windowing_set_default_display (GdkDisplay *display)
{
	ENTER();
	_gdk_display = GDK_DISPLAY_XYNTH(display);
	LEAVE();
}

void gdk_display_beep (GdkDisplay *display)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_display_sync (GdkDisplay *display)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_display_flush (GdkDisplay *display)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_notify_startup_complete (void)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

GdkScreen * gdk_display_get_default_screen (GdkDisplay *display)
{
	ENTER();
	LEAVE();
	return _gdk_screen;
}

int gdk_display_get_n_screens (GdkDisplay *display)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

GdkScreen * gdk_display_get_screen (GdkDisplay *display, gint screen_num)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return NULL;
}

void gdk_display_keyboard_ungrab (GdkDisplay *display, guint32 time)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

GList * gdk_display_list_devices (GdkDisplay *dpy)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return NULL;
}

GdkDisplay * gdk_display_open (const gchar *display_name)
{
	s_window_t *xynth_window;
	ENTER();
	if (_gdk_display) {
		return GDK_DISPLAY_OBJECT(_gdk_display);
	}

	if (s_window_init(&xynth_window)) {
		return NULL;
	}

	_gdk_display = g_object_new(GDK_TYPE_DISPLAY_XYNTH,NULL);
	_gdk_screen = g_object_new(GDK_TYPE_SCREEN, NULL);

	_gdk_display->xynth_window = xynth_window;

	_gdk_visual_init();
	gdk_screen_set_default_colormap(_gdk_screen, gdk_screen_get_system_colormap(_gdk_screen));
	_gdk_windowing_window_init();

	_gdk_input_init();
	_gdk_dnd_init();
	
	_gdk_events_init();
	
	g_signal_emit_by_name(gdk_display_manager_get(), "display_opened", _gdk_display);
	LEAVE();
	return GDK_DISPLAY_OBJECT(_gdk_display);
}

GType gdk_display_xynth_get_type (void)
{
	static GType object_type = 0;
	ENTER();
	if (!object_type) {
		static const GTypeInfo object_info = {
			sizeof(GdkDisplayXYNTHClass),
			(GBaseInitFunc) NULL,
			(GBaseFinalizeFunc) NULL,
			(GClassInitFunc) NULL,
			NULL,                 /* class_finalize */
			NULL,                 /* class_data */
			sizeof(GdkDisplayXYNTH),
			0,                    /* n_preallocs */
			(GInstanceInitFunc) NULL,
		};
		object_type = g_type_register_static(GDK_TYPE_DISPLAY, "GdkDisplayXYNTH", &object_info, 0);
	}
	LEAVE();
	return object_type;
}

gint gdk_display_pointer_is_grabbed (GdkDisplay *display)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_display_pointer_ungrab (GdkDisplay *display, guint32 time)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

gboolean gdk_display_request_selection_notification (GdkDisplay *display, GdkAtom selection)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_display_store_clipboard (GdkDisplay *display, GdkWindow  *clipboard_window, guint32 time_, GdkAtom *targets, gint n_targets)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

gboolean gdk_display_supports_clipboard_persistence (GdkDisplay *display)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

gboolean gdk_display_supports_selection_notification (GdkDisplay *display)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

G_CONST_RETURN gchar * gdk_display_get_name (GdkDisplay *display)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return NULL;
}
