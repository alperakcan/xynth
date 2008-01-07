/***************************************************************************
    begin                : Mon Feb 17 2003
    copyright            : (C) 2003 - 2008 by Alper Akcan
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

#include "xynth_.h"

#if !defined(CONFIG_SINGLE_APPLICATION)
	#include <sys/shm.h>
	#include <sys/mman.h>
	#include <fcntl.h>
#endif

void s_surface_attach_matrix (s_window_t *window)
{
	void *addr;
	
	if (window->surface->need_expose & SURFACE_NEEDSTREAM) {
		return;
	}
        if (window->type & (WINDOW_TEMP | WINDOW_CHILD)) {
		window->surface->matrix = window->parent->surface->matrix;
		return;
	}
#if defined(CONFIG_SINGLE_APPLICATION)
	addr = (void *) window->surface->matrix;
#else
        if ((addr = (void *) shmat(window->surface->shm_mid, NULL, SHM_RDONLY)) < 0) {
		debugf(DCLI | DFAT | DSYS, "[%d] Can not attach the shared memory", window->id);
        }
#endif
        window->surface->matrix = (unsigned char *) addr;
}

void s_surface_attach_buffer (s_window_t *window)
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
#if defined(CONFIG_SINGLE_APPLICATION)
	addr = (void *) window->surface->linear_buf;
#else
	if (window->surface->need_expose & SURFACE_NEEDEXPOSE) {
		if ((addr = (void *) shmat(window->surface->shm_sid, NULL, 0)) < 0) {
			debugf(DCLI | DFAT | DSYS, "[%d] Can not attach the shared memory", window->id);
		}
		window->surface->linear_buf = (unsigned char *) addr;
	} else {
		fd = open(window->surface->device, O_RDWR);
		if (fd < 0) {
			debugf(DCLI | DSYS | DFAT, "[%d] open(%s, O_RDWR) failed", window->surface->device);
		}
		if (window->surface->linear_mem_size){
			addr = (void *) mmap((caddr_t) 0, window->surface->linear_mem_size,
			                                  PROT_WRITE | PROT_READ,
						          MAP_SHARED,
						          fd,
						          (off_t) window->surface->linear_mem_base);
			if (addr == MAP_FAILED) {
				debugf(DCLI | DFAT | DSYS, "[%d] mmap failed", window->id);
			}
		} else {
			debugf(DCLI | DFAT, "[%d] mmap failed", window->id);
		}
		close(fd);
	}
#endif
	window->surface->linear_buf = (unsigned char *) addr;
}

int s_surface_attach (s_window_t *window)
{
	window->surface->mode = SURFACE_VIRTUAL;
//	window->surface->need_expose = SURFACE_NEEDSTREAM;
	if (window->surface->need_expose & SURFACE_NEEDSTREAM) {
		return 0;
	}
	s_surface_attach_matrix(window);
       	s_surface_attach_buffer(window);
       	return 0;
}

int s_surface_init (s_window_t *window)
{
	window->surface = (s_surface_t *) s_calloc(1, sizeof(s_surface_t));
	window->surface->buf = (s_rect_t *) s_calloc(1, sizeof(s_rect_t));
	window->surface->win = (s_rect_t *) s_calloc(1, sizeof(s_rect_t));
	window->surface->window = window;
       	return 0;
}

void s_surface_uninit (s_window_t *window)
{
	if (window->surface == NULL) {
		return;
	}
#if defined(CONFIG_SINGLE_APPLICATION)
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
        s_free(window->surface->device);
	s_free(window->surface);
	window->surface = NULL;
}

int s_surface_clip_virtual (s_surface_t *surface, int x, int y, int w, int h, s_rect_t *coor)
{
	s_rect_t thip;
	s_rect_t clip;

	thip.x = x;
	thip.y = y;
	thip.w = w;
	thip.h = h;

	clip.x = 0;
	clip.y = 0;
	clip.w = surface->width;
	clip.h = surface->height;

	if (s_region_rect_intersect(&thip, &clip, coor)) {
		return -1;
	}
	return 0;
}

int s_surface_clip_real (s_surface_t *surface, int x, int y, int w, int h, s_rect_t *coor)
{
        s_rect_t that;
	s_rect_t thip;
	s_rect_t cliv;
	s_rect_t clir;
	
        thip.x = x;
	thip.y = y;
	thip.w = w;
	thip.h = h;

	cliv.x = 0;
	cliv.y = 0;
	cliv.w = MIN(surface->buf->w, surface->width);
	cliv.h = MIN(surface->buf->h, surface->height);

	clir.x = 0;
	clir.y = 0;
	clir.w = surface->linear_buf_width;
	clir.h = surface->linear_buf_height;

	if (s_region_rect_intersect(&thip, &cliv, &that)) {
		return -1;
	}
	that.x += surface->buf->x;
	that.y += surface->buf->y;
	if (s_region_rect_intersect(&clir, &that, coor)) {
		return -1;
	}
	coor->x -= surface->buf->x;
	coor->y -= surface->buf->y;
	return 0;
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
	if (s_surface_clip_real(window->surface, x, y, changed->w, changed->h, &coor)) {
		return;
	}

	if (window->surface->need_expose & SURFACE_NEEDSTREAM) {
		s_socket_request(window, SOC_DATA_EXPOSE, &coor);
	} else {
		if (window->surface->mode & SURFACE_VIRTUAL) {
			bpp_putbox_o(window->surface, window->id, coor.x, coor.y, coor.w, coor.h,
			             window->surface->vbuf + ((coor.y * window->surface->width) + coor.x) * window->surface->bytesperpixel,
			             window->surface->width);
		} else {
			bpp_fillbox_o(window->surface, window->id, coor.x, coor.y, coor.w, coor.h, 0);
		}
		if (window->surface->need_expose & SURFACE_NEEDEXPOSE) {
			s_socket_request(window, SOC_DATA_EXPOSE, &coor);
		}
	}
}
