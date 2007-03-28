
#include <math.h>
#include "xynth_.h"

static inline double xround (double x)
{
	return floor(x + 0.5);
}

int s_render_init (s_render_t **render, S_RENDER_TYPE type, unsigned int width, unsigned int height)
{
	s_render_t *rnd;
	rnd = (s_render_t *) s_malloc(sizeof(s_render_t));
	memset(rnd, 0, sizeof(s_render_t));
	rnd->type = type;
	rnd->width = width;
	rnd->height = height;
	rnd->stride = width * sizeof(unsigned int);
	rnd->argb = (unsigned int *) s_malloc(rnd->stride * rnd->height);
	memset(rnd->argb, 0xff000000, rnd->stride * rnd->height);
	rnd->clip.x = 0;
	rnd->clip.y = 0;
	rnd->clip.w = rnd->width;
	rnd->clip.h = rnd->height;
	s_list_init(&(rnd->clips));
	s_render_set_clip_region(rnd, 0, NULL);
	*render = rnd;
	return 0;
}

int s_render_uninit (s_render_t *render)
{
	s_rect_t *rect;
	if (render == NULL) {
		return 0;
	}
	while (!s_list_eol(render->clips, 0)) {
		rect = (s_rect_t *) s_list_get(render->clips, 0);
		s_list_remove(render->clips, 0);
		s_free(rect);
	}
	s_list_uninit(render->clips);
	s_free(render->argb);
	s_free(render);
	return 0;
}

int s_render_set_clip_region (s_render_t *render, int nrects, s_rect_t *rects)
{
	int i;
	s_rect_t *rect;
	while (!s_list_eol(render->clips, 0)) {
		rect = (s_rect_t *) s_list_get(render->clips, 0);
		s_list_remove(render->clips, 0);
		s_free(rect);
	}
	for (i = 0; i < nrects; i++) {
		rect = (s_rect_t *) s_malloc(sizeof(s_rect_t));
		rect->x = rects[i].x;
		rect->y = rects[i].y;
		rect->w = rects[i].w;
		rect->h = rects[i].h;
		s_list_add(render->clips, rect, -1);
	}
	return 0;
}

static int s_render_fill_rectangle_operator_clear (s_render_t *render, s_rect_t *rect, unsigned char alpha, unsigned char red, unsigned char green, unsigned char blue)
{
	unsigned int w;
	unsigned int h;
	unsigned int c;
	unsigned int dw; 
	unsigned int *d;
	c = 0x00000000;
	d = render->argb + (rect->y * render->width + rect->x);
	dw = render->width - rect->w;
	for (h = rect->h; h; h--) {
		for (w = rect->w; w; w--) {
			*d++ = c;
		}
		d += dw;
	}
	return 0;
}

static int s_render_fill_rectangle_operator_source (s_render_t *render, s_rect_t *rect, unsigned char alpha, unsigned char red, unsigned char green, unsigned char blue)
{
	unsigned int w;
	unsigned int h;
	unsigned int c;
	unsigned int dw; 
	unsigned int *d;
	c = (alpha << 0x18) | (red << 0x10) | (green << 0x08) | (blue << 0x00);
	d = render->argb + (rect->y * render->width + rect->x);
	dw = render->width - rect->w;
	for (h = rect->h; h; h--) {
		for (w = rect->w; w; w--) {
			*d++ = c;
		}
		d += dw;
	}
	return 0;
}

