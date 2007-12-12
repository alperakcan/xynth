
#include "gdkxynth.h"

static gpointer parent_class = NULL;

static GHashTable *window_id_ht = NULL;

static guint update_idle = 0;
static GSList *update_windows = NULL;

static GdkWindow *gdk_xynth_window_containing_pointer = NULL;

void gdk_xynth_window_id_table_remove (int xynth_id)
{
	ENTER();
	if (window_id_ht) {
		g_hash_table_remove(window_id_ht, GUINT_TO_POINTER(xynth_id));
	}
	LEAVE();
}

static int create_xynth_window (GdkWindowImplXYNTH *impl, s_window_t *parent, S_WINDOW window_type, s_rect_t *position)
{
	s_window_t *xynth;
	ENTER();
	if (s_window_init(&xynth)) {
		LEAVE();
		return -1;
	}
	if (s_window_new(xynth, window_type, parent)) {
		s_window_uninit(xynth);
		LEAVE();
		return -1;
	}
	if (impl->input_only) {
		impl->drawable.surface = NULL;
	} else {
		impl->drawable.surface = xynth->surface;
	}
	if (position) {
		s_window_set_coor(xynth, WINDOW_NOFORM, position->x, position->y, position->w, position->h);
	}
	impl->xynth = xynth;
	LEAVE();
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
	
	private->window_type        = GDK_WINDOW_ROOT;
	private->state              = 0;
	private->children           = NULL;
	impl->drawable.paint_region = NULL;
	impl->gdkWindow             = _gdk_parent_root;
	impl->input_only            = 0;
	impl->xynth                 = NULL;
	impl->drawable.abs_x        = 0;
	impl->drawable.abs_y        = 0;
	impl->drawable.width        = _gdk_display->xynth->surface->linear_buf_width;
	impl->drawable.height       = _gdk_display->xynth->surface->linear_buf_height;
	impl->drawable.wrapper      = GDK_DRAWABLE(private);
	
	/* should i open window for root ?
	 */
	pos.x = 0;
	pos.y = 0;
	pos.w = _gdk_display->xynth->surface->linear_buf_width;
	pos.h = _gdk_display->xynth->surface->linear_buf_height;
	create_xynth_window(impl, _gdk_display->xynth, WINDOW_CHILD, &pos);

	private->depth = impl->drawable.surface->bitsperpixel;
	gdk_drawable_set_colormap(GDK_DRAWABLE (_gdk_parent_root), gdk_colormap_get_system());
	
	LEAVE();
}

static GdkRegion * gdk_window_impl_xynth_get_visible_region (GdkDrawable *drawable)
{
	GdkRegion *region;
	GdkRectangle rect;
	GdkDrawableImplXYNTH *priv;
	ENTER();
	priv = GDK_DRAWABLE_IMPL_XYNTH(drawable);
	rect.x = 0;
	rect.y = 0;
	rect.width = 0;
	rect.height = 0;
	if (priv->surface) {
		rect.x      = priv->surface->buf->x;
		rect.y      = priv->surface->buf->y;
		rect.width  = priv->surface->buf->w;
		rect.height = priv->surface->buf->h;
	}
	region = gdk_region_rectangle(&rect);
	LEAVE();
	return region;
}

static void g_free_2nd (gpointer a, gpointer b, gpointer data)
{
	ENTER();
	g_free(b);
	LEAVE();
}

static void gdk_window_impl_xynth_finalize (GObject *object)
{
	GdkWindowImplXYNTH *impl;
	ENTER();
	impl = GDK_WINDOW_IMPL_XYNTH(object);
	if (GDK_WINDOW_IS_MAPPED(impl->drawable.wrapper)) {
		gdk_window_hide(impl->drawable.wrapper);
	}
	if (impl->cursor) {
		gdk_cursor_unref(impl->cursor);
	}
	if (impl->properties) {
		g_hash_table_foreach(impl->properties, g_free_2nd, NULL);
		g_hash_table_destroy(impl->properties);
	}
	if (impl->xynth) {
		gdk_xynth_window_id_table_remove(impl->xynth_id);
		/* native window resource must be release before we can finalize !*/
		impl->xynth = NULL;
	}
	if (G_OBJECT_CLASS(parent_class)->finalize) {
		G_OBJECT_CLASS(parent_class)->finalize(object);
	}
	LEAVE();
}

