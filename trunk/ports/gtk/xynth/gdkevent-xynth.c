
#include "gdk-xynth.h"
#include "gdkinternals.h"

static gboolean xynth_events_dispatch (GSource *source, GSourceFunc callback, gpointer user_data)
{
	GdkEvent *event;
	ENT();
	GDK_THREADS_ENTER();
	while ((event = _gdk_event_unqueue (gdk_display_get_default ()))) {
		if (_gdk_event_func)
			(*_gdk_event_func) (event, _gdk_event_data);
		gdk_event_free (event);
	}
	GDK_THREADS_LEAVE ();
	LEV();
	return TRUE;
}

static gboolean xynth_events_check (GSource *source)
{
	gboolean retval;
	ENT();
	GDK_THREADS_ENTER();
	retval = (_gdk_event_queue_find_first(gdk_display_get_default()) != NULL);
	GDK_THREADS_LEAVE();
	LEV();
	return retval;
}

static gboolean xynth_events_prepare (GSource *source, gint *timeout)
{
	ENT();
	*timeout = -1;
	LEV();
	return xynth_events_check(source);
}

static GSourceFuncs xynth_events_funcs = {
	xynth_events_prepare,
	xynth_events_check,
	xynth_events_dispatch,
	NULL
};

void _gdk_windowing_event_init (GdkDisplay *display)
{
	GSource *source;
	ENT();
	source = g_source_new(&xynth_events_funcs, sizeof(GSource));
	g_source_set_priority(source, GDK_PRIORITY_EVENTS);
	g_source_set_can_recurse (source, TRUE);
	g_source_attach (source, NULL);
	LEV();
}
