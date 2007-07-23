
#include <config.h>
#include "gdk.h"

#include "gdkdnd.h"
#include "gdkproperty.h"
#include "gdkalias.h"

#include "gdkprivate-xynth.h"
#include "gdkxynth.h"

#define GDK_DRAG_CONTEXT_PRIVATE_DATA(ctx) ((GdkDragContextPrivate *) GDK_DRAG_CONTEXT (ctx)->windowing_data)

typedef struct _GdkDragContextPrivate GdkDragContextPrivate;

static gpointer  parent_class = NULL;

static GList *contexts = NULL;

struct _GdkDragContextPrivate {
	GdkAtom local_selection;
	
	guint16 last_x;		 /* Coordinates from last event */
	guint16 last_y;
	guint   drag_status : 4; /* current status of drag      */
};

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

static void gdk_drag_context_init (GdkDragContext *dragcontext)
{
	ENTER();
	dragcontext->windowing_data = g_new(GdkDragContextPrivate, 1);
	contexts = g_list_prepend (contexts, dragcontext);
	LEAVE();
}

static void gdk_drag_context_finalize (GObject *object)
{
	GdkDragContext *context = GDK_DRAG_CONTEXT(object);
	GdkDragContextPrivate *private = GDK_DRAG_CONTEXT_PRIVATE_DATA(object);
	ENTER();
	g_list_free(context->targets);
	if (context->source_window) {
		g_object_unref(context->source_window);
	}
	if (context->dest_window) {
		g_object_unref(context->dest_window);
	}
	if (private) {
		g_free(private);
		context->windowing_data = NULL;
	}
	contexts = g_list_remove(contexts, context);
	G_OBJECT_CLASS (parent_class)->finalize (object);
	LEAVE();
}

static void gdk_drag_context_class_init (GdkDragContextClass *klass)
{
	GObjectClass *object_class;
	ENTER();
	object_class = G_OBJECT_CLASS(klass);
	parent_class = g_type_class_peek_parent(klass);
	object_class->finalize = gdk_drag_context_finalize;
	LEAVE();
}

GType gdk_drag_context_get_type (void)
{
	static GType object_type = 0;
	ENTER();
	if (!object_type) {
		static const GTypeInfo object_info = {
			sizeof(GdkDragContextClass),
			(GBaseInitFunc) NULL,
			(GBaseFinalizeFunc) NULL,
			(GClassInitFunc) gdk_drag_context_class_init,
			NULL,           /* class_finalize */
			NULL,           /* class_data */
			sizeof(GdkDragContext),
			0,              /* n_preallocs */
			(GInstanceInitFunc) gdk_drag_context_init,
		};
		object_type = g_type_register_static(G_TYPE_OBJECT, "GdkDragContext", &object_info, 0);
	}
	LEAVE();
	return object_type;
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

void _gdk_dnd_init (void)
{
	ENTER();
	LEAVE();
}
