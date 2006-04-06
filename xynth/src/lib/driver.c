/***************************************************************************
    begin                : Wed Jul 30 2003
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
#include "mem.h"

#define OFFSE1	(y * surface->width + x)
#define OFFSE2	(y1 * surface->width + x)
#define OFFSE3	(y * surface->width + x1)
#define OFFSE4	((y + surface->buf.y) * surface->linear_buf_width + x + surface->buf.x)
#define OFFSE5	((y1 + surface->buf.y) * surface->linear_buf_width + x + surface->buf.x)
#define OFFSE6	((y + surface->buf.y) * surface->linear_buf_width + x1 + surface->buf.x)
#define OFFSE4P	((y + surface->buf.y) * surface->linear_buf_pitch + x + surface->buf.x)
#define OFFSE5P	((y1 + surface->buf.y) * surface->linear_buf_pitch + x + surface->buf.x)
#define OFFSE6P	((y + surface->buf.y) * surface->linear_buf_pitch + x1 + surface->buf.x)

#define prepare(offset)\
	unsigned char *m = surface->matrix + offset;\
	int m_;\
	m_ = surface->linear_buf_width;

void bpp_setpixel (s_surface_t *surface, int x, int y, int c)
{
	char *b = surface->vbuf + OFFSE1 * surface->bytesperpixel;
	switch (surface->bytesperpixel) {
		case 1:	*(unsigned char *) b = (unsigned int) c;	break;
		case 2:	*(unsigned short int *) b = (unsigned int) c;	break;
		case 4:	*(unsigned int *) b = (unsigned int) c;		break;
	}
}

void bpp_setpixel_o (s_surface_t *surface, int id, int x, int y, int c)
{
	char *b = surface->linear_buf + OFFSE4P * surface->bytesperpixel;
	prepare(OFFSE4);
	if (*m != id) {
		return;
	}
	switch (surface->bytesperpixel) {
		case 1:	*(unsigned char *) b = (unsigned int) c;	break;
		case 2:	*(unsigned short int *) b = (unsigned int) c;	break;
		case 4:	*(unsigned int *) b = (unsigned int) c;		break;
	}
}

int bpp_getpixel (s_surface_t *surface, int x, int y)
{
	char *b = surface->vbuf + OFFSE1 * surface->bytesperpixel;
	switch (surface->bytesperpixel) {
		case 1:		return *(unsigned char *) b;
		case 2:		return *(unsigned short int *) b;
		case 4:		return *(unsigned int *) b;
		default:	return -1;
	}
}

int bpp_getpixel_o (s_surface_t *surface, int id,  int x, int y)
{
	char *b = surface->linear_buf + OFFSE4P * surface->bytesperpixel;
	prepare(OFFSE4);
	if (*m != id) {
		return -1;
	}
	switch (surface->bytesperpixel) {
		case 1:		return *(unsigned char *) b;
		case 2:		return *(unsigned short int *) b;
		case 4:		return *(unsigned int *) b;
		default:	return -1;
	}
}

void bpp_hline (s_surface_t *surface, int x1, int y, int x2, int c)
{
	char *d = surface->vbuf + OFFSE3 * surface->bytesperpixel;
	switch (surface->bytesperpixel) {
		case 1:	s_memset1(d, c, x2 - x1 + 1);	break;
		case 2:	s_memset2(d, c, x2 - x1 + 1);	break;
		case 4:	s_memset4(d, c, x2 - x1 + 1);	break;
	}
}

void bpp_hline_o (s_surface_t *surface, int id,  int x1, int y, int x2, int c)
{
	char *d = surface->linear_buf + OFFSE6P * surface->bytesperpixel;
	prepare(OFFSE6);
	switch (surface->bytesperpixel) {
		case 1:	s_memset1o(m, id, d, c, x2 - x1 + 1);	break;
		case 2:	s_memset2o(m, id, d, c, x2 - x1 + 1);	break;
		case 4:	s_memset4o(m, id, d, c, x2 - x1 + 1);	break;
	}
}

#define driverloop1(func)\
	while (i--) {\
		func(d, c, 1);\
		d += d_;\
	}
void bpp_vline (s_surface_t *surface, int x, int y1, int y2, int c)
{
	int i = (y2 > y1) ? (y2 - y1 + 1) : (y1 - y2 + 1);
	int d_ = surface->width * surface->bytesperpixel;
	char *d = surface->vbuf + OFFSE2 * surface->bytesperpixel;
	switch (surface->bytesperpixel) {
		case 1:	driverloop1(s_memset1);		break;
		case 2:	driverloop1(s_memset2);		break;
		case 4:	driverloop1(s_memset4);		break;
	}
}

#define driverloop2(func)\
	while (i--) {\
		func(m, id, d, c, 1);\
		d += d_;\
		m += m_;\
	}
void bpp_vline_o (s_surface_t *surface, int id,  int x, int y1, int y2, int c)
{
	int i = (y2 > y1) ? (y2 - y1 + 1) : (y1 - y2 + 1);
	int d_ = surface->linear_buf_pitch * surface->bytesperpixel;
	char *d = surface->linear_buf + OFFSE5P * surface->bytesperpixel;
	prepare(OFFSE5);
	switch (surface->bytesperpixel) {
		case 1:	driverloop2(s_memset1o);	break;
		case 2:	driverloop2(s_memset2o);	break;
		case 4:	driverloop2(s_memset4o);	break;
	}
}

#define driverloop3(func)\
	while (i--) {\
		func(d, c, w);\
		d += d_;\
	}
void bpp_fillbox (s_surface_t *surface, int x, int y, int w, int h, int c)
{
        int i = h;
        int d_ = surface->width * surface->bytesperpixel;
	char *d = surface->vbuf + OFFSE1 * surface->bytesperpixel;
	switch (surface->bytesperpixel) {
		case 1:	driverloop3(s_memset1);		break;
		case 2:	driverloop3(s_memset2);		break;
		case 4:	driverloop3(s_memset4);		break;
	}
}

#define driverloop4(func)\
	while (i--) {\
		func(m, id, d, c, w);\
		d += d_;\
		m += m_;\
	}
void bpp_fillbox_o (s_surface_t *surface, int id,  int x, int y, int w, int h, int c)
{
        int i = h;
        int d_ = surface->linear_buf_pitch * surface->bytesperpixel;
	char *d = surface->linear_buf + OFFSE4P * surface->bytesperpixel;
        prepare(OFFSE4);
	switch (surface->bytesperpixel) {
		case 1:	driverloop4(s_memset1o);	break;
		case 2:	driverloop4(s_memset2o);	break;
		case 4:	driverloop4(s_memset4o);	break;
	}
}

#define driverloop5(func)\
	while (i--) {\
		func(d, s, w);\
		s += s_;\
		d += d_;\
	}
void bpp_putbox (s_surface_t *surface, int x, int y, int w, int h, char *sp, int bw)
{
	int i = h;
	int s_ = bw * surface->bytesperpixel;
	int d_ = surface->width * surface->bytesperpixel;
	char *d = surface->vbuf + OFFSE1 * surface->bytesperpixel;
	char *s = sp;
	switch (surface->bytesperpixel) {
		case 1:	driverloop5(s_memcpy1);		break;
		case 2:	driverloop5(s_memcpy2);		break;
		case 4:	driverloop5(s_memcpy4);		break;
	}
}

#define driverloop5m(func)\
	while (i--) {\
		func(d, s, m, w);\
		s += s_;\
		d += d_;\
		m += m_;\
	}
void bpp_putbox_mask (s_surface_t *surface, int x, int y, int w, int h, char *sp, unsigned char *sm, int bw)
{
	int i = h;
	int m_ = bw;
	int s_ = bw * surface->bytesperpixel;
	int d_ = surface->width * surface->bytesperpixel;
	char *d = surface->vbuf + OFFSE1 * surface->bytesperpixel;
	char *s = sp;
	unsigned char *m = sm;
	switch (surface->bytesperpixel) {
		case 1:	driverloop5m(s_memcpy1m);	break;
		case 2:	driverloop5m(s_memcpy2m);	break;
		case 4:	driverloop5m(s_memcpy4m);	break;
	}
}

void bpp_getbox (s_surface_t *surface, int x, int y, int w, int h, char *dp)
{
	int i = h;
	int s_ = surface->width * surface->bytesperpixel;
	int d_ = w * surface->bytesperpixel;
	char *s = surface->vbuf + OFFSE1 * surface->bytesperpixel;
	char *d = dp;
        switch (surface->bytesperpixel) {
		case 1:	driverloop5(s_memcpy1);		break;
		case 2:	driverloop5(s_memcpy2);		break;
		case 4:	driverloop5(s_memcpy4);		break;
	}

	dp = d;
}

#define driverloop6(func)\
	while (i--) {\
		func(m, id, d, s, w);\
		s += s_;\
		d += d_;\
		m += m_;\
	}
void bpp_putbox_o (s_surface_t *surface, int id,  int x, int y, int w, int h, char *sp, int bw)
{
	int i = h;
	int s_ = bw * surface->bytesperpixel;
	int d_ = surface->linear_buf_pitch * surface->bytesperpixel;
	char *s = sp;
	char *d = surface->linear_buf + OFFSE4P * surface->bytesperpixel;
	prepare(OFFSE4);
	switch (surface->bytesperpixel) {
		case 1:	driverloop6(s_memcpy1o);	break;
		case 2:	driverloop6(s_memcpy2o);	break;
		case 4:	driverloop6(s_memcpy4o);	break;
	}
}

#define driverloop6m(func)\
	while (i--) {\
		func(m, id, d, s, msk, w);\
		s += s_;\
		d += d_;\
		m += m_;\
		msk += msk_;\
	}
void bpp_putbox_mask_o (s_surface_t *surface, int id,  int x, int y, int w, int h, char *sp, unsigned char *sm, int bw)
{
	int i = h;
        int msk_ = bw;
	int s_ = bw * surface->bytesperpixel;
	int d_ = surface->linear_buf_pitch * surface->bytesperpixel;
        unsigned char *msk = sm;
	char *s = sp;
	char *d = surface->linear_buf + OFFSE4P * surface->bytesperpixel;
	prepare(OFFSE4);
	switch (surface->bytesperpixel) {
		case 1:	driverloop6m(s_memcpy1om);	break;
		case 2:	driverloop6m(s_memcpy2om);	break;
		case 4:	driverloop6m(s_memcpy4om);	break;
	}
}

void bpp_getbox_o (s_surface_t *surface, int id,  int x, int y, int w, int h, char *dp)
{
	int i = h;
	int s_ = surface->linear_buf_pitch * surface->bytesperpixel;
	int d_ = w * surface->bytesperpixel;
	char *s = surface->linear_buf + OFFSE4P * surface->bytesperpixel;
	char *d = dp;
	prepare(OFFSE4);
	switch (surface->bytesperpixel) {
		case 1:	driverloop6(s_memcpy1o);	break;
		case 2:	driverloop6(s_memcpy2o);	break;
		case 4:	driverloop6(s_memcpy4o);	break;
	}
	dp = d;
}