static void gdk_window_impl_xynth_class_init (GdkWindowImplXYNTHClass *klass)
{
	GObjectClass     *object_class;
	GdkDrawableClass *drawable_class;
	
	ENTER();
	
	object_class   = G_OBJECT_CLASS(klass);
	drawable_class = GDK_DRAWABLE_CLASS(klass);

	parent_class = g_type_class_peek_parent(klass);
	object_class->finalize = gdk_window_impl_xynth_finalize;
	
	/* Visible and clip regions are the same */
	drawable_class->get_clip_region = gdk_window_impl_xynth_get_visible_region;
	drawable_class->get_visible_region = gdk_window_impl_xynth_get_visible_region;
	
	LEAVE();
}

static void gdk_window_impl_xynth_init (GdkWindowImplXYNTH *impl)
{
	ENTER();
	impl->drawable.width  = 1;
	impl->drawable.height = 1;
	/* cannot use gdk_cursor_new here since gdk_display_get_default
	 * does not work yet.
	 */
	impl->cursor          = gdk_cursor_new_for_display(GDK_DISPLAY_OBJECT(_gdk_display),GDK_LEFT_PTR);
	LEAVE();
}

static void gdk_window_xynth_process_all_updates (void)
{
	GSList *tmp_list;
	GSList *old_update_windows;
	
	ENTER();
	
	old_update_windows = update_windows;
	tmp_list = update_windows;

	if (update_idle) {
		g_source_remove (update_idle);
	}
	
	update_windows = NULL;
	update_idle = 0;
	
	g_slist_foreach(old_update_windows, (GFunc) g_object_ref, NULL);
	
	while (tmp_list) {
		GdkWindowObject *private = (GdkWindowObject *) tmp_list->data;
		if (private->update_freeze_count) {
			update_windows = g_slist_prepend(update_windows, private);
		} else {
			gdk_window_process_updates(tmp_list->data,TRUE);
		}
		g_object_unref(tmp_list->data);
		tmp_list = tmp_list->next;
	}
	
	g_slist_free(old_update_windows);
	
	LEAVE();
}

static gboolean gdk_window_update_idle (gpointer data)
{
	ENTER();
	GDK_THREADS_ENTER();
	gdk_window_xynth_process_all_updates();
	GDK_THREADS_LEAVE();
	LEAVE();
	return FALSE;
}

