
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

