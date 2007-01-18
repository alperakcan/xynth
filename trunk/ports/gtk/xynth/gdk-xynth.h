
#include <config.h>
#include "gdk.h"
#include "gdkprivate.h"
#include "gdkalias.h"
#include "gdktypes.h"
#include "gdkregion-generic.h"

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

#if 1
#define LEV() \
	__ent__--;\
	printf("LEV [%d]: %s (%s:%d)\n", __ent__, __FUNCTION__, __FILE__, __LINE__);
#else
#define LEV()
#endif

#define DBG(dbg...) \
	printf("DBG ");\
	printf(dbg);\
	printf(" [%s (%s:%d)]\n", __FUNCTION__, __FILE__, __LINE__);

#define DBG_WINDOW_TYPE() \
	switch (draw_impl->window_type) { \
		case GDK_WINDOW_TOPLEVEL: DBG("GDK_WINDOW_TOPLEVEL"); break; \ 
		case GDK_WINDOW_CHILD:    DBG("GDK_WINDOW_CHILD");    break; \
		case GDK_WINDOW_DIALOG:   DBG("GDK_WINDOW_DIALOG");   break; \
		case GDK_WINDOW_TEMP:     DBG("GDK_WINDOW_TEMP");     break; \
		case GDK_WINDOW_ROOT:     DBG("GDK_WINDOW_ROOT");     break; \
		default:                  DBG("GDK_WINDOW_UNKNOWN");  break; \
	}


typedef struct _GdkDisplayXynth           GdkDisplayXynth;
typedef struct _GdkDisplayXynthClass      GdkDisplayXynthClass;
typedef struct _GdkDrawableImplXynth      GdkDrawableImplXynth;
typedef struct _GdkDrawableImplXynthClass GdkDrawableImplXynthClass;
typedef struct _GdkWindowImplXynth        GdkWindowImplXynth;
typedef struct _GdkWindowImplXynthClass   GdkWindowImplXynthClass;
typedef struct _GdkDevicePrivate          GdkDevicePrivate;
typedef struct _GdkPixmapImplXynth        GdkPixmapImplXynth;
typedef struct _GdkPixmapImplXynthClass   GdkPixmapImplXynthClass;
typedef struct _GdkGCXynth                GdkGCXynth;
typedef struct _GdkGCXynthClass           GdkGCXynthClass;

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
	GdkDrawable *wrapper;
	GdkWindowType window_type;
	GdkColormap *colormap;
	w_object_t *object;
};

struct _GdkDrawableImplXynthClass {
	GdkDrawableClass parent_class;
};

struct _GdkWindowImplXynth {
	GdkDrawableImplXynth parent_instance;
	GdkWindow *wrapper;
	int width;
	int height;
	GdkRectangle clip;
	w_window_t *window;
};

struct _GdkWindowImplXynthClass {
	GdkDrawableImplXynthClass parent_class;
};

struct _GdkDevicePrivate {
	GdkDevice  info;
};

struct _GdkDeviceClass {
	GObjectClass parent_class;
};

struct _GdkPixmapImplXynth {
	GdkDrawableImplXynth parent_instance;
	gint width;
	gint height;
	guint is_foreign;
};
 
struct _GdkPixmapImplXynthClass {
	GdkDrawableImplXynthClass parent_class;
};

struct _GdkGCXynth {
	GdkGC parent_instance;
	int depth;
	GdkGCValues values;
	GdkGCValuesMask values_mask;
	GdkRegion *clip_region;
	gchar *dash_list;
	gint dash_offset;
	gushort dash_list_len;
};

struct _GdkGCXynthClass {
	GdkGCClass parent_class;
};

#define GDK_TYPE_DISPLAY_XYNTH                  (gdk_display_xynth_get_type())
#define GDK_DISPLAY_XYNTH(object)               (G_TYPE_CHECK_INSTANCE_CAST ((object), GDK_TYPE_DISPLAY_XYNTH, GdkDisplayXynth))

