/***************************************************************************
    begin                : Thu Jan 30 2003
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

#if defined(DEMO_TERM)

#include <fcntl.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#include "xynth_.h"

static pid_t pid;
static char *ptydev;
static int slave;
static int cmd_fd;
static struct passwd *pw;

static s_font_t *font;

static int cursorx = 1;
static int cursory = 1;
static int scursorx = 1;
static int scursory = 1;

static int colors[9] = {0x000000, /* black   */
		        0xB41818, /* red     */
		        0x18B218, /* green   */
		        0xFFFF52, /* yellow  */
		        0x1818B4, /* blue    */
		        0xFF5552, /* magenta */
		        0x18B2B4, /* cyan    */
		        0xB4B2B4, /* white   */
		        0x18F218};/* default */

static int charset = 0;
static int charset_G[2] = {0, 0};

static struct {
	int wraparound;
	int application;
} mode = {0, 0};

static struct {
	int bold;
	int fg_color;
	int bg_color;
} chr_attr;

#define FONTW	10
#define FONTH	16
#define SCREENW	80
#define SCREENH	25

static struct {
	int y0;
	int y1;
} scroll_reg = {1, SCREENH};

extern int getpt (void);
extern int grantpt (int);
extern int unlockpt (int);

#define CTRLCODES	51
static S_KEYCODE_CODE ctrlcodes_c[CTRLCODES] = {S_KEYCODE_a, S_KEYCODE_A, S_KEYCODE_b, S_KEYCODE_B, S_KEYCODE_c, S_KEYCODE_C,
						S_KEYCODE_d, S_KEYCODE_D, S_KEYCODE_e, S_KEYCODE_E, S_KEYCODE_f, S_KEYCODE_F,
						S_KEYCODE_g, S_KEYCODE_G, S_KEYCODE_h, S_KEYCODE_H, S_KEYCODE_k, S_KEYCODE_K,
						S_KEYCODE_l, S_KEYCODE_L, S_KEYCODE_n, S_KEYCODE_N, S_KEYCODE_o, S_KEYCODE_O,
						S_KEYCODE_p, S_KEYCODE_P, S_KEYCODE_q, S_KEYCODE_Q, S_KEYCODE_r, S_KEYCODE_R,
						S_KEYCODE_s, S_KEYCODE_S, S_KEYCODE_t, S_KEYCODE_T, S_KEYCODE_u, S_KEYCODE_U,
						S_KEYCODE_v, S_KEYCODE_V, S_KEYCODE_w, S_KEYCODE_W, S_KEYCODE_x, S_KEYCODE_X,
						S_KEYCODE_y, S_KEYCODE_Y, S_KEYCODE_z, S_KEYCODE_Z,
						S_KEYCODE_BRACKETLEFT, S_KEYCODE_BACKSLASH, S_KEYCODE_BRACKETRIGHT,
						S_KEYCODE_ASCIICIRCUM, S_KEYCODE_UNDERSCORE};
static char *ctrlcodes_s[CTRLCODES] = {"\001", "\001", "\002", "\002", "\003", "\003",
				       "\004", "\004", "\005", "\005", "\006", "\006",
				       "\007", "\007", "\010", "\010", "\013", "\013",
				       "\014", "\014", "\016", "\016", "\017", "\017",
				       "\020", "\020", "\021", "\021", "\022", "\022",
				       "\023", "\023", "\024", "\024", "\025", "\025",
				       "\026", "\026", "\027", "\027", "\030", "\030",
				       "\031", "\031", "\032", "\032",
				       "\033", "\034", "\035",
				       "\036", "\037"};

