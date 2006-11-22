/***************************************************************************
    begin                : Wed Nov 22 2006
    copyright            : (C) 2006 by Alper Akcan
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

#if defined(VIDEO_HELPER_IRSERIAL)

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define IR_CODE_LEN 255
static int irserial_fd = -1;
unsigned int irserial_time;
static char irserial_buf[IR_CODE_LEN];
static struct termios irserial_oldtio;
static struct termios irserial_newtio;

int irserial_init (char *dev, unsigned int brate)
{
        irserial_fd = open(dev, O_RDWR | O_NOCTTY ); 
        if (irserial_fd < 0) {
        	debugf(DSER, "could not open file %s\n", dev);
        	return irserial_fd;
        }
        tcgetattr(irserial_fd, &irserial_oldtio);
        memset(&irserial_newtio, 0, sizeof(irserial_newtio));
        irserial_newtio.c_cflag = brate | CRTSCTS | CS8 | CLOCAL | CREAD;
        irserial_newtio.c_iflag = IGNPAR;
        irserial_newtio.c_oflag = 0;
        irserial_newtio.c_lflag = 0;
        irserial_newtio.c_cc[VTIME] = 0;
        irserial_newtio.c_cc[VMIN] = 5;
        tcflush(irserial_fd, TCIFLUSH);
        tcsetattr(irserial_fd, TCSANOW, &irserial_newtio);
	return irserial_fd;
}

int irserial_uninit (void)
{
        tcsetattr(irserial_fd, TCSANOW, &irserial_oldtio);
        close(irserial_fd);
	return 0;
}

char * irserial_getcode (void)
{
	int len;
	unsigned int t;
	len = read(irserial_fd, irserial_buf, IR_CODE_LEN - 1);
	if (len > 0) {
		t = time(NULL);
		if (irserial_time < t) {
			irserial_time = t;
			return irserial_buf;
		}
	}
	return NULL;
}

#endif
