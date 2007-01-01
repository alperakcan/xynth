/***************************************************************************
    begin                : Fri Dec 30 2005
    copyright            : (C) 2005 - 2007 by Caglar Oral
    email                : caglaroral@yahoo.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#if defined(VIDEO_GDI)

#include "../../../lib/xynth_.h"
#include "server.h"
#include "gdi.h"

#undef VK_ESCAPE
#undef VK_1
#undef VK_2
#undef VK_3
#undef VK_4
#undef VK_5
#undef VK_6
#undef VK_7
#undef VK_8
#undef VK_9
#undef VK_0
#undef VK_MINUS
#undef VK_EQUALS
#undef VK_BACKSPACE
#undef VK_TAB
#undef VK_q
#undef VK_w
#undef VK_e
#undef VK_r
#undef VK_t
#undef VK_y
#undef VK_u
#undef VK_i
#undef VK_o
#undef VK_p
#undef VK_LEFTBRACKET
#undef VK_RIGHTBRACKET
#undef VK_RETURN
#undef VK_LCTRL
#undef VK_a
#undef VK_s
#undef VK_d
#undef VK_f
#undef VK_g
#undef VK_h
#undef VK_j
#undef VK_k
#undef VK_l
#undef VK_SEMICOLON
#undef VK_QUOTE
#undef VK_BACKQUOTE
#undef VK_LSHIFT
#undef VK_BACKSLASH
#undef VK_z
#undef VK_x
#undef VK_c
#undef VK_v
#undef VK_b
#undef VK_n
#undef VK_m
#undef VK_COMMA
#undef VK_PERIOD
#undef VK_SLASH
#undef VK_RSHIFT
#undef VK_KP_MULTIPLY
#undef VK_LALT
#undef VK_SPACE
#undef VK_CAPSLOCK
#undef VK_F1
#undef VK_F2
#undef VK_F3
#undef VK_F4
#undef VK_F5
#undef VK_F6
#undef VK_F7
#undef VK_F8
#undef VK_F9
#undef VK_F10
#undef VK_NUMLOCK
#undef VK_SCROLLOCK
#undef VK_KP7
#undef VK_KP8
#undef VK_KP9
#undef VK_KP_MINUS
#undef VK_KP4
#undef VK_KP5
#undef VK_KP6
#undef VK_KP_PLUS
#undef VK_KP1
#undef VK_KP2
#undef VK_KP3
#undef VK_KP0
#undef VK_KP_PERIOD
#undef VK_LESS
#undef VK_F11
#undef VK_F12
#undef VK_KP_ENTER
#undef VK_RCTRL
#undef VK_KP_DIVIDE
#undef VK_PRINT
#undef VK_RALT
#undef VK_BREAK
#undef VK_HOME
#undef VK_UP
#undef VK_PAGEUP
#undef VK_LEFT
#undef VK_RIGHT
#undef VK_END
#undef VK_DOWN
#undef VK_PAGEDOWN
#undef VK_INSERT
#undef VK_DELETE
#undef VK_PAUSE

#define VK_ESCAPE	1
#define VK_1		2
#define VK_2		3
#define VK_3		4
#define VK_4		5
#define VK_5		6
#define VK_6		7
#define VK_7		8
#define VK_8		9
#define VK_9		10
#define VK_0		11
#define VK_MINUS	12
#define VK_EQUALS	13
#define VK_BACKSPACE	14
#define VK_TAB		15
#define VK_q		16
#define VK_w		17
#define VK_e		18
#define VK_r		19
#define VK_t		20
#define VK_y		21
#define VK_u		22
#define VK_i		23
#define VK_o		24
#define VK_p		25
#define VK_LEFTBRACKET	26
#define VK_RIGHTBRACKET	27
#define VK_RETURN	28
#define VK_LCTRL	29
#define VK_a		30
#define VK_s		31
#define VK_d		32
#define VK_f		33
#define VK_g		34
#define VK_h		35
#define VK_j		36
#define VK_k		37
#define VK_l		38
#define VK_SEMICOLON	39
#define VK_QUOTE	40
#define VK_BACKQUOTE	41
#define VK_LSHIFT	42
#define VK_BACKSLASH	43
#define VK_z		44
#define VK_x		45
#define VK_c		46
#define VK_v		47
#define VK_b		48
#define VK_n		49
#define VK_m		50
#define VK_COMMA	51
#define VK_PERIOD	52
#define VK_SLASH	53
#define VK_RSHIFT	54
#define VK_KP_MULTIPLY	55
#define VK_LALT		56
#define VK_SPACE	57
#define VK_CAPSLOCK	58
#define VK_F1		59
#define VK_F2		60
#define VK_F3		61
#define VK_F4		62
#define VK_F5		63
#define VK_F6		64
#define VK_F7		65
#define VK_F8		66
#define VK_F9		67
#define VK_F10		68
#define VK_NUMLOCK	69
#define VK_SCROLLOCK	70
#define VK_KP7		71
#define VK_KP8		72
#define VK_KP9		73
#define VK_KP_MINUS	74
#define VK_KP4		75
#define VK_KP5		76
#define VK_KP6		77
#define VK_KP_PLUS	78
#define VK_KP1		79
#define VK_KP2		80
#define VK_KP3		81
#define VK_KP0		82
#define VK_KP_PERIOD	83
#define VK_LESS		86
#define VK_F11		87
#define VK_F12		88
#define VK_KP_ENTER	28
#define VK_RCTRL	29
#define VK_KP_DIVIDE	89
#define VK_PRINT	90
#define VK_RALT		91
#define VK_BREAK	92
#define VK_HOME		93
#define VK_UP		72
#define VK_PAGEUP	73
#define VK_LEFT		75
#define VK_RIGHT	77
#define VK_END		79
#define VK_DOWN		80
#define VK_PAGEDOWN	81
#define VK_INSERT	82
#define VK_DELETE	83
#define VK_PAUSE	69

int s_video_gdi_kbd_init (s_server_conf_t *cfg)
{
        int i;
	s_video_gdi_data_t *priv = server->driver->driver_data;

	if (s_pipe_api_pipe(priv->keybd_fd)) {
		debugf(DSER | DSYS, "pipe failed");
		goto err0;
	}

	for (i = 0; i < GDI_MAX_KEYS; i++) {
		priv->keymap[i] = S_KEYCODE_NOCODE;
	}

	priv->keymap[VK_ESCAPE] = S_KEYCODE_ESCAPE;
	priv->keymap[VK_1] = S_KEYCODE_ONE;
	priv->keymap[VK_2] = S_KEYCODE_TWO;
	priv->keymap[VK_3] = S_KEYCODE_THREE;
	priv->keymap[VK_4] = S_KEYCODE_FOUR;
	priv->keymap[VK_5] = S_KEYCODE_FIVE;
	priv->keymap[VK_6] = S_KEYCODE_SIX;
	priv->keymap[VK_7] = S_KEYCODE_SEVEN;
	priv->keymap[VK_8] = S_KEYCODE_EIGHT;
	priv->keymap[VK_9] = S_KEYCODE_NINE;
	priv->keymap[VK_0] = S_KEYCODE_ZERO;
	priv->keymap[VK_MINUS] = S_KEYCODE_MINUS;
	priv->keymap[VK_EQUALS] = S_KEYCODE_EQUAL;
	priv->keymap[VK_BACKSPACE] = S_KEYCODE_DELETE;
	priv->keymap[VK_TAB] = S_KEYCODE_TAB;
	priv->keymap[VK_q] = S_KEYCODE_q;
	priv->keymap[VK_w] = S_KEYCODE_w;
	priv->keymap[VK_e] = S_KEYCODE_e;
	priv->keymap[VK_r] = S_KEYCODE_r;
	priv->keymap[VK_t] = S_KEYCODE_t;
	priv->keymap[VK_y] = S_KEYCODE_y;
	priv->keymap[VK_u] = S_KEYCODE_u;
	priv->keymap[VK_i] = S_KEYCODE_i;
	priv->keymap[VK_o] = S_KEYCODE_o;
	priv->keymap[VK_p] = S_KEYCODE_p;
	priv->keymap[VK_LEFTBRACKET] = S_KEYCODE_BRACKETLEFT;
	priv->keymap[VK_RIGHTBRACKET] = S_KEYCODE_BRACKETRIGHT;
	priv->keymap[VK_RETURN] = S_KEYCODE_RETURN;
	priv->keymap[VK_LCTRL] = S_KEYCODE_LEFTCONTROL;
	priv->keymap[VK_a] = S_KEYCODE_a;
	priv->keymap[VK_s] = S_KEYCODE_s;
	priv->keymap[VK_d] = S_KEYCODE_d;
	priv->keymap[VK_f] = S_KEYCODE_f;
	priv->keymap[VK_g] = S_KEYCODE_g;
	priv->keymap[VK_h] = S_KEYCODE_h;
	priv->keymap[VK_j] = S_KEYCODE_j;
	priv->keymap[VK_k] = S_KEYCODE_k;
	priv->keymap[VK_l] = S_KEYCODE_l;
	priv->keymap[VK_SEMICOLON] = S_KEYCODE_SEMICOLON;
	priv->keymap[VK_QUOTE] = S_KEYCODE_APOSTROPHE;
	priv->keymap[VK_BACKQUOTE] = S_KEYCODE_GRAVE;
	priv->keymap[VK_LSHIFT] = S_KEYCODE_LEFTSHIFT;
	priv->keymap[VK_BACKSLASH] = S_KEYCODE_BACKSLASH;
	priv->keymap[VK_z] = S_KEYCODE_z;
	priv->keymap[VK_x] = S_KEYCODE_x;
	priv->keymap[VK_c] = S_KEYCODE_c;
	priv->keymap[VK_v] = S_KEYCODE_v;
	priv->keymap[VK_b] = S_KEYCODE_b;
	priv->keymap[VK_n] = S_KEYCODE_n;
	priv->keymap[VK_m] = S_KEYCODE_m;
	priv->keymap[VK_COMMA] = S_KEYCODE_COMMA;
	priv->keymap[VK_PERIOD] = S_KEYCODE_PERIOD;
	priv->keymap[VK_SLASH] = S_KEYCODE_SLASH;
	priv->keymap[VK_RSHIFT] = S_KEYCODE_RIGHTSHIFT;
	priv->keymap[VK_KP_MULTIPLY] = S_KEYCODE_KP_MULTIPLY;
	priv->keymap[VK_LALT] = S_KEYCODE_ALT;
	priv->keymap[VK_SPACE] = S_KEYCODE_SPACE;
	priv->keymap[VK_CAPSLOCK] = S_KEYCODE_CAPS_LOCK;
	priv->keymap[VK_F1] = S_KEYCODE_F1;
	priv->keymap[VK_F2] = S_KEYCODE_F2;
	priv->keymap[VK_F3] = S_KEYCODE_F3;
	priv->keymap[VK_F4] = S_KEYCODE_F4;
	priv->keymap[VK_F5] = S_KEYCODE_F5;
	priv->keymap[VK_F6] = S_KEYCODE_F6;
	priv->keymap[VK_F7] = S_KEYCODE_F7;
	priv->keymap[VK_F8] = S_KEYCODE_F8;
	priv->keymap[VK_F9] = S_KEYCODE_F9;
	priv->keymap[VK_F10] = S_KEYCODE_F10;
	priv->keymap[VK_NUMLOCK] = S_KEYCODE_NUM_LOCK;
	priv->keymap[VK_SCROLLOCK] = S_KEYCODE_SCROLL_LOCK;
	priv->keymap[VK_KP7] = S_KEYCODE_KP_7;
	priv->keymap[VK_KP8] = S_KEYCODE_KP_8;
	priv->keymap[VK_KP9] = S_KEYCODE_KP_9;
	priv->keymap[VK_KP_MINUS] = S_KEYCODE_KP_SUBTRACT;
	priv->keymap[VK_KP4] = S_KEYCODE_KP_4;
	priv->keymap[VK_KP5] = S_KEYCODE_KP_5;
	priv->keymap[VK_KP6] = S_KEYCODE_KP_6;
	priv->keymap[VK_KP_PLUS] = S_KEYCODE_KP_ADD;
	priv->keymap[VK_KP1] = S_KEYCODE_KP_1;
	priv->keymap[VK_KP2] = S_KEYCODE_KP_2;
	priv->keymap[VK_KP3] = S_KEYCODE_KP_3;
	priv->keymap[VK_KP0] = S_KEYCODE_KP_0;
	priv->keymap[VK_KP_PERIOD] = S_KEYCODE_KP_PERIOD;
	priv->keymap[VK_LESS] = S_KEYCODE_LESS;
	priv->keymap[VK_F11] = S_KEYCODE_F11;
	priv->keymap[VK_F12] = S_KEYCODE_F12;
	priv->keymap[VK_KP_ENTER] = S_KEYCODE_KP_ENTER;
	priv->keymap[VK_RCTRL] = S_KEYCODE_RIGHTCONTROL;
	priv->keymap[VK_KP_DIVIDE] = S_KEYCODE_KP_DIVIDE;
	priv->keymap[VK_PRINT] = S_KEYCODE_VOIDSYMBOL;
	priv->keymap[VK_RALT] = S_KEYCODE_ALTGR;
	priv->keymap[VK_BREAK] = S_KEYCODE_BREAK;
	priv->keymap[VK_HOME] = S_KEYCODE_HOME;
	priv->keymap[VK_UP] = S_KEYCODE_UP;
	priv->keymap[VK_PAGEUP] = S_KEYCODE_PAGEUP;
	priv->keymap[VK_LEFT] = S_KEYCODE_LEFT;
	priv->keymap[VK_RIGHT] = S_KEYCODE_RIGHT;
	priv->keymap[VK_END] = S_KEYCODE_END;
	priv->keymap[VK_DOWN] = S_KEYCODE_DOWN;
	priv->keymap[VK_PAGEDOWN] = S_KEYCODE_PAGEDOWN;
	priv->keymap[VK_INSERT] = S_KEYCODE_INSERT;
	priv->keymap[VK_DELETE] = S_KEYCODE_REMOVE;
	priv->keymap[VK_PAUSE] = S_KEYCODE_PAUSE;

	return priv->keybd_fd[0];
err0:   priv->keybd_fd[0] = -1;
	priv->keybd_fd[1] = -1;
	return -1;
}

int s_video_gdi_kbd_update (s_video_input_data_t *keybd)
{
        int k = 0;
	s_video_gdi_data_t *priv = server->driver->driver_data;
	s_pipe_api_read(priv->keybd_fd[0], &k, sizeof(int));
	keybd->keybd.ascii = priv->keybd.ascii;
	keybd->keybd.scancode = priv->keybd.scancode;
	keybd->keybd.keycode = priv->keybd.keycode;
	keybd->keybd.button = priv->keybd.button;
        keybd->keybd.state = priv->keybd.state;
        return 0;
}

void s_video_gdi_kbd_uninit (void)
{
	s_video_gdi_data_t *priv = server->driver->driver_data;
	s_pipe_api_close(priv->keybd_fd[0]);
	s_pipe_api_close(priv->keybd_fd[1]);
	priv->keybd_fd[0] = -1;
	priv->keybd_fd[1] = -1;
}

#endif /* VIDEO_GDI */
