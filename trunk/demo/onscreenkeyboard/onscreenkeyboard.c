/***************************************************************************
    begin                : Fri Jun 9 2006
    copyright            : (C) 2006 - 2008 by Alper Akcan
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

/*
 * Left Shift  : shift layout down
 * Right Shift : shift layout up
 * Right       : move right
 * Left        : move left
 * Up          : move up
 * Down        : move down
 *
 * Mode 1; (chars_highlight == 1)
 *     j       : char select / left
 *     k       : char select / down
 *     l       : char select / right
 *     i       : char select / up
 *
 * Mode 2; (chars_highlight == 0)
 *     space   : switch select char, move
 *     left    : char select / left
 *     down    : char select / down
 *     right   : char select / right
 *     up      : char select / up
 */

#include "xynth_.h"

#define BOX_W	(45)
#define BOX_H	(BOX_W)

typedef enum {
	CHARS_NORMAL,
	CHARS_SHIFT,
	CHARS_EXTRA,
	CHARS_NUMBER,
	CHARS_MAX
} CHARS;

typedef enum {
	SELECT_MAP,
	SELECT_CHAR
} CHARS_SELECT;

static int chars_x = 1;
static int chars_y = 1;
static int chars_highlight = 0;
static int chars_select = SELECT_MAP;
static CHARS chars_type = CHARS_NORMAL;

typedef struct osk_char_s {
	char ascii;
	int button;
	int scancode;
	char *name;
} osk_char_t;

typedef struct image_bin_s {
	unsigned int width;
	unsigned int height;
	unsigned int bytes_per_pixel;
	char *pixel_data;
} image_bin_t;

static image_bin_t image_gray = {
	1, 45, 3,
	"\276\276\276\274\274\274\271\271\271\267\267\267\264\264\264\262\262\262"
	"\260\257\257\255\255\255\253\252\252\250\250\250\246\245\245\244\243\243"
	"\241\241\241\237\236\236\234\233\233\231\231\231\227\227\227\225\224\224"
	"\222\222\222\220\217\217\215\214\214\213\212\212\211\210\210\206\205\205"
	"\204\203\203\201\200\200\200\177\177~||{zzywwvuutrrqppommlkkjhhhffeddcaa"
	"`^^^\\\\[YYYWWWUUTRR",
};

static image_bin_t image_orange = {
	1, 45, 3,
	"\361\337\257\361\336\254\362\334\250\362\333\244\362\331\240\362\330\234"
	"\363\326\230\363\325\224\363\323\221\363\322\215\364\320\211\364\317\205"
	"\364\315\201\365\314~\365\313{\365\311w\365\310s\366\306o\366\305l\366\303"
	"h\366\302d\367\300`\367\277\\\367\275Y\367\274U\370\272Q\370\271M\370\267"
	"I\371\266E\371\264A\371\263=\371\2619\372\2606\372\2572\372\255.\373\254"
	"*\373\252'\373\251#\373\247\37\374\246\33\374\244\27\374\243\24\374\241\20"
	"\375\240\14\375\236\10",
};

