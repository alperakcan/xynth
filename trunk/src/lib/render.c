
#include <math.h>
#include "xynth_.h"

int s_render_color_init (s_render_color_t **color);
int s_render_color_uninit (s_render_color_t *color);
int s_render_color_rgba (s_render_color_t *color, double alpha, double red, double green, double blue);
int s_render_init (s_render_t **render, S_RENDER_TYPE type, unsigned int width, unsigned int height);
int s_render_uninit (s_render_t *render);
int s_render_set_clip_region (s_render_t *render, int nrects, s_rect_t *rects);
int s_render_fill_rectangle (s_render_t *render, S_RENDER_OPERATOR operator, s_render_color_t *color, int x, int y, int width, int height);
int s_render_fill_rectangles (s_render_t *render, S_RENDER_OPERATOR operator, s_render_color_t *color, int nrects, s_rect_t *rects);

static inline double xround (double x)
{
	return floor(x + 0.5);
}

int s_render_color_init (s_render_color_t **color)
{
	s_render_color_t *clr;
	clr = s_malloc(sizeof(s_render_color_t));
	memset(clr, 0, sizeof(s_render_color_t));
	*color = clr;
	return 0;
}

int s_render_color_uninit (s_render_color_t *color)
{
	s_free(color);
	return 0;
}

int s_render_init (s_render_t **render, S_RENDER_TYPE type, unsigned int width, unsigned int height)
{
	int w;
	int h;
	s_render_t *rnd;
	s_render_color_t *clr;
	
	if ((width == 0) || (height == 0) || ((type & S_RENDER_TYPE_MASK) == 0)) {
		return -1;
	}
	
	rnd = (s_render_t *) s_malloc(sizeof(s_render_t));
	memset(rnd, 0, sizeof(s_render_t));
	rnd->type = type;
	rnd->width = width;
	rnd->height = height;
	
	rnd->colors = (s_render_color_t *) s_malloc(sizeof(s_render_color_t) * rnd->width * rnd->height);
	memset(rnd->colors, 0, sizeof(s_render_color_t) * rnd->width * rnd->height);
	
	if (rnd->type == S_RENDER_TYPE_RGB) {
		clr = rnd->colors;
		for (h = 0; h < height; h++) {
			for (w = 0; w < width; w++) {
				clr->alpha = 1.0;
				clr++;
			}
		}
	}


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
	s_free(render->colors);
	s_free(render);
	return 0;
}

int s_render_set_repeat (s_render_t *render, S_RENDER_REPEAT repeat)
{
	render->repeat = repeat;
	return 0;
}

