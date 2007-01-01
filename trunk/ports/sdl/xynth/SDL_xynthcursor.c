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

#if 0
	#define debugf(a) printf(a);
#else
	#define debugf(a)
#endif

/* Free a window manager cursor
   This function can be NULL if CreateWMCursor is also NULL.
 */
void sdl_xynth_FreeWMCursor (_THIS, WMcursor *cursor)
{
	s_free(cursor);
	debugf("sdl_xynth_FreeWMCursor\n");
}

/* If not NULL, create a black/white window manager cursor */
WMcursor * sdl_xynth_CreateWMCursor (_THIS, Uint8 *data, Uint8 *mask, int w, int h, int hot_x, int hot_y)
{
	WMcursor *cursor = (WMcursor *) s_malloc(sizeof(WMcursor));
	debugf("sdl_xynth_CreateWMCursor\n");
	return cursor;
}

/* Show the specified cursor, or hide if cursor is NULL */
int sdl_xynth_ShowWMCursor (_THIS, WMcursor *cursor)
{
	debugf("sdl_xynth_ShowWMCursor\n");
	return 1;
}

/* Warp the window manager cursor to (x,y)
   If NULL, a mouse motion event is posted internally.
 */
void sdl_xynth_WarpWMCursor (_THIS, Uint16 x, Uint16 y)
{
	debugf("sdl_xynth_WarpWMCursor\n");
}

/* If not NULL, this is called when a mouse motion event occurs */
void sdl_xynth_MoveWMCursor (_THIS, int x, int y)
{
	debugf("sdl_xynth_MoveWMCursor\n");
}

/* Determine whether the mouse should be in relative mode or not.
   This function is called when the input grab state or cursor
   visibility state changes.
   If the cursor is not visible, and the input is grabbed, the
   driver can place the mouse in relative mode, which may result
   in higher accuracy sampling of the pointer motion.
*/
void sdl_xynth_CheckMouseMode (_THIS)
{
	debugf("sdl_xynth_CheckMouseMode\n");
}
