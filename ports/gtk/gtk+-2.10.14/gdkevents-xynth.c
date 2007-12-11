
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
