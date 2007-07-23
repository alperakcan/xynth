
#include "config.h"
#include "gdk.h"

#include "gdkinternals.h"
#include "gdkkeysyms.h"
#include "gdkalias.h"
#include "gdkaliasdef.c"

#include "gdkxynth.h"

void _gdk_events_queue (GdkDisplay *display)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

GdkEvent * gdk_event_get_graphics_expose (GdkWindow *window)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return NULL;
}

gboolean gdk_event_send_client_message_for_display (GdkDisplay *display, GdkEvent *event, guint32 xid)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_flush (void)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_screen_broadcast_client_message (GdkScreen *screen, GdkEvent *sev)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

gboolean gdk_screen_get_setting (GdkScreen *screen, const gchar *name, GValue *value)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}
