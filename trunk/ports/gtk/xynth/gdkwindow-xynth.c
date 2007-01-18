
#include "gdk-xynth.h"
#include "gdkinternals.h"

static gpointer parent_class = NULL;

static GdkRegion * gdk_window_impl_xynth_get_visible_region (GdkDrawable *drawable)
{
	GdkDrawableImplXynth *draw_impl = GDK_DRAWABLE_IMPL_XYNTH(drawable);
	GdkWindowImplXynth *window_impl = GDK_WINDOW_IMPL_XYNTH(drawable);
	GdkRectangle result_rect;
	ENT();
	result_rect.x = 0;
	result_rect.y = 0;
	result_rect.width = window_impl->width;
	result_rect.height = window_impl->height;
	DBG("Rect: %d %d, %d %d", result_rect.x, result_rect.y, result_rect.width, result_rect.height);
//	gdk_rectangle_intersect(&result_rect, &window_impl->clip, &result_rect);
	switch (draw_impl->window_type) {
		case GDK_WINDOW_TOPLEVEL:
			DBG("GDK_WINDOW_TOPLEVEL");
			break;
		case GDK_WINDOW_CHILD:
			DBG("GDK_WINDOW_CHILD");
			break;
		case GDK_WINDOW_DIALOG:
			DBG("GDK_WINDOW_DIALOG");
			break;
		case GDK_WINDOW_TEMP:
			DBG("GDK_WINDOW_TEMP");
			break;
		case GDK_WINDOW_ROOT:
			DBG("GDK_WINDOW_ROOT");
			break;
		default:
			DBG("GDK_WINDOW_UNKNOWN");
			break;
	}
	DBG("Rect: %d %d, %d %d", result_rect.x, result_rect.y, result_rect.width, result_rect.height);
	LEV();
	return gdk_region_rectangle(&result_rect);
}

static void gdk_window_impl_xynth_init (GdkWindowImplXynth *impl)
{
	ENT();
	impl->width = -1;
	impl->height = -1;
	LEV();
}

