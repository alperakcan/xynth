/***************************************************************************
    begin                : Sun Sep 7 2003
    copyright            : (C) 2003 - 2006 by Alper Akcan
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
	if (surface->mode & SURFACE_VIRTUAL) {
		clipv(x, y, 1, 1);
		bpp_setpixel(surface, coor.x, coor.y, c);
	}
	if (surface->mode & SURFACE_REAL) {
		clipr(x, y, 1, 1);
		gr_sendstream(
			bpp_setpixel_o(surface, *(surface->id), coor.x, coor.y, c);
		);
	}
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
	if (s_rect_intersect(&thix, &clip, &coor)) {
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
	int x1_ = x1;
	int x2_ = x2;

	if (x1_ > x2_) {
		x1 = x2_;
		x2 = x1_;
	}

	if (surface->mode & SURFACE_VIRTUAL) {
		clipv(x1, y, x2 - x1 + 1, 1);
		bpp_hline(surface, coor.x, coor.y, coor.w + coor.x - 1, c);
	}
	if (surface->mode & SURFACE_REAL) {
		clipr(x1, y, x2 - x1 + 1, 1);
		gr_sendstream(
			bpp_hline_o(surface, *(surface->id), coor.x, coor.y, coor.w + coor.x - 1, c);
		);
	}
}

void s_vline (s_surface_t *surface, int x, int y1, int y2, int c)
{
	int y1_ = y1;
	int y2_ = y2;

	if (y1_ > y2_) {
		y1 = y2_;
		y2 = y1_;
	}

	if (surface->mode & SURFACE_VIRTUAL) {
		clipv(x, y1, 1, y2 - y1 + 1);
		bpp_vline(surface, coor.x, coor.y, coor.h + coor.y - 1, c);
	}
	if (surface->mode & SURFACE_REAL) {
		clipr(x, y1, 1, y2 - y1 + 1);
		gr_sendstream(
			bpp_vline_o(surface, *(surface->id), coor.x, coor.y, coor.h + coor.y - 1, c);
		);
	}
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
	if (surface->mode & SURFACE_VIRTUAL) {
		clipv(x, y, w, h);
		bpp_fillbox(surface, coor.x, coor.y, coor.w, coor.h, c);
	}
	if (surface->mode & SURFACE_REAL) {
		clipr(x, y, w, h);
		gr_sendstream(
			bpp_fillbox_o(surface, *(surface->id), coor.x, coor.y, coor.w, coor.h, c);
		);
	}
}

void s_putbox (s_surface_t *surface, int x, int y, int w, int h, char *sp)
{
	if (surface->mode & SURFACE_VIRTUAL) {
		clipv(x, y, w, h);
		bpp_putbox(surface, coor.x, coor.y, coor.w, coor.h, sp + (y0 * w + x0) * surface->bytesperpixel, w);
	}
	if (surface->mode & SURFACE_REAL) {
		clipr(x, y, w, h);
		gr_sendstream(
			bpp_putbox_o(surface, *(surface->id), coor.x, coor.y, coor.w, coor.h, sp + (y0 * w + x0) * surface->bytesperpixel , w);
		);
	}
}

void s_putboxmask (s_surface_t *surface, int x, int y, int w, int h, char *sp, unsigned char *sm)
{
	if (surface->mode & SURFACE_VIRTUAL) {
		clipv(x, y, w, h);
		bpp_putbox_mask(surface, coor.x, coor.y, coor.w, coor.h, sp + (y0 * w + x0) * surface->bytesperpixel, sm + (y0 * w + x0), w);
	}
	if (surface->mode & SURFACE_REAL) {
		clipr(x, y, w, h);
    		gr_sendstream(
			bpp_putbox_mask_o(surface, *(surface->id), coor.x, coor.y, coor.w, coor.h, sp + (y0 * w + x0) * surface->bytesperpixel, sm + (y0 * w + x0), w);
		);
	}
}

void s_putboxalpha (s_surface_t *surface, int x, int y, int w, int h, char *sp, unsigned char *sm)
{
	if (surface->mode & SURFACE_VIRTUAL) {
		clipv(x, y, w, h);
		bpp_putbox_alpha(surface, coor.x, coor.y, coor.w, coor.h, sp + (y0 * w + x0) * surface->bytesperpixel, sm + (y0 * w + x0), w);
	}
	if (surface->mode & SURFACE_REAL) {
		clipr(x, y, w, h);
    		gr_sendstream(
			bpp_putbox_alpha_o(surface, *(surface->id), coor.x, coor.y, coor.w, coor.h, sp + (y0 * w + x0) * surface->bytesperpixel, sm + (y0 * w + x0), w);
		);
	}
}

void s_putboxrgb (s_surface_t *surface, int x, int y, int w, int h, unsigned int *rgba)
{
	s_putboxpartrgb(surface, x, y, w, h, w, h, rgba, 0, 0);
}

void s_putboxrgba (s_surface_t *surface, int x, int y, int w, int h, unsigned int *rgba)
{
	s_putboxpartrgba(surface, x, y, w, h, w, h, rgba, 0, 0);
}

void s_getbox (s_surface_t *surface, int x, int y, int w, int h, char *dp)
{
	clipv(x, y, w, h);
	bpp_getbox(surface, coor.x, coor.y, coor.w, coor.h, dp + (y0 * w + x0) * surface->bytesperpixel);
	return;
}

void s_putboxpart (s_surface_t *surface, int x, int y, int w, int h, int bw, int bh, char *sp, int xo, int yo)
{
	if (surface->mode & SURFACE_VIRTUAL) {
		clipv(x, y, w, h);
		bpp_putbox(surface, coor.x, coor.y, coor.w, coor.h, sp + ((yo + y0) * bw + xo + x0) * surface->bytesperpixel, bw);
	}
	if (surface->mode & SURFACE_REAL) {
		clipr(x, y, w, h);
		gr_sendstream(
			bpp_putbox_o(surface, *(surface->id), coor.x, coor.y, coor.w, coor.h, sp + ((yo + y0) * bw + xo + x0) * surface->bytesperpixel, bw);
		);
	}
}

void s_putboxpartmask (s_surface_t *surface, int x, int y, int w, int h, int bw, int bh, char *sp, unsigned char *sm, int xo, int yo)
{
	if (surface->mode & SURFACE_VIRTUAL) {
		clipv(x, y, w, h);
		bpp_putbox_mask(surface, coor.x, coor.y, coor.w, coor.h, sp + ((yo + y0) * bw + xo + x0) * surface->bytesperpixel, sm + ((yo + y0) * bw + xo + x0), bw);
	}
	if (surface->mode & SURFACE_REAL) {
		clipr(x, y, w, h);
		gr_sendstream(
			bpp_putbox_mask_o(surface, *(surface->id), coor.x, coor.y, coor.w, coor.h, sp + ((yo + y0) * bw + xo + x0) * surface->bytesperpixel, sm + ((yo + y0) * bw + xo + x0), bw);
		);
	}
}

void s_putboxpartalpha (s_surface_t *surface, int x, int y, int w, int h, int bw, int bh, char *sp, unsigned char *sm, int xo, int yo)
{
	if (surface->mode & SURFACE_VIRTUAL) {
		clipv(x, y, w, h);
		bpp_putbox_alpha(surface, coor.x, coor.y, coor.w, coor.h, sp + ((yo + y0) * bw + xo + x0) * surface->bytesperpixel, sm + ((yo + y0) * bw + xo + x0), bw);
	}
	if (surface->mode & SURFACE_REAL) {
		clipr(x, y, w, h);
		gr_sendstream(
			bpp_putbox_alpha_o(surface, *(surface->id), coor.x, coor.y, coor.w, coor.h, sp + ((yo + y0) * bw + xo + x0) * surface->bytesperpixel, sm + ((yo + y0) * bw + xo + x0), bw);
		);
	}
}

void s_putboxpartrgb (s_surface_t *surface, int x, int y, int w, int h, int bw, int bh, unsigned int *rgba, int xo, int yo)
{
	int txd;
	int tyd;
	int txs;
	int tys;
	for (tyd = y, tys = yo; ((tyd - y) < h) && (tys < bh); tyd++, tys++) {
		for (txd = x, txs = xo; ((txd - x) < w) && (txs < bw); txd++, txs++) {
			s_setpixelrgb(surface, txd, tyd, (*(rgba + (tys * bw + txs)) >> 0x18) & 0xFF,
			                                 (*(rgba + (tys * bw + txs)) >> 0x10) & 0xFF,
			                                 (*(rgba + (tys * bw + txs)) >> 0x08) & 0xFF);
		}
	}
}

void s_putboxpartrgba (s_surface_t *surface, int x, int y, int w, int h, int bw, int bh, unsigned int *rgba, int xo, int yo)
{
	int txd;
	int tyd;
	int txs;
	int tys;
	unsigned char alpha;
	for (tyd = y, tys = yo; ((tyd - y) < h) && (tys < bh); tyd++, tys++) {
		for (txd = x, txs = xo; ((txd - x) < w) && (txs < bw); txd++, txs++) {
			alpha = (*(rgba + (tys * bw + txs)) >> 0x00) & 0xFF;
			if (alpha == 0xFF) {
				continue;
			}
			s_setpixelrgba(surface, txd, tyd, (*(rgba + (tys * bw + txs)) >> 0x18) & 0xFF,
			                                  (*(rgba + (tys * bw + txs)) >> 0x10) & 0xFF,
			                                  (*(rgba + (tys * bw + txs)) >> 0x08) & 0xFF,
			                                  alpha);
		}
	}
}

void s_copybox (s_surface_t *surface, int x1, int y1, int w, int h, int x2, int y2)
{
        char *p;
        p = (char *) s_malloc(w * h * surface->bytesperpixel);
	s_getbox(surface, x1, y1, w, h, p);
	s_putbox(surface, x2, y2, w, h, p);
	s_free(p);
}

void s_getsurfacevirtual (s_surface_t *s, int w, int h, int bitspp, char *vbuf)
{
        switch (bitspp) {
		case 8:
                        s->bytesperpixel = 1;
			s->bitsperpixel = 8;
			s->blueoffset = 0;
			s->greenoffset = 3;
			s->redoffset = 6;
#if defined(VIDEO_PSPDEV)
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
#if defined(VIDEO_PSPDEV)
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
#if defined(VIDEO_PSPDEV)
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
#if defined(VIDEO_PSPDEV)
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
			s->bluelength = 8;
#if defined(VIDEO_PSPDEV)
			s->blueoffset = 16;
			s->greenoffset = 8;
			s->redoffset = 0;
#endif
			s->greenlength = 8;
			s->redlength = 8;
			break;
	}

	s->mode = SURFACE_VIRTUAL;
	s->width = w;
	s->height = h;
	s->vbuf = vbuf;
}

int s_copybuffer (char *sb, int sbitspp, char **db, int dbitspp, int w, int h)
{
        int x;
        int y;
        int r;
        int g;
        int b;
        s_surface_t ss;
        s_surface_t ds;
        
	*db = (char *) s_malloc(w * h * ((dbitspp + 1) / 8));
	s_getsurfacevirtual(&ss, w, h, sbitspp, sb);
	s_getsurfacevirtual(&ds, w, h, dbitspp, *db);

	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			s_getpixelrgb(&ss, x, y, &r, &g, &b);
			s_setpixelrgb(&ds, x, y, r, g, b);
		}
	}
	
	return 0;
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

#include "mem.h"
int s_putmaskpart (char *dp, int dw, int dh, int x, int y, int w, int h, int bw, int bh, char *sp, int xo, int yo)
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

	if (s_rect_intersect(&thip, &clip, &coor)) {
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
	
	if (s_rect_intersect(&thip, &clip, &coor)) {
		return -1;
	}
	
	x0 = coor.x - x;
	y0 = coor.y - y;

	{
		int i = coor.h;
		int s_ = bw;
		int d_ = dw;
		char *d = dp + (coor.y * dw + coor.x);
		char *s = sp + (yo + y0) * bw + xo + x0;
		while (i--) {
			s_memcpy1(d, s, coor.w);
			s += s_;
			d += d_;
		}
	}

	return 0;
}
