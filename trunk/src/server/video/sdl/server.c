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
#include "../helper/helper.h"
#include "sdl.h"

#if defined(CONFIG_SINGLE_APPLICATION)
#else
	#include <sys/shm.h>
#endif

s_video_input_t s_video_sdl_input_keybd = {
	VIDEO_INPUT_KEYBD,
	s_video_sdl_kbd_init,
	s_video_sdl_kbd_update,
	s_video_sdl_kbd_uninit,
};

s_video_input_t s_video_sdl_input_mouse = {
	VIDEO_INPUT_MOUSE,
	s_video_sdl_mouse_init,
	s_video_sdl_mouse_update,
	s_video_sdl_mouse_uninit,
};

s_video_input_t s_video_sdl_input_irr = {
	VIDEO_INPUT_IRR,
	s_video_helper_irr_init,
	s_video_helper_irr_update,
	s_video_helper_irr_uninit,
};


s_video_input_t *s_video_sdl_input[] = {
	&s_video_sdl_input_keybd,
	&s_video_sdl_input_mouse,
	&s_video_sdl_input_irr,
	NULL,
};

s_video_driver_t s_video_sdl = {
	"sdl",
	"NULL",
	s_video_sdl_input,
	s_video_sdl_server_init,
	s_video_sdl_server_uninit,
	NULL,
	NULL,
	NULL,
	s_video_sdl_server_surface_update,
	s_video_sdl_server_fullscreen,
	NULL
};

void s_video_sdl_server_uninit (void)
{
	SDL_Event event;
	s_video_sdl_data_t *priv;
	if (xynth_server->driver->driver_data == NULL) {
		return;
	}
	priv =  xynth_server->driver->driver_data;
#if defined(CONFIG_SINGLE_APPLICATION)
	s_free((void *) xynth_server->window->surface->linear_mem_base);
#else
	shmdt((void *) xynth_server->window->surface->linear_mem_base);
#endif
	priv->screen->pixels = (char *) s_malloc(1);
	event.type = SDL_QUIT;
	SDL_PushEvent(&event);
	s_thread_join(priv->event_tid, NULL);
	if(SDL_WasInit(SDL_INIT_VIDEO)) {
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
	}
	SDL_Quit();
	s_free(priv);
	xynth_server->driver->driver_data = NULL;
}