int s_render_color_rgba (s_render_color_t *color, double alpha, double red, double green, double blue)
{
	/* store premultiplied color values */
	color->alpha = alpha;
	color->red = red * alpha;
	color->green = green * alpha;
	color->blue = blue * alpha;
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

static void s_render_fill_rectangle_operator_clear (s_render_t *render, s_render_color_t *color, s_rect_t *rect)
{
	int w;
	int h;
	int w_add;
	s_render_color_t *rnd_clr;
	rnd_clr = render->colors + rect->y * render->width + rect->x;
	w_add = render->width - rect->w;
	for (h = 0; h < rect->h; h++) {
		for (w = 0; w < rect->w; w++) {
			rnd_clr->alpha = 0;
			rnd_clr->red = 0;
			rnd_clr->green = 0;
			rnd_clr->blue = 0;
			rnd_clr++;
		}
		rnd_clr += w_add;
	}
} 

static void s_render_fill_rectangle_operator_source (s_render_t *render, s_render_color_t *color, s_rect_t *rect)
{
	int w;
	int h;
	int w_add;
	s_render_color_t *rnd_clr;
	rnd_clr = render->colors + rect->y * render->width + rect->x;
	w_add = render->width - rect->w;
	for (h = 0; h < rect->h; h++) {
		for (w = 0; w < rect->w; w++) {
			rnd_clr->alpha = color->alpha;
			rnd_clr->red = color->red ;
			rnd_clr->green = color->green;
			rnd_clr->blue = color->blue;
			rnd_clr++;
		}
		rnd_clr += w_add;
	}
} 

static void s_render_fill_rectangle_operator_over (s_render_t *render, s_render_color_t *color, s_rect_t *rect)
{
	int w;
	int h;
	int w_add;
	s_render_color_t *rnd_clr;
	rnd_clr = render->colors + rect->y * render->width + rect->x;
	w_add = render->width - rect->w;
	for (h = 0; h < rect->h; h++) {
		for (w = 0; w < rect->w; w++) {
			rnd_clr->alpha = color->alpha + rnd_clr->alpha * (1 - color->alpha);
			rnd_clr->red = color->red + rnd_clr->red * (1 - color->alpha);
			rnd_clr->green = color->green + rnd_clr->green * (1 - color->alpha);
			rnd_clr->blue = color->blue + rnd_clr->blue * (1 - color->alpha);
			rnd_clr++;
		}
		rnd_clr += w_add;
	}
} 

int s_render_fill_rectangle (s_render_t *render, S_RENDER_OPERATOR operator, s_render_color_t *color, int x, int y, int width, int height)
{
	int p;
	s_rect_t *tmp;
	s_rect_t rect;
	s_rect_t clip;
	void (*function) (s_render_t *, s_render_color_t *, s_rect_t *);

	switch (operator) {
		case S_RENDER_OPERATOR_CLEAR:
			function = s_render_fill_rectangle_operator_clear;
			break;
		case S_RENDER_OPERATOR_SOURCE:
			function = s_render_fill_rectangle_operator_source;
			break;
		case S_RENDER_OPERATOR_OVER:
			function = s_render_fill_rectangle_operator_over;
			break;
		default:
			debugf(DFAT, "NIY");
			return -1;
	}

	rect.x = x;
	rect.y = y;
	rect.w = width;
	rect.h = height;
	if (s_rect_intersect(&render->clip, &rect, &clip)) {
		debugf(DFAT, "NIY");
		return -1;
	}
	
	if (s_list_eol(render->clips, 0)) {
		function(render, color, &clip);
	} else {
		for (p = 0; !s_list_eol(render->clips, p); p++) {
			tmp = (s_rect_t *) s_list_get(render->clips, p);
			if (s_rect_intersect(&clip, tmp, &rect) == 0) {
				function(render, color, &rect);
			}
		}
	}

	return 0;
}

int s_render_fill_rectangles (s_render_t *render, S_RENDER_OPERATOR operator, s_render_color_t *color, int nrects, s_rect_t *rects)
{
	int n;
	for (n = 0; n < nrects; n++) {
		s_render_fill_rectangle(render, operator, color, rects[n].x, rects[n].y, rects[n].w, rects[n].h);
	}
	return 0;
}

static void s_render_composite_operator_source (s_render_t *dst, s_render_t *src,
                                                int dst_x, int dst_y, int src_x, int src_y,
                                                int width, int height)
{
	int w;
	int h;
	int sw;
	int dw;
	s_render_color_t *s;
	s_render_color_t *d;
	sw = src->width - width;
	dw = dst->width - width;
	s = src->colors + src_y * src->width + src_x;
	d = dst->colors + dst_y * dst->width + dst_x;
	for (h = 0; h < height; h++) {
		for (w = 0; w < width; w++) {
			d->alpha = s->alpha;
			d->red   = s->red;
			d->green = s->green;
			d->blue  = s->blue;
			s++;
			d++;
		}
		s += sw;
		d += dw;
	}
}

static void s_render_composite_operator_over (s_render_t *dst, s_render_t *src,
                                                int dst_x, int dst_y, int src_x, int src_y,
                                                int width, int height)
{
	int w;
	int h;
	int sw;
	int dw;
	s_render_color_t *s;
	s_render_color_t *d;
	sw = src->width - width;
	dw = dst->width - width;
	s = src->colors + src_y * src->width + src_x;
	d = dst->colors + dst_y * dst->width + dst_x;
	for (h = 0; h < height; h++) {
		for (w = 0; w < width; w++) {
			d->alpha = s->alpha + d->alpha * (1 - s->alpha);
			d->red   = s->red + d->red * (1 - s->alpha);;
			d->green = s->green + d->green * (1 - s->alpha);
			d->blue  = s->blue + d->blue * (1 - s->alpha);
			s++;
			d++;
		}
		s += sw;
		d += dw;
	}
}

static void s_render_composite_operator_in_reverse (s_render_t *dst, s_render_t *src,
                                                    int dst_x, int dst_y, int src_x, int src_y,
                                                    int width, int height)
{
	int w;
	int h;
	int sw;
	int dw;
	s_render_color_t *s;
	s_render_color_t *d;
	sw = src->width - width;
	dw = dst->width - width;
	s = src->colors + src_y * src->width + src_x;
	d = dst->colors + dst_y * dst->width + dst_x;
	for (h = 0; h < height; h++) {
		for (w = 0; w < width; w++) {
			d->alpha = d->alpha * s->alpha;
			d->red   = d->red * s->alpha;
			d->green = d->green * s->alpha;
			d->blue  = d->blue * s->alpha;
			s++;
			d++;
		}
		s += sw;
		d += dw;
	}
}

static int s_render_composite_ (s_render_t *render, S_RENDER_OPERATOR operator, s_render_t *source,
                                int dst_x, int dst_y, int src_x, int src_y,
                                int width, int heigt)
{
	int p;
	s_rect_t *tmp;
	s_rect_t rect_dst;
	s_rect_t rect_src;
	s_rect_t clip_dst;
	s_rect_t clip_src;
	void (*function) (s_render_t *, s_render_t *, int, int, int, int, int, int);

	switch (operator) {
		case S_RENDER_OPERATOR_SOURCE:
			function = s_render_composite_operator_source;
			break;
		case S_RENDER_OPERATOR_OVER:
			function = s_render_composite_operator_over;
			break;
		case S_RENDER_OPERATOR_IN_REVERSE:
			function = s_render_composite_operator_in_reverse;
			break;
		default:
			debugf(DFAT, "NIY");
			return -1;
	}
	
	rect_src.x = src_x;
	rect_src.y = src_y;
	rect_src.w = width;
	rect_src.h = heigt;
	if (s_rect_intersect(&source->clip, &rect_src, &clip_src)) {
		debugf(DFAT, "NIY");
		return -1;
	}
	//printf("src: %d %d %d %d, clip: %d %d %d %d\n", rect_src.x, rect_src.y, rect_src.w, rect_src.h, clip_src.x, clip_src.y, clip_src.w, clip_src.h);

	rect_dst.x = dst_x + clip_src.x - rect_src.x;
	rect_dst.y = dst_y + clip_src.y - rect_src.y;
	rect_dst.w = clip_src.w;
	rect_dst.h = clip_src.h;
	//printf("src: %d %d %d %d, clip: %d %d %d %d\n", rect_dst.x, rect_dst.y, rect_dst.w, rect_dst.h, clip_dst.x, clip_dst.y, clip_dst.w, clip_dst.h);
	if (s_rect_intersect(&render->clip, &rect_dst, &clip_dst)) {
		debugf(DFAT, "NIY");
		return -1;
	}
	//printf("src: %d %d %d %d, clip: %d %d %d %d\n", rect_dst.x, rect_dst.y, rect_dst.w, rect_dst.h, clip_dst.x, clip_dst.y, clip_dst.w, clip_dst.h);
	
	clip_src.x = clip_src.x + clip_dst.x - rect_dst.x;
	clip_src.y = clip_src.y + clip_dst.y - rect_dst.y;
	clip_src.w = clip_dst.w;
	clip_src.h = clip_dst.h; 
	
	
	if (s_list_eol(render->clips, 0)) {
		function(render, source, clip_dst.x, clip_dst.y, clip_src.x, clip_src.y, clip_src.w, clip_src.h);
	} else {
		for (p = 0; !s_list_eol(render->clips, p); p++) {
			tmp = (s_rect_t *) s_list_get(render->clips, p);
			if (s_rect_intersect(&clip_dst, tmp, &rect_dst) == 0) {
				function(render, source, rect_dst.x, rect_dst.y, clip_src.x + (rect_dst.x - clip_dst.x), clip_src.y + (rect_dst.y - clip_dst.y), rect_dst.w, rect_dst.h);
			}
		}
	}

	return 0;
}

int s_render_composite (s_render_t *render, S_RENDER_OPERATOR operator, s_render_t *source, s_render_t *mask,
                        int dst_x, int dst_y, int src_x, int src_y, int mask_x, int mask_y,
                        int width, int height)
{
	int x;
	int y;
	s_render_t *tmp;

#define mod(a,b)	((b) == 1 ? 0 : (a) % (b))

	printf("%d %d (%dx%d), %d %d (%dx%d), %d %d (%dx%d), (%d %d)\n",
	       dst_x, dst_y, render->width, render->height,
	       src_x, src_y, source->width, source->height,
	       mask_x, mask_y, (mask) ? mask->width : 0, (mask) ? mask->height : 0,
	       width, height);

	s_render_init(&tmp, render->type, width, height);

	if (source->repeat == S_RENDER_REPEAT_NONE) {
		s_render_composite_(tmp, S_RENDER_OPERATOR_SOURCE, source, 0, 0, src_x, src_y, width, height);
	} else if (source->repeat == S_RENDER_REPEAT_TILE) {
		for (y = 0; y < height; y+= source->height) {
			for (x = 0; x < width; x+= source->width) {
				s_render_composite_(tmp, S_RENDER_OPERATOR_SOURCE, source, x, y, 0, 0, source->width, source->height);
			}
		} 
	}
#if 0
	{
		s_bitmap_t *bm_;
		printf("source\n");
		s_bitmap_from_render(&bm_, source);
		s_bitmap_uninit(bm_);
		printf("after source\n");
		s_bitmap_from_render(&bm_, tmp);
		s_bitmap_uninit(bm_);
	}
#endif
	if (mask) {
		if (mask->repeat == S_RENDER_REPEAT_NONE) {
			s_render_composite_(tmp, S_RENDER_OPERATOR_IN_REVERSE, mask, 0, 0, mask_x, mask_y, width, height);
		} else if (mask->repeat == S_RENDER_REPEAT_TILE) {
			for (y = 0; y < height; y+= mask->height) {
				for (x = 0; x < width; x+= mask->width) {
					s_render_composite_(tmp, S_RENDER_OPERATOR_IN_REVERSE, mask, x, y, 0, 0, mask->width, mask->height);
				}
			} 
		}
#if 0
		{
			s_bitmap_t *bm_;
			printf("mask\n");
			s_bitmap_from_render(&bm_, mask);
			s_bitmap_uninit(bm_);
			printf("after mask\n");
			s_bitmap_from_render(&bm_, tmp);
			s_bitmap_uninit(bm_);
		}
#endif
	}
	s_render_composite_(render, operator, tmp, dst_x, dst_y, 0, 0, width, height);
	s_render_uninit(tmp);
	return 0;
}

int s_bitmap_init (s_bitmap_t **bitmap,
                   unsigned int bitsperpixel,
                   unsigned int width, unsigned int height, unsigned int stride,
                   unsigned int alpha_mask, unsigned int red_mask,
                   unsigned int green_mask, unsigned int blue_mask)
{
	s_bitmap_t *bm;
	bm = (s_bitmap_t *) s_malloc(sizeof(s_bitmap_t));
	memset(bm, 0, sizeof(s_bitmap_t));
	bm->bitsperpixel = bitsperpixel;
	bm->bytesperpixel = bitsperpixel / 8; 
	bm->width = width;
	bm->height = height;
	bm->stride = stride;
	bm->alpha_mask = alpha_mask;
	bm->red_mask = red_mask;
	bm->green_mask = green_mask;
	bm->blue_mask = blue_mask;
	bm->bitmap = (void *) s_malloc(bm->height * bm->stride);
	memset(bm->bitmap, 0, bm->height * bm->stride);
	*bitmap = bm;
	return 0;
}

int s_bitmap_uninit (s_bitmap_t *bitmap)
{
	s_free(bitmap->bitmap);
	s_free(bitmap);
	return 0;
}

int s_bitmap_init_from_data (s_bitmap_t **bitmap,
                             unsigned int bitsperpixel,
                             unsigned int width, unsigned int height, unsigned int stride,
                             unsigned int alpha_mask, unsigned int red_mask,
                             unsigned int green_mask, unsigned int blue_mask,
                             void *data)
{
	s_bitmap_t *bm;
	s_bitmap_init(&bm, bitsperpixel, width, height, stride, alpha_mask, red_mask, green_mask, blue_mask);
	memcpy(bm->bitmap, data, bm->height * bm->stride);
	*bitmap = bm;
	return 0;
}

int s_render_from_bitmap (s_render_t **render, s_bitmap_t *bitmap)
{
	s_render_t *rnd;
	s_render_init(&rnd, S_RENDER_TYPE_ARGB, bitmap->width, bitmap->height);
	if (bitmap->bitsperpixel == 8) {
		int w;
		int h;
		int s;
		char *data = bitmap->bitmap;
		s_render_color_t *clr = rnd->colors;
		s = (bitmap->stride / bitmap->bytesperpixel) - bitmap->width;
//		printf("render\n");
		for (h = 0; h < bitmap->height; h++) {
			for (w = 0; w < bitmap->width; w++) {
				clr->alpha = (bitmap->alpha_mask) ? (*data & bitmap->alpha_mask) / (double) bitmap->alpha_mask : 1.00;
				clr->red   = (bitmap->red_mask) ? (*data & bitmap->red_mask) / (double) bitmap->red_mask : 0;
				clr->green = (bitmap->green_mask) ? (*data & bitmap->green_mask) / (double) bitmap->green_mask : 0;
				clr->blue  = (bitmap->blue_mask) ? (*data & bitmap->blue_mask) / (double) bitmap->blue_mask : 0;
//				printf("0x%08x, %f ", *data, clr->alpha);
				clr++;
				data++;
			}
//			printf("\n");
			data += s;
		}
	} else if (bitmap->bitsperpixel == 32) {
		int w;
		int h;
		int s;
		unsigned int *data = bitmap->bitmap;
		s_render_color_t *clr = rnd->colors;
		s = (bitmap->stride / bitmap->bytesperpixel) - bitmap->width;
//		printf("render\n");
		for (h = 0; h < bitmap->height; h++) {
			for (w = 0; w < bitmap->width; w++) {
				clr->alpha = (bitmap->alpha_mask) ? (*data & bitmap->alpha_mask) / (double) bitmap->alpha_mask : 1.00;
				clr->red   = (bitmap->red_mask) ? (*data & bitmap->red_mask) / (double) bitmap->red_mask : 0;
				clr->green = (bitmap->green_mask) ? (*data & bitmap->green_mask) / (double) bitmap->green_mask : 0;
				clr->blue  = (bitmap->blue_mask) ? (*data & bitmap->blue_mask) / (double) bitmap->blue_mask : 0;
//				printf("0x%08x, %f ", *data, clr->alpha);
				clr++;
				data++;
			}
//			printf("\n");
			data += s;
		}
	} else if (bitmap->bitsperpixel == 1) {
		int w;
		int h;
		unsigned char *data = bitmap->bitmap;
		s_render_color_t *clr = rnd->colors;
//		printf("render\n");
		for (h = 0; h < bitmap->height; h++) {
			for (w = 0; w < bitmap->width; w++) {
				clr->alpha = (data[w >> 3] & (1 << (w & 7))) ? 1.00 : 0.00;
				clr->red   = 0.00;
				clr->green = 0.00;
				clr->blue  = 0.00;
				clr++;
			}
//			printf("\n");
			data += bitmap->stride;
		}
	} else {
		debugf(DFAT, "depth not supported: %d", bitmap->bytesperpixel);
	}
	*render = rnd;
	return 0;
}

int s_bitmap_from_render (s_bitmap_t **bitmap, s_render_t *render)
{
	int w;
	int h;
	s_bitmap_t *bm;
	unsigned char a;
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned int *argb;
	s_render_color_t *clr;
	s_bitmap_init(&bm, 32, render->width, render->height, render->width * 4, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
	clr = render->colors;
	argb = (unsigned int *) bm->bitmap;
	for (h = 0; h < render->height; h++) {
		for (w = 0; w < render->width; w++) {
			a = (unsigned char) xround(255.00 * clr->alpha);
			r = (unsigned char) xround(255.00 * clr->red);
			g = (unsigned char) xround(255.00 * clr->green);
			b = (unsigned char) xround(255.00 * clr->blue);
			*argb = (a << 0x18) | (r << 0x10) | (g << 0x08) | (b << 0x00);
			printf("%08x ", *argb);
			clr++;
			argb++;
		}
		printf("\n");
	}
	*bitmap = bm;
	return 0;
}
