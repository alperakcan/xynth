/***************************************************************************
    begin                : Tue Oct 5 2004
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

#include "desktop.h"

void taskbar_progs_handler_r (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
        int w;
        int px;
        int py;
        int pos;
        tbar_data_t *tbar_data;
        tbar_progs_t *tbar_progs;
        s_desktop_client_t *desktopc;

        tbar_data = (tbar_data_t *) window->data;
        tbar_progs = (tbar_progs_t *) tbar_data->tbar_progs;

	px = event->mouse->x - tbar_progs->rect.x;
	py = event->mouse->y - tbar_progs->rect.y;
	if (tbar_progs->desktop->clients->nb_elt <= 0) {
		return;
	}
	w = ((tbar_progs->rect.w / tbar_progs->desktop->clients->nb_elt) > 125) ? 125 : (tbar_progs->rect.w / tbar_progs->desktop->clients->nb_elt);

	if ((px > 0) && (px < w * tbar_progs->desktop->clients->nb_elt)) {
		pos = px / w;
	} else {
		return;
	}

	desktopc = (s_desktop_client_t *) s_list_get(tbar_progs->desktop->clients, pos);
	if (desktopc != NULL) {
		s_socket_request(window, SOC_DATA_SHOW, desktopc->id, 0);
	}
}

void taskbar_progs_draw_client (tbar_progs_t *tbar_progs, s_surface_t *surface, s_desktop_client_t *client, int x, int y, int w, int h)
{
        int x_;
        int w_ = 0;
        int _w = 0;
        char *text;
	s_font_t *font;

	x += 2;
	w -= 4;

	text = (char *) s_malloc(sizeof(char) * (50 + 1));
	snprintf(text, 50, "%s", client->title);

	font = tbar_progs->prog_font;
	s_font_set_str(font, text);
	s_font_set_size(font, h - 8);
	s_font_get_glyph(font);
	s_image_get_handler(font->glyph.img);

	s_fillbox(surface, x, y, w, h, s_rgbcolor(surface, 123, 121, 115));

	if (!client->pri) {
		for (x_ = w - 3; x_ >= 0; x_--) {
			s_putbox(surface, x + 1 + x_, y + 1, 1, h - 2, tbar_progs->prog_img[0]->buf);
		}
		x += 1;
		y += 1;
	} else {
		for (x_ = w - 3; x_ >= 0; x_--) {
			s_putbox(surface, x + 1 + x_, y + 1, 1, h - 2, tbar_progs->prog_img[1]->buf);
		}
	}

	y += 5;
	x += 4;
	w_ = font->glyph.img->w;

	if (font->glyph.img->w > (w - 8)) {
		w_ = w - 8;
		_w = font->glyph.img->w - (w - 8);
	}

	s_putboxpartrgba(surface, x, y, w_, font->glyph.img->h, font->glyph.img->w, font->glyph.img->h, font->glyph.img->rgba, 0, 0);

	s_free(text);
	s_image_uninit(font->glyph.img);
	s_image_init(&(font->glyph.img));
}

void taskbar_progs_draw (s_window_t *window)
{
        int x;
        int pos;
        int w = 0;
        char *vbuf;
        s_surface_t *srf;
        tbar_data_t *tbar_data;
        tbar_progs_t *tbar_progs;
        s_desktop_client_t *desktopc;

        pos = 0;
        tbar_data = (tbar_data_t *) window->data;
        tbar_progs = (tbar_progs_t *) tbar_data->tbar_progs;

	if (tbar_progs->desktop->clients->nb_elt > 0) {
		w = ((tbar_progs->rect.w / tbar_progs->desktop->clients->nb_elt) > 125) ? 125 : (tbar_progs->rect.w / tbar_progs->desktop->clients->nb_elt);
	}

	srf = (s_surface_t *) s_malloc(sizeof(s_surface_t));
        vbuf = (char *) s_malloc(window->surface->bytesperpixel * tbar_progs->rect.w * tbar_progs->rect.h + 1);
        s_getsurfacevirtual(srf, tbar_progs->rect.w, tbar_progs->rect.h, window->surface->bitsperpixel, vbuf);

	for (x = 0; x <= tbar_progs->rect.w; x++) {
		s_putboxpart(srf, x, 0, 1, tbar_progs->rect.h, 1, 30, tbar_progs->tbar_img->buf, 0, tbar_progs->rect.y);
	}

	while (!s_list_eol(tbar_progs->desktop->clients, pos)) {
		desktopc = (s_desktop_client_t *) s_list_get(tbar_progs->desktop->clients, pos);
		taskbar_progs_draw_client(tbar_progs, srf, desktopc, w * pos, 0, w, tbar_progs->rect.h);
		pos++;
	}

        s_putbox(window->surface, tbar_progs->rect.x, tbar_progs->rect.y, tbar_progs->rect.w, tbar_progs->rect.h, srf->vbuf);
        s_free(vbuf);
        s_free(srf);
}

void taskbar_start_menu_icon (s_window_t *window)
{
        char *vbuf;
        s_surface_t *srf;
        tbar_data_t *tbar_data;
        tbar_smenu_t *tbar_smenu;

        tbar_data = (tbar_data_t *) window->data;
        tbar_smenu = (tbar_smenu_t *) tbar_data->tbar_smenu;

	srf = (s_surface_t *) s_malloc(sizeof(s_surface_t));
        vbuf = (char *) s_malloc(window->surface->bytesperpixel * tbar_smenu->img->w * tbar_smenu->img->h + 1);
        s_getsurfacevirtual(srf, tbar_smenu->img->w, tbar_smenu->img->h, window->surface->bitsperpixel, vbuf);
        s_getbox(window->surface, tbar_smenu->rect.x, tbar_smenu->rect.y, tbar_smenu->img->w, tbar_smenu->img->h, srf->vbuf);
        s_putboxrgba(srf, 0, 0, tbar_smenu->img->w, tbar_smenu->img->h, tbar_smenu->img->rgba);
        s_putbox(window->surface, tbar_smenu->rect.x, tbar_smenu->rect.y, tbar_smenu->img->w, tbar_smenu->img->h, srf->vbuf);
        s_free(vbuf);
        s_free(srf);
}

void taskbar_start_menu_handler_p (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
        char *box;
        tbar_data_t *tbar_data;
        tbar_smenu_t *tbar_smenu;

        tbar_data = (tbar_data_t *) window->data;
        tbar_smenu = (tbar_smenu_t *) tbar_data->tbar_smenu;

        while (tbar_data->tbar_smenu->running) {
		usleep(20000);
	}

	box = (char *) s_malloc(tbar_smenu->rect.w * tbar_smenu->rect.h * window->surface->bytesperpixel);
	s_getbox(window->surface, tbar_smenu->rect.x, tbar_smenu->rect.y, tbar_smenu->rect.w, tbar_smenu->rect.h, box);
	s_fillbox(window->surface, tbar_smenu->rect.x, tbar_smenu->rect.y, tbar_smenu->rect.w + 2, tbar_smenu->rect.h + 2, s_rgbcolor(window->surface, 115, 117, 115));
	s_fillbox(window->surface, tbar_smenu->rect.x + 1, tbar_smenu->rect.y + 1, tbar_smenu->rect.w + 1, tbar_smenu->rect.h + 1, s_rgbcolor(window->surface, 255, 255, 255));
	s_putbox(window->surface, tbar_smenu->rect.x + 1, tbar_smenu->rect.y + 1, tbar_smenu->rect.w, tbar_smenu->rect.h, box);
	s_free(box);
	start_menu_start(window, tbar_data->tbar_smenu->progs, window->surface->buf->x, window->surface->buf->y);
}

void taskbar_start_menu_handler_rh (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
        int x;
        s_surface_t *srf;
        tbar_data_t *tbar_data;
        tbar_progs_t *tbar_progs;
        tbar_smenu_t *tbar_smenu;

        tbar_data = (tbar_data_t *) window->data;
        tbar_smenu = (tbar_smenu_t *) tbar_data->tbar_smenu;
        tbar_progs = (tbar_progs_t *) tbar_data->tbar_progs;

	srf = (s_surface_t *) s_malloc(sizeof(s_surface_t));
        srf->vbuf = (unsigned char *) s_malloc(window->surface->bytesperpixel * (tbar_smenu->rect.w + tbar_progs->tbar_img->w + 2) * (tbar_smenu->rect.h + 2));
	s_getsurfacevirtual(srf, (tbar_smenu->rect.w + tbar_progs->tbar_img->w + 2), (tbar_smenu->rect.h + 2), window->surface->bitsperpixel, srf->vbuf);
	for (x = 0; x <= tbar_smenu->rect.w + 2; x++) {
		s_putboxpart(srf, x, 0, tbar_progs->tbar_img->w, tbar_smenu->rect.h + 2, tbar_progs->tbar_img->w, tbar_progs->tbar_img->h, tbar_progs->tbar_img->buf, 0, tbar_smenu->rect.y);
	}
        s_putboxrgba(srf, 0, 0, tbar_smenu->img->w, tbar_smenu->img->h, tbar_smenu->img->rgba);
	s_putbox(window->surface, tbar_smenu->rect.x, tbar_smenu->rect.y, srf->width, srf->height, srf->vbuf);
	s_free(srf->vbuf);
	s_free(srf);
}

void taskbar_clock_popup_atexit (s_window_t *window)
{
        tbar_data_t *tbar_data;
        tbar_clock_t *tbar_clock;
        tbar_data = (tbar_data_t *) window->parent->data;
        tbar_clock = (tbar_clock_t *) tbar_data->tbar_clock;
	tbar_clock->clock = NULL;
}

void taskbar_clock_popup_atevent (s_window_t *window, s_event_t *event)
{
	int x;
	int y;
	tbar_data_t *tbar_data;
	tbar_clock_t *tbar_clock;
	if (event->type & MOUSE_EVENT) {
		tbar_data = (tbar_data_t *) window->parent->data;
		tbar_clock = (tbar_clock_t *) tbar_data->tbar_clock;
		x = event->mouse->x - window->parent->surface->buf->x;
		y = event->mouse->y - window->parent->surface->buf->y;
		if (!((x >= tbar_clock->rect.x) &&
		      (y >= tbar_clock->rect.y) &&
		      (x <= (tbar_clock->rect.x + tbar_clock->rect.w - 1)) &&
		      (y <= (tbar_clock->rect.y + tbar_clock->rect.h - 1)))) {
			s_window_quit(window);
		}
	}
}

void taskbar_clock_handler_oh (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
        tbar_data_t *tbar_data;
        tbar_clock_t *tbar_clock;
	tbar_data = (tbar_data_t *) window->data;
	tbar_clock = tbar_data->tbar_clock;
	if (tbar_clock->clock == NULL) {
		return;
	}
	s_window_quit(tbar_clock->clock);
}

void taskbar_clock_handler_o (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	time_t t_;
	char *text;
	struct tm *t;
        int wday =  0;
        s_font_t *font;
	s_window_t *temp;
        tbar_data_t *tbar_data;
        tbar_clock_t *tbar_clock;
	int mon[] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};
	char *days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
	char *mons[] = {"January", "February", "March", "April", "May", "June", "July",
	                "August", "September", "October", "November", "December"};

	tbar_data = (tbar_data_t *) window->data;
	tbar_clock = tbar_data->tbar_clock;

	if (tbar_clock->clock != NULL) {
		return;
	}

	t_ = time(NULL);
        t = localtime(&t_);
	text = (char *) s_malloc(sizeof(char) * 256);

	s_window_init(&temp);
        s_window_new(temp, WINDOW_TEMP | WINDOW_NOFORM, window);
        s_window_set_alwaysontop(temp, 1);

	/* W = C + Y + L + M + D (mod 7)
	   Where:
	   W is the day of the week (0 = Sunday, through 6 = Saturday)
	   C is a code for the century from this table (for the Gregorian calendar only):
	   	1400s, 1800s, 2200s	2	1800 is not a leap year
	   	1500s, 1900s, 2300s     0       1900 is not a leap year
	   	1600s, 2000s, 2400s     5       2000 is a leap year
	   	1700s, 2100s, 2500s     4       2100 is not a leap year
	   Y is the last two digits of the year.
	   L is the number of leap days since the beginning of the century.
	   	Step 1:	Divide the year (two digits) by 4 and throw away the fraction.
	    	Step 2:	Notice that 1900 and 1800 were not leap years, and 2000 was. Only century
	     		years divisible by 400 are leap years. So, add 1 for those centuries
	       		divisible by 4 (as we haven't counted the leap day for year 00 yet).
		Step 3:	Also, don't count a leap day if it happens after the date that you are
			calculating. In other words subtract one, if you are calculating a date of
			January or February of a leap year.
	   M is the code for the month, from this table:
	   	1. Jan. 0	5. May  1	9.  Sep. 5
	    	2. Feb. 3	6. June 4	10. Oct. 0
	     	3. Mar. 3	7. July 6	11. Nov. 3
	        4. Apr. 6	8. Aug. 2	12. Dec. 5
	   D is the date (day of month).
	*/
        switch ((t->tm_year + 1900) / 100) {
		case 14: case 18: case 22: wday = 2; break;
		case 15: case 19: case 23: wday = 0; break;
		case 16: case 20: case 24: wday = 5; break;
		case 17: case 21: case 25: wday = 4; break;
	}
	wday += t->tm_year % 100;
	wday += ((t->tm_year % 100) / 4);
	wday += ((((t->tm_year + 1900) / 100) % 4) == 0) ? 1 : 0;
	wday += mon[t->tm_mon];
	wday += t->tm_mday;
	wday %= 7;

        sprintf(text, "%s %d %s %d", days[wday], t->tm_mday, mons[t->tm_mon], t->tm_year + 1900);

	s_font_init(&font, "arial.ttf");
	s_font_set_str(font, text);
	s_font_set_size(font, 12);
	s_font_get_glyph(font);
	s_image_get_handler(font->glyph.img);

	s_window_set_coor(temp, 0, event->mouse->x - font->glyph.img->handler->w - 8,
	                           event->mouse->y - font->glyph.img->handler->h - 8,
	                           font->glyph.img->handler->w + 8,
	                           font->glyph.img->handler->h + 8);
	s_fillbox(temp->surface, 0, 0, temp->surface->buf->w, temp->surface->buf->h, s_rgbcolor(temp->surface, 0, 0, 0));
	s_fillbox(temp->surface, 1, 1, temp->surface->buf->w - 2, temp->surface->buf->h - 2, s_rgbcolor(temp->surface, 255, 255, 222));
	s_putboxrgba(temp->surface, 4, 4, font->glyph.img->w, font->glyph.img->h, font->glyph.img->rgba);
	s_font_uninit(font);
	s_free(text);

	s_window_atevent(temp, taskbar_clock_popup_atevent);
	s_window_atexit(temp, taskbar_clock_popup_atexit);

	s_window_show(temp);
	s_window_main(temp);

	tbar_clock->clock = temp;
}