static void gdk_window_impl_xynth_finalize (GObject *object)
{
	GdkDrawableImplXynth *draw_impl;
	GdkWindowImplXynth *window_impl;
	ENT();
	GType gdk_window_impl_xynth_get_type (void);
	g_return_if_fail(GDK_IS_WINDOW_IMPL_XYNTH(object));
	draw_impl = GDK_DRAWABLE_IMPL_XYNTH(object);
	window_impl = GDK_WINDOW_IMPL_XYNTH(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	LEV();
}

static void gdk_window_impl_xynth_class_init (GdkWindowImplXynthClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	GdkDrawableClass *drawable_class = GDK_DRAWABLE_CLASS(klass);
	ENT();
	parent_class = g_type_class_peek_parent(klass);
	object_class->finalize = gdk_window_impl_xynth_finalize;
	/* Visible and clip regions are the same */
	drawable_class->get_clip_region = gdk_window_impl_xynth_get_visible_region;
	drawable_class->get_visible_region = gdk_window_impl_xynth_get_visible_region;
	LEV();
}

GType gdk_window_impl_xynth_get_type (void)
{
	static GType object_type = 0;
	ENT();
	if (!object_type) {
		static const GTypeInfo object_info = {
			sizeof(GdkWindowImplXynthClass),
			(GBaseInitFunc) NULL,
			(GBaseFinalizeFunc) NULL,
			(GClassInitFunc) gdk_window_impl_xynth_class_init,
			NULL,           /* class_finalize */
			NULL,           /* class_data */
			sizeof(GdkWindowImplXynth),
			0,              /* n_preallocs */
			(GInstanceInitFunc) gdk_window_impl_xynth_init,
		};
		object_type = g_type_register_static(GDK_TYPE_DRAWABLE_IMPL_XYNTH, "GdkWindowImplXynth", &object_info, 0);
	}
	LEV();
	return object_type;
}

GType _gdk_window_impl_get_type (void)
{
	ENT();
	LEV();
	return gdk_window_impl_xynth_get_type ();
}

void _gdk_windowing_window_init (GdkDisplay *display)
{
	GdkWindowObject *private;
	GdkDisplayXynth *display_xynth;
	GdkWindowImplXynth *window_impl;
	GdkDrawableImplXynth *draw_impl;
	ENT();
	g_assert(_gdk_parent_root == NULL);
	_gdk_parent_root = g_object_new(GDK_TYPE_WINDOW, NULL);
	display_xynth = GDK_DISPLAY_XYNTH(display);
	private = (GdkWindowObject *) _gdk_parent_root;
	draw_impl = GDK_DRAWABLE_IMPL_XYNTH(private->impl);
	window_impl = GDK_WINDOW_IMPL_XYNTH(private->impl);
	private->window_type = GDK_WINDOW_ROOT;
	private->depth = gdk_visual_get_system()->depth;
	draw_impl->wrapper = GDK_DRAWABLE(private);
	window_impl->width = display_xynth->window->window->surface->linear_buf_width;
	window_impl->height = display_xynth->window->window->surface->linear_buf_height; 
	window_impl->window = display_xynth->window;
	window_impl->wrapper = _gdk_parent_root;
	LEV();
}

void gdk_window_set_events (GdkWindow *window, GdkEventMask event_mask)
{
	ENT();
	g_return_if_fail(window != NULL);
	g_return_if_fail(GDK_IS_WINDOW (window));
	if (event_mask & GDK_BUTTON_MOTION_MASK) {
		event_mask |= GDK_BUTTON1_MOTION_MASK | GDK_BUTTON2_MOTION_MASK | GDK_BUTTON3_MOTION_MASK;
	}
	GDK_WINDOW_OBJECT(window)->event_mask = GDK_STRUCTURE_MASK | event_mask;
	LEV();
}

static const gchar * get_default_title (void)
{
	const char *title;
	ENT();
	title = g_get_application_name();
	if (!title)
		title = g_get_prgname();
	LEV();
	return title;
}

void gdk_window_move_resize (GdkWindow *window, gint x, gint y, gint width, gint height)
{
	GdkWindowObject *private;
	GdkDrawableImplXynth *draw_impl;
	GdkWindowImplXynth *window_impl;
	ENT();
	private = (GdkWindowObject *) window;
	draw_impl = GDK_DRAWABLE_IMPL_XYNTH(private->impl);
	window_impl = GDK_WINDOW_IMPL_XYNTH(private->impl);
        private->state = 0;
	DBG("x:%d, y:%d, w:%d, h:%d", x, y, width, height);
	switch (draw_impl->window_type) {
		case GDK_WINDOW_DIALOG:
		case GDK_WINDOW_TOPLEVEL:
		case GDK_WINDOW_TEMP:
			private->x = x;
			private->y = y;
			window_impl->width = width;
			window_impl->height = height;
			w_object_move(draw_impl->object, 0, 0, width, height);
			w_window_set_coor(window_impl->window, x, y, width, height);
			break;
		default:
			w_object_move(draw_impl->object, x, y, width, height);
			break;
	}
	if (private->input_only == FALSE) {
		gdk_window_invalidate_rect(window, NULL, TRUE);
	}
	LEV();
}

void gdk_window_set_title (GdkWindow *window, const gchar *title)
{
	GdkWindowObject *private;
	GdkDrawableImplXynth *draw_impl;
	GdkWindowImplXynth *window_impl;
	ENT();
	private = (GdkWindowObject *) window;
	draw_impl = GDK_DRAWABLE_IMPL_XYNTH(private->impl);
	window_impl = GDK_WINDOW_IMPL_XYNTH(private->impl);
	s_window_set_title(window_impl->window->window, (char *) title);
	DBG("title: %s", title);
	LEV();
}

GdkWindow * gdk_window_new (GdkWindow *parent, GdkWindowAttr *attributes, gint attributes_mask)
{
	int x;
	int y;
	int depth;
	gchar *title;
	GdkVisual *visual;
	GdkWindow *window;
	GdkWindowObject *private;
	GdkWindowObject *pprivate;
	GdkDrawableImplXynth *draw_impl;
	GdkWindowImplXynth *window_impl;
	GdkDrawableImplXynth *pdraw_impl;
	GdkWindowImplXynth *pwindow_impl;

	ENT();

	g_return_val_if_fail(attributes != NULL, NULL);

	if (!parent ||
	    (attributes->window_type != GDK_WINDOW_CHILD &&
	     attributes->window_type != GDK_WINDOW_TEMP)) {
		parent = _gdk_parent_root;
	}
	
	window = (GdkWindow *) g_object_new(GDK_TYPE_WINDOW, NULL);
	private = (GdkWindowObject *) window;
	pprivate = (GdkWindowObject*) parent;
	draw_impl = GDK_DRAWABLE_IMPL_XYNTH(private->impl);
	window_impl = GDK_WINDOW_IMPL_XYNTH(private->impl);
	pdraw_impl = GDK_DRAWABLE_IMPL_XYNTH(pprivate->impl);
	pwindow_impl = GDK_WINDOW_IMPL_XYNTH(pprivate->impl);

	private->parent = pprivate;
	private->accept_focus = TRUE;
	private->focus_on_map = TRUE;

	if (attributes_mask & GDK_WA_X) {
		x = attributes->x;
	} else {
		x = 0;
	}
	if (attributes_mask & GDK_WA_Y) {
		y = attributes->y;
	} else {
		y = 0;
	}
	
	DBG("x: %d, y: %d, w: %d, h: %d", x, y, (attributes->width > 1) ? (attributes->width) : (1), (attributes->height > 1) ? (attributes->height) : (1));
	
	gdk_window_set_events (window, attributes->event_mask);
	
	if (attributes_mask & GDK_WA_VISUAL) {
		visual = attributes->visual;
	} else {
		visual = gdk_visual_get_system();
	}

	private->x = x;
	private->y = y;
	private->window_type = attributes->window_type;
	draw_impl->wrapper = GDK_DRAWABLE(private);
	draw_impl->window_type = attributes->window_type;
	window_impl->width = (attributes->width > 1) ? (attributes->width) : (1);
	window_impl->height = (attributes->height > 1) ? (attributes->height) : (1);
	window_impl->wrapper = window;
	
	switch (draw_impl->window_type) {
		case GDK_WINDOW_TOPLEVEL:
			DBG("GDK_WINDOW_TOPLEVEL");
			break;
		case GDK_WINDOW_CHILD:
			DBG("GDK_WINDOW_CHILD");
			break;
		case GDK_WINDOW_DIALOG:
			DBG("GDK_WINDOW_DIALOG");
			break;
		case GDK_WINDOW_TEMP:
			DBG("GDK_WINDOW_TEMP");
			break;
		case GDK_WINDOW_ROOT:
			DBG("GDK_WINDOW_ROOT");
			break;
		default:
			DBG("GDK_WINDOW_UNKNOWN");
			break;
	}
	if (attributes->wclass == GDK_INPUT_OUTPUT) {
		DBG("GDK_INPUT_OUTPUT");
	} else {
		DBG("GDK_INPUT_OUNLY");
	}	
	if (attributes->wclass == GDK_INPUT_OUTPUT) {
		depth = visual->depth;
		private->input_only = FALSE;
		private->depth = depth;
		if ((attributes_mask & GDK_WA_COLORMAP) && attributes->colormap) {
			draw_impl->colormap = attributes->colormap;
		} else {
			draw_impl->colormap = gdk_colormap_get_system();
		}
	} else {
		depth = 0;
		private->depth = 0;
		private->input_only = TRUE;
		draw_impl->colormap = gdk_screen_get_system_colormap(_gdk_screen);
	}

	switch (draw_impl->window_type) {
		case GDK_WINDOW_TOPLEVEL:
			w_window_init(&(window_impl->window), WINDOW_CHILD, pwindow_impl->window);
			w_object_init(window_impl->window, &(draw_impl->object), NULL, window_impl->window->object);
			s_window_main(window_impl->window->window);
			break;
		case GDK_WINDOW_CHILD:
			w_object_init(pwindow_impl->window, &(draw_impl->object), NULL, pdraw_impl->object);
			break;
		case GDK_WINDOW_DIALOG:
			break;
		case GDK_WINDOW_TEMP:
			break;
		case GDK_WINDOW_ROOT:
			if (_gdk_parent_root) {
				g_error("cannot make windows of type GDK_WINDOW_ROOT");
			}
			break;
		default:
			g_error("cannot make windows of type GDK_WINDOW_UNKNOWN");
			break;
	}

	gdk_window_move_resize(window, x, y, window_impl->width, window_impl->height);
	switch (draw_impl->window_type) {
		case GDK_WINDOW_DIALOG:
		case GDK_WINDOW_TOPLEVEL:
		case GDK_WINDOW_TEMP:
			if (attributes_mask & GDK_WA_TITLE) {
				title = attributes->title;
			} else {
				title = (gchar *) get_default_title();
			}
			gdk_window_set_title(window, title);
			break;
		default:
			break;
	}
	
	if (draw_impl->colormap) {
		gdk_colormap_ref(draw_impl->colormap);
	}
	gdk_window_set_cursor(window, ((attributes_mask & GDK_WA_CURSOR) ? (attributes->cursor) : NULL));
	
	if (pprivate) {
		pprivate->children = g_list_prepend(pprivate->children, window);
	}
	g_object_ref(window);

	LEV();

	return window;
}

void gdk_window_enable_synchronized_configure (GdkWindow *window)
{
	ENT();
	LEV();
}

void gdk_window_set_background (GdkWindow *window, const GdkColor *color)
{
	GdkWindowObject *private = (GdkWindowObject *) window;
	ENT();
	g_return_if_fail(window != NULL);
	g_return_if_fail(GDK_IS_WINDOW (window));
	private->bg_color = *color;
	if (private->bg_pixmap &&
	    private->bg_pixmap != GDK_PARENT_RELATIVE_BG &&
	    private->bg_pixmap != GDK_NO_BG) {
		g_object_unref(private->bg_pixmap);
		private->bg_pixmap = NULL;
	}
	LEV();
}

void _gdk_windowing_window_get_offsets (GdkWindow *window, gint *x_offset, gint *y_offset)
{
	ENT();
	*x_offset = 0;
	*y_offset = 0;
	LEV();
}

void gdk_window_set_type_hint (GdkWindow *window, GdkWindowTypeHint hint)
{
	ENT();
	g_return_if_fail(window != NULL);
	g_return_if_fail(GDK_IS_WINDOW (window));
	LEV();
}

void gdk_window_set_accept_focus (GdkWindow *window, gboolean accept_focus)
{
	GdkWindowObject *private;
	ENT();
	g_return_if_fail(window != NULL);
	g_return_if_fail(GDK_IS_WINDOW (window));
	private = (GdkWindowObject *)window;  
	accept_focus = accept_focus != FALSE;
	if (private->accept_focus != accept_focus)
		private->accept_focus = accept_focus;
	LEV();
}

void gdk_window_set_focus_on_map (GdkWindow *window, gboolean focus_on_map)
{
	GdkWindowObject *private;
	ENT();
	g_return_if_fail(window != NULL);
	g_return_if_fail(GDK_IS_WINDOW (window));
	private = (GdkWindowObject *)window;  
	focus_on_map = focus_on_map != FALSE;
	if (private->focus_on_map != focus_on_map)
		private->focus_on_map = focus_on_map;
	LEV();
}

void gdk_window_set_modal_hint (GdkWindow *window, gboolean modal)
{
	ENT();
	g_return_if_fail(GDK_IS_WINDOW (window));
	LEV();
}

void gdk_window_set_icon_list (GdkWindow *window, GList *pixbufs)
{
	ENT();
	DBG("What da fuck");
	LEV();
}

void gdk_window_set_icon (GdkWindow *window, GdkWindow *icon_window, GdkPixmap *pixmap, GdkBitmap *mask)
{
	ENT();
	g_return_if_fail(window != NULL);
	g_return_if_fail(GDK_IS_WINDOW (window));
	LEV();
}

void gdk_window_set_geometry_hints (GdkWindow *window, GdkGeometry *geometry, GdkWindowHints geom_mask)
{
	ENT();
	LEV();
}

void gdk_window_unmaximize (GdkWindow *window)
{
	ENT();
	g_return_if_fail(GDK_IS_WINDOW (window));
	gdk_synthesize_window_state (window, GDK_WINDOW_STATE_MAXIMIZED, 0);
	LEV();
}

void gdk_window_unstick (GdkWindow *window)
{
	ENT();
	g_return_if_fail(GDK_IS_WINDOW (window));
	LEV();
}

void gdk_window_deiconify (GdkWindow *window)
{
	ENT();
	g_return_if_fail(window != NULL);
	g_return_if_fail(GDK_IS_WINDOW (window));
	LEV();
}

void gdk_window_unfullscreen (GdkWindow *window)
{
	ENT();
	g_return_if_fail(GDK_IS_WINDOW (window));
	LEV();
}

void gdk_window_set_keep_above (GdkWindow *window, gboolean setting)
{
	ENT();
	g_return_if_fail(GDK_IS_WINDOW (window));
	LEV();
}

void gdk_window_set_keep_below (GdkWindow *window, gboolean setting)
{
	ENT();
	g_return_if_fail(GDK_IS_WINDOW (window));
	LEV();
}

void gdk_window_show (GdkWindow *window)
{
	GdkRectangle rect;
	GdkWindow *mousewin;
	GdkWindowObject *private;
	GdkDrawableImplXynth *draw_impl;
	GdkWindowImplXynth *window_impl;
	ENT();
	g_return_if_fail(window != NULL);
	private = (GdkWindowObject *) window;
	draw_impl = GDK_DRAWABLE_IMPL_XYNTH(private->impl);
	window_impl = GDK_WINDOW_IMPL_XYNTH(private->impl);
	if (private->destroyed) {
		return;
	}
	if (!GDK_WINDOW_IS_MAPPED(window)) {
		gdk_synthesize_window_state(window, GDK_WINDOW_STATE_WITHDRAWN, 0);
        }
        g_assert(GDK_WINDOW_IS_MAPPED(window));
        
	switch (draw_impl->window_type) {
		case GDK_WINDOW_TOPLEVEL:
			DBG("GDK_WINDOW_TOPLEVEL");
			break;
		case GDK_WINDOW_CHILD:
			DBG("GDK_WINDOW_CHILD");
			break;
		case GDK_WINDOW_DIALOG:
			DBG("GDK_WINDOW_DIALOG");
			break;
		case GDK_WINDOW_TEMP:
			DBG("GDK_WINDOW_TEMP");
			break;
		case GDK_WINDOW_ROOT:
			DBG("GDK_WINDOW_ROOT");
			break;
		default:
			DBG("GDK_WINDOW_UNKNOWN");
			break;
	}

	switch (draw_impl->window_type) {
		case GDK_WINDOW_DIALOG:
		case GDK_WINDOW_TOPLEVEL:
		case GDK_WINDOW_TEMP:
			w_object_show(draw_impl->object);
			s_window_show(window_impl->window->window);
			break;
		default:
			break;
	}

	LEV();
}

gboolean _gdk_windowing_window_queue_antiexpose (GdkWindow *window, GdkRegion *area)
{
	ENT();
	LEV();
	return FALSE;
}
