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

/* Set the gamma correction directly (emulated with gamma ramps) */
int sdl_xynth_SetGamma (_THIS, float red, float green, float blue)
{
	return 0;
}

/* Get the gamma correction directly (emulated with gamma ramps) */
int sdl_xynth_GetGamma (_THIS, float *red, float *green, float *blue)
{
	return 0;
}

/* Set the gamma ramp */
int sdl_xynth_SetGammaRamp (_THIS, Uint16 *ramp)
{
	return 0;
}

/* Get the gamma ramp */
int sdl_xynth_GetGammaRamp (_THIS, Uint16 *ramp)
{
	return 0;
}
