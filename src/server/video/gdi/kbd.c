/***************************************************************************
    begin                : Fri Dec 30 2005
    copyright            : (C) 2005 - 2006 by Caglar Oral
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
#include "../../server.h"
#include "gdi.h"

#ifndef VK_0
#define VK_0	'0'
#define VK_1	'1'
#define VK_2	'2'
#define VK_3	'3'
#define VK_4	'4'
#define VK_5	'5'
#define VK_6	'6'
#define VK_7	'7'
#define VK_8	'8'
#define VK_9	'9'
#define VK_A	'A'
#define VK_B	'B'
#define VK_C	'C'
#define VK_D	'D'
#define VK_E	'E'
#define VK_F	'F'
#define VK_G	'G'
#define VK_H	'H'
#define VK_I	'I'
#define VK_J	'J'
#define VK_K	'K'
#define VK_L	'L'
#define VK_M	'M'
#define VK_N	'N'
#define VK_O	'O'
#define VK_P	'P'
#define VK_Q	'Q'
#define VK_R	'R'
#define VK_S	'S'
#define VK_T	'T'
#define VK_U	'U'
#define VK_V	'V'
#define VK_W	'W'
#define VK_X	'X'
#define VK_Y	'Y'
#define VK_Z	'Z'
#endif

#define VK_SEMICOLON	0xBA
#define VK_EQUALS	0xBB
#define VK_COMMA	0xBC
#define VK_MINUS	0xBD
#define VK_PERIOD	0xBE
#define VK_SLASH	0xBF
#define VK_GRAVE	0xC0
#define VK_LBRACKET	0xDB
#define VK_BACKSLASH	0xDC
#define VK_RBRACKET	0xDD
#define VK_APOSTROPHE	0xDE
#define VK_BACKTICK	0xDF

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
	priv->keymap[VK_BACK] = S_KEYCODE_DELETE;
	priv->keymap[VK_TAB] = S_KEYCODE_TAB;
	priv->keymap[VK_Q] = S_KEYCODE_q;
	priv->keymap[VK_W] = S_KEYCODE_w;
	priv->keymap[VK_E] = S_KEYCODE_e;
	priv->keymap[VK_R] = S_KEYCODE_r;
	priv->keymap[VK_T] = S_KEYCODE_t;
	priv->keymap[VK_Y] = S_KEYCODE_y;
	priv->keymap[VK_U] = S_KEYCODE_u;
	priv->keymap[VK_I] = S_KEYCODE_i;
	priv->keymap[VK_O] = S_KEYCODE_o;
	priv->keymap[VK_P] = S_KEYCODE_p;
	priv->keymap[VK_LBRACKET] = S_KEYCODE_BRACKETLEFT;
	priv->keymap[VK_RBRACKET] = S_KEYCODE_BRACKETRIGHT;
	priv->keymap[VK_RETURN] = S_KEYCODE_RETURN;
	priv->keymap[VK_LCONTROL] = S_KEYCODE_LEFTCONTROL;
	priv->keymap[VK_A] = S_KEYCODE_a;
	priv->keymap[VK_S] = S_KEYCODE_s;
	priv->keymap[VK_D] = S_KEYCODE_d;
	priv->keymap[VK_F] = S_KEYCODE_f;
	priv->keymap[VK_G] = S_KEYCODE_g;
	priv->keymap[VK_H] = S_KEYCODE_h;
	priv->keymap[VK_J] = S_KEYCODE_j;
	priv->keymap[VK_K] = S_KEYCODE_k;
	priv->keymap[VK_L] = S_KEYCODE_l;
	priv->keymap[VK_SEMICOLON] = S_KEYCODE_SEMICOLON;
	priv->keymap[VK_APOSTROPHE] = S_KEYCODE_APOSTROPHE;
	priv->keymap[VK_GRAVE] = S_KEYCODE_GRAVE;
	priv->keymap[VK_BACKTICK] = S_KEYCODE_GRAVE;
	priv->keymap[VK_LSHIFT] = S_KEYCODE_LEFTSHIFT;
	priv->keymap[VK_BACKSLASH] = S_KEYCODE_BACKSLASH;
	priv->keymap[VK_Z] = S_KEYCODE_z;
	priv->keymap[VK_X] = S_KEYCODE_x;
	priv->keymap[VK_C] = S_KEYCODE_c;
	priv->keymap[VK_V] = S_KEYCODE_v;
	priv->keymap[VK_B] = S_KEYCODE_b;
	priv->keymap[VK_N] = S_KEYCODE_n;
	priv->keymap[VK_M] = S_KEYCODE_m;
	priv->keymap[VK_COMMA] = S_KEYCODE_COMMA;
	priv->keymap[VK_PERIOD] = S_KEYCODE_PERIOD;
	priv->keymap[VK_SLASH] = S_KEYCODE_SLASH;
	priv->keymap[VK_RSHIFT] = S_KEYCODE_RIGHTSHIFT;
	priv->keymap[VK_MULTIPLY] = S_KEYCODE_KP_MULTIPLY;
	priv->keymap[VK_LMENU] = S_KEYCODE_ALT;
	priv->keymap[VK_SPACE] = S_KEYCODE_SPACE;
	priv->keymap[VK_CAPITAL] = S_KEYCODE_CAPS_LOCK;
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
	priv->keymap[VK_SCROLL] = S_KEYCODE_SCROLL_LOCK;
	priv->keymap[VK_NUMPAD7] = S_KEYCODE_KP_7;
	priv->keymap[VK_NUMPAD8] = S_KEYCODE_KP_8;
	priv->keymap[VK_NUMPAD9] = S_KEYCODE_KP_9;
	priv->keymap[VK_SUBTRACT] = S_KEYCODE_KP_SUBTRACT;
	priv->keymap[VK_NUMPAD4] = S_KEYCODE_KP_4;
	priv->keymap[VK_NUMPAD5] = S_KEYCODE_KP_5;
	priv->keymap[VK_NUMPAD6] = S_KEYCODE_KP_6;
	priv->keymap[VK_ADD] = S_KEYCODE_KP_ADD;
	priv->keymap[VK_NUMPAD1] = S_KEYCODE_KP_1;
	priv->keymap[VK_NUMPAD2] = S_KEYCODE_KP_2;
	priv->keymap[VK_NUMPAD3] = S_KEYCODE_KP_3;
	priv->keymap[VK_NUMPAD0] = S_KEYCODE_KP_0;
	priv->keymap[VK_DECIMAL] = S_KEYCODE_KP_PERIOD;
	priv->keymap[VK_F11] = S_KEYCODE_F11;
	priv->keymap[VK_F12] = S_KEYCODE_F12;
	priv->keymap[VK_RCONTROL] = S_KEYCODE_RIGHTCONTROL;
	priv->keymap[VK_DIVIDE] = S_KEYCODE_KP_DIVIDE;
	priv->keymap[VK_PRINT] = S_KEYCODE_VOIDSYMBOL;
	priv->keymap[VK_RMENU] = S_KEYCODE_ALTGR;
	priv->keymap[VK_CANCEL] = S_KEYCODE_BREAK;
	priv->keymap[VK_HOME] = S_KEYCODE_HOME;
	priv->keymap[VK_UP] = S_KEYCODE_UP;
	priv->keymap[VK_PRIOR] = S_KEYCODE_PAGEUP;
	priv->keymap[VK_LEFT] = S_KEYCODE_LEFT;
	priv->keymap[VK_RIGHT] = S_KEYCODE_RIGHT;
	priv->keymap[VK_END] = S_KEYCODE_END;
	priv->keymap[VK_DOWN] = S_KEYCODE_DOWN;
	priv->keymap[VK_NEXT] = S_KEYCODE_PAGEDOWN;
	priv->keymap[VK_INSERT] = S_KEYCODE_INSERT;
	priv->keymap[VK_DELETE] = S_KEYCODE_REMOVE;
	priv->keymap[VK_PAUSE] = S_KEYCODE_PAUSE;

	return priv->keybd_fd[0];
err0:   priv->keybd_fd[0] = -1;
	priv->keybd_fd[1] = -1;
	return -1;
}

void s_video_gdi_kbd_update (s_keybd_driver_t *keybd)
{
        int k = 0;
	s_video_gdi_data_t *priv = server->driver->driver_data;
	s_pipe_api_read(priv->keybd_fd[0], &k, sizeof(int));
	keybd->ascii = priv->keybd.ascii;
	keybd->scancode = priv->keybd.scancode;
	keybd->keycode = priv->keybd.keycode;
	keybd->button = priv->keybd.button;
        keybd->state = priv->keybd.state;
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
