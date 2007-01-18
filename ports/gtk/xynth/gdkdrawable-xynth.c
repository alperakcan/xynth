
#include "gdk-xynth.h"

static gpointer parent_class = NULL;

static void gdk_xynth_draw_rectangle (GdkDrawable *drawable, GdkGC *gc, gboolean filled, gint x, gint y, gint width, gint height)
{
	int i;
	GdkColor color;
	GdkGCXynth *gc_impl;
	GdkDrawableImplXynth *draw_impl;
	ENT();
	gc_impl = GDK_GC_XYNTH(gc);
	draw_impl = GDK_DRAWABLE_IMPL_XYNTH(drawable);
	if (GDK_GC_XYNTH(gc)->values_mask & GDK_GC_FOREGROUND) {
		color = GDK_GC_XYNTH(gc)->values.foreground;
	} else if (GDK_IS_WINDOW(draw_impl->wrapper)) {
		color = ((GdkWindowObject *) (draw_impl->wrapper))->bg_color;
	} else {
		color.pixel = 0;
	}
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
	if (gc_impl->clip_region) {
		DBG("clip_region->size: %d, numRects: %d", gc_impl->clip_region->size, gc_impl->clip_region->numRects);
		for (i = 0; i < gc_impl->clip_region->numRects; i++) {
			DBG("rect: %d %d %d %d", gc_impl->clip_region->rects[i].x1, gc_impl->clip_region->rects[i].y1,
			                         gc_impl->clip_region->rects[i].x2 - gc_impl->clip_region->rects[i].x1,
			                         gc_impl->clip_region->rects[i].y2 - gc_impl->clip_region->rects[i].y1);
		}
	}
	DBG("filled:%d, x:%d, y:%d, w:%d, h:%d c:%d(%d, %d, %d)", filled, x, y, width, height, color.pixel, color.red, color.green, color.blue);
	if (filled) {
		s_fillbox(draw_impl->object->surface, x, y, width, height, color.pixel);
		DBG("OBJ: %p buf:%d %d %d %d win:%d %d %d %d", draw_impl->object,
		draw_impl->object->surface->buf->x,draw_impl->object->surface->buf->y,draw_impl->object->surface->buf->w,draw_impl->object->surface->buf->h,
		draw_impl->object->surface->win->x,draw_impl->object->surface->win->y,draw_impl->object->surface->win->w,draw_impl->object->surface->win->h);
	} else {
		NIY();
	}
	LEV();
}

static void gdk_xynth_draw_arc (GdkDrawable *drawable, GdkGC *gc, gboolean filled, gint x, gint y, gint width, gint height, gint angle1, gint angle2)
{
	NIY();
}

static void gdk_xynth_draw_polygon (GdkDrawable *drawable, GdkGC *gc, gboolean filled, GdkPoint *points, gint npoints)
{
	NIY();
}

static void gdk_xynth_draw_text (GdkDrawable *drawable, GdkFont *font, GdkGC *gc, gint x, gint y, const gchar *text, gint text_length)
{
	NIY();
}

static void gdk_xynth_draw_text_wc (GdkDrawable *drawable, GdkFont *font, GdkGC *gc, gint x, gint y, const GdkWChar *text, gint text_length)
{
	NIY();
}

static void gdk_xynth_draw_glyphs (GdkDrawable *drawable, GdkGC *gc, PangoFont *font, gint x, gint y, PangoGlyphString *glyphs)
{
	NIY();
}

