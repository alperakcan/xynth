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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SDL_xynth.h"

#define xynthVID_DRIVER_NAME "xynth"

VideoBootStrap XYNTH_bootstrap = {
	xynthVID_DRIVER_NAME, "SDL xynth video driver",
	sdl_xynth_Available, sdl_xynth_CreateDevice
};

int sdl_xynth_Available(void)
{
	const char *envr = getenv("SDL_VIDEODRIVER");
	if ((envr) && (strcmp(envr, xynthVID_DRIVER_NAME) == 0)) {
		return(1);
	}

	return(0);
}

void sdl_xynth_DeleteDevice(SDL_VideoDevice *device)
{
	s_free(device->hidden);
	s_free(device);
}

SDL_VideoDevice *sdl_xynth_CreateDevice(int devindex)
{
	SDL_VideoDevice *device;

	/* Initialize all variables that we clean on shutdown */
	device = (SDL_VideoDevice *) s_malloc(sizeof(SDL_VideoDevice));
	if (device) {
		memset(device, 0, (sizeof *device));
		device->hidden = (struct SDL_PrivateVideoData *) s_malloc((sizeof *device->hidden));
	}
	if ((device == NULL) || (device->hidden == NULL)) {
		SDL_OutOfMemory();
		if (device) {
			s_free(device);
		}
		return(0);
	}
	memset(device->hidden, 0, (sizeof *device->hidden));

	device->VideoInit = sdl_xynth_VideoInit;
	device->ListModes = sdl_xynth_ListModes;
	device->SetVideoMode = sdl_xynth_SetVideoMode;
	device->ToggleFullScreen = sdl_xynth_ToggleFullScreen;
	device->UpdateMouse = sdl_xynth_UpdateMouse;
	device->CreateYUVOverlay = sdl_xynth_CreateYUVOverlay;
	device->SetColors = sdl_xynth_SetColors;
	device->UpdateRects = sdl_xynth_UpdateRects;
	device->VideoQuit = sdl_xynth_VideoQuit;

	device->AllocHWSurface = sdl_xynth_AllocHWSurface;
	device->CheckHWBlit = NULL;
	device->FillHWRect = NULL;
	device->SetHWColorKey = NULL;
	device->SetHWAlpha = NULL;
	device->LockHWSurface = sdl_xynth_LockHWSurface;
	device->UnlockHWSurface = sdl_xynth_UnlockHWSurface;
	device->FlipHWSurface = NULL;
	device->FreeHWSurface = sdl_xynth_FreeHWSurface;

	device->SetGamma = NULL;
	device->GetGamma = NULL;
	device->SetGammaRamp = NULL;
	device->GetGammaRamp = NULL;

	device->GL_LoadLibrary = NULL;
	device->GL_GetProcAddress = NULL;
	device->GL_GetAttribute = NULL;
	device->GL_MakeCurrent = NULL;
	device->GL_SwapBuffers = NULL;
	
	device->SetCaption = sdl_xynth_SetCaption;
	device->SetIcon = NULL;
	device->IconifyWindow = NULL;
	device->GrabInput = NULL;
	device->GetWMInfo = NULL;

	device->FreeWMCursor = sdl_xynth_FreeWMCursor;
	device->CreateWMCursor = sdl_xynth_CreateWMCursor;
	device->ShowWMCursor = sdl_xynth_ShowWMCursor;
	device->WarpWMCursor = sdl_xynth_WarpWMCursor;
	device->MoveWMCursor = sdl_xynth_MoveWMCursor;
	device->CheckMouseMode = sdl_xynth_CheckMouseMode;	
	
	device->InitOSKeymap = sdl_xynth_InitOSKeymap;
	device->PumpEvents = sdl_xynth_PumpEvents;

	device->free = sdl_xynth_DeleteDevice;

	return device;
}

/* Initialize the native video subsystem, filling 'vformat' with the
   "best" display pixel format, returning 0 or -1 if there's an error.
 */
int sdl_xynth_VideoInit(_THIS, SDL_PixelFormat *vformat)
{
	if (s_window_init(&this->hidden->window)) {
		fprintf(stderr, "Could not connect to Xynth!\n");
		exit(1);
	}

	vformat->BitsPerPixel = this->hidden->window->surface->bitsperpixel;
	vformat->BytesPerPixel = this->hidden->window->surface->bytesperpixel;;

	this->info.wm_available = 1;

	return 0;
}

/* List the available video modes for the given pixel format, sorted
   from largest to smallest.
 */
SDL_Rect **sdl_xynth_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags)
{
   	 return (SDL_Rect **) -1;
}

/* Set the requested video mode, returning a surface which will be
   set to the SDL_VideoSurface.  The width and height will already
   be verified by ListModes(), and the video subsystem is free to
   set the mode to a supported bit depth different from the one
   specified -- the desired bpp will be emulated with a shadow
   surface if necessary.  If a new mode is returned, this function
   should take care of cleaning up the current mode.
 */
