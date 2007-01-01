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

/* Sets the dll to use for OpenGL and loads it */
int sdl_xynth_GL_LoadLibrary (_THIS, const char *path)
{
	return 0;
}

/* Retrieves the address of a function in the gl library */
void * sdl_xynth_GL_GetProcAddress (_THIS, const char *proc)
{
	return NULL;
}

/* Get attribute information from the windowing system. */
int sdl_xynth_GL_GetAttribute (_THIS, SDL_GLattr attrib, int *value)
{
	return 0;
}

/* Make the context associated with this driver current */
int sdl_xynth_GL_MakeCurrent (_THIS)
{
	return 0;
}

/* Swap the current buffers in double buffer mode. */
void sdl_xynth_GL_SwapBuffers (_THIS)
{
}