#define KEYCODES	27
static S_KEYCODE_CODE keycodes_c[KEYCODES] = {S_KEYCODE_F1, S_KEYCODE_F2, S_KEYCODE_F3, S_KEYCODE_F4, S_KEYCODE_F5,
					      S_KEYCODE_F6, S_KEYCODE_F7, S_KEYCODE_F8, S_KEYCODE_F9, S_KEYCODE_F10,
                                              S_KEYCODE_F11, S_KEYCODE_F12, S_KEYCODE_TAB, S_KEYCODE_END, S_KEYCODE_HOME,
                                              S_KEYCODE_UP, S_KEYCODE_DOWN, S_KEYCODE_LEFT, S_KEYCODE_RIGHT, S_KEYCODE_RETURN,
                                              S_KEYCODE_DELETE, S_KEYCODE_INSERT, S_KEYCODE_REMOVE, S_KEYCODE_ESCAPE,
                                              S_KEYCODE_PAGEUP, S_KEYCODE_PAGEDOWN, S_KEYCODE_KP_ENTER};
static char *keycodes_s[KEYCODES] = {"\033[[A", "\033[[B", "\033[[C", "\033[[D", "\033[[E", "\033[17~", "\033[18~",
				     "\033[19~", "\033[20~", "\033[21~", "\033[23~", "\033[24~", "\t", "\033[4~",
				     "\033[1~", "\033[A", "\033[B", "\033[D", "\033[C", "\n", "\010", "\033[2~",
				     "\033[3~", "\033", "\033[5~", "\033[6~", "\n"};

static int tabs_horizontal[SCREENH][SCREENW];

static void sigchld_handler (int a)
{
	int status = 0;

	if (waitpid(pid, &status, 0) < 0) {
		printf("Waiting for pid %hd failed\n", pid);
		exit(1);
	}
	if (WIFEXITED(status)) {
		if (WEXITSTATUS(status)) {
			printf("Process %hd exited with non-zero status %d\n", pid, WEXITSTATUS(status));
			exit(1);
		}
	} else if (WIFSIGNALED(status)) {
		printf("Process %hd was killed by signal %d\n", pid, WTERMSIG(status));
		exit(1);
	} else {
		printf("Process %hd terminated abnormally\n", pid);
		exit(1);
	}

	printf("Child exited normally\n");
	exit(0);
}

static void get_pty (void)
{
        int fd;
	extern char *ptsname();

	if ((fd = getpt()) >= 0) {
		if ((grantpt(fd) == 0) && (unlockpt(fd) == 0)) {
			ptydev = ptsname(fd);
			if ((slave = open(ptydev, O_RDWR | O_NOCTTY)) < 0) {
				printf("Error opening slave pty\n");
				exit(2);
			}
			cmd_fd = fd;
			return;
		}
		close(fd);
	}
	printf("Can not open pseudo-tty\n");
	exit(2);
}

static void get_tty (void)
{
	int i;
	struct winsize w;

	for (i = 0; i < 100 ; i++) {
		if (i != slave) {
			close(i);
		}
	}

	cmd_fd = slave;
	setsid(); /* create a new process group */

	dup2(cmd_fd, 0);
	dup2(cmd_fd, 1);
	dup2(cmd_fd, 2);

	if (ioctl(cmd_fd, TIOCSCTTY, NULL) < 0) {
		printf("Couldn't set controlling terminal\n");
		exit(2);
	}
	w.ws_row = SCREENH;
	w.ws_col = SCREENW;
	w.ws_xpixel = 0;
	w.ws_ypixel = 0;
	if (ioctl(cmd_fd, TIOCSWINSZ, &w) < 0) {
		printf("Couldn't set window size\n");
		exit(2);
	}
}

static char get_char (int fd)
{
        int r;
        int f;
	char c = 0;
	
	r = read(fd, &c, 1);
	if (r <= 0) {
		fcntl(fd, F_GETFL, &f);
		if (f & O_NONBLOCK) {
			printf("Could not read input fd\n");
			exit(2);
		}
	}

	return c;
}

static void cursor_up (int n)
{
	if (n == 0) {
		n = 1;
	}
	cursory -= n;
	if (cursory < 1) {
		cursory = 1;
	}
}

static void cursor_down (int n)
{
	if (n == 0) {
		n = 1;
	}
	cursory += n;
	if (cursory > SCREENH) {
		cursory = SCREENH;
	}
}

static void cursor_left (int n)
{
	if (n == 0) {
		n = 1;
	}
	cursorx -= n;
	if (cursorx < 1) {
		cursorx = 1;
	}
}

