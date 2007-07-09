
#include <config.h>
#include "gdk.h"

#include "gdkinput.h"
#include "gdkkeysyms.h"
#include "gdkalias.h"

#include "gdkxynth.h"

gboolean gdk_device_get_axis (GdkDevice *device, gdouble *axes, GdkAxisUse use, gdouble *value)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

GType gdk_device_get_type (void)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
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
