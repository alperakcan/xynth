/***************************************************************************
    begin                : Mon Oct 20 2003
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

#if defined(VIDEO_HELPER_MOUSE)

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

#define MS3B

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <math.h>


#if defined(DEBUG)
/*
	#define DEBUG_MOUSE
	#define DEBUG_ACCEL
	#define DEBUG_WHEEL
*/
#endif

struct mouse_coor {
	int x;
	int y;
	int z;
	int rx;
	int ry;
	int rz;
};

struct mouse_accel {
	char type;
	int thresh;
	float mult;
	int maxdelta;
	int amaxdelta;
	float *powertable;
	float power;
	float offset;
	int force;
};

static struct {
	int type;
	int baud;
	int sample;
	char *dev;
	int fd;
	int fd_mode;
	int modem_ctl;
	unsigned int old_modem_info;
	char modem_info_valid;
	struct mouse_accel accel;

	int wheel_steps;
	int wheel_delta;

	struct mouse_coor mouse;
	struct mouse_coor mouse_min;
	struct mouse_coor mouse_max;

	int mouse_button;
	int mouse_scale;

	int mouse_open;
	int mouse_flag;
	char *mouse_dev;

	void (*currentinthandler) (int);
	struct sigaction oldsiga;
} ms = {0,			/* type			*/
	1200,			/* baud			*/
	0,			/* sample		*/
	NULL,			/* *dev			*/
	-1,			/* fd			*/
	0,			/* fd_mode		*/
	0,			/* modem_ctl		*/
	0,			/* old_modem_info	*/
	0,			/* modem_info_valid	*/
	/* accel     */	{DEFAULT_ACCEL_TYPE, DEFAULT_ACCEL_THRESH, DEFAULT_ACCEL_MULT,
			 DEFAULT_MAXDELTA, DEFAULT_ACCEL_MAXDELTA, NULL, DEFAULT_ACCEL_POWER,
			 DEFAULT_ACCEL_OFFSET, 0},
	DEFAULT_WHEEL_STEPS,	/* wheel steps		*/
	DEFAULT_WHEEL_DELTA,	/* wheel delta		*/
	/* mouse     */	{0, 0, 0, 0, 0, 0},
	/* mouse_min */	{0, 0, 0, 0, 0, 0},
	/* mouse_max */	{32767, 32767, 32767, 32767, 32767, 32767},
	0,			/* mouse_button		*/
	1,			/* mouse_scale		*/
	0,			/* mouse_open		*/
	0,			/* mouse_flag		*/
	"/dev/psaux",		/* *mouse_dev		*/
	NULL,			/* *currentinthandler	*/
				/* oldsiga		*/
};

static const unsigned short mouse_cflag[] = {
	(CS7 | CREAD | CLOCAL | HUPCL),			 /* MicroSoft			*/
	(CS8 | CSTOPB | CREAD | CLOCAL | HUPCL),	 /* MouseSystems		*/
	(CS8 | PARENB | PARODD | CREAD | CLOCAL | HUPCL),/* MMSeries			*/
	(CS8 | CSTOPB | CREAD | CLOCAL | HUPCL),	 /* Logitech			*/
	0,						 /* BusMouse			*/
	0,						 /* PS/2			*/
	(CS7 | CREAD | CLOCAL | HUPCL),			 /* MouseMan			*/
	(CS8 | CSTOPB | CREAD | CLOCAL | HUPCL),	 /* GPM (MouseSystems)		*/
	(CS8 | CLOCAL | CREAD | IXON | IXOFF ),		 /* Spaceball			*/
	0,						 /* Dummy entry for MOUSE_NONE	*/
	(CS7 | CREAD | CLOCAL | HUPCL),			 /* IntelliMouse (Serial)	*/
	CS7,       					 /* IntelliMouse (PS/2)		*/
	(CS7 | CREAD | CLOCAL | HUPCL),			 /* plug'n'pray			*/
	(CS8 | CREAD | CLOCAL | HUPCL),			 /* Wacom Graphire tablet/mouse	*/
	0,						 /* DRMOUSE4DS			*/
	0,						 /* IntelliMouse Explorer (PS/2)*/
};

