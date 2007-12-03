
#include "gdk-xynth.h"

void _gdk_windowing_image_init (GdkDisplay *display)
{
	ENT();
	LEV();
}

gint _gdk_windowing_get_bits_for_depth (GdkDisplay *display, gint depth)
{
	if ((1 == depth) ||
	    (8 == depth) ||
	    (16 == depth) ||
	    (24 == depth) ||
	    (32 == depth)) {
		return depth;
	} else if (15 == depth) {
		return 16;
	} else {
		g_assert_not_reached();
	}
	return 0;	
}