static void gdk_window_schedule_update (GdkWindow *window)
{
	ENTER();
	if (window && GDK_WINDOW_OBJECT(window)->update_freeze_count) {
		LEAVE();
		return;
	}
	if (!update_idle) {
		update_idle = g_idle_add_full(GDK_PRIORITY_REDRAW, gdk_window_update_idle, NULL, NULL);
	}
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

	GdkRegion *visible_region;
	GList *tmp_list;

	g_return_if_fail(window != NULL);
	g_return_if_fail(GDK_IS_WINDOW(window));

	if (GDK_WINDOW_DESTROYED(window)) {
		LEAVE();
		return;
	}
	if (private->input_only || !GDK_WINDOW_IS_MAPPED(window)) {
		LEAVE();
		return;
	}

	visible_region = gdk_drawable_get_visible_region(window);
	gdk_region_intersect(visible_region, region);

	tmp_list = private->children;
	while (tmp_list) {
		GdkWindowObject *child = tmp_list->data;
		if (!child->input_only) {
			GdkRegion *child_region;
			GdkRectangle child_rect;
			
			gdk_window_get_position((GdkWindow *)child, &child_rect.x, &child_rect.y);
			gdk_drawable_get_size((GdkDrawable *)child, &child_rect.width, &child_rect.height);

			child_region = gdk_region_rectangle(&child_rect);
			
			/* remove child area from the invalid area of the parent */
			if (GDK_WINDOW_IS_MAPPED(child) && !child->shaped) {
				gdk_region_subtract(visible_region, child_region);
			}
			
			if (child_func && (*child_func)((GdkWindow *)child, user_data)) {
				gdk_region_offset(region, - child_rect.x, - child_rect.y);
				gdk_region_offset(child_region, - child_rect.x, - child_rect.y);
				gdk_region_intersect(child_region, region);
				
				gdk_window_invalidate_maybe_recurse((GdkWindow *)child, child_region, child_func, user_data);
				gdk_region_offset(region, child_rect.x, child_rect.y);
			}
			gdk_region_destroy(child_region);
		}
		tmp_list = tmp_list->next;
	}
	if (!gdk_region_empty(visible_region)) {
		if (private->update_area) {
			gdk_region_union(private->update_area, visible_region);
		} else {
			update_windows = g_slist_prepend(update_windows, window);
			private->update_area = gdk_region_copy(visible_region);
			gdk_window_schedule_update(window);
		}
	}
	gdk_region_destroy(visible_region);
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
	private = (GdkWindowObject *) window;
	gboolean save_region = FALSE;
	
	/* If an update got queued during update processing, we can get a
	 * window in the update queue that has an empty update_area.
	 * just ignore it.
	 */
	if (private->update_area) {
		GdkRegion *update_area = private->update_area;
		private->update_area = NULL;
		
		if (_gdk_event_func && gdk_window_is_viewable(window)) {
			GdkRectangle window_rect;
			GdkRegion *expose_region;
			GdkRegion *window_region;
			gint width, height;
			save_region = _gdk_windowing_window_queue_antiexpose(window, update_area);
			
			if (save_region) {
				expose_region = gdk_region_copy(update_area);
			} else {
				expose_region = update_area;
			}
			gdk_drawable_get_size(GDK_DRAWABLE(private), &width, &height);

			window_rect.x = 0;
			window_rect.y = 0;
			window_rect.width = width;
			window_rect.height = height;

			window_region = gdk_region_rectangle(&window_rect);
			gdk_region_intersect(expose_region, window_region);
			gdk_region_destroy(window_region);

			if (!gdk_region_empty (expose_region) && (private->event_mask & GDK_EXPOSURE_MASK)) {
				GdkEvent event;
				event.expose.type = GDK_EXPOSE;
				event.expose.window = g_object_ref(window);
				event.expose.send_event = FALSE;
				event.expose.count = 0;
				event.expose.region = expose_region;
				gdk_region_get_clipbox(expose_region, &event.expose.area);
				(*_gdk_event_func) (&event, _gdk_event_data);
				g_object_unref (window);
			}
			if (expose_region != update_area) {
				gdk_region_destroy(expose_region);
			}
		}
		if (!save_region) {
			gdk_region_destroy (update_area);
		}
	}
	
	LEAVE();
}

static void gdk_window_impl_xynth_begin_paint_region (GdkPaintable *paintable, GdkRegion *region)
{
	gint i;
	GdkWindowImplXYNTH *wimpl;
	GdkDrawableImplXYNTH *impl;
	
	g_assert(region != NULL);
	
	ENTER();
	
	wimpl = GDK_WINDOW_IMPL_XYNTH(paintable);
	impl = (GdkDrawableImplXYNTH *) wimpl;
	impl->buffered = TRUE;
	impl->paint_depth++;
	
	if (!region) {
		LEAVE();
		return;
	}
	if (impl->paint_region) {
		gdk_region_union(impl->paint_region, region);
	} else {
		impl->paint_region = gdk_region_copy(region);
	}
	
	for (i = 0; i < region->numRects; i++) {
		GdkRegionBox *box = &region->rects[i];
		_gdk_windowing_window_clear_area(GDK_WINDOW(wimpl->gdkWindow), box->x1, box->y1, box->x2 - box->x1, box->y2 - box->y1);
	}

	LEAVE();
}

