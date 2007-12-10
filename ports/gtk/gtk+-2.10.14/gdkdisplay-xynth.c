
#include "gdkxynth.h"

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
	
	_gdk_display = g_object_new(GDK_TYPE_DISPLAY_XYNTH, NULL);
	_gdk_screen  = g_object_new(GDK_TYPE_SCREEN, NULL);
	
	_gdk_display->xynth = xynth;
	
	_gdk_xynth_keyboard_init();
	_gdk_visual_init();

	gdk_screen_set_default_colormap(_gdk_screen, gdk_screen_get_system_colormap(_gdk_screen));

	  ASSERT();
	LEAVE();
	return 0;
#if 0
	  DFBResult  ret;
	  IDirectFB              *directfb;
	  IDirectFBDisplayLayer  *layer;
	  IDirectFBInputDevice   *keyboard;

	  int argc=0;
	  char **argv=NULL;

	#if 1  /* arg hack arg support broken*/
	  if(directfb_args ) {
		argc=2;
		argv = (char **)g_malloc(sizeof(char *)*argc);
		argv[0] = "simple";
		argv[1] = "--dfb:system=SDL";
	  }
	#endif

	  ret = DirectFBInit (&argc,&argv);
	  if (ret != DFB_OK)
	{
	      DirectFBError ("gdk_display_open: DirectFBInit", ret);
	      return NULL;
	    }

	    ret = DirectFBCreate (&directfb);

	  if (ret != DFB_OK)
	    {
	      DirectFBError ("gdk_display_open: DirectFBCreate", ret);
	      return NULL;
	    }
	  _gdk_display = g_object_new(GDK_TYPE_DISPLAY_DFB,NULL);
	  _gdk_display->directfb=directfb;

	  ret = directfb->GetDisplayLayer (directfb, DLID_PRIMARY, &layer);
	  if (ret != DFB_OK)
	    {
	      DirectFBError ("gdk_display_open: GetDisplayLayer", ret);
	      directfb->Release (directfb);
		  directfb = NULL;
	      return NULL;
	    }


	  ret=directfb->GetInputDevice (directfb, DIDID_KEYBOARD, &keyboard);

	  if (ret != DFB_OK){
	      DirectFBError ("gdk_display_open: GetDisplayLayer", ret);
	   	return NULL;
	  }

	  _gdk_display->layer=layer;
	  _gdk_display->keyboard=keyboard;

	    _gdk_directfb_keyboard_init ();

	  _gdk_screen = g_object_new (GDK_TYPE_SCREEN, NULL);

	  _gdk_visual_init ();

	  gdk_screen_set_default_colormap (_gdk_screen,
	                                   gdk_screen_get_system_colormap (_gdk_screen));
	  _gdk_windowing_window_init ();
	  _gdk_windowing_image_init ();

	  _gdk_input_init ();
	  _gdk_dnd_init ();

	  _gdk_events_init ();
	  layer->EnableCursor (layer, 1);

	  g_signal_emit_by_name (gdk_display_manager_get (),
				 "display_opened", _gdk_display);

	  return GDK_DISPLAY_OBJECT(_gdk_display);
#endif
}