void taskbar_clock_draw (s_window_t *window, s_timer_t *timer)
{
	int w_;
	time_t t_;
	struct tm *t;
	int _w = 0;
        char *vbuf;
        s_surface_t *srf;
        tbar_data_t *tbar_data;
        tbar_clock_t *tbar_clock;
	int c0 = s_rgbcolor(window->surface, 96, 96, 96);
	int c1 = s_rgbcolor(window->surface, 255, 255, 255);
	int c2 = s_rgbcolor(window->surface, 220, 220, 220);

	tbar_data = (tbar_data_t *) window->data;
	tbar_clock = tbar_data->tbar_clock;

	t_ = time(NULL);
        t = localtime(&t_);

	vbuf = (char *) s_malloc(sizeof(char) * 10);
	if (t->tm_sec & 1) {
		sprintf(vbuf, "%02d:%02d ", t->tm_hour, t->tm_min);
	} else {
		sprintf(vbuf, "%02d %02d ", t->tm_hour, t->tm_min);
	}
	s_font_set_str(tbar_clock->font, vbuf);
	s_free(vbuf);

	s_font_get_glyph(tbar_clock->font);

        vbuf = (char *) s_malloc(window->surface->bytesperpixel * tbar_clock->rect.w * tbar_clock->rect.h + 1);
	srf = (s_surface_t *) s_malloc(sizeof(s_surface_t));
        s_getsurfacevirtual(srf, tbar_clock->rect.w, tbar_clock->rect.h, window->surface->bitsperpixel, vbuf);

	s_fillbox(srf, 0, 0, tbar_clock->rect.w, tbar_clock->rect.h, c0);
	s_fillbox(srf, 1, 1, tbar_clock->rect.w - 1, tbar_clock->rect.h - 1, c1);
	s_fillbox(srf, 1, 1, tbar_clock->rect.w - 2, tbar_clock->rect.h - 2, c2);

	w_ = tbar_clock->font->glyph.img->w;
	if (tbar_clock->font->glyph.img->w > (tbar_clock->rect.w - 6)) {
		w_ = tbar_clock->rect.w - 6;
		_w = tbar_clock->font->glyph.img->w - (tbar_clock->rect.w - 6);
	}
	s_putboxpartrgba(srf, 3, 4, w_, tbar_clock->font->glyph.img->h, tbar_clock->font->glyph.img->w, tbar_clock->font->glyph.img->h, tbar_clock->font->glyph.img->rgba, 0, 0);

        s_putbox(window->surface, tbar_clock->rect.x, tbar_clock->rect.y, tbar_clock->rect.w, tbar_clock->rect.h, srf->vbuf);
        s_free(vbuf);
        s_free(srf);
        return;
}

