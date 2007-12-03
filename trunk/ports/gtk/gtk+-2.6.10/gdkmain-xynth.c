
#include "gdk-xynth.h"

void _gdk_windowing_init (void)
{
	ENT();
	_gdk_selection_property = gdk_atom_intern("GDK_SELECTION", FALSE);
	LEV();
}

void gdk_notify_startup_complete (void)
{
	ENT();
	LEV();
}

guint32 gdk_xynth_get_time(void)
{
	GTimeVal tv;
	ENT();
	g_get_current_time(&tv);
	LEV();
	return (guint32) tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

gboolean gdk_pointer_grab_info_libgtk_only (GdkDisplay *display, GdkWindow **grab_window, gboolean *owner_events)
{
	GdkDisplayXynth *display_xynth;
	ENT();
	LEV();
	return FALSE;
#if 0
	g_return_val_if_fail(GDK_IS_DISPLAY(display), False);
	display_xynth = GDK_DISPLAY_XYNTH(display);
	if (display_x11->pointer_xgrab_window) {
		if (grab_window) {
			*grab_window = (GdkWindow *)display_x11->pointer_xgrab_window;
		}
		if (owner_events) {
			*owner_events = display_x11->pointer_xgrab_owner_events;
		}
		return TRUE;
	} else {
		return FALSE;
	}
#endif
}
