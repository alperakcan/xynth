/***************************************************************************
    begin                : Mon Oct 20 2003
    copyright            : (C) 2003 - 2008 by Alper Akcan
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

#include "config.h"

#if defined(CONFIG_VIDEO_HELPER_MOUSE)

/* mouse library functions (mouse_1.c) and mouse driver (mouse_2.c) are heavily
 * based on svgalib source code. here is the comments taken from the original
 * files.
 *
 * mouse library;
 * hhanemaa@cs.ruu.nl
 * Mouse library functions
 *
 * mouse drivers;
 * Based on:
 * simple driver for serial mouse
 * Andrew Haylett, 14th December 1992
 * and on the driver in XFree86.
 * Edited for svgalib (hhanemaa@cs.ruu.nl).
 * This probably doesn't work with all types of bus mouse.
 * HH: Added PS/2 mouse support.
 * Fixed Logitech support thanks to Daniel Jackson.
 * MouseSystems movement overflow fixed by Steve VanDevender.
 * Logitech fixed again.
 * Michael: Added support for controlling DTR and RTS.
 * Added mouse acceleration 3/97 - Mike Chapman mike@paranoia.com
 * Added Intellimouse support 5/98 - Brion Vibber brion@pobox.com
 * Totally customisable mouse behaviour 28 Mar 1998 - by 101; 101@kempelen.inf.bme.hu
 * Added rx-axis support for IntelliMouse wheel and improved fake keyboard
 *  event scancode setting 7/98 - Brion
 */

#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <math.h>

#define zero_sa_mask(maskptr) memset(maskptr, 0, sizeof(sigset_t))

static void mouse_handler (int button, int dx, int dy, int dz, int drx, int dry, int drz);

/* include low-level mouse driver */
#include "mouse_2.c"

static void mouse_int_handler (int signal)
{
	mouse_close();
	/* restore old interrupt */
	sigaction(SIGINT, &ms.oldsiga, NULL);
	raise(SIGINT);
}

static int mouse_init_return_fd (char *dev, int type, int samplerate)
{
	struct sigaction siga;

	if (!ms.mouse_open) {
		if (strcmp(dev, "") == 0)
			ms.dev = "/dev/mouse";
		else
			ms.dev = dev;
		ms.type = type & MOUSE_TYPE_MASK;
		ms.modem_ctl = type & ~MOUSE_TYPE_MASK;
		ms.sample = samplerate;
		ms.currentinthandler = NULL;
		/* Initialize mouse device. */
		if (ms.type == MOUSE_NONE || ms.type < MOUSE_MICROSOFT || ms.type > MOUSE_LAST)
			return -1;
		if (ms_init())
			return -1;
		/* Install interrupt handler. */
		ms.currentinthandler = mouse_int_handler;
		siga.sa_handler = mouse_int_handler;
		siga.sa_flags = 0;
		zero_sa_mask(&(siga.sa_mask));
		sigaction(SIGINT, &siga, &ms.oldsiga);
		ms.mouse_open = 1;
	}
	return ms.fd;	/* Return mouse fd. */
}

/* Reads an event from the mouse buffer.
   Warning: For now, we assume there's no console switching. */
static int mouse_update (void)
{
	int result;

	result = get_ms_event(0);

	return result;
}

static void mouse_close (void)
{
	ms_close();
	if (ms.currentinthandler != NULL)
		/* Restore old interrupt. */
		sigaction(SIGINT, &ms.oldsiga, NULL);
}

static void mouse_handler (int button, int dx, int dy, int dz, int drx, int dry, int drz)
{
	ms.mouse_button = button;

	ms.mouse.x += dx;
	ms.mouse.y += dy;
	ms.mouse.z += dz;
	if (ms.mouse.x / ms.mouse_scale > ms.mouse_max.x) {
		ms.mouse.x = ms.mouse_max.x * ms.mouse_scale;
	}
	if (ms.mouse.x / ms.mouse_scale < ms.mouse_min.x) {
		ms.mouse.x = ms.mouse_min.x * ms.mouse_scale;
	}
	if (ms.mouse.y / ms.mouse_scale > ms.mouse_max.y) {
		ms.mouse.y = ms.mouse_max.y * ms.mouse_scale;
	}
	if (ms.mouse.y / ms.mouse_scale < ms.mouse_min.y) {
		ms.mouse.y = ms.mouse_min.y * ms.mouse_scale;
	}
	if (ms.mouse.z / ms.mouse_scale > ms.mouse_max.z) {
		ms.mouse.z = ms.mouse_max.z * ms.mouse_scale;
	}
	if (ms.mouse.z / ms.mouse_scale < ms.mouse_min.z) {
		ms.mouse.z = ms.mouse_min.z * ms.mouse_scale;
	}

	ms.mouse.rx = drx;
	ms.mouse.ry = dry;
	ms.mouse.rz = drz;
	if (ms.mouse.rx / ms.mouse_scale > ms.mouse_max.rx) {
		ms.mouse.rx = ms.mouse_max.rx * ms.mouse_scale;
	}
	if (ms.mouse.rx / ms.mouse_scale < ms.mouse_min.rx) {
		ms.mouse.rx = ms.mouse_min.rx * ms.mouse_scale;
	}
	if (ms.mouse.ry / ms.mouse_scale > ms.mouse_max.ry) {
		ms.mouse.ry = ms.mouse_max.ry * ms.mouse_scale;
	}
	if (ms.mouse.ry / ms.mouse_scale < ms.mouse_min.ry) {
		ms.mouse.ry = ms.mouse_min.ry * ms.mouse_scale;
	}
	if (ms.mouse.rz / ms.mouse_scale > ms.mouse_max.rz) {
		ms.mouse.rz = ms.mouse_max.rz * ms.mouse_scale;
	}
	if (ms.mouse.rz / ms.mouse_scale < ms.mouse_min.rz) {
		ms.mouse.rz = ms.mouse_min.rz * ms.mouse_scale;
	}
}

