
#include "gdk-xynth.h"

static void gdk_visual_finalize (GObject *object)
{
	ENT();
	g_error("A GdkVisual object was finalized. This should not happen");
	LEV();
}

static void gdk_visual_class_init (GObjectClass *class)
{
	ENT();
	class->finalize = gdk_visual_finalize;
	LEV();
}

GType gdk_visual_get_type (void)
{
	static GType object_type = 0;
	ENT();
	if (!object_type) {
		static const GTypeInfo object_info = {
			sizeof(GdkVisualClass),
			(GBaseInitFunc) NULL,
			(GBaseFinalizeFunc) NULL,
			(GClassInitFunc) gdk_visual_class_init,
			NULL,           /* class_finalize */
			NULL,           /* class_data */
			sizeof(GdkVisual),
			0,              /* n_preallocs */
			(GInstanceInitFunc) NULL,
		};
		object_type = g_type_register_static(G_TYPE_OBJECT, "GdkVisual", &object_info, 0);
	}
	LEV();
	return object_type;
}

void _gdk_visual_init (GdkDisplay *display)
{
	GdkDisplayXynth *display_xynth;
	
	ENT();
	
	display_xynth = GDK_DISPLAY_XYNTH(display);
	
	_gdk_visual = g_object_new(GDK_TYPE_VISUAL, NULL);
	_gdk_visual->depth = display_xynth->window->window->surface->bitsperpixel;
	_gdk_visual->bits_per_rgb = display_xynth->window->window->surface->bitsperpixel;
	_gdk_visual->byte_order = GDK_LSB_FIRST;
	_gdk_visual->colormap_size = 0;
	
	_gdk_visual->type = GDK_VISUAL_TRUE_COLOR;
	
	_gdk_visual->red_prec = display_xynth->window->window->surface->redlength;
	_gdk_visual->red_shift = display_xynth->window->window->surface->redoffset;
	_gdk_visual->red_mask = ((1 << (_gdk_visual->red_prec)) - 1) << _gdk_visual->red_shift;

	_gdk_visual->green_prec = display_xynth->window->window->surface->greenlength;
	_gdk_visual->green_shift = display_xynth->window->window->surface->greenoffset;
	_gdk_visual->green_mask = ((1 << (_gdk_visual->green_prec)) - 1) << _gdk_visual->green_shift;

	_gdk_visual->blue_prec = display_xynth->window->window->surface->bluelength;
	_gdk_visual->blue_shift = display_xynth->window->window->surface->blueoffset;
	_gdk_visual->blue_mask = ((1 << (_gdk_visual->blue_prec)) - 1) << _gdk_visual->blue_shift;
	
	LEV();
}