SDL_Surface * sdl_xynth_SetVideoMode(_THIS, SDL_Surface *current, int width, int height, int bpp, Uint32 flags)
{
	if (this->hidden->buffer) {
		s_free(this->hidden->buffer);
	}

        bpp = this->hidden->window->surface->bitsperpixel;
        if (bpp == 15) {
		bpp = 16;
	}
        
	this->hidden->buffer = s_malloc(width * height * (bpp / 8));
        this->hidden->bpp = bpp;
	memset(this->hidden->buffer, 0, width * height * (bpp / 8));

	if (!SDL_ReallocFormat(current, bpp, 0, 0, 0, 0)) {
		s_free(this->hidden->buffer);
		this->hidden->buffer = NULL;
		SDL_SetError("Couldn't allocate new pixel format for requested mode");
		return NULL;
	}

	current->flags = flags & SDL_FULLSCREEN;
	this->hidden->w = current->w = width;
	this->hidden->h = current->h = height;
	current->pitch = current->w * (bpp / 8);
	current->pixels = this->hidden->buffer;

	s_window_new(this->hidden->window, WINDOW_MAIN, NULL);
	s_window_set_coor(this->hidden->window, 0, 0, 0, width, height);
	s_window_set_coor(this->hidden->window, WINDOW_NOFORM, this->hidden->window->surface->buf->x, this->hidden->window->surface->buf->y, width, height);
	s_window_set_title(this->hidden->window, "Xynth - SDL");
	s_window_show(this->hidden->window);

	s_window_atexit(this->hidden->window, sdl_xynth_atexit);
	s_window_atevent(this->hidden->window, sdl_xynth_atevent);

	this->hidden->tid = s_thread_create((void * (*) (void *)) &s_window_main, this->hidden->window);
	sdl_xynth_running = 1;

	return current ;
}

/* Toggle the fullscreen mode */
int sdl_xynth_ToggleFullScreen (_THIS, int on)
{
	return -1;
}

/* This is called after the video mode has been set, to get the
   initial mouse state.  It should queue events as necessary to
   properly represent the current mouse focus and position.
 */
void sdl_xynth_UpdateMouse (_THIS)
{
}

/* Create a YUV video surface (possibly overlay) of the given
   format.  The hardware should be able to perform at least 2x
   scaling on display.
 */
SDL_Overlay * sdl_xynth_CreateYUVOverlay (_THIS, int width, int height, Uint32 format, SDL_Surface *display)
{
	return NULL;
}

/* Sets the color entries { firstcolor .. (firstcolor+ncolors-1) }
   of the physical palette to those in 'colors'. If the device is
   using a software palette (SDL_HWPALETTE not set), then the
   changes are reflected in the logical palette of the screen
   as well.
   The return value is 1 if all entries could be set properly
   or 0 otherwise.
 */
int sdl_xynth_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors)
{
	return 1;
}

/* This pointer should exist in the native video subsystem and should
   point to an appropriate update function for the current video mode
 */
void sdl_xynth_UpdateRects(_THIS, int numrects, SDL_Rect *rects)
{
        char *buf;
        int n = numrects;
        SDL_Rect *r = rects;

        if (!sdl_xynth_running) {
		return;
	}
	
	sdl_xynth_running_ = 1;
	
        if (this->hidden->bpp != this->hidden->window->surface->bitsperpixel) {
		s_copybuffer(this->hidden->buffer, this->hidden->bpp, &buf, this->hidden->window->surface->bitsperpixel, this->hidden->w, this->hidden->h);
	        while (n--) {
			s_putboxpart(this->hidden->window->surface, r->x, r->y, r->w, r->h, this->hidden->w, this->hidden->h, buf, r->x, r->y);
			r++;
		}
		s_free(buf);
	} else {
		while (n--) {
			s_putboxpart(this->hidden->window->surface, r->x, r->y, r->w, r->h, this->hidden->w, this->hidden->h, this->hidden->buffer, r->x, r->y);
			r++;
		}
	}
	
	sdl_xynth_running_ = 0;
}

/* Reverse the effects VideoInit() -- called if VideoInit() fails
   or if the application is shutting down the video subsystem.
 */
void sdl_xynth_VideoQuit(_THIS)
{
	while (sdl_xynth_running_) {
		usleep(20000);
	}
	
	if (this->screen->pixels != NULL)
	{
		s_free(this->screen->pixels);
		this->screen->pixels = NULL;
	}
	
	if (sdl_xynth_running) {
		s_window_quit(this->hidden->window);
		s_thread_join(this->hidden->tid, NULL);
	}
	sdl_xynth_running = 0;
}
