
#include "gdkxynth.h"

static int event_fd[2];
static s_list_t *event_list;
static s_thread_mutex_t *event_lock;

static GdkEvent * gdk_event_translate (s_event_t *xevent, GdkWindow *window)
{
	GdkWindowObject *private;
	GdkDisplay      *display;
	GdkEvent        *event = NULL;
	S_EVENT         xtype;
	
	ENTER();
	
	if (xevent == NULL) {
		LEAVE();
		return NULL;
	}
	if (GDK_IS_WINDOW(window) == 0) {
		LEAVE();
		return NULL;
	}

	private = GDK_WINDOW_OBJECT (window);
	g_object_ref(G_OBJECT (window));
	display = gdk_drawable_get_display(GDK_DRAWABLE(window));

	xtype = xevent->type & EVENT_MASK;
	switch (xtype) {
		case QUIT_EVENT:
			DEBUG("QUIT_EVENT");
			{
				GdkWindow *event_win;
				event_win = gdk_xynth_other_event_window(window, GDK_DELETE);
				if (event_win) {
					event = gdk_xynth_event_make(event_win, GDK_DELETE);
				}
			}
			break;
		case KEYBD_EVENT:
			DEBUG("KEYBD_EVENT");
			break;
		case MOUSE_EVENT:
			DEBUG("MOUSE_EVENT");
			break;
		case EXPOSE_EVENT:
			DEBUG("EXPOSE_EVENT");
			break;
		case CONFIG_EVENT:
			DEBUG("CONFIG_EVENT");
			if (xevent->type & CONFIG_CHNGX) {
				DEBUG("CONFIG_CHNGX");
			}
			if (xevent->type & CONFIG_CHNGY) {
				DEBUG("CONFIG_CHNGY");
			}
			if (xevent->type & CONFIG_CHNGW) {
				DEBUG("CONFIG_CHNGW");
			}
			if (xevent->type & CONFIG_CHNGH) {
				DEBUG("CONFIG_CHNGH");
			}
			{
				GdkDrawableImplXYNTH *impl;
				GdkWindow            *event_win;
				GList                *list;
				impl = GDK_DRAWABLE_IMPL_XYNTH(private->impl);
				event_win = gdk_xynth_other_event_window(window, GDK_CONFIGURE);
			        if (event_win) {
			        	event = gdk_xynth_event_make(event_win, GDK_CONFIGURE);
			        	event->configure.x      = xevent->window->surface->buf->x;
			        	event->configure.y      = xevent->window->surface->buf->y;
			        	event->configure.width  = xevent->window->surface->buf->w;
			        	event->configure.height = xevent->window->surface->buf->h;
			        }
			        impl->width  = xevent->window->surface->buf->w;
			        impl->height = xevent->window->surface->buf->h;
			        for (list = private->children; list; list = list->next) {
			        	GdkWindowObject      *win;
			        	GdkDrawableImplXYNTH *impl;
			        	win  = GDK_WINDOW_OBJECT(list->data);
			        	impl = GDK_DRAWABLE_IMPL_XYNTH(win->impl);
			        	_gdk_xynth_move_resize_child(GDK_WINDOW(win), win->x, win->y, impl->width, impl->height);
			        }
			        _gdk_xynth_calc_abs(window);
			        gdk_window_clear(window);
			        gdk_window_invalidate_rect(window, NULL, TRUE);
			      }
			break;
		case FOCUS_EVENT:
			DEBUG("FOCUS_EVENT");
			if (xevent->type & FOCUSIN_EVENT) {
				DEBUG("FOCUSIN_EVENT");
				gdk_xynth_change_focus(window);
			}
			if (xevent->type & FOCUSOUT_EVENT) {
				DEBUG("FOCUSOUT_EVENT");
				gdk_xynth_change_focus(_gdk_parent_root);
			}
			break;
		case DESKTOP_EVENT:
			DEBUG("DESKTOP_EVENT");
			break;
		case TIMER_EVENT:
			DEBUG("TIMER_EVENT");
			break;
		default:
			ERROR("unhandled event");
			break;
	}

	g_object_unref(G_OBJECT(window));
	LEAVE();
	return event;
}

static gboolean xynth_events_io_func (GIOChannel *channel, GIOCondition condition, gpointer data)
{
	gint buf[1];
	gsize read;
	GIOStatus result;
	s_event_t *event;
	GdkWindow *window;

	ENTER();
	result = g_io_channel_read_chars(channel, (gchar *) buf, sizeof(buf), &read, NULL);
	if (result == G_IO_STATUS_ERROR) {
		g_warning("%s: GIOError occured", __FUNCTION__);
		LEAVE();
		return TRUE;
	}
	s_thread_mutex_lock(event_lock);
	while (!s_list_eol(event_list, 0)) {
		event = s_list_get(event_list, 0);
		s_list_remove(event_list, 0);
		window = gdk_xynth_window_id_table_lookup(event->window->id);
		if (window != NULL) {
			DEBUG("received event for window %d", event->window->id);
			gdk_event_translate(event, window);
		}
		s_event_uninit(event);
	}
	s_thread_mutex_unlock(event_lock);
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
	if (s_list_init(&event_list)) {
		ASSERT();
	}
	if (s_thread_mutex_init(&event_lock)) {
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

void gdk_xynth_atevent (s_window_t *window, s_event_t *event)
{
	s_event_t *e;
	GdkWindow *gwindow;
	s_thread_mutex_lock(event_lock);
	gwindow = gdk_xynth_window_id_table_lookup(event->window->id);
	if (gwindow != NULL) {
		s_event_copy(event, &e);
		s_list_add(event_list, e, -1);
		write(event_fd[1], &event->window->id, sizeof(int));
	}
	s_thread_mutex_unlock(event_lock);
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
	if (impl->xynth == NULL) {
		LEAVE();
		return;
	}
	s_window_atevent(impl->xynth, gdk_xynth_atevent);
}
