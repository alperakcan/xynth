
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
	if (attributes == NULL) {
		LEAVE();
		return NULL;
	}
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
	if (s_window_init(&xynth_window)) {
		return -1;
	}
	if (s_window_new(xynth_window, window_type, parent)) {
		s_window_uninit(xynth_window);
		return -1;
	}
	if (impl->input_only) {
		impl->drawable.xynth_surface = NULL;
	} else {
		impl->drawable.xynth_surface = xynth_window->surface;
	}
	if (position) {
		s_window_set_coor(xynth_window, window_type & WINDOW_NOFORM, position->x, position->y, position->w, position->h);
	}
	return 0;
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
