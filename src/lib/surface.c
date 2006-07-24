/***************************************************************************
    begin                : Mon Feb 17 2003
    copyright            : (C) 2003 - 2006 by Alper Akcan
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

#if !defined(SINGLE_APP)
	#include <sys/shm.h>
	#include <sys/mman.h>
	#include <fcntl.h>
#endif

#include "xynth_.h"

int s_surface_init (s_window_t *window)
{
	window->surface = (s_surface_t *) s_calloc(1, sizeof(s_surface_t));
	window->surface->buf = (s_rect_t *) s_calloc(1, sizeof(s_rect_t));
	window->surface->win = (s_rect_t *) s_calloc(1, sizeof(s_rect_t));
	window->surface->id = &(window->client->id);
	window->surface->window = window;
	return 0;
}

void s_surface_create (s_window_t *window)
{
	window->surface->vbuf = (char *) s_calloc(sizeof(char), window->surface->width *
	                                                        window->surface->height *
	                                                        window->surface->bytesperpixel + 1);
	window->surface->mode = SURFACE_VIRTUAL;
        if (window->surface->need_expose & SURFACE_NEEDSTREAM) {
		return;
	}
	s_surface_shm_attach(window);
       	s_surface_linear(window);
}

void s_surface_shm_attach (s_window_t *window)
{
	void *addr;
	
	if (window->surface->need_expose & SURFACE_NEEDSTREAM) {
		return;
	}
        if (window->type & (WINDOW_TEMP | WINDOW_CHILD)) {
		window->surface->matrix = window->parent->surface->matrix;
		return;
	}
#if defined(SINGLE_APP)
	addr = (void *) window->surface->matrix;
#else
        if ((addr = (void *) shmat(window->surface->shm_mid, NULL, SHM_RDONLY)) < 0) {
		debugf(DCLI | DFAT | DSYS, "[%d] Can not attach the shared memory", window->client->id);
        }
#endif
        window->surface->matrix = (unsigned char *) addr;
}

void s_surface_linear (s_window_t *window)
{
	int fd;
	void *addr;
	
	fd = -1;
	addr = NULL;
	
	if (window->surface->need_expose & SURFACE_NEEDSTREAM) {
		return;
	}
	if (window->type & (WINDOW_TEMP | WINDOW_CHILD)) {
		window->surface->linear_buf = window->parent->surface->linear_buf;
		return;
	}
#if defined(SINGLE_APP)
	addr = (void *) window->surface->linear_buf;
#else
	if (window->surface->need_expose & SURFACE_NEEDEXPOSE) {
		if ((addr = (void *) shmat(window->surface->shm_sid, NULL, 0)) < 0) {
			debugf(DCLI | DFAT | DSYS, "[%d] Can not attach the shared memory", window->client->id);
		}
		window->surface->linear_buf = (char *) addr;
	} else {
		fd = open(window->client->device, O_RDWR);
		if (fd < 0) {
			debugf(DCLI | DSYS | DFAT, "[%d] open(%s, O_RDWR) failed", window->client->device);
		}
		if (window->surface->linear_mem_size){
			addr = (void *) mmap((caddr_t) 0, window->surface->linear_mem_size,
			                                  PROT_WRITE | PROT_READ,
						          MAP_SHARED,
						          fd,
						          (off_t) window->surface->linear_mem_base);
			if (addr == MAP_FAILED) {
				debugf(DCLI | DFAT | DSYS, "[%d] mmap failed", window->client->id);
			}
		} else {
			debugf(DCLI | DFAT, "[%d] mmap failed", window->client->id);
		}
		close(fd);
	}
#endif
	window->surface->linear_buf = (char *) addr;
}

void s_surface_uninit (s_window_t *window)
{
#if defined(SINGLE_APP)
#else
	if ((window->type & WINDOW_MAIN) &&
	    (!(window->surface->need_expose & SURFACE_NEEDSTREAM))) {
		if (window->surface->need_expose & SURFACE_NEEDEXPOSE) {
			shmdt(window->surface->linear_buf);
		} else {
			munmap(window->surface->linear_buf, window->surface->linear_mem_size);
		}
		shmdt(window->surface->matrix);
	}
#endif
	s_free(window->surface->buf);
	s_free(window->surface->win);
	s_free(window->surface->vbuf);
	s_free(window->surface);
	window->surface = NULL;
}

void s_surface_changed (s_window_t *window, s_rect_t *changed)
{
	int x;
	int y;
	s_rect_t coor;

	if (!(window->surface->mode & SURFACE_REAL)) {
		return;
	}
	
	x = changed->x - window->surface->buf->x;
	y = changed->y - window->surface->buf->y;
	if (s_rect_clip_real(window->surface, x, y, changed->w, changed->h, &coor)) {
		return;
	}

	if (window->surface->need_expose & SURFACE_NEEDSTREAM) {
		s_socket_request(window, SOC_DATA_EXPOSE, &coor);
	} else {
		bpp_putbox_o(window->surface, window->client->id, coor.x, coor.y, coor.w, coor.h,
		             window->surface->vbuf + ((coor.y * window->surface->width) + coor.x) * window->surface->bytesperpixel,
		             window->surface->width);
		if (window->surface->need_expose & SURFACE_NEEDEXPOSE) {
			s_socket_request(window, SOC_DATA_EXPOSE, &coor);
		}
	}
}
