/***************************************************************************
    begin                : Mon Feb 21 2005
    copyright            : (C) 2005 - 2008 by Alper Akcan
    email                : alper.akcan@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#include "../../../lib/xynth_.h"
#include "server.h"
#include "sdl.h"

#include <unistd.h>

int s_video_sdl_kbd_init (s_server_conf_t *cfg)
{
        int i;
	s_video_sdl_data_t *priv = server->driver->driver_data;

	if (s_pipe_api_pipe(priv->keybd_fd)) {
		debugf(DSER | DSYS, "pipe failed");
		goto err0;
	}

        SDL_EnableUNICODE(1);

	for (i = 0; i < SDLK_LAST; i++) {
		priv->keymap[i] = KEYBOARD_BUTTON_NOCODE;
	}

	priv->keymap[SDLK_ESCAPE] = KEYBOARD_BUTTON_ESCAPE;
	priv->keymap[SDLK_1] = KEYBOARD_BUTTON_ONE;
	priv->keymap[SDLK_2] = KEYBOARD_BUTTON_TWO;
	priv->keymap[SDLK_3] = KEYBOARD_BUTTON_THREE;
	priv->keymap[SDLK_4] = KEYBOARD_BUTTON_FOUR;
	priv->keymap[SDLK_5] = KEYBOARD_BUTTON_FIVE;
	priv->keymap[SDLK_6] = KEYBOARD_BUTTON_SIX;
	priv->keymap[SDLK_7] = KEYBOARD_BUTTON_SEVEN;
	priv->keymap[SDLK_8] = KEYBOARD_BUTTON_EIGHT;
	priv->keymap[SDLK_9] = KEYBOARD_BUTTON_NINE;
	priv->keymap[SDLK_0] = KEYBOARD_BUTTON_ZERO;
	priv->keymap[SDLK_MINUS] = KEYBOARD_BUTTON_MINUS;
	priv->keymap[SDLK_EQUALS] = KEYBOARD_BUTTON_EQUAL;
	priv->keymap[SDLK_BACKSPACE] = KEYBOARD_BUTTON_DELETE;
	priv->keymap[SDLK_TAB] = KEYBOARD_BUTTON_TAB;
	priv->keymap[SDLK_q] = KEYBOARD_BUTTON_q;
	priv->keymap[SDLK_w] = KEYBOARD_BUTTON_w;
	priv->keymap[SDLK_e] = KEYBOARD_BUTTON_e;
	priv->keymap[SDLK_r] = KEYBOARD_BUTTON_r;
	priv->keymap[SDLK_t] = KEYBOARD_BUTTON_t;
	priv->keymap[SDLK_y] = KEYBOARD_BUTTON_y;
	priv->keymap[SDLK_u] = KEYBOARD_BUTTON_u;
	priv->keymap[SDLK_i] = KEYBOARD_BUTTON_i;
	priv->keymap[SDLK_o] = KEYBOARD_BUTTON_o;
	priv->keymap[SDLK_p] = KEYBOARD_BUTTON_p;
	priv->keymap[SDLK_LEFTBRACKET] = KEYBOARD_BUTTON_BRACKETLEFT;
	priv->keymap[SDLK_RIGHTBRACKET] = KEYBOARD_BUTTON_BRACKETRIGHT;
	priv->keymap[SDLK_RETURN] = KEYBOARD_BUTTON_RETURN;
	priv->keymap[SDLK_LCTRL] = KEYBOARD_BUTTON_LEFTCONTROL;
	priv->keymap[SDLK_a] = KEYBOARD_BUTTON_a;
	priv->keymap[SDLK_s] = KEYBOARD_BUTTON_s;
	priv->keymap[SDLK_d] = KEYBOARD_BUTTON_d;
	priv->keymap[SDLK_f] = KEYBOARD_BUTTON_f;
	priv->keymap[SDLK_g] = KEYBOARD_BUTTON_g;
	priv->keymap[SDLK_h] = KEYBOARD_BUTTON_h;
	priv->keymap[SDLK_j] = KEYBOARD_BUTTON_j;
	priv->keymap[SDLK_k] = KEYBOARD_BUTTON_k;
	priv->keymap[SDLK_l] = KEYBOARD_BUTTON_l;
	priv->keymap[SDLK_SEMICOLON] = KEYBOARD_BUTTON_SEMICOLON;
	priv->keymap[SDLK_QUOTE] = KEYBOARD_BUTTON_APOSTROPHE;
	priv->keymap[SDLK_BACKQUOTE] = KEYBOARD_BUTTON_GRAVE;
	priv->keymap[SDLK_LSHIFT] = KEYBOARD_BUTTON_LEFTSHIFT;
	priv->keymap[SDLK_BACKSLASH] = KEYBOARD_BUTTON_BACKSLASH;
	priv->keymap[SDLK_z] = KEYBOARD_BUTTON_z;
	priv->keymap[SDLK_x] = KEYBOARD_BUTTON_x;
	priv->keymap[SDLK_c] = KEYBOARD_BUTTON_c;
	priv->keymap[SDLK_v] = KEYBOARD_BUTTON_v;
	priv->keymap[SDLK_b] = KEYBOARD_BUTTON_b;
	priv->keymap[SDLK_n] = KEYBOARD_BUTTON_n;
	priv->keymap[SDLK_m] = KEYBOARD_BUTTON_m;
	priv->keymap[SDLK_COMMA] = KEYBOARD_BUTTON_COMMA;
	priv->keymap[SDLK_PERIOD] = KEYBOARD_BUTTON_PERIOD;
	priv->keymap[SDLK_SLASH] = KEYBOARD_BUTTON_SLASH;
	priv->keymap[SDLK_RSHIFT] = KEYBOARD_BUTTON_RIGHTSHIFT;
	priv->keymap[SDLK_KP_MULTIPLY] = KEYBOARD_BUTTON_KP_MULTIPLY;
	priv->keymap[SDLK_LALT] = KEYBOARD_BUTTON_ALT;
	priv->keymap[SDLK_SPACE] = KEYBOARD_BUTTON_SPACE;
	priv->keymap[SDLK_CAPSLOCK] = KEYBOARD_BUTTON_CAPS_LOCK;
	priv->keymap[SDLK_F1] = KEYBOARD_BUTTON_F1;
	priv->keymap[SDLK_F2] = KEYBOARD_BUTTON_F2;
	priv->keymap[SDLK_F3] = KEYBOARD_BUTTON_F3;
	priv->keymap[SDLK_F4] = KEYBOARD_BUTTON_F4;
	priv->keymap[SDLK_F5] = KEYBOARD_BUTTON_F5;
	priv->keymap[SDLK_F6] = KEYBOARD_BUTTON_F6;
	priv->keymap[SDLK_F7] = KEYBOARD_BUTTON_F7;
	priv->keymap[SDLK_F8] = KEYBOARD_BUTTON_F8;
	priv->keymap[SDLK_F9] = KEYBOARD_BUTTON_F9;
	priv->keymap[SDLK_F10] = KEYBOARD_BUTTON_F10;
	priv->keymap[SDLK_NUMLOCK] = KEYBOARD_BUTTON_NUM_LOCK;
	priv->keymap[SDLK_SCROLLOCK] = KEYBOARD_BUTTON_SCROLL_LOCK;
	priv->keymap[SDLK_KP7] = KEYBOARD_BUTTON_KP_7;
	priv->keymap[SDLK_KP8] = KEYBOARD_BUTTON_KP_8;
	priv->keymap[SDLK_KP9] = KEYBOARD_BUTTON_KP_9;
	priv->keymap[SDLK_KP_MINUS] = KEYBOARD_BUTTON_KP_SUBTRACT;
	priv->keymap[SDLK_KP4] = KEYBOARD_BUTTON_KP_4;
	priv->keymap[SDLK_KP5] = KEYBOARD_BUTTON_KP_5;
	priv->keymap[SDLK_KP6] = KEYBOARD_BUTTON_KP_6;
	priv->keymap[SDLK_KP_PLUS] = KEYBOARD_BUTTON_KP_ADD;
	priv->keymap[SDLK_KP1] = KEYBOARD_BUTTON_KP_1;
	priv->keymap[SDLK_KP2] = KEYBOARD_BUTTON_KP_2;
	priv->keymap[SDLK_KP3] = KEYBOARD_BUTTON_KP_3;
	priv->keymap[SDLK_KP0] = KEYBOARD_BUTTON_KP_0;
	priv->keymap[SDLK_KP_PERIOD] = KEYBOARD_BUTTON_KP_PERIOD;
	priv->keymap[SDLK_LESS] = KEYBOARD_BUTTON_LESS;
	priv->keymap[SDLK_F11] = KEYBOARD_BUTTON_F11;
	priv->keymap[SDLK_F12] = KEYBOARD_BUTTON_F12;
	priv->keymap[SDLK_KP_ENTER] = KEYBOARD_BUTTON_KP_ENTER;
	priv->keymap[SDLK_RCTRL] = KEYBOARD_BUTTON_RIGHTCONTROL;
	priv->keymap[SDLK_KP_DIVIDE] = KEYBOARD_BUTTON_KP_DIVIDE;
	priv->keymap[SDLK_PRINT] = KEYBOARD_BUTTON_VOIDSYMBOL;
	priv->keymap[SDLK_RALT] = KEYBOARD_BUTTON_ALTGR;
	priv->keymap[SDLK_BREAK] = KEYBOARD_BUTTON_BREAK;
	priv->keymap[SDLK_HOME] = KEYBOARD_BUTTON_HOME;
	priv->keymap[SDLK_UP] = KEYBOARD_BUTTON_UP;
	priv->keymap[SDLK_PAGEUP] = KEYBOARD_BUTTON_PAGEUP;
	priv->keymap[SDLK_LEFT] = KEYBOARD_BUTTON_LEFT;
	priv->keymap[SDLK_RIGHT] = KEYBOARD_BUTTON_RIGHT;
	priv->keymap[SDLK_END] = KEYBOARD_BUTTON_END;
	priv->keymap[SDLK_DOWN] = KEYBOARD_BUTTON_DOWN;
	priv->keymap[SDLK_PAGEDOWN] = KEYBOARD_BUTTON_PAGEDOWN;
	priv->keymap[SDLK_INSERT] = KEYBOARD_BUTTON_INSERT;
	priv->keymap[SDLK_DELETE] = KEYBOARD_BUTTON_REMOVE;
	priv->keymap[SDLK_PAUSE] = KEYBOARD_BUTTON_PAUSE;
	
	return priv->keybd_fd[0];
err0:   priv->keybd_fd[0] = -1;
	priv->keybd_fd[1] = -1;
	return -1;
}

int s_video_sdl_kbd_update (s_video_input_data_t *keybd)
{
	SDL_Event event;
	s_video_sdl_data_t *priv = server->driver->driver_data;

	s_pipe_api_read(priv->keybd_fd[0], &event.key, sizeof(event.key));

	keybd->keybd.ascii = event.key.keysym.unicode;
	keybd->keybd.scancode = event.key.keysym.scancode;
	keybd->keybd.keycode = priv->keymap[event.key.keysym.sym];
	keybd->keybd.button = priv->keymap[event.key.keysym.sym];

	keybd->keybd.state = (event.key.state == SDL_PRESSED) ? EVENT_TYPE_KEYBOARD_PRESSED : EVENT_TYPE_KEYBOARD_RELEASED;
	if ((keybd->keybd.keycode == KEYBOARD_BUTTON_NUM_LOCK) ||
	    (keybd->keybd.keycode == KEYBOARD_BUTTON_CAPS_LOCK)) {
		keybd->keybd.state = EVENT_TYPE_KEYBOARD_PRESSED;
	}
	return 0;
}

void s_video_sdl_kbd_uninit (void)
{
	s_video_sdl_data_t *priv = server->driver->driver_data;
	s_pipe_api_close(priv->keybd_fd[0]);
	s_pipe_api_close(priv->keybd_fd[1]);
	priv->keybd_fd[0] = -1;
	priv->keybd_fd[1] = -1;
}
