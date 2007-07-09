
#include <config.h>
#include "gdk.h"
#include "gdkwindow.h"

#include "gdkregion-generic.h"

#include "gdkinternals.h"
#include "gdkalias.h"

#include "gdkxynth.h"

GType _gdk_window_impl_get_type (void)
{
	return 0;
}

GdkWindow * _gdk_windowing_window_at_pointer (GdkDisplay *display, gint *win_x, gint *win_y)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return NULL;
}

void _gdk_windowing_window_clear_area (GdkWindow *window, gint x, gint y, gint width, gint height)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void _gdk_windowing_window_clear_area_e (GdkWindow *window, gint x, gint y, gint width, gint height)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void _gdk_windowing_window_destroy (GdkWindow *window, gboolean recursing, gboolean foreign_destroy)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void _gdk_windowing_window_destroy_foreign (GdkWindow *window)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

GdkWindow * _gdk_windowing_window_get_pointer (GdkDisplay *display, GdkWindow *window, gint *x, gint *y, GdkModifierType *mask)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return NULL;
}

void _gdk_windowing_get_pointer (GdkDisplay *display, GdkScreen **screen, gint *x, gint *y, GdkModifierType *mask)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_display_warp_pointer (GdkDisplay *display, GdkScreen *screen, gint x, gint y)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_begin_move_drag (GdkWindow *window, gint button, gint root_x, gint root_y, guint32 timestamp)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_begin_resize_drag (GdkWindow *window, GdkWindowEdge edge, gint button, gint root_x, gint root_y, guint32 timestamp)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_configure_finished (GdkWindow *window)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_deiconify (GdkWindow *window)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_enable_synchronized_configure (GdkWindow *window)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_focus (GdkWindow *window, guint32 timestamp)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

GdkWindow * gdk_window_foreign_new_for_display (GdkDisplay* display,GdkNativeWindow anid)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return NULL;
}

void gdk_window_fullscreen (GdkWindow *window)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

gboolean gdk_window_get_deskrelative_origin (GdkWindow *window, gint *x, gint *y)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

GdkEventMask gdk_window_get_events (GdkWindow *window)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_window_get_frame_extents (GdkWindow *window, GdkRectangle *rect)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_get_geometry (GdkWindow *window, gint *x, gint *y, gint *width, gint *height, gint *depth)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

gboolean gdk_window_get_origin (GdkWindow *window, gint *x, gint *y)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return 0;
}

void gdk_window_hide (GdkWindow *window)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_iconify (GdkWindow *window)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_input_shape_combine_mask (GdkWindow *window, GdkBitmap *mask, gint x, gint y)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

GdkWindow * gdk_window_lookup_for_display (GdkDisplay *display, GdkNativeWindow anid)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return NULL;
}

void gdk_window_lower (GdkWindow *window)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_maximize (GdkWindow *window)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_move (GdkWindow *window, gint x, gint y)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_move_resize (GdkWindow *window, gint x, gint y, gint width, gint height)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

GdkWindow * gdk_window_new (GdkWindow *parent, GdkWindowAttr *attributes, gint attributes_mask)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
	return NULL;
}

void gdk_window_raise (GdkWindow *window)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_reparent (GdkWindow *window, GdkWindow *new_parent, gint x, gint y)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_resize (GdkWindow *window, gint width, gint height)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_set_accept_focus (GdkWindow *window, gboolean accept_focus)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_set_back_pixmap (GdkWindow *window, GdkPixmap *pixmap, gint parent_relative)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_set_background (GdkWindow *window, const GdkColor  *color)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_set_cursor (GdkWindow *window, GdkCursor *cursor)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_set_decorations (GdkWindow *window, GdkWMDecoration decorations)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_set_events (GdkWindow *window, GdkEventMask event_mask)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_set_focus_on_map (GdkWindow *window, gboolean focus_on_map)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_set_functions (GdkWindow *window, GdkWMFunction functions)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_set_geometry_hints (GdkWindow *window, GdkGeometry *geometry, GdkWindowHints geom_mask)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_set_icon (GdkWindow *window, GdkWindow *icon_window, GdkPixmap *pixmap, GdkBitmap *mask)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_set_icon_list (GdkWindow *window, GList *pixbufs)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_set_keep_above (GdkWindow *window, gboolean setting)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_set_keep_below (GdkWindow *window, gboolean setting)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_set_modal_hint (GdkWindow *window, gboolean modal)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_set_role (GdkWindow *window, const gchar *role)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_set_skip_pager_hint (GdkWindow *window, gboolean skips_pager)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_set_skip_taskbar_hint (GdkWindow *window, gboolean skips_taskbar)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_set_title (GdkWindow *window, const gchar *title)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_set_transient_for (GdkWindow *window, GdkWindow *parent)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_set_type_hint (GdkWindow *window, GdkWindowTypeHint hint)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_set_urgency_hint (GdkWindow *window, gboolean urgent)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_shape_combine_mask (GdkWindow *window, GdkBitmap *mask, gint x, gint y)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_show_unraised (GdkWindow *window)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_show (GdkWindow *window)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_stick (GdkWindow *window)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_unfullscreen (GdkWindow *window)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_unmaximize (GdkWindow *window)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_unstick (GdkWindow *window)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_withdraw (GdkWindow *window)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_get_root_origin (GdkWindow *window, gint *x, gint *y)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_shape_combine_region (GdkWindow *window, GdkRegion *shape_region, gint offset_x, gint offset_y)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_set_icon_name (GdkWindow *window, const gchar *name)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}
