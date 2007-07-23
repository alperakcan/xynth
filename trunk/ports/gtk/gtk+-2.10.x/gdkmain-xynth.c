
#include <config.h>
#include "gdk.h"

#include "gdkdisplay.h"

#include "gdkinternals.h"

#include "gdkintl.h"
#include "gdkalias.h"

#include "gdkxynth.h"

void _gdk_windowing_display_set_sm_client_id (GdkDisplay *display, const gchar *sm_client_id)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void _gdk_windowing_init (void)
{
	ENTER();
	/* nothing to do */
	LEAVE();
}

gint gdk_error_trap_pop (void)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_error_trap_push (void)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

GdkGrabStatus gdk_keyboard_grab (GdkWindow *window, gint owner_events, guint32 time) 
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

gboolean gdk_keyboard_grab_info_libgtk_only (GdkDisplay *display, GdkWindow **grab_window, gboolean *owner_events)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

GdkGrabStatus gdk_pointer_grab (GdkWindow *window, gint owner_events, GdkEventMask event_mask, GdkWindow *confine_to, GdkCursor *cursor, guint32 time)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

gboolean gdk_pointer_grab_info_libgtk_only (GdkDisplay *display, GdkWindow **grab_window, gboolean *owner_events)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}