static osk_char_t chars[][9][4] = {
	{{{'a', KEYBOARD_BUTTON_a, 0, "a"},
	  {',', KEYBOARD_BUTTON_COMMA, 0, ","},
	  {'c', KEYBOARD_BUTTON_c, 0, "c"},
	  {'b', KEYBOARD_BUTTON_b, 0, "b"}},
	 {{'d', KEYBOARD_BUTTON_d, 0, "d"},
	  {'.', KEYBOARD_BUTTON_PERIOD, 0, "."},
	  {'f', KEYBOARD_BUTTON_f, 0, "f"},
	  {'e', KEYBOARD_BUTTON_e, 0, "e"}},
	 {{'g', KEYBOARD_BUTTON_g, 0, "g"},
	  {'!', KEYBOARD_BUTTON_EXCLAM, 0, "!"},
	  {'i', KEYBOARD_BUTTON_i, 0, "i"},
	  {'h', KEYBOARD_BUTTON_h, 0, "h"}},
	 {{'j', KEYBOARD_BUTTON_j, 0, "j"},
	  {'-', KEYBOARD_BUTTON_MINUS, 0, "-"},
	  {'l', KEYBOARD_BUTTON_l, 0, "l"},
	  {'k', KEYBOARD_BUTTON_k, 0, "k"}},
	 {{'m', KEYBOARD_BUTTON_m, 0, "m"},
	  {0, KEYBOARD_BUTTON_DELETE, 0, "del"},
	  {'n', KEYBOARD_BUTTON_n, 0, "n"},
	  {' ', KEYBOARD_BUTTON_SPACE, 0, "space"}},
	 {{'o', KEYBOARD_BUTTON_o, 0, "o"},
	  {'?', KEYBOARD_BUTTON_QUESTION, 0, "?"},
	  {'q', KEYBOARD_BUTTON_q, 0, "q"},
	  {'p', KEYBOARD_BUTTON_p, 0, "p"}},
	 {{'r', KEYBOARD_BUTTON_r, 0, "r"},
	  {'(', KEYBOARD_BUTTON_PARENLEFT, 0, "("},
	  {'t', KEYBOARD_BUTTON_t, 0, "t"},
	  {'s', KEYBOARD_BUTTON_s, 0, "s"}},
	 {{'u', KEYBOARD_BUTTON_u, 0, "u"},
	  {':', KEYBOARD_BUTTON_COLON, 0, ":"},
	  {'w', KEYBOARD_BUTTON_w, 0, "w"},
	  {'v', KEYBOARD_BUTTON_v, 0, "v"}},
	 {{'x', KEYBOARD_BUTTON_u, 0, "x"},
	  {')', KEYBOARD_BUTTON_PARENRIGHT, 0, ")"},
	  {'z', KEYBOARD_BUTTON_w, 0, "z"},
	  {'y', KEYBOARD_BUTTON_v, 0, "y"}}},

	{{{'A', KEYBOARD_BUTTON_A, 0, "A"},
	  {'^', KEYBOARD_BUTTON_ASCIICIRCUM, 0, "^"},
	  {'C', KEYBOARD_BUTTON_C, 0, "C"},
	  {'B', KEYBOARD_BUTTON_B, 0, "B"}},
	 {{'D', KEYBOARD_BUTTON_D, 0, "D"},
	  {'@', KEYBOARD_BUTTON_AT, 0, "@"},
	  {'F', KEYBOARD_BUTTON_F, 0, "F"},
	  {'E', KEYBOARD_BUTTON_E, 0, "E"}},
	 {{'G', KEYBOARD_BUTTON_G, 0, "G"},
	  {'*', KEYBOARD_BUTTON_ASTERISK, 0, "*"},
	  {'I', KEYBOARD_BUTTON_I, 0, "I"},
	  {'H', KEYBOARD_BUTTON_H, 0, "H"}},
	 {{'J', KEYBOARD_BUTTON_J, 0, "J"},
	  {'_', KEYBOARD_BUTTON_UNDERSCORE, 0, "_"},
	  {'L', KEYBOARD_BUTTON_L, 0, "L"},
	  {'K', KEYBOARD_BUTTON_K, 0, "K"}},
	 {{'M', KEYBOARD_BUTTON_M, 0, "M"},
	  {0, KEYBOARD_BUTTON_REMOVE, 0, "remove"},
	  {'N', KEYBOARD_BUTTON_N, 0, "N"},
	  {'\n', KEYBOARD_BUTTON_RETURN, 0, "enter"}},
	 {{'O', KEYBOARD_BUTTON_O, 0, "O"},
	  {'\"', KEYBOARD_BUTTON_QUOTEDBL, 0, "\""},
	  {'Q', KEYBOARD_BUTTON_Q, 0, "Q"},
	  {'P', KEYBOARD_BUTTON_P, 0, "P"}},
	 {{'R', KEYBOARD_BUTTON_R, 0, "R"},
	  {'=', KEYBOARD_BUTTON_EQUAL, 0, "="},
	  {'T', KEYBOARD_BUTTON_T, 0, "T"},
	  {'S', KEYBOARD_BUTTON_S, 0, "S"}},
	 {{'U', KEYBOARD_BUTTON_U, 0, "U"},
	  {';', KEYBOARD_BUTTON_SEMICOLON, 0, ";"},
	  {'W', KEYBOARD_BUTTON_W, 0, "W"},
	  {'V', KEYBOARD_BUTTON_V, 0, "V"}},
	 {{'X', KEYBOARD_BUTTON_U, 0, "X"},
	  {'/', KEYBOARD_BUTTON_SLASH, 0, "/"},
	  {'Z', KEYBOARD_BUTTON_W, 0, "Z"},
	  {'Y', KEYBOARD_BUTTON_V, 0, "Y"}}},

	{{{'(', KEYBOARD_BUTTON_PARENLEFT, 0, "("},
	  {',', KEYBOARD_BUTTON_COMMA, 0, ","},
	  {')', KEYBOARD_BUTTON_PARENRIGHT, 0, ")"},
	  {'.', KEYBOARD_BUTTON_PERIOD, 0, "."}},
	 {{'<', KEYBOARD_BUTTON_LESS, 0, "<"},
	  {'\"', KEYBOARD_BUTTON_QUOTEDBL, 0, "\""},
	  {'>', KEYBOARD_BUTTON_GREATER, 0, ">"},
	  {'\'', KEYBOARD_BUTTON_APOSTROPHE, 0, "\'"}},
	 {{'[', KEYBOARD_BUTTON_BRACKETLEFT, 0, "["},
	  {'-', KEYBOARD_BUTTON_MINUS, 0, "-"},
	  {']', KEYBOARD_BUTTON_BRACKETRIGHT, 0, "]"},
	  {'_', KEYBOARD_BUTTON_UNDERSCORE, 0, "_"}},
	 {{'{', KEYBOARD_BUTTON_BRACELEFT, 0, "{"},
	  {'!', KEYBOARD_BUTTON_EXCLAM, 0, "!"},
	  {'}', KEYBOARD_BUTTON_BRACERIGHT, 0, "}"},
	  {'?', KEYBOARD_BUTTON_QUESTION, 0, "?"}},
	 {{0, 0, 0, ""},
	  {0, KEYBOARD_BUTTON_DELETE, 0, "del"},
	  {0, 0, 0, ""},
	  {' ', KEYBOARD_BUTTON_SPACE, 0, "space"}},
	 {{'\\', KEYBOARD_BUTTON_BACKSLASH, 0, "\\"},
	  {'+', KEYBOARD_BUTTON_PLUS, 0, "+"},
	  {'/', KEYBOARD_BUTTON_SLASH, 0, "/"},
	  {'=', KEYBOARD_BUTTON_EQUAL, 0, "="}},
	 {{'@', KEYBOARD_BUTTON_AT, 0, "@"},
	  {':', KEYBOARD_BUTTON_COLON, 0, ":"},
	  {'#', KEYBOARD_BUTTON_NUMBERSIGN, 0, "#"},
	  {';', KEYBOARD_BUTTON_SEMICOLON, 0, ";"}},
	 {{'$', KEYBOARD_BUTTON_DOLLAR, 0, "$"},
	  {'~', KEYBOARD_BUTTON_ASCIITILDE, 0, "~"},
	  {'%', KEYBOARD_BUTTON_PERCENT, 0, "%"},
	  {'`', KEYBOARD_BUTTON_GRAVE, 0, "`"}},
	 {{'^', KEYBOARD_BUTTON_ASCIICIRCUM, 0, "^"},
	  {'*', KEYBOARD_BUTTON_ASTERISK, 0, "*"},
	  {'&', KEYBOARD_BUTTON_AMPERSAND, 0, "&"},
	  {'|', KEYBOARD_BUTTON_BAR, 0, "|"}}},

	{{{0, 0, 0, ""},
	  {0, 0, 0, ""},
	  {'1', KEYBOARD_BUTTON_ONE, 0, "1"},
	  {0, 0, 0, ""}},
	 {{0, 0, 0, ""},
	  {0, 0, 0, ""},
	  {'2', KEYBOARD_BUTTON_TWO, 0, "2"},
	  {0, 0, 0, ""}},
	 {{0, 0, 0, ""},
	  {0, 0, 0, ""},
	  {'3', KEYBOARD_BUTTON_THREE, 0, "3"},
	  {0, 0, 0, ""}},
	 {{0, 0, 0, ""},
	  {0, 0, 0, ""},
	  {'4', KEYBOARD_BUTTON_FOUR, 0, "4"},
	  {0, 0, 0, ""}},
	 {{0, 0, 0, ""},
	  {0, 0, 0, ""},
	  {'5', KEYBOARD_BUTTON_FIVE, 0, "5"},
	  {0, 0, 0, ""}},
	 {{0, 0, 0, ""},
	  {0, 0, 0, ""},
	  {'6', KEYBOARD_BUTTON_SIX, 0, "6"},
	  {0, 0, 0, ""}},
	 {{0, 0, 0, ""},
	  {0, 0, 0, ""},
	  {'7', KEYBOARD_BUTTON_SEVEN, 0, "7"},
	  {0, 0, 0, ""}},
	 {{0, 0, 0, ""},
	  {0, 0, 0, ""},
	  {'8', KEYBOARD_BUTTON_EIGHT, 0, "8"},
	  {0, 0, 0, ""}},
	 {{0, 0, 0, ""},
	  {0, 0, 0, ""},
	  {'9', KEYBOARD_BUTTON_NINE, 0, "9"},
	  {'0', KEYBOARD_BUTTON_ZERO, 0, "0"}}}
};

