/***************************************************************************
    begin                : Mon Nov 3 2003
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

#if defined(VIDEO_HELPER_KBD)

#include <signal.h>
#include <termios.h>
#include <linux/vt.h>
#include <linux/kd.h>
#include <linux/keyboard.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "../../../lib/xynth_.h"
#include "server.h"
#include "kbd.h"
#include "helper.h"

extern char * strsignal (int v);

static char s_video_helper_keybd_sig2catch[] = {
	SIGHUP, SIGINT, SIGQUIT, SIGILL,
	SIGTRAP, SIGIOT, SIGBUS, SIGFPE,
	SIGSEGV, SIGPIPE, SIGALRM, SIGTERM,
	SIGXCPU, SIGXFSZ, SIGVTALRM,
	/* SIGPROF ,*/ SIGPWR
};

static struct {
	int fd;
	int oldmode;
	int pid;
	struct termios oldtermios;
	struct termios newtermios;
	struct sigaction old_signal_handler[sizeof(s_video_helper_keybd_sig2catch)];
} s_video_helper_keybd = {-1, -1, -1};

static void s_video_helper_kbd_signal_handler (int v);
static void s_video_helper_kbd_atexit (void);
static void s_video_helper_kbd_keymap_init (void);

static void s_video_helper_kbd_signal_handler (int v)
{
	int i;

	s_video_helper_kbd_uninit();
	s_server_quit(server->window);

	debugf(DSER, "Signal %d: %s received %s", v, strsignal(v), (v == SIGINT) ? "(ctrl-alt-backspace or ctrl-c pressed)" : "");

	for (i = 0; i < sizeof(s_video_helper_keybd_sig2catch); i++) {
		if (s_video_helper_keybd_sig2catch[i] == v) {
			sigaction(v, s_video_helper_keybd.old_signal_handler + i, NULL);
			raise(v);
			break;
		}
	}
	if (i >= sizeof(s_video_helper_keybd_sig2catch)) {
		debugf(DSER, "Illegal call to signal_handler, raising segfault");
		raise(SIGSEGV);
	}
}

static void s_video_helper_kbd_atexit (void)
{
	if (s_video_helper_keybd.fd < 0) {
		return;
	}
	if (s_video_helper_keybd.pid == getpid()) {
		s_video_helper_kbd_uninit();
		s_server_quit(server->window);
	}
}

static void s_video_helper_kbd_keymap_init (void)
{
        int i;
        int map;
	struct kbentry entry = {0, 0, 0};

	for (map = 0; map < NUM_VGAKEYMAPS; ++map) {
		memset(s_video_helper_keybd_keymap[map], 0, NR_KEYS * sizeof(unsigned short));
		for (i = 0; i < NR_KEYS; ++i) {
			entry.kb_table = map;
			entry.kb_index = i;
			if (ioctl(s_video_helper_keybd.fd, KDGKBENT, &entry) == 0) {
				if (entry.kb_value == K_ENTER) {
					entry.kb_value = K(KT_ASCII,13);
				}
				if (KTYP(entry.kb_value) == KT_PAD) {
					switch (entry.kb_value) {
						case K_P0:
						case K_P1:
						case K_P2:
						case K_P3:
						case K_P4:
						case K_P5:
						case K_P6:
						case K_P7:
						case K_P8:
						case K_P9:
							s_video_helper_keybd_keymap[map][i] = entry.kb_value;
							s_video_helper_keybd_keymap[map][i] += '0';
							break;
						case K_PPLUS:
							s_video_helper_keybd_keymap[map][i] = K(KT_ASCII, '+');
							break;
						case K_PMINUS:
							s_video_helper_keybd_keymap[map][i] = K(KT_ASCII, '-');
							break;
						case K_PSTAR:
							s_video_helper_keybd_keymap[map][i] = K(KT_ASCII, '*');
							break;
						case K_PSLASH:
							s_video_helper_keybd_keymap[map][i] = K(KT_ASCII, '/');
							break;
						case K_PENTER:
							s_video_helper_keybd_keymap[map][i] = K(KT_ASCII, '\r');
							break;
						case K_PCOMMA:
							s_video_helper_keybd_keymap[map][i] = K(KT_ASCII, ',');
							break;
						case K_PDOT:
							s_video_helper_keybd_keymap[map][i] = K(KT_ASCII, '.');
							break;
						default:
							break;
					}
				}
				if ((KTYP(entry.kb_value) == KT_LATIN) ||
				    (KTYP(entry.kb_value) == KT_ASCII) ||
				    (KTYP(entry.kb_value) == KT_LETTER) ) {
					s_video_helper_keybd_keymap[map][i] = entry.kb_value;
				}
			}
		}
	}
}

void s_video_helper_kbd_set_attr (void)
{
        tcsetattr(s_video_helper_keybd.fd, TCSAFLUSH, &s_video_helper_keybd.newtermios);
}

