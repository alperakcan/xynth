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
#include "../helper/helper.h"
#include "sdl.h"

#if defined(SINGLE_APP)
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
	if (server->driver->driver_data == NULL) {
		return;
	}
	priv =  server->driver->driver_data;
#if defined(SINGLE_APP)
	s_free((void *) server->window->surface->linear_mem_base);
#else
	shmdt((void *) server->window->surface->linear_mem_base);
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
	server->driver->driver_data = NULL;
}


int s_video_sdl_server_init (s_server_conf_t *cfg)
{
        void *addr;
	s_video_sdl_data_t *priv;
	s_video_helper_mode_info_t *gmode;
	
	priv = (s_video_sdl_data_t *) s_malloc(sizeof(s_video_sdl_data_t));
	server->driver->driver_data = (void *) priv;
	
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
		server->window->surface->width = priv->screen->w;
		server->window->surface->height = priv->screen->h;
		server->window->surface->bytesperpixel = priv->screen->format->BytesPerPixel;
		server->window->surface->bitsperpixel = priv->screen->format->BitsPerPixel;
                server->window->surface->blueoffset = 0;
                server->window->surface->greenoffset = 0;
                server->window->surface->redoffset = 0;
                server->window->surface->bluelength = 0;
                server->window->surface->greenlength = 0;
                server->window->surface->redlength = 0;
                server->window->surface->colors = 0;
                switch (server->window->surface->bitsperpixel) {
			case 8:
				server->window->surface->colors = 256;
				server->window->surface->bitsperpixel = 8;
				server->window->surface->blueoffset = 0;
				server->window->surface->greenoffset = 3;
				server->window->surface->redoffset = 6;
				server->window->surface->bluelength = 3;
				server->window->surface->greenlength = 3;
				server->window->surface->redlength = 2;
				break;
			case 15:
				server->window->surface->colors = 32768;
				server->window->surface->bitsperpixel = 15;
				server->window->surface->blueoffset = 0;
				server->window->surface->greenoffset = 5;
				server->window->surface->redoffset = 10;
				server->window->surface->bluelength = 5;
				server->window->surface->greenlength = 5;
				server->window->surface->redlength = 5;
				break;
			case 16:
				server->window->surface->colors = 65536;
				server->window->surface->bitsperpixel = 16;
				server->window->surface->blueoffset = 0;
				server->window->surface->greenoffset = 5;
				server->window->surface->redoffset = 11;
				server->window->surface->bluelength = 5;
				server->window->surface->greenlength = 6;
				server->window->surface->redlength = 5;
				break;
			case 24:
			case 32:
				server->window->surface->colors = 256 * 65536;
				server->window->surface->bitsperpixel = server->window->surface->bytesperpixel * 8;
				server->window->surface->blueoffset = 0;
				server->window->surface->greenoffset = 8;
				server->window->surface->redoffset = 16;
				server->window->surface->bluelength = 8;
				server->window->surface->greenlength = 8;
				server->window->surface->redlength = 8;
				break;
		}
#if defined(SINGLE_APP)
		addr = (void *) s_malloc(sizeof(char) * server->window->surface->width * server->window->surface->height * server->window->surface->bytesperpixel);
#else
		if ((priv->screen_shm_mid = shmget(IPC_PRIVATE, sizeof(char) * server->window->surface->width * server->window->surface->height * server->window->surface->bytesperpixel, IPC_CREAT | 0644)) < 0) {
			debugf(DSER | DSYS, "Can not get id for shared memory");
			goto err1;
		}
		if ((addr = (void *) shmat(priv->screen_shm_mid, NULL, 0)) < 0) {
			debugf(DSER | DSYS, "Can not attach the shared memory");
			goto err1;
		}
		server->window->surface->shm_sid = priv->screen_shm_mid;
                shmctl(priv->screen_shm_mid, IPC_RMID, 0);
#endif
		server->window->surface->linear_mem_base = (unsigned int) addr;
		server->window->surface->linear_mem_size = (unsigned int) (sizeof(char) * server->window->surface->width * server->window->surface->height * server->window->surface->bytesperpixel);
		priv->screen->pixels = (char *) addr;
		server->window->surface->vbuf = (unsigned char *) addr;
		server->window->surface->linear_buf = (unsigned char *) addr;

		server->window->surface->need_expose = SURFACE_NEEDEXPOSE;

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
	s_video_sdl_data_t *priv = server->driver->driver_data;
	SDL_UpdateRect(priv->screen, coor->x, coor->y, coor->w, coor->h);
}

void s_video_sdl_server_fullscreen (void)
{
	s_video_sdl_data_t *priv = server->driver->driver_data;
	SDL_WM_ToggleFullScreen(priv->screen);
}

#endif /* VIDEO_SDL */
