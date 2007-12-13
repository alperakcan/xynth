
#include "gdkxynth.h"

GdkScreen * gdk_display_get_default_screen (GdkDisplay *display)
{
	ENTER();
	LEAVE();
	return _gdk_screen;
}

void _gdk_windowing_set_default_display (GdkDisplay *display)
{
	ENTER();
	_gdk_display = GDK_DISPLAY_XYNTH(display);
	LEAVE();
}

GType gdk_display_xynth_get_type (void)
{
	static GType object_type = 0;
	ENTER();
	if (!object_type) {
		static const GTypeInfo object_info = {
			sizeof(GdkDisplayXYNTHClass),
			(GBaseInitFunc) NULL,
			(GBaseFinalizeFunc) NULL,
			(GClassInitFunc) NULL,
			NULL,                 /* class_finalize */
			NULL,                 /* class_data */
			sizeof(GdkDisplayXYNTH),
			0,                    /* n_preallocs */
			(GInstanceInitFunc) NULL,
		};
		object_type = g_type_register_static(GDK_TYPE_DISPLAY, "GdkDisplayXYNTH", &object_info, 0);
	}
	LEAVE();
	return object_type;
}

GdkDisplay * gdk_display_open (const gchar *display_name)
{
	s_window_t *xynth;
	ENTER();
	
	if (_gdk_display) {
		 /* single display only */
		return GDK_DISPLAY_OBJECT(_gdk_display);
	}
	
	if (s_window_init(&xynth)) {
		ERROR("Can not open xynth window");
		return NULL;
	}
	if (s_window_new(xynth, WINDOW_MAIN, NULL)) {
		ERROR("Can not open xynth window");
		return NULL;
	}
	s_thread_create(s_window_main, xynth);
	
	_gdk_display = g_object_new(GDK_TYPE_DISPLAY_XYNTH, NULL);
	_gdk_screen  = g_object_new(GDK_TYPE_SCREEN, NULL);
	
	_gdk_display->xynth = xynth;
	
	_gdk_xynth_keyboard_init();
	_gdk_visual_init();

	gdk_screen_set_default_colormap(_gdk_screen, gdk_screen_get_system_colormap(_gdk_screen));

	_gdk_windowing_window_init();
	_gdk_windowing_image_init();
	_gdk_input_init();
	_gdk_dnd_init();
	
	_gdk_events_init();

	g_signal_emit_by_name(gdk_display_manager_get(), "display_opened", _gdk_display);
	
	LEAVE();
	return GDK_DISPLAY_OBJECT(_gdk_display);
}

s_surface_t * gdk_display_xynth_create_surface (GdkDisplayXYNTH *display, int bpp, int width, int height)
{
	s_surface_t *surface;
	ENTER();
	if (s_surface_create(&surface, width, height, bpp)) {
		LEAVE();
		return NULL;
	}
	LEAVE();
	return surface;

}

void gdk_notify_startup_complete (void)
{
	ENTER();
	LEAVE();
}