static void cursor_right (int n)
{
	if (n == 0) {
		n = 1;
	}
	cursorx += n;
	if (cursorx > SCREENW) {
		cursorx = SCREENW;
	}
}

static void cursor_goto (int x, int y)
{
	cursorx = x;
	if (cursorx < 1) {
		cursorx = 1;
	}
	if (cursorx > SCREENW) {
		cursorx = SCREENW;
	}

	cursory = y;
	if (cursory < 1) {
		cursory = 1;
	}
	if (cursory > SCREENH) {
		cursory = SCREENH;
	}
}

static void erase_in_display (s_window_t *window, int v)
{
        int x = 0;
        int y = 0;
        int w = 0;
        int h = 0;

	switch (v) {
		case 0: /* Erase from line active position to the end of screen */
			x = (cursorx - 1) * FONTW;
			y = (cursory - 1) * FONTH;
			w = FONTW * SCREENW - x;
			h = FONTH * SCREENH - y;
			break;
		case 1: /* Erase from start position to the active position */
			x = 0;
			y = 0;
			w = (cursorx - 1) * FONTW;
			h = (cursory - 1) * FONTH;
			break;
		case 2: /* Erase all of the display -- all lines erased, changed to single width, and the cursor does not move */
			x = 0;
			y = 0;
			w = FONTW * SCREENW;
			h = FONTH * SCREENH;
			break;
	}
	s_fillbox(window->surface, x, y, w, h, chr_attr.bg_color);
}

static void erase_in_line (s_window_t *window, int v)
{
        int x = 0;
        int y = 0;
        int w = 0;
        int h = 0;

	switch (v) {
		case 0: /* Erase from line active position to the end of line */
			x = (cursorx - 1) * FONTW;
			y = (cursory - 1) * FONTH;
			w = FONTW * SCREENW - x;
			h = FONTH;
			break;
		case 1: /* Erase from start position to the active position */
			x = 0;
			y = (cursory - 1) * FONTH;
			w = (cursorx - 1) * FONTW;
			h = FONTH;
			break;
		case 2: /* Erase all of the line */
			x = 0;
			y = (cursory - 1) * FONTH;
			w = FONTW * SCREENW;
			h = FONTH;
			break;
	}
	s_fillbox(window->surface, x, y, w, h, chr_attr.bg_color);
}

static void erase_chars (s_window_t *window, int c)
{
        int x = 0;
        int y = 0;
        int w = 0;
        int h = 0;
        char *box;
        
        box = (char *) s_malloc(SCREENW * FONTW * SCREENH * FONTH * window->surface->bytesperpixel + 1);

        /* Erase [c] character(s) from line active position.*/

	if (c == 0) {
		c = 1;
	}

	x = (cursorx - 1) * FONTW;
	y = (cursory - 1) * FONTH;
	w = FONTW * c;
	h = FONTH;

	s_getbox(window->surface, x + w, y, (FONTW * SCREENW) - x - w, h, box);
	erase_in_line(window, 0);
	s_putbox(window->surface, x, y, (FONTW * SCREENW) - x - w, h, box);
	
	s_free(box);
}

static void erase_chars_ (s_window_t *window, int c)
{
        int x = 0;
        int y = 0;
        int w = 0;
        int h = 0;

        /* Erase [c] character(s) from line active position.*/

	if (c == 0) {
		c = 1;
	}

	x = (cursorx - 1) * FONTW;
	y = (cursory - 1) * FONTH;
	w = FONTW * c;
	h = FONTH;

	s_fillbox(window->surface, x, y, w, h, chr_attr.bg_color);
}

static void device_attributes (int fd)
{
	/*
	   No options			"\033[?1;0c"
	   Processor option (STP)	"\033[?1;1c"
	   Advanced video option (AVO)	"\033[?1;2c"
	   AVO and STP			"\033[?1;3c"
	   Graphics option (GO)		"\033[?1;4c"
	   GO and STP			"\033[?1;5c"
	   GO and AVO			"\033[?1;6c"
	   GO, STP, and AVO		"\033[?1;7c"
	*/
	char *chr = "\033[?1;2c";
	write(fd, chr, strlen(chr));
}