int s_video_sdl_server_init (s_server_conf_t *cfg)
{
        void *addr;
	s_video_sdl_data_t *priv;
	s_video_helper_mode_info_t *gmode;
	
	priv = (s_video_sdl_data_t *) s_malloc(sizeof(s_video_sdl_data_t));
	xynth_server->driver->driver_data = (void *) priv;
	
        priv->mouse_fd[0] = -1;
        priv->mouse_fd[1] = -1;
        priv->keybd_fd[0] = -1;
        priv->keybd_fd[1] = -1;
        
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		debugf(DSER, "Couldn't initialize SDL: %s", SDL_GetError());
		goto err0;
	}
	if (s_video_helper_mode_find(cfg, &gmode)) {
		debugf(DSER, "Couldn't find mode: %s", cfg->general.mode);
		goto err0;
	}
	priv->screen = SDL_SetVideoMode(gmode->xdim, gmode->ydim, gmode->bytesperpixel * 8, SDL_SWSURFACE);
	if (priv->screen == NULL) {
		debugf(DSER, "Couldn't set %dx%dx%d video mode: %s", gmode->xdim, gmode->ydim, gmode->bytesperpixel, SDL_GetError());
		goto err1;
	} else {
		xynth_server->window->surface->width = priv->screen->w;
		xynth_server->window->surface->height = priv->screen->h;
		xynth_server->window->surface->bytesperpixel = priv->screen->format->BytesPerPixel;
		xynth_server->window->surface->bitsperpixel = priv->screen->format->BitsPerPixel;
                xynth_server->window->surface->blueoffset = 0;
                xynth_server->window->surface->greenoffset = 0;
                xynth_server->window->surface->redoffset = 0;
                xynth_server->window->surface->bluelength = 0;
                xynth_server->window->surface->greenlength = 0;
                xynth_server->window->surface->redlength = 0;
                xynth_server->window->surface->colors = 0;
                switch (xynth_server->window->surface->bitsperpixel) {
			case 8:
				xynth_server->window->surface->colors = 256;
				xynth_server->window->surface->bitsperpixel = 8;
				xynth_server->window->surface->blueoffset = 0;
				xynth_server->window->surface->greenoffset = 3;
				xynth_server->window->surface->redoffset = 6;
				xynth_server->window->surface->bluelength = 3;
				xynth_server->window->surface->greenlength = 3;
				xynth_server->window->surface->redlength = 2;
				break;
			case 15:
				xynth_server->window->surface->colors = 32768;
				xynth_server->window->surface->bitsperpixel = 15;
				xynth_server->window->surface->blueoffset = 0;
				xynth_server->window->surface->greenoffset = 5;
				xynth_server->window->surface->redoffset = 10;
				xynth_server->window->surface->bluelength = 5;
				xynth_server->window->surface->greenlength = 5;
				xynth_server->window->surface->redlength = 5;
				break;
			case 16:
				xynth_server->window->surface->colors = 65536;
				xynth_server->window->surface->bitsperpixel = 16;
				xynth_server->window->surface->blueoffset = 0;
				xynth_server->window->surface->greenoffset = 5;
				xynth_server->window->surface->redoffset = 11;
				xynth_server->window->surface->bluelength = 5;
				xynth_server->window->surface->greenlength = 6;
				xynth_server->window->surface->redlength = 5;
				break;
			case 24:
			case 32:
				xynth_server->window->surface->colors = 256 * 65536;
				xynth_server->window->surface->bitsperpixel = xynth_server->window->surface->bytesperpixel * 8;
				xynth_server->window->surface->blueoffset = 0;
				xynth_server->window->surface->greenoffset = 8;
				xynth_server->window->surface->redoffset = 16;
				xynth_server->window->surface->bluelength = 8;
				xynth_server->window->surface->greenlength = 8;
				xynth_server->window->surface->redlength = 8;
				break;
		}
#if defined(CONFIG_SINGLE_APPLICATION)
		addr = (void *) s_malloc(sizeof(char) * xynth_server->window->surface->width * xynth_server->window->surface->height * xynth_server->window->surface->bytesperpixel);
#else
		if ((priv->screen_shm_mid = shmget(IPC_PRIVATE, sizeof(char) * xynth_server->window->surface->width * xynth_server->window->surface->height * xynth_server->window->surface->bytesperpixel, IPC_CREAT | 0644)) < 0) {
			debugf(DSER | DSYS, "Can not get id for shared memory");
			goto err1;
		}
		if ((addr = (void *) shmat(priv->screen_shm_mid, NULL, 0)) < 0) {
			debugf(DSER | DSYS, "Can not attach the shared memory");
			goto err1;
		}
		xynth_server->window->surface->shm_sid = priv->screen_shm_mid;
                shmctl(priv->screen_shm_mid, IPC_RMID, 0);
#endif
		xynth_server->window->surface->linear_mem_base = (unsigned int) addr;
		xynth_server->window->surface->linear_mem_size = (unsigned int) (sizeof(char) * xynth_server->window->surface->width * xynth_server->window->surface->height * xynth_server->window->surface->bytesperpixel);
		priv->screen->pixels = (char *) addr;
		xynth_server->window->surface->vbuf = (unsigned char *) addr;
		xynth_server->window->surface->linear_buf = (unsigned char *) addr;

		xynth_server->window->surface->need_expose = SURFACE_NEEDEXPOSE;

		SDL_WM_SetCaption("Xynth Windowing system (video driver = sdl)", NULL);
		priv->event_tid = s_thread_create(&s_video_sdl_event_parse, (void *) NULL);
		return gmode->number;
	}

err1:	SDL_Quit();
err0:	s_free(priv);
	return -1;
}

void s_video_sdl_server_surface_update (s_rect_t *coor)
{
	s_video_sdl_data_t *priv = xynth_server->driver->driver_data;
	SDL_UpdateRect(priv->screen, coor->x, coor->y, coor->w, coor->h);
}

void s_video_sdl_server_fullscreen (void)
{
	s_video_sdl_data_t *priv = xynth_server->driver->driver_data;
	SDL_WM_ToggleFullScreen(priv->screen);
}
