/***************************************************************************
    begin                : Thu Oct 9 2003
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

#if defined(VIDEO_HELPER_CONSOLE)

#include <unistd.h>
#include <signal.h>
#include <sys/vt.h>
#include <sys/kd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "../../../lib/xynth_.h"
#include "server.h"
#include "helper.h"

extern char * strsignal (int v);

static char s_video_helper_console_sig2catch[] = {
	SIGHUP, SIGINT, SIGQUIT, SIGILL,
	SIGTRAP, SIGIOT, SIGBUS, SIGFPE,
	SIGSEGV, SIGPIPE, SIGALRM, SIGTERM,
	SIGXCPU, SIGXFSZ, SIGVTALRM,
	/* SIGPROF ,*/ SIGPWR
};

static struct {
	int fd;
	int vc;
	int svc;
	int pid;

	int forbidvtrelease;
	int forbidvtacquire;

	struct vt_mode vtm;
	struct vt_stat vts;

	struct sigaction old_signal_handler[sizeof(s_video_helper_console_sig2catch)];
} s_video_helper_console = {-1, -1, -1, -1, 0, 0};


static void s_video_helper_console_waitvtactive (int vc);
static int s_video_helper_console_check_owner (int vc);
static void s_video_helper_console_open (void);
static void s_video_helper_console_restore (void);
static void s_video_helper_console_signal_handler (int v);
static void s_video_helper_console_releasevt_signal (int n);
static void s_video_helper_console_acquirevt_signal (int n);
static void s_video_helper_console_atexit (void);

static void s_video_helper_console_waitvtactive (int vc)
{
	if (s_video_helper_console.fd < 0) {
		return;
	}
	while (ioctl(s_video_helper_console.fd, VT_WAITACTIVE, vc) < 0) {
		if ((errno != EAGAIN) && (errno != EINTR)) {
			perror("ioctl(VT_WAITACTIVE)");
			exit(1);
		}
		usleep(20000);
	}
}

static int s_video_helper_console_check_owner (int vc)
{
	char fname[30];
	struct stat sbuf;

	sprintf(fname, "/dev/tty%d", vc);
	if ((stat(fname, &sbuf) >= 0) && (getuid() == sbuf.st_uid)) {
		return 1;
	}
	debugf(DSER, "You must be the owner of the current console to use");

	return 0;
}

static void s_video_helper_console_open (void)
{
	char fname[30];

	s_video_helper_console.pid = getpid();
	if (s_video_helper_console.fd >= 0) {
		return;
	}

	/*  The code below assumes file descriptors 0, 1, and 2
	 *  are already open; make sure that's true.  */
	if (fcntl(0, F_GETFD) < 0) { open("/dev/null", O_RDONLY); }
	if (fcntl(1, F_GETFD) < 0) { open("/dev/null", O_WRONLY); }
	if (fcntl(2, F_GETFD) < 0) { open("/dev/null", O_WRONLY); }

#if 0
	{
		struct stat sbuf;
	        struct vt_mode vtm;
		for (s_video_helper_console.fd = 0; s_video_helper_console.fd < 3; s_video_helper_console.fd++) {
			if (fstat(s_video_helper_console.fd, &sbuf) < 0) continue;
			if (ioctl(s_video_helper_console.fd, VT_GETMODE, &vtm) < 0) continue;
			if ((sbuf.st_rdev & 0xff00) != 0x400) continue;
			if (!(sbuf.st_rdev & 0xff)) continue;
			s_video_helper_console.vc = sbuf.st_rdev & 0xff;
			return;
		}
        }
#endif
        
	if ((s_video_helper_console.fd = open("/dev/console", O_RDWR)) < 0) {
		debugf(DSER | DSYS | DFAT, "Can't open /dev/console");
	}
	if (ioctl(s_video_helper_console.fd, VT_OPENQRY, &s_video_helper_console.vc) < 0) {
		goto error;
	}
	if (s_video_helper_console.vc <= 0) {
		goto error;
	}
	sprintf(fname, "/dev/tty%d", s_video_helper_console.vc);
	close(s_video_helper_console.fd);

	/* change our control terminal: */
	setpgid(0, getppid());
	setsid();

	/* We must use RDWR to allow for output... */
	if (((s_video_helper_console.fd = open(fname, O_RDWR)) >= 0) &&
	     (ioctl(s_video_helper_console.fd, VT_GETSTATE, &s_video_helper_console.vts) >= 0)) {
		if (!s_video_helper_console_check_owner(s_video_helper_console.vts.v_active)) {
			goto error;
		}
		/* success, redirect all stdios */
		debugf(DSER, "[allocated virtual console #%d]", s_video_helper_console.vc);
		fflush(stdin);
		fflush(stdout);
		fflush(stderr);

		close(0);
		close(1);
		close(2);

		dup(s_video_helper_console.fd);
		dup(s_video_helper_console.fd);
		dup(s_video_helper_console.fd);

		fwrite("\e[H\e[J", 6, 1, stderr);
		fflush(stderr);

		if (s_video_helper_console.vc != s_video_helper_console.vts.v_active) {
			s_video_helper_console.svc = s_video_helper_console.vts.v_active;
			ioctl(s_video_helper_console.fd, VT_ACTIVATE, s_video_helper_console.vc);
			s_video_helper_console_waitvtactive(s_video_helper_console.vc);
		}
	} else {
error:		if (s_video_helper_console.fd > 2) {
			close(s_video_helper_console.fd);
		}
		s_video_helper_console.fd = - 1;
		debugf(DSER | DSYS | DFAT, "Not running in a graphics capable console, and unable to find one");
	}
}

