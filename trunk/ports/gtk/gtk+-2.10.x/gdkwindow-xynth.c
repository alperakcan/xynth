
#include <config.h>
#include "gdk.h"
#include "gdkwindow.h"

#include "gdkregion-generic.h"

#include "gdkinternals.h"
#include "gdkalias.h"

#include "gdkxynth.h"
#include "gdkprivate-xynth.h"

GdkWindow *_gdk_parent_root  = NULL;
static gpointer parent_class = NULL;

GdkWindow * _gdk_windowing_window_at_pointer (GdkDisplay *display, gint *win_x, gint *win_y)
{
	gint wx;
	gint wy;
	GdkWindow *retval;
	ENTER();
	if (!win_x || !win_y) {
		gdk_xynth_mouse_get_info(&wx, &wy, NULL);
	}
	if (win_x) {
		wx = *win_x;
	}
	if (win_y) {
		wy = *win_y;
	}
	retval = gdk_xynth_child_at(_gdk_parent_root, &wx, &wy);
	if (win_x) {
		*win_x = wx;
	}
	if (win_y) {
		*win_y = wy;
	}
	LEAVE();
	return retval;
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
	LEAVE();
}

void gdk_window_focus (GdkWindow *window, guint32 timestamp)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

GdkWindow * gdk_window_foreign_new_for_display (GdkDisplay* display, GdkNativeWindow anid)
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