static void modes_set (int set)
{
	switch (set) {
		case 7:
			mode.wraparound = 1;
			return;
	}
	printf("Modes Set %d\n", set);
}

static void modes_reset (int reset)
{
	switch (reset) {
		case 7:
			mode.wraparound = 0;
			return;
	}
	printf("Modes Reset %d\n", reset);
}

static void chr_attr_fg_color (s_window_t *window, int c)
{
	chr_attr.fg_color = colors[c];
	chr_attr.fg_color = s_rgbcolor(window->surface, (colors[c] >> 16) & 0xFF, (colors[c] >> 8) & 0xFF, (colors[c] >> 0) & 0xFF);
}

static void chr_attr_bg_color (s_window_t *window, int c)
{
	chr_attr.bg_color = colors[c];
	chr_attr.bg_color = s_rgbcolor(window->surface, (colors[c] >> 16) & 0xFF, (colors[c] >> 8) & 0xFF, (colors[c] >> 0) & 0xFF);
}

static void chr_attr_bold (int b)
{
	switch (b) {
		case 0:
//			s_font_set(&font, FONTR);
			break;
		case 1:
//			s_font_set(&font, FONTB);
			break;
	}
}

static void chr_attr_reset (s_window_t *window)
{
        chr_attr_bold(0);
	chr_attr_fg_color(window, 8);
}

static void set_charset (int g, unsigned char which)
{
	switch (which) {
		case '0': /* graphics  */
			charset_G[g] = 1;
			break;
		case 'B': /* iso8859-1 */
			charset_G[g] = 0;
			break;
		default:
			printf("Charset unknown %c\n", which);
	}
}

static void insert_lines (s_window_t *window, int lines)
{
	int i = lines;
	char *box;

	box = (char *) s_malloc(FONTW * SCREENW * (scroll_reg.y1 - scroll_reg.y0) * FONTH * window->surface->bytesperpixel + 1);

        while (i--) {
		s_getbox(window->surface, 0, (scroll_reg.y0 - 1) * FONTH, FONTW * SCREENW, (scroll_reg.y1 - scroll_reg.y0) * FONTH, box);
		s_fillbox(window->surface, 0, (scroll_reg.y0 - 1) * FONTH, FONTW * SCREENW, FONTH, chr_attr.bg_color);
		s_putbox(window->surface, 0, (scroll_reg.y0) * FONTH, FONTW * SCREENW, (scroll_reg.y1 - scroll_reg.y0) * FONTH, box);
	}

	s_free(box);
}

static void delete_lines (s_window_t *window, int lines)
{
        int i = lines;
	char *box;

	box = (char *) s_malloc(FONTW * SCREENW * (scroll_reg.y1 - scroll_reg.y0) * FONTH * window->surface->bytesperpixel + 1);

        while (i--) {
		s_getbox(window->surface, 0, (scroll_reg.y0) * FONTH, FONTW * SCREENW, (scroll_reg.y1 - scroll_reg.y0) * FONTH, box);
		s_fillbox(window->surface, 0, (scroll_reg.y1 - 1) * FONTH, FONTW * SCREENW, FONTH, chr_attr.bg_color);
		s_putbox(window->surface, 0, (scroll_reg.y0 - 1) * FONTH, FONTW * SCREENW, (scroll_reg.y1 - scroll_reg.y0) * FONTH, box);
	}

	s_free(box);
}

static void scroll_region (int y0, int y1)
{
	if (y0 <= 0) {
		scroll_reg.y0 = 1;
	} else {
		scroll_reg.y0 = y0;
	}
	if (y1 <= 0) {
		scroll_reg.y1 = SCREENH;
	} else {
		scroll_reg.y1 = y1;
	}
}

static void tabulation_clear (int g)
{
        int x = cursorx - 1;
        int y = cursory - 1;

	switch (g) {
		case 0: /* Clear horizontal tab stop at current position */
			tabs_horizontal[y][x] = 0;
			break;
		case 1: /* Clear vertical tab stop at current line */
		case 2: /* Clear all horizontal tab stops at current line only */
			break;
		case 3: /* Clear all tab stops in the terminal */
			for (y = 0; y < SCREENH; y++) {
				for (x = 0; x < SCREENW; x++) {
					tabs_horizontal[y][x] = 0;
				}
			}
			break;
	}
}

