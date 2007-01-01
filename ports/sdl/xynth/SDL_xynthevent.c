/***************************************************************************
    begin                : Thu Jan 22 2004
    copyright            : (C) 2004 - 2007 by Alper Akcan
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

#include "SDL_xynth.h"

static SDLKey sdl_xynth_keymap[128];

void sdl_xynth_atexit (s_window_t *window)
{
	if (sdl_xynth_running) {
		while (sdl_xynth_running_) {
			usleep(20000);
		}
		sdl_xynth_running = 0;
		SDL_PrivateQuit();
	}
}

void sdl_xynth_atevent (s_window_t *window, s_event_t *event)
{
        int state = 0;
        int button = 0;
        
	if ((event->type & MOUSE_EVENT) &&
	    ((event->mouse->x >= window->surface->buf->x) &&
	     (event->mouse->x <= window->surface->buf->x + window->surface->buf->w) &&
	     (event->mouse->y >= window->surface->buf->y) &&
	     (event->mouse->y <= window->surface->buf->y + window->surface->buf->h))) {
		if (event->type & MOUSE_OVER) {
			if (event->type & MOUSE_HINT) {
				state = SDL_PRESSED;
			}
			SDL_PrivateMouseMotion(state, 0, event->mouse->x - window->surface->buf->x, event->mouse->y - window->surface->buf->y);
		} else if (event->type & (MOUSE_PRESSED | MOUSE_RELEASED | MOUSE_CLICKED)) {
			if (event->type & (MOUSE_PRESSED)) {
				state = SDL_PRESSED;
			}
			if (event->type & (MOUSE_RELEASED | MOUSE_CLICKED)) {
				state = SDL_RELEASED;
			}
			switch (event->mouse->b) {
				case MOUSE_RIGHTBUTTON:		button = SDL_BUTTON_RIGHT;	break;
				case MOUSE_MIDDLEBUTTON:	button = SDL_BUTTON_MIDDLE;	break;
				case MOUSE_LEFTBUTTON:		button = SDL_BUTTON_LEFT;	break;
				default:			button = 0;			break;
			}
			SDL_PrivateMouseButton(state, button, event->mouse->x - window->surface->buf->x, event->mouse->y - window->surface->buf->y);
		}
	}
	if (event->type & KEYBD_EVENT) {
		SDL_keysym keysym;
		if (event->type & KEYBD_PRESSED) {
			state = SDL_PRESSED;
		} else if (event->type & KEYBD_RELEASED) {
			state = SDL_RELEASED;
		}
		SDL_PrivateKeyboard(state, sdl_xynth_translatekey(event, &keysym));
	}
}

/* Handle any queued OS events */
void sdl_xynth_PumpEvents(_THIS)
{
}

