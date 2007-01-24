/***************************************************************************
    begin                : Thr Dec 15 2005
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

#include <stdio.h>
#include <stdlib.h>
#include "xynth_.h"

static void xynthlogout_atevent (s_window_t *window, s_event_t *event)
{
	/* never loose focus */
	if (event->type & FOCUS_EVENT) {
		if (window->pri > 0) {
			s_window_show(window);
		}
	}
}

static void xynthlogout_cancel (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_window_quit(window);
}

static void xynthlogout_logout (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_socket_request(window, SOC_DATA_CLOSE_SERVER);
}

int main (int argc, char *argv[])
{
	int x;
	int y;
	int w;
	int h;
	s_font_t *font;
	s_handler_t *hndl;
	s_window_t *window;

	s_window_init(&window);

	s_window_new(window, WINDOW_MAIN | WINDOW_NOFORM | WINDOW_DESKTOP, NULL);

	s_window_set_resizeable(window, 0);
	s_window_set_alwaysontop(window, 1);
	s_window_set_title(window, "Xynth logout");

	w = 300;
	h = 100;
	x = (window->surface->width - w) / 2;
	y = (window->surface->height - h) / 2;
	s_window_set_coor(window, WINDOW_NOFORM, x, y, w, h);

	s_free(window->surface->vbuf);
	window->surface->width = w;
	window->surface->height = h;
	window->surface->vbuf = (unsigned char *) malloc(w * h * window->surface->bytesperpixel);

	s_window_atevent(window, xynthlogout_atevent);

	s_fillbox(window->surface, 0, 0, w, h, s_rgbcolor(window->surface, 0, 0, 0));
	for (y = 2; y < h - 2; y++) {
		s_fillbox(window->surface, 2, y, w - 4, 1, s_rgbcolor(window->surface, 255 - (int) ((float) y * 255.00 / 100.00),
		                                                                       255 - (int) ((float) y * 255.00 / 100.00),
		                                                                       255));
	}

	s_font_init(&font, "arial.ttf");
	s_font_set_str(font, "Close Xynth Windowing System...");
	s_font_set_size(font, 16);
	s_font_get_glyph(font);
	s_image_get_handler(font->glyph.img);
	s_putboxrgba(window->surface, 18, 13, font->glyph.img->w, font->glyph.img->h, font->glyph.img->rgba);

	s_fillbox(window->surface, 18, 15 + font->glyph.img->h + 2, font->glyph.img->w, 2, 0);
	s_font_uninit(font);

	s_fillbox(window->surface, 40, 48, 35, 35, s_rgbcolor(window->surface, 0, 0, 0));
	s_fillbox(window->surface, 42, 50, 31, 31, s_rgbcolor(window->surface, 255, 30, 30));

	s_font_init(&font, "arial.ttf");
	s_font_set_str(font, "Shutdown");
	s_font_set_size(font, 16);
	s_font_get_glyph(font);
	s_image_get_handler(font->glyph.img);
	s_putboxrgba(window->surface, 40 + 35 + 5, 48 + 12, font->glyph.img->w, font->glyph.img->h, font->glyph.img->rgba);
	s_font_uninit(font);
	
	s_handler_init(&hndl);
	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = 40;
	hndl->mouse.y = 48;
	hndl->mouse.w = 35;
	hndl->mouse.h = 35;
	hndl->mouse.r = xynthlogout_logout;
	hndl->mouse.button = MOUSE_LEFTBUTTON;
	s_handler_add(window, hndl);

	s_fillbox(window->surface, 175, 48, 35, 35, s_rgbcolor(window->surface, 0, 0, 0));
	s_fillbox(window->surface, 177, 50, 31, 31, s_rgbcolor(window->surface, 30, 255, 30));

	s_font_init(&font, "arial.ttf");
	s_font_set_str(font, "Cancel");
	s_font_set_size(font, 16);
	s_font_get_glyph(font);
	s_image_get_handler(font->glyph.img);
	s_putboxrgba(window->surface, 175 + 35 + 5, 48 + 12, font->glyph.img->w, font->glyph.img->h, font->glyph.img->rgba);
	s_font_uninit(font);

	s_handler_init(&hndl);
	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = 175;
	hndl->mouse.y = 48;
	hndl->mouse.w = 35;
	hndl->mouse.h = 35;
	hndl->mouse.r = xynthlogout_cancel;
	hndl->mouse.button = MOUSE_LEFTBUTTON;
	s_handler_add(window, hndl);

	s_window_show(window);
	s_window_main(window);
	
	return 0;
}

#if defined(SINGLE_APP)
s_single_app_t single_xynthlogout = {
	xynthlogout_main,
	1,
	{"xynthlogout"}
};
#endif
