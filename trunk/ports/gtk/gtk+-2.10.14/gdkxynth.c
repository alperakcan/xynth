
#include "gdkxynth.h"

const GOptionEntry _gdk_windowing_args[] = {
	{ NULL }
};

gint gdk_text_property_to_utf8_list_for_display (GdkDisplay *display, GdkAtom encoding, gint format, const guchar *text, gint length, gchar ***list)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

gboolean gdk_event_send_client_message_for_display (GdkDisplay *display, GdkEvent *event, guint32 xid)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

gboolean gdk_display_supports_cursor_alpha (GdkDisplay *display)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

GdkWindow * gdk_selection_owner_get_for_display (GdkDisplay *display, GdkAtom selection)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

GdkAtom gdk_atom_intern (const gchar *atom_name, gboolean only_if_exists)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_drop_finish (GdkDragContext *context, gboolean success, guint32 time)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void gdk_screen_broadcast_client_message (GdkScreen *screen, GdkEvent *sev)
{
	ENTER();
	ASSERT();
	LEAVE();
}

gint gdk_screen_get_height (GdkScreen *screen)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_device_set_key (GdkDevice *device, guint index, guint keyval, GdkModifierType modifiers)
{
	ENTER();
	ASSERT();
	LEAVE();
}

gboolean gdk_keymap_translate_keyboard_state (GdkKeymap *keymap, guint keycode, GdkModifierType state, gint group, guint *keyval, gint *effective_group, gint *level, GdkModifierType *consumed_modifiers)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

gboolean gdk_display_pointer_is_grabbed (GdkDisplay *display)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

GdkEvent * gdk_event_get_graphics_expose (GdkWindow *window)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_window_focus (GdkWindow *window, guint32 timestamp)
{
	ENTER();
	ASSERT();
	LEAVE();
}

gboolean gdk_property_get (GdkWindow *window, GdkAtom property, GdkAtom type, gulong offset, gulong length, gint pdelete, GdkAtom *actual_property_type, gint *actual_format_type, gint *actual_length, guchar **data)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

GList * gdk_display_list_devices (GdkDisplay *display)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

GdkGrabStatus gdk_keyboard_grab (GdkWindow *window, gboolean owner_events, guint32 time)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

gchar * gdk_atom_name (GdkAtom atom)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_window_configure_finished (GdkWindow *window)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void gdk_window_scroll (GdkWindow *window, gint dx, gint dy)
{
	ENTER();
	ASSERT();
	LEAVE();
}

gint gdk_window_get_origin (GdkWindow *window, gint *x, gint *y)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_window_set_skip_pager_hint (GdkWindow *window, gboolean skips_pager)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void gdk_device_set_axis_use (GdkDevice *device, guint index, GdkAxisUse use)
{
	ENTER();
	ASSERT();
	LEAVE();
}

gchar * gdk_set_locale (void)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

GdkGrabStatus gdk_pointer_grab (GdkWindow *window, gboolean owner_events, GdkEventMask event_mask, GdkWindow *confine_to, GdkCursor *cursor, guint32 time)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

GdkCursor * gdk_cursor_new_from_name (GdkDisplay  *display, const gchar *name)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_drag_abort (GdkDragContext *context, guint32 time)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void gdk_colormap_query_color (GdkColormap *colormap, gulong pixel, GdkColor *result)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void gdk_window_stick (GdkWindow *window)
{
	ENTER();
	ASSERT();
	LEAVE();
}

gboolean gdk_selection_owner_set_for_display (GdkDisplay *display, GdkWindow *owner, GdkAtom selection, guint32 time, gboolean send_event)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

gboolean gdk_display_supports_clipboard_persistence (GdkDisplay *display)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

gchar * gdk_utf8_to_string_target (const gchar *str)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

gboolean gdk_device_get_axis (GdkDevice *device, gdouble *axes, GdkAxisUse use, gdouble *value)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

GType gdk_image_get_type (void)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

GdkCursor * gdk_cursor_new_from_pixbuf (GdkDisplay *display, GdkPixbuf *pixbuf, gint x, gint y)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_window_move (GdkWindow *window, gint x, gint y)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void gdk_drag_drop (GdkDragContext *context, guint32 time)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void gdk_window_set_functions (GdkWindow *window, GdkWMFunction functions)
{
	ENTER();
	ASSERT();
	LEAVE();
}

GdkAtom gdk_drag_get_selection (GdkDragContext *context)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

