
#include "gdk-xynth.h"
#include "gdkinternals.h"

static gboolean xynth_events_dispatch (GSource *source, GSourceFunc callback, gpointer user_data)
{
	GdkEvent *event;
	ENT();
	GDK_THREADS_ENTER();
	while ((event = _gdk_event_unqueue (gdk_display_get_default()))) {
		if (_gdk_event_func)
			(*_gdk_event_func) (event, _gdk_event_data);
		gdk_event_free (event);
	}
	GDK_THREADS_LEAVE();
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

GdkEvent * gdk_event_make (GdkWindow *window, GdkEventType type, gboolean append_to_queue)
{
	GdkEvent *event = gdk_event_new(type);
	guint32 the_time;
	ENT();
	the_time = gdk_xynth_get_time();
	event->any.window = g_object_ref(window);
	event->any.send_event = FALSE;
	switch (type) {
		case GDK_MOTION_NOTIFY:
			event->motion.time = the_time;
			event->motion.axes = NULL;
			break;
		case GDK_BUTTON_PRESS:
		case GDK_2BUTTON_PRESS:
		case GDK_3BUTTON_PRESS:
		case GDK_BUTTON_RELEASE:
			event->button.time = the_time;
			event->button.axes = NULL;
			break;
		case GDK_KEY_PRESS:
		case GDK_KEY_RELEASE:
			event->key.time = the_time;
			break;
		case GDK_ENTER_NOTIFY:
		case GDK_LEAVE_NOTIFY:
			event->crossing.time = the_time;
			break;
		case GDK_PROPERTY_NOTIFY:
			event->property.time = the_time;
			break;
		case GDK_SELECTION_CLEAR:
		case GDK_SELECTION_REQUEST:
		case GDK_SELECTION_NOTIFY:
			event->selection.time = the_time;
			break;
		case GDK_PROXIMITY_IN:
		case GDK_PROXIMITY_OUT:
			event->proximity.time = the_time;
			break;
		case GDK_DRAG_ENTER:
		case GDK_DRAG_LEAVE:
		case GDK_DRAG_MOTION:
		case GDK_DRAG_STATUS:
		case GDK_DROP_START:
		case GDK_DROP_FINISHED:
			event->dnd.time = the_time;
			break;
		case GDK_FOCUS_CHANGE:
		case GDK_CONFIGURE:
		case GDK_MAP:
		case GDK_UNMAP:
		case GDK_CLIENT_EVENT:
		case GDK_VISIBILITY_NOTIFY:
		case GDK_NO_EXPOSE:
		case GDK_SCROLL:
		case GDK_DELETE:
		case GDK_DESTROY:
		case GDK_EXPOSE:
		default:
			break;
	}
	if (append_to_queue) {
		_gdk_event_queue_append(gdk_display_get_default(), event);
	}
	return event;
}
