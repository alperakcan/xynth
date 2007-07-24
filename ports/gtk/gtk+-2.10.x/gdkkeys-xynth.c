
#include <config.h>
#include "gdk.h"

#include "gdkkeysyms.h"
#include "gdkalias.h"

#include "gdkxynth.h"

GdkModifierType _gdk_xynth_modifiers = 0;

PangoDirection gdk_keymap_get_direction (GdkKeymap *keymap)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return PANGO_DIRECTION_NEUTRAL;
}

gboolean gdk_keymap_get_entries_for_keycode (GdkKeymap *keymap, guint hardware_keycode, GdkKeymapKey **keys, guint **keyvals, gint *n_entries)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

gboolean gdk_keymap_get_entries_for_keyval (GdkKeymap *keymap, guint keyval, GdkKeymapKey **keys, gint *n_keys)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

GdkKeymap * gdk_keymap_get_for_display (GdkDisplay *display)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return NULL;
}

gboolean gdk_keymap_translate_keyboard_state (GdkKeymap *keymap, guint keycode, GdkModifierType state, gint group, guint *keyval, gint *effective_group, gint *level, GdkModifierType *consumed_modifiers)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

guint gdk_keyval_from_name (const gchar *keyval_name)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

gchar * gdk_keyval_name (guint keyval)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return NULL;
}
