
#include "gdk-xynth.h"

GType gdk_display_xynth_get_type (void)
{
	static GType object_type = 0;
	ENT();
	if (!object_type) {
		static const GTypeInfo object_info = {
			sizeof(GdkDisplayXynthClass),
			(GBaseInitFunc) NULL,
			(GBaseFinalizeFunc) NULL,
			(GClassInitFunc) NULL,
			NULL,                 /* class_finalize */
			NULL,                 /* class_data */
			sizeof(GdkDisplayXynth),
			0,                    /* n_preallocs */
			(GInstanceInitFunc) NULL,
		};
		object_type = g_type_register_static(GDK_TYPE_DISPLAY, "GdkDisplayXynth", &object_info, 0);
	}
	LEV();
	return object_type;
}

GdkDisplay * gdk_display_open (const gchar *display_name)
{
	GdkScreen *screen;
	GdkDisplay *display;
	GdkDisplayXynth *display_xynth;

	ENT();

	if (_gdk_display != NULL) {
		/* single display only, should we return null ? */
		return NULL;
	}

	screen = g_object_new (GDK_TYPE_SCREEN, NULL);
	display = g_object_new(GDK_TYPE_DISPLAY_XYNTH, NULL);
	display_xynth = GDK_DISPLAY_XYNTH(display);

	w_window_init(&(display_xynth->window), WINDOW_MAIN | WINDOW_NOFORM, NULL);
	w_window_set_coor(display_xynth->window, 0, 0, display_xynth->window->window->surface->linear_buf_width, display_xynth->window->window->surface->linear_buf_height);

	_gdk_visual_init(display);
	gdk_screen_set_default_colormap(screen, gdk_screen_get_system_colormap(screen));
	_gdk_windowing_window_init(display);
	
	_gdk_screen = screen;
	_gdk_display = display;

	LEV();
	
	return display;
}