static void s_video_helper_console_restore (void)
{
	struct vt_stat vts_ = {0};

	if (s_video_helper_console.fd < 0) {
		return;
	}
	ioctl(s_video_helper_console.fd, VT_GETSTATE, &vts_);
	if (vts_.v_active != s_video_helper_console.vc) {
		ioctl(s_video_helper_console.fd, VT_ACTIVATE, s_video_helper_console.vc);
		s_video_helper_console_waitvtactive(s_video_helper_console.vc);
	}

	s_server_restore();
	
	ioctl(s_video_helper_console.fd, VT_SETMODE, &s_video_helper_console.vtm);
	ioctl(s_video_helper_console.fd, KDSETMODE, KD_TEXT);

	if (s_video_helper_console.svc != s_video_helper_console.vc) {
		ioctl(s_video_helper_console.fd, VT_ACTIVATE, s_video_helper_console.svc);
		s_video_helper_console_waitvtactive(s_video_helper_console.svc);
	}

	ioctl(s_video_helper_console.fd, VT_UNLOCKSWITCH);
	close(s_video_helper_console.fd);
	s_video_helper_console.fd = -1;
}


static void s_video_helper_console_signal_handler (int v)
{
	int i;

	s_video_helper_console_restore();

	debugf(DSER, "Signal %d: %s received %s", v, strsignal(v), (v == SIGINT) ? "(ctrl-alt-backspace) or (ctrl-c)" : "");

	for (i = 0; i < sizeof(s_video_helper_console_sig2catch); i++) {
		if (s_video_helper_console_sig2catch[i] == v) {
			sigaction(v, s_video_helper_console.old_signal_handler + i, NULL);
			raise(v);
			break;
		}
	}
	if (i >= sizeof(s_video_helper_console_sig2catch)) {
		debugf(DSER, "Illegal call to signal_handler, raising segfault");
		raise(SIGSEGV);
	}
}

static void s_video_helper_console_releasevt_signal (int n)
{
	s_video_helper_console.forbidvtacquire = 1;

	if (s_video_helper_console.forbidvtrelease) {
		s_video_helper_console.forbidvtacquire = 0;
		ioctl(s_video_helper_console.fd, VT_RELDISP, 0);
		return;
	}

	s_server_goto_back();

	ioctl(s_video_helper_console.fd, VT_RELDISP, 1);
	s_video_helper_console.forbidvtacquire = 0;

	/* Suspend program until switched to again. */
	s_video_helper_console_waitvtactive(s_video_helper_console.vc);
}

static void s_video_helper_console_acquirevt_signal (int n)
{
	s_video_helper_console.forbidvtrelease = 1;

	if (s_video_helper_console.forbidvtacquire) {
		s_video_helper_console.forbidvtrelease = 0;
		return;
	}
	s_video_helper_console_waitvtactive(s_video_helper_console.vc);

        s_server_comefrom_back();

	ioctl(s_video_helper_console.fd, VT_RELDISP, VT_ACKACQ);

	s_video_helper_console.forbidvtrelease = 0;
}

static void s_video_helper_console_atexit (void)
{
	if (s_video_helper_console.pid == getpid()) {
		s_video_helper_console_restore();
	}
}

int s_video_helper_console_init (void)
{
	int i;
	struct sigaction siga;
	struct vt_mode m;
	
	s_video_helper_console_open();
	if (s_video_helper_console.fd < 0) {
		return -1;
	}
	atexit(s_video_helper_console_atexit);

	memset(&m, 0, sizeof(struct vt_mode));
	m.mode = VT_PROCESS;
	m.waitv = 1;
	m.relsig = SIGUSR1;
	m.acqsig = SIGUSR2;

#define SETSIG(sa, sig, func) {\
	sa.sa_handler = func; \
	sa.sa_flags = SA_RESTART; \
	memset(&sa.sa_mask, 0, sizeof(sigset_t));\
	sigaction(sig, &sa, NULL); \
	}

	SETSIG(siga, SIGUSR1, s_video_helper_console_releasevt_signal);
	SETSIG(siga, SIGUSR2, s_video_helper_console_acquirevt_signal);

	ioctl(s_video_helper_console.fd, VT_SETMODE, &m);
	ioctl(s_video_helper_console.fd, VT_LOCKSWITCH);
	ioctl(s_video_helper_console.fd, KDSETMODE, KD_GRAPHICS);

	for (i = 0; i < sizeof(s_video_helper_console_sig2catch); i++) {
		siga.sa_handler = s_video_helper_console_signal_handler;
		siga.sa_flags = 0;
		memset(&siga.sa_mask, 0, sizeof(sigset_t));
		sigaction((int) s_video_helper_console_sig2catch[i], &siga, s_video_helper_console.old_signal_handler + i);
	}

	ioctl(s_video_helper_console.fd, VT_UNLOCKSWITCH);

	return s_video_helper_console.fd;
}

int s_video_helper_console_switch (int vt)
{
	if (vt != s_video_helper_console.vc) {
		ioctl(s_video_helper_console.fd, VT_ACTIVATE, vt);
	}
	return 0;
}

#endif /* VIDEO_HELPER_CONSOLE */
