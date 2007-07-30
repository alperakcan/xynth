/***************************************************************************
    begin                : Thu Oct 21 2004
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
#include <unistd.h>

#include "xynth_.h"

#undef DEBUG
#define KERNEL_26

typedef struct load_data_s {
	int running;

	int width;
	int height;

	s_rect_t c_rect;
	s_rect_t m_rect;
	
	float c_user;
	float c_nice;
	float c_sys;
	float c_idle;
	float c_total;
	unsigned long long int c_user_;
	unsigned long long int c_nice_;
	unsigned long long int c_sys_;
	unsigned long long int c_idle_;

	double a_1min;
	double a_5min;
	double a_15min;

	unsigned int m_total;
	unsigned int m_used;
	unsigned int m_free;
	unsigned int m_shared;
	unsigned int m_buffers;
	unsigned int m_cached;
	unsigned int s_total;
	unsigned int s_used;
	unsigned int s_free;
	unsigned int s_cached;

	int c_data[4][230];
	int m_data[2][230];
} load_data_t;

static int load_draw_cpu_bar (s_surface_t *surface, load_data_t *ldata)
{
        int i;
	int x;
	int y;
	int w;
	int h;
	int c;
	int c0;
	int c1;
	int h_;
	int lvl;
	char text[5];
	s_font_t *font;

	x = ldata->c_rect.x;
	y = ldata->c_rect.y;
	w = ldata->c_rect.w / 7;
	h = ldata->c_rect.h;
	
	c0 = s_rgbcolor(surface, 66, 99, 66);
	c1 = s_rgbcolor(surface, 0, 250, 24);

        lvl = 20;
        h_ = (h - 15) / lvl;

	s_fillbox(surface, x, y, w, h_ * lvl, s_rgbcolor(surface, 0, 0, 0));

        i = 0;
	while (lvl--) {
		if (i <= (int) (ldata->c_user / 5.0)) {
			c = c1;
		} else {
			c = c0;
		}
		s_fillbox(surface, x, y + (lvl * h_) + 1, w, h_ - 2, c);
		i++;
	}

	for (i = 229; i > 0; i--) {
		ldata->c_data[0][i] = ldata->c_data[0][i - 1];
		ldata->c_data[1][i] = ldata->c_data[1][i - 1];
		ldata->c_data[2][i] = ldata->c_data[2][i - 1];
		ldata->c_data[3][i] = ldata->c_data[3][i - 1];
	}
	ldata->c_data[0][0] = (int) ((ldata->c_user / 100.0) * (double) (h_ * 20));
	ldata->c_data[1][0] = (int) ((ldata->a_1min) * (double) (h_ * 20));
	ldata->c_data[2][0] = (int) ((ldata->a_5min) * (double) (h_ * 20));
	ldata->c_data[3][0] = (int) ((ldata->a_15min) * (double) (h_ * 20));
	
	s_font_init(&font, "arial.ttf");
	sprintf(text, "%% %02d", (int) (ldata->c_user));
	s_font_set_str(font, text);
	s_font_set_size(font, 12);
	s_font_get_glyph(font);
	s_image_get_handler(font->glyph.img);
	s_putboxrgba(surface, ldata->c_rect.x,
	                      ldata->c_rect.y + ldata->c_rect.h - 10,
	                      font->glyph.img->w,
	                      font->glyph.img->h,
	                      font->glyph.img->rgba);
	s_font_uninit(font);

	return 0;
}

static int load_draw_cpu_graph (s_surface_t *surface, load_data_t *ldata)
{
	int i;
	int x;
	int y;
	int w;
	int h;
	int c0;
	int c1;
	int c2;
	int c3;
	int c4;
	int w_;
	int h_;
	int lvl;
	char text[50];
	s_font_t *font;

	x = ldata->c_rect.x + (ldata->c_rect.w / 7) + 10;
	y = ldata->c_rect.y;
	w = ((ldata->c_rect.w * 6) / 7) - 10;
	h = ldata->c_rect.h;

	c0 = s_rgbcolor(surface, 66, 99, 66);
	c1 = s_rgbcolor(surface, 0, 250, 24);
	c2 = s_rgbcolor(surface, 255, 255, 255);
	c3 = s_rgbcolor(surface, 255, 0, 0);
	c4 = s_rgbcolor(surface, 0, 0, 255);

        lvl = 20;
        h_ = (h - 15) / lvl;

	s_fillbox(surface, x, y, w, h_ * lvl, s_rgbcolor(surface, 0, 0, 0));

	while (lvl--) {
		lvl--;
		s_hline(surface, x, y + (lvl * h_), x + w - 1, c0);
	}
	lvl = 10;
	w_ = w / lvl;
	while (lvl--) {
		s_vline(surface, x + (lvl * w_), y, y + (h_ * 20) - 1, c0);
	}

	for (i = 0; i < 229; i++) {
		s_vline(surface, x + w - 1 - i, y + (h_ * 20) - ldata->c_data[3][i + 1], y + (h_ * 20) - ldata->c_data[3][i], c4);
		s_vline(surface, x + w - 1 - i, y + (h_ * 20) - ldata->c_data[2][i + 1], y + (h_ * 20) - ldata->c_data[2][i], c3);
		s_vline(surface, x + w - 1 - i, y + (h_ * 20) - ldata->c_data[1][i + 1], y + (h_ * 20) - ldata->c_data[1][i], c2);
		s_vline(surface, x + w - 1 - i, y + (h_ * 20) - ldata->c_data[0][i + 1], y + (h_ * 20) - ldata->c_data[0][i], c1);
	}
	
	s_font_init(&font, "arial.ttf");
	sprintf(text, "Cpu :     1Min :     5Min :     15Min :");
	s_font_set_str(font, text);
	s_font_set_size(font, 12);
	s_font_get_glyph(font);
	s_image_get_handler(font->glyph.img);
	s_putboxrgba(surface, x,
	                      y + h - 10,
	                      font->glyph.img->w,
	                      font->glyph.img->h,
	                      font->glyph.img->rgba);
	s_font_uninit(font);
	
	x += 35;
	s_fillbox(surface, x, y + h - 5, 10, 3, c1);
	x += 55;
	s_fillbox(surface, x, y + h - 5, 10, 3, c2);
	x += 55;
	s_fillbox(surface, x, y + h - 5, 10, 3, c3);
	x += 60;
	s_fillbox(surface, x, y + h - 5, 10, 3, c4);

	return 0;
}

static int load_draw_mem_bar (s_surface_t *surface, load_data_t *ldata)
{                       
	int x;
	int y;
	int w;
	int h;
	int i;
	int c;
	int c0;
	int c1;
	int h_;
	int lvl;
	double m_;
	int m_used;
	int m_used_;
	int m_buffers;
	int m_cached;
	int m_total;
	char text[5];
	s_font_t *font;

	x = ldata->m_rect.x;
	y = ldata->m_rect.y;
	w = ldata->m_rect.w / 7;
	h = ldata->m_rect.h;

	c0 = s_rgbcolor(surface, 66, 99, 66);
	c1 = s_rgbcolor(surface, 0, 250, 24);

        lvl = 20;
        h_ = (h - 15) / lvl;

	s_fillbox(surface, x, y, w, h_ * lvl, s_rgbcolor(surface, 0, 0, 0));

	m_used = ldata->m_used / 1000;
	m_buffers = ldata->m_buffers / 1000;
	m_cached = ldata->m_cached / 1000;
	m_total = ldata->m_total / 1000;
	if (!m_total) ++m_total; // ugly
	m_used = m_used - m_buffers - m_cached;
	m_used_ = (m_used * 100) / m_total;
	
        i = 0;
	while (lvl--) {
		if (i <= m_used_ / 5) {
			c = c1;
		} else {
			c = c0;
		}
		s_fillbox(surface, x, y + (lvl * h_) + 1, w, h_ - 2, c);
		i++;
	}

	for (i = 229; i > 0; i--) {
		ldata->m_data[0][i] = ldata->m_data[0][i - 1];
		ldata->m_data[1][i] = ldata->m_data[1][i - 1];
	}
	m_ = (((double) m_used) / ((double) m_total));
	ldata->m_data[0][0] = (int) ( m_ * (double) (h_ * 20));
	if (ldata->s_total == 0) {
		m_ = 0;
	} else {
		m_ = (((double) (ldata->s_used / 1000000)) / ((double) (ldata->s_total / 1000000)));
	}
	ldata->m_data[1][0] = (int) ( m_ * (double) (h_ * 20));

	s_font_init(&font, "arial.ttf");
	sprintf(text, "%% %02d", (int) (m_used_));
	s_font_set_str(font, text);
	s_font_set_size(font, 12);
	s_font_get_glyph(font);
	s_image_get_handler(font->glyph.img);
	s_putboxrgba(surface, ldata->m_rect.x,
	                      ldata->m_rect.y + ldata->m_rect.h - 10,
	                      font->glyph.img->w,
	                      font->glyph.img->h,
	                      font->glyph.img->rgba);
	s_font_uninit(font);
	
	return 0;
}

static int load_draw_mem_graph (s_surface_t *surface, load_data_t *ldata)
{
	int i;
	int x;
	int y;
	int w;
	int h;
	int c0;
	int c1;
	int c2;
	int w_;
	int h_;
	int lvl;
	char text[50];
	s_font_t *font;

	x = ldata->m_rect.x + (ldata->m_rect.w / 7) + 10;
	y = ldata->m_rect.y;
	w = ((ldata->m_rect.w * 6) / 7) - 10;
	h = ldata->m_rect.h;

	c0 = s_rgbcolor(surface, 66, 99, 66);
	c1 = s_rgbcolor(surface, 0, 250, 24);
	c2 = s_rgbcolor(surface, 255, 255, 255);

        lvl = 20;
        h_ = (h - 15) / lvl;

	s_fillbox(surface, x, y, w, h_ * lvl, s_rgbcolor(surface, 0, 0, 0));

	while (lvl--) {
		lvl--;
		s_hline(surface, x, y + (lvl * h_), x + w - 1, c0);
	}
	lvl = 10;
	w_ = w / lvl;
	while (lvl--) {
		s_vline(surface, x + (lvl * w_), y, y + (h_ * 20) - 1, c0);
	}
	
	for (i = 0; i < 229; i++) {
		s_vline(surface, x + w - 1 - i, y + (h_ * 20) - ldata->m_data[1][i + 1], y + (h_ * 20) - ldata->m_data[1][i], c2);
		s_vline(surface, x + w - 1 - i, y + (h_ * 20) - ldata->m_data[0][i + 1], y + (h_ * 20) - ldata->m_data[0][i], c1);
	}

	s_font_init(&font, "arial.ttf");
	sprintf(text, "Mem :     Swap :");
	s_font_set_str(font, text);
	s_font_set_size(font, 12);
	s_font_get_glyph(font);
	s_image_get_handler(font->glyph.img);
	s_putboxrgba(surface, x,
	                      y + h - 10,
	                      font->glyph.img->w,
	                      font->glyph.img->h,
	                      font->glyph.img->rgba);
	s_font_uninit(font);
	
	x += 40;
	s_fillbox(surface, x, y + h - 5, 10, 3, c1);
	x += 60;
	s_fillbox(surface, x, y + h - 5, 10, 3, c2);
	
	return 0;
}

static int load_draw_load (s_window_t *window)
{
        load_data_t *ldata;
	s_surface_t *surface;

	ldata = (load_data_t *) window->data;

	if (s_surface_create(&surface, ldata->width, ldata->height, window->surface->bitsperpixel)) {
		return 0;
	}
	
	s_fillbox(surface, 0, 0, ldata->width, ldata->height, s_rgbcolor(window->surface, 222, 222, 222));

	load_draw_cpu_bar(surface, ldata);
	load_draw_cpu_graph(surface, ldata);
	load_draw_mem_bar(surface, ldata);
	load_draw_mem_graph(surface, ldata);

	s_putbox(window->surface, 0, 0, ldata->width, ldata->height, surface->vbuf);

	s_surface_destroy(surface);
	return 0;
}

static int load_get_load (s_window_t *window)
{
	FILE *fp;
	char *str;
	char buf[1000];
        load_data_t *ldata;
	unsigned long long int c_user_;
	unsigned long long int c_nice_;
	unsigned long long int c_sys_;
	unsigned long long int c_idle_;

	ldata = (load_data_t *) window->data;

	fp = fopen("/proc/stat", "r");
	while (!feof(fp)) {
		fgets(buf, sizeof(buf), fp);
		if (strncmp(buf, "cpu ", 4) == 0) {
			break;
		}
	}
        c_user_ = ldata->c_user_;
        c_nice_ = ldata->c_nice_;
        c_sys_ = ldata->c_sys_;
        c_idle_ = ldata->c_idle_;
	str = buf + 4;
	ldata->c_user_ = strtoull(str, &str, 0);
	ldata->c_nice_ = strtoull(str, &str, 0);
	ldata->c_sys_ = strtoull(str, &str, 0);
	ldata->c_idle_ = strtoull(str, &str, 0);

	if ((ldata->c_user_ == 0) &&
	    (ldata->c_nice_ == 0) &&
	    (ldata->c_sys_ == 0) &&
	    (ldata->c_idle_ == 0)) {
		return 0;
	}

	ldata->c_user = (float) (ldata->c_user_ - c_user_);
	ldata->c_nice = (float) (ldata->c_nice_ - c_nice_);
	ldata->c_sys = (float) (ldata->c_sys_ - c_sys_);
	ldata->c_idle = (float) (ldata->c_idle_ - c_idle_);
	ldata->c_total = ldata->c_user + ldata->c_nice + ldata->c_sys + ldata->c_idle;
	ldata->c_user = (ldata->c_user * 100) / ldata->c_total;
	ldata->c_nice = (ldata->c_nice * 100) / ldata->c_total;
	ldata->c_sys = (ldata->c_sys * 100) / ldata->c_total;
	ldata->c_idle = (ldata->c_idle * 100) / ldata->c_total;
	ldata->c_total = (ldata->c_total * 100) / ldata->c_total;
#ifdef DEBUG	
	printf("Cpu: %.1f %.1f %.1f %.1f %.1f\n", ldata->c_total, ldata->c_user, ldata->c_nice, ldata->c_sys, ldata->c_idle);
#endif
	fclose(fp);

	fp = fopen("/proc/loadavg", "r");
	fgets(buf, sizeof(buf), fp);
	str = buf;
	ldata->a_1min = strtod(str, &str);
	ldata->a_5min = strtod(str, &str);
	ldata->a_15min = strtod(str, &str);
	if (ldata->a_1min > 1.0) { ldata->a_1min = 1.0; }
	if (ldata->a_5min > 1.0) { ldata->a_5min = 1.0; }
	if (ldata->a_15min > 1.0) { ldata->a_15min = 1.0; }
#ifdef DEBUG	
	printf("Loadavg: %.2f %.2f %.2f\n", ldata->a_1min, ldata->a_5min, ldata->a_15min);
#endif
	fclose(fp);

	fp = fopen("/proc/meminfo", "r");
	while (!feof(fp)) {
		fgets(buf, sizeof(buf), fp);
#if defined(KERNEL_26)
		if ((str = strstr(buf, "MemTotal: ")) != NULL) {
			str = buf + 10; ldata->m_total = strtoul(str, &str, 0);
		} else if ((str = strstr(buf, "MemFree: ")) != NULL) {
			str = buf + 9; ldata->m_free = strtoul(str, &str, 0);
		} else if ((str = strstr(buf, "Buffers: ")) != NULL) {
			str = buf + 9; ldata->m_buffers = strtoul(str, &str, 0);
		} else if ((str = strstr(buf, "SwapTotal: ")) != NULL) {
			str = buf + 11; ldata->s_total = strtoul(str, &str, 0);
		} else if ((str = strstr(buf, "SwapFree: ")) != NULL) {
			str = buf + 10; ldata->s_free = strtoul(str, &str, 0);
		} else if ((str = strstr(buf, "SwapCached: ")) != NULL) {
			str = buf + 12; ldata->s_cached = strtoul(str, &str, 0);
		} else if ((str = strstr(buf, "Cached: ")) != NULL) {
			str = buf + 8; ldata->m_cached = strtoul(str, &str, 0);
		}
		ldata->m_used = ldata->m_total - ldata->m_free;
		ldata->s_used = ldata->s_total - ldata->s_free - ldata->s_cached;
#else
		if (strncmp(buf, "Mem: ", 5) == 0) {
			str = buf + 5;
			ldata->m_total = strtoul(str, &str, 0);
			ldata->m_used = strtoul(str, &str, 0);
			ldata->m_free = strtoul(str, &str, 0);
			ldata->m_shared = strtoul(str, &str, 0);
			ldata->m_buffers = strtoul(str, &str, 0);
			ldata->m_cached = strtoul(str, &str, 0);
		} else if (strncmp(buf, "Swap: ", 6) == 0) {
			str = buf + 6;
			ldata->s_total = strtoul(str, &str, 0);
			ldata->s_used = strtoul(str, &str, 0);
			ldata->s_free = strtoul(str, &str, 0);
		}
#endif
	}
#ifdef DEBUG	
	printf("Mem: %lu %lu %lu %lu %lu %lu\n", ldata->m_total, ldata->m_used, ldata->m_free, ldata->m_shared, ldata->m_buffers, ldata->m_cached);
	printf("Swap: %lu %lu %lu\n\n", ldata->s_total, ldata->s_used,ldata->s_free);
#endif	
	fclose(fp);

	load_draw_load(window);

	return 0;
}

static void load_timer (s_window_t *window, s_timer_t *timer)
{
	load_data_t *ldata;
	ldata = (load_data_t *) window->data;
	if (ldata->running) {
		load_get_load(window);
	}
}

static void load_atexit (s_window_t *window)
{
	load_data_t *ldata;

	ldata = (load_data_t *) window->data;
	ldata->running = 0;
	s_free(ldata);
}

int main (int argc, char *argv[])
{
	s_timer_t *timer;
	s_window_t *window;
	load_data_t *ldata;

	s_window_init(&window);
	s_window_new(window, WINDOW_MAIN, NULL);
	s_window_set_coor(window, WINDOW_NOFORM, 100, 100, 300, 300);
	s_window_set_resizeable(window, 0);
	s_window_set_title(window, "Xynth Load");

	s_free(window->surface->vbuf);
	window->surface->width = window->surface->buf->w;
	window->surface->height = window->surface->buf->h;
	window->surface->vbuf = (unsigned char *) s_malloc(window->surface->width * window->surface->height * window->surface->bytesperpixel);

	s_window_atexit(window, load_atexit);

	ldata = (load_data_t *) s_calloc(1, sizeof(load_data_t));
	window->data = (void *) ldata;

	ldata->running = 1;
	ldata->width = 300;
	ldata->height = 300;

	ldata->c_rect.x = 10;
	ldata->c_rect.y = 10;
	ldata->c_rect.w = ldata->width - 20;
	ldata->c_rect.h = (ldata->height / 2) - 15;

	ldata->m_rect.x = 10;
	ldata->m_rect.y = (ldata->height / 2) + 5;
	ldata->m_rect.w = ldata->width - 20;;
	ldata->m_rect.h = (ldata->height / 2) - 15;

	ldata->c_user_ = 0;
	ldata->c_nice_ = 0;
	ldata->c_sys_ = 0;
	ldata->c_idle_ = 0;

	load_get_load(window);
	s_timer_init(&timer);
	timer->timeval = 1000;
	timer->cb = load_timer;
	s_timer_add(window, timer);

	s_window_show(window);
	s_window_main(window);

	return 0;
}

#if defined(SINGLE_APP)
s_single_app_t single_load = {
	load_main,
	1,
	{"load"}
};
#endif
