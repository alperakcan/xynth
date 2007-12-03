
#include "gdk-xynth.h"

GList *_gdk_input_devices;
gint   _gdk_input_ignore_core;

static GdkDeviceAxis gdk_input_core_axes[] = {
	{ GDK_AXIS_X, 0, 0 },
	{ GDK_AXIS_Y, 0, 0 }
};

void _gdk_init_input_core (GdkDisplay *display)
{
	ENT();
	_gdk_core_pointer = g_object_new (GDK_TYPE_DEVICE, NULL);
	_gdk_core_pointer->name = "Core Pointer";
	_gdk_core_pointer->source = GDK_SOURCE_MOUSE;
	_gdk_core_pointer->mode = GDK_MODE_SCREEN;
	_gdk_core_pointer->has_cursor = TRUE;
	_gdk_core_pointer->num_axes = 2;
	_gdk_core_pointer->axes = gdk_input_core_axes;
	_gdk_core_pointer->num_keys = 0;
	_gdk_core_pointer->keys = NULL;
	display->core_pointer = _gdk_core_pointer;
	LEV();
}

void _gdk_windowing_input_init (GdkDisplay *display)
{
	ENT();
	_gdk_init_input_core(display);
	_gdk_input_devices = g_list_append (NULL, _gdk_core_pointer);
	_gdk_input_ignore_core = FALSE;
	LEV();
}

static void gdk_device_finalize (GObject *object)
{
	ENT();
	g_error("A GdkDevice object was finalized. This should not happen");
	LEV();
}

static void gdk_device_class_init (GObjectClass *class)
{
	ENT();
	class->finalize = gdk_device_finalize;
	LEV();
}

GType gdk_device_get_type (void)
{
	static GType object_type = 0;
	ENT();
	if (!object_type) {
		static const GTypeInfo object_info = {
			sizeof(GdkDeviceClass),
			(GBaseInitFunc) NULL,
			(GBaseFinalizeFunc) NULL,
			(GClassInitFunc) gdk_device_class_init,
			NULL,           /* class_finalize */
			NULL,           /* class_data */
			sizeof(GdkDevicePrivate),
			0,              /* n_preallocs */
			(GInstanceInitFunc) NULL,
		};
		object_type = g_type_register_static(G_TYPE_OBJECT, "GdkDevice", &object_info, 0);
	}
	LEV();
	return object_type;
}
