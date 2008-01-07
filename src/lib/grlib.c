/***************************************************************************
    begin                : Sun Sep 7 2003
    copyright            : (C) 2003 - 2007 by Alper Akcan
    email                : distchx@yahoo.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#include "xynth_.h"
#include "mem.h"

#define clipc(a, b, c, d, fonk)\
	int nrects;\
	s_rect_t crect;\
	s_rect_t wrect;\
	s_rect_t *crects;\
	if (surface->parent != NULL) {\
		wrect.x = a;\
		wrect.y = b;\
		wrect.w = c;\
		wrect.h = d;\
		if (s_region_rect_intersect(&wrect, surface->win, &crect) != 0) {\
			return;\
		}\
	} else {\
		crect.x = a;\
		crect.y = b;\
		crect.w = c;\
		crect.h = d;\
	}\
	if (surface->clip == NULL) {\
		fonk;\
	} else {\
		s_rect_t cintr;\
		cintr.x = crect.x;\
		cintr.y = crect.y;\
		cintr.w = crect.w;\
		cintr.h = crect.h;\
		nrects = s_region_num_rectangles(surface->clip);\
		crects = s_region_rectangles(surface->clip);\
		while (nrects--) {\
			if (s_region_rect_intersect(&crects[nrects], &cintr, &crect) == 0) {\
				fonk;\
			}\
		}\
	}

#define clipr(a, b, c, d)\
	int x0;\
	int y0;\
	s_rect_t coor;\
	if (s_rect_clip_real(surface, a, b, c, d, &coor)) {\
		return;\
	}\
	x0 = coor.x - a;\
	y0 = coor.y - b;

#define clipv(a, b, c, d)\
	int x0;\
	int y0;\
	s_rect_t coor;\
	if (s_rect_clip_virtual(surface, a, b, c, d, &coor)) {\
		return;\
	}\
	x0 = coor.x - a;\
	y0 = coor.y - b;

#define clipb()\
	s_rect_t thip;\
	s_rect_t clip;\
	s_rect_t intr;\
	thip.x = 0; thip.y = 0; thip.w = bw; thip.h = bh;\
	clip.x = xo; clip.y = yo; clip.w = w; clip.h = h;\
	if (s_region_rect_intersect(&thip, &clip, &intr)) {\
		return;\
	}\
	xo = intr.x; yo = intr.y; w = intr.w; h = intr.h;

#define gr_sendstream(fonk)\
	if (surface->need_expose & SURFACE_NEEDSTREAM) {\
		s_socket_request(surface->window, SOC_DATA_EXPOSE, &coor);\
	} else {\
		fonk;\
		if (surface->need_expose & SURFACE_NEEDEXPOSE) {\
			s_socket_request(surface->window, SOC_DATA_EXPOSE, &coor);\
		}\
	}

int s_rgbcolor (s_surface_t *surface, int r, int g, int b)
{
	return (((((unsigned char) r) >> (8 - surface->redlength)) << surface->redoffset) |
	        ((((unsigned char) g) >> (8 - surface->greenlength)) << surface->greenoffset) |
	        ((((unsigned char) b) >> (8 - surface->bluelength)) << surface->blueoffset));
}

void s_colorrgb (s_surface_t *surface, int c, int *r, int *g, int *b)
{
        *r = (unsigned char) ((((unsigned int) c) >> surface->redoffset) << (8 - surface->redlength));
        *g = (unsigned char) ((((unsigned int) c) >> surface->greenoffset) << (8 - surface->greenlength));
        *b = (unsigned char) ((((unsigned int) c) >> surface->blueoffset) << (8 - surface->bluelength));
}

void s_setpixel (s_surface_t *surface, int x, int y, int c)
{
	clipc(x, y, 1, 1, 
	if (surface->mode & SURFACE_VIRTUAL) {
		clipv(crect.x, crect.y, crect.w, crect.h);
		bpp_setpixel(surface, coor.x, coor.y, c);
	}
	if (surface->mode & SURFACE_REAL) {
		clipr(crect.x, crect.y, crect.w, crect.h);
		gr_sendstream(
			bpp_setpixel_o(surface, surface->window->id, coor.x, coor.y, c);
		);
	}
	);
}

void s_setpixelrgb (s_surface_t *surface, int x, int y, int r, int g, int b)
{
	s_setpixel(surface, x, y, s_rgbcolor(surface, r, g, b));
}

void s_setpixelrgba (s_surface_t *surface, int x, int y, int r, int g, int b, int a)
{
	int br;
	int bg;
	int bb;
	int ar;
	int ag;
	int ab;
	unsigned char a_ = (unsigned char) (a & 0xFF);
	if (a_ == 0xff) {
		return;
	}
	s_getpixelrgb(surface, x, y, &br, &bg, &bb);
	ar  = (a_ * (br - r) >> 8) + r;
	ag  = (a_ * (bg - g) >> 8) + g;
	ab  = (a_ * (bb - b) >> 8) + b;
	s_setpixelrgb(surface, x, y, ar, ag, ab);
}

int s_getpixel (s_surface_t *surface, int x, int y)
{
	s_rect_t coor;
	s_rect_t thix;
	s_rect_t clip;
	thix.x = x;
	thix.y = y;
	thix.w = 1;
	thix.h = 1;
	clip.x = 0;
	clip.y = 0;
	clip.w = surface->width;
	clip.h = surface->height;
	if (s_region_rect_intersect(&thix, &clip, &coor)) {
		return 0;
	}
	return (bpp_getpixel(surface, coor.x, coor.y));
}

void s_getpixelrgb (s_surface_t *surface, int x, int y, int *r, int *g, int *b)
{
	s_colorrgb(surface, s_getpixel(surface, x, y), r, g, b);
}

void s_hline (s_surface_t *surface, int x1, int y, int x2, int c)
{
	int x1_ = MIN(x1, x2);
	int x2_ = MAX(x1, x2);
	s_fillbox(surface, x1_, y, x2_ - x1_ + 1, 1, c);
}

void s_vline (s_surface_t *surface, int x, int y1, int y2, int c)
{
	int y1_ = MIN(y1, y2);
	int y2_ = MAX(y1, y2);
	s_fillbox(surface, x, y1_, 1, y2_ - y1_ + 1, c);
}

void s_line (s_surface_t *surface, int x1, int y1, int x2, int y2, int c)
{
	int x;
	int y;
	int num;
	int den;
	int xinc1;
	int xinc2;
	int yinc1;
	int yinc2;
	int numadd;
	int deltax;
	int deltay;
	int curpixel;
	int numpixels;
	if (x1 == x2) {
		s_vline(surface, x1, y1, y2, c);
		return;
	} else if (y1 == y2) {
		s_hline(surface, x1, y1, x2, c);
		return;
	}
	x = x1;
	y = y1;
	deltax = abs(x2 - x1);
	deltay = abs(y2 - y1);
	if (x2 >= x1) {
		xinc1 = 1;
		xinc2 = 1;
	} else {
		xinc1 = -1;
		xinc2 = -1;
	}
	if (y2 >= y1) {
		yinc1 = 1;
		yinc2 = 1;
	} else {
		yinc1 = -1;
		yinc2 = -1;
	}
	if (deltax >= deltay) {
		xinc1 = 0;
		yinc2 = 0;
		den = deltax;
		num = deltax / 2;
		numadd = deltay;
		numpixels = deltax;
	} else {
		xinc2 = 0;
		yinc1 = 0;
		den = deltay;
		num = deltay / 2;
		numadd = deltax;
		numpixels = deltay;
	}
	for (curpixel = 0; curpixel <= numpixels; curpixel++) {
		s_setpixel(surface, x, y, c);
		num += numadd;
		if (num >= den) {
			num -= den;
			x += xinc1;
			y += yinc1;
		}
		x += xinc2;
		y += yinc2;
	}
}

void s_fillbox (s_surface_t *surface, int x, int y, int w, int h, int c)
{
	clipc(x, y, w, h,
	if (surface->mode & SURFACE_VIRTUAL) {
		clipv(crect.x, crect.y, crect.w, crect.h);
		bpp_fillbox(surface, coor.x, coor.y, coor.w, coor.h, c);
	}
	if (surface->mode & SURFACE_REAL) {
		clipr(crect.x, crect.y, crect.w, crect.h);
		gr_sendstream(
			bpp_fillbox_o(surface, surface->window->id, coor.x, coor.y, coor.w, coor.h, c);
		);
	}
	);
}

void s_putbox (s_surface_t *surface, int x, int y, int w, int h, void *sp)
{
	s_putboxpart(surface, x, y, w, h, w, h, sp, 0, 0);
}

void s_putboxmask (s_surface_t *surface, int x, int y, int w, int h, char *sp, unsigned char *sm)
{
	s_putboxpartmask(surface, x, y, w, h, w, h, sp, sm, 0, 0);
}

void s_putboxalpha (s_surface_t *surface, int x, int y, int w, int h, char *sp, unsigned char *sm)
{
	s_putboxpartalpha(surface, x, y, w, h, w, h, sp, sm, 0, 0);
}

void s_putboxrgb (s_surface_t *surface, int x, int y, int w, int h, unsigned int *rgba)
{
	s_putboxpartrgb(surface, x, y, w, h, w, h, rgba, 0, 0);
}

void s_putboxrgba (s_surface_t *surface, int x, int y, int w, int h, unsigned int *rgba)
{
	s_putboxpartrgba(surface, x, y, w, h, w, h, rgba, 0, 0);
}

void s_getbox (s_surface_t *surface, int x, int y, int w, int h, void *dp)
{
	clipc(x, y, w, h,
	clipv(crect.x, crect.y, crect.w, crect.h);
	bpp_getbox(surface, coor.x, coor.y, coor.w, coor.h, dp + (y0 * w + x0) * surface->bytesperpixel);
	);
}

void s_putboxpart (s_surface_t *surface, int x, int y, int w, int h, int bw, int bh, void *sp, int xo, int yo)
{
	clipb();
	clipc(x, y, w, h,
	if (surface->mode & SURFACE_VIRTUAL) {
		clipv(crect.x, crect.y, crect.w, crect.h);
		bpp_putbox(surface, coor.x, coor.y, coor.w, coor.h, sp + ((yo + y0) * bw + xo + x0) * surface->bytesperpixel, bw);
	}
	if (surface->mode & SURFACE_REAL) {
		clipr(crect.x, crect.y, crect.w, crect.h);
		gr_sendstream(
			bpp_putbox_o(surface, surface->window->id, coor.x, coor.y, coor.w, coor.h, sp + ((yo + y0) * bw + xo + x0) * surface->bytesperpixel, bw);
		);
	}
	);
}

void s_putboxpartmask (s_surface_t *surface, int x, int y, int w, int h, int bw, int bh, char *sp, unsigned char *sm, int xo, int yo)
{
	clipb();
	clipc(x, y, w, h,
	if (surface->mode & SURFACE_VIRTUAL) {
		clipv(crect.x, crect.y, crect.w, crect.h);
		bpp_putbox_mask(surface, coor.x, coor.y, coor.w, coor.h, sp + ((yo + y0) * bw + xo + x0) * surface->bytesperpixel, sm + ((yo + y0) * bw + xo + x0), bw);
	}
	if (surface->mode & SURFACE_REAL) {
		clipr(crect.x, crect.y, crect.w, crect.h);
		gr_sendstream(
			bpp_putbox_mask_o(surface, surface->window->id, coor.x, coor.y, coor.w, coor.h, sp + ((yo + y0) * bw + xo + x0) * surface->bytesperpixel, sm + ((yo + y0) * bw + xo + x0), bw);
		);
	}
	);
}

void s_putboxpartalpha (s_surface_t *surface, int x, int y, int w, int h, int bw, int bh, void *sp, unsigned char *sm, int xo, int yo)
{
	clipb();
	clipc(x, y, w, h,
	if (surface->mode & SURFACE_VIRTUAL) {
		clipv(crect.x, crect.y, crect.w, crect.h);
		bpp_putbox_alpha(surface, coor.x, coor.y, coor.w, coor.h, sp + ((yo + y0) * bw + xo + x0) * surface->bytesperpixel, sm + ((yo + y0) * bw + xo + x0), bw);
	}
	if (surface->mode & SURFACE_REAL) {
		clipr(crect.x, crect.y, crect.w, crect.h);
		gr_sendstream(
			bpp_putbox_alpha_o(surface, surface->window->id, coor.x, coor.y, coor.w, coor.h, sp + ((yo + y0) * bw + xo + x0) * surface->bytesperpixel, sm + ((yo + y0) * bw + xo + x0), bw);
		);
	}
	);
}

void s_putboxpartrgb (s_surface_t *surface, int x, int y, int w, int h, int bw, int bh, unsigned int *rgba, int xo, int yo)
{
	clipb();
	clipc(x, y, w, h,
	if (surface->mode & SURFACE_VIRTUAL) {
		clipv(crect.x, crect.y, crect.w, crect.h);
		bpp_putbox_rgb(surface, coor.x, coor.y, coor.w, coor.h, rgba + ((yo + y0) * bw + xo + x0), bw);
	}
	if (surface->mode & SURFACE_REAL) {
		clipr(crect.x, crect.y, crect.w, crect.h);
		gr_sendstream(
			bpp_putbox_rgb_o(surface, surface->window->id, coor.x, coor.y, coor.w, coor.h, rgba + ((yo + y0) * bw + xo + x0), bw);
		);
	}
	);
}

void s_putboxpartrgba (s_surface_t *surface, int x, int y, int w, int h, int bw, int bh, unsigned int *rgba, int xo, int yo)
{
	clipb();
	clipc(x, y, w, h,
	if (surface->mode & SURFACE_VIRTUAL) {
		clipv(crect.x, crect.y, crect.w, crect.h);
		bpp_putbox_rgba(surface, coor.x, coor.y, coor.w, coor.h, rgba + ((yo + y0) * bw + xo + x0), bw);
	}
	if (surface->mode & SURFACE_REAL) {
		clipr(crect.x, crect.y, crect.w, crect.h);
		gr_sendstream(
			bpp_putbox_rgba_o(surface, surface->window->id, coor.x, coor.y, coor.w, coor.h, rgba + ((yo + y0) * bw + xo + x0), bw);
		);
	}
	);
}

void s_copybox (s_surface_t *surface, int x1, int y1, int w, int h, int x2, int y2)
{
        char *p;
        p = (char *) s_malloc(w * h * surface->bytesperpixel);
	s_getbox(surface, x1, y1, w, h, p);
	s_putbox(surface, x2, y2, w, h, p);
	s_free(p);
}

int s_copybuffer (char *sb, int sbitspp, char **db, int dbitspp, int w, int h)
{
        int x;
        int y;
        int r;
        int g;
        int b;
        s_surface_t *ss;
        s_surface_t *ds;
        
	if (s_surface_create_from_data(&ss, w, h, sbitspp, sb)) {
		goto err0;
	}
	*db = (char *) s_malloc(w * h * ((dbitspp + 1) / 8));
	if (s_surface_create_from_data(&ds, w, h, dbitspp, *db)) {
		goto err1;
	}

	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			s_getpixelrgb(ss, x, y, &r, &g, &b);
			s_setpixelrgb(ds, x, y, r, g, b);
		}
	}
	
	s_surface_destroy(ds);
	s_surface_destroy(ss);
	return 0;
err1:	s_surface_destroy(ss);
err0:	return -1;
}

#define s_rotatebox_macro(type, angle) {\
	int x;\
	int y;\
	unsigned type *stmp = (unsigned type *) sbuf;\
	unsigned type *dtmp = (unsigned type *) dbuf;\
	switch (angle) {\
		case 90:\
		case -270:\
			drect->x = surface->width - srect->y - srect->h;\
			drect->y = srect->x;\
			drect->w = srect->h;\
			drect->h = srect->w;\
			for (y = 0; y < srect->h; y++) {\
				for (x = 0; x < srect->w; x++) {\
					dtmp[(x * srect->h) + srect->h - 1 - y] = stmp[(y * srect->w) + x];\
				}\
			}\
			break;\
		case 180:\
		case -180:\
			drect->x = surface->width - srect->x - srect->w;\
			drect->y = surface->height - srect->y - srect->h;\
			drect->w = srect->w;\
			drect->h = srect->h;\
			for (y = 0; y < srect->h; y++) {\
				for (x = 0; x < srect->w; x++) {\
					dtmp[((srect->h - 1) - y) * srect->w + ((srect->w - 1) - x)] = stmp[(y * srect->w) + x];\
				}\
			}\
			break;\
		case 270:\
		case -90:\
			drect->x = srect->y;\
			drect->y = surface->height - srect->x - srect->w;\
			drect->w = srect->h;\
			drect->h = srect->w;\
			for (y = 0; y < srect->h; y++) {\
				for (x = 0; x < srect->w; x++) {\
					dtmp[((srect->w - 1) - x) * srect->h + y] = stmp[(y * srect->w) + x];\
				}\
			}\
			break;\
	}\
}

void s_rotatebox (s_surface_t *surface, s_rect_t *srect, void *sbuf, s_rect_t *drect, void *dbuf, int rotate)
{
	int atmp = rotate % 360;
	switch (surface->bytesperpixel) {
		case 1: s_rotatebox_macro(char, atmp); break;
		case 2: s_rotatebox_macro(short, atmp); break;
		case 4: s_rotatebox_macro(int, atmp); break;
	}
}

void s_scalebox (s_surface_t *surface, int w1, int h1, void *_dp1, int w2, int h2, void *_dp2)
{
	unsigned char *dp1 = _dp1;
	unsigned char *dp2 = _dp2;
	int xfactor;
	int yfactor;

	if ((w2 == 0) || (h2 == 0)) {
		return;
	}

	xfactor = w1 * 65536 / w2;
	yfactor = h1 * 65536 / h2;

	switch (surface->bytesperpixel) {
		case 1:
			{
				int y;
				int sy = 0;
                                for (y = 0; y < h2; ) {
					int x = 0;
					int sx = 0;
					unsigned char *dp2old = dp2;
                                        while (x < w2 - 8) {
						*(dp2 + x) = *(dp1 + (sx >> 16));
						sx += xfactor;
						*(dp2 + x + 1) = *(dp1 + (sx >> 16));
						sx += xfactor;
						*(dp2 + x + 2) = *(dp1 + (sx >> 16));
						sx += xfactor;
						*(dp2 + x + 3) = *(dp1 + (sx >> 16));
						sx += xfactor;
						*(dp2 + x + 4) = *(dp1 + (sx >> 16));
						sx += xfactor;
						*(dp2 + x + 5) = *(dp1 + (sx >> 16));
						sx += xfactor;
						*(dp2 + x + 6) = *(dp1 + (sx >> 16));
						sx += xfactor;
						*(dp2 + x + 7) = *(dp1 + (sx >> 16));
						sx += xfactor;
						x += 8;
					}
					while (x < w2) {
						*(dp2 + x) = *(dp1 + (sx >> 16));
						sx += xfactor;
						x++;
					}
					dp2 += w2;
					y++;
					while (y < h2) {
						int l;
						int syint = sy >> 16;
						sy += yfactor;
						if ((sy >> 16) != syint) {
							break;
						}
						l = dp2 - dp2old;
						memcpy(dp2, dp2old, l);
						dp2old = dp2;
						dp2 += l;
						y++;
					}
					dp1 = ((unsigned char *) _dp1) + (sy >> 16) * w1;
				}
			}
			break;
		case 2:
			{
				int y;
				int sy = 0;
				for (y = 0; y < h2; ) {
					int x = 0;
					int sx = 0;
					unsigned char *dp2old = dp2;
					while (x < w2) {
						*(unsigned short *) (dp2 + x * 2) = *(unsigned short *) (dp1 + (sx >> 16) * 2);
						sx += xfactor;
						x++;
					}
					dp2 += w2 * 2;
					y++;
					while (y < h2) {
						int l;
						int syint = sy >> 16;
						sy += yfactor;
						if ((sy >> 16) != syint) {
							break;
						}
						l = dp2 - dp2old;
						memcpy(dp2, dp2old, l);
						dp2old = dp2;
						dp2 += l;
						y++;
					}
					dp1 = ((unsigned char *) _dp1) + (sy >> 16) * w1 * 2;
				}
			}
			break;
		case 4:
			{
				int y;
				int sy = 0;
				for (y = 0; y < h2; ) {
					int x = 0;
					int sx = 0;
					unsigned char *dp2old = dp2;
					while (x < w2) {
						*(unsigned *) (dp2 + x * 4) = *(unsigned *) (dp1 + (sx >> 16) * 4);
						sx += xfactor;
						x++;
					}
					dp2 += w2 * 4;
					y++;
					while (y < h2) {
						int l;
						int syint = sy >> 16;
						sy += yfactor;
						if ((sy >> 16) != syint) {
							break;
						}
						l = dp2 - dp2old;
						memcpy(dp2, dp2old, l);
						dp2old = dp2;
						dp2 += l;
						y++;
					}
					dp1 = ((unsigned char *) _dp1) + (sy >> 16) * w1 * 4;
				}
			}
			break;
	}
}

int s_putmaskpart (unsigned char *dp, int dw, int dh, int x, int y, int w, int h, int bw, int bh, unsigned char *sp, int xo, int yo)
{
	int x0;
	int y0;
	s_rect_t coor;
	s_rect_t clip;
	s_rect_t thip;
	
	thip.x = xo;
	thip.y = yo;
	thip.w = w;
	thip.h = h;
	
	clip.x = 0;
	clip.y = 0;
	clip.w = bw;
	clip.h = bh;

	if (s_region_rect_intersect(&thip, &clip, &coor)) {
		return -1;
	}
	
	thip.x = x;
	thip.y = y;
	thip.w = coor.w;
	thip.h = coor.h;
	
	clip.x = 0;
	clip.y = 0;
	clip.w = dw;
	clip.h = dh;
	
	if (s_region_rect_intersect(&thip, &clip, &coor)) {
		return -1;
	}
	
	x0 = coor.x - x;
	y0 = coor.y - y;

	{
		int i = coor.h;
		int s_ = bw;
		int d_ = dw;
		unsigned char *d = dp + (coor.y * dw + coor.x);
		unsigned char *s = sp + (yo + y0) * bw + xo + x0;
		while (i--) {
			s_memcpy1(d, s, coor.w);
			s += s_;
			d += d_;
		}
	}

	return 0;
}

int s_surface_destroy (s_surface_t *surface)
{
	s_surface_t *c;
	if (surface) {
		s_thread_mutex_lock(surface->subm);
		if (surface->parent) {
			s_thread_mutex_lock(surface->parent->subm);
			s_list_remove(surface->parent->subs, s_list_get_pos(surface->parent->subs, surface));
		}
		if (surface->evbuf == 0 && surface->vbuf) {
			free(surface->vbuf);
		}
		if (surface->clip != NULL) {
			s_region_destroy(surface->clip);
		}
		while (!s_list_eol(surface->subs, 0)) {
			c = s_list_get(surface->subs, 0);
			s_list_remove(surface->subs, 0);
			c->parent = NULL;
			s_surface_destroy(c);
		}
		if (surface->parent) {
			s_thread_mutex_unlock(surface->parent->subm);
		}
		s_thread_mutex_unlock(surface->subm);
		s_list_uninit(surface->subs);
		s_thread_mutex_destroy(surface->subm);
		free(surface->buf);
		free(surface->win);
		free(surface);
	}
	return 0;
}

int s_surface_create_from_data (s_surface_t **surface, int width, int height, int bitsperpixel, void *vbuf)
{
	s_surface_t *s;

	s = (s_surface_t *) s_malloc(sizeof(s_surface_t));
	memset(s, 0, sizeof(s_surface_t));
	if (s_thread_mutex_init(&s->subm)) {
		goto err0;
	}
	if (s_list_init(&s->subs)) {
		goto err1;
	}
	s->buf = (s_rect_t *) s_malloc(sizeof(s_rect_t));
	memset(s->buf, 0, sizeof(s_rect_t));
	s->win = (s_rect_t *) s_malloc(sizeof(s_rect_t));
	memset(s->win, 0, sizeof(s_rect_t));

        switch (bitsperpixel) {
		case 8:
                        s->bytesperpixel = 1;
			s->bitsperpixel = 8;
			s->blueoffset = 0;
			s->greenoffset = 3;
			s->redoffset = 6;
#if defined(CONFIG_VIDEO_PSPDEV)
			s->blueoffset = 6;
			s->greenoffset = 3;
			s->redoffset = 0;
#endif
			s->bluelength = 3;
			s->greenlength = 3;
			s->redlength = 2;
			break;
		case 15:
                        s->bytesperpixel = 2;
			s->bitsperpixel = 15;
			s->blueoffset = 0;
			s->greenoffset = 5;
			s->redoffset = 10;
#if defined(CONFIG_VIDEO_PSPDEV)
			s->blueoffset = 10;
			s->greenoffset = 5;
			s->redoffset = 0;
#endif
			s->bluelength = 5;
			s->greenlength = 5;
			s->redlength = 5;
			break;
		case 16:
                        s->bytesperpixel = 2;
			s->bitsperpixel = 16;
			s->blueoffset = 0;
			s->greenoffset = 5;
			s->redoffset = 11;
#if defined(CONFIG_VIDEO_PSPDEV)
			s->blueoffset = 11;
			s->greenoffset = 5;
			s->redoffset = 0;
#endif
			s->bluelength = 5;
			s->greenlength = 6;
			s->redlength = 5;
			break;
                case 18:
			s->bytesperpixel = 4;
                        s->bitsperpixel = 18;
                        s->blueoffset = 0;
                        s->greenoffset = 6;
                        s->redoffset = 12;
#if defined(CONFIG_VIDEO_PSPDEV)
			s->blueoffset = 12;
			s->greenoffset = 6;
			s->redoffset = 0;
#endif
                        s->bluelength = 6;
                        s->greenlength = 6;
                        s->redlength = 6;
                        break;
		case 24:
		case 32:
                        s->bytesperpixel = 4;
			s->bitsperpixel = 32;
			s->blueoffset = 0;
			s->greenoffset = 8;
			s->redoffset = 16;
#if defined(CONFIG_VIDEO_PSPDEV)
			s->blueoffset = 16;
			s->greenoffset = 8;
			s->redoffset = 0;
#endif
			s->bluelength = 8;
			s->greenlength = 8;
			s->redlength = 8;
			break;
		default:
			goto err2;
	}
        
        s->bluemask = ((1 << s->bluelength) - 1) << s->blueoffset;
        s->greenmask = ((1 << s->greenlength) - 1) << s->greenoffset;
        s->redmask = ((1 << s->redlength) - 1) << s->redoffset;

	s->mode = SURFACE_VIRTUAL;
	
	s->width = width;
	s->height = height;

	s->buf->x = 0;
	s->buf->y = 0;
	s->buf->w = width;
	s->buf->h = height;

	s->win->x = 0;
	s->win->y = 0;
	s->win->w = width;
	s->win->h = height;

	s->evbuf = 1;
	s->vbuf = vbuf;

	*surface = s;
	return 0;
err2:	s_list_uninit(s->subs);
err1:	s_thread_mutex_destroy(s->subm);
err0:	s_free(s);
	*surface = NULL;
	return -1;
}

int s_surface_create (s_surface_t **surface, int width, int height, int bitsperpixel)
{
	s_surface_t *s;
	if (s_surface_create_from_data(&s, width, height, bitsperpixel, NULL)) {
		goto err0;
	}
	s->evbuf = 0;
	s->vbuf = (unsigned char *) s_malloc(s->width * s->height * s->bytesperpixel);
	*surface = s;
	return 0;
err0:	*surface = NULL;
	return -1;
}

int s_surface_create_sub (s_surface_t **surface, int x, int y, int width, int height, s_surface_t *parent)
{
	s_rect_t p;
	s_rect_t r;
	s_surface_t *s;
	if (parent == NULL) {
		goto err0;
	}
	s_thread_mutex_lock(parent->subm);
	if (s_surface_create_from_data(&s, parent->width, parent->height, parent->bitsperpixel, parent->vbuf)) {
		goto err1;
	}
	s_list_add(parent->subs, s, -1);
	s->parent = parent;
	p.x = 0;
	p.y = 0;
	p.w = parent->buf->w;
	p.h = parent->buf->h;
	r.x = x;
	r.y = y;
	r.w = width;
	r.h = height;
	if (s_region_rect_intersect(&p, &r, s->buf)) {
		goto err2;
	}
	r.x = s->buf->x + parent->win->x;
	r.y = s->buf->y + parent->win->y;
	r.w = s->buf->w;
	r.h = s->buf->h;
	if (s_region_rect_intersect(&p, &r, s->win)) {
		goto err3;
	}
	s_thread_mutex_unlock(parent->subm);
	*surface = s;
	return 0;
err3:
err2:
err1:	s_thread_mutex_unlock(parent->subm);
err0:	*surface = NULL;
	return -1;
}

int s_surface_set_clip_region (s_surface_t *surface, s_region_t *region)
{
	int nrect;
	s_rect_t *rects;
	if (surface->clip != NULL) {
		s_region_destroy(surface->clip);
		surface->clip = NULL;
	}
	if (region == NULL) {
		return 0;
	}
	if (s_region_create(&surface->clip)) {
		return -1;
	}
	nrect = s_region_num_rectangles(region);
	rects = s_region_rectangles(region);
	while (nrect--) {
		if (s_region_addrect(surface->clip, &rects[nrect])) {
			s_region_destroy(surface->clip);
			return -1;
		}
	}
	return 0;
}
