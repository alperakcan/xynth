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

#ifndef SDL_xynth_h
#define SDL_xynth_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <xynth.h>

#include "SDL.h"
#include "SDL_error.h"
#include "SDL_mouse.h"
#include "SDL_mutex.h"
#include "../SDL_sysvideo.h"
#include "../../events/SDL_sysevents.h"
#include "../../events/SDL_events_c.h"

#define _THIS	SDL_VideoDevice *this

int sdl_xynth_running;
int sdl_xynth_running_;

struct WMcursor {
	int unused;
};

struct SDL_PrivateVideoData {
    int w;
    int h;
    int bpp;
    void *buffer;
    s_thread_t *tid;
    s_window_t *window;
};

/* cursor.c */
void sdl_xynth_FreeWMCursor (_THIS, WMcursor *cursor);
WMcursor * sdl_xynth_CreateWMCursor (_THIS, Uint8 *data, Uint8 *mask, int w, int h, int hot_x, int hot_y);
int sdl_xynth_ShowWMCursor (_THIS, WMcursor *cursor);
void sdl_xynth_WarpWMCursor (_THIS, Uint16 x, Uint16 y);
void sdl_xynth_MoveWMCursor (_THIS, int x, int y);
void sdl_xynth_CheckMouseMode (_THIS);

/* event.c */
void sdl_xynth_atexit (s_window_t *window);
void sdl_xynth_atevent (s_window_t *window, s_event_t *event);
void sdl_xynth_InitOSKeymap (_THIS);
void sdl_xynth_PumpEvents (_THIS);
SDL_keysym * sdl_xynth_translatekey(s_event_t *event, SDL_keysym *keysym);

/* gamma.c */
int sdl_xynth_SetGamma (_THIS, float red, float green, float blue);
int sdl_xynth_GetGamma (_THIS, float *red, float *green, float *blue);
int sdl_xynth_SetGammaRamp (_THIS, Uint16 *ramp);
int sdl_xynth_GetGammaRamp (_THIS, Uint16 *ramp);

/* gl.c */
int sdl_xynth_GL_LoadLibrary (_THIS, const char *path);
void* sdl_xynth_GL_GetProcAddress (_THIS, const char *proc);
int sdl_xynth_GL_GetAttribute (_THIS, SDL_GLattr attrib, int* value);
int sdl_xynth_GL_MakeCurrent (_THIS);
void sdl_xynth_GL_SwapBuffers (_THIS);

/* hw.c */
int sdl_xynth_AllocHWSurface (_THIS, SDL_Surface *surface);
int sdl_xynth_CheckHWBlit (_THIS, SDL_Surface *src, SDL_Surface *dst);
int sdl_xynth_FillHWRect (_THIS, SDL_Surface *dst, SDL_Rect *rect, Uint32 color);
int sdl_xynth_SetHWColorKey (_THIS, SDL_Surface *surface, Uint32 key);
int sdl_xynth_SetHWAlpha (_THIS, SDL_Surface *surface, Uint8 value);
int sdl_xynth_LockHWSurface (_THIS, SDL_Surface *surface);
void sdl_xynth_UnlockHWSurface (_THIS, SDL_Surface *surface);
int sdl_xynth_FlipHWSurface (_THIS, SDL_Surface *surface);
void sdl_xynth_FreeHWSurface (_THIS, SDL_Surface *surface);

/* video.c */
int sdl_xynth_Available(void);
void sdl_xynth_DeleteDevice(SDL_VideoDevice *device);
SDL_VideoDevice *sdl_xynth_CreateDevice(int devindex);
int sdl_xynth_VideoInit (_THIS, SDL_PixelFormat *vformat);
SDL_Rect ** sdl_xynth_ListModes (_THIS, SDL_PixelFormat *format, Uint32 flags);
SDL_Surface * sdl_xynth_SetVideoMode (_THIS, SDL_Surface *current, int width, int height, int bpp, Uint32 flags);
int sdl_xynth_ToggleFullScreen (_THIS, int on);
void sdl_xynth_UpdateMouse (_THIS);
SDL_Overlay * sdl_xynth_CreateYUVOverlay (_THIS, int width, int height, Uint32 format, SDL_Surface *display);
int sdl_xynth_SetColors (_THIS, int firstcolor, int ncolors, SDL_Color *colors);
void sdl_xynth_UpdateRects (_THIS, int numrects, SDL_Rect *rects);
void sdl_xynth_VideoQuit (_THIS);

/* wm.c */
void sdl_xynth_SetCaption (_THIS, const char *title, const char *icon);
void sdl_xynth_SetIcon (_THIS, SDL_Surface *icon, Uint8 *mask);
int sdl_xynth_IconifyWindow (_THIS);
SDL_GrabMode sdl_xynth_GrabInput (_THIS, SDL_GrabMode mode);
int sdl_xynth_GetWMInfo (_THIS, SDL_SysWMinfo *info);

#endif /* SDL_xynth_h */
