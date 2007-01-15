
#include "gdk-xynth.h"
#include <pango/pangoft2.h>

PangoContext * gdk_pango_context_get_for_screen (GdkScreen *screen)
{
	ENT();
	LEV();
	return pango_ft2_get_context(75.0, 75.0);
}