#include <pango/pangoft2.h>
#include <freetype/ftglyph.h>
static void gdk_xynth_draw_glyphs_transformed (GdkDrawable *drawable, GdkGC *gc, PangoMatrix *matrix, PangoFont *font, gint x, gint y, PangoGlyphString *glyphs)
{
	int i;
	int _x;
	int _y;
	int xpos;
	int ypos;
	FT_Face face;
	FT_UInt glyph_index;
	PangoGlyphInfo *glyph_info;
	GdkDrawableImplXynth *draw_impl;
	ENT();
	
	g_return_if_fail(font);

	draw_impl = GDK_DRAWABLE_IMPL_XYNTH(drawable);
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
	DBG("OBJ: %p buf:%d %d %d %d win:%d %d %d %d", draw_impl->object,
	draw_impl->object->surface->buf->x,draw_impl->object->surface->buf->y,draw_impl->object->surface->buf->w,draw_impl->object->surface->buf->h,
	draw_impl->object->surface->win->x,draw_impl->object->surface->win->y,draw_impl->object->surface->win->w,draw_impl->object->surface->win->h);
	DBG("matrix: %p, x: %d(%d), y: %d(%d)", matrix, x,  PANGO_PIXELS(x), y, PANGO_PIXELS(y));
	if (matrix) {
		DBG("Matrix:: xx: %d, xy: %d, yx: %d, yy: %d", matrix->xx, matrix->xy, matrix->yx, matrix->yy);
		NIY();
	}
	x =  PANGO_PIXELS(x);
	y =  PANGO_PIXELS(y);
	glyph_info = glyphs->glyphs;
	for (i = 0, xpos = 0; i < glyphs->num_glyphs; i++, glyph_info++) {
		if (glyph_info->glyph) {
			glyph_index = glyph_info->glyph;
			face = pango_ft2_font_get_face (font);
			if (face) {
				FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
				if (face->glyph->format != ft_glyph_format_bitmap)
					FT_Render_Glyph(face->glyph, ft_render_mode_normal);
				
				ypos = y - face->glyph->bitmap_top + 1;

				for (_y = 0; _y < face->glyph->bitmap.rows; _y++) {
					for (_x = 0; _x < face->glyph->bitmap.width; _x++) {
						s_setpixelrgba(draw_impl->object->surface,
						(x + PANGO_PIXELS(xpos) + face->glyph->bitmap_left + _x),
						(ypos + _y),
						255, 0, 0,
						(~*(face->glyph->bitmap.buffer + _y * face->glyph->bitmap.pitch + _x) & 0xFF));						
					}
				}
			}
		}
		xpos += glyphs->glyphs[i].geometry.width;
	}
	//NIY();
}

static void gdk_xynth_draw_drawable (GdkDrawable *drawable, GdkGC *gc, GdkPixmap *src, gint xsrc, gint ysrc, gint xdest, gint ydest, gint width, gint height)
{
	int i;
	GdkGCXynth *gc_impl;
	GdkDrawableImplXynth *draw_impl;
	GdkDrawableImplXynth *pdraw_impl;
	ENT();
	DBG("xsrc:%d, ysrc:%d, xdest:%d, ydest:%d, width:%d, height:%d", xsrc, ysrc, xdest, ydest, width, height);
	gc_impl = GDK_GC_XYNTH(gc);
	draw_impl = GDK_DRAWABLE_IMPL_XYNTH(drawable);
	pdraw_impl = GDK_DRAWABLE_IMPL_XYNTH(GDK_PIXMAP_OBJECT(src)->impl);
	if (draw_impl == pdraw_impl) {
		DBG("draw_impl == pdraw_impl");
		NIY();
	}
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
	if (gc_impl->clip_region) {
		DBG("clip_region->size: %d, numRects: %d", gc_impl->clip_region->size, gc_impl->clip_region->numRects);
		for (i = 0; i < gc_impl->clip_region->numRects; i++) {
			DBG("rect: %d %d %d %d", gc_impl->clip_region->rects[i].x1, gc_impl->clip_region->rects[i].y1,
			                         gc_impl->clip_region->rects[i].x2 - gc_impl->clip_region->rects[i].x1,
			                         gc_impl->clip_region->rects[i].y2 - gc_impl->clip_region->rects[i].y1);
		}
	}
	DBG("OBJ: %p buf:%d %d %d %d win:%d %d %d %d", draw_impl->object,
	draw_impl->object->surface->buf->x,draw_impl->object->surface->buf->y,draw_impl->object->surface->buf->w,draw_impl->object->surface->buf->h,
	draw_impl->object->surface->win->x,draw_impl->object->surface->win->y,draw_impl->object->surface->win->w,draw_impl->object->surface->win->h);
	s_putboxpart(draw_impl->object->surface, xdest, ydest, width, height, pdraw_impl->object->surface->width, pdraw_impl->object->surface->height, pdraw_impl->object->surface->vbuf, xsrc, ysrc);
	w_object_update(draw_impl->object, draw_impl->object->surface->win);
	LEV();
}