void _gdk_xynth_calc_abs (GdkWindow *window)
{
	GdkWindowObject      *private;
	GdkDrawableImplXYNTH *impl;
	GList                *list;
	ENTER();
	if (GDK_IS_WINDOW(window) == 0) {
		LEAVE();
		return;
	}
	private = GDK_WINDOW_OBJECT(window);
	impl = GDK_DRAWABLE_IMPL_XYNTH(private->impl);
	
	impl->abs_x = private->x;
	impl->abs_y = private->y;
	
	if (private->parent) {
		GdkDrawableImplXYNTH *parent_impl = GDK_DRAWABLE_IMPL_XYNTH(GDK_WINDOW_OBJECT(private->parent)->impl);
		impl->abs_x += parent_impl->abs_x;
		impl->abs_y += parent_impl->abs_y;
	}
	for (list = private->children; list; list = list->next) {
		_gdk_xynth_calc_abs(list->data);
	}	
	LEAVE();
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

void gdk_xynth_window_send_crossing_events (GdkWindow *src, GdkWindow *dest, GdkCrossingMode  mode)
{
	ENTER();
	NIY();
	ASSERT();
	LEAVE();
}

void gdk_window_set_cursor (GdkWindow *window, GdkCursor *cursor)
{
	GdkWindowImplXYNTH *impl;
	GdkCursor          *old_cursor;
	ENTER();
	if (GDK_IS_WINDOW(window) == 0) {
		LEAVE();
	}
	impl = GDK_WINDOW_IMPL_XYNTH(GDK_WINDOW_OBJECT(window)->impl);
	old_cursor = impl->cursor;
	impl->cursor = (cursor ? gdk_cursor_ref(cursor) : gdk_cursor_new(GDK_LEFT_PTR));
	if (gdk_window_at_pointer(NULL, NULL) == window) {
		/* This is a bit evil but we want to keep all cursor changes in
		 * one place, so let gdk_xynth_window_send_crossing_events
		 * do the work for us. */
		gdk_xynth_window_send_crossing_events(window, window, GDK_CROSSING_NORMAL);
	} else if (impl->xynth_window) {
		GdkCursorXYNTH *xynth_cursor = (GdkCursorXYNTH *) impl->cursor;
		/* this branch takes care of setting the cursor for unmapped windows */
		NIY();
	}
	if (old_cursor) {
		gdk_cursor_unref(old_cursor);
	}
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
	if (GDK_IS_WINDOW(window) == 0) {
		LEAVE();
		return;
	}
	if (event_mask & GDK_BUTTON_MOTION_MASK) {
		event_mask |= (GDK_BUTTON1_MOTION_MASK | GDK_BUTTON2_MOTION_MASK | GDK_BUTTON3_MOTION_MASK);
	}
	GDK_WINDOW_OBJECT(window)->event_mask = event_mask;
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
	if (GDK_IS_WINDOW(window) == 0) {
		LEAVE();
		return;
	}
	if (GDK_WINDOW_DESTROYED(window)) {
		LEAVE();
		return;
	}
	DEBUG("gdk_window_set_type_hint: %d", hint);
	((GdkWindowImplXYNTH *) ((GdkWindowObject *) window)->impl)->type_hint = hint;
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

static void gdk_window_impl_xynth_process_updates (GdkPaintable *paintable, gboolean update_children)
{
	GdkWindow *window;
	GdkWindowObject *private;
	GdkWindowImplXYNTH *wimpl;
	GdkDrawableImplXYNTH *impl;
	ENTER();
	wimpl = GDK_WINDOW_IMPL_XYNTH(paintable);
	impl = (GdkDrawableImplXYNTH *) wimpl;
	window = wimpl->gdkWindow;
	private = (GdkWindowObject *)window;
	NIY();
	ASSERT();
	LEAVE();
}

static void gdk_window_impl_xynth_invalidate_maybe_recurse (GdkPaintable *paintable, GdkRegion *region, gboolean (*child_func) (GdkWindow *, gpointer), gpointer user_data)
{
	GdkWindow *window;
	GdkWindowObject *private;
	GdkWindowImplXYNTH *wimpl;
	GdkDrawableImplXYNTH *impl;
	ENTER();
	wimpl = GDK_WINDOW_IMPL_XYNTH(paintable);
	impl = (GdkDrawableImplXYNTH *) wimpl;
	window = wimpl->gdkWindow;
	private = (GdkWindowObject *) window;
	NIY();
	ASSERT();
	LEAVE();
}

static void gdk_window_impl_xynth_end_paint (GdkPaintable *paintable)
{
	GdkDrawableImplXYNTH *impl;
	ENTER();
	impl = GDK_DRAWABLE_IMPL_XYNTH(paintable);
	NIY();
	ASSERT();
	LEAVE();
}

static void gdk_window_impl_xynth_begin_paint_region (GdkPaintable *paintable, GdkRegion *region)
{
	GdkDrawableImplXYNTH *impl;
	GdkWindowImplXYNTH *wimpl;
	ENTER();
	g_assert(region != NULL);
	wimpl = GDK_WINDOW_IMPL_XYNTH(paintable);
	impl = (GdkDrawableImplXYNTH *) wimpl;
	NIY();
	ASSERT();
	LEAVE();
}

static void gdk_window_impl_xynth_paintable_init (GdkPaintableIface *iface)
{
	ENTER();
	iface->begin_paint_region = gdk_window_impl_xynth_begin_paint_region;
	iface->end_paint = gdk_window_impl_xynth_end_paint;
	iface->invalidate_maybe_recurse = gdk_window_impl_xynth_invalidate_maybe_recurse;
	iface->process_updates = gdk_window_impl_xynth_process_updates;
	LEAVE();
}

static void gdk_window_impl_xynth_init (GdkWindowImplXYNTH *impl)
{
	ENTER();
	impl->drawable.width = 1;
	impl->drawable.height = 1;
	impl->cursor = gdk_cursor_new_for_display(GDK_DISPLAY_OBJECT(_gdk_display), GDK_LEFT_PTR);
	LEAVE();
}

static GdkRegion * gdk_window_impl_xynth_get_visible_region (GdkDrawable *drawable)
{
	GdkRegion *reg;
	GdkRectangle rect = {0, 0, 0, 0};
	GdkDrawableImplXYNTH *priv = GDK_DRAWABLE_IMPL_XYNTH(drawable);
	ENTER();
	DEBUG("Set region rectangle");
	reg = gdk_region_rectangle(&rect);
	LEAVE();
	return reg;
}

static void gdk_window_impl_xynth_set_colormap (GdkDrawable *drawable, GdkColormap *colormap)
{
	ENTER();
	GDK_DRAWABLE_CLASS(parent_class)->set_colormap(drawable, colormap);
	LEAVE();
}

static void gdk_window_impl_xynth_finalize (GObject *object)
{
	GdkWindowImplXYNTH *impl = GDK_WINDOW_IMPL_XYNTH(object);
	ENTER();
	if (GDK_WINDOW_IS_MAPPED(impl->drawable.wrapper)) {
		gdk_window_hide(impl->drawable.wrapper);
	}
	if (impl->cursor) {
		gdk_cursor_unref(impl->cursor);
	}
	DEBUG("Release any window impl allocations to avoid memory leaks!");
	if (G_OBJECT_CLASS(parent_class)->finalize) {
		G_OBJECT_CLASS(parent_class)->finalize(object);
	}
	LEAVE();
}

static void gdk_window_impl_xynth_class_init (GdkWindowImplXYNTHClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	GdkDrawableClass *drawable_class = GDK_DRAWABLE_CLASS(klass);
	ENTER();
	parent_class = g_type_class_peek_parent(klass);
	object_class->finalize = gdk_window_impl_xynth_finalize;
	drawable_class->set_colormap = gdk_window_impl_xynth_set_colormap;
	/* Visible and clip regions are the same */
	drawable_class->get_clip_region = gdk_window_impl_xynth_get_visible_region;
	drawable_class->get_visible_region = gdk_window_impl_xynth_get_visible_region;
	LEAVE();
}

GType gdk_window_impl_xynth_get_type (void)
{
	static GType object_type = 0;
	ENTER();
	if (!object_type) {
		static const GTypeInfo object_info = {
			sizeof(GdkWindowImplXYNTHClass),
			(GBaseInitFunc) NULL,
			(GBaseFinalizeFunc) NULL,
			(GClassInitFunc) gdk_window_impl_xynth_class_init,
			NULL,           /* class_finalize */
			NULL,           /* class_data */
			sizeof (GdkWindowImplXYNTH),
			0,              /* n_preallocs */
			(GInstanceInitFunc) gdk_window_impl_xynth_init,
		};
		static const GInterfaceInfo paintable_info = {
			(GInterfaceInitFunc) gdk_window_impl_xynth_paintable_init,
			NULL,
			NULL
		};
		object_type = g_type_register_static(GDK_TYPE_DRAWABLE_IMPL_XYNTH, "GdkWindowImplXYNTH", &object_info, 0);
		g_type_add_interface_static(object_type, GDK_TYPE_PAINTABLE, &paintable_info);
	}
	LEAVE();
	return object_type;
}

GType _gdk_window_impl_get_type (void)
{
	GType rtype;
	ENTER();
	rtype = gdk_window_impl_xynth_get_type();
	LEAVE();
	return rtype;
}

static void xynth_window_atevent (s_window_t *window, s_event_t *event)
{
	ENTER();
	/* write to gtk */
	LEAVE();
}

static int create_xynth_window (GdkWindowImplXYNTH *impl, s_window_t *parent, S_WINDOW window_type, s_rect_t *position)
{
	s_window_t *xynth_window;
	ENTER();
	if (s_window_init(&xynth_window)) {
		LEAVE();
		return -1;
	}
	if (s_window_new(xynth_window, window_type, parent)) {
		s_window_uninit(xynth_window);
		LEAVE();
		return -1;
	}
	if (impl->input_only) {
		impl->drawable.xynth_surface = NULL;
	} else {
		impl->drawable.xynth_surface = xynth_window->surface;
	}
	if (position) {
		s_window_set_coor(xynth_window, WINDOW_NOFORM, position->x, position->y, position->w, position->h);
	}
	impl->xynth_window = xynth_window;
	return 0;
}

GdkWindow * gdk_xynth_window_new (GdkWindow *parent, GdkWindowAttr *attributes, gint attributes_mask)
{
	GdkWindow             *window;
	GdkWindowObject       *private;
	GdkWindowObject       *parent_private;
	GdkWindowImplXYNTH    *impl;
	GdkWindowImplXYNTH    *parent_impl;
	GdkVisual             *visual;
	s_rect_t pos;
	gint x, y;
	ENTER();
	if (attributes == NULL) {
		LEAVE();
		return NULL;
	}
	if (!parent || attributes->window_type != GDK_WINDOW_CHILD) {
		parent = _gdk_parent_root;
	}

	window = g_object_new(GDK_TYPE_WINDOW, NULL);
	private = GDK_WINDOW_OBJECT(window);
	
	parent_private = GDK_WINDOW_OBJECT(parent);
	parent_impl = GDK_WINDOW_IMPL_XYNTH(parent_private->impl);
	private->parent = parent_private;
	
	x = (attributes_mask & GDK_WA_X) ? attributes->x : 0;
	y = (attributes_mask & GDK_WA_Y) ? attributes->y : 0;
	
	gdk_window_set_events (window, attributes->event_mask | GDK_STRUCTURE_MASK);
	
	impl = GDK_WINDOW_IMPL_XYNTH(private->impl);
	impl->drawable.wrapper = GDK_DRAWABLE(window);
	impl->gdkWindow = window;
	
	private->x = x;
	private->y = y;
	
	_gdk_xynth_calc_abs(window);
	
	impl->drawable.width  = MAX(1, attributes->width);
	impl->drawable.height = MAX(1, attributes->height);
	
	private->window_type = attributes->window_type;
	
	if (attributes_mask & GDK_WA_VISUAL) {
		visual = attributes->visual;
	} else {
		visual = gdk_drawable_get_visual(parent);
	}
	
	switch (attributes->wclass) {
		case GDK_INPUT_OUTPUT:
			DEBUG("wclass: GDK_INPUT_OUTPUT");
			private->input_only = FALSE;
			impl->input_only = 0;
			break;
		case GDK_INPUT_ONLY:
			DEBUG("wclass: GDK_INPUT_ONLY");
			private->input_only = TRUE;
			impl->input_only = 1;
			break;
		default:
			g_warning("gdk_window_new: unsupported window class\n");
			_gdk_window_destroy(window, FALSE);
			LEAVE();
			return NULL;
	}
	switch (private->window_type) {
		case GDK_WINDOW_TOPLEVEL:
			DEBUG("wtype: GDK_WINDOW_TOPLEVEL");
			goto open_window;
		case GDK_WINDOW_DIALOG:
			DEBUG("wtype: GDK_WINDOW_DIALOG");
			goto open_window;
		case GDK_WINDOW_TEMP:
			DEBUG("wtype: GDK_WINDOW_TEMP");
open_window:
			pos.x = x;
			pos.y = y;
			pos.w = impl->drawable.width;
			pos.h = impl->drawable.height;
			if (create_xynth_window(impl, parent_impl->xynth_window, WINDOW_CHILD, &pos)) {
			          _gdk_window_destroy(window, FALSE);
			          return NULL;
			}
			break;
		case GDK_WINDOW_CHILD:
			DEBUG("wtype: GDK_WINDOW_CHILD");
			NIY();
			ASSERT();
			break;
		default:
			g_warning("gdk_window_new: unsupported window type: %d", private->window_type);
			_gdk_window_destroy(window, FALSE);
			LEAVE();
			return NULL;
	}
	
	if (impl->drawable.xynth_surface) {
		GdkColormap *colormap;
		private->depth = impl->drawable.xynth_surface->bitsperpixel;
		if ((attributes_mask & GDK_WA_COLORMAP) && attributes->colormap) {
			colormap = attributes->colormap;
		} else {
			if (gdk_visual_get_system() == visual) {
				colormap = gdk_colormap_get_system();
			} else {
				colormap = gdk_drawable_get_colormap(parent);
			}
		}
		gdk_drawable_set_colormap(GDK_DRAWABLE (window), colormap);
	} else {
		private->depth = visual->depth;
	}
	
	gdk_window_set_cursor(window, ((attributes_mask & GDK_WA_CURSOR) ? (attributes->cursor) : NULL));
	
	if (parent_private) {
		parent_private->children = g_list_prepend(parent_private->children, window);
	}
	
	/* we hold a reference count on ourselves */
	g_object_ref(window);
	
	if (impl->xynth_window) {
		s_window_atevent(impl->xynth_window, xynth_window_atevent);
	}
	if (attributes_mask & GDK_WA_TYPE_HINT) {
		gdk_window_set_type_hint(window, attributes->type_hint);
	}
	LEAVE();
	return window;
}

GdkWindow * gdk_window_new (GdkWindow *parent, GdkWindowAttr *attributes, gint attributes_mask)
{
	ENTER();
	if (attributes == NULL) {
		LEAVE();
		return NULL;
	}
	LEAVE();
	return gdk_xynth_window_new(parent, attributes, attributes_mask);
}


void _gdk_windowing_window_init (void)
{
	s_rect_t pos;
	GdkWindowObject *private;
	GdkWindowImplXYNTH *impl;
	
	ENTER();
	g_assert(_gdk_parent_root == NULL);
	
	_gdk_parent_root = g_object_new(GDK_TYPE_WINDOW, NULL);
	private = GDK_WINDOW_OBJECT(_gdk_parent_root);
	impl = GDK_WINDOW_IMPL_XYNTH(private->impl);
	
	private->window_type   = GDK_WINDOW_ROOT;
	private->state         = 0;
	private->children      = NULL;
	impl->drawable.paint_region  = NULL;
	impl->gdkWindow        = _gdk_parent_root;
	impl->input_only       = 0;
	impl->xynth_window     = NULL;
	impl->drawable.abs_x   = 0;
	impl->drawable.abs_y   = 0;
	impl->drawable.width   = _gdk_display->xynth_window->surface->linear_buf_width;
	impl->drawable.height  = _gdk_display->xynth_window->surface->linear_buf_height;
	impl->drawable.wrapper = GDK_DRAWABLE(private);
	
	/* should i open window for root ?
	 */
	pos.x = 0;
	pos.y = 0;
	pos.w = _gdk_display->xynth_window->surface->linear_buf_width;
	pos.h = _gdk_display->xynth_window->surface->linear_buf_height;
	create_xynth_window(impl, _gdk_display->xynth_window, WINDOW_CHILD, &pos);

	private->depth = impl->drawable.xynth_surface->bitsperpixel;
	gdk_drawable_set_colormap(GDK_DRAWABLE (_gdk_parent_root), gdk_colormap_get_system());
	
	LEAVE();
}