static int s_render_fill_rectangle_operator_source_over (s_render_t *render, s_rect_t *rect, unsigned char alpha, unsigned char red, unsigned char green, unsigned char blue)
{
	unsigned int w;
	unsigned int h;
	unsigned int dw; 
	unsigned int *d;
	unsigned char ma;
	unsigned char sa;
	unsigned char sr;
	unsigned char sg;
	unsigned char sb;
	unsigned char da;
	unsigned char dr;
	unsigned char dg;
	unsigned char db;
	ma = alpha;
	sa = 255;
	sr = red;
	sg = green;
	sb = blue;
	d = render->argb + (rect->y * render->width + rect->x);
	dw = render->width - rect->w;
	for (h = rect->h; h; h--) {
		for (w = rect->w; w; w--) {
			da = *d >> 0x18;
			dr = *d >> 0x10;
			dg = *d >> 0x08;
			db = *d >> 0x00;
			da = xround(ma * (sa - da) / 255.00 + da);
			dr = xround(ma * (sr - dr) / 255.00 + dr);
			dg = xround(ma * (sg - dg) / 255.00 + dg);
			db = xround(ma * (sb - db) / 255.00 + db);
			*d++ = (da << 0x18) | (dr << 0x10) | (dg << 0x08) | (db << 0x00);
		}
		d += dw;
	}
	return 0;
}

int s_render_fill_rectangle (s_render_t *render, S_RENDER_OPERATOR operator, int x, int y, int width, int height, unsigned char alpha, unsigned char red, unsigned char green, unsigned char blue)
{
	int p;
	s_rect_t clip;
	s_rect_t rect;
	s_rect_t *tmp;
	int (*func) (s_render_t *, s_rect_t *, unsigned char, unsigned char, unsigned char, unsigned char);
	clip.x = x;
	clip.y = y;
	clip.w = width;
	clip.h = height;
	if (s_rect_intersect(&render->clip, &clip, &rect)) {
		return -1;
	}
	switch (operator) {
		case S_RENDER_OPERATOR_CLEAR:
			func = s_render_fill_rectangle_operator_clear;
			break;
		case S_RENDER_OPERATOR_SOURCE:
			func = s_render_fill_rectangle_operator_source;
			break;
		case S_RENDER_OPERATOR_SOURCE_OVER:
			func = s_render_fill_rectangle_operator_source_over;
			break;
		default:
			debugf(DFAT, "Not implemented (%x)", operator);
			return -1;
	}
	if (s_list_eol(render->clips, 0) != 0) {
		return func(render, &rect, alpha, red, green, blue);
	}
	for (p = 0; !s_list_eol(render->clips, p); p++) {
		tmp = (s_rect_t *) s_list_get(render->clips, p);
		if (s_rect_intersect(&rect, tmp, &clip) == 0){
			func(render, &clip, alpha, red, green, blue);
		}
	}
	return 0;
}

int s_render_putboxpartmask_operator_source_over (s_render_t *destination, s_render_t *source, s_render_t *mask, int dst_x, int dst_y, int src_x, int src_y, int mask_x, int mask_y, int width, int height)
{
	int w;
	int h;
	int dw = 0;
	int sw = 0;
	int mw = 0;
	unsigned char ma;
	unsigned char sa;
	unsigned char sr;
	unsigned char sg;
	unsigned char sb;
	unsigned char da;
	unsigned char dr;
	unsigned char dg;
	unsigned char db;
	unsigned int *d = NULL;
	unsigned int *s = NULL;
	unsigned int *m = NULL;
	d = destination->argb + (dst_y * destination->width + dst_x);
	dw = destination->width - width;
	if (source) {
		s = source->argb + (src_y * source->width + src_x);
		sw = source->width - width;
	}
	if (mask) {
		m = mask->argb + (mask_y * mask->width + mask_x);
		mw = mask->width - width;
	}
	for (h = 0; h < height; h++) {
		for (w = 0; w < width; w++) {
			ma = *m >> 0x18;
			sa = *s >> 0x18;
			sr = *s >> 0x10;
			sg = *s >> 0x08;
			sb = *s >> 0x00;
			da = *d >> 0x18;
			dr = *d >> 0x10;
			dg = *d >> 0x08;
			db = *d >> 0x00;
			da = xround(ma * (sa - da) / 255.00 + da);
			dr = xround(ma * (sr - dr) / 255.00 + dr);
			dg = xround(ma * (sg - dg) / 255.00 + dg);
			db = xround(ma * (sb - db) / 255.00 + db);
			*d = (da << 0x18) | (dr << 0x10) | (dg << 0x08) | (db << 0x00);
			m++;
			d++;
			s++;
		}
		d += dw;
		s += sw;
		m += mw;
	}
	return 0;
}