static const unsigned char mouse_proto[][5] = {
	/* hd_mask  hd_id  dp_mask  dp_id  nobytes					*/
	{0x40, 0x40, 0x40, 0x00, 3},	/* MicroSoft					*/
	{0xf8, 0x80, 0x00, 0x00, 5},	/* MouseSystems					*/
	{0xe0, 0x80, 0x80, 0x00, 3},	/* MMSeries					*/
	{0xe0, 0x80, 0x00, 0x00, 3},	/* Logitech					*/
	{0xf8, 0x80, 0x00, 0x00, 5},	/* BusMouse					*/
	{0xc0, 0x00, 0x00, 0x00, 3},	/* PS/2 mouse					*/
	{0x40, 0x40, 0x40, 0x00, 3},	/* Mouseman					*/
	{0xf8, 0x80, 0x00, 0x00, 5},	/* gpm (MouseSystems)				*/
	{0xe0, 0x40, 0x00, 0x00, 6},	/* Spaceball					*/
	{0, 0, 0, 0, 0},		/* Dummy entry for MOUSE_NONE			*/
	{0xc0, 0x40, 0xc0, 0x00, 4},	/* IntelliMouse (Serial)			*/
	{0xc8, 0x08, 0x00, 0x00, 4},	/* IntelliMouse (PS/2)				*/
	{0x40, 0x40, 0x40, 0x00, 3},	/* pnp						*/
	{0x80, 0x80, 0x80, 0x00, 7},	/* Wacom Graphire				*/
	{0xc8, 0x08, 0x00, 0x00, 4},	/* DRMOUSE4DS (Digital Research 2-wheel PS/2)	*/
	{0xc8, 0x08, 0x00, 0x00, 4},	/* IntelliMouse Explorer (PS/2)			*/
};

static void ms_setspeed (const int old, const int new, const unsigned short c_cflag)
{
	char *c;
	struct termios tty;

	tcgetattr(ms.fd, &tty);

	tty.c_iflag = IGNBRK | IGNPAR;
	tty.c_oflag = 0;
	tty.c_lflag = 0;
	tty.c_line  = 0;
	tty.c_cc[VTIME] = 0;
	tty.c_cc[VMIN]  = 1;

	switch (old) {
		case 9600:
			tty.c_cflag = c_cflag | B9600;
			break;
		case 4800:
			tty.c_cflag = c_cflag | B4800;
			break;
		case 2400:
			tty.c_cflag = c_cflag | B2400;
			break;
		case 1200:
		default:
			tty.c_cflag = c_cflag | B1200;
			break;
	}

	tcsetattr(ms.fd, TCSAFLUSH, &tty);

	switch (new) {
		case 9600:
			c = "*q";
			tty.c_cflag = c_cflag | B9600;
			break;
		case 4800:
			c = "*p";
			tty.c_cflag = c_cflag | B4800;
			break;
		case 2400:
			c = "*o";
			tty.c_cflag = c_cflag | B2400;
			break;
		case 1200:
		default:
			c = "*n";
			tty.c_cflag = c_cflag | B1200;
			break;
	}

	write(ms.fd, c, 2);
	usleep(10000);
	tcsetattr(ms.fd, TCSAFLUSH, &tty);
}

