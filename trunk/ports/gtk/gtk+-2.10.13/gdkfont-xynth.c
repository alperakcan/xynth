
#include <config.h>
#include "gdk.h"

#include "gdkinternals.h"
#include "gdkfont.h"
#include "gdkalias.h"

#include "gdkxynth.h"

void _gdk_font_destroy (GdkFont *font)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

gint _gdk_font_strlen (GdkFont *font, const gchar *str)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

gint gdk_font_equal (const GdkFont *fonta, const GdkFont *fontb)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

GdkFont * gdk_font_from_description_for_display (GdkDisplay * display, PangoFontDescription *font_desc)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return NULL;
}

gint gdk_font_id (const GdkFont *font)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

GdkFont * gdk_font_load_for_display (GdkDisplay *display, const gchar *font_name)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return NULL;
}

void gdk_text_extents (GdkFont *font, const gchar *text, gint text_length, gint *lbearing, gint *rbearing, gint *width, gint *ascent, gint *descent)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

gint gdk_text_width (GdkFont *font, const gchar  *text, gint text_length)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

gint gdk_text_width_wc (GdkFont *font, const GdkWChar *text, gint text_length)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}
