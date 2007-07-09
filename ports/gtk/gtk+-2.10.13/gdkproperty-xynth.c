
#include <config.h>
#include "gdk.h"

#include "gdkproperty.h"
#include "gdkinternals.h"
#include "gdkalias.h"

#include "gdkxynth.h"

GdkAtom gdk_atom_intern (const gchar *atom_name, gboolean only_if_exists)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return NULL;
}

GdkAtom gdk_atom_intern_static_string (const gchar *atom_name)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return NULL;
}

gchar * gdk_atom_name (GdkAtom atom)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return NULL;
}

void gdk_property_change (GdkWindow *window, GdkAtom property, GdkAtom type, gint format, GdkPropMode mode, const guchar *data, gint nelements)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_property_delete (GdkWindow *window, GdkAtom property)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

gboolean gdk_property_get (GdkWindow *window, GdkAtom property, GdkAtom type, gulong offset, gulong length, gint pdelete, GdkAtom *actual_property_type, gint *actual_format_type, gint *actual_length, guchar **data)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}