#define GDK_TYPE_DRAWABLE_IMPL_XYNTH            (_gdk_drawable_impl_xynth_get_type())
#define GDK_DRAWABLE_IMPL_XYNTH(object)         (G_TYPE_CHECK_INSTANCE_CAST ((object), GDK_TYPE_DRAWABLE_IMPL_XYNTH, GdkDrawableImplXynth))
#define GDK_DRAWABLE_IMPL_XYNTH_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GDK_TYPE_DRAWABLE_IMPL_XYNTH, GdkDrawableImplXynthClass))
#define GDK_IS_DRAWABLE_IMPL_XYNTH(object)      (G_TYPE_CHECK_INSTANCE_TYPE ((object), GDK_TYPE_DRAWABLE_IMPL_XYNTH))
#define GDK_IS_DRAWABLE_IMPL_XYNTH_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GDK_TYPE_DRAWABLE_IMPL_XYNTH))
#define GDK_DRAWABLE_IMPL_XYNTH_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GDK_TYPE_DRAWABLE_IMPL_XYNTH, GdkDrawableImplXynthClass))

#define GDK_TYPE_WINDOW_IMPL_XYNTH              (gdk_window_impl_xynth_get_type())
#define GDK_WINDOW_IMPL_XYNTH(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), GDK_TYPE_WINDOW_IMPL_XYNTH, GdkWindowImplXynth))
#define GDK_WINDOW_IMPL_XYNTH_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GDK_TYPE_WINDOW_IMPL_XYNTH, GdkWindowImplXynthClass))
#define GDK_IS_WINDOW_IMPL_XYNTH(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), GDK_TYPE_WINDOW_IMPL_XYNTH))
#define GDK_IS_WINDOW_IMPL_XYNTH_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GDK_TYPE_WINDOW_IMPL_XYNTH))
#define GDK_WINDOW_IMPL_XYNTH_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GDK_TYPE_WINDOW_IMPL_XYNTH, GdkWindowImplXynthClass))

#define GDK_TYPE_PIXMAP_IMPL_XYNTH              (gdk_pixmap_impl_xynth_get_type ())
#define GDK_PIXMAP_IMPL_XYNTH(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), GDK_TYPE_PIXMAP_IMPL_XYNTH, GdkPixmapImplXynth))
#define GDK_PIXMAP_IMPL_XYNTH_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GDK_TYPE_PIXMAP_IMPL_XYNTH, GdkPixmapImplXynthClass))
#define GDK_IS_PIXMAP_IMPL_XYNTH(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), GDK_TYPE_PIXMAP_IMPL_XYNTH))
#define GDK_IS_PIXMAP_IMPL_XYNTH_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GDK_TYPE_PIXMAP_IMPL_XYNTH))
#define GDK_PIXMAP_IMPL_XYNTH_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GDK_TYPE_PIXMAP_IMPL_XYNTH, GdkPixmapImplXynthClass))

#define GDK_TYPE_GC_XYNTH                       (_gdk_gc_xynth_get_type())
#define GDK_GC_XYNTH(object)                    (G_TYPE_CHECK_INSTANCE_CAST ((object), GDK_TYPE_GC_XYNTH, GdkGCXynth))
#define GDK_GC_XYNTH_CLASS(klass)               (G_TYPE_CHECK_CLASS_CAST ((klass), GDK_TYPE_GC_XYNTH, GdkGCXynthClass))
#define GDK_IS_GC_XYNTH(object)                 (G_TYPE_CHECK_INSTANCE_TYPE ((object), GDK_TYPE_GC_XYNTH))
#define GDK_IS_GC_XYNTH_CLASS(klass)            (G_TYPE_CHECK_CLASS_TYPE ((klass), GDK_TYPE_GC_XYNTH))
#define GDK_GC_XYNTH_GET_CLASS(obj)             (G_TYPE_INSTANCE_GET_CLASS ((obj), GDK_TYPE_GC_XYNTH, GdkGCXynthClass))

GdkAtom _gdk_selection_property;

GdkDisplay  *_gdk_display;
GdkScreen   *_gdk_screen;
GdkVisual   *_gdk_visual;
GdkColormap *_gdk_colormap;
GdkWindow   *_gdk_parent_root;

void _gdk_windowing_visual_init (GdkDisplay *display);
void _gdk_windowing_window_init (GdkDisplay *display);
void _gdk_windowing_image_init (GdkDisplay *display);
void _gdk_windowing_event_init (GdkDisplay *display);
void _gdk_windowing_input_init (GdkDisplay *display);
void _gdk_windowing_dnd_init (GdkDisplay *display);

GType _gdk_drawable_impl_xynth_get_type (void);
GType gdk_window_impl_xynth_get_type (void);
GdkGC * gdk_xynth_gc_new (GdkDrawable *drawable, GdkGCValues *values, GdkGCValuesMask values_mask);
GdkEvent * gdk_event_make (GdkWindow *window, GdkEventType type, gboolean append_to_queue);