/* Set the mouse position for up to six dimensions. */
static void mouse_setposition_6d (int x, int y, int z, int rx, int ry, int rz, int dim_mask)
{
	if (dim_mask & MOUSE_XDIM)
		ms.mouse.x = x * ms.mouse_scale;
	if (dim_mask & MOUSE_YDIM)
		ms.mouse.y = y * ms.mouse_scale;
	if (dim_mask & MOUSE_ZDIM)
		ms.mouse.z = z * ms.mouse_scale;
	if (dim_mask & MOUSE_RXDIM)
		ms.mouse.rx = rx * ms.mouse_scale;
	if (dim_mask & MOUSE_RYDIM)
		ms.mouse.ry = ry * ms.mouse_scale;
	if (dim_mask & MOUSE_RZDIM)
		ms.mouse.rz = rz * ms.mouse_scale;
}

/* Set the mouse range for up to six dimensions. */
static void mouse_setrange_6d (int x1, int x2, int y1, int y2, int z1, int z2, int rx1, int rx2, int ry1, int ry2, int rz1, int rz2, int dim_mask)
{
	if (dim_mask & MOUSE_XDIM) {
		ms.mouse_min.x = x1;
		ms.mouse_max.x = x2;
	}
	if (dim_mask & MOUSE_YDIM) {
		ms.mouse_min.y = y1;
		ms.mouse_max.y = y2;
	}
	if (dim_mask & MOUSE_ZDIM) {
		ms.mouse_min.z = z1;
		ms.mouse_max.z = z2;
	}
	if (dim_mask & MOUSE_RXDIM) {
		ms.mouse_min.rx = rx1;
		ms.mouse_max.rx = rx2;
	}
	if (dim_mask & MOUSE_RYDIM) {
		ms.mouse_min.ry = ry1;
		ms.mouse_max.ry = ry2;
	}
	if (dim_mask & MOUSE_RZDIM) {
		ms.mouse_min.rz = rz1;
		ms.mouse_max.rz = rz2;
	}
}

/* set the sensitivity of the mouse
   This routine sets the ms.mouse_scale factor between the motion reported by the
   mouse and the size of one pixel.  The larger ms.mouse_scale is, the slower the
   mouse cursor apears to move.
   Bugs: Scale cannot be set less than one, since it is an integer.  This
   means that there is no war to make the mouse faster than it inherently
   is.*/
static void mouse_setscale (int s)
{
	if (ms.mouse_scale == 0) {
		debugf(DSER, "MOUSE : Mouse ms.mouse_scale must be non-zero!");
	} else {
		ms.mouse.x = (ms.mouse.x * s) / ms.mouse_scale;
		ms.mouse.y = (ms.mouse.y * s) / ms.mouse_scale;
		ms.mouse.z = (ms.mouse.z * s) / ms.mouse_scale;
		ms.mouse.rx = (ms.mouse.rx * s) / ms.mouse_scale;
		ms.mouse.ry = (ms.mouse.ry * s) / ms.mouse_scale;
		ms.mouse.rz = (ms.mouse.rz * s) / ms.mouse_scale;
		ms.mouse_scale = s;
	}
}

static void mouse_getposition_6d (int *x, int *y, int *z, int *rx, int *ry, int *rz, int *btns)
{
	if (x)	*x = ms.mouse.x / ms.mouse_scale;
	if (y)	*y = ms.mouse.y / ms.mouse_scale;
	if (z)	*z = ms.mouse.z / ms.mouse_scale;
	if (rx)	*rx = ms.mouse.rx / ms.mouse_scale;
	if (ry)	*ry = ms.mouse.ry / ms.mouse_scale;
	if (rz)	*rz = ms.mouse.rz / ms.mouse_scale;
	if (btns) *btns = ms.mouse_button;
}

#endif /* CONFIG_VIDEO_HELPER_MOUSE */
