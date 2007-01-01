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

/* Set the title and icon text */
void sdl_xynth_SetCaption (_THIS, const char *title, const char *icon)
{
	s_window_set_title(this->hidden->window, (char *) title);
}

/* Set the window icon image */
void sdl_xynth_SetIcon (_THIS, SDL_Surface *icon, Uint8 *mask)
{
}

/* Iconify the window.
   This function returns 1 if there is a window manager and the
   window was actually iconified, it returns 0 otherwise.
*/
int sdl_xynth_IconifyWindow (_THIS)
{
	return 0;
}

/* Grab or ungrab keyboard and mouse input */
SDL_GrabMode sdl_xynth_GrabInput (_THIS, SDL_GrabMode mode)
{
	return SDL_GRAB_OFF;
}

/* Get some platform dependent window information */
int sdl_xynth_GetWMInfo (_THIS, SDL_SysWMinfo *info)
{
	return 0;
}