static void tabulation_set_current (void)
{
        int x = cursorx - 1;
        int y = cursory - 1;

	tabs_horizontal[y][x] = 1;
}

static void xterm_sequence (s_window_t *window, int fd, int op)
{
	int len;
	int buflen;
	unsigned char c;
	unsigned char *buf;
	unsigned char *buf_;

	buflen = 20;
	buf = (unsigned char *) s_malloc(sizeof(unsigned char) * buflen);

	c = get_char(fd);
	for (len = 0; c != '\007'; len++) {
		if (len > buflen) {
			buflen <<= 1;
			buf = (unsigned char *) s_realloc(buf, buflen);
		}
		buf[len] = c;
		c = get_char(fd);
	}
	buf[++len] = c;

 	printf("Xterm Sequence %d, arg: %s\n", op, buf);

	switch (op) {
		case 2:
			buf_ = (unsigned char *) s_malloc(strlen((char *) buf) + strlen("Xyntherminal - "));
			sprintf((char *) buf_, "Xyntherminal - %s", buf);
			s_window_set_title(window, (char *) buf_);
			s_free(buf_);
			break;
	}

	s_free(buf);
}

static void write_char (s_window_t *window, unsigned chr)
{
        int x;
        int y;
        char *vbuf;
	char str[2];
        s_surface_t s;
        unsigned char c;
        unsigned int *mat;

	c = (unsigned char) chr;
	if (charset_G[charset]) {
		if (c == 0x5f) {
			chr = 0x7f;
		} else if ((c > 0x5f) && (c < 0x7f)) {
			chr = c - 0x5f;
		} else {
			chr = c;
		}
	} else {
		chr = c;
	}
	str[0] = chr;
	str[1] = '\0';
	font->str = str;

	s_font_get_glyph(font);
	mat = font->img->rgba;

        vbuf = (char *) s_malloc(window->surface->bytesperpixel * FONTW * FONTH + 1);
        s_getsurfacevirtual(&s, FONTW, FONTH, window->surface->bitsperpixel, vbuf);

	s_fillbox(&s, 0, 0, FONTW, FONTH, chr_attr.bg_color);

	for (y = 0; y < font->img->h; y++) {
		for (x = 0; x < font->img->w; x++) {
			if (~*mat & 0xff) {
				int r;
				int g;
				int b;
				s_colorrgb(&s, chr_attr.fg_color, &r, &g, &b);
				s_setpixelrgba(&s, x, y + FONTH - font->yMax - 4, r, g, b, *mat & 0xff);
			} else {
				s_setpixel(&s, x, y + FONTH - font->yMax - 4, chr_attr.bg_color);
			}
			mat++;
		}
	}
        s_putbox(window->surface, (cursorx - 1) * FONTW, (cursory - 1) * FONTH, FONTW, FONTH, s.vbuf);

	cursorx++;
        s_free(vbuf);

        font->str = NULL;
	s_image_uninit(font->img);
	s_image_init(&(font->img));
}

