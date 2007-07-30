
#include "config.h"
#include "gdk.h"

#include "gdkinternals.h"
#include "gdkalias.h"

#include "gdkxynth.h"

void _gdk_windowing_window_get_offsets (GdkWindow *window, gint *x_offset, gint *y_offset)
{
	ENTER();
	if (x_offset) {
		*x_offset = 0;
	}
	if (y_offset) {
		*y_offset = 0;
	}
	LEAVE();
}

gboolean _gdk_windowing_window_queue_antiexpose (GdkWindow *window, GdkRegion *area)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_window_scroll (GdkWindow *window, gint dx, gint dy)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}
