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

#define memcpyloop3()\
	while (n--) {\
		*d++ = *s++;\
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

#define memsetloop1()\
	while (n--) {\
		if (*m == id) {\
			*d = dc;\
		}\
		d++;\
		m++;\
	}

#define memsetloop3()\
	while (n--) {\
		*d++ = dc;\
	}

static inline void s_memcpy1o (unsigned char *m, int id, char *dest, char *src, int n)
{
	unsigned char *d = (unsigned char *) dest;
	unsigned char *s = (unsigned char *) src;

	memcpyloop1();
}

static inline void s_memcpy2o (unsigned char *m, int id, char *dest, char *src, int n)
{
	unsigned short int *d = (unsigned short int *) dest;
	unsigned short int *s = (unsigned short int *) src;
	
	memcpyloop1();
}

static inline void s_memcpy4o (unsigned char *m, int id, char *dest, char *src, int n)
{
	unsigned int *d = (unsigned int *) dest;
	unsigned int *s = (unsigned int *) src;

	memcpyloop1();
}

static inline void s_memcpy1om (unsigned char *m, int id, char *dest, char *src, unsigned char *mask, int n)
{
	unsigned char *d = (unsigned char *) dest;
	unsigned char *s = (unsigned char *) src;
        unsigned char *m_ = (unsigned char *) mask;

	memcpyloop2();
}

static inline void s_memcpy2om (unsigned char *m, int id, char *dest, char *src, unsigned char *mask, int n)
{
	unsigned short int *d = (unsigned short int *) dest;
	unsigned short int *s = (unsigned short int *) src;
        unsigned char *m_ = (unsigned char *) mask;

	memcpyloop2();
}

static inline void s_memcpy4om (unsigned char *m, int id, char *dest, char *src, unsigned char *mask, int n)
{
	unsigned int *d = (unsigned int *) dest;
	unsigned int *s = (unsigned int *) src;
        unsigned char *m_ = (unsigned char *) mask;

	memcpyloop2();
}

static inline void s_memcpy1 (char *dest, char *src, int n)
{
	unsigned char *d = (unsigned char *) dest;
	unsigned char *s = (unsigned char *) src;

	memcpyloop3();
}

static inline void s_memcpy2 (char *dest, char *src, int n)
{
	unsigned short int *d = (unsigned short int *) dest;
	unsigned short int *s = (unsigned short int *) src;

	memcpyloop3();
}

static inline void s_memcpy4 (char *dest, char *src, int n)
{
	unsigned int *d = (unsigned int *) dest;
	unsigned int *s = (unsigned int *) src;

	memcpyloop3();
}

static inline void s_memcpy1m (char *dest, char *src, unsigned char *mask, int n)
{
        unsigned char *m = (unsigned char *) mask;
	unsigned char *d = (unsigned char *) dest;
	unsigned char *s = (unsigned char *) src;

	memcpyloop4();
}

static inline void s_memcpy2m (char *dest, char *src, unsigned char *mask, int n)
{
        unsigned char *m = (unsigned char *) mask;
	unsigned short int *d = (unsigned short int *) dest;
	unsigned short int *s = (unsigned short int *) src;

	memcpyloop4();
}

static inline void s_memcpy4m (char *dest, char *src, unsigned char *mask, int n)
{
        unsigned char *m = (unsigned char *) mask;
	unsigned int *d = (unsigned int *) dest;
	unsigned int *s = (unsigned int *) src;

	memcpyloop4();
}

static inline void s_memset1o (unsigned char *m, int id, char *dest, int c, int n)
{
        unsigned char dc = (unsigned char) c;
	unsigned char *d = (unsigned char *) dest;

	memsetloop1();
}

static inline void s_memset2o (unsigned char *m, int id, char *dest, int c, int n)
{
        unsigned short int dc = (unsigned short int) c;
	unsigned short int *d = (unsigned short int *) dest;
	
	memsetloop1();
	
}

static inline void s_memset4o (unsigned char *m, int id, char *dest, int c, int n)
{
        unsigned int dc = (unsigned int) c;
	unsigned int *d = (unsigned int *) dest;

	memsetloop1();
}

static inline void s_memset1 (char *dest, int c, int n)
{
        unsigned char dc = (unsigned char) c;
	unsigned char *d = (unsigned char *) dest;

	memsetloop3();
}

static inline void s_memset2 (char *dest, int c, int n)
{
        unsigned short int dc = (unsigned short int) c;
	unsigned short int *d = (unsigned short int *) dest;

	memsetloop3();
}

static inline void s_memset4 (char *dest, int c, int n)
{
        unsigned int dc = (unsigned int) c;
	unsigned int *d = (unsigned int *) dest;
	
	memsetloop3();
}

#ifdef __cplusplus
}
#endif
