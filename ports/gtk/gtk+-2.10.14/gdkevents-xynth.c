
#include "gdkxynth.h"

static int event_fd[2];

static gboolean xynth_events_io_func (GIOChannel *channel, GIOCondition condition, gpointer data)
{
	gint buf[1];
	gsize read;
	GIOStatus  result;
	ENTER();
	result = g_io_channel_read_chars(channel, (gchar *) buf, sizeof(buf), &read, NULL);
	if (result == G_IO_STATUS_ERROR) {
		g_warning("%s: GIOError occured", __FUNCTION__);
		LEAVE();
		return TRUE;
	}
	DEBUG("process event");
	LEAVE();
	return TRUE;
}

void _gdk_events_init (void)
{
	GIOChannel *channel;
	GSource    *source;

	ENTER();

	if (pipe(event_fd) != 0) {
		ASSERT();
	}
	
	channel = g_io_channel_unix_new(event_fd[0]);
	g_io_channel_set_encoding(channel, NULL, NULL);
	g_io_channel_set_buffered(channel, FALSE);
	source = g_io_create_watch(channel, G_IO_IN);
	g_source_set_priority(source, G_PRIORITY_DEFAULT);
	g_source_set_can_recurse(source, FALSE);
	g_source_set_callback(source, (GSourceFunc) xynth_events_io_func, NULL, NULL);
	g_source_attach(source, NULL);
	g_source_unref(source);
	
	LEAVE();
}

gboolean gdk_screen_get_setting (GdkScreen *screen, const gchar *name, GValue *value)
{
	ENTER();
	LEAVE();
	return FALSE;
}

guint32 gdk_xynth_get_time (void)
{
	guint32 re;
	GTimeVal tv;
	ENTER();
	g_get_current_time(&tv);
	re = (guint32) tv.tv_sec * 1000 + tv.tv_usec / 1000;
	LEAVE();
	return re;
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
		if (GDK_WINDOW_IS_MAPPED (win) &&
		    *winx >= win->x  &&
                    *winx <  win->x + GDK_DRAWABLE_IMPL_XYNTH(win->impl)->width  &&
                    *winy >= win->y  &&
                    *winy <  win->y + GDK_DRAWABLE_IMPL_XYNTH(win->impl)->height) {
			*winx -= win->x;
			*winy -= win->y;
			LEAVE();
			return gdk_xynth_child_at(GDK_WINDOW(win), winx, winy);
		}
	}
	LEAVE();
	return window;
}

void gdk_xynth_event_windows_add (GdkWindow *window)
{
	GdkWindowImplXYNTH *impl;
	ENTER();
	if (GDK_IS_WINDOW(window) == 0) {
		LEAVE();
		return;
	}
	impl = GDK_WINDOW_IMPL_XYNTH(GDK_WINDOW_OBJECT(window)->impl);
	if (!impl->xynth) {
		LEAVE();
		return;
	}
	DEBUG("Event buffer set for xynth window");
}
