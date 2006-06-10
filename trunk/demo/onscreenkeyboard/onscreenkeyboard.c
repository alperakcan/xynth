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

#define HIGHLIGHT

#define BOX_W	(45)
#define BOX_H	(BOX_W)

typedef enum {
	CHARS_NORMAL,
	CHARS_SHIFT,
	CHARS_EXTRA,
	CHARS_NUMBER,
	CHARS_MAX
} CHARS;

static int chars_x = 0;
static int chars_y = 0;
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
	{{"A", "^", "C", "B"},
	 {"D", "@", "F", "E"},
	 {"G", "*", "I", "H"},
	 {"J", "-", "K", "L"},
	 {"M", "del", "N", "space"},
	 {"O", "\"", "Q", "P"},
	 {"R", "=", "T", "S"},
	 {"U", ";", "W", "V"},
	 {"X", "/", "Z", "Y"}},
	{{"(", ",", ")", "."},
	 {"<", "\"", ">", "'"},
	 {"[", "-", "]", "_"},
	 {"{", "!", "}", "?"},
	 {"", "del", "", "space"},
	 {"\\", "+", "/", "="},
	 {"@", ":", "#", ";"},
	 {"$", "~", "%", "`"},
	 {"^", "*", "&", "|"}},
	{{"", "", "1", ""},
	 {"", "", "2", ""},
	 {"", "", "3", ""},
	 {"", "", "4", ""},
	 {"", "del", "5", "space"},
	 {"", "", "6", ""},
	 {"", "", "7", ""},
	 {"", "", "8", ""},
	 {"", "", "9", "0"}}
};

static int draw_single_box (s_surface_t *wsurface, s_rect_t *rect, char *c[4], int colors[3])
{
	int i;
	int r;
	int g;
	int b;
	int fh;
	int x = 0;
	int y = 0;
	char *vbuf;
	s_font_t *font;
	s_surface_t *surface;
	
	vbuf = s_malloc(sizeof(char) * rect->w * rect->h * wsurface->bytesperpixel);
	surface = (s_surface_t *) s_malloc(sizeof(s_surface_t));
	s_getsurfacevirtual(surface, rect->w, rect->h, wsurface->bitsperpixel, vbuf);
	
	s_fillbox(surface, 0, 0, rect->w, rect->h, colors[0]);
	s_fillbox(surface, 1, 1, rect->w - 2, rect->h - 2, colors[1]);

	fh = rect->h / 3;
	
	for (i = 0; i < 4; i++) {
		s_font_init(&font, "veramono.ttf");
		s_font_set_str(font, c[i]);
		s_colorrgb(wsurface, colors[2], &r, &g, &b);
		s_font_set_rgb(font, r, g, b);
		if (strlen(c[i]) > 1) {
			s_font_set_size(font, fh - 5);
		} else {
			s_font_set_size(font, fh);
		}
		s_font_get_glyph(font);
		s_image_get_handler(font->img);

		font->img->handler->x = 0;
		font->img->handler->y = 0;
		font->img->handler->w = font->img->w;
		font->img->handler->h = font->img->h;

		switch (i) {
			case 0:
				x = 5;
				y = (rect->h - fh) / 2;
				break;
			case 1:
				x = (rect->w - font->img->handler->w) / 2;
				y = 2;
				break;
			case 2:
				x = rect->w - font->img->handler->w - 3;
				y = (rect->h - fh) / 2;
				break;
			case 3:
				y = rect->h - fh + font->yMin - 6;
				x = (rect->w - font->img->handler->w) / 2;
				break;
		}
		s_putboxpartrgba(surface, x, y + fh - font->yMax, font->img->handler->w, font->img->handler->h, font->img->w, font->img->h, font->img->rgba, font->img->handler->x, font->img->handler->y);

		s_font_uninit(font);
	}
	
        s_putbox(wsurface, rect->x, rect->y, rect->w, rect->h, surface->vbuf);

        s_free(vbuf);
        s_free(surface);
	return 0;
}

