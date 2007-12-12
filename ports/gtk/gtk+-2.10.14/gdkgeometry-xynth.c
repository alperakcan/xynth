
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
	LEAVE();
	return FALSE;
}
