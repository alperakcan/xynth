
#include "gdkxynth.h"

GdkDisplayXYNTH *_gdk_display = NULL;
GdkScreen       *_gdk_screen  = NULL;

GdkVisual       *system_visual = NULL;

GdkWindow       *_gdk_parent_root = NULL;

GdkCursor       *_gdk_xynth_pointer_grab_cursor        = NULL;
