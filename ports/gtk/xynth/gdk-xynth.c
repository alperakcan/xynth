
#include "gdk-xynth.h"

gchar * gdk_atom_name (GdkAtom atom)
{
	NIY();
}

GdkPixmap * gdk_bitmap_create_from_data (GdkDrawable *drawable, const gchar *data, gint width, gint height)
{
	NIY();
}


void gdk_colormap_free_colors (GdkColormap *colormap, GdkColor *colors, gint ncolors)
{
	NIY();
}

void gdk_colormap_query_color (GdkColormap *colormap, gulong pixel, GdkColor *result)
{
	NIY();
}

void _gdk_cursor_destroy (GdkCursor *cursor)
{
	NIY();
}

GdkDisplay * gdk_cursor_get_display (GdkCursor *cursor)
{
	NIY();
}

GdkCursor * gdk_cursor_new_for_display (GdkDisplay *display, GdkCursorType cursor_type)
{
	NIY();
}

GdkCursor * gdk_cursor_new_from_pixmap (GdkPixmap *source, GdkPixmap *mask, const GdkColor *fg, const GdkColor *bg, gint x, gint y)
{
	NIY();
}

void gdk_device_set_axis_use (GdkDevice *device, guint index, GdkAxisUse use)
{
	NIY();
}

void gdk_device_set_key (GdkDevice *device, guint index, guint keyval, GdkModifierType modifiers)
{
	NIY();
}

gboolean gdk_device_set_mode (GdkDevice *device, GdkInputMode mode)
{
	NIY();
}

void gdk_display_beep (GdkDisplay * display)
{
	NIY();
}

gint gdk_display_get_n_screens (GdkDisplay * display)
{
	NIY();
}

GdkScreen * gdk_display_get_screen (GdkDisplay * display, gint screen_num)
{
	NIY();
}

void gdk_display_keyboard_ungrab (GdkDisplay *display, guint32 time)
{
	NIY();
}

GList * gdk_display_list_devices (GdkDisplay *display)
{
	NIY();
}

gboolean gdk_display_pointer_is_grabbed (GdkDisplay * display)
{
	NIY();
}

void gdk_display_pointer_ungrab (GdkDisplay *display, guint32 time)
{
	NIY();
}

gboolean gdk_display_request_selection_notification (GdkDisplay *display, GdkAtom selection)
{
	NIY();
}

void gdk_display_store_clipboard (GdkDisplay *display, GdkWindow  *clipboard_window, guint32 time_, GdkAtom *targets, gint n_targets)
{
	NIY();
}

gboolean gdk_display_supports_clipboard_persistence (GdkDisplay *display)
{
	NIY();
}

gboolean gdk_display_supports_selection_notification (GdkDisplay *display)
{
	NIY();
}

void gdk_display_sync (GdkDisplay * display)
{
	NIY();
}

void gdk_drag_abort (GdkDragContext *context, guint32 time)
{
	NIY();
}

GdkDragContext * gdk_drag_begin (GdkWindow *window, GList *targets)
{
	NIY();
}

void gdk_drag_drop (GdkDragContext *context, guint32 time)
{
	NIY();
}

void gdk_drag_find_window_for_screen (GdkDragContext *context, GdkWindow *drag_window, GdkScreen *screen, gint x_root, gint y_root, GdkWindow **dest_window, GdkDragProtocol *protocol)
{
	NIY();
}

GdkAtom gdk_drag_get_selection (GdkDragContext *context)
{
	NIY();
}

gboolean gdk_drag_motion (GdkDragContext *context, GdkWindow *dest_window, GdkDragProtocol protocol, gint x_root, gint y_root, GdkDragAction suggested_action, GdkDragAction possible_actions, guint32 time)
{
	NIY();
}

void gdk_drag_status (GdkDragContext *context, GdkDragAction action, guint32 time)
{
	NIY();
}

gboolean gdk_draw_rectangle_alpha_libgtk_only (GdkDrawable *drawable, gint x, gint y, gint width, gint height, GdkColor *color, guint16 alpha)
{
	NIY();
}

void gdk_drop_finish (GdkDragContext *context, gboolean success, guint32 time)
{
	NIY();
}

void gdk_drop_reply (GdkDragContext *context, gboolean ok, guint32 time)
{
	NIY();
}

gint gdk_error_trap_pop (void)
{
	NIY();
}

void gdk_error_trap_push (void)
{
	NIY();
}

GdkEvent * gdk_event_get_graphics_expose (GdkWindow *window)
{
	NIY();
}

