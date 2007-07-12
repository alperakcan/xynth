
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

typedef struct _GdkDrawableImplXYNTH GdkDrawableImplXYNTH;
typedef struct _GdkWindowImplXYNTH   GdkWindowImplXYNTH;

#define GDK_TYPE_DRAWABLE_IMPL_XYNTH       (gdk_drawable_impl_xynth_get_type())
#define GDK_DRAWABLE_IMPL_XYNTH(object)    (G_TYPE_CHECK_INSTANCE_CAST((object), GDK_TYPE_DRAWABLE_IMPL_XYNTH, GdkDrawableImplXYNTH))
#define GDK_IS_DRAWABLE_IMPL_XYNTH(object) (G_TYPE_CHECK_INSTANCE_TYPE((object), GDK_TYPE_DRAWABLE_IMPL_XYNTH))

#define GDK_TYPE_WINDOW_IMPL_XYNTH         (gdk_window_impl_xynth_get_type())
#define GDK_WINDOW_IMPL_XYNTH(object)      (G_TYPE_CHECK_INSTANCE_CAST((object), GDK_TYPE_WINDOW_IMPL_XYNTH, GdkWindowImplXYNTH))
#define GDK_IS_WINDOW_IMPL_XYNTH(object)   (G_TYPE_CHECK_INSTANCE_TYPE((object), GDK_TYPE_WINDOW_IMPL_XYNTH))

typedef struct {
	GdkVisual visual;
} GdkVisualXYNTH;

typedef struct {
	GdkDrawableClass parent_class;
} GdkDrawableImplXYNTHClass;

typedef struct {
	GdkDrawableImplXYNTHClass parent_class;
} GdkWindowImplXYNTHClass;

struct _GdkDrawableImplXYNTH {
	GdkDrawable parent_object;
	GdkDrawable *wrapper;
	GdkRegion *paint_region;
	gint paint_depth;
	gint width;
	gint height;
	gint abs_x;
	gint abs_y;
	GdkColormap *colormap;
	cairo_surface_t *cairo_surface;
};

struct _GdkWindowImplXYNTH {
	GdkDrawableImplXYNTH drawable;
	GdkWindow *gdkWindow;
	GdkCursor *cursor;
	GdkWindowTypeHint type_hint;
};

GType      gdk_drawable_impl_xynth_get_type (void);

#endif