int s_video_helper_kbd_init (s_server_conf_t *cfg, int kfd)
{
	int i;
	struct sigaction siga;
	
	s_video_helper_keybd.fd = kfd;
	if (s_video_helper_keybd.fd < 0) {
		return -1;
	}

	if (ioctl(s_video_helper_keybd.fd, KDGKBMODE, &s_video_helper_keybd.oldmode)) {
		debugf(DSER | DSYS | DFAT, "Cannot get keyboard mode");
	}

	tcgetattr(s_video_helper_keybd.fd, &s_video_helper_keybd.oldtermios);
	s_video_helper_keybd.newtermios = s_video_helper_keybd.oldtermios;

	s_video_helper_keybd.newtermios.c_lflag &= ~(ICANON | ECHO | ISIG);
	s_video_helper_keybd.newtermios.c_iflag &= ~(ISTRIP | IGNCR | ICRNL | INLCR | IXOFF | IXON);
	s_video_helper_keybd.newtermios.c_cc[VMIN] = 0;
	s_video_helper_keybd.newtermios.c_cc[VTIME] = 0;

	s_video_helper_keybd.pid = getpid();
	atexit(s_video_helper_kbd_atexit);

        s_video_helper_kbd_set_attr();

        ioctl(s_video_helper_keybd.fd, KDSKBMODE, K_MEDIUMRAW);

	for (i = 0; i < sizeof(s_video_helper_keybd_sig2catch); i++) {
		siga.sa_handler = s_video_helper_kbd_signal_handler;
		siga.sa_flags = 0;
		memset(&siga.sa_mask, 0, sizeof(sigset_t));
		sigaction((int) s_video_helper_keybd_sig2catch[i], &siga, s_video_helper_keybd.old_signal_handler + i);
	}

	s_video_helper_kbd_keymap_init();

	return s_video_helper_keybd.fd;
}

int s_video_helper_kbd_update (s_video_input_data_t *keybd)
{
	int i;
	int led = 0;
	int pressed;
	int scancode;
	int bytesread;
	unsigned char buf;

	int map = 0;
	S_KEYCODE_FLAG keycode_flag;
	
	bytesread = read(s_video_helper_keybd.fd, &buf, 1);

	for (i = 0; i < bytesread; i++) {
		scancode = buf & 0x7f;
		pressed = (buf & 0x80) ? KEYBD_RELEASED : KEYBD_PRESSED;

                keybd->keybd.state = pressed;
		keybd->keybd.scancode = scancode;
		keybd->keybd.button = s_video_helper_keybd_keycode_[scancode][KEYCODE_PLAIN];
		keybd->keybd.keycode = s_video_helper_keybd_keycode_[scancode][KEYCODE_PLAIN];

		keycode_flag = server->window->event->keybd->flag;
		switch (keybd->keybd.button) {
			case S_KEYCODE_LEFTSHIFT:
			case S_KEYCODE_RIGHTSHIFT:
			case S_KEYCODE_LEFTCONTROL:
			case S_KEYCODE_RIGHTCONTROL:
			case S_KEYCODE_ALT:
			case S_KEYCODE_ALTGR:
			case S_KEYCODE_CAPS_LOCK:
				goto keycode_plain;
			default:
				break;
		}
		if (keycode_flag & KEYCODE_SHIFTF) {
			keybd->keybd.button = s_video_helper_keybd_keycode_[scancode][KEYCODE_SHIFT];
			map |= (1 << KG_SHIFT);
		}
		if (keycode_flag & KEYCODE_CTRLF) {
			map |= (1 << KG_CTRL);
		}
		if (keycode_flag & KEYCODE_LALTF) {
			map |= (1 << KG_ALT);
		}
		if (keycode_flag & KEYCODE_ALTGRF) {
			keybd->keybd.button = s_video_helper_keybd_keycode_[scancode][KEYCODE_ALTGR];
			map |= (1 << KG_ALTGR);
		}
keycode_plain:
		if (KTYP(s_video_helper_keybd_keymap[map][scancode]) == KT_LETTER) {
			if (keycode_flag & KEYCODE_CPLCKF) {
				map ^= (1 << KG_SHIFT);
			}
		}
		if (KTYP(s_video_helper_keybd_keymap[map][scancode]) == KT_PAD) {
			if (keycode_flag & KEYCODE_NMLCKF) {
				keybd->keybd.ascii = KVAL(s_video_helper_keybd_keymap[map][scancode]);
			}
		} else {
			keybd->keybd.ascii = KVAL(s_video_helper_keybd_keymap[map][scancode]);
		}

		ioctl(s_video_helper_keybd.fd, KDGETLED, &led);
		if (keycode_flag & KEYCODE_CPLCKF) {
			led |= LED_CAP;
		} else {
			led &= ~LED_CAP;
		}
		if (keycode_flag & KEYCODE_NMLCKF) {
			led |= LED_NUM;
		} else {
			led &= ~LED_NUM;
		}
		ioctl(s_video_helper_keybd.fd, KDSETLED, led);
	}
	return 0;
}

void s_video_helper_kbd_uninit (void)
{
	if (s_video_helper_keybd.fd < 0) {
		return;
	}

        ioctl(s_video_helper_keybd.fd, KDSKBMODE, s_video_helper_keybd.oldmode);
        tcsetattr(s_video_helper_keybd.fd, 0, &s_video_helper_keybd.oldtermios);

	s_video_helper_keybd.fd = -1;
}

#endif /* VIDEO_HELPER_KBD */