void taskbar_clean_smenu (s_list_t *list)
{
	smenu_prog_t *p;
	while (!s_list_eol(list, 0)) {
		p = s_list_get(list, 0);
		s_list_remove(list, 0);
		s_free(p->name);
		s_free(p->icon);
		s_free(p->menu);
		s_free(p->exec);
		taskbar_clean_smenu(p->progs);
		s_free(p);
	}
	s_free(list);
}

void taskbar_atexit (s_window_t *window)
{
        tbar_data_t *tbar_data;
        tbar_progs_t *tbar_progs;
        tbar_clock_t *tbar_clock;
        s_desktop_client_t *desktopc;

        tbar_data = (tbar_data_t *) window->data;
        tbar_progs = (tbar_progs_t *) tbar_data->tbar_progs;
        tbar_clock = (tbar_clock_t *) tbar_data->tbar_clock;

        s_font_uninit(tbar_clock->font);
        s_font_uninit(tbar_progs->prog_font);
        s_image_uninit(tbar_progs->tbar_img);
        s_image_uninit(tbar_progs->prog_img[0]);
        s_image_uninit(tbar_progs->prog_img[1]);

	while (!s_list_eol(tbar_progs->desktop->clients, 0)) {
		desktopc = (s_desktop_client_t *) s_list_get(tbar_progs->desktop->clients, 0);
		s_list_remove(tbar_progs->desktop->clients, 0);
		s_free(desktopc->title);
		s_free(desktopc);
	}
	s_free(tbar_progs->desktop->clients);
	s_free(tbar_progs->desktop);
	s_free(tbar_progs);
	s_free(tbar_data->tbar_clock);
	taskbar_clean_smenu(tbar_data->tbar_smenu->progs);
	s_image_uninit(tbar_data->tbar_smenu->img);
	s_free(tbar_data->tbar_smenu);

	s_free(tbar_data);
}

