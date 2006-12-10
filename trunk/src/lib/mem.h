/***************************************************************************
    begin                : Thu Jul 31 2003
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

#ifdef __cplusplus
extern "C" {
#endif

#define memcpyloop1()\
	while (n--) {\
		if (*m == id) {\
			*d = *s;\
		}\
		d++;\
		s++;\
		m++;\
	}

#define memcpyloop2()\
	while (n--) {\
		if ((*m == id) && (*m_)) {\
			*d = *s;\
		}\
		d++;\
		s++;\
		m++;\
		m_++;\
	}


#define memcpyloop4()\
	while (n--) {\
		if (*m) {\
			*d = *s;\
		}\
		d++;\
		s++;\
		m++;\
	}		

#define memcpyloop5()\
	while (n--) {\
		a = ~(*m & 0xff);\
		if (a != 0xff) {\
			if (a == 0x00) {\
				*d = *s;\
			} else {\
				dr = (*d >> ro) << rl;\
				dg = (*d >> go) << gl;\
				db = (*d >> bo) << bl;\
				sr = (*s >> ro) << rl;\
				sg = (*s >> go) << gl;\
				sb = (*s >> bo) << bl;\
				dr  = (a * (dr - sr) >> 8) + sr;\
				dg  = (a * (dg - sg) >> 8) + sg;\
				db  = (a * (db - sb) >> 8) + sb;\
				*d = ((dr >> rl) << ro) | ((dg >> gl) << go) | ((db >> bl) << bo);\
			}\
		}\
		d++;\
		s++;\
		m++;\
	}		

#define memcpyloop6()\
	while (n--) {\
		a = ~(*m_ & 0xff);\
		if ((*m == id) && (a != 0xff)) {\
			if (a == 0x00) {\
				*d = *s;\
			} else {\
				dr = (*d >> ro) << rl;\
				dg = (*d >> go) << gl;\
				db = (*d >> bo) << bl;\
				sr = (*s >> ro) << rl;\
				sg = (*s >> go) << gl;\
				sb = (*s >> bo) << bl;\
				dr  = (a * (dr - sr) >> 8) + sr;\
				dg  = (a * (dg - sg) >> 8) + sg;\
				db  = (a * (db - sb) >> 8) + sb;\
				*d = ((dr >> rl) << ro) | ((dg >> gl) << go) | ((db >> bl) << bo);\
			}\
		}\
		d++;\
		s++;\
		m++;\
		m_++;\
	}

#define memcpyloop7()\
	while (n--) {\
		dr = (*s >> 0x18) & 0xff;\
		dg = (*s >> 0x10) & 0xff;\
		db = (*s >> 0x08) & 0xff;\
		*d = ((dr >> rl) << ro) | ((dg >> gl) << go) | ((db >> bl) << bo);\
		d++;\
		s++;\
	}

#define memcpyloop8()\
	while (n--) {\
		if (*m == id) {\
			dr = (*s >> 0x18) & 0xff;\
			dg = (*s >> 0x10) & 0xff;\
			db = (*s >> 0x08) & 0xff;\
			*d = ((dr >> rl) << ro) | ((dg >> gl) << go) | ((db >> bl) << bo);\
		}\
		d++;\
		s++;\
		m++;\
	}

#define memcpyloop9()\
	while (n--) {\
		a = (*s & 0xff);\
		if (a != 0xff) {\
			sr = (*s >> 0x18) & 0xff;\
			sg = (*s >> 0x10) & 0xff;\
			sb = (*s >> 0x08) & 0xff;\
			if (a == 0x00) {\
				dr = sr;\
				dg = sg;\
				db = sb;\
			} else {\
				dr = (*d >> ro) << rl;\
				dg = (*d >> go) << gl;\
				db = (*d >> bo) << bl;\
				dr  = (a * (dr - sr) >> 8) + sr;\
				dg  = (a * (dg - sg) >> 8) + sg;\
				db  = (a * (db - sb) >> 8) + sb;\
			}\
			*d = ((dr >> rl) << ro) | ((dg >> gl) << go) | ((db >> bl) << bo);\
		}\
		d++;\
		s++;\
	}

#define memcpyloop10()\
	while (n--) {\
		a = (*s & 0xff);\
		if ((*m == id) && (a != 0xff)) {\
			sr = (*s >> 0x18) & 0xff;\
			sg = (*s >> 0x10) & 0xff;\
			sb = (*s >> 0x08) & 0xff;\
			if (a == 0x00) {\
				dr = sr;\
				dg = sg;\
				db = sb;\
			} else {\
				dr = (*d >> ro) << rl;\
				dg = (*d >> go) << gl;\
				db = (*d >> bo) << bl;\
				dr  = (a * (dr - sr) >> 8) + sr;\
				dg  = (a * (dg - sg) >> 8) + sg;\
				db  = (a * (db - sb) >> 8) + sb;\
			}\
			*d = ((dr >> rl) << ro) | ((dg >> gl) << go) | ((db >> bl) << bo);\
		}\
		d++;\
		s++;\
		m++;\
	}

#define memsetloop1()\
	while (n--) {\
		if (*m == id) {\
			*d = dc;\
		}\
		d++;\
		m++;\
	}

static inline void s_memcpy1o (unsigned char *m, int id, void *dest, void *src, int n)
{
	unsigned char *d = (unsigned char *) dest;
	unsigned char *s = (unsigned char *) src;
	memcpyloop1();
}

static inline void s_memcpy2o (unsigned char *m, int id, void *dest, void *src, int n)
{
	unsigned short int *d = (unsigned short int *) dest;
	unsigned short int *s = (unsigned short int *) src;
	memcpyloop1();
}

static inline void s_memcpy4o (unsigned char *m, int id, void *dest, void *src, int n)
{
	unsigned int *d = (unsigned int *) dest;
	unsigned int *s = (unsigned int *) src;
	memcpyloop1();
}

static inline void s_memcpy1om (unsigned char *m, int id, void *dest, void *src, unsigned char *mask, int n)
{
	unsigned char *d = (unsigned char *) dest;
	unsigned char *s = (unsigned char *) src;
	unsigned char *m_ = (unsigned char *) mask;
	memcpyloop2();
}

static inline void s_memcpy2om (unsigned char *m, int id, void *dest, void *src, unsigned char *mask, int n)
{
	unsigned short int *d = (unsigned short int *) dest;
	unsigned short int *s = (unsigned short int *) src;
	unsigned char *m_ = (unsigned char *) mask;
	memcpyloop2();
}

static inline void s_memcpy4om (unsigned char *m, int id, void *dest, void *src, unsigned char *mask, int n)
{
	unsigned int *d = (unsigned int *) dest;
	unsigned int *s = (unsigned int *) src;
	unsigned char *m_ = (unsigned char *) mask;
	memcpyloop2();
}

static inline void s_memcpy1oa (unsigned char *m, int id, void *dest, void *src, unsigned char *mask, int n, int rl, int gl, int bl, int ro, int go, int bo)
{
	unsigned char a;
	unsigned char sr;
	unsigned char sg;
	unsigned char sb;
	unsigned char dr;
	unsigned char dg;
	unsigned char db;
	unsigned char *d = (unsigned char *) dest;
	unsigned char *s = (unsigned char *) src;
	unsigned char *m_ = (unsigned char *) mask;
	rl = 8 - rl;
	gl = 8 - gl;
	bl = 8 - bl;
	memcpyloop6();
}

static inline void s_memcpy2oa (unsigned char *m, int id, void *dest, void *src, unsigned char *mask, int n, int rl, int gl, int bl, int ro, int go, int bo)
{
	unsigned char a;
	unsigned char sr;
	unsigned char sg;
	unsigned char sb;
	unsigned char dr;
	unsigned char dg;
	unsigned char db;
	unsigned short int *d = (unsigned short int *) dest;
	unsigned short int *s = (unsigned short int *) src;
	unsigned char *m_ = (unsigned char *) mask;
	rl = 8 - rl;
	gl = 8 - gl;
	bl = 8 - bl;
	memcpyloop6();
}

static inline void s_memcpy4oa (unsigned char *m, int id, void *dest, void *src, unsigned char *mask, int n, int rl, int gl, int bl, int ro, int go, int bo)
{
	unsigned char a;
	unsigned char sr;
	unsigned char sg;
	unsigned char sb;
	unsigned char dr;
	unsigned char dg;
	unsigned char db;
	unsigned int *d = (unsigned int *) dest;
	unsigned int *s = (unsigned int *) src;
	unsigned char *m_ = (unsigned char *) mask;
	rl = 8 - rl;
	gl = 8 - gl;
	bl = 8 - bl;
	memcpyloop6();
}

static inline void s_memcpy1orgb (unsigned char *m, int id, void *dest, unsigned int *rgb, int n, int rl, int gl, int bl, int ro, int go, int bo)
{
	unsigned char dr;
	unsigned char dg;
	unsigned char db;
	unsigned char *d = (unsigned char *) dest;
	unsigned int *s = (unsigned int *) rgb;
	rl = 8 - rl;
	gl = 8 - gl;
	bl = 8 - bl;
	memcpyloop8();
}

static inline void s_memcpy2orgb (unsigned char *m, int id, void *dest, unsigned int *rgb, int n, int rl, int gl, int bl, int ro, int go, int bo)
{
	unsigned char dr;
	unsigned char dg;
	unsigned char db;
	unsigned short int *d = (unsigned short int *) dest;
	unsigned int *s = (unsigned int *) rgb;
	rl = 8 - rl;
	gl = 8 - gl;
	bl = 8 - bl;
	memcpyloop8();
}

static inline void s_memcpy4orgb (unsigned char *m, int id, void *dest, unsigned int *rgb, int n, int rl, int gl, int bl, int ro, int go, int bo)
{
	unsigned char dr;
	unsigned char dg;
	unsigned char db;
	unsigned int *d = (unsigned int *) dest;
	unsigned int *s = (unsigned int *) rgb;
	rl = 8 - rl;
	gl = 8 - gl;
	bl = 8 - bl;
	memcpyloop8();
}

static inline void s_memcpy1orgba (unsigned char *m, int id, void *dest, unsigned int *rgb, int n, int rl, int gl, int bl, int ro, int go, int bo)
{
	unsigned char a;
	unsigned char sr;
	unsigned char sg;
	unsigned char sb;
	unsigned char dr;
	unsigned char dg;
	unsigned char db;
	unsigned char *d = (unsigned char *) dest;
	unsigned int *s = (unsigned int *) rgb;
	rl = 8 - rl;
	gl = 8 - gl;
	bl = 8 - bl;
	memcpyloop10();
}

static inline void s_memcpy2orgba (unsigned char *m, int id, void *dest, unsigned int *rgb, int n, int rl, int gl, int bl, int ro, int go, int bo)
{
	unsigned char a;
	unsigned char sr;
	unsigned char sg;
	unsigned char sb;
	unsigned char dr;
	unsigned char dg;
	unsigned char db;
	unsigned short int *d = (unsigned short int *) dest;
	unsigned int *s = (unsigned int *) rgb;
	rl = 8 - rl;
	gl = 8 - gl;
	bl = 8 - bl;
	memcpyloop10();
}

static inline void s_memcpy4orgba (unsigned char *m, int id, void *dest, unsigned int *rgb, int n, int rl, int gl, int bl, int ro, int go, int bo)
{
	unsigned char a;
	unsigned char sr;
	unsigned char sg;
	unsigned char sb;
	unsigned char dr;
	unsigned char dg;
	unsigned char db;
	unsigned int *d = (unsigned int *) dest;
	unsigned int *s = (unsigned int *) rgb;
	rl = 8 - rl;
	gl = 8 - gl;
	bl = 8 - bl;
	memcpyloop10();
}

#define s_memcpy1(dst, src, n) s_memcpy(dst, src, n * 1)
#define s_memcpy2(dst, src, n) s_memcpy(dst, src, n * 2)
#define s_memcpy4(dst, src, n) s_memcpy(dst, src, n * 4)
static inline void s_memcpy (void *dst, void *src, unsigned int n)
{
	unsigned int l;
	l = n / sizeof(unsigned int);
	while (l--) {
		*(unsigned int *) dst = *(unsigned int *) src;
		dst += 4;
		src += 4;
	}
	l = n % sizeof(unsigned int);
	switch (l) {
		case 1:
			*(unsigned char *) dst = *(unsigned char *) src;
			break;
		case 2:
			*(unsigned short *) dst = *(unsigned short *) src;
			break;
		case 3:
		        *(unsigned char *) dst++ = *(unsigned char *) src;
			*(unsigned short *) dst = *(unsigned short *) src;
			break;
	}
}

static inline void s_memcpy1m (void *dest, void *src, unsigned char *mask, int n)
{
	unsigned char *m = (unsigned char *) mask;
	unsigned char *d = (unsigned char *) dest;
	unsigned char *s = (unsigned char *) src;
	memcpyloop4();
}

static inline void s_memcpy2m (void *dest, void *src, unsigned char *mask, int n)
{
	unsigned char *m = (unsigned char *) mask;
	unsigned short int *d = (unsigned short int *) dest;
	unsigned short int *s = (unsigned short int *) src;
	memcpyloop4();
}

static inline void s_memcpy4m (void *dest, void *src, unsigned char *mask, int n)
{
	unsigned char *m = (unsigned char *) mask;
	unsigned int *d = (unsigned int *) dest;
	unsigned int *s = (unsigned int *) src;
	memcpyloop4();
}

static inline void s_memcpy1a (void *dest, void *src, unsigned char *mask, int n, int rl, int gl, int bl, int ro, int go, int bo)
{
	unsigned char sr;
	unsigned char sg;
	unsigned char sb;
	unsigned char dr;
	unsigned char dg;
	unsigned char db;
	unsigned char a;
	unsigned char *m = (unsigned char *) mask;
	unsigned char *d = (unsigned char *) dest;
	unsigned char *s = (unsigned char *) src;
	rl = 8 - rl;
	gl = 8 - gl;
	bl = 8 - bl;
	memcpyloop5();
}

static inline void s_memcpy2a (void *dest, void *src, unsigned char *mask, int n, int rl, int gl, int bl, int ro, int go, int bo)
{
	unsigned char sr;
	unsigned char sg;
	unsigned char sb;
	unsigned char dr;
	unsigned char dg;
	unsigned char db;
	unsigned char a;
	unsigned char *m = (unsigned char *) mask;
	unsigned short int *d = (unsigned short int *) dest;
	unsigned short int *s = (unsigned short int *) src;
	rl = 8 - rl;
	gl = 8 - gl;
	bl = 8 - bl;
	memcpyloop5();
}

static inline void s_memcpy4a (void *dest, void *src, unsigned char *mask, int n, int rl, int gl, int bl, int ro, int go, int bo)
{
	unsigned char sr;
	unsigned char sg;
	unsigned char sb;
	unsigned char dr;
	unsigned char dg;
	unsigned char db;
	unsigned char a;
	unsigned char *m = (unsigned char *) mask;
	unsigned int *d = (unsigned int *) dest;
	unsigned int *s = (unsigned int *) src;
	rl = 8 - rl;
	gl = 8 - gl;
	bl = 8 - bl;
	memcpyloop5();
}

static inline void s_memcpy1rgb (void *dest, unsigned int *rgb, int n, int rl, int gl, int bl, int ro, int go, int bo)
{
	unsigned char dr;
	unsigned char dg;
	unsigned char db;
	unsigned char *d = (unsigned char *) dest;
	unsigned int *s = (unsigned int *) rgb;
	rl = 8 - rl;
	gl = 8 - gl;
	bl = 8 - bl;
	memcpyloop7();
}

static inline void s_memcpy2rgb (void *dest, unsigned int *rgb, int n, int rl, int gl, int bl, int ro, int go, int bo)
{
	unsigned char dr;
	unsigned char dg;
	unsigned char db;
	unsigned short int *d = (unsigned short int *) dest;
	unsigned int *s = (unsigned int *) rgb;
	rl = 8 - rl;
	gl = 8 - gl;
	bl = 8 - bl;
	memcpyloop7();
}

static inline void s_memcpy4rgb (void *dest, unsigned int *rgb, int n, int rl, int gl, int bl, int ro, int go, int bo)
{
	unsigned char dr;
	unsigned char dg;
	unsigned char db;
	unsigned int *d = (unsigned int *) dest;
	unsigned int *s = (unsigned int *) rgb;
	rl = 8 - rl;
	gl = 8 - gl;
	bl = 8 - bl;
	memcpyloop7();
}

static inline void s_memcpy1rgba (void *dest, unsigned int *rgba, int n, int rl, int gl, int bl, int ro, int go, int bo)
{
	unsigned char a;
	unsigned char sr;
	unsigned char sg;
	unsigned char sb;
	unsigned char dr;
	unsigned char dg;
	unsigned char db;
	unsigned char *d = (unsigned char *) dest;
	unsigned int *s = (unsigned int *) rgba;
	rl = 8 - rl;
	gl = 8 - gl;
	bl = 8 - bl;
	memcpyloop9();
}

static inline void s_memcpy2rgba (void *dest, unsigned int *rgba, int n, int rl, int gl, int bl, int ro, int go, int bo)
{
	unsigned char a;
	unsigned char sr;
	unsigned char sg;
	unsigned char sb;
	unsigned char dr;
	unsigned char dg;
	unsigned char db;
	unsigned short int *d = (unsigned short int *) dest;
	unsigned int *s = (unsigned int *) rgba;
	rl = 8 - rl;
	gl = 8 - gl;
	bl = 8 - bl;
	memcpyloop9();
}

static inline void s_memcpy4rgba (void *dest, unsigned int *rgba, int n, int rl, int gl, int bl, int ro, int go, int bo)
{
	unsigned char a;
	unsigned char sr;
	unsigned char sg;
	unsigned char sb;
	unsigned char dr;
	unsigned char dg;
	unsigned char db;
	unsigned int *d = (unsigned int *) dest;
	unsigned int *s = (unsigned int *) rgba;
	rl = 8 - rl;
	gl = 8 - gl;
	bl = 8 - bl;
	memcpyloop9();
}

static inline void s_memset1o (unsigned char *m, int id, void *dest, int c, int n)
{
	unsigned char dc = (unsigned char) c;
	unsigned char *d = (unsigned char *) dest;
	memsetloop1();
}

static inline void s_memset2o (unsigned char *m, int id, void *dest, int c, int n)
{
	unsigned short int dc = (unsigned short int) c;
	unsigned short int *d = (unsigned short int *) dest;
	memsetloop1();
}

static inline void s_memset4o (unsigned char *m, int id, void *dest, int c, int n)
{
	unsigned int dc = (unsigned int) c;
	unsigned int *d = (unsigned int *) dest;
	memsetloop1();
}

#define s_memset1(dst, c, n) s_memset(dst, c * 0x01010101UL, n * 1)
#define s_memset2(dst, c, n) s_memset(dst, c * 0x00010001UL, n * 2)
#define s_memset4(dst, c, n) s_memset(dst, c * 0x00000001UL, n * 4)
static inline void s_memset (void *dst, unsigned int c, unsigned int n)
{
	unsigned int l;
	l = n / sizeof(unsigned int);
	while (l--) {
		*(unsigned int *) dst = (unsigned int) c;
		dst += 4;
	}
	l = n % sizeof(unsigned int);
	switch (l) {
		case 1:
			*(unsigned char *) dst = (unsigned char) c;
			break;
		case 2:
			*(unsigned short *) dst = (unsigned short) c;
			break;
		case 3:
		        *(unsigned char *) dst++ = (unsigned char) c;
			*(unsigned short *) dst = (unsigned short) c;
			break;
	}
}

#ifdef __cplusplus
}
#endif
