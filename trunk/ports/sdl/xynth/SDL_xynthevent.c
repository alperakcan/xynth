/***************************************************************************
    begin                : Thu Jan 22 2004
    copyright            : (C) 2004 - 2009 by Alper Akcan
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
        
	if ((event->type & EVENT_TYPE_MOUSE) &&
	    ((event->mouse->x >= window->surface->buf->x) &&
	     (event->mouse->x <= window->surface->buf->x + window->surface->buf->w) &&
	     (event->mouse->y >= window->surface->buf->y) &&
	     (event->mouse->y <= window->surface->buf->y + window->surface->buf->h))) {
		if (event->type & EVENT_TYPE_MOUSE_OVER) {
			if (event->type & EVENT_TYPE_MOUSE_HINT1) {
				state = SDL_PRESSED;
			}
			SDL_PrivateMouseMotion(state, 0, event->mouse->x - window->surface->buf->x, event->mouse->y - window->surface->buf->y);
		} else if (event->type & (EVENT_TYPE_MOUSE_PRESSED | EVENT_TYPE_MOUSE_RELEASED | EVENT_TYPE_MOUSE_CLICKED)) {
			if (event->type & (EVENT_TYPE_MOUSE_PRESSED)) {
				state = SDL_PRESSED;
			}
			if (event->type & (EVENT_TYPE_MOUSE_RELEASED | EVENT_TYPE_MOUSE_CLICKED)) {
				state = SDL_RELEASED;
			}
			switch (event->mouse->b) {
				case MOUSE_BUTTON_RIGHT:	button = SDL_BUTTON_RIGHT;	break;
				case MOUSE_BUTTON_MIDDLE:	button = SDL_BUTTON_MIDDLE;	break;
				case MOUSE_BUTTON_LEFT:		button = SDL_BUTTON_LEFT;	break;
				default:			button = 0;			break;
			}
			SDL_PrivateMouseButton(state, button, event->mouse->x - window->surface->buf->x, event->mouse->y - window->surface->buf->y);
		}
	}
	if (event->type & EVENT_TYPE_KEYBOARD) {
		SDL_keysym keysym;
		if (event->type & EVENT_TYPE_KEYBOARD_PRESSED) {
			state = SDL_PRESSED;
		} else if (event->type & EVENT_TYPE_KEYBOARD_RELEASED) {
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
	
	sdl_xynth_keymap[KEYBOARD_BUTTON_ESCAPE] = SDLK_ESCAPE;
	sdl_xynth_keymap[KEYBOARD_BUTTON_ONE] = SDLK_1;
	sdl_xynth_keymap[KEYBOARD_BUTTON_TWO] = SDLK_2;
	sdl_xynth_keymap[KEYBOARD_BUTTON_THREE] = SDLK_3;
	sdl_xynth_keymap[KEYBOARD_BUTTON_FOUR] = SDLK_4;
	sdl_xynth_keymap[KEYBOARD_BUTTON_FIVE] = SDLK_5;
	sdl_xynth_keymap[KEYBOARD_BUTTON_SIX] = SDLK_6;
	sdl_xynth_keymap[KEYBOARD_BUTTON_SEVEN] = SDLK_7;
	sdl_xynth_keymap[KEYBOARD_BUTTON_EIGHT] = SDLK_8;
	sdl_xynth_keymap[KEYBOARD_BUTTON_NINE] = SDLK_9;
	sdl_xynth_keymap[KEYBOARD_BUTTON_ZERO] = SDLK_0;
	sdl_xynth_keymap[KEYBOARD_BUTTON_MINUS] = SDLK_MINUS;
	sdl_xynth_keymap[KEYBOARD_BUTTON_EQUAL] = SDLK_EQUALS;
	sdl_xynth_keymap[KEYBOARD_BUTTON_DELETE] = SDLK_BACKSPACE;
	sdl_xynth_keymap[KEYBOARD_BUTTON_TAB] = SDLK_TAB;
	sdl_xynth_keymap[KEYBOARD_BUTTON_q] = SDLK_q;
	sdl_xynth_keymap[KEYBOARD_BUTTON_w] = SDLK_w;
	sdl_xynth_keymap[KEYBOARD_BUTTON_e] = SDLK_e;
	sdl_xynth_keymap[KEYBOARD_BUTTON_r] = SDLK_r;
	sdl_xynth_keymap[KEYBOARD_BUTTON_t] = SDLK_t;
	sdl_xynth_keymap[KEYBOARD_BUTTON_y] = SDLK_y;
	sdl_xynth_keymap[KEYBOARD_BUTTON_u] = SDLK_u;
	sdl_xynth_keymap[KEYBOARD_BUTTON_i] = SDLK_i;
	sdl_xynth_keymap[KEYBOARD_BUTTON_o] = SDLK_o;
	sdl_xynth_keymap[KEYBOARD_BUTTON_p] = SDLK_p;
	sdl_xynth_keymap[KEYBOARD_BUTTON_BRACKETLEFT] = SDLK_LEFTBRACKET;
	sdl_xynth_keymap[KEYBOARD_BUTTON_BRACKETRIGHT] = SDLK_RIGHTBRACKET;
	sdl_xynth_keymap[KEYBOARD_BUTTON_RETURN] = SDLK_RETURN;
	sdl_xynth_keymap[KEYBOARD_BUTTON_LEFTCONTROL] = SDLK_LCTRL;
	sdl_xynth_keymap[KEYBOARD_BUTTON_a] = SDLK_a;
	sdl_xynth_keymap[KEYBOARD_BUTTON_s] = SDLK_s;
	sdl_xynth_keymap[KEYBOARD_BUTTON_d] = SDLK_d;
	sdl_xynth_keymap[KEYBOARD_BUTTON_f] = SDLK_f;
	sdl_xynth_keymap[KEYBOARD_BUTTON_g] = SDLK_g;
	sdl_xynth_keymap[KEYBOARD_BUTTON_h] = SDLK_h;
	sdl_xynth_keymap[KEYBOARD_BUTTON_j] = SDLK_j;
	sdl_xynth_keymap[KEYBOARD_BUTTON_k] = SDLK_k;
	sdl_xynth_keymap[KEYBOARD_BUTTON_l] = SDLK_l;
	sdl_xynth_keymap[KEYBOARD_BUTTON_SEMICOLON] = SDLK_SEMICOLON;
	sdl_xynth_keymap[KEYBOARD_BUTTON_APOSTROPHE] = SDLK_QUOTE;
	sdl_xynth_keymap[KEYBOARD_BUTTON_GRAVE] = SDLK_BACKQUOTE;
	sdl_xynth_keymap[KEYBOARD_BUTTON_LEFTSHIFT] = SDLK_LSHIFT;
	sdl_xynth_keymap[KEYBOARD_BUTTON_BACKSLASH] = SDLK_BACKSLASH;
	sdl_xynth_keymap[KEYBOARD_BUTTON_z] = SDLK_z;
	sdl_xynth_keymap[KEYBOARD_BUTTON_x] = SDLK_x;
	sdl_xynth_keymap[KEYBOARD_BUTTON_c] = SDLK_c;
	sdl_xynth_keymap[KEYBOARD_BUTTON_v] = SDLK_v;
	sdl_xynth_keymap[KEYBOARD_BUTTON_b] = SDLK_b;
	sdl_xynth_keymap[KEYBOARD_BUTTON_n] = SDLK_n;
	sdl_xynth_keymap[KEYBOARD_BUTTON_m] = SDLK_m;
	sdl_xynth_keymap[KEYBOARD_BUTTON_COMMA] = SDLK_COMMA;
	sdl_xynth_keymap[KEYBOARD_BUTTON_PERIOD] = SDLK_PERIOD;
	sdl_xynth_keymap[KEYBOARD_BUTTON_SLASH] = SDLK_SLASH;
	sdl_xynth_keymap[KEYBOARD_BUTTON_RIGHTSHIFT] = SDLK_RSHIFT;
	sdl_xynth_keymap[KEYBOARD_BUTTON_KP_MULTIPLY] = SDLK_KP_MULTIPLY;
	sdl_xynth_keymap[KEYBOARD_BUTTON_ALT] = SDLK_LALT;
	sdl_xynth_keymap[KEYBOARD_BUTTON_SPACE] = SDLK_SPACE;
	sdl_xynth_keymap[KEYBOARD_BUTTON_CAPS_LOCK] = SDLK_CAPSLOCK;
	sdl_xynth_keymap[KEYBOARD_BUTTON_F1] = SDLK_F1;
	sdl_xynth_keymap[KEYBOARD_BUTTON_F2] = SDLK_F2;
	sdl_xynth_keymap[KEYBOARD_BUTTON_F3] = SDLK_F3;
	sdl_xynth_keymap[KEYBOARD_BUTTON_F4] = SDLK_F4;
	sdl_xynth_keymap[KEYBOARD_BUTTON_F5] = SDLK_F5;
	sdl_xynth_keymap[KEYBOARD_BUTTON_F6] = SDLK_F6;
	sdl_xynth_keymap[KEYBOARD_BUTTON_F7] = SDLK_F7;
	sdl_xynth_keymap[KEYBOARD_BUTTON_F8] = SDLK_F8;
	sdl_xynth_keymap[KEYBOARD_BUTTON_F9] = SDLK_F9;
	sdl_xynth_keymap[KEYBOARD_BUTTON_F10] = SDLK_F10;
	sdl_xynth_keymap[KEYBOARD_BUTTON_NUM_LOCK] = SDLK_NUMLOCK;
	sdl_xynth_keymap[KEYBOARD_BUTTON_SCROLL_LOCK] = SDLK_SCROLLOCK;
	sdl_xynth_keymap[KEYBOARD_BUTTON_KP_7] = SDLK_KP7;
	sdl_xynth_keymap[KEYBOARD_BUTTON_KP_8] = SDLK_KP8;
	sdl_xynth_keymap[KEYBOARD_BUTTON_KP_9] = SDLK_KP9;
	sdl_xynth_keymap[KEYBOARD_BUTTON_KP_SUBTRACT] = SDLK_KP_MINUS;
	sdl_xynth_keymap[KEYBOARD_BUTTON_KP_4] = SDLK_KP4;
	sdl_xynth_keymap[KEYBOARD_BUTTON_KP_5] = SDLK_KP5;
	sdl_xynth_keymap[KEYBOARD_BUTTON_KP_6] = SDLK_KP6;
	sdl_xynth_keymap[KEYBOARD_BUTTON_KP_ADD] = SDLK_KP_PLUS;
	sdl_xynth_keymap[KEYBOARD_BUTTON_KP_1] = SDLK_KP1;
	sdl_xynth_keymap[KEYBOARD_BUTTON_KP_2] = SDLK_KP2;
	sdl_xynth_keymap[KEYBOARD_BUTTON_KP_3] = SDLK_KP3;
	sdl_xynth_keymap[KEYBOARD_BUTTON_KP_0] = SDLK_KP0;
	sdl_xynth_keymap[KEYBOARD_BUTTON_KP_PERIOD] = SDLK_KP_PERIOD;
	sdl_xynth_keymap[KEYBOARD_BUTTON_LESS] = SDLK_LESS;
	sdl_xynth_keymap[KEYBOARD_BUTTON_F11] = SDLK_F11;
	sdl_xynth_keymap[KEYBOARD_BUTTON_F12] = SDLK_F12;
	sdl_xynth_keymap[KEYBOARD_BUTTON_KP_ENTER] = SDLK_KP_ENTER;
	sdl_xynth_keymap[KEYBOARD_BUTTON_RIGHTCONTROL] = SDLK_RCTRL;
	sdl_xynth_keymap[KEYBOARD_BUTTON_KP_DIVIDE] = SDLK_KP_DIVIDE;
	sdl_xynth_keymap[KEYBOARD_BUTTON_VOIDSYMBOL] = SDLK_PRINT;
	sdl_xynth_keymap[KEYBOARD_BUTTON_ALTGR] = SDLK_RALT;
	sdl_xynth_keymap[KEYBOARD_BUTTON_BREAK] = SDLK_BREAK;
	sdl_xynth_keymap[KEYBOARD_BUTTON_HOME] = SDLK_HOME;
	sdl_xynth_keymap[KEYBOARD_BUTTON_UP] = SDLK_UP;
	sdl_xynth_keymap[KEYBOARD_BUTTON_PAGEUP] = SDLK_PAGEUP;
	sdl_xynth_keymap[KEYBOARD_BUTTON_LEFT] = SDLK_LEFT;
	sdl_xynth_keymap[KEYBOARD_BUTTON_RIGHT] = SDLK_RIGHT;
	sdl_xynth_keymap[KEYBOARD_BUTTON_END] = SDLK_END;
	sdl_xynth_keymap[KEYBOARD_BUTTON_DOWN] = SDLK_DOWN;
	sdl_xynth_keymap[KEYBOARD_BUTTON_PAGEDOWN] = SDLK_PAGEDOWN;
	sdl_xynth_keymap[KEYBOARD_BUTTON_INSERT] = SDLK_INSERT;
	sdl_xynth_keymap[KEYBOARD_BUTTON_REMOVE] = SDLK_DELETE;
	sdl_xynth_keymap[KEYBOARD_BUTTON_PAUSE] = SDLK_PAUSE;
}

SDL_keysym * sdl_xynth_translatekey(s_event_t *event, SDL_keysym *keysym)
{
	keysym->scancode = event->keybd->scancode;
	keysym->sym = (event->keybd->keycode == KEYBOARD_BUTTON_NOCODE) ? SDLK_UNKNOWN : sdl_xynth_keymap[event->keybd->keycode];
	keysym->mod = KMOD_NONE;
	keysym->unicode = event->keybd->ascii;
	
	if (SDL_TranslateUNICODE) {
	}
	
	return keysym;
}