gint gdk_screen_get_width (GdkScreen *screen)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

GdkDragContext * gdk_drag_begin (GdkWindow *window, GList *targets)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_window_maximize (GdkWindow *window)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void gdk_free_compound_text (guchar *ctext)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void _gdk_windowing_display_set_sm_client_id (GdkDisplay *display, const gchar *sm_client_id)
{
	ENTER();
	ASSERT();
	LEAVE();
}

gint _gdk_windowing_get_bits_for_depth (GdkDisplay *display, gint depth)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

void _gdk_windowing_get_pointer (GdkDisplay *display, GdkScreen **screen, gint *x, gint *y, GdkModifierType  *mask)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void _gdk_windowing_window_destroy (GdkWindow *window, gboolean recursing, gboolean foreign_destroy)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void gdk_window_withdraw (GdkWindow *window)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void gdk_window_set_urgency_hint (GdkWindow *window, gboolean urgent)
{
	ENTER();
	ASSERT();
	LEAVE();
}

GList * gdk_screen_list_visuals (GdkScreen *screen)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_display_flush (GdkDisplay *display)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void gdk_screen_get_monitor_geometry (GdkScreen *screen, gint monitor_num, GdkRectangle *dest)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void _gdk_cursor_destroy (GdkCursor *cursor)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void gdk_property_delete (GdkWindow *window, GdkAtom property)
{
	ENTER();
	ASSERT();
	LEAVE();
}

gboolean gdk_font_equal (const GdkFont *fonta, const GdkFont *fontb)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_window_register_dnd (GdkWindow *window)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void gdk_selection_convert (GdkWindow *requestor, GdkAtom selection, GdkAtom target, guint32 time)
{
	ENTER();
	ASSERT();
	LEAVE();
}

gboolean gdk_device_set_mode (GdkDevice *device, GdkInputMode mode)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_window_fullscreen (GdkWindow *window)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void gdk_display_sync (GdkDisplay *display)
{
	ENTER();
	ASSERT();
	LEAVE();
}

gint gdk_text_width (GdkFont *font, const gchar *text, gint text_length)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

gint gdk_mbstowcs (GdkWChar *dest, const gchar *src, gint dest_max)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

gboolean gdk_display_supports_cursor_color (GdkDisplay *display)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

gboolean gdk_screen_is_composited (GdkScreen *screen)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_window_iconify (GdkWindow *window)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void gdk_selection_send_notify_for_display (GdkDisplay *display, guint32 requestor, GdkAtom selection, GdkAtom target, GdkAtom property, guint32 time)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void gdk_input_set_extension_events (GdkWindow *window, gint mask, GdkExtensionMode mode)
{
	ENTER();
	ASSERT();
	LEAVE();
}

guint32 gdk_drag_get_protocol_for_display (GdkDisplay *display, guint32 xid, GdkDragProtocol *protocol)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_error_trap_push (void)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void gdk_window_get_geometry (GdkWindow *window, gint *x, gint *y, gint *width, gint *height, gint *depth)
{
	ENTER();
	ASSERT();
	LEAVE();
}

GdkAtom gdk_atom_intern_static_string (const gchar *atom_name)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_display_beep (GdkDisplay *display)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void  gdk_device_get_state (GdkDevice *device, GdkWindow *window, gdouble *axes, GdkModifierType *mask)
{
	ENTER();
	ASSERT();
	LEAVE();
}

gboolean gdk_utf8_to_compound_text_for_display (GdkDisplay *display, const gchar *str, GdkAtom *encoding, gint *format, guchar **ctext, gint *length)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

GdkWindow * _gdk_windowing_window_get_pointer (GdkDisplay *display, GdkWindow *window, gint *x, gint *y, GdkModifierType *mask)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_window_shape_combine_mask (GdkWindow *window, GdkBitmap *mask, gint x, gint y)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void gdk_window_set_back_pixmap (GdkWindow *window, GdkPixmap *pixmap, gboolean parent_relative)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void _gdk_events_queue (GdkDisplay *display)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void _gdk_windowing_window_clear_area_e (GdkWindow *window, gint x, gint y, gint width, gint height)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void _gdk_windowing_window_destroy_foreign (GdkWindow *window)
{
	ENTER();
	ASSERT();
	LEAVE();
}

GdkFont * gdk_font_load_for_display (GdkDisplay *display, const gchar *font_name)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

