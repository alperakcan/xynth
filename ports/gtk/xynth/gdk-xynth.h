
#include <config.h>
#include "gdk.h"
#include "gdkprivate.h"
#include "gdkalias.h"

#include <xynth.h>
#include <widget.h>

#define NIY() {\
	static int __niy__ = 1;\
	if (__niy__) {\
		printf("NIY: %s (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);\
		__niy__ = 0;\
		exit(1);\
	}\
}

#define ENT() \
	static int __ent__ = 0;\
	printf("ENT [%d]: %s (%s:%d)\n", __ent__, __FUNCTION__, __FILE__, __LINE__);\
	__ent__++;

#define LEV() \
	__ent__--;\
	printf("LEV [%d]: %s (%s:%d)\n", __ent__, __FUNCTION__, __FILE__, __LINE__);

typedef struct _GdkDisplayXynth           GdkDisplayXynth;
typedef struct _GdkDisplayXynthClass      GdkDisplayXynthClass;
typedef struct _GdkDrawableImplXynth      GdkDrawableImplXynth;
typedef struct _GdkDrawableImplXynthClass GdkDrawableImplXynthClass;
typedef struct _GdkWindowImplXynth        GdkWindowImplXynth;
typedef struct _GdkWindowImplXynthClass   GdkWindowImplXynthClass;

struct _GdkDisplayXynth {
	GdkDisplay parent;
	w_window_t *window;
};

struct _GdkDisplayXynthClass {
	GdkDisplayClass parent;
};

struct _GdkVisualClass {
	GObjectClass parent_class;
};

struct _GdkDrawableImplXynth {
	GdkDrawable parent_object;
};

struct _GdkDrawableImplXynthClass {
	GdkDrawableClass parent_class;
};

struct _GdkWindowImplXynth {
	GdkDrawableImplXynth parent_instance;
	
	int width;
	int height;
	
	GdkRectangle clip;
};

struct _GdkWindowImplXynthClass {
	GdkDrawableImplXynthClass parent_class;
};

#define GDK_TYPE_DISPLAY_XYNTH                  (gdk_display_xynth_get_type())
#define GDK_DISPLAY_XYNTH(object)               (G_TYPE_CHECK_INSTANCE_CAST ((object), GDK_TYPE_DISPLAY_XYNTH, GdkDisplayXynth))
#define GDK_TYPE_DRAWABLE_IMPL_XYNTH            (_gdk_drawable_impl_xynth_get_type())
#define GDK_DRAWABLE_IMPL_XYNTH(object)         (G_TYPE_CHECK_INSTANCE_CAST ((object), GDK_TYPE_DRAWABLE_IMPL_XYNTH, GdkDrawableImplXynth))

#define GDK_TYPE_WINDOW_IMPL_XYNTH              (gdk_window_impl_xynth_get_type())
#define GDK_WINDOW_IMPL_XYNTH(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), GDK_TYPE_WINDOW_IMPL_XYNTH, GdkWindowImplXynth))
#define GDK_WINDOW_IMPL_XYNTH_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GDK_TYPE_WINDOW_IMPL_XYNTH, GdkWindowImplXynthClass))
#define GDK_IS_WINDOW_IMPL_XYNTH(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), GDK_TYPE_WINDOW_IMPL_XYNTH))
#define GDK_IS_WINDOW_IMPL_XYNTH_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GDK_TYPE_WINDOW_IMPL_XYNTH))
#define GDK_WINDOW_IMPL_XYNTH_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GDK_TYPE_WINDOW_IMPL_XYNTH, GdkWindowImplXynthClass))

GdkAtom _gdk_selection_property;

GdkDisplay  *_gdk_display;
GdkScreen   *_gdk_screen;
GdkVisual   *_gdk_visual;
GdkColormap *_gdk_colormap;
GdkWindow   *_gdk_parent_root;

void _gdk_visual_init (GdkDisplay *display);
void _gdk_windowing_window_init (GdkDisplay *display);
GType gdk_window_impl_xynth_get_type (void);