static void pfd_in_escape (s_window_t *window, int fd)
{
        int pos;
        int digit;
        int question;
        int carriage;
        int backspace;
        int verticaltab;
        int narg = 0;
	int args[16];
        unsigned char chr;

	for (pos = 0; pos < 16; pos++) {
		args[pos] = 0;
	}

	chr = get_char(fd);

	switch (chr) {
		case '[': /* CSI */
			narg = 0;
			digit = 0;
			question = 0;
			carriage = 0;
			backspace = 0;
			verticaltab = 0;
			chr = get_char(fd);
			while (isdigit(chr) || (chr == ';') || (chr == '?') ||
			       (chr == '\010') || (chr == '\013') || (chr == '\015')) {
				if (chr == ';') {
					args[narg] = 0;
					digit = 0;
				} else {
					if (chr == '\010') {
						backspace++;
						if (!digit) {
							while (backspace) {
								cursorx--;
								backspace--;
							}
						}
					} else if (chr == '\015') {
						carriage++;
						if (!digit) {
							while (carriage) {
								carriage--;
								cursorx = 1;
							}
						}
					} else if (chr == '\013') {
						verticaltab++;
						if (!digit) {
							while (verticaltab) {
								cursory++;
								verticaltab--;
							}
						}
					} else if (chr == '?') {
						question = 1;
					} else {
						if (!digit) {
							narg++;
						}
						digit = 1;
						args[narg - 1] *= 10;
						args[narg - 1] += (chr - '0');
					}
				}
				chr = get_char(fd);
			}
			switch (chr) {
				case 'A': /* CUU Cursor Up */
					cursor_up(args[0]);
					break;
				case 'B': /* CUD Cursor Down */
					cursor_down(args[0]);
					break;
				case 'C': /* CUF Cursor Forward Right */
					cursor_right(args[0]);
					break;
				case 'D': /* CUB Cursor Backward Left */
					cursor_left(args[0]);
					break;
				case 'G': /* Cursor Move Col */
					cursor_goto(args[0], cursory);
					break;
				case 'H': /* CUP Direct Cursor Addressing */
				case 'f':
					cursor_goto(args[1], args[0]);
					break;
				case 'J': /* ED Erase In Display */
					erase_in_display(window, args[0]);
					break;
				case 'P': /* DCH Delete Character */
					erase_chars(window, args[0]);
					break;
				case 'K': /* ED Erase In Line */
					if (narg == 0) {
						break;
					}
					erase_in_line(window, args[0]);
					break;
				case 'L': /* Insert Line */
					insert_lines(window, args[0]);
					break;
				case 'M': /* Delete Line */
					delete_lines(window, args[0]);
					break;
				case 'X': /* Erase Chars */
					erase_chars_(window, args[0]);
					break;
				case 'c': /* DA Device Attributes */
					if (narg == 0) {
						device_attributes(fd);
					}
					break;
				case 'd': /* Cursor Move Row */
					cursor_goto(cursorx, args[0]);
					break;
				case 'g': /* TBC Tabulation clear */
					tabulation_clear(args[0]);
					break;
				case 'h': /* Modes Set */
					pos = narg;
					while (pos > 0) {
						modes_set(args[pos - 1]);
						pos--;
					}
					break;
				case 'l': /* Modes Reset */
					pos = narg;
					while (pos > 0) {
						modes_reset(args[pos - 1]);
						pos--;
					}
					break;
				case 'm': /* SGR Set Graphics Rendition */
					if (narg == 0) {
						chr_attr_reset(window);
					}
					for (pos = 0; pos < narg; pos++) {
						switch (args[pos]) {
							case 0:
								chr_attr_reset(window);
								break;
							case 1:
								chr_attr_bold(1);
								break;
							case 4: /* underscode */
							case 5: /* blinking */
							case 7: /* display negative image */
							case 22: /* display normal intensity */
							case 24: /* not underlined */
							case 25: /* not blinking */
							case 27: /* display positive image */
								break;
							case 30 ... 37:
								chr_attr_fg_color(window, args[pos] - 30);
								break;
							case 39:
								chr_attr_fg_color(window, 8);
								break;
							case 40 ... 47:
								chr_attr_bg_color(window, args[pos] - 40);
								break;
							case 49:
								chr_attr_bg_color(window, 0);
								break;
//							default:
//								printf("m args : %d\n", args[pos]);
						}
					}
					break;
				case 'r': /* Scroll Region */
					scroll_region(args[0], args[1]);
					break;
				default:
					printf("Unknown CSI %c", chr);
					if (narg) {
						printf(" args :");
						for (pos = 0; pos < narg; pos++) {
							printf(" %d", args[pos]);
						}
					}
					printf("\n");
			}
			while (backspace) {
				cursorx--;
				backspace--;
			}
			while (carriage) {
				carriage--;
				cursorx = 1;
			}
			while (verticaltab) {
				cursory++;
				verticaltab--;
			}
			break;
		case '(': /* G0 */
			set_charset(0, get_char(fd));
			break;
		case ')': /* G1 */
			set_charset(1, get_char(fd));
			break;
		case '=': /* Set application keypad mode */
			mode.application = 1;
			break;
		case '>': /* Set numeric keypad mode */
			mode.application = 0;
			break;
		case 'M': /* Reverse Index */
			cursory--;
			if (cursory < 1) {
				insert_lines(window, 1);
				cursory = 1;
			}
			break;
		case 'E': /* Next Line */
			cursorx = 1;
		case 'D': /* Index */
			cursory++;
			if (cursory > SCREENH) {
				delete_lines(window, 1);
				cursory = SCREENH;
			}
			break;
		case 'H': /* HTS Horizontal tabulation set at current position */
			tabulation_set_current();
			break;
		case '7': /* Save cursor position */
			scursorx = cursorx;
			scursory = cursory;
			break;
		case '8': /* Restore cursor position */
			cursorx = scursorx;
			cursory = scursory;
			break;
		case ']': /* Xterm Sequence */
			digit = 0;
			chr = get_char(fd);
			while (isdigit(chr)) {
				if (!digit) {
					narg++;
				}
				digit = 1;
				args[narg - 1] *= 10;
				args[narg - 1] += (chr - '0');
				chr = get_char(fd);
			}
			xterm_sequence(window, fd, args[0]);
			break;
		default:
			printf("Unknown Sequence %c\n", chr);
	}
}

