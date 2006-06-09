/***************************************************************************
    begin                : Fri Jun 9 2006
    copyright            : (C) 2006 by Alper Akcan
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

#define BOX_W	(50)
#define BOX_H	(BOX_W)
#define FONT_H  ((BOX_H * 5) / 12)

typedef enum {
	CHARS_NORMAL,
	CHARS_SHIFT,
	CHARS_MAX
} CHARS;

static int chars_x = 0;
static int chars_y = 0;
static int chars_char = 0;
static CHARS chars_type = CHARS_NORMAL;

static char *chars[][9][4] = {
	{{"a", ",", "c", "b"},
	 {"d", ".", "f", "e"},
	 {"g", "!", "i", "h"},
	 {"j", "-", "k", "l"},
	 {"m", "del", "n", "space"},
	 {"o", "?", "q", "p"},
	 {"r", "(", "t", "s"},
	 {"u", ":", "w", "v"},
	 {"x", ")", "z", "y"}},
	{{"A", ",", "C", "B"},
	 {"D", ".", "F", "E"},
	 {"G", "!", "I", "H"},
	 {"J", "-", "K", "L"},
	 {"M", "del", "N", "space"},
	 {"O", "?", "Q", "P"},
	 {"R", "(", "T", "S"},
	 {"U", ":", "W", "V"},
	 {"X", ")", "Z", "Y"}}
};

static int draw_single_box (s_window_t *window, s_rect_t *rect, char *c[4], int colors[3])
{
	int i;
	int r;
	int g;
	int b;
	int x = 0;
	int y = 0;
	char *vbuf;
	s_font_t *font;
	s_surface_t *surface;
	
	vbuf = s_malloc(sizeof(char) * rect->w * rect->h * window->surface->bytesperpixel);
	surface = (s_surface_t *) s_malloc(sizeof(s_surface_t));
	s_getsurfacevirtual(surface, rect->w, rect->h, window->surface->bitsperpixel, vbuf);
	
	s_fillbox(surface, 0, 0, rect->w, rect->h, colors[0]);
	s_fillbox(surface, 1, 1, rect->w - 2, rect->h - 2, colors[1]);
	
	for (i = 0; i < 4; i++) {
		s_font_init(&font, "arial.ttf");
		s_font_set_str(font, c[i]);
		s_colorrgb(window->surface, colors[2], &r, &g, &b);
		s_font_set_rgb(font, r, g, b);
		if (strlen(c[i]) > 1) {
			s_font_set_size(font, FONT_H - 7);
		} else {
			s_font_set_size(font, FONT_H);
		}
		s_font_get_glyph(font);
		s_image_get_handler(font->img);
	
		switch (i) {
			case 0:
				x = 4;
				y = (BOX_H - font->img->handler->h) / 2;
				break;
			case 1:
				y = 5;
				x = (BOX_W - font->img->handler->w) / 2;
				break;
			case 2:
				x = BOX_W - font->img->handler->w - 4;
				y = (BOX_H - font->img->handler->h) / 2;
				break;
			case 3:
				y = BOX_H - font->img->handler->h - 4;
				x = (BOX_W - font->img->handler->w) / 2;
				break;
		}
		s_putboxpartrgba(surface, x, y, font->img->handler->w, font->img->handler->h, font->img->w, font->img->h, font->img->rgba, font->img->handler->x, font->img->handler->y);

		s_font_uninit(font);
	}
	
        s_putbox(window->surface, rect->x, rect->y, rect->w, rect->h, surface->vbuf);

        s_free(vbuf);
        s_free(surface);
	return 0;
}

static int draw_boxes (s_window_t *window)
{
	int x;
	int y;
	int colors[3];
	s_rect_t rect;
	
	for (y = 0; y < 3; y++) {
		for (x = 0; x < 3; x++) {
			rect.x = x * BOX_W;
			rect.y = y * BOX_H;
			rect.w = BOX_W;
			rect.h = BOX_H;
			if (chars_x == x &&
			    chars_y == y) {
				colors[0] = s_rgbcolor(window->surface, 0, 0, 0);
				colors[1] = s_rgbcolor(window->surface, 255, 140, 100);
				colors[2] = s_rgbcolor(window->surface, 255, 255, 255);
			} else {
				colors[0] = s_rgbcolor(window->surface, 0, 0, 0);
				colors[1] = s_rgbcolor(window->surface, 222, 222, 222);
				colors[2] = s_rgbcolor(window->surface, 0, 0, 0);
			}
			draw_single_box(window, &rect, chars[chars_type][y * 3 + x], colors);
		}
	}
	
	return 0;
}

static void handler_shift (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	chars_type++;
	chars_type %= CHARS_MAX;
	draw_boxes(window);
}

int main (int argc, char *argv[])
{
	int x;
	int y;
	int w;
	int h;
	s_handler_t *hndl;
	s_window_t *window;

	s_client_init(&window);
	s_window_new(window, WINDOW_MAIN | WINDOW_NOFORM, NULL);
	s_window_set_title(window, "Demo - %s ", argv[0]);
	
	x = 100;
	y = 100;
	w = BOX_W * 3;
	h = BOX_H * 3;
	s_window_set_coor(window, 0, x, y, w, h);
	
	draw_boxes(window);
	
	s_handler_init(&hndl);
	hndl->type = KEYBD_HANDLER;
	hndl->keybd.flag = 0;
	hndl->keybd.button = S_KEYCODE_RIGHTSHIFT;
	hndl->keybd.r = handler_shift;
	s_handler_add(window, hndl);

	s_handler_init(&hndl);
	hndl->type = KEYBD_HANDLER;
	hndl->keybd.flag = 0;
	hndl->keybd.button = S_KEYCODE_LEFTSHIFT;
	hndl->keybd.r = handler_shift;
	s_handler_add(window, hndl);

	s_window_show(window);
	s_client_main(window);
	
	return 0;
}

#if defined(SINGLE_APP)
s_single_app_t single_onscreenkeyboard = {
	main,
	1,
	{"onscreenkeyboard"}
};
#endif
