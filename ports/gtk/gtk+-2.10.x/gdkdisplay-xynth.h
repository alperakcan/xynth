
#ifndef GDKDISPLAYXYNTH_H_
#define GDKDISPLAYXYNTH_H_

#include <xynth.h>
#include <gdk/gdkdisplay.h>
#include <gdk/gdkkeys.h>

G_BEGIN_DECLS

typedef struct _GdkDisplayXYNTH GdkDisplayXYNTH;
typedef struct _GdkDisplayXYNTHClass GdkDisplayXYNTHClass;

#define GDK_TYPE_DISPLAY_XYNTH              (gdk_display_xynth_get_type())
#define GDK_DISPLAY_XYNTH(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), GDK_TYPE_DISPLAY_XYNTH, GdkDisplayXYNTH))
#define GDK_DISPLAY_XYNTH_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GDK_TYPE_DISPLAY_XYNTH, GdkDisplayXYNTHClass))
#define GDK_IS_DISPLAY_XYNTH(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), GDK_TYPE_DISPLAY_XYNTH))
#define GDK_IS_DISPLAY_XYNTH_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GDK_TYPE_DISPLAY_XYNTH))
#define GDK_DISPLAY_XYNTH_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GDK_TYPE_DISPLAY_XYNTH, GdkDisplayXYNTHClass))

struct _GdkDisplayXYNTH {
	GdkDisplay parent;
	gint xynth_event_fd[2];
	s_window_t *xynth_window;
};

struct _GdkDisplayXYNTHClass {
	GdkDisplayClass parent;
};

GType gdk_display_xynth_get_type (void);

G_END_DECLS

#endif /*GDKDISPLAYXYNTH_H_*/