static void gdk_xynth_draw_image (GdkDrawable *drawable, GdkGC *gc, GdkImage *image, gint xsrc, gint ysrc, gint xdest, gint ydest, gint width, gint height)
{
	NIY();
}

static void gdk_xynth_draw_points (GdkDrawable *drawable, GdkGC *gc, GdkPoint *points, gint npoints)
{
	NIY();
}

static void gdk_xynth_draw_segments (GdkDrawable *drawable, GdkGC *gc, GdkSegment *segs, gint nsegs)
{
	int i;
	GdkColor color;
	GdkGCXynth *gc_impl;
	GdkDrawableImplXynth *draw_impl;
	ENT();
	gc_impl = GDK_GC_XYNTH(gc);
	draw_impl = GDK_DRAWABLE_IMPL_XYNTH(drawable);
	if (GDK_GC_XYNTH(gc)->values_mask & GDK_GC_FOREGROUND) {
		color = GDK_GC_XYNTH(gc)->values.foreground;
	} else if (GDK_IS_WINDOW(draw_impl->wrapper)) {
		color = ((GdkWindowObject *) (draw_impl->wrapper))->bg_color;
	} else {
		color.pixel = 0;
	}
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
	DBG("OBJ: %p buf:%d %d %d %d win:%d %d %d %d (%dx%d)", draw_impl->object,
	draw_impl->object->surface->buf->x,draw_impl->object->surface->buf->y,draw_impl->object->surface->buf->w,draw_impl->object->surface->buf->h,
	draw_impl->object->surface->win->x,draw_impl->object->surface->win->y,draw_impl->object->surface->win->w,draw_impl->object->surface->win->h,
	draw_impl->object->surface->width, draw_impl->object->surface->height);
	if (gc_impl->clip_region) {
		DBG("clip_region->size: %d, numRects: %d", gc_impl->clip_region->size, gc_impl->clip_region->numRects);
		for (i = 0; i < gc_impl->clip_region->numRects; i++) {
			DBG("rect: %d %d %d %d", gc_impl->clip_region->rects[i].x1, gc_impl->clip_region->rects[i].y1,
			                         gc_impl->clip_region->rects[i].x2 - gc_impl->clip_region->rects[i].x1,
			                         gc_impl->clip_region->rects[i].y2 - gc_impl->clip_region->rects[i].y1);
		}
	}
	for(i = 0; i < nsegs; i++) {
		DBG("srect: %d %d %d %d, %d", segs[i].x1, segs[i].y1, segs[i].x2, segs[i].y2, color.pixel);
		s_line(draw_impl->object->surface, segs[i].x1, segs[i].y1, segs[i].x2, segs[i].y2, color.pixel);
	}
	LEV();
}

static void gdk_xynth_draw_lines (GdkDrawable *drawable, GdkGC *gc, GdkPoint *points, gint npoints)
{
	NIY();
}

static GdkColormap * gdk_xynth_get_colormap (GdkDrawable *drawable)
{
	GdkColormap *retval;
	GdkDrawableImplXynth *draw_impl;
	ENT();
	draw_impl = GDK_DRAWABLE_IMPL_XYNTH(drawable);
	retval = draw_impl->colormap;
	if (!retval) {
		retval = gdk_colormap_get_system();
	}
	LEV();
	return retval;
}

static void gdk_xynth_set_colormap (GdkDrawable *drawable, GdkColormap *colormap)
{
	GdkDrawableImplXynth *impl;
	ENT();
	impl = GDK_DRAWABLE_IMPL_XYNTH(drawable);
	if (impl->colormap == colormap) {
		return;
	}
	if (impl->colormap) {
		g_object_unref(impl->colormap);
	}
	impl->colormap = colormap;
	if (impl->colormap) {
		g_object_ref(impl->colormap);
	}
	LEV();
}

