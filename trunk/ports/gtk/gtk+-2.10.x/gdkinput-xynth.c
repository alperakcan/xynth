
#include <config.h>
#include "gdk.h"

#include "gdkinput.h"
#include "gdkkeysyms.h"
#include "gdkalias.h"

#include "gdkprivate-xynth.h"
#include "gdkxynth.h"
#include "gdkinput-xynth.h"

static GdkDeviceAxis gdk_input_core_axes[] = {
	{ GDK_AXIS_X, 0, 0 },
	{ GDK_AXIS_Y, 0, 0 },
};

GList *_gdk_input_devices       = NULL;
GdkDevice *_gdk_core_pointer    = NULL;
gboolean _gdk_input_ignore_core = FALSE;

int _gdk_xynth_mouse_x = 0;
int _gdk_xynth_mouse_y = 0;

void gdk_xynth_mouse_get_info (gint *x, gint *y, GdkModifierType *mask)
{
	ENTER();
	if (x) {
		*x = _gdk_xynth_mouse_x;
	}
	if (y) {
		*y = _gdk_xynth_mouse_y;
	}
	if (mask) {
		*mask = _gdk_xynth_modifiers;
	}
	LEAVE();
}

gboolean gdk_device_get_axis (GdkDevice *device, gdouble *axes, GdkAxisUse use, gdouble *value)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

static void gdk_device_finalize (GObject *object)
{
	ENTER();
	g_error("A GdkDevice object was finalized. This should not happen");
	LEAVE();
}

static void gdk_device_class_init (GObjectClass *class)
{
	ENTER();
	class->finalize = gdk_device_finalize;
	LEAVE();
}

GType gdk_device_get_type (void)
{
	static GType object_type = 0;
	ENTER();
	if (!object_type) {
		static const GTypeInfo object_info = {
			sizeof(GdkDeviceClass),
			(GBaseInitFunc) NULL,
			(GBaseFinalizeFunc) NULL,
			(GClassInitFunc) gdk_device_class_init,
			NULL,           /* class_finalize */
			NULL,           /* class_data */
			sizeof(GdkDevice),
			0,              /* n_preallocs */
			(GInstanceInitFunc) NULL,
		};
		object_type = g_type_register_static(G_TYPE_OBJECT, "GdkDevice", &object_info, 0);
	}
	LEAVE();
	return object_type;
}

void gdk_device_set_axis_use (GdkDevice *device, guint index, GdkAxisUse use)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_device_set_key (GdkDevice *device, guint index, guint keyval, GdkModifierType modifiers)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

gboolean gdk_device_set_mode (GdkDevice *device, GdkInputMode mode)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_input_set_extension_events (GdkWindow *window, gint mask, GdkExtensionMode mode)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

gboolean gdk_device_get_history  (GdkDevice *device, GdkWindow *window, guint32 start, guint32 stop, GdkTimeCoord ***events, gint *n_events)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_device_free_history (GdkTimeCoord **events, gint n_events)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_device_get_state (GdkDevice *device, GdkWindow *window, gdouble *axes, GdkModifierType *mask)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void _gdk_init_input_core (void)
{
	GdkDisplay *display;
	ENTER();
	display = GDK_DISPLAY_OBJECT(_gdk_display);
	_gdk_core_pointer = g_object_new(GDK_TYPE_DEVICE, NULL);
	_gdk_core_pointer->name       = "Core Pointer";
	_gdk_core_pointer->source     = GDK_SOURCE_MOUSE;
	_gdk_core_pointer->mode       = GDK_MODE_SCREEN;
	_gdk_core_pointer->has_cursor = TRUE;
	_gdk_core_pointer->num_axes   = 2;
	_gdk_core_pointer->axes       = gdk_input_core_axes;
	_gdk_core_pointer->num_keys   = 0;
	_gdk_core_pointer->keys       = NULL;
	display->core_pointer         = _gdk_core_pointer;
	LEAVE();
}

void _gdk_input_init (void)
{
	ENTER();
	_gdk_init_input_core();
	_gdk_input_devices = g_list_append(NULL, _gdk_core_pointer);
	_gdk_input_ignore_core = FALSE;
	LEAVE();
}