static int image_load (s_image_t *img, int w, int h, unsigned char *rgb)
{
	int i;
	unsigned int *tmp;
	img->w = w;
	img->h = h;
	img->rgba = (unsigned int *) s_malloc(sizeof(unsigned int) * w * h);
	tmp = img->rgba;
	for (i = 0; i < w * h; i++) {
		*tmp = (*(rgb + 0) << 24) | (*(rgb + 1) << 16) | (*(rgb + 2) << 8) | 0x00;
		rgb += 3;
		tmp++;
	}
	return 0;
}

static int draw_single_box (s_surface_t *wsurface, s_rect_t *rect, osk_char_t c[4], int colors[2], image_bin_t *image_bin)
{
	int i;
	int r;
	int g;
	int b;
	int fh;
	int x = 0;
	int y = 0;
	char *tbuf;
	s_image_t *img;
	s_font_t *font;
	s_surface_t *surface;
	
	if (s_surface_create(&surface, rect->w, rect->h, wsurface->bitsperpixel)) {
		return -1;
	}
	
	s_fillbox(surface, 0, 0, rect->w, rect->h, colors[0]);
	
	s_image_init(&img);
	image_load(img, image_bin->width, image_bin->height, (unsigned char *) image_bin->pixel_data);
	s_image_get_buf(surface, img);
	tbuf = (char *) s_malloc(sizeof(char) * (rect->w - 2) * (rect->h - 2) * surface->bytesperpixel);
	s_scalebox(surface, img->w, img->h, img->buf, rect->w - 2, rect->h - 2, tbuf);
	s_putbox(surface, 1, 1, rect->w - 2, rect->h - 2, tbuf);
	s_free(tbuf);
	s_image_uninit(img);
	
	fh = rect->h / 3;
	
	for (i = 0; i < 4; i++) {
		s_font_init(&font, "veramono.ttf");
		s_font_set_str(font, c[i].name);
		s_colorrgb(wsurface, colors[1], &r, &g, &b);
		s_font_set_rgb(font, r, g, b);
		if (strlen(c[i].name) > 1) {
			s_font_set_size(font, fh - 5);
		} else {
			s_font_set_size(font, fh);
		}
		s_font_get_glyph(font);
		s_image_get_handler(font->glyph.img);

		font->glyph.img->handler->x = 0;
		font->glyph.img->handler->y = 0;
		font->glyph.img->handler->w = font->glyph.img->w;
		font->glyph.img->handler->h = font->glyph.img->h;

		switch (i) {
			case 0:
				x = 5;
				y = (rect->h - fh) / 2;
				break;
			case 1:
				x = (rect->w - font->glyph.img->handler->w) / 2;
				y = 2;
				break;
			case 2:
				x = rect->w - font->glyph.img->handler->w - 3;
				y = (rect->h - fh) / 2;
				break;
			case 3:
				y = rect->h - fh - 7;
				x = (rect->w - font->glyph.img->handler->w) / 2;
				break;
		}
		s_putboxpartrgba(surface, x, y + fh - font->glyph.yMax, font->glyph.img->handler->w, font->glyph.img->handler->h, font->glyph.img->w, font->glyph.img->h, font->glyph.img->rgba, font->glyph.img->handler->x, font->glyph.img->handler->y);

		s_font_uninit(font);
	}
	
        s_putbox(wsurface, rect->x, rect->y, rect->w, rect->h, surface->vbuf);

        s_surface_destroy(surface);
	return 0;
}

