
#include <config.h>
#include "gdk.h"

#include "gdkdnd.h"
#include "gdkproperty.h"
#include "gdkalias.h"

#include "gdkxynth.h"

void gdk_drag_abort (GdkDragContext *context, guint32 time)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

GdkDragContext * gdk_drag_begin (GdkWindow *window, GList *targets)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return NULL;
}

GType gdk_drag_context_get_type (void)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_drag_drop (GdkDragContext *context, guint32 time)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_drag_find_window_for_screen (GdkDragContext *context, GdkWindow *drag_window, GdkScreen *screen, gint x_root, gint y_root, GdkWindow **dest_window, GdkDragProtocol *protocol)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

guint32 gdk_drag_get_protocol_for_display (GdkDisplay *display, guint32 xid, GdkDragProtocol *protocol)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

GdkAtom gdk_drag_get_selection (GdkDragContext *context)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return NULL;
}

gboolean gdk_drag_motion (GdkDragContext *context, GdkWindow *dest_window, GdkDragProtocol protocol, gint x_root, gint y_root, GdkDragAction suggested_action, GdkDragAction possible_actions, guint32 time)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_drag_status (GdkDragContext *context, GdkDragAction action, guint32 time)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_drop_finish (GdkDragContext *context, gboolean success, guint32 time)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_drop_reply (GdkDragContext *context, gboolean ok, guint32 time)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_register_dnd (GdkWindow *window)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}