static int ms_init (void)
{
	if (ms.accel.maxdelta < 0  &&  !ms.accel.force) {
		ms.accel.maxdelta = DEFAULT_MAXDELTA;
	}
        if (ms.accel.amaxdelta < 0  &&  !ms.accel.force) {
		ms.accel.amaxdelta = DEFAULT_ACCEL_MAXDELTA;
	}
	if (ms.accel.type) {
		if ((ms.accel.mult <= 0  ||  ms.accel.mult > 200.0)  &&  !ms.accel.force) {
			ms.accel.mult = DEFAULT_ACCEL_MULT;
		}
		if ((ms.accel.power < -200.0  ||  ms.accel.power > 200.0)  &&  !ms.accel.force) {
			ms.accel.power = DEFAULT_ACCEL_POWER;
		}
		if ((ms.accel.offset < -100.0  ||  ms.accel.offset > 100.0)  &&  !ms.accel.force) {
			ms.accel.offset = DEFAULT_ACCEL_OFFSET;
		}
		if (!ms.accel.force) {
			if (ms.accel.thresh <= 0  ||  ms.accel.thresh > 200) {
				ms.accel.thresh = DEFAULT_ACCEL_THRESH;
			}
		} else if (ms.accel.thresh < 0) {
			ms.accel.thresh = DEFAULT_ACCEL_THRESH;
		}
		if (ms.accel.offset >= ms.accel.mult) {
			;
		}
		if (ms.accel.thresh > 1 && ms.accel.type == MOUSE_ACCEL_TYPE_POWER) {
			if ((ms.accel.powertable = (float *) s_malloc(ms.accel.thresh * sizeof(float))) == 0 ) {
				debugf(DFAT | DSER | DSYS, "MOUSE : Out of memory in mouse init !");
			} else {
				int i;
#if defined(DEBUG_ACCEL)
				debugf(DSER, "MOUSE : ms.accel.powertable :");
#endif
				for (i = 1; i < ms.accel.thresh; i++) {
					ms.accel.powertable[i] = pow((double) (i - 1) / (ms.accel.thresh-1), ms.accel.power)
								 * (ms.accel.mult - ms.accel.offset) + ms.accel.offset;
#if defined(DEBUG_ACCEL)
					debugf(DSER, "MOUSE :	%ld => %f (%f)", (long)i,
										 (double) ms.accel.powertable[i] * i,
										 (double) ms.accel.powertable[i]);
#endif
				}
			}
		}
	}

#if defined(DEBUG_ACCEL)
	debugf(DSER, "MOUSE : ms.accel.type: %ld",		(long) ms.accel.type);
	debugf(DSER, "MOUSE : ms.accel.force: %ld",	(long) ms.accel.force);
	debugf(DSER, "MOUSE : ms.accel.thresh: %ld",	(long) ms.accel.thresh);
	debugf(DSER, "MOUSE : ms.accel.offset: %ld",	(long) ms.accel.offset);
	debugf(DSER, "MOUSE : ms.accel.mult: %f",		(double) ms.accel.mult);
	debugf(DSER, "MOUSE : ms.accel.power: %f",		(double) ms.accel.power);
	debugf(DSER, "MOUSE : ms.accel.maxdelta: %ld",	(long) ms.accel.maxdelta);
	debugf(DSER, "MOUSE : ms.accel.amaxdelta: %ld",	(long) ms.accel.amaxdelta);
#endif
	/*  Ensure that the open will get a file descriptor greater
	 *  than 2, else problems can occur with stdio functions
	 *  under certain strange conditions:  */
	if (fcntl(0,F_GETFD) < 0) open("/dev/null", O_RDONLY);
	if (fcntl(1,F_GETFD) < 0) open("/dev/null", O_WRONLY);
	if (fcntl(2,F_GETFD) < 0) open("/dev/null", O_WRONLY);

	/* Set the proper wheel delta */
	if(ms.wheel_steps)
		ms.wheel_delta = (360 / ms.wheel_steps);
	else
		ms.wheel_delta = 0;

	/* Added O_NDELAY here. */
	if ((ms.fd = open(ms.dev, O_RDWR | O_NDELAY )) < 0)
		return -1;
	if (ms.type == MOUSE_BUSMOUSE || ms.type == MOUSE_PS2 ||
	    ms.type == MOUSE_IMPS2 || ms.type == MOUSE_GPM ||
	    ms.type == MOUSE_DRMOUSE4DS || ms.type == MOUSE_EXPPS2)
		ms.modem_ctl = 0;
	/* If no signal will change there is no need to restore
	  or safe original settings. */
	if (!ms.modem_ctl)
		ms.modem_info_valid = 0;
	else {
		/* Get current modem signals; keep silent on errors.. */
		ms.modem_info_valid = !ioctl(ms.fd,
		TIOCMGET, &ms.old_modem_info);
		if (ms.modem_info_valid) {
			unsigned int param = ms.old_modem_info;
			/* Prepare new stat: */
			/* set DTR as ordered.. */
			if (ms.modem_ctl & MOUSE_CHG_DTR) {
				param &= ~TIOCM_DTR;
				if (ms.modem_ctl & MOUSE_DTR_HIGH)
					param |= TIOCM_DTR;
			}
			/*set RTS as ordered.. */
			if (ms.modem_ctl & MOUSE_CHG_RTS) {
				param &= ~TIOCM_RTS;
				if (ms.modem_ctl & MOUSE_RTS_HIGH)
					param |= TIOCM_RTS;
			}
			if (ioctl(ms.fd, TIOCMSET, &param))
				ms.modem_info_valid = 0;	/* No try to restore if this failed */
		}
	}

	if (ms.type == MOUSE_SPACEBALL) {
		ms.baud = 9600;
		ms_setspeed(1200, ms.baud, mouse_cflag[ms.type]);
	} else if (ms.type == MOUSE_LOGIMAN) {
		ms_setspeed(9600, 1200, mouse_cflag[ms.type]);
		ms_setspeed(4800, 1200, mouse_cflag[ms.type]);
		ms_setspeed(2400, 1200, mouse_cflag[ms.type]);
		ms_setspeed(1200, 1200, mouse_cflag[ms.type]);
		write(ms.fd, "*X", 2);
		ms_setspeed(1200, ms.baud, mouse_cflag[ms.type]);
	} else if (ms.type == MOUSE_WACOM_GRAPHIRE) {
		ms.baud = 9600;
		ms_setspeed(1200, ms.baud, mouse_cflag[ms.type]);
		/* Reset baud rate */
		write(ms.fd, "\r$", 2);
		usleep(250000);
		/* Reset tablet */
		write(ms.fd, "\r#", 2);
		usleep(75000);
		/* Set hardware filtering */
		write(ms.fd, "\rSU3", 4);
		usleep(75000);
		/* Start sending coordinates */
		write(ms.fd, "\rST\r", 4);
	} else if (ms.type == MOUSE_IMPS2 || ms.type == MOUSE_DRMOUSE4DS) {
		/* Initialize the mouse into wheel mode */
		write(ms.fd, "\363\310\363\144\363\120", 6);
	} else if (ms.type == MOUSE_EXPPS2) {
		write(ms.fd, "\363\310\363\310\363\120", 6);
	} else if (ms.type == MOUSE_PNP) {
		/* Need to do this termios stuff here, by hand, ms_setspeed won't
		   work with pnp */
		struct termios tty;
		ms.baud = 1200;
		tcgetattr(ms.fd, &tty);
		tty.c_iflag = IGNBRK | IGNPAR;
		tty.c_oflag = 0;
		tty.c_lflag = 0;
		tty.c_line = 0;
		tty.c_cc[VTIME] = 0;
		tty.c_cc[VMIN] = 1;
		tty.c_cflag = mouse_cflag[ms.type] | B1200;
		tcsetattr(ms.fd, TCSAFLUSH, &tty);
	} else if (ms.type != MOUSE_BUSMOUSE && ms.type != MOUSE_PS2) {
		ms_setspeed(9600, ms.baud, mouse_cflag[ms.type]);
		ms_setspeed(4800, ms.baud, mouse_cflag[ms.type]);
		ms_setspeed(2400, ms.baud, mouse_cflag[ms.type]);
		ms_setspeed(1200, ms.baud, mouse_cflag[ms.type]);
		if (ms.type == MOUSE_LOGITECH) {
			write(ms.fd, "S", 1);
			ms_setspeed(ms.baud, ms.baud, mouse_cflag[MOUSE_MMSERIES]);
		}
		if (ms.sample <= 0)
			write(ms.fd, "O", 1);
		else if (ms.sample <= 15)
			write(ms.fd, "J", 1);
		else if (ms.sample <= 27)
			write(ms.fd, "K", 1);
		else if (ms.sample <= 42)
			write(ms.fd, "L", 1);
		else if (ms.sample <= 60)
			write(ms.fd, "R", 1);
		else if (ms.sample <= 85)
			write(ms.fd, "M", 1);
		else if (ms.sample <= 125)
			write(ms.fd, "Q", 1);
		else
			write(ms.fd, "N", 1);
	}

	return 0;
}