void taskbar_atevent (s_window_t *window, s_event_t *event)
{
        int pos = 0;
        tbar_data_t *tbar_data;
        tbar_progs_t *tbar_progs;
        s_desktop_client_t *desktopc;
	
	if (event->type & MOUSE_EXITED) {
	}

	if (!(event->type & DESKTOP_EVENT)) {
		return;
	}

        tbar_data = (tbar_data_t *) window->data;
        tbar_progs = (tbar_progs_t *) tbar_data->tbar_progs;

	while (!s_list_eol(tbar_progs->desktop->clients, 0)) {
		desktopc = (s_desktop_client_t *) s_list_get(tbar_progs->desktop->clients, 0);
		s_list_remove(tbar_progs->desktop->clients, 0);
		s_free(desktopc->title);
		s_free(desktopc);
	}
	while (!s_list_eol(event->desktop->clients, pos)) {
		desktopc = (s_desktop_client_t *) s_list_get(event->desktop->clients, pos);
		if ((desktopc->id != window->id) &&
		    (desktopc->id != window->parent->id)) {
			s_list_remove(event->desktop->clients, pos);
			s_list_add(tbar_progs->desktop->clients, desktopc, -1);
		} else {
			pos++;
		}
	}
	taskbar_progs_draw(window);
}