static int pfd_in (s_window_t *window, s_pollfd_t *pfd)
{
        int x;
        char chr;
        char chr_ = 0;
	char *box;

	box = (char *) s_malloc(FONTW * SCREENW * FONTH * SCREENH * window->surface->bytesperpixel + 1);

        chr = get_char(pfd->fd);

pdf_in_g0:
	switch (chr) {
		case '\000': /* null */
		case '\001':
			break;
		case '\007': /* bell */
			break;
		case '\010': /* backspace */
			cursorx--;
			break;
		case '\011': /* tab */
			for (x = cursorx; x < SCREENW; x++) {
				if (tabs_horizontal[cursory - 1][x]) {
					cursorx = x + 1;
					break;
				}
			}
			break;
		case '\016': /* charset G1 */
			charset = 1;
			break;
		case '\017': /* charset G0 */
			charset = 0;
			break;
		case '\033': /* escape */
			pfd_in_escape(window, pfd->fd);
			break;
		case '\r':   /* carriage return */
			cursorx = 1;
			break;
		case '\n':   /* return carriage */
			if (mode.application == 0) {
				cursorx = 1;
			}
			cursory++;
			break;
		default:
			write_char(window, chr);
	}

	if (cursorx < 1) {
		cursorx = 1;
	}
	if (cursorx > SCREENW) {
		cursorx = 1;
		if (mode.wraparound == 0) {
			chr = get_char(pfd->fd);
			if ((chr != '\n') && (chr != '\r') && (chr != '\033')) {
				chr_ = 1;
				cursory++;
			}
			if (chr == '\033') {
				chr_ = 1;
			}
		}
	}
	if (cursory < 1) {
		cursory = 1;
	}
	if (cursory > SCREENH) {
		s_getbox(window->surface, 0, FONTH, FONTW * SCREENW, FONTH * (SCREENH - 1), box);
		s_fillbox(window->surface, 0, FONTH * (SCREENH - 1), FONTW * SCREENW, FONTH, chr_attr.bg_color);
//		s_putbox(window->surface, 0, 0, FONTW * SCREENW, FONTH * (SCREENH - 1), box);
		s_putboxpart(window->surface, 0, 0, FONTW * SCREENW, FONTH * (SCREENH - 1), FONTW * SCREENW, FONTH * (SCREENH - 1), box, 0, 0);
		cursory = SCREENH;
	}
	if (chr_) {
		chr_ = 0;
		goto pdf_in_g0;
	}

	s_free(box);

	return 0;
}

static int pfd_err (s_window_t *window, s_pollfd_t *pfd)
{
	exit(2);
}

