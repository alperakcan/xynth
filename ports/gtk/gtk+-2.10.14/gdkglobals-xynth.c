
#include "gdkxynth.h"

GdkDisplayXYNTH *_gdk_display = NULL;
GdkScreen       *_gdk_screen  = NULL;

GdkVisual       *system_visual = NULL;

GdkWindow       *_gdk_parent_root = NULL;

GdkWindow       *_gdk_xynth_pointer_grab_window        = NULL;
GdkCursor       *_gdk_xynth_pointer_grab_cursor        = NULL;
gboolean         _gdk_xynth_pointer_grab_owner_events  = FALSE;
GdkEventMask     _gdk_xynth_pointer_grab_events        = 0;
GdkWindow       *_gdk_xynth_keyboard_grab_window       = NULL;
gboolean         _gdk_xynth_keyboard_grab_owner_events = FALSE;
GdkEventMask     _gdk_xynth_keyboard_grab_events       = 0;

int             _gdk_xynth_mouse_x   = 0;
int             _gdk_xynth_mouse_y   = 0;
GdkModifierType _gdk_xynth_modifiers = 0;
