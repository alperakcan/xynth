
#include "gdk-xynth.h"

GdkDisplay * gdk_display_open (const gchar *display_name)
{
	ENT();
	if (_gdk_display != NULL) {
		/* single display only, should we return null ? */
		return NULL;
	}
	LEV();
}