static int draw_boxes (s_window_t *window)
{
	int x;
	int y;
	int colors[3];
	s_rect_t rect;
	s_surface_t *surface;
	
	if (s_surface_create(&surface, window->surface->buf->w, window->surface->buf->h, window->surface->bitsperpixel)) {
		return -1;
	}

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
				colors[1] = s_rgbcolor(window->surface, 0, 0, 0);
			}
			draw_single_box(surface, &rect, chars[chars_type][y * 3 + x], colors, &image_gray);
		}
	}
	{
		colors[0] = s_rgbcolor(window->surface, 0, 0, 0);
		colors[1] = s_rgbcolor(window->surface, 255, 255, 255);
		rect.x = chars_x * BOX_W;
		rect.y = chars_y * BOX_H;
		rect.w = BOX_W;
		rect.h = BOX_H;
		
		if (chars_select == SELECT_CHAR ||
		    chars_highlight == 1) {
			rect.x -= 5;
        		rect.y -= 5;
			rect.w += 10;
			rect.h += 10;
			rect.x = MAX(0, rect.x);
			rect.y = MAX(0, rect.y);
			rect.x = MIN(window->surface->buf->w - rect.w, rect.x);
			rect.y = MIN(window->surface->buf->h - rect.h, rect.y);
		}
		
		draw_single_box(surface, &rect, chars[chars_type][chars_y * 3 + chars_x], colors, &image_orange);
	}

        s_putbox(window->surface, 0, 0, surface->width, surface->height, surface->vbuf);
        
        s_surface_destroy(surface);
	return 0;
}

