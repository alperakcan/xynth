
#include <config.h>
#include "gdk.h"

#include "gdkdisplay.h"

#include "gdkinternals.h"

#include "gdkintl.h"
#include "gdkalias.h"

#include "gdkxynth.h"
#include "gdkprivate-xynth.h"

static const guint type_masks[] = {
	GDK_STRUCTURE_MASK,        /* GDK_DELETE            =  0, */
	GDK_STRUCTURE_MASK,        /* GDK_DESTROY           =  1, */
	GDK_EXPOSURE_MASK,         /* GDK_EXPOSE            =  2, */
	GDK_POINTER_MOTION_MASK,   /* GDK_MOTION_NOTIFY     =  3, */
	GDK_BUTTON_PRESS_MASK,     /* GDK_BUTTON_PRESS      =  4, */
	GDK_BUTTON_PRESS_MASK,     /* GDK_2BUTTON_PRESS     =  5, */
	GDK_BUTTON_PRESS_MASK,     /* GDK_3BUTTON_PRESS     =  6, */
	GDK_BUTTON_RELEASE_MASK,   /* GDK_BUTTON_RELEASE    =  7, */
	GDK_KEY_PRESS_MASK,        /* GDK_KEY_PRESS         =  8, */
	GDK_KEY_RELEASE_MASK,      /* GDK_KEY_RELEASE       =  9, */
	GDK_ENTER_NOTIFY_MASK,     /* GDK_ENTER_NOTIFY      = 10, */
	GDK_LEAVE_NOTIFY_MASK,     /* GDK_LEAVE_NOTIFY      = 11, */
	GDK_FOCUS_CHANGE_MASK,     /* GDK_FOCUS_CHANGE      = 12, */
	GDK_STRUCTURE_MASK,        /* GDK_CONFIGURE         = 13, */
	GDK_VISIBILITY_NOTIFY_MASK,/* GDK_MAP               = 14, */
	GDK_VISIBILITY_NOTIFY_MASK,/* GDK_UNMAP             = 15, */
	GDK_PROPERTY_CHANGE_MASK,  /* GDK_PROPERTY_NOTIFY   = 16, */
	GDK_PROPERTY_CHANGE_MASK,  /* GDK_SELECTION_CLEAR   = 17, */
	GDK_PROPERTY_CHANGE_MASK,  /* GDK_SELECTION_REQUEST = 18, */
	GDK_PROPERTY_CHANGE_MASK,  /* GDK_SELECTION_NOTIFY  = 19, */
	GDK_PROXIMITY_IN_MASK,     /* GDK_PROXIMITY_IN      = 20, */
	GDK_PROXIMITY_OUT_MASK,    /* GDK_PROXIMITY_OUT     = 21, */
	GDK_ALL_EVENTS_MASK,       /* GDK_DRAG_ENTER        = 22, */
	GDK_ALL_EVENTS_MASK,       /* GDK_DRAG_LEAVE        = 23, */
	GDK_ALL_EVENTS_MASK,       /* GDK_DRAG_MOTION       = 24, */
	GDK_ALL_EVENTS_MASK,       /* GDK_DRAG_STATUS       = 25, */
	GDK_ALL_EVENTS_MASK,       /* GDK_DROP_START        = 26, */
	GDK_ALL_EVENTS_MASK,       /* GDK_DROP_FINISHED     = 27, */
	GDK_ALL_EVENTS_MASK,       /* GDK_CLIENT_EVENT      = 28, */
	GDK_VISIBILITY_NOTIFY_MASK,/* GDK_VISIBILITY_NOTIFY = 29, */
	GDK_EXPOSURE_MASK,         /* GDK_NO_EXPOSE         = 30, */
	GDK_SCROLL_MASK            /* GDK_SCROLL            = 31  */
};

GdkWindow * gdk_xynth_other_event_window (GdkWindow *window, GdkEventType type)
{
	guint32 evmask;
	GdkWindow *w;
	ENTER();
	w = window;
	while (w != _gdk_parent_root) {
		/* Huge hack, so that we don't propagate events to GtkWindow->frame */
		if ((w != window) &&
		    (GDK_WINDOW_OBJECT(w)->window_type != GDK_WINDOW_CHILD) &&
		    (g_object_get_data(G_OBJECT(w), "gdk-window-child-handler"))) {
			break;
		}
		evmask = GDK_WINDOW_OBJECT(w)->event_mask;
		if (evmask & type_masks[type]) {
			LEAVE();
			return w;
		}
		w = gdk_window_get_parent(w);
	}
	LEAVE();
	return NULL;
}

GdkEvent * gdk_xynth_event_make (GdkWindow *window, GdkEventType type)
{
	GdkEvent *event;
	guint32 the_time;
	ENTER();
	the_time = gdk_xynth_get_time();
	event = gdk_event_new(GDK_NOTHING);
	event->any.type = type;
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
		case GDK_SCROLL:
			event->scroll.time = the_time;
			break;
		case GDK_FOCUS_CHANGE:
		case GDK_CONFIGURE:
		case GDK_MAP:
		case GDK_UNMAP:
		case GDK_CLIENT_EVENT:
		case GDK_VISIBILITY_NOTIFY:
		case GDK_NO_EXPOSE:
		case GDK_DELETE:
		case GDK_DESTROY:
		case GDK_EXPOSE:
		default:
			break;
	}
	_gdk_event_queue_append(gdk_display_get_default(), event);
	LEAVE();
	return event;
}

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