static void gdk_window_impl_xynth_end_paint (GdkPaintable *paintable)
{
	GdkDrawableImplXYNTH *impl;
	ENTER();
	impl = GDK_DRAWABLE_IMPL_XYNTH(paintable);
	if (impl->paint_depth <= 0) {
		LEAVE();
		return;
	}
	impl->paint_depth--;
	if (impl->paint_depth == 0) {
		impl->buffered = FALSE;
		if (impl->paint_region) {
			s_rect_t rect = {
				impl->paint_region->extents.x1,
				impl->paint_region->extents.y1,
				impl->paint_region->extents.x2,
				impl->paint_region->extents.y2
			};
			DEBUG("End Paint, must update: (%d %d %d %d)", rect.x, rect.y, rect.w, rect.h);
			ERROR("Not implemented");
			gdk_region_destroy(impl->paint_region);
			impl->paint_region = NULL;
		}
	}
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
			sizeof(GdkWindowImplXYNTH),
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
	GType type;
	ENTER();
	type = gdk_window_impl_xynth_get_type();
	LEAVE();
	return type;
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

void _gdk_xynth_calc_abs (GdkWindow *window)
{
	GList *list;
	GdkWindowObject *private;
	GdkDrawableImplXYNTH *impl;
	
	ENTER();
	if (GDK_IS_WINDOW(window) == 0) {
		LEAVE();
	}

	private = GDK_WINDOW_OBJECT(window);
	impl = GDK_DRAWABLE_IMPL_XYNTH(private->impl);

	impl->abs_x = private->x;
	impl->abs_y = private->y;

	if (private->parent) {
		GdkDrawableImplXYNTH *parent_impl;
		parent_impl = GDK_DRAWABLE_IMPL_XYNTH(GDK_WINDOW_OBJECT(private->parent)->impl);
		impl->abs_x += parent_impl->abs_x;
		impl->abs_y += parent_impl->abs_y;
	}
	for (list = private->children; list; list = list->next) {
		_gdk_xynth_calc_abs(list->data);
	}
	LEAVE();
}

GdkWindow * gdk_xynth_window_find_toplevel (GdkWindow *window)
{
	ENTER();
	while (window && window != _gdk_parent_root) {
		GdkWindow *parent = (GdkWindow *) (GDK_WINDOW_OBJECT(window))->parent;
		if ((parent == _gdk_parent_root) && GDK_WINDOW_IS_MAPPED(window)) {
			return window;
		}
		window = parent;
	}
	LEAVE();
	return _gdk_parent_root;
}

void gdk_xynth_window_send_crossing_events (GdkWindow *src, GdkWindow *dest, GdkCrossingMode mode)
{
	GdkWindow       *c;
	GdkWindow       *win, *last, *next;
	GdkEvent        *event;
	gint             x, y, x_int, y_int;
	GdkModifierType  modifiers;
	GSList          *path, *list;
	gboolean         non_linear;
	GdkWindow       *a;
	GdkWindow       *b;
	GdkWindow       *event_win;
	
	ENTER();
	
	/* Do a possible cursor change before checking if we need to
	 * generate crossing events so cursor changes due to pointer
	 * grabs work correctly.
	 */
	{
		static GdkCursorXYNTH *last_cursor = NULL;
		GdkWindowObject       *private     = GDK_WINDOW_OBJECT(dest);
		GdkWindowImplXYNTH    *impl        = GDK_WINDOW_IMPL_XYNTH (private->impl);
		GdkCursorXYNTH        *cursor;
		
		if (_gdk_xynth_pointer_grab_cursor) {
			cursor = (GdkCursorXYNTH*) _gdk_xynth_pointer_grab_cursor;
		} else {
			cursor = (GdkCursorXYNTH*) impl->cursor;
		}
		
		if (cursor != last_cursor) {
			win     = gdk_xynth_window_find_toplevel(dest);
			private = GDK_WINDOW_OBJECT(win);
			impl    = GDK_WINDOW_IMPL_XYNTH(private->impl);
			if (impl->xynth) {
				DEBUG("set window cursor");
				ASSERT();
			}
			last_cursor = cursor;
		}
	}
	if (dest == gdk_xynth_window_containing_pointer) {
		LEAVE();
		return;
	}
	if (gdk_xynth_window_containing_pointer == NULL) {
		gdk_xynth_window_containing_pointer = g_object_ref(_gdk_parent_root);
	}
	if (src) {
		a = src;
	} else {
		a = gdk_xynth_window_containing_pointer;
	}
	b = dest;
	if (a == b) {
		LEAVE();
		return;
	}
	/* gdk_xynth_window_containing_pointer might have been destroyed.
	 * The refcount we hold on it should keep it, but it's parents
	 * might have died.
	 */
	if (GDK_WINDOW_DESTROYED(a)) {
		a = _gdk_parent_root;
	}
	gdk_xynth_mouse_get_info(&x, &y, &modifiers);
	c = gdk_xynth_find_common_ancestor(a, b);
	non_linear = (c != a) && (c != b);
	event_win = gdk_xynth_pointer_event_window(a, GDK_LEAVE_NOTIFY);
	if (event_win) {
		event = gdk_xynth_event_make(event_win, GDK_LEAVE_NOTIFY);
		event->crossing.subwindow = NULL;
		gdk_window_get_origin(a, &x_int, &y_int);
		event->crossing.x      = x - x_int;
		event->crossing.y      = y - y_int;
		event->crossing.x_root = x;
		event->crossing.y_root = y;
		event->crossing.mode   = mode;
		if (non_linear) {
			event->crossing.detail = GDK_NOTIFY_NONLINEAR;
		} else if (c == a) {
			event->crossing.detail = GDK_NOTIFY_INFERIOR;
		} else {
			event->crossing.detail = GDK_NOTIFY_ANCESTOR;
		}
		event->crossing.focus = FALSE;
		event->crossing.state = modifiers;
	}
	/* Traverse up from a to (excluding) c */
	if (c != a) {
		last = a;
		win = GDK_WINDOW(GDK_WINDOW_OBJECT(a)->parent);
		while (win != c) {
			event_win = gdk_xynth_pointer_event_window(win, GDK_LEAVE_NOTIFY);
			if (event_win) {
				event = gdk_xynth_event_make(event_win, GDK_LEAVE_NOTIFY);
				event->crossing.subwindow = g_object_ref(last);
				gdk_window_get_origin(win, &x_int, &y_int);
				event->crossing.x      = x - x_int;
				event->crossing.y      = y - y_int;
				event->crossing.x_root = x;
				event->crossing.y_root = y;
				event->crossing.mode   = mode;
				if (non_linear) {
					event->crossing.detail = GDK_NOTIFY_NONLINEAR_VIRTUAL;
				} else {
					event->crossing.detail = GDK_NOTIFY_VIRTUAL;
				}
				event->crossing.focus = FALSE;
				event->crossing.state = modifiers;
			}
			last = win;
			win = GDK_WINDOW(GDK_WINDOW_OBJECT(win)->parent);
		}
	}
	/* Traverse down from c to b */
	if (c != b) {
		path = NULL;
		win = GDK_WINDOW(GDK_WINDOW_OBJECT(b)->parent);
		while (win != c) {
			path = g_slist_prepend(path, win);
			win = GDK_WINDOW(GDK_WINDOW_OBJECT(win)->parent);
		}
		list = path;
		while (list) {
			win = GDK_WINDOW(list->data);
			list = g_slist_next(list);
			if (list) {
				next = GDK_WINDOW(list->data);
			} else {
				next = b;
			}
			event_win = gdk_xynth_pointer_event_window(win, GDK_ENTER_NOTIFY);
			if (event_win) {
				event = gdk_xynth_event_make(event_win, GDK_ENTER_NOTIFY);
				event->crossing.subwindow = g_object_ref(next);
				gdk_window_get_origin(win, &x_int, &y_int);
				event->crossing.x      = x - x_int;
				event->crossing.y      = y - y_int;
				event->crossing.x_root = x;
				event->crossing.y_root = y;
				event->crossing.mode   = mode;
				if (non_linear) {
					event->crossing.detail = GDK_NOTIFY_NONLINEAR_VIRTUAL;
				} else {
					event->crossing.detail = GDK_NOTIFY_VIRTUAL;
				}
				event->crossing.focus = FALSE;
				event->crossing.state = modifiers;
			}
		}
		g_slist_free(path);
	}
	event_win = gdk_xynth_pointer_event_window(b, GDK_ENTER_NOTIFY);
	if (event_win) {
		event = gdk_xynth_event_make(event_win, GDK_ENTER_NOTIFY);
		event->crossing.subwindow = NULL;
		gdk_window_get_origin(b, &x_int, &y_int);
		event->crossing.x      = x - x_int;
		event->crossing.y      = y - y_int;
		event->crossing.x_root = x;
		event->crossing.y_root = y;
		event->crossing.mode   = mode;
		if (non_linear) {
			event->crossing.detail = GDK_NOTIFY_NONLINEAR;
		} else if (c == a) {
			event->crossing.detail = GDK_NOTIFY_ANCESTOR;
		} else {
			event->crossing.detail = GDK_NOTIFY_INFERIOR;
		}
		event->crossing.focus = FALSE;
		event->crossing.state = modifiers;
	}
	if (mode != GDK_CROSSING_GRAB) {
		if (b != gdk_xynth_window_containing_pointer) {
			g_object_unref(gdk_xynth_window_containing_pointer);
			gdk_xynth_window_containing_pointer = g_object_ref(b);
		}
	}
	LEAVE();
}

void gdk_window_set_cursor (GdkWindow *window, GdkCursor *cursor)
{
	GdkCursor *old_cursor;
	GdkWindowImplXYNTH *impl;

	ENTER();
	if (GDK_IS_WINDOW(window) == 0) {
		LEAVE();
		return;
	}
	impl = GDK_WINDOW_IMPL_XYNTH(GDK_WINDOW_OBJECT(window)->impl);
	old_cursor = impl->cursor;

	impl->cursor = (cursor ? gdk_cursor_ref(cursor) : gdk_cursor_new(GDK_LEFT_PTR));
	
	if (gdk_window_at_pointer(NULL, NULL) == window) {
		/* This is a bit evil but we want to keep all cursor changes in
		 * one place, so let gdk_xynth_window_send_crossing_events
		 * do the work for us.
		 */
		gdk_xynth_window_send_crossing_events(window, window, GDK_CROSSING_NORMAL);
	} else if (impl->xynth) {
		GdkCursorXYNTH *xynth_cursor;
		xynth_cursor = (GdkCursorXYNTH *) impl->cursor;
		/* this branch takes care of setting the cursor for unmapped windows */
		DEBUG("set window cursor");
		ASSERT();
	}
	if (old_cursor) {
		gdk_cursor_unref(old_cursor);
	}
	LEAVE();
}

GdkWindow * gdk_xynth_window_new (GdkWindow *parent, GdkWindowAttr *attributes, gint attributes_mask)
{
	gint x;
	gint y;
	GdkWindow          *window;
	GdkWindowObject    *private;
	GdkWindowObject    *parent_private;
	GdkWindowImplXYNTH *impl;
	GdkWindowImplXYNTH *parent_impl;
	GdkVisual          *visual;

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

	gdk_window_set_events(window, attributes->event_mask | GDK_STRUCTURE_MASK);
	
	impl = GDK_WINDOW_IMPL_XYNTH(private->impl);
	impl->drawable.wrapper = GDK_DRAWABLE(window);
	impl->gdkWindow        = window;

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
			private->input_only = FALSE;
			impl->input_only = 0;
			break;
		case GDK_INPUT_ONLY:
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
		case GDK_WINDOW_DIALOG:
		case GDK_WINDOW_TEMP:
			{
				s_rect_t rect;
				rect.x = x;
				rect.y = y;
				rect.w = impl->drawable.width;
				rect.h = impl->drawable.height;
				if (create_xynth_window(impl, parent_impl->xynth, WINDOW_CHILD, &rect) != 0) {
					g_assert(0);
					_gdk_window_destroy(window, FALSE);
					LEAVE();
					return NULL;
				}
			}
			break;
		case GDK_WINDOW_CHILD:
			impl->xynth = NULL;
			if (!private->input_only && parent_impl->drawable.surface) {
				s_rect_t rect;
				rect.x = x;
				rect.y = y;
				rect.w = impl->drawable.width;
				rect.h = impl->drawable.height;
				DEBUG("create child window");
				ASSERT();
			}
			break;
		default:
			g_warning("gdk_window_new: unsupported window type: %d", private->window_type);
			_gdk_window_destroy(window, FALSE);
			LEAVE();
			return NULL;
	}

	if (impl->drawable.surface) {
		GdkColormap *colormap;
		private->depth = impl->drawable.surface->bitsperpixel;
		if ((attributes_mask & GDK_WA_COLORMAP) && attributes->colormap) {
			colormap = attributes->colormap;
		} else {
			if (gdk_visual_get_system () == visual) {
				colormap = gdk_colormap_get_system();
			} else {
				colormap = gdk_drawable_get_colormap(parent);
			}
		}
		gdk_drawable_set_colormap(GDK_DRAWABLE(window), colormap);
	} else {
		private->depth = visual->depth;
	}

	gdk_window_set_cursor(window, ((attributes_mask & GDK_WA_CURSOR) ? (attributes->cursor) : NULL));
	
	ASSERT();
	LEAVE();
	return NULL;
}

GdkWindow * gdk_window_new (GdkWindow *parent, GdkWindowAttr *attributes, gint attributes_mask)
{
	GdkWindow *window;
	ENTER();
	if (attributes == NULL) {
		LEAVE();
		return NULL;
	}
	window = gdk_xynth_window_new(parent, attributes, attributes_mask);
	LEAVE();
	return window;
}