/* Scooped from X driver. */
static inline void ms_close (void)
{
	if (ms.accel.powertable)
		s_free(ms.accel.powertable);
	if (ms.fd == -1)
		return;
	if (ms.type == MOUSE_LOGITECH) {
		write(ms.fd, "U", 1);
		ms_setspeed(ms.baud, 1200, mouse_cflag[MOUSE_LOGITECH]);
	}
	/* Try to restore modem signals if we could get them. */
	if (ms.modem_info_valid)
		ioctl(ms.fd, TIOCMSET, &ms.old_modem_info);
	close(ms.fd);
	ms.mouse_open=0;
	ms.fd = -1;
}

#define MOUSEBUFFERSIZE 256

static int get_ms_event (int wait)
{
	/*
	   Changed to process multiple packets.
	   wait value:
	   0    Process any mouse events, and return status.
	   1    Wait for mouse event, then return.
	   Status indicates whether an event was processed.
	*/
	static unsigned char buf[MOUSEBUFFERSIZE];
	static int nu_bytes = 0;
	int nu_packets = 0;
	char event_handled = 0;
	int bytesread;
	int i, wheel;
	static unsigned int prev = 0;
	static int but = 0;		/* static is hack for MouseMan */
	static int mouse_orientation = 0;
	int dx = 0, dy = 0, dz = 0, drx = 0, dry = 0, drz = 0;
	int ax = 0, ay = 0;
	static int oldax = 0, olday = 0, nodev = 0;
	int j;
	char SpaceWare[] = "SpaceWare!";
	if (ms.fd == -1)
		return -1;
again:
	if (ms.fd_mode == 1) {
		/* We don't want to wait, set NDELAY mode. */
		fcntl(ms.fd, F_SETFL, O_RDONLY | O_NDELAY );
		ms.fd_mode = 0;
	}
	bytesread = read(ms.fd, &buf[nu_bytes], MOUSEBUFFERSIZE - nu_bytes);
	i = 0;
	if (bytesread >= 1)
		nu_bytes += bytesread;
#if defined(DEBUG_MOUSE)
	debugf(DSER, "MOUSE : #bytes in buffer: %d", nu_bytes);
#endif
handle_packets:
	/* Handle packets in buffer. */
#if defined(DEBUG_MOUSE)
	debugf(DSER, "MOUSE : Bytes left in buffer: %d at %d, packet is %d bytes", nu_bytes - i,
											i,
											mouse_proto[ms.type][4]);
	if (nu_bytes - i > 0)
		debugf(DSER, "MOUSE : Header byte: %c %d", (buf[i] & 0177), buf[i]);
#endif
	if ((ms.type == MOUSE_LOGIMAN) &&
	   ((nu_bytes - i) >= 1) &&
	   ((buf[i] & mouse_proto[ms.type][0]) != mouse_proto[ms.type][1]) &&
	   ((char) (buf[i] & ~0x33) == 0)) {
		/* s/23/33/, for 4-but trackman */
		/* Hack-o-matic, stolen from xf86_Mouse.c */
		but = ((buf[i] & 0x20) >> 4) | ((buf[i] & 0x10) >> 1) | (but & 0x05);
		mouse_handler(but, 0, 0, 0, 0, 0, 0);
		event_handled++;
		i++;
	}
	if ((ms.type == MOUSE_SPACEBALL)) {
		j = i;
		while ((nu_bytes - j > 0) && (buf[j]!=13))
			j++;
		nu_packets=(buf[j]==13);
	} else {
		nu_packets = 1;
	}
	if ((nu_packets==0)||(nu_bytes - i < mouse_proto[ms.type][4])) {
		/* No full packet available. */
		if (wait == 0 || (wait == 1 && event_handled)) {
			if (i >= nu_bytes) {
				nu_bytes = 0;
				i = 0;
			} else {
				/* Move partial packet to front of buffer. */
				for (j = i; j < nu_bytes; j++)
					buf[j - i] = buf[j];
				nu_bytes -= i;
			}
			return event_handled;
		} else {
			/* (wait == 1 && !event_handled) */
			if (i >= nu_bytes) {
				nu_bytes = 0;
				i = 0;
			}
			/* Wait mode, we'll sleep on reads. */
			fcntl(ms.fd, F_SETFL, O_RDONLY);
			ms.fd_mode = 1;
			read(ms.fd, &buf[nu_bytes], 1);
			if ((ms.type == MOUSE_SPACEBALL)) {
				nu_packets=(buf[nu_bytes]==13);
			} else {
				nu_packets=1;
			}
			nu_bytes++;
			if ((nu_packets==0)||(nu_bytes - i < mouse_proto[ms.type][4]))
				/* Not a complete packet. */
				goto again;
		}
	}

	/* Check header byte. */
	if ((buf[i] & mouse_proto[ms.type][0]) != mouse_proto[ms.type][1]) {
		/* Not a header byte. */
#if defined(DEBUG_MOUSE)
		debugf(DSER, "MOUSE : Bad header byte: %c %d", (buf[i] & 0177), buf[i]);
#endif
		i++;
		goto handle_packets;
	}

	/* Check whether it's a valid data packet. */
	if ((ms.type != MOUSE_PS2)&&(ms.type != MOUSE_IMPS2) &&
	    (ms.type != MOUSE_EXPPS2)&&(ms.type != MOUSE_SPACEBALL) &&
	    (ms.type != MOUSE_WACOM_GRAPHIRE) && (ms.type != MOUSE_DRMOUSE4DS))
	    	for (j = 1; j < mouse_proto[ms.type][4]; j++)
	     		if ((buf[i + j] & mouse_proto[ms.type][2]) != mouse_proto[ms.type][3] ||
	       		     buf[i + j] == 0x80) {
		       		i = i + j + 1;
		         	goto handle_packets;
			}
	/* Construct the event. */
	switch (ms.type) {
#if defined(MS3B)
		case MOUSE_MICROSOFT:	/* Microsoft */
			but = (but & 8) | ((buf[i] & 0x20) >> 3) | ((buf[i] & 0x10) >> 4);
			dx = (char) (((buf[i] & 0x03) << 6) | (buf[i + 1] & 0x3F));
			dy = (char) (((buf[i] & 0x0C) << 4) | (buf[i + 2] & 0x3F));
			if((dx == 0) && (dy == 0) && (but == (prev &~ MOUSE_MIDDLEBUTTON)))
				but = prev ^ MOUSE_MIDDLEBUTTON;
			else
				but |= prev & MOUSE_MIDDLEBUTTON;
			prev = but;
			break;
#else
		case MOUSE_MICROSOFT:	/* Microsoft */
#endif
		case MOUSE_LOGIMAN:	/* MouseMan / TrackMan */
		case MOUSE_PNP:
		default:
			but = (but & 0x0A) | ((buf[i] & 0x20) >> 3) | ((buf[i] & 0x10) >> 4);
			dx = (char) (((buf[i] & 0x03) << 6) | (buf[i + 1] & 0x3F));
			dy = (char) (((buf[i] & 0x0C) << 4) | (buf[i + 2] & 0x3F));
			break;
		case MOUSE_WACOM_GRAPHIRE: /* Wacom Graphire Tablet */
			if (!(buf[i] & 0x40)) { /* no device on tablet */
				nodev = 1;
				break;
			}
			but = (buf[i + 3] & 0x08) ? MOUSE_LEFTBUTTON : 0 |
			      (buf[i + 3] & 0x10) ? MOUSE_RIGHTBUTTON : 0 |
			      (buf[i + 3] & 0x20) ? MOUSE_MIDDLEBUTTON : 0;
			/* The absolute position is returned, not the change in position, so
			   we convert it. */
			ax = ((buf[i + 0] & 0x03) << 14) | (buf[i + 1] << 7) | buf[i + 2];
			ay = ((buf[i + 3] & 0x03) << 14) | (buf[i + 4] << 7) | buf[i + 5];
			if (nodev) {
				oldax = ax;
				olday = ay;
				nodev = 0;
			}
			dx = ax - oldax;
			dy = ay - olday;
			dz = (((buf[i+6] & 0x3f) << 1) | ((buf[i+3] & 0x04) >> 2));
			if (buf[i+6] & 0x40)
				dz = -dz;
			/* The tablet has *very* high resolution, so we normalize
			   that a bit. */
			dx /= 2;
			dy /= 2;
			oldax = ax;
			olday = ay;
			if (buf[i] & 0x20 && dz) /* stylus has pressure */
				but |= MOUSE_LEFTBUTTON;
			else if (buf[i+6] & 0x30) { /* roller is being turned */
				wheel = (buf[i + 6] & 0x30) >> 3;
				if (buf[i+6] & 0x40)
					wheel = -wheel;
#if defined(DEBUG_WHEEL)
				debugf(DSER, "MOUSE : Wheel turned (0x%02x)", wheel);
#endif
				/* RX-axis */
				if(ms.wheel_delta) {
					drx = ((wheel < 0) ? (-ms.wheel_delta) : ms.wheel_delta);
#if defined(DEBUG_WHEEL)
					debugf(DSER, "MOUSE : RX axis delta = %d", drx);
#endif
				}
			}
			break;
		case MOUSE_INTELLIMOUSE:    /* Serial IntelliMouse */
			/* This bit modified from gpm 1.13 */
			but = ((buf[i] & 0x20) >> 3) |		/* left */
			      ((buf[i + 3] & 0x10) >> 3) |	/* middle */
			      ((buf[i] & 0x10) >> 4);     /* right */
			dx = (char) (((buf[i] & 0x03) << 6) | (buf[i + 1] & 0x3F));
			dy = (char) (((buf[i] & 0x0C) << 4) | (buf[i + 2] & 0x3F));
			/* Did we turn the wheel? */
			if((wheel = buf[i + 3] & 0x0f) != 0) {
#if defined(DEBUG_WHEEL)
				debugf(DSER, "MOUSE : Wheel turned (0x%02x)", wheel);
#endif
				/* RX-axis */
				if(ms.wheel_delta) {
					drx = ((wheel > 7) ? (-ms.wheel_delta) : ms.wheel_delta);
#if defined(DEBUG_WHEEL)
					debugf(DSER, "MOUSE : RX axis delta = %d", drx);
#endif
				}
			}
			break;
		case MOUSE_MOUSESYSTEMS:	/* Mouse Systems Corp */
		case MOUSE_GPM:
			but = (~buf[i]) & 0x07;
			dx = (char) (buf[i + 1]);
			dx += (char) (buf[i + 3]);
			dy = -((char) (buf[i + 2]));
			dy -= (char) (buf[i + 4]);
			break;
		case MOUSE_MMSERIES:	/* MM Series */
		case MOUSE_LOGITECH:	/* Logitech */
			but = buf[i] & 0x07;
			dx = (buf[i] & 0x10) ? buf[i + 1] : -buf[i + 1];
			dy = (buf[i] & 0x08) ? -buf[i + 2] : buf[i + 2];
			break;
		case MOUSE_BUSMOUSE:	/* BusMouse */
			but = (~buf[i]) & 0x07;
			dx = (char) buf[i + 1];
			dy = -(char) buf[i + 2];
			break;
		case MOUSE_PS2:		/* PS/2 mouse */
			but = (buf[i] & 0x04) >> 1 |	/* Middle */
			      (buf[i] & 0x02) >> 1 |	/* Right */
			      (buf[i] & 0x01) << 2;	/* Left */
			dx = (buf[i] & 0x10) ? buf[i + 1] - 256 : buf[i + 1];
			dy = (buf[i] & 0x20) ? -(buf[i + 2] - 256) : -buf[i + 2];
			break;
		case MOUSE_DRMOUSE4DS:
			/* Digital Research 4-Axis mouse - like the PS/2 IntelliMouse, but
			   has two wheels.  */
			/* This bit modified from the gpm 1.13 imps2 patch by Tim Goodwin */
			but = ((buf[i] & 1) << 2) |	/* left */
			      ((buf[i] & 6) >> 1); /* middle and right */
			dx = (buf[i] & 0x10) ? buf[i + 1] - 256 : buf[i + 1];
			dy = (buf[i] & 0x20) ? -(buf[i + 2] - 256) : -buf[i + 2];
			/* Did we turn the wheel? */
			if((wheel = buf[i + 3]) != 0) {
#if defined(DEBUG_WHEEL)
				debugf(DSER, "MOUSE : Wheel turned (0x%02x)", wheel);
#endif
				/* RX reports as 1 or F, RY reports as 2 or E */
				/* both never report at the same time, which makes life easier */
				if (ms.wheel_delta) {
					switch (wheel & 0x0F) {
						case 1:		drx = ms.wheel_delta; break;
						case 2:		dry = -ms.wheel_delta; break;
						case 14:	dry = ms.wheel_delta; break;
						case 15:	drx = -ms.wheel_delta; break;
					}
#if defined(DEBUG_WHEEL)
					debugf(DSER, "MOUSE : RX axis delta = %d : RY axis delta = %d", drx, dry);
#endif
#if defined(DEBUG_WHEEL)
					debugf(DSER, "MOUSE : .");
#endif
				}
			}
			break;
		case MOUSE_IMPS2:           /* PS/2 IntelliMouse */
			/* This bit modified from the gpm 1.13 imps2 patch by Tim Goodwin */
			but = ((buf[i] & 1) << 2) |	/* left */
			      ((buf[i] & 6) >> 1); /* middle and right */
			dx = (buf[i] & 0x10) ? buf[i + 1] - 256 : buf[i + 1];
			dy = (buf[i] & 0x20) ? -(buf[i + 2] - 256) : -buf[i + 2];
			/* Did we turn the wheel? */
			if((wheel = buf[i + 3]) != 0) {
#if defined(DEBUG_WHEEL)
				debugf(DSER, "MOUSE : Wheel turned (0x%02x)", wheel);
#endif
				/* RX-axis */
				if(ms.wheel_delta) {
					drx = ((wheel > 0x7f) ? (-ms.wheel_delta) : ms.wheel_delta);
#if defined(DEBUG_WHEEL)
					debugf(DSER, "MOUSE : RX axis delta = %d", drx);
#endif
				}
#if defined(DEBUG_WHEEL)
				debugf(DSER, "MOUSE : .");
#endif
			}
			break;
		case MOUSE_EXPPS2:           /* PS/2 IntelliMouse Explorer */
			/* This bit modified from the gpm 1.13 imps2 patch by Tim Goodwin */
			but = ((buf[i] & 1) << 2) |	/* left */
			      ((buf[i] & 6) >> 1) |	/* middle and right */
			      ((buf[i+3] & 48) >> 1);
			dx = (buf[i] & 0x10) ? buf[i + 1] - 256 : buf[i + 1];
			dy = (buf[i] & 0x20) ? -(buf[i + 2] - 256) : -buf[i + 2];
			/* Did we turn the wheel? */
			if(((wheel = buf[i + 3]) & 15) != 0) {
#if defined(DEBUG_WHEEL)
				debugf(DSER, "MOUSE : Wheel turned (0x%02x)", wheel);
#endif
				/* RX-axis */
				if(ms.wheel_delta) {
					drx = ((wheel > 7) ? (-ms.wheel_delta) : ms.wheel_delta);
#if defined(DEBUG_WHEEL)
					debugf(DSER, "MOUSE : RX axis delta = %d", drx);
#endif
				}
#if defined(DEBUG_WHEEL)
				debugf(DSER, "MOUSE : .");
#endif
			}
			break;
		case MOUSE_SPACEBALL:
			switch (buf[i]) {
				case 'D':
					but = 0177 & buf[i+1];
					/* Strip the MSB, which is a parity bit */
					for (j = 2; j < 11; ++j) {
						buf[i+j] &= 0177;           /* Make sure everything is 7bit */
						buf[i+j] ^= SpaceWare[j-2]; /* What's this doing in the data? */
					}
					/* Turn chars into 10 bit integers */
					if (mouse_orientation == MOUSE_ORIENTATION_VERTICAL) {
						dx = ((buf[i + 2] & 0177) << 3) | ((buf[i + 3] & 0160) >> 4);
						dy = ((buf[i + 3] & 0017) << 6) | ((buf[i + 4] & 0176) >> 1);
						dz = ((buf[i + 4] & 0001) << 9) | ((buf[i + 5] & 0177) << 2)|
						     ((buf[i + 6] & 0140) >> 5);
						drx = ((buf[i + 6] & 0037) << 5) | ((buf[i + 7] & 0174) >> 2);
						dry = ((buf[i + 7] & 0003) << 8) | ((buf[i + 8] & 0177) << 1)|
						      ((buf[i + 9] & 0100) >> 6);
						drz = ((buf[i + 9] & 0077) << 4) | ((buf[i + 10] & 0170) >> 3);
					} else {
						dx = ((buf[i + 2] & 0177) << 3) | ((buf[i + 3] & 0160) >> 4);
						dz = ((buf[i + 3] & 0017) << 6) | ((buf[i + 4] & 0176) >> 1);
						dy = ((buf[i + 4] & 0001) << 9) | ((buf[i + 5] & 0177) << 2)|
						     ((buf[i + 6] & 0140) >> 5);
						drx = ((buf[i + 6] & 0037) << 5) | ((buf[i + 7] & 0174) >> 2);
						drz = ((buf[i + 7] & 0003) << 8) | ((buf[i + 8] & 0177) << 1)|
						      ((buf[i + 9] & 0100) >> 6);
						dry = ((buf[i + 9] & 0077) << 4) | ((buf[i + 10] & 0170) >> 3);
					}
					/* Get the sign right. */
					if (dx > 511) dx -= 1024;
					if (dy > 511) dy -= 1024;
					if (dz > 511) dz -= 1024;
					if (drx > 511) drx -= 1024;
					if (dry > 511) dry -= 1024;
					if (drz > 511) drz -= 1024;
					if (mouse_orientation == MOUSE_ORIENTATION_HORIZONTAL) {
						dz *= -1;
						drz *= -1;
					}
					/*    if (fabs(dx) < sorb_trans_thresh[1]) dx = 0; */
					i += 13;
#if defined(DEBUG_MOUSE)
					debugf(DSER, "MOUSE : Got D packet! but=%d, x=%d y=%d z=%d rx=%d ry=%d rz=%d",
						    but, dx, dy, dz, drx, dry, drz);
#endif
					break;
				case 'K':
					/* Button press/release w/out motion */
					but=0177 & buf[i+2];
					if (but == MOUSE_RESETBUTTON)
						mouse_orientation = 1 - mouse_orientation;
#if defined(DEBUG_MOUSE)
					debugf(DSER, "MOUSE : Got K packet! but=%d, x=%d y=%d z=%d rx=%d ry=%d rz=%d",
					            but, dx, dy, dz, drx, dry, drz);
#endif
					i += 6;
					break;
				case 'R':
#if defined(DEBUG_MOUSE)
					debugf(DSER, "MOUSE : Got init string!");
#endif
					for (j = i; ((buf[j] != 13) && (j < nu_bytes)); j++) {
#if defined(DEBUG_MOUSE)
						debugf(DSER, "MOUSE : %c",(buf[j] & 0177));
					}
					debugf(DSER, "MOUSE : ");
#else
						;

					}
#endif
					i = j + 1;
					break;
				default:
#if defined(DEBUG_MOUSE)
					debugf(DSER, "MOUSE : Got unknown packet!");
#endif
					i++;
					break;
			}
			break;
	}
	if (ms.type != MOUSE_SPACEBALL)
		i += mouse_proto[ms.type][4];
	/* Try to snag that optional mouseman fourth byte, if present */
	if ((ms.type == MOUSE_LOGIMAN) &&
	   ((nu_bytes - i) >= 1) &&
	   ((buf[i] & mouse_proto[ms.type][0]) != mouse_proto[ms.type][1]) &&
	   ((char) (buf[i] & ~0x23) == 0)) {
		/* Hack-o-matic, stolen from xf86_Mouse.c */
		but = ((buf[i] & 0x20) >> 4) | (but & 0x05);
		i++;
	}
	if (ms.accel.maxdelta) {
		if (abs(dx) > ms.accel.maxdelta)
			dx = (dx > 0) ? ms.accel.maxdelta : -ms.accel.maxdelta;
		if (abs(dy) > ms.accel.maxdelta)
			dy = (dy > 0) ? ms.accel.maxdelta : -ms.accel.maxdelta;
		if (abs(dz) > ms.accel.maxdelta)
			dz = (dz > 0) ? ms.accel.maxdelta : -ms.accel.maxdelta;
	}
	switch (ms.accel.type) {
		int delta;
		case MOUSE_ACCEL_TYPE_NORMAL:
#if defined(DEBUG_ACCEL)
			debugf(DSER, "MOUSE : %ld", (long) dx);
#endif
			if (abs(dx) > ms.accel.thresh) dx = (int) ((float) dx * ms.accel.mult);
			if (abs(dy) > ms.accel.thresh) dy = (int) ((float) dy * ms.accel.mult);
			if (abs(dz) > ms.accel.thresh) dz = (int) ((float) dz * ms.accel.mult);
#if defined(DEBUG_ACCEL)
			debugf(DSER, "MOUSE : %ld", (long) dx);
#endif
			break;
		case MOUSE_ACCEL_TYPE_POWER:
#if defined(DEBUG_ACCEL)
			debugf(DSER, "MOUSE : %ld", (long) dx);
#endif
			delta = abs(dx);
			dx = (delta >= ms.accel.thresh) ? (float) dx * ms.accel.mult : (float) dx * ms.accel.powertable[delta];
			delta = abs(dy);
			dy = (delta >= ms.accel.thresh) ? (float) dy * ms.accel.mult : (float) dy * ms.accel.powertable[delta];
			delta = abs(dz);
			dz = (delta >= ms.accel.thresh) ? (float) dz * ms.accel.mult : (float) dz * ms.accel.powertable[delta];
#if defined(DEBUG_ACCEL)
			debugf(DSER, "MOUSE : %ld", (long) dx);
#endif
			break;
	}
	if (ms.accel.amaxdelta  &&  ms.accel.type) {
		if (abs(dx) > ms.accel.amaxdelta)
			dx = (dx > 0) ? ms.accel.amaxdelta : -ms.accel.amaxdelta;
		if (abs(dy) > ms.accel.amaxdelta)
			dy = (dy > 0) ? ms.accel.amaxdelta : -ms.accel.amaxdelta;
		if (abs(dz) > ms.accel.amaxdelta)
			dz = (dz > 0) ? ms.accel.amaxdelta : -ms.accel.amaxdelta;
	}

	mouse_handler(but, dx, dy, dz, drx, dry, drz);

	event_handled = 1;

	goto handle_packets;
}

#endif /* VIDEO_HELPER_MOUSE */