static void atevent (s_window_t *window, s_event_t *event)
{
        int i;

	if (event->type & KEYBD_PRESSED) {
		if (event->keybd->flag & KEYCODE_LCTRLF) {
			for (i = 0; i < CTRLCODES; i++) {
				if (ctrlcodes_c[i] == event->keybd->button) {
				        write(cmd_fd, ctrlcodes_s[i], strlen(ctrlcodes_s[i]));
					return;
				}
			}
		}
		if (isprint(event->keybd->ascii)) {
			write(cmd_fd, &event->keybd->ascii, sizeof(int));
		} else {
			for (i = 0; i < KEYCODES; i++) {
				if (keycodes_c[i] == event->keybd->button) {
					write(cmd_fd, keycodes_s[i], strlen(keycodes_s[i]));
					return;
				}
			}
		}
	}
}

static void term (s_window_t *window)
{
        char **args;
	s_pollfd_t *pfd;

	get_pty();

	if ((pid = fork()) < 0) {
		printf("Could not fork()\n");
		exit(2);
	}
	if (!pid) {
		get_tty();
		putenv("TERM=linux");
		setenv("SDL_VIDEODRIVER", "xynth", 1);
		chdir("~");

		args = (char **) s_calloc(3, sizeof(char *));
		args[0] = (char *) s_malloc(strlen("/bin/bash") + 1);
		strcpy(args[0], "/bin/bash");
		args[1] = NULL;
		args[2] = NULL;

		execvp("/bin/bash", args);

		printf("Error executing %s\n", "/bin/bash");
		exit(2);
	}
	
	s_pollfd_init(&pfd);
	pfd->fd = cmd_fd;
	pfd->pf_in = pfd_in;
	pfd->pf_err = pfd_err;

	s_client_atevent(window, atevent);
	s_pollfd_add(window, pfd);

	close(slave);
	signal(SIGCHLD, sigchld_handler);
}

static void clean_vars (void)
{
	int x;
	int y;
	pid = -1;
	ptydev = NULL;
	slave = -1;
	cmd_fd = -1;
	pw = NULL;
	font = NULL;
	cursorx = 1;
	cursory = 1;
	scursorx = 1;
	scursory = 1;
	charset = 0;
	charset_G[0] = 0;
	charset_G[1] = 0;
	mode.wraparound = 0;
	mode.application = 0;
	chr_attr.bold = 0;
	chr_attr.fg_color = 0;
	chr_attr.bg_color = 0;
	scroll_reg.y0 = 1;
	scroll_reg.y1 = SCREENH;
	for (y = 0; y < SCREENH; y++) {
		for (x = 0; x < SCREENW; x++) {
			tabs_horizontal[y][x] = 0;
		}
	}
}

int main (int argc, char *argv[])
{
	s_window_t *window;
	
	clean_vars();

	s_client_init(&window);

	s_window_new(window, WINDOW_MAIN, NULL);

	s_window_set_title(window, "Xyntherminal");
	s_window_set_coor(window, 0, 100, 100, FONTW * SCREENW, FONTH * SCREENH);
	s_window_set_coor(window, WINDOW_NOFORM, window->surface->buf->x, window->surface->buf->y, FONTW * SCREENW, FONTH * SCREENH);
	s_window_set_resizeable(window, 0);

	s_free(window->surface->vbuf);
	window->surface->vbuf = (char *) s_malloc(window->surface->buf->w * window->surface->buf->h * window->surface->bytesperpixel);
	window->surface->width = window->surface->buf->w;
	window->surface->height = window->surface->buf->h;
	s_fillbox(window->surface, 0, 0, window->surface->width, window->surface->height, s_rgbcolor(window->surface, 0, 0, 0));

        s_font_init(&font, "veramono.ttf");
        s_font_set_size(font, FONTH);
        
	chr_attr_reset(window);
	chr_attr_bg_color(window, 0);

        term(window);

	s_window_show(window);

	s_client_main(window);

	s_font_uninit(font);

	return 0;
}

#if defined(SINGLE_APP)
s_single_app_t single_term = {
	term_main,
	1,
	{"term"}
};
#endif

#endif /* DEMO_TERM */