gboolean gdk_drag_motion (GdkDragContext *context, GdkWindow *dest_window, GdkDragProtocol protocol, gint x_root, gint y_root, GdkDragAction suggested_action, GdkDragAction possible_actions, guint32 time)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

GdkPixbuf * gdk_cursor_get_image (GdkCursor *cursor)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_drag_find_window_for_screen (GdkDragContext *context, GdkWindow *drag_window, GdkScreen *screen, gint x_root, gint y_root, GdkWindow **dest_window, GdkDragProtocol *protocol)
{
	ENTER();
	ASSERT();
	LEAVE();
}

guint32 gdk_image_get_pixel (GdkImage *image, gint x, gint y)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_display_store_clipboard (GdkDisplay *display, GdkWindow *clipboard_window, guint32 time_, GdkAtom *targets, gint n_targets)
{
	ENTER();
	ASSERT();
	LEAVE();
}

gboolean gdk_pointer_grab_info_libgtk_only (GdkDisplay *display, GdkWindow **grab_window, gboolean *owner_events)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

GdkScreen * gdk_visual_get_screen (GdkVisual *visual)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

GdkScreen * gdk_display_get_screen (GdkDisplay *display, gint screen_num)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

gint gdk_text_property_to_text_list_for_display (GdkDisplay *display, GdkAtom encoding, gint format, const guchar *text, gint length, gchar ***list)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

gint gdk_string_to_compound_text_for_display (GdkDisplay *display, const gchar *str, GdkAtom *encoding, gint *format, guchar **ctext, gint *length)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

gint gdk_error_trap_pop (void)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

void _gdk_font_destroy (GdkFont *font)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void _gdk_windowing_gc_copy (GdkGC *dst_gc, GdkGC *src_gc)
{
	ENTER();
	ASSERT();
	LEAVE();
}

gboolean gdk_keyboard_grab_info_libgtk_only (GdkDisplay *display, GdkWindow **grab_window, gboolean *owner_events)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_display_warp_pointer (GdkDisplay *display, GdkScreen *screen, gint x, gint y)
{
	ENTER();
	ASSERT();
	LEAVE();
}

gint gdk_font_id (const GdkFont *font)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

guint gdk_keyval_from_name (const gchar *keyval_name)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_window_reparent (GdkWindow *window, GdkWindow *new_parent, gint x, gint y)
{
	ENTER();
	ASSERT();
	LEAVE();
}

GdkPixmap * gdk_bitmap_create_from_data (GdkDrawable *drawable, const gchar *data, gint width, gint height)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

gint gdk_screen_get_width_mm (GdkScreen *screen)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_text_extents (GdkFont *font, const gchar *text, gint text_length, gint *lbearing, gint *rbearing, gint *width, gint *ascent, gint *descent)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void gdk_window_get_frame_extents (GdkWindow *window, GdkRectangle *rect)
{
	ENTER();
	ASSERT();
	LEAVE();
}

gboolean gdk_window_get_deskrelative_origin (GdkWindow *window, gint *x, gint *y)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

gchar * gdk_wcstombs (const GdkWChar *src)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

gint gdk_screen_get_n_monitors (GdkScreen *screen)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_window_set_transient_for (GdkWindow *window, GdkWindow *parent)
{
	ENTER();
	ASSERT();
	LEAVE();
}

gint gdk_text_width_wc (GdkFont *font, const GdkWChar *text, gint text_length)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

GdkDisplay * gdk_cursor_get_display (GdkCursor *cursor)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_window_set_role (GdkWindow *window, const gchar *role)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void gdk_window_set_decorations (GdkWindow *window, GdkWMDecoration decorations)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void gdk_window_show_unraised (GdkWindow *window)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void gdk_colormap_change (GdkColormap *colormap, gint ncolors)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void gdk_property_change (GdkWindow *window, GdkAtom property, GdkAtom type, gint format, GdkPropMode mode, const guchar *data, gint nelements)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void gdk_window_begin_resize_drag (GdkWindow *window, GdkWindowEdge edge, gint button, gint root_x, gint root_y, guint32 timestamp)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void gdk_colormap_free_colors (GdkColormap *colormap, GdkColor *colors, gint ncolors)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void gdk_window_set_skip_taskbar_hint (GdkWindow *window, gboolean skips_taskbar)
{
	ENTER();
	ASSERT();
	LEAVE();
}

GdkImage * _gdk_image_new_for_depth (GdkScreen *screen, GdkImageType type, GdkVisual *visual, gint width, gint height, gint depth)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

gint _gdk_font_strlen (GdkFont *font, const gchar *str)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