gboolean gdk_event_send_client_message_for_display (GdkDisplay *display, GdkEvent *event, GdkNativeWindow winid)
{
	NIY();
}

void _gdk_events_queue (GdkDisplay *display)
{
	NIY();
}

void gdk_flush (void)
{
	NIY();
}

gboolean gdk_font_equal (const GdkFont *fonta, const GdkFont *fontb)
{
	NIY();
}

GdkFont * gdk_font_from_description_for_display (GdkDisplay *display, PangoFontDescription *font_desc)
{
	NIY();
}

gint gdk_font_id (const GdkFont *font)
{
	NIY();
}

GdkFont * gdk_font_load_for_display (GdkDisplay *display, const gchar *font_name)
{
	NIY();
}

void gdk_free_compound_text (guchar *ctext)
{
	NIY();
}

void gdk_gc_copy (GdkGC *dst_gc, GdkGC *src_gc)
{
	NIY();
}

guint32 gdk_image_get_pixel (GdkImage *image, gint x, gint y)
{
	NIY();
}

GType gdk_image_get_type (void)
{
	NIY();
}

GdkImage * _gdk_image_new_for_depth (GdkScreen *screen, GdkImageType type, GdkVisual *visual, gint width, gint height, gint depth)
{
	NIY();
}

void gdk_input_set_extension_events (GdkWindow *window, gint mask, GdkExtensionMode mode)
{
	NIY();
}

GdkGrabStatus gdk_keyboard_grab (GdkWindow *window, gboolean owner_events, guint32 time)
{
	NIY();
}

gboolean gdk_keyboard_grab_info_libgtk_only (GdkDisplay *display, GdkWindow **grab_window, gboolean *owner_events)
{
	NIY();
}

PangoDirection gdk_keymap_get_direction (GdkKeymap *keymap)
{
	NIY();
}

gboolean gdk_keymap_get_entries_for_keycode (GdkKeymap *keymap, guint hardware_keycode, GdkKeymapKey **keys, guint **keyvals, gint *n_entries)
{
	NIY();
}

gboolean gdk_keymap_get_entries_for_keyval (GdkKeymap *keymap, guint keyval, GdkKeymapKey **keys, gint *n_keys)
{
	NIY();
}

GdkKeymap * gdk_keymap_get_for_display (GdkDisplay *display)
{
	NIY();
}

gboolean gdk_keymap_translate_keyboard_state (GdkKeymap *keymap, guint hardware_keycode, GdkModifierType state, gint group, guint *keyval, gint *effective_group, gint *level, GdkModifierType *consumed_modifiers)
{
	NIY();
}

guint gdk_keyval_from_name (const gchar *keyval_name)
{
	NIY();
}

gchar * gdk_keyval_name (guint keyval)
{
	NIY();
}

gint gdk_mbstowcs (GdkWChar *dest, const gchar *src, gint dest_max)
{
	NIY();
}

GdkGrabStatus gdk_pointer_grab (GdkWindow *window, gboolean owner_events, GdkEventMask event_mask, GdkWindow *confine_to, GdkCursor *cursor, guint32 time)
{
	NIY();
}

gboolean gdk_pointer_grab_info_libgtk_only (GdkDisplay *display, GdkWindow **grab_window, gboolean *owner_events)
{
	NIY();
}

void gdk_property_change (GdkWindow *window, GdkAtom property, GdkAtom type, gint format, GdkPropMode mode, const guchar *data, gint nelements)
{
	NIY();
}

void gdk_property_delete (GdkWindow *window, GdkAtom property)
{
	NIY();
}

gboolean gdk_property_get (GdkWindow *window, GdkAtom property, GdkAtom type, gulong offset, gulong length, gint pdelete, GdkAtom *actual_property_type, gint *actual_format_type, gint *actual_length, guchar **data)
{
	NIY();
}

void gdk_screen_broadcast_client_message (GdkScreen *screen, GdkEvent *event)
{
	NIY();
}

gint gdk_screen_get_height (GdkScreen *screen)
{
	NIY();
}

void gdk_screen_get_monitor_geometry (GdkScreen *screen, gint monitor_num, GdkRectangle *dest)
{
	NIY();
}

gint gdk_screen_get_n_monitors (GdkScreen *screen)
{
	NIY();
}

gint gdk_screen_get_width (GdkScreen *screen)
{
	NIY();
}

void gdk_selection_convert (GdkWindow *requestor, GdkAtom selection, GdkAtom target, guint32 time)
{
	NIY();
}

GdkWindow * gdk_selection_owner_get_for_display (GdkDisplay *display, GdkAtom selection)
{
	NIY();
}

