
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
