
#include <config.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <xynth.h>

#include "gdk.h"
#include "gdkwindow.h"

#include "gdkregion-generic.h"

#include "gdkinternals.h"
#include "gdkalias.h"

#define XYNTH_DEBUG 98

#if XYNTH_DEBUG != 0
#define DEBUG(fmt...) {\
	printf(fmt);\
	printf(" [%s (%s:%d)]\n", __FUNCTION__, __FILE__, __LINE__);\
}
#else
#define DEBUG(fmt...) do { } while (0)
#endif

#if XYNTH_DEBUG == 99
#define ENTER() DEBUG("Enter");
#define LEAVE() DEBUG("Leave");
#else
#define ENTER() do { } while (0)
#define LEAVE() do { } while (0)
#endif

#define NIY()   {\
	DEBUG("Not Implemented Yet");\
}

#define ASSERT() {\
	DEBUG("This should not happen");\
	assert(0);\
}

#define ERROR(fmt...) {\
	DEBUG("ERROR" fmt); \
	ASSERT(); \
}

G_BEGIN_DECLS

/* gdkdisplay-xynth.c */

typedef struct _GdkDisplayXYNTH           GdkDisplayXYNTH;
typedef struct _GdkDisplayXYNTHClass      GdkDisplayXYNTHClass;

#define GDK_TYPE_DISPLAY_XYNTH            (gdk_display_xynth_get_type())
#define GDK_DISPLAY_XYNTH(object)         (G_TYPE_CHECK_INSTANCE_CAST((object), GDK_TYPE_DISPLAY_XYNTH, GdkDisplayXYNTH))
#define GDK_DISPLAY_XYNTH_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),     GDK_TYPE_DISPLAY_XYNTH, GdkDisplayXYNTHClass))
#define GDK_IS_DISPLAY_XYNTH(object)      (G_TYPE_CHECK_INSTANCE_TYPE((object), GDK_TYPE_DISPLAY_XYNTH))
#define GDK_IS_DISPLAY_XYNTH_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),     GDK_TYPE_DISPLAY_XYNTH))
#define GDK_DISPLAY_XYNTH_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),     GDK_TYPE_DISPLAY_XYNTH, GdkDisplayXYNTHClass))

struct _GdkDisplayXYNTH {
	GdkDisplay parent;
	s_window_t *xynth;
};

struct _GdkDisplayXYNTHClass {
	GdkDisplayClass parent;
};

GType gdk_display_xynth_get_type (void);

/* gdkdrawable-xynth.c */

typedef struct _GdkDrawableImplXYNTH      GdkDrawableImplXYNTH;
typedef struct _GdkDrawableImplXYNTHClass GdkDrawableImplXYNTHClass;

#define GDK_TYPE_DRAWABLE_IMPL_XYNTH       (gdk_drawable_impl_xynth_get_type())
#define GDK_DRAWABLE_IMPL_XYNTH(object)    (G_TYPE_CHECK_INSTANCE_CAST((object), GDK_TYPE_DRAWABLE_IMPL_XYNTH, GdkDrawableImplXYNTH))
#define GDK_IS_DRAWABLE_IMPL_XYNTH(object) (G_TYPE_CHECK_INSTANCE_TYPE((object), GDK_TYPE_DRAWABLE_IMPL_XYNTH))

struct _GdkDrawableImplXYNTH {
	GdkDrawable           parent_object;
	GdkDrawable           *wrapper;
	gboolean              buffered;
	GdkRegion             *paint_region;
	gint                  paint_depth;
	gint                  width;
	gint                  height;
	gint                  abs_x;
	gint                  abs_y;
	GdkColormap           *colormap;
	s_surface_t           *surface;
	cairo_surface_t       *cairo_surface;
};

struct _GdkDrawableImplXYNTHClass {
	GdkDrawableClass parent_class;
};

/* gdkwndow-xynth.c */

typedef struct _GdkWindowImplXYNTH      GdkWindowImplXYNTH;
typedef struct _GdkWindowImplXYNTHClass GdkWindowImplXYNTHClass;

#define GDK_TYPE_WINDOW_IMPL_XYNTH         (gdk_window_impl_xynth_get_type())
#define GDK_WINDOW_IMPL_XYNTH(object)      (G_TYPE_CHECK_INSTANCE_CAST((object), GDK_TYPE_WINDOW_IMPL_XYNTH, GdkWindowImplXYNTH))
#define GDK_IS_WINDOW_IMPL_XYNTH(object)   (G_TYPE_CHECK_INSTANCE_TYPE((object), GDK_TYPE_WINDOW_IMPL_XYNTH))

struct _GdkWindowImplXYNTH {
	GdkDrawableImplXYNTH drawable;
	GdkWindow            *gdkWindow;
	
	s_window_t           *xynth;
	int                  xynth_id;
	int                  input_only;
	
	GdkCursor            *cursor;
	GHashTable           *properties;
	
	GdkWindowTypeHint    type_hint;
};

struct _GdkWindowImplXYNTHClass {
	GdkDrawableImplXYNTHClass parent_class;
};

void _gdk_windowing_window_init (void);

/* gdkkeys-xynth.c */

void _gdk_xynth_keyboard_init (void);

/* gdkvisual-xynth.c */

void _gdk_visual_init (void);

/* gdkglobals-xynth.c */

extern GdkDisplayXYNTH *_gdk_display;
extern GdkScreen       *_gdk_screen;
extern GdkVisual       *system_visual;
extern GdkWindow       *_gdk_parent_root;

G_END_DECLS