gboolean gdk_selection_owner_set_for_display (GdkDisplay *display, GdkWindow *owner, GdkAtom selection, guint32 time, gboolean send_event)
{
	NIY();
}

gint gdk_selection_property_get (GdkWindow *requestor, guchar **data, GdkAtom *ret_type, gint *ret_format)
{
	NIY();
}

void gdk_selection_send_notify_for_display (GdkDisplay *display, guint32 requestor, GdkAtom selection, GdkAtom target, GdkAtom property, guint32 time)
{
	NIY();
}

gchar * gdk_set_locale (void)
{
	NIY();
}

gint gdk_text_property_to_utf8_list_for_display (GdkDisplay *display, GdkAtom encoding, gint format, const guchar *text, gint length, gchar ***list)
{
	NIY();
}

gint gdk_text_width (GdkFont *font, const gchar *text, gint text_length)
{
	NIY();
}

gint gdk_text_width_wc (GdkFont *font, const GdkWChar *text, gint text_length)
{
	NIY();
}

gboolean gdk_utf8_to_compound_text_for_display (GdkDisplay *display, const gchar *str, GdkAtom *encoding, gint *format, guchar **ctext, gint *length)
{
	NIY();
}

gchar * gdk_utf8_to_string_target (const gchar *str)
{
	NIY();
}

gchar * gdk_wcstombs (const GdkWChar *src)
{
	NIY();
}

void gdk_window_begin_move_drag (GdkWindow *window, gint button, gint root_x, gint root_y, guint32 timestamp)
{
	NIY();
}

void gdk_window_begin_resize_drag (GdkWindow *window, GdkWindowEdge edge, gint button, gint root_x, gint root_y, guint32 timestamp)
{
	NIY();
}

void gdk_window_focus (GdkWindow *window, guint32 timestamp)
{
	NIY();
}

GdkWindow * gdk_window_foreign_new_for_display (GdkDisplay *display, GdkNativeWindow anid)
{
	NIY();
}

void gdk_window_fullscreen (GdkWindow *window)
{
	NIY();
}

gboolean gdk_window_get_deskrelative_origin (GdkWindow *window, gint *x, gint *y)
{
	NIY();
}

GdkEventMask gdk_window_get_events (GdkWindow *window)
{
	NIY();
}

void gdk_window_get_frame_extents (GdkWindow *window, GdkRectangle *rect)
{
	NIY();
}

void gdk_window_get_geometry (GdkWindow *window, gint *x, gint *y, gint *width, gint *height, gint *depth)
{
	NIY();
}

gint gdk_window_get_origin (GdkWindow *window, gint *x, gint *y)
{
	NIY();
}

void gdk_window_hide (GdkWindow *window)
{
	NIY();
}

void gdk_window_iconify (GdkWindow *window)
{
	NIY();
}

GdkWindow * gdk_window_lookup_for_display (GdkDisplay *display, GdkNativeWindow anid)
{
	NIY();
}

void gdk_window_lower (GdkWindow *window)
{
	NIY();
}

void gdk_window_maximize (GdkWindow *window)
{
	NIY();
}

void gdk_window_move (GdkWindow *window, gint x, gint y)
{
	NIY();
}

void gdk_window_raise (GdkWindow *window)
{
	NIY();
}

void gdk_window_register_dnd (GdkWindow *window)
{
	NIY();
}

void gdk_window_reparent (GdkWindow *window, GdkWindow *new_parent, gint x, gint y)
{
	NIY();
}

void gdk_window_resize (GdkWindow *window, gint width, gint height)
{
	NIY();
}

void gdk_window_scroll (GdkWindow *window, gint dx, gint dy)
{
	NIY();
}

void gdk_window_set_back_pixmap (GdkWindow *window, GdkPixmap *pixmap, gboolean parent_relative)
{
	NIY();
}

void gdk_window_set_cursor (GdkWindow *window, GdkCursor *cursor)
{
	ENT();
	DBG("What da fuck");
	LEV();
//	NIY();
}

void gdk_window_set_decorations (GdkWindow *window, GdkWMDecoration decorations)
{
	NIY();
}

void gdk_window_set_role (GdkWindow *window, const gchar *role)
{
	NIY();
}

void gdk_window_set_skip_pager_hint (GdkWindow *window, gboolean skips_pager)
{
	NIY();
}

void gdk_window_set_skip_taskbar_hint (GdkWindow *window, gboolean skips_taskbar)
{
	NIY();
}

void gdk_window_set_transient_for (GdkWindow *window, GdkWindow *parent)
{
	NIY();
}