void taskbar_start (s_window_t *window, s_config_t *cfg)
{
        int x;
	s_timer_t *timer;
	s_handler_t *hndl;
        tbar_data_t *tbar_data;

	s_window_set_coor(window, WINDOW_NOFORM, 0, window->surface->height - 30, window->surface->width, 30);
	s_window_set_alwaysontop(window, 1);
	s_window_set_resizeable(window, 0);
	s_window_atevent(window, taskbar_atevent);
	s_window_atexit(window, taskbar_atexit);

	s_free(window->surface->vbuf);
	window->surface->width = window->surface->buf->w;
	window->surface->height = window->surface->buf->h;
	window->surface->vbuf = (unsigned char *) s_malloc(window->surface->width * window->surface->height * window->surface->bytesperpixel);

        tbar_data = (tbar_data_t *) s_malloc(sizeof(tbar_data_t));
        tbar_data->tbar_clock = (tbar_clock_t *) s_malloc(sizeof(tbar_clock_t));
        tbar_data->tbar_smenu = (tbar_smenu_t *) s_malloc(sizeof(tbar_smenu_t));
	s_list_init(&(tbar_data->tbar_smenu->progs));
	s_image_init(&(tbar_data->tbar_smenu->img));
	s_image_img(DESKTOPDIR "/img/icons/xynth.png", tbar_data->tbar_smenu->img);
	s_image_get_handler(tbar_data->tbar_smenu->img);
	tbar_data->tbar_smenu->running = 0;

        tbar_data->tbar_progs = (tbar_progs_t *) s_malloc(sizeof(tbar_progs_t));
	tbar_data->tbar_progs->desktop = (s_desktop_t *) s_malloc(sizeof(s_desktop_t));
	s_list_init(&(tbar_data->tbar_progs->desktop->clients));

	s_font_init(&(tbar_data->tbar_progs->prog_font), "arial.ttf");

	s_image_init(&(tbar_data->tbar_progs->tbar_img));
	s_image_img(DESKTOPDIR "/img/widget/taskbar.png", tbar_data->tbar_progs->tbar_img);
	s_image_get_buf(window->surface, tbar_data->tbar_progs->tbar_img);

	s_image_init(&(tbar_data->tbar_progs->prog_img[0]));
	s_image_img(DESKTOPDIR "/img/widget/button4.png", tbar_data->tbar_progs->prog_img[0]);
	s_image_get_buf(window->surface, tbar_data->tbar_progs->prog_img[0]);

	s_image_init(&(tbar_data->tbar_progs->prog_img[1]));
	s_image_img(DESKTOPDIR "/img/widget/button3.png", tbar_data->tbar_progs->prog_img[1]);
	s_image_get_buf(window->surface, tbar_data->tbar_progs->prog_img[1]);

	window->data = (void *) tbar_data;

	s_fillbox(window->surface, 0, 0, window->surface->buf->w, window->surface->buf->h, s_rgbcolor(window->surface, 255, 255, 255));
	s_fillbox(window->surface, 1, 1, window->surface->buf->w - 1, window->surface->buf->h - 1, s_rgbcolor(window->surface, 115, 117, 115));

	for (x = 1; x <= window->surface->buf->w; x++) {
		s_putboxpart(window->surface, x, 1, 1, window->surface->buf->h - 2, 1, 30, tbar_data->tbar_progs->tbar_img->buf, 0, 1);
	}

	tbar_data->tbar_clock->clock = NULL;
        tbar_data->tbar_clock->rect.x = window->surface->width - 3 - 75;
        tbar_data->tbar_clock->rect.y = 3;
        tbar_data->tbar_clock->rect.w = 75;
        tbar_data->tbar_clock->rect.h = window->surface->height - 6;
	s_font_init(&(tbar_data->tbar_clock->font), "veramono.ttf");
	s_font_set_size(tbar_data->tbar_clock->font, tbar_data->tbar_clock->rect.h);

        tbar_data->tbar_smenu->rect.x = 3;
        tbar_data->tbar_smenu->rect.y = 3;
        tbar_data->tbar_smenu->rect.w = window->surface->height - 6;
        tbar_data->tbar_smenu->rect.h = window->surface->height - 6;

	tbar_data->tbar_progs->rect.x = 3 + tbar_data->tbar_smenu->rect.w + 3;
	tbar_data->tbar_progs->rect.y = 3;
	tbar_data->tbar_progs->rect.w = window->surface->width - 3 - tbar_data->tbar_smenu->rect.w - 3 - 3 - tbar_data->tbar_clock->rect.w - 3;
	tbar_data->tbar_progs->rect.h = window->surface->height - 6;

	s_handler_init(&hndl);
	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = tbar_data->tbar_progs->rect.x;
	hndl->mouse.y = tbar_data->tbar_progs->rect.y;
	hndl->mouse.w = tbar_data->tbar_progs->rect.w;
	hndl->mouse.h = tbar_data->tbar_progs->rect.h;
	hndl->mouse.r = taskbar_progs_handler_r;
	hndl->mouse.button = MOUSE_LEFTBUTTON;
	s_handler_add(window, hndl);

	s_handler_init(&hndl);
	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = tbar_data->tbar_smenu->rect.x;
	hndl->mouse.y = tbar_data->tbar_smenu->rect.y;
	hndl->mouse.w = tbar_data->tbar_smenu->rect.w;
	hndl->mouse.h = tbar_data->tbar_smenu->rect.h;
	hndl->mouse.p = taskbar_start_menu_handler_p;
	hndl->mouse.button = MOUSE_LEFTBUTTON;
	s_handler_add(window, hndl);
	taskbar_start_menu_icon(window);

	s_handler_init(&hndl);
	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = tbar_data->tbar_clock->rect.x;
	hndl->mouse.y = tbar_data->tbar_clock->rect.y;
	hndl->mouse.w = tbar_data->tbar_clock->rect.w;
	hndl->mouse.h = tbar_data->tbar_clock->rect.h;
	hndl->mouse.o = taskbar_clock_handler_o;
	hndl->mouse.oh = taskbar_clock_handler_oh;
	hndl->data = NULL;
	s_handler_add(window, hndl);
	
	start_menu_setup(window, cfg);

	s_timer_init(&timer);
	timer->timeval = 1000;
	timer->cb = taskbar_clock_draw;
	s_timer_add(window, timer);
	taskbar_clock_draw(window, timer);
	
	s_window_show(window);
	s_window_main(window);
}