static int draw_boxes (s_window_t *window)
{
	int x;
	int y;
	char *vbuf;
	int colors[3];
	s_rect_t rect;
	s_surface_t *surface;
	
	vbuf = s_malloc(sizeof(char) * window->surface->buf->w * window->surface->buf->h * window->surface->bytesperpixel);
	surface = (s_surface_t *) s_malloc(sizeof(s_surface_t));
	s_getsurfacevirtual(surface, window->surface->buf->w, window->surface->buf->h, window->surface->bitsperpixel, vbuf);

	for (y = 0; y < 3; y++) {
		for (x = 0; x < 3; x++) {
			rect.x = x * BOX_W;
			rect.y = y * BOX_H;
			rect.w = BOX_W;
			rect.h = BOX_H;
			if (chars_x == x && chars_y == y) {
				continue;
			} else {
				colors[0] = s_rgbcolor(window->surface, 0, 0, 0);
				colors[1] = s_rgbcolor(window->surface, 222, 222, 222);
				colors[2] = s_rgbcolor(window->surface, 0, 0, 0);
			}
			draw_single_box(surface, &rect, chars[chars_type][y * 3 + x], colors);
		}
	}
	{
		colors[0] = s_rgbcolor(window->surface, 0, 0, 0);
		colors[1] = s_rgbcolor(window->surface, 255, 140, 100);
		colors[2] = s_rgbcolor(window->surface, 255, 255, 255);
		rect.x = chars_x * BOX_W;
		rect.y = chars_y * BOX_H;
		rect.w = BOX_W;
		rect.h = BOX_H;
#if defined HIGHLIGHT
		rect.x -= 4;
		rect.y -= 4;
		rect.w += 8;
		rect.h += 8;
		rect.x = MAX(0, rect.x);
		rect.y = MAX(0, rect.y);
		rect.x = MIN(window->surface->buf->w - rect.w, rect.x);
		rect.y = MIN(window->surface->buf->h - rect.h, rect.y);
#endif
		draw_single_box(surface, &rect, chars[chars_type][chars_y * 3 + chars_x], colors);
	}

        s_putbox(window->surface, 0, 0, surface->width, surface->height, surface->vbuf);
	free(surface);
	free(vbuf);

	return 0;
}

static void handler_shift_up (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	chars_type++;
	chars_type %= CHARS_MAX;
	draw_boxes(window);
}

static void handler_shift_down (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	chars_type--;
	chars_type = MAX(0, chars_type);
	chars_type = MIN(CHARS_MAX - 1, chars_type);
	draw_boxes(window);
}

static void handler_left (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	chars_x--;
	if (chars_x < 0) {
		chars_x = 2;
	}
	draw_boxes(window);
}

static void handler_right (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	chars_x++;
	if (chars_x > 2) {
		chars_x = 0;
	}
	draw_boxes(window);
}

static void handler_up (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	chars_y--;
	if (chars_y < 0) {
		chars_y = 2;
	}
	draw_boxes(window);
}

static void handler_down (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	chars_y++;
	if (chars_y > 2) {
		chars_y = 0;
	}
	draw_boxes(window);
}

static void handler_set_left (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	printf("%s\n", chars[chars_type][chars_y * 3 + chars_x][0]);
}

static void handler_set_right (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	printf("%s\n", chars[chars_type][chars_y * 3 + chars_x][2]);
}

static void handler_set_up (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	printf("%s\n", chars[chars_type][chars_y * 3 + chars_x][1]);
}

static void handler_set_down (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	printf("%s\n", chars[chars_type][chars_y * 3 + chars_x][3]);
}

static void handler_set (s_window_t *window, S_KEYCODE_CODE key, void (*func) (s_window_t *, s_event_t *, s_handler_t *))
{
	s_handler_t *hndl;
	s_handler_init(&hndl);
	hndl->type = KEYBD_HANDLER;
	hndl->keybd.flag = 0;
	hndl->keybd.button = key;
	hndl->keybd.r = func;
	s_handler_add(window, hndl);
}

int main (int argc, char *argv[])
{
	int x;
	int y;
	int w;
	int h;
	s_window_t *window;

	s_client_init(&window);
	s_window_new(window, WINDOW_MAIN | WINDOW_NOFORM, NULL);
	s_window_set_title(window, "Demo - %s ", argv[0]);
	
	w = BOX_W * 3;
	h = BOX_H * 3;
	x = (window->surface->linear_buf_width - w) / 2;
	y = (window->surface->linear_buf_height - h) / 2;
	s_window_set_coor(window, 0, x, y, w, h);
	
	draw_boxes(window);

	handler_set(window, S_KEYCODE_RIGHTSHIFT, handler_shift_up);
	handler_set(window, S_KEYCODE_LEFTSHIFT, handler_shift_down);
	handler_set(window, S_KEYCODE_LEFT, handler_left);
	handler_set(window, S_KEYCODE_RIGHT, handler_right);
	handler_set(window, S_KEYCODE_UP, handler_up);
	handler_set(window, S_KEYCODE_DOWN, handler_down);
	handler_set(window, S_KEYCODE_i, handler_set_up);
	handler_set(window, S_KEYCODE_j, handler_set_left);
	handler_set(window, S_KEYCODE_k, handler_set_down);
	handler_set(window, S_KEYCODE_l, handler_set_right);

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