static gint gdk_xynth_get_depth (GdkDrawable *drawable)
{
	ENT();
	LEV();
	return gdk_drawable_get_depth(GDK_DRAWABLE_IMPL_XYNTH(drawable)->wrapper);
}

static GdkScreen * gdk_xynth_get_screen (GdkDrawable *drawable)
{
	ENT();
	LEV();
	return gdk_screen_get_default();
}

static GdkVisual * gdk_xynth_get_visual (GdkDrawable *drawable)
{
	NIY();
}

static void gdk_xynth_get_size (GdkDrawable *drawable, gint *width, gint *height)
{
	ENT();
	g_return_if_fail(GDK_IS_WINDOW_IMPL_XYNTH(drawable));
	if (width) {
		*width = GDK_WINDOW_IMPL_XYNTH(drawable)->width;
	}
	if (height) {
		*height = GDK_WINDOW_IMPL_XYNTH(drawable)->height;
	}
	DBG("width:%d, height:%d", *width, *height);
	LEV();
}

static GdkImage * gdk_xynth_copy_to_image (GdkDrawable *drawable, GdkImage *image, gint src_x, gint src_y, gint dest_x, gint dest_y, gint width, gint height)
{
	NIY();
}

static void gdk_xynth_drawable_finalize (GObject *object)
{
	ENT();
	gdk_drawable_set_colormap(GDK_DRAWABLE(object), NULL);
	G_OBJECT_CLASS (parent_class)->finalize (object);
	LEV();
}

static void gdk_drawable_impl_xynth_class_init (GdkDrawableImplXynthClass *klass)
{
	GdkDrawableClass *drawable_class = GDK_DRAWABLE_CLASS(klass);
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	ENT();
	parent_class = g_type_class_peek_parent(klass);
	object_class->finalize = gdk_xynth_drawable_finalize;
	drawable_class->create_gc = gdk_xynth_gc_new;
	drawable_class->draw_rectangle = gdk_xynth_draw_rectangle;
	drawable_class->draw_arc = gdk_xynth_draw_arc;
	drawable_class->draw_polygon = gdk_xynth_draw_polygon;
	drawable_class->draw_text = gdk_xynth_draw_text;
	drawable_class->draw_text_wc = gdk_xynth_draw_text_wc;
	drawable_class->draw_drawable = gdk_xynth_draw_drawable;
	drawable_class->draw_points = gdk_xynth_draw_points;
	drawable_class->draw_segments = gdk_xynth_draw_segments;
	drawable_class->draw_lines = gdk_xynth_draw_lines;
	drawable_class->draw_glyphs = gdk_xynth_draw_glyphs;
	drawable_class->draw_glyphs_transformed = gdk_xynth_draw_glyphs_transformed;
	drawable_class->draw_image = gdk_xynth_draw_image;
	drawable_class->set_colormap = gdk_xynth_set_colormap;
	drawable_class->get_colormap = gdk_xynth_get_colormap;
	drawable_class->get_size = gdk_xynth_get_size;
	drawable_class->get_depth = gdk_xynth_get_depth;
	drawable_class->get_screen = gdk_xynth_get_screen;
	drawable_class->get_visual = gdk_xynth_get_visual;
	drawable_class->_copy_to_image = gdk_xynth_copy_to_image;
	LEV();
}

GType _gdk_drawable_impl_xynth_get_type (void)
{
	static GType object_type = 0;
	ENT();
	if (!object_type) {
		static const GTypeInfo object_info = {
			sizeof(GdkDrawableImplXynthClass),
			(GBaseInitFunc) NULL,
			(GBaseFinalizeFunc) NULL,
			(GClassInitFunc) gdk_drawable_impl_xynth_class_init,
			NULL,           /* class_finalize */
			NULL,           /* class_data */
			sizeof(GdkDrawableImplXynth),
			0,              /* n_preallocs */
			(GInstanceInitFunc) NULL,
		};
		object_type = g_type_register_static(GDK_TYPE_DRAWABLE, "GdkDrawableImplXynth", &object_info, 0);
	}
	LEV();
	return object_type;
}