void gdk_window_shape_combine_mask (GdkWindow *window, GdkBitmap *mask, gint x, gint y)
{
	NIY();
}

void gdk_window_show_unraised (GdkWindow *window)
{
	NIY();
}

void gdk_window_stick (GdkWindow *window)
{
	NIY();
}

void gdk_window_withdraw (GdkWindow *window)
{
	NIY();
}

void _gdk_windowing_window_destroy_foreign (GdkWindow *window)
{
	NIY();
}

void _gdk_windowing_display_set_sm_client_id (GdkDisplay  *display, const gchar *sm_client_id)
{
	NIY();
}

gint _gdk_windowing_get_bits_for_depth (GdkDisplay *display, gint depth)
{
	NIY();
}

gint gdk_text_property_to_text_list_for_display (GdkDisplay *display, GdkAtom encoding, gint format, const guchar *text, gint length, gchar ***list)
{
	NIY();
}

gint gdk_screen_get_width_mm (GdkScreen *screen)
{
	NIY();
}

GList * gdk_screen_list_visuals (GdkScreen *screen)
{
	NIY();
}

void _gdk_windowing_window_clear_area_e (GdkWindow *window, gint x, gint y, gint width, gint height)
{
	NIY();
}

void _gdk_windowing_gc_get_foreground (GdkGC *gc, GdkColor *color)
{
	NIY();
}

GdkWindow * _gdk_windowing_window_get_pointer (GdkDisplay *display, GdkWindow *window, gint *x, gint *y, GdkModifierType *mask)
{
	NIY();
}

void _gdk_windowing_window_clear_area (GdkWindow *window, gint x, gint y, gint width, gint height)
{
	NIY();
}

GOptionEntry _gdk_windowing_args[] = {
  { NULL }
};

GdkWindow * _gdk_windowing_window_at_pointer (GdkDisplay *display, gint *win_x, gint *win_y)
{
	NIY();
}

void _gdk_windowing_window_destroy (GdkWindow *window, gboolean recursing, gboolean foreign_destroy)
{
	NIY();
}

gint gdk_screen_get_height_mm (GdkScreen *screen)
{
	NIY();
}

GdkColormap * gdk_colormap_new (GdkVisual *visual, gboolean allocate)
{
	NIY();
}

guint32 gdk_drag_get_protocol_for_display (GdkDisplay *display, guint32 xid, GdkDragProtocol *protocol)
{
	NIY();
}

GdkScreen * gdk_visual_get_screen (GdkVisual *visual)
{
	NIY();
}

void _gdk_windowing_get_pointer (GdkDisplay *display, GdkScreen **screen, gint *x, gint *y, GdkModifierType *mask)
{
	NIY();
}

gint gdk_string_to_compound_text_for_display (GdkDisplay *display, const gchar *str, GdkAtom *encoding, gint *format, guchar **ctext, gint *length)
{
	NIY();
}

gint _gdk_font_strlen (GdkFont *font, const gchar *str)
{
	NIY();
}

void gdk_text_extents (GdkFont *font, const gchar *text, gint text_length, gint *lbearing, gint *rbearing, gint *width, gint *ascent, gint *descent)
{
	NIY();
}

void _gdk_font_destroy (GdkFont *font)
{
	NIY();
}

gchar * _gdk_windowing_substitute_screen_number (const gchar *display_name, gint screen_number)
{
	NIY();
}

gboolean gdk_device_get_axis (GdkDevice *device, gdouble *axes, GdkAxisUse use, gdouble *value)
{
	NIY();
}

G_CONST_RETURN gchar * gdk_display_get_name (GdkDisplay * display)
{
	NIY();
}

void gdk_window_get_root_origin (GdkWindow *window, gint *x, gint *y)
{
	NIY();
}

void gdk_window_shape_combine_region (GdkWindow *window, GdkRegion *shape_region, gint offset_x, gint offset_y)
{
	NIY();
}

void gdk_window_set_icon_name (GdkWindow *window, const gchar *name)
{
	NIY();
}

void gdk_window_set_functions (GdkWindow *window, GdkWMFunction functions)
{
	NIY();
}

gboolean gdk_device_get_history (GdkDevice *device, GdkWindow *window, guint32 start, guint32 stop, GdkTimeCoord ***events, gint *n_events)
{
	NIY();
}

void gdk_device_free_history (GdkTimeCoord **events, gint n_events)
{
	NIY();
}

void gdk_device_get_state (GdkDevice *device, GdkWindow *window, gdouble *axes, GdkModifierType *mask)
{
	NIY();
}
