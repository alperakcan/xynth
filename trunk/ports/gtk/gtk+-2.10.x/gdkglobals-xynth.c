
#include <config.h>
#include "gdk.h"

#include "gdkxynth.h"
#include "gdkprivate-xynth.h"

#include "gdkalias.h"

GdkDisplayXYNTH *_gdk_display = NULL;
GdkScreen       *_gdk_screen  = NULL;

GdkWindow       *_gdk_xynth_pointer_grab_window = NULL;
GdkCursor       *_gdk_xynth_pointer_grab_cursor = NULL;
gboolean         _gdk_xynth_pointer_grab_owner_events  = FALSE;
GdkEventMask     _gdk_xynth_pointer_grab_events        = 0;

#include "gdkaliasdef.c"
