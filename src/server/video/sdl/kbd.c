/***************************************************************************
    begin                : Mon Feb 21 2005
    copyright            : (C) 2005 - 2007 by Alper Akcan
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

#if defined(VIDEO_SDL)

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
		priv->keymap[i] = S_KEYCODE_NOCODE;
	}

	priv->keymap[SDLK_ESCAPE] = S_KEYCODE_ESCAPE;
	priv->keymap[SDLK_1] = S_KEYCODE_ONE;
	priv->keymap[SDLK_2] = S_KEYCODE_TWO;
	priv->keymap[SDLK_3] = S_KEYCODE_THREE;
	priv->keymap[SDLK_4] = S_KEYCODE_FOUR;
	priv->keymap[SDLK_5] = S_KEYCODE_FIVE;
	priv->keymap[SDLK_6] = S_KEYCODE_SIX;
	priv->keymap[SDLK_7] = S_KEYCODE_SEVEN;
	priv->keymap[SDLK_8] = S_KEYCODE_EIGHT;
	priv->keymap[SDLK_9] = S_KEYCODE_NINE;
	priv->keymap[SDLK_0] = S_KEYCODE_ZERO;
	priv->keymap[SDLK_MINUS] = S_KEYCODE_MINUS;
	priv->keymap[SDLK_EQUALS] = S_KEYCODE_EQUAL;
	priv->keymap[SDLK_BACKSPACE] = S_KEYCODE_DELETE;
	priv->keymap[SDLK_TAB] = S_KEYCODE_TAB;
	priv->keymap[SDLK_q] = S_KEYCODE_q;
	priv->keymap[SDLK_w] = S_KEYCODE_w;
	priv->keymap[SDLK_e] = S_KEYCODE_e;
	priv->keymap[SDLK_r] = S_KEYCODE_r;
	priv->keymap[SDLK_t] = S_KEYCODE_t;
	priv->keymap[SDLK_y] = S_KEYCODE_y;
	priv->keymap[SDLK_u] = S_KEYCODE_u;
	priv->keymap[SDLK_i] = S_KEYCODE_i;
	priv->keymap[SDLK_o] = S_KEYCODE_o;
	priv->keymap[SDLK_p] = S_KEYCODE_p;
	priv->keymap[SDLK_LEFTBRACKET] = S_KEYCODE_BRACKETLEFT;
	priv->keymap[SDLK_RIGHTBRACKET] = S_KEYCODE_BRACKETRIGHT;
	priv->keymap[SDLK_RETURN] = S_KEYCODE_RETURN;
	priv->keymap[SDLK_LCTRL] = S_KEYCODE_LEFTCONTROL;
	priv->keymap[SDLK_a] = S_KEYCODE_a;
	priv->keymap[SDLK_s] = S_KEYCODE_s;
	priv->keymap[SDLK_d] = S_KEYCODE_d;
	priv->keymap[SDLK_f] = S_KEYCODE_f;
	priv->keymap[SDLK_g] = S_KEYCODE_g;
	priv->keymap[SDLK_h] = S_KEYCODE_h;
	priv->keymap[SDLK_j] = S_KEYCODE_j;
	priv->keymap[SDLK_k] = S_KEYCODE_k;
	priv->keymap[SDLK_l] = S_KEYCODE_l;
	priv->keymap[SDLK_SEMICOLON] = S_KEYCODE_SEMICOLON;
	priv->keymap[SDLK_QUOTE] = S_KEYCODE_APOSTROPHE;
	priv->keymap[SDLK_BACKQUOTE] = S_KEYCODE_GRAVE;
	priv->keymap[SDLK_LSHIFT] = S_KEYCODE_LEFTSHIFT;
	priv->keymap[SDLK_BACKSLASH] = S_KEYCODE_BACKSLASH;
	priv->keymap[SDLK_z] = S_KEYCODE_z;
	priv->keymap[SDLK_x] = S_KEYCODE_x;
	priv->keymap[SDLK_c] = S_KEYCODE_c;
	priv->keymap[SDLK_v] = S_KEYCODE_v;
	priv->keymap[SDLK_b] = S_KEYCODE_b;
	priv->keymap[SDLK_n] = S_KEYCODE_n;
	priv->keymap[SDLK_m] = S_KEYCODE_m;
	priv->keymap[SDLK_COMMA] = S_KEYCODE_COMMA;
	priv->keymap[SDLK_PERIOD] = S_KEYCODE_PERIOD;
	priv->keymap[SDLK_SLASH] = S_KEYCODE_SLASH;
	priv->keymap[SDLK_RSHIFT] = S_KEYCODE_RIGHTSHIFT;
	priv->keymap[SDLK_KP_MULTIPLY] = S_KEYCODE_KP_MULTIPLY;
	priv->keymap[SDLK_LALT] = S_KEYCODE_ALT;
	priv->keymap[SDLK_SPACE] = S_KEYCODE_SPACE;
	priv->keymap[SDLK_CAPSLOCK] = S_KEYCODE_CAPS_LOCK;
	priv->keymap[SDLK_F1] = S_KEYCODE_F1;
	priv->keymap[SDLK_F2] = S_KEYCODE_F2;
	priv->keymap[SDLK_F3] = S_KEYCODE_F3;
	priv->keymap[SDLK_F4] = S_KEYCODE_F4;
	priv->keymap[SDLK_F5] = S_KEYCODE_F5;
	priv->keymap[SDLK_F6] = S_KEYCODE_F6;
	priv->keymap[SDLK_F7] = S_KEYCODE_F7;
	priv->keymap[SDLK_F8] = S_KEYCODE_F8;
	priv->keymap[SDLK_F9] = S_KEYCODE_F9;
	priv->keymap[SDLK_F10] = S_KEYCODE_F10;
	priv->keymap[SDLK_NUMLOCK] = S_KEYCODE_NUM_LOCK;
	priv->keymap[SDLK_SCROLLOCK] = S_KEYCODE_SCROLL_LOCK;
	priv->keymap[SDLK_KP7] = S_KEYCODE_KP_7;
	priv->keymap[SDLK_KP8] = S_KEYCODE_KP_8;
	priv->keymap[SDLK_KP9] = S_KEYCODE_KP_9;
	priv->keymap[SDLK_KP_MINUS] = S_KEYCODE_KP_SUBTRACT;
	priv->keymap[SDLK_KP4] = S_KEYCODE_KP_4;
	priv->keymap[SDLK_KP5] = S_KEYCODE_KP_5;
	priv->keymap[SDLK_KP6] = S_KEYCODE_KP_6;
	priv->keymap[SDLK_KP_PLUS] = S_KEYCODE_KP_ADD;
	priv->keymap[SDLK_KP1] = S_KEYCODE_KP_1;
	priv->keymap[SDLK_KP2] = S_KEYCODE_KP_2;
	priv->keymap[SDLK_KP3] = S_KEYCODE_KP_3;
	priv->keymap[SDLK_KP0] = S_KEYCODE_KP_0;
	priv->keymap[SDLK_KP_PERIOD] = S_KEYCODE_KP_PERIOD;
	priv->keymap[SDLK_LESS] = S_KEYCODE_LESS;
	priv->keymap[SDLK_F11] = S_KEYCODE_F11;
	priv->keymap[SDLK_F12] = S_KEYCODE_F12;
	priv->keymap[SDLK_KP_ENTER] = S_KEYCODE_KP_ENTER;
	priv->keymap[SDLK_RCTRL] = S_KEYCODE_RIGHTCONTROL;
	priv->keymap[SDLK_KP_DIVIDE] = S_KEYCODE_KP_DIVIDE;
	priv->keymap[SDLK_PRINT] = S_KEYCODE_VOIDSYMBOL;
	priv->keymap[SDLK_RALT] = S_KEYCODE_ALTGR;
	priv->keymap[SDLK_BREAK] = S_KEYCODE_BREAK;
	priv->keymap[SDLK_HOME] = S_KEYCODE_HOME;
	priv->keymap[SDLK_UP] = S_KEYCODE_UP;
	priv->keymap[SDLK_PAGEUP] = S_KEYCODE_PAGEUP;
	priv->keymap[SDLK_LEFT] = S_KEYCODE_LEFT;
	priv->keymap[SDLK_RIGHT] = S_KEYCODE_RIGHT;
	priv->keymap[SDLK_END] = S_KEYCODE_END;
	priv->keymap[SDLK_DOWN] = S_KEYCODE_DOWN;
	priv->keymap[SDLK_PAGEDOWN] = S_KEYCODE_PAGEDOWN;
	priv->keymap[SDLK_INSERT] = S_KEYCODE_INSERT;
	priv->keymap[SDLK_DELETE] = S_KEYCODE_REMOVE;
	priv->keymap[SDLK_PAUSE] = S_KEYCODE_PAUSE;
	
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

	keybd->keybd.state = (event.key.state == SDL_PRESSED) ? KEYBD_PRESSED : KEYBD_RELEASED;
	if ((keybd->keybd.keycode == S_KEYCODE_NUM_LOCK) ||
	    (keybd->keybd.keycode == S_KEYCODE_CAPS_LOCK)) {
		keybd->keybd.state = KEYBD_PRESSED;
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

#endif /* VIDEO_SDL */
