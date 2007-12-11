
#include "gdkxynth.h"

typedef struct _GdkDragContextPrivate GdkDragContextPrivate;

struct _GdkDragContextPrivate {
	GdkAtom local_selection;
	guint16 last_x;		/* Coordinates from last event */
	guint16 last_y;
	guint   drag_status : 4;	/* current status of drag      */
};

static GList *contexts = NULL;

static gpointer parent_class = NULL;

#define GDK_DRAG_CONTEXT_PRIVATE_DATA(ctx) ((GdkDragContextPrivate *) GDK_DRAG_CONTEXT (ctx)->windowing_data)

void _gdk_dnd_init (void)
{
	ENTER();
	LEAVE();
}

static void gdk_drag_context_init (GdkDragContext *dragcontext)
{
	ENTER();
	dragcontext->windowing_data = g_new(GdkDragContextPrivate, 1);
	contexts = g_list_prepend(contexts, dragcontext);
	LEAVE();
}

static void gdk_drag_context_finalize (GObject *object)
{
	GdkDragContext        *context;
	GdkDragContextPrivate *private;
	ENTER();
	context = GDK_DRAG_CONTEXT(object);
	private = GDK_DRAG_CONTEXT_PRIVATE_DATA(object);
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
	G_OBJECT_CLASS (parent_class)->finalize(object);
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
