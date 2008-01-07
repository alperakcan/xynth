/***************************************************************************
    begin                : Wed Oct 8 2003
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

#define	MOUSE_MICROSOFT		0
#define MOUSE_MOUSESYSTEMS	1
#define MOUSE_MMSERIES		2
#define MOUSE_LOGITECH		3
#define MOUSE_BUSMOUSE		4
#define MOUSE_PS2		5
#define MOUSE_LOGIMAN		6
#define MOUSE_GPM		7
#define MOUSE_SPACEBALL		8
#define MOUSE_NONE		9
#define MOUSE_INTELLIMOUSE	10
#define MOUSE_IMPS2		11
#define MOUSE_PNP		12
#define MOUSE_WACOM_GRAPHIRE	13
#define MOUSE_DRMOUSE4DS	14
#define MOUSE_EXPPS2		15
#define MOUSE_LAST 		MOUSE_EXPPS2

#define MOUSE_ORIENTATION_VERTICAL	0
#define MOUSE_ORIENTATION_HORIZONTAL	1

#define MOUSE_ACCEL_TYPE_NORMAL	1
#define MOUSE_ACCEL_TYPE_POWER	2

#define DEFAULT_ACCEL_THRESH	5
#define DEFAULT_ACCEL_POWER	1.2
#define DEFAULT_ACCEL_OFFSET	1
#define DEFAULT_ACCEL_MULT	10
#define DEFAULT_MAXDELTA	0
#define DEFAULT_ACCEL_MAXDELTA	0
#define DEFAULT_ACCEL_TYPE	0

#define DEFAULT_WHEEL_STEPS     18
#define DEFAULT_WHEEL_DELTA     (360 / DEFAULT_WHEEL_STEPS)

#define MOUSE_CHG_DTR	0x80000000
#define MOUSE_DTR_HIGH	0x40000000
#define MOUSE_CHG_RTS	0x20000000
#define MOUSE_RTS_HIGH	0x10000000

#define MOUSE_TYPE_MASK 0xffff

#define MOUSE_XDIM	1
#define MOUSE_YDIM	2
#define MOUSE_ZDIM	4
#define MOUSE_RXDIM	8
#define MOUSE_RYDIM	16
#define MOUSE_RZDIM	32
#define MOUSE_2DIM	3
#define MOUSE_3DIM	7
#define MOUSE_6DIM	63

static int mouse_init_return_fd (char *dev, int type, int samplerate);
static void mouse_close (void);
static int mouse_update (void);

static void mouse_setposition_6d (int x, int y, int z, int rx, int ry, int rz, int dim_mask);
static void mouse_setrange_6d (int x1, int x2, int y1, int y2, int z1, int z2, int rx1, int rx2, int ry1, int ry2, int rz1, int rz2, int dim_mask);
static void mouse_setscale (int s);

static void mouse_getposition_6d (int *x, int *y, int *z, int *rx, int *ry, int *rz, int *btns);