/* Initialize keyboard mapping for this driver */
void sdl_xynth_InitOSKeymap (_THIS)
{
	int i;
	
	for (i = 0; i < SDL_TABLESIZE(sdl_xynth_keymap); i++) {
		sdl_xynth_keymap[i] = SDLK_UNKNOWN;
	}
	
	sdl_xynth_keymap[S_KEYCODE_ESCAPE] = SDLK_ESCAPE;
	sdl_xynth_keymap[S_KEYCODE_ONE] = SDLK_1;
	sdl_xynth_keymap[S_KEYCODE_TWO] = SDLK_2;
	sdl_xynth_keymap[S_KEYCODE_THREE] = SDLK_3;
	sdl_xynth_keymap[S_KEYCODE_FOUR] = SDLK_4;
	sdl_xynth_keymap[S_KEYCODE_FIVE] = SDLK_5;
	sdl_xynth_keymap[S_KEYCODE_SIX] = SDLK_6;
	sdl_xynth_keymap[S_KEYCODE_SEVEN] = SDLK_7;
	sdl_xynth_keymap[S_KEYCODE_EIGHT] = SDLK_8;
	sdl_xynth_keymap[S_KEYCODE_NINE] = SDLK_9;
	sdl_xynth_keymap[S_KEYCODE_ZERO] = SDLK_0;
	sdl_xynth_keymap[S_KEYCODE_MINUS] = SDLK_MINUS;
	sdl_xynth_keymap[S_KEYCODE_EQUAL] = SDLK_EQUALS;
	sdl_xynth_keymap[S_KEYCODE_DELETE] = SDLK_BACKSPACE;
	sdl_xynth_keymap[S_KEYCODE_TAB] = SDLK_TAB;
	sdl_xynth_keymap[S_KEYCODE_q] = SDLK_q;
	sdl_xynth_keymap[S_KEYCODE_w] = SDLK_w;
	sdl_xynth_keymap[S_KEYCODE_e] = SDLK_e;
	sdl_xynth_keymap[S_KEYCODE_r] = SDLK_r;
	sdl_xynth_keymap[S_KEYCODE_t] = SDLK_t;
	sdl_xynth_keymap[S_KEYCODE_y] = SDLK_y;
	sdl_xynth_keymap[S_KEYCODE_u] = SDLK_u;
	sdl_xynth_keymap[S_KEYCODE_i] = SDLK_i;
	sdl_xynth_keymap[S_KEYCODE_o] = SDLK_o;
	sdl_xynth_keymap[S_KEYCODE_p] = SDLK_p;
	sdl_xynth_keymap[S_KEYCODE_BRACKETLEFT] = SDLK_LEFTBRACKET;
	sdl_xynth_keymap[S_KEYCODE_BRACKETRIGHT] = SDLK_RIGHTBRACKET;
	sdl_xynth_keymap[S_KEYCODE_RETURN] = SDLK_RETURN;
	sdl_xynth_keymap[S_KEYCODE_LEFTCONTROL] = SDLK_LCTRL;
	sdl_xynth_keymap[S_KEYCODE_a] = SDLK_a;
	sdl_xynth_keymap[S_KEYCODE_s] = SDLK_s;
	sdl_xynth_keymap[S_KEYCODE_d] = SDLK_d;
	sdl_xynth_keymap[S_KEYCODE_f] = SDLK_f;
	sdl_xynth_keymap[S_KEYCODE_g] = SDLK_g;
	sdl_xynth_keymap[S_KEYCODE_h] = SDLK_h;
	sdl_xynth_keymap[S_KEYCODE_j] = SDLK_j;
	sdl_xynth_keymap[S_KEYCODE_k] = SDLK_k;
	sdl_xynth_keymap[S_KEYCODE_l] = SDLK_l;
	sdl_xynth_keymap[S_KEYCODE_SEMICOLON] = SDLK_SEMICOLON;
	sdl_xynth_keymap[S_KEYCODE_APOSTROPHE] = SDLK_QUOTE;
	sdl_xynth_keymap[S_KEYCODE_GRAVE] = SDLK_BACKQUOTE;
	sdl_xynth_keymap[S_KEYCODE_LEFTSHIFT] = SDLK_LSHIFT;
	sdl_xynth_keymap[S_KEYCODE_BACKSLASH] = SDLK_BACKSLASH;
	sdl_xynth_keymap[S_KEYCODE_z] = SDLK_z;
	sdl_xynth_keymap[S_KEYCODE_x] = SDLK_x;
	sdl_xynth_keymap[S_KEYCODE_c] = SDLK_c;
	sdl_xynth_keymap[S_KEYCODE_v] = SDLK_v;
	sdl_xynth_keymap[S_KEYCODE_b] = SDLK_b;
	sdl_xynth_keymap[S_KEYCODE_n] = SDLK_n;
	sdl_xynth_keymap[S_KEYCODE_m] = SDLK_m;
	sdl_xynth_keymap[S_KEYCODE_COMMA] = SDLK_COMMA;
	sdl_xynth_keymap[S_KEYCODE_PERIOD] = SDLK_PERIOD;
	sdl_xynth_keymap[S_KEYCODE_SLASH] = SDLK_SLASH;
	sdl_xynth_keymap[S_KEYCODE_RIGHTSHIFT] = SDLK_RSHIFT;
	sdl_xynth_keymap[S_KEYCODE_KP_MULTIPLY] = SDLK_KP_MULTIPLY;
	sdl_xynth_keymap[S_KEYCODE_ALT] = SDLK_LALT;
	sdl_xynth_keymap[S_KEYCODE_SPACE] = SDLK_SPACE;
	sdl_xynth_keymap[S_KEYCODE_CAPS_LOCK] = SDLK_CAPSLOCK;
	sdl_xynth_keymap[S_KEYCODE_F1] = SDLK_F1;
	sdl_xynth_keymap[S_KEYCODE_F2] = SDLK_F2;
	sdl_xynth_keymap[S_KEYCODE_F3] = SDLK_F3;
	sdl_xynth_keymap[S_KEYCODE_F4] = SDLK_F4;
	sdl_xynth_keymap[S_KEYCODE_F5] = SDLK_F5;
	sdl_xynth_keymap[S_KEYCODE_F6] = SDLK_F6;
	sdl_xynth_keymap[S_KEYCODE_F7] = SDLK_F7;
	sdl_xynth_keymap[S_KEYCODE_F8] = SDLK_F8;
	sdl_xynth_keymap[S_KEYCODE_F9] = SDLK_F9;
	sdl_xynth_keymap[S_KEYCODE_F10] = SDLK_F10;
	sdl_xynth_keymap[S_KEYCODE_NUM_LOCK] = SDLK_NUMLOCK;
	sdl_xynth_keymap[S_KEYCODE_SCROLL_LOCK] = SDLK_SCROLLOCK;
	sdl_xynth_keymap[S_KEYCODE_KP_7] = SDLK_KP7;
	sdl_xynth_keymap[S_KEYCODE_KP_8] = SDLK_KP8;
	sdl_xynth_keymap[S_KEYCODE_KP_9] = SDLK_KP9;
	sdl_xynth_keymap[S_KEYCODE_KP_SUBTRACT] = SDLK_KP_MINUS;
	sdl_xynth_keymap[S_KEYCODE_KP_4] = SDLK_KP4;
	sdl_xynth_keymap[S_KEYCODE_KP_5] = SDLK_KP5;
	sdl_xynth_keymap[S_KEYCODE_KP_6] = SDLK_KP6;
	sdl_xynth_keymap[S_KEYCODE_KP_ADD] = SDLK_KP_PLUS;
	sdl_xynth_keymap[S_KEYCODE_KP_1] = SDLK_KP1;
	sdl_xynth_keymap[S_KEYCODE_KP_2] = SDLK_KP2;
	sdl_xynth_keymap[S_KEYCODE_KP_3] = SDLK_KP3;
	sdl_xynth_keymap[S_KEYCODE_KP_0] = SDLK_KP0;
	sdl_xynth_keymap[S_KEYCODE_KP_PERIOD] = SDLK_KP_PERIOD;
	sdl_xynth_keymap[S_KEYCODE_LESS] = SDLK_LESS;
	sdl_xynth_keymap[S_KEYCODE_F11] = SDLK_F11;
	sdl_xynth_keymap[S_KEYCODE_F12] = SDLK_F12;
	sdl_xynth_keymap[S_KEYCODE_KP_ENTER] = SDLK_KP_ENTER;
	sdl_xynth_keymap[S_KEYCODE_RIGHTCONTROL] = SDLK_RCTRL;
	sdl_xynth_keymap[S_KEYCODE_KP_DIVIDE] = SDLK_KP_DIVIDE;
	sdl_xynth_keymap[S_KEYCODE_VOIDSYMBOL] = SDLK_PRINT;
	sdl_xynth_keymap[S_KEYCODE_ALTGR] = SDLK_RALT;
	sdl_xynth_keymap[S_KEYCODE_BREAK] = SDLK_BREAK;
	sdl_xynth_keymap[S_KEYCODE_HOME] = SDLK_HOME;
	sdl_xynth_keymap[S_KEYCODE_UP] = SDLK_UP;
	sdl_xynth_keymap[S_KEYCODE_PAGEUP] = SDLK_PAGEUP;
	sdl_xynth_keymap[S_KEYCODE_LEFT] = SDLK_LEFT;
	sdl_xynth_keymap[S_KEYCODE_RIGHT] = SDLK_RIGHT;
	sdl_xynth_keymap[S_KEYCODE_END] = SDLK_END;
	sdl_xynth_keymap[S_KEYCODE_DOWN] = SDLK_DOWN;
	sdl_xynth_keymap[S_KEYCODE_PAGEDOWN] = SDLK_PAGEDOWN;
	sdl_xynth_keymap[S_KEYCODE_INSERT] = SDLK_INSERT;
	sdl_xynth_keymap[S_KEYCODE_REMOVE] = SDLK_DELETE;
	sdl_xynth_keymap[S_KEYCODE_PAUSE] = SDLK_PAUSE;
}

SDL_keysym * sdl_xynth_translatekey(s_event_t *event, SDL_keysym *keysym)
{
	keysym->scancode = event->keybd->scancode;
	keysym->sym = (event->keybd->keycode == S_KEYCODE_NOCODE) ? SDLK_UNKNOWN : sdl_xynth_keymap[event->keybd->keycode];
	keysym->mod = KMOD_NONE;
	keysym->unicode = event->keybd->ascii;
	
	if (SDL_TranslateUNICODE) {
	}
	
	return keysym;
}
