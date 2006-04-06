/*
Linux Real Mode Interface - A library of DPMI-like functions for Linux.

Copyright (C) 1998 by Josh Vanderhoof

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL JOSH VANDERHOOF BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef LRMI_H
#define LRMI_H

#if defined(__i386__) && (defined(__linux__) || defined(__NetBSD__) \
	|| defined(__FreeBSD__) || defined(__OpenBSD__))

struct LRMI_regs {
	unsigned int edi;
	unsigned int esi;
	unsigned int ebp;
	unsigned int reserved;
	unsigned int ebx;
	unsigned int edx;
	unsigned int ecx;
	unsigned int eax;
	unsigned short int flags;
	unsigned short int es;
	unsigned short int ds;
	unsigned short int fs;
	unsigned short int gs;
	unsigned short int ip;
	unsigned short int cs;
	unsigned short int sp;
	unsigned short int ss;
};

#ifndef LRMI_PREFIX
#define LRMI_PREFIX LRMI_
#endif

#define LRMI_CONCAT2(a, b) 	a ## b
#define LRMI_CONCAT(a, b) 	LRMI_CONCAT2(a, b)
#define LRMI_MAKENAME(a) 	LRMI_CONCAT(LRMI_PREFIX, a)

/*
 Package version (high 16bit = major, low 16bit minor)
*/
#define LRMI_version 0x0009 /* 0.9 */

/*
 Initialize
 returns 1 if sucessful, 0 for failure
*/
#define LRMI_init LRMI_MAKENAME(init)
int
LRMI_init(void);

/*
 Simulate a 16 bit far call
 returns 1 if sucessful, 0 for failure
*/
#define LRMI_call LRMI_MAKENAME(call)
int
LRMI_call(struct LRMI_regs *r);

/*
 Simulate a 16 bit interrupt
 returns 1 if sucessful, 0 for failure
*/
#define LRMI_int LRMI_MAKENAME(int)
int
LRMI_int(int interrupt, struct LRMI_regs *r);

/*
 Allocate real mode memory
 The returned block is paragraph (16 byte) aligned
*/
#define LRMI_alloc_real LRMI_MAKENAME(alloc_real)
void *
LRMI_alloc_real(int size);

/*
 Free real mode memory
*/
#define LRMI_free_real LRMI_MAKENAME(free_real)
void
LRMI_free_real(void *m);

#else /* (__linux__ || __NetBSD__ || __FreeBSD__) && __i386__ */
#warning "LRMI is not supported on your system!"
#endif

#endif
