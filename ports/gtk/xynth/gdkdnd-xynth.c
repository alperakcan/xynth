
#include "gdk-xynth.h"

typedef struct _GdkDragContextPrivate GdkDragContextPrivate;

typedef enum {
	GDK_DRAG_STATUS_DRAG,
	GDK_DRAG_STATUS_MOTION_WAIT,
	GDK_DRAG_STATUS_ACTION_WAIT,
	GDK_DRAG_STATUS_DROP
} GtkDragStatus;

struct _GdkDragContextPrivate {
	GdkAtom local_selection;
	guint16 last_x;		/* Coordinates from last event */
	guint16 last_y;
	guint drag_status : 4;	/* current status of drag */
};

#define GDK_DRAG_CONTEXT_PRIVATE_DATA(ctx) ((GdkDragContextPrivate *) GDK_DRAG_CONTEXT (ctx)->windowing_data)

static GList *contexts;
static gpointer parent_class = NULL;

static void gdk_drag_context_init (GdkDragContext *dragcontext)
{
	ENT();
	dragcontext->windowing_data = g_new0(GdkDragContextPrivate, 1);
	contexts = g_list_prepend (contexts, dragcontext);
	LEV();
}

static void gdk_drag_context_finalize (GObject *object)
{
	GdkDragContext *context = GDK_DRAG_CONTEXT(object);
	GdkDragContextPrivate *private = GDK_DRAG_CONTEXT_PRIVATE_DATA(object);
	ENT();
	g_list_free (context->targets);
	if (context->source_window)
		g_object_unref(context->source_window);
	if (context->dest_window)
		g_object_unref(context->dest_window);
	if (private) {
		g_free(private);
		context->windowing_data = NULL;
	}
	contexts = g_list_remove(contexts, context);
	G_OBJECT_CLASS (parent_class)->finalize(object);
	LEV();
}

static void gdk_drag_context_class_init (GdkDragContextClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	ENT();
	parent_class = g_type_class_peek_parent (klass);
	object_class->finalize = gdk_drag_context_finalize;
	LEV();
}

GType gdk_drag_context_get_type (void)
{
	static GType object_type = 0;
	ENT();
	if (!object_type) {
		static const GTypeInfo object_info = {
			sizeof(GdkDragContextClass),
			(GBaseInitFunc) NULL,
			(GBaseFinalizeFunc) NULL,
			(GClassInitFunc) gdk_drag_context_class_init,
			NULL,           /* class_finalize */
			NULL,           /* class_data */
			sizeof (GdkDragContext),
			0,              /* n_preallocs */
			(GInstanceInitFunc) gdk_drag_context_init,
		};
		object_type = g_type_register_static(G_TYPE_OBJECT, "GdkDragContext", &object_info, 0);
	}
	LEV();
	return object_type;
}

void _gdk_windowing_dnd_init (GdkDisplay *display)
{
	ENT();
	LEV();
}