static void handler_set_char (s_window_t *window, osk_char_t *chr)
{
	s_event_t *evt;
	s_event_init(&evt);
	evt->type = EVENT_TYPE_KEYBOARD | EVENT_TYPE_KEYBOARD_PRESSED;
	evt->keybd->ascii = chr->ascii;
	evt->keybd->button = chr->button;
	evt->keybd->keycode = chr->button;
	evt->keybd->scancode = chr->scancode;
	s_socket_request(window, SOC_DATA_EVENT, evt);
	evt->type = EVENT_TYPE_KEYBOARD | EVENT_TYPE_KEYBOARD_RELEASED;
	s_socket_request(window, SOC_DATA_EVENT, evt);
	s_event_uninit(evt);
}

static void handler_set_left (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	osk_char_t *chr;
	chr = &chars[chars_type][chars_y * 3 + chars_x][0];
	handler_set_char(window, chr);
}

static void handler_set_right (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	osk_char_t *chr;
	chr = &chars[chars_type][chars_y * 3 + chars_x][2];
	handler_set_char(window, chr);
}

static void handler_set_up (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	osk_char_t *chr;
	chr = &chars[chars_type][chars_y * 3 + chars_x][1];
	handler_set_char(window, chr);
}

static void handler_set_down (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	osk_char_t *chr;
	chr = &chars[chars_type][chars_y * 3 + chars_x][3];
	handler_set_char(window, chr);
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
	if (chars_select == SELECT_MAP) {
		chars_x--;
		if (chars_x < 0) {
			chars_x = 2;
		}
		draw_boxes(window);
	} else if (chars_select == SELECT_CHAR) {
		handler_set_left(window, event, handler);
	}
}

