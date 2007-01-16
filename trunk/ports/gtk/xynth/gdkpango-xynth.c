
#include "gdk-xynth.h"
#include <pango/pangoft2.h>

PangoContext * gdk_pango_context_get_for_screen (GdkScreen *screen)
{
	PangoContext *pcon;
	ENT();
	pcon = pango_ft2_get_context(75.0, 75.0);
	DBG("pcon: %p", pcon);
	LEV();
	return pcon;
}
