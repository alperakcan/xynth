
#include "gdk-xynth.h"

void _gdk_windowing_init (void)
{
	ENT();
	_gdk_selection_property = gdk_atom_intern("GDK_SELECTION", FALSE);
	LEV();
}

void gdk_notify_startup_complete (void)
{
	ENT();
	LEV();
}

guint32 gdk_xynth_get_time(void)
{
	GTimeVal tv;
	ENT();
	g_get_current_time(&tv);
	LEV();
	return (guint32) tv.tv_sec * 1000 + tv.tv_usec / 1000;
}
