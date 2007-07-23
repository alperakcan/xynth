
#include <config.h>
#include "gdk.h"

#include "gdkinternals.h"

#include "gdkimage.h"
#include "gdkalias.h"

#include "gdkxynth.h"

GdkImage * _gdk_image_new_for_depth (GdkScreen *screen, GdkImageType type, GdkVisual *visual, gint width, gint height, gint depth)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return NULL;
}

gint _gdk_windowing_get_bits_for_depth (GdkDisplay *display, gint depth)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

guint32 gdk_image_get_pixel (GdkImage *image, gint x, gint y)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

GType gdk_image_get_type (void)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

GdkImage * _gdk_xynth_copy_to_image (GdkDrawable *drawable, GdkImage *image, gint src_x, gint src_y, gint dest_x, gint dest_y, gint width, gint height)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return NULL;
}

void _gdk_windowing_image_init (void)
{
	ENTER();
	LEAVE();
}
