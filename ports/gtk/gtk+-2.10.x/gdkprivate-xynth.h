
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

extern GdkWindow       *_gdk_xynth_pointer_grab_window;
extern GdkCursor       *_gdk_xynth_pointer_grab_cursor;
extern gboolean         _gdk_xynth_pointer_grab_owner_events;
extern GdkEventMask     _gdk_xynth_pointer_grab_events;

typedef struct _GdkWindowImplXYNTH   GdkWindowImplXYNTH;
typedef struct _GdkPixmapImplXYNTH   GdkPixmapImplXYNTH;
typedef struct _GdkDrawableImplXYNTH GdkDrawableImplXYNTH;

#define GDK_TYPE_DRAWABLE_IMPL_XYNTH       (gdk_drawable_impl_xynth_get_type())
#define GDK_DRAWABLE_IMPL_XYNTH(object)    (G_TYPE_CHECK_INSTANCE_CAST((object), GDK_TYPE_DRAWABLE_IMPL_XYNTH, GdkDrawableImplXYNTH))
#define GDK_IS_DRAWABLE_IMPL_XYNTH(object) (G_TYPE_CHECK_INSTANCE_TYPE((object), GDK_TYPE_DRAWABLE_IMPL_XYNTH))

#define GDK_TYPE_WINDOW_IMPL_XYNTH         (gdk_window_impl_xynth_get_type())
#define GDK_WINDOW_IMPL_XYNTH(object)      (G_TYPE_CHECK_INSTANCE_CAST((object), GDK_TYPE_WINDOW_IMPL_XYNTH, GdkWindowImplXYNTH))
#define GDK_IS_WINDOW_IMPL_XYNTH(object)   (G_TYPE_CHECK_INSTANCE_TYPE((object), GDK_TYPE_WINDOW_IMPL_XYNTH))

#define GDK_TYPE_PIXMAP_IMPL_XYNTH         (gdk_pixmap_impl_xynth_get_type())
#define GDK_PIXMAP_IMPL_XYNTH(object)      (G_TYPE_CHECK_INSTANCE_CAST((object), GDK_TYPE_PIXMAP_IMPL_XYNTH, GdkPixmapImplXYNTH))
#define GDK_IS_PIXMAP_IMPL_XYNTH(object)   (G_TYPE_CHECK_INSTANCE_TYPE((object), GDK_TYPE_PIXMAP_IMPL_XYNTH))

#define GDK_TYPE_GC_XYNTH                  (gdk_gc_xynth_get_type())
#define GDK_GC_XYNTH(object)               (G_TYPE_CHECK_INSTANCE_CAST((object), GDK_TYPE_GC_XYNTH, GdkGCXYNTH))
#define GDK_IS_GC_XYNTH(object)            (G_TYPE_CHECK_INSTANCE_TYPE((object), GDK_TYPE_GC_XYNTH))

typedef struct {
	GdkVisual visual;
} GdkVisualXYNTH;

typedef struct {
	GdkDrawableClass parent_class;
} GdkDrawableImplXYNTHClass;

typedef struct {
	GdkDrawableImplXYNTHClass parent_class;
} GdkWindowImplXYNTHClass;

typedef struct {
	GdkDrawableImplXYNTHClass parent_class;
} GdkPixmapImplXYNTHClass;

typedef struct {
	GdkGCClass        parent_class;
} GdkGCXYNTHClass;

typedef struct {
	GdkGC             parent_instance;
	GdkRegion        *clip_region;
	GdkGCValuesMask   values_mask;
	GdkGCValues       values;
} GdkGCXYNTH;

struct _GdkDrawableImplXYNTH {
	GdkDrawable parent_object;
	GdkDrawable *wrapper;

	gboolean  buffered;
	GdkRegion *paint_region;
	gint      paint_depth;

	gint width;
	gint height;
	gint abs_x;
	gint abs_y;
	GdkColormap *colormap;
	
	s_surface_t *xynth_surface;
};

struct _GdkWindowImplXYNTH {
	GdkDrawableImplXYNTH drawable;
	GdkWindow *gdkWindow;
	GdkWindowTypeHint type_hint;

	GdkCursor *cursor;

	int input_only;
	s_window_t *xynth_window;
};

struct _GdkPixmapImplXYNTH {
	GdkDrawableImplXYNTH parent_instance;
};

typedef struct {
	GdkCursor         cursor;
	gint              hot_x;
	gint              hot_y;
} GdkCursorXYNTH;

GType gdk_drawable_impl_xynth_get_type (void);
GdkGC * _gdk_xynth_gc_new (GdkDrawable *drawable, GdkGCValues *values, GdkGCValuesMask values_mask);
GdkImage * _gdk_xynth_copy_to_image (GdkDrawable *drawable, GdkImage *image, gint src_x, gint src_y, gint dest_x, gint dest_y, gint width, gint height);
GdkWindow * gdk_xynth_child_at (GdkWindow *window, gint *winx, gint *winy);
void gdk_xynth_mouse_get_info (gint *x, gint *y, GdkModifierType *mask);
GdkWindow * gdk_xynth_other_event_window (GdkWindow *window, GdkEventType type);
GdkEvent * gdk_xynth_event_make (GdkWindow *window, GdkEventType type);
guint32 gdk_xynth_get_time (void);
GdkWindow * gdk_xynth_pointer_event_window (GdkWindow *window, GdkEventType type);

#endif
