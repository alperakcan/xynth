
#ifndef GDKPRIVATEXYNTH_H_
#define GDKPRIVATEXYNTH_H_

#include <gdk/gdkprivate.h>
#include "gdkinternals.h"
#include "gdkcursor.h"
#include "gdkdisplay-xynth.h"
#include <cairo.h>

#include "gdkxynth.h"

extern GdkDisplayXYNTH *_gdk_display;
extern GdkScreen       *_gdk_screen;

typedef struct {
	GdkVisual visual;
} GdkVisualXYNTH;

#endif