static void handler_right (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	if (chars_select == SELECT_MAP) {
		chars_x++;
		if (chars_x > 2) {
			chars_x = 0;
		}
		draw_boxes(window);
	} else if (chars_select == SELECT_CHAR) {
		handler_set_right(window, event, handler);
	}
}

static void handler_up (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	if (chars_select == SELECT_MAP) {
		chars_y--;
		if (chars_y < 0) {
			chars_y = 2;
		}
		draw_boxes(window);
	} else if (chars_select == SELECT_CHAR) {
		handler_set_up(window, event, handler);
	}
}

static void handler_down (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	if (chars_select == SELECT_MAP) {
		chars_y++;
		if (chars_y > 2) {
			chars_y = 0;
		}
		draw_boxes(window);
	} else if (chars_select == SELECT_CHAR) {
		handler_set_down(window, event, handler);
	}
}

static void handler_set_map (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	chars_select++;
	chars_select %= 2;
	draw_boxes(window);
}

static void handler_set (s_window_t *window, s_keyboard_button_t key, void (*func) (s_window_t *, s_event_t *, s_handler_t *))
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

	s_window_init(&window);
	s_window_new(window, WINDOW_TYPE_MAIN /*| WINDOW_TYPE_NOFORM*/, NULL);
	s_window_set_title(window, "Demo - %s ", argv[0]);
	s_window_set_resizeable(window, 0);
	s_window_set_alwaysontop(window, 1);
	
	w = BOX_W * 3;
	h = BOX_H * 3;
	x = (window->surface->linear_buf_width - w) / 2;
	y = (window->surface->linear_buf_height - h) / 2;
	s_window_set_coor(window, WINDOW_TYPE_NOFORM, x, y, w, h);

	window->surface->width = window->surface->buf->w;
	window->surface->height = window->surface->buf->h;
	s_free(window->surface->vbuf);
	window->surface->vbuf = s_malloc(window->surface->width * window->surface->height * window->surface->bytesperpixel);
	
	draw_boxes(window);

	handler_set(window, KEYBOARD_BUTTON_RIGHTSHIFT, handler_shift_up);
	handler_set(window, KEYBOARD_BUTTON_LEFTSHIFT, handler_shift_down);
	handler_set(window, KEYBOARD_BUTTON_LEFT, handler_left);
	handler_set(window, KEYBOARD_BUTTON_RIGHT, handler_right);
	handler_set(window, KEYBOARD_BUTTON_UP, handler_up);
	handler_set(window, KEYBOARD_BUTTON_DOWN, handler_down);
	handler_set(window, KEYBOARD_BUTTON_SPACE, handler_set_map);
	handler_set(window, KEYBOARD_BUTTON_i, handler_set_up);
	handler_set(window, KEYBOARD_BUTTON_j, handler_set_left);
	handler_set(window, KEYBOARD_BUTTON_k, handler_set_down);
	handler_set(window, KEYBOARD_BUTTON_l, handler_set_right);

	s_window_show(window);
	s_window_main(window);
	
	return 0;
}

#if defined(CONFIG_SINGLE_APPLICATION)
s_single_app_t single_onscreenkeyboard = {
	main,
	1,
	{"onscreenkeyboard"}
};
#endif