int s_render_putboxpartmask (s_render_t *render, S_RENDER_OPERATOR operator, s_render_t *source, s_render_t *mask, int dst_x, int dst_y, int src_x, int src_y, int mask_x, int mask_y, int width, int height)
{
	int w;
	int h;
	int p;
	int x_offset;
	int y_offset;
	s_rect_t clip;
	s_rect_t rect;
	s_rect_t *tmp;
	int (*func) (s_render_t *, s_render_t *, s_render_t *, int, int, int, int, int, int , int, int);
	printf("putboxmaskpart operator: 0x%08x, dst: %d %d, src: %d %d, msk: %d %d, (%dx%d) (%p %p %p)\n", operator, dst_x, dst_y, src_x, src_y, mask_x, mask_y, width, height, render, source, mask);
	printf("clips: %d %d %d\n", (render) ? render->clips->nb_elt : 0, (source) ? source->clips->nb_elt : 0, (mask) ? mask->clips->nb_elt : 0); 
#if 1
	if (render) {
		printf("Destination:\n");
		for (h = 0; h < render->height; h++) {
			for (w = 0; w < render->width; w++) {
				printf("%08x ", render->argb[h * render->width + w]);
			}
			printf("\n");
		}
	}
	if (source) {
		printf("Source:\n");
		for (h = 0; h < source->height; h++) {
			for (w = 0; w < source->width; w++) {
				printf("%08x ", source->argb[h * source->width + w]);
			}
			printf("\n");
		}
	}
	if (mask) {
		printf("Mask:\n");
		for (h = 0; h < mask->height; h++) {
			for (w = 0; w < mask->width; w++) {
				printf("%08x ", mask->argb[h * mask->width + w]);
			}
			printf("\n");
		}
	}
#endif
	clip.x = dst_x;
	clip.y = dst_y;
	clip.w = width;
	clip.h = height;
	switch (operator) {
		case S_RENDER_OPERATOR_SOURCE_OVER:
			func = s_render_putboxpartmask_operator_source_over;
			break;
		default:
			//debugf(DFAT, "Not implemented (%x)", operator);
			return -1;
	}
	if (s_rect_intersect(&render->clip, &clip, &rect)) {
		return -1;
	}
	if (s_list_eol(render->clips, 0) != 0) {
		x_offset = rect.x - dst_x;
		y_offset = rect.y - dst_y;
		return func(render, source, mask, dst_x + x_offset, dst_y + y_offset,
		                                  src_x + x_offset, src_y + y_offset,
		                                  mask_x + x_offset, mask_y + y_offset,
		                                  rect.w, rect.h);
	}
	for (p = 0; !s_list_eol(render->clips, p); p++) {
		tmp = (s_rect_t *) s_list_get(render->clips, p);
		if (s_rect_intersect(&rect, tmp, &clip) == 0){
			x_offset = clip.x - dst_x;
			y_offset = clip.y - dst_y;
			func(render, source, mask, dst_x + x_offset, dst_y + y_offset,
		                                   src_x + x_offset, src_y + y_offset,
		                                   mask_x + x_offset, mask_y + y_offset,
		                                   clip.w, clip.h);
		}
	}
	return 0;
}

int s_render_getbox (s_render_t *render, s_render_t **dest, int x, int y, int width, int height)
{
	int w;
	int h;
	s_render_t *dst;
	s_render_init(&dst, render->type, width, height);
	for (h = 0; h < height; h++) {
		for (w = 0; w < width; w++) {
			dst->argb[h * dst->width + w] = render->argb[(y + h) * render->width + (x + w)];
		}
	}
	*dest = dst;
	return 0;
}

int s_render_putbox (s_render_t *render, S_RENDER_OPERATOR operator, s_render_t *source, int x, int y)
{
	return s_render_putboxpartmask(render, operator, source, NULL, x, y, 0, 0, 0, 0, source->width, source->height); 
}
