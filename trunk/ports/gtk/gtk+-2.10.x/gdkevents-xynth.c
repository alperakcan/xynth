
#include "config.h"
#include "gdk.h"

#include "gdkinternals.h"
#include "gdkkeysyms.h"
#include "gdkalias.h"
#include "gdkaliasdef.c"

#include "gdkprivate-xynth.h"
#include "gdkxynth.h"

guint32 gdk_xynth_get_time (void)
{
	GTimeVal tv;
	ENTER();
	g_get_current_time(&tv);
	LEAVE();
	return (guint32) tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

GdkWindow * gdk_xynth_child_at (GdkWindow *window, gint *winx, gint *winy)
{
	GList *list;
	GdkWindowObject *private;
	ENTER();
	if (GDK_IS_WINDOW(window) == 0) {
		LEAVE();
		return NULL;
	}
	private = GDK_WINDOW_OBJECT(window);
	for (list = private->children; list; list = list->next) {
		GdkWindowObject *win = list->data;
		if (GDK_WINDOW_IS_MAPPED(win) &&
		    *winx >= win->x  &&
		    *winx <  win->x + GDK_DRAWABLE_IMPL_XYNTH(win->impl)->width  &&
		    *winy >= win->y  &&
		    *winy <  win->y + GDK_DRAWABLE_IMPL_XYNTH(win->impl)->height) {
			*winx -= win->x;
			*winy -= win->y;
			LEAVE();
			return gdk_xynth_child_at(GDK_WINDOW(win), winx, winy );
		}
	}
	LEAVE();
	return window;
}

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
	LEAVE();
	return FALSE;
}

static void xynth_events_process_window_event (s_window_t *xynth_window, s_event_t *xynth_event)
{
#if 0
	GdkWindow *window = gdk_directfb_window_id_table_lookup(event->window_id);
	if (!window) {
		return;
	}
	gdk_event_translate(event, window);
#else
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
#endif
}

static void xynth_events_dispatch (void)
{
#if 0
	GdkEvent *event;
	GdkDisplay *display;
	ENTER();
	display = gdk_display_get_default();
	while ((event = _gdk_event_unqueue(display)) != NULL) {
		if (_gdk_event_func) {
			(*_gdk_event_func) (event, _gdk_event_data);
		}
		gdk_event_free (event);
	}
	LEAVE();
#endif
}

static gboolean xynth_events_io_func (GIOChannel *channel, GIOCondition condition, gpointer data)
{
#if 0
	gsize      i;
	gsize      read;
	GIOStatus  result;
	char buf[23];
	ENTER();
	result = g_io_channel_read_chars(channel, (gchar *) buf, sizeof (buf), &read, NULL);
	if (result == G_IO_STATUS_ERROR) {
		g_warning ("%s: GIOError occured", __FUNCTION__);
		LEAVE();
		return TRUE;
	}
	dfb_events_process_window_event (&event->window);
	dfb_events_dispatch ();
	LEAVE();
	return TRUE;
#else
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
#endif
}

void _gdk_events_init (void)
{
	GIOChannel *channel;
	GSource    *source;
	gint        fd;
	
	ENTER();

	pipe(_gdk_display->xynth_event_fd);
	
	fd = _gdk_display->xynth_event_fd[0];
	channel = g_io_channel_unix_new(fd);
	
	g_io_channel_set_encoding(channel, NULL, NULL);
	g_io_channel_set_buffered(channel, FALSE);
	
	source = g_io_create_watch(channel, G_IO_IN);
	
	g_source_set_priority(source, G_PRIORITY_DEFAULT);
	g_source_set_can_recurse(source, TRUE);
	g_source_set_callback(source, (GSourceFunc) xynth_events_io_func, NULL, NULL);
	
	g_source_attach(source, NULL);
	g_source_unref(source);

	LEAVE();
}
