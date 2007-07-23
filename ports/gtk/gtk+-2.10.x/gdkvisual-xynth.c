
#include "config.h"

#include "gdkscreen.h"
#include "gdkvisual.h"
#include "gdkalias.h"

#include "gdkprivate-xynth.h"
#include "gdkxynth.h"

struct _GdkVisualClass {
	GObjectClass parent_class;
};

GdkVisual *system_visual = NULL;

GdkVisual * gdk_screen_get_system_visual (GdkScreen *screen)
{
	ENTER();
	g_assert(system_visual);
	LEAVE();
	return system_visual;
}

GList * gdk_screen_list_visuals (GdkScreen *screen)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return NULL;
}

GdkScreen * gdk_visual_get_screen (GdkVisual *visual)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return NULL;
}

static void gdk_visual_finalize (GObject *object)
{
	g_error ("A GdkVisual object was finalized. This should not happen");
}

static void gdk_visual_class_init (GObjectClass *class)
{
	class->finalize = gdk_visual_finalize;
}

GType gdk_visual_get_type (void)
{
	static GType object_type = 0;
	if (!object_type) {
		static const GTypeInfo object_info = {
			sizeof(GdkVisualClass),
			(GBaseInitFunc) NULL,
			(GBaseFinalizeFunc) NULL,
			(GClassInitFunc) gdk_visual_class_init,
			NULL,           /* class_finalize */
			NULL,           /* class_data */
			sizeof(GdkVisualXYNTH),
			0,              /* n_preallocs */
			(GInstanceInitFunc) NULL,
		};
		object_type = g_type_register_static(G_TYPE_OBJECT, "GdkVisual", &object_info, 0);
	}
	return object_type;
}

void _gdk_visual_init (void)
{
	ENTER();
	system_visual = g_object_new(GDK_TYPE_VISUAL, NULL);
	g_assert(system_visual != NULL);
	system_visual->depth = _gdk_display->xynth_window->surface->bitsperpixel;
	system_visual->bits_per_rgb = _gdk_display->xynth_window->surface->bitsperpixel;
	system_visual->byte_order = GDK_LSB_FIRST;
	system_visual->colormap_size = 0;
	system_visual->type = GDK_VISUAL_TRUE_COLOR;
	system_visual->red_prec = _gdk_display->xynth_window->surface->redlength;
	system_visual->red_shift = _gdk_display->xynth_window->surface->redoffset;
	system_visual->red_mask = _gdk_display->xynth_window->surface->redmask;
	system_visual->green_prec = _gdk_display->xynth_window->surface->greenlength;
	system_visual->green_shift = _gdk_display->xynth_window->surface->greenoffset;
	system_visual->green_mask = _gdk_display->xynth_window->surface->greenmask;
	system_visual->blue_prec = _gdk_display->xynth_window->surface->bluelength;
	system_visual->blue_shift = _gdk_display->xynth_window->surface->blueoffset;
	system_visual->blue_mask = _gdk_display->xynth_window->surface->bluemask;
	
	DEBUG("RGB len: %d %d %d, off: %d %d %d, mask: 0x%08X 0x%08X 0x%08X",
	      system_visual->red_prec, system_visual->green_prec, system_visual->blue_prec,
	      system_visual->red_shift, system_visual->green_shift, system_visual->blue_shift,
	      system_visual->red_mask, system_visual->green_mask, system_visual->blue_mask);

	LEAVE();
}
