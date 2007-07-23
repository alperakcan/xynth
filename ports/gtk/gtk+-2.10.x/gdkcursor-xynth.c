
#include <config.h>
#include "gdk.h"

#include "gdkcursor.h"
#include "gdkalias.h"

#include "gdkxynth.h"

void _gdk_cursor_destroy (GdkCursor *cursor)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

GdkDisplay * gdk_cursor_get_display (GdkCursor *cursor)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return NULL;
}

GdkPixbuf * gdk_cursor_get_image (GdkCursor *cursor)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return NULL;
}

GdkCursor * gdk_cursor_new_for_display (GdkDisplay *display, GdkCursorType cursor_type)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return NULL;
}

GdkCursor * gdk_cursor_new_from_name (GdkDisplay *display, const gchar *name)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return NULL;
}

GdkCursor * gdk_cursor_new_from_pixmap (GdkPixmap *source, GdkPixmap *mask, const GdkColor *fg, const GdkColor *bg, gint x, gint y)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return NULL;
}

GdkCursor * gdk_cursor_new_from_pixbuf (GdkDisplay *display, GdkPixbuf *pixbuf, gint x, gint y)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return NULL;
}

void gdk_display_get_maximal_cursor_size (GdkDisplay *display, guint *width, guint *height)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

gboolean gdk_display_supports_cursor_alpha (GdkDisplay *display)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

gboolean gdk_display_supports_cursor_color (GdkDisplay *display)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}
