
#include "config.h"

#include "gdkproperty.h"
#include "gdkselection.h"
#include "gdkprivate.h"
#include "gdkalias.h"

#include "gdkxynth.h"

void gdk_free_compound_text (guchar *ctext)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_selection_convert (GdkWindow *requestor, GdkAtom selection, GdkAtom target, guint32 time)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

GdkWindow * gdk_selection_owner_get_for_display (GdkDisplay *display, GdkAtom selection)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return NULL;
}

gint gdk_selection_owner_set_for_display (GdkDisplay *display, GdkWindow *owner, GdkAtom selection, guint32 time, gint send_event)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

gint gdk_selection_property_get (GdkWindow *requestor, guchar **data, GdkAtom *ret_type, gint *ret_format)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_selection_send_notify_for_display (GdkDisplay *display, guint32 requestor, GdkAtom selection, GdkAtom target, GdkAtom property, guint32 time)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

gint gdk_string_to_compound_text_for_display (GdkDisplay *display, const gchar *str, GdkAtom *encoding, gint *format, guchar **ctext, gint *length)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

gint gdk_text_property_to_text_list_for_display (GdkDisplay *display, GdkAtom encoding, gint format, const guchar *text, gint length, gchar ***list)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

gint gdk_text_property_to_utf8_list_for_display (GdkDisplay *display, GdkAtom encoding, gint format, const guchar *text, gint length, gchar ***list)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

gchar * gdk_utf8_to_string_target (const gchar *str)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return NULL;
}

gboolean gdk_utf8_to_compound_text_for_display (GdkDisplay *display, const gchar *str, GdkAtom *encoding, gint *format, guchar **ctext, gint *length)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}