gint gdk_selection_property_get (GdkWindow *requestor, guchar **data, GdkAtom *ret_type, gint *ret_format)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

gchar * _gdk_windowing_substitute_screen_number (const gchar *display_name, gint screen_number)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

GdkCursor * gdk_cursor_new_from_pixmap (GdkPixmap *source, GdkPixmap *mask, const GdkColor *fg, const GdkColor *bg, gint x, gint y)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_flush (void)
{
	ENTER();
	ASSERT();
	LEAVE();
}

PangoDirection gdk_keymap_get_direction (GdkKeymap *keymap)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_window_set_title (GdkWindow *window, const gchar *title)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void gdk_window_hide (GdkWindow *window)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void gdk_display_get_maximal_cursor_size (GdkDisplay *display, guint *width, guint *height)
{
	ENTER();
	ASSERT();
	LEAVE();
}

GdkWindow * gdk_window_foreign_new_for_display (GdkDisplay *display, GdkNativeWindow anid)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

GdkEventMask gdk_window_get_events (GdkWindow *window)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

void  gdk_window_input_shape_combine_mask (GdkWindow *window, GdkBitmap *mask, gint x, gint y)
{
	ENTER();
	ASSERT();
	LEAVE();
}

gint gdk_screen_get_height_mm (GdkScreen *screen)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_drag_status (GdkDragContext *context, GdkDragAction action, guint32 time)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void gdk_display_keyboard_ungrab (GdkDisplay *display, guint32 time)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void gdk_window_lower (GdkWindow *window)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void gdk_window_begin_move_drag (GdkWindow *window, gint button, gint root_x, gint root_y, guint32 timestamp)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void gdk_window_move_resize (GdkWindow *window, gint x, gint y, gint width, gint height)
{
	ENTER();
	ASSERT();
	LEAVE();
}

gboolean gdk_display_request_selection_notification (GdkDisplay *display, GdkAtom selection)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_display_pointer_ungrab (GdkDisplay *display, guint32 time)
{
	ENTER();
	ASSERT();
	LEAVE();
}

gint gdk_display_get_n_screens (GdkDisplay *display)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_window_resize (GdkWindow *window, gint width, gint height)
{
	ENTER();
	ASSERT();
	LEAVE();
}

GdkFont * gdk_font_from_description_for_display (GdkDisplay *display, PangoFontDescription *font_desc)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

gboolean gdk_keymap_get_entries_for_keyval (GdkKeymap *keymap, guint keyval, GdkKeymapKey **keys, gint *n_keys)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

GdkWindow * gdk_window_lookup_for_display (GdkDisplay *display, GdkNativeWindow anid)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_drop_reply (GdkDragContext *context, gboolean ok, guint32 time)
{
	ENTER();
	ASSERT();
	LEAVE();
}

GdkKeymap * gdk_keymap_get_for_display (GdkDisplay *display)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

gchar * gdk_keyval_name (guint keyval)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

gboolean gdk_spawn_on_screen (GdkScreen *screen, const gchar *working_directory, gchar **argv, gchar **envp, GSpawnFlags flags, GSpawnChildSetupFunc child_setup, gpointer user_data, gint *child_pid, GError **error)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

gboolean gdk_keymap_get_entries_for_keycode (GdkKeymap *keymap, guint hardware_keycode, GdkKeymapKey **keys, guint **keyvals, gint *n_entries)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

gboolean gdk_display_supports_selection_notification (GdkDisplay *display)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

G_CONST_RETURN gchar * gdk_display_get_name (GdkDisplay *display)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_window_shape_combine_region (GdkWindow *window, GdkRegion *shape_region, gint offset_x, gint offset_y)
{
	ENTER();
	ASSERT();
	LEAVE();
}

GdkColormap * gdk_screen_get_rgba_colormap (GdkScreen *screen)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_window_set_icon_name (GdkWindow *window, const gchar *name)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void gdk_window_get_root_origin (GdkWindow *window, gint *x, gint *y)
{
	ENTER();
	ASSERT();
	LEAVE();
}

void gdk_device_free_history (GdkTimeCoord **events, gint n_events)
{
	ENTER();
	ASSERT();
	LEAVE();
}

gboolean gdk_device_get_history (GdkDevice *device, GdkWindow *window, guint32 start, guint32 stop, GdkTimeCoord ***events, gint *n_events)
{
	ENTER();
	ASSERT();
	LEAVE();
	return 0;
}
