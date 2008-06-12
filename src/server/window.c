/***************************************************************************
    begin                : Sun Feb 23 2003
    copyright            : (C) 2003 - 2008 by Alper Akcan
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

#include "../lib/xynth_.h"
#include "server.h"

void s_server_window_new (int id)
{
	int v = (s_server_id_pri(id) <= 0) ? 1 : 0;

	xynth_server->client[id].win.x = xynth_server->client[id].buf.x;
	xynth_server->client[id].win.y = xynth_server->client[id].buf.y;

	if (xynth_server->client[id].type & WINDOW_TYPE_NOFORM) {
		xynth_server->client[id].win.w = xynth_server->client[id].buf.w;
		xynth_server->client[id].win.h = xynth_server->client[id].buf.h;
	} else {
		xynth_server->client[id].win.w = xynth_server->client[id].buf.w + (xynth_server->theme.form[v][LEFT].w + xynth_server->theme.form[v][RIGHT].w);
		xynth_server->client[id].win.h = xynth_server->client[id].buf.h + (xynth_server->theme.form[v][TOP_1].h + xynth_server->theme.form[v][BTM].h);
	}

        /* if the caller does not know what he/she is doing we will correct his/her shit */
	xynth_server->client[id].win.x = (xynth_server->client[id].win.x < 0) ? 0 : xynth_server->client[id].win.x;
	xynth_server->client[id].win.y = (xynth_server->client[id].win.y < 0) ? 0 : xynth_server->client[id].win.y;

        if (xynth_server->client[id].type & WINDOW_TYPE_NOFORM) {
		if (xynth_server->client[id].win.w < 0) {
			xynth_server->client[id].win.w = 0;
		}
		if (xynth_server->client[id].win.h < 0) {
			xynth_server->client[id].win.h = 0;
		}
	} else {
		if (xynth_server->client[id].win.w < xynth_server->theme.form_min.w_) {
			xynth_server->client[id].win.w = xynth_server->theme.form_min.w_;
		}
		if (xynth_server->client[id].win.h < xynth_server->theme.form_min.h) {
			xynth_server->client[id].win.h = xynth_server->theme.form_min.h;
		}
	}

        if (xynth_server->client[id].type & WINDOW_TYPE_NOFORM) {
		xynth_server->client[id].win.x = (xynth_server->client[id].win.x > xynth_server->window->surface->width) ? 0 : xynth_server->client[id].win.x;
		xynth_server->client[id].win.y = (xynth_server->client[id].win.y > xynth_server->window->surface->height) ? 0 : xynth_server->client[id].win.y;
	} else {
		xynth_server->client[id].win.x = (xynth_server->client[id].win.x > xynth_server->window->surface->width - xynth_server->theme.form_min.w) ? 0 : xynth_server->client[id].win.x;
		xynth_server->client[id].win.y = (xynth_server->client[id].win.y > xynth_server->window->surface->height - xynth_server->theme.form_min.h) ? 0 : xynth_server->client[id].win.y;
	}

	/* coor&dim correcting, done. */

        if (xynth_server->client[id].type & WINDOW_TYPE_NOFORM) {
		xynth_server->client[id].buf.x = xynth_server->client[id].win.x;
		xynth_server->client[id].buf.y = xynth_server->client[id].win.y;
		xynth_server->client[id].buf.w = xynth_server->client[id].win.w;
		xynth_server->client[id].buf.h = xynth_server->client[id].win.h;
	} else {
		xynth_server->client[id].buf.x = xynth_server->client[id].win.x + xynth_server->theme.form[v][LEFT].w;
		xynth_server->client[id].buf.y = xynth_server->client[id].win.y + xynth_server->theme.form[v][TOP_1].h;
		xynth_server->client[id].buf.w = xynth_server->client[id].win.w - xynth_server->theme.form[v][LEFT].w - xynth_server->theme.form[v][RIGHT].w;
		xynth_server->client[id].buf.h = xynth_server->client[id].win.h - xynth_server->theme.form[v][TOP_1].h - xynth_server->theme.form[v][BTM].h;
	}

	xynth_server->client[id].title.str = (char *) s_malloc(sizeof(char) * 23);
	sprintf(xynth_server->client[id].title.str, "Xynth Windowing System");
	s_server_window_title(id, xynth_server->client[id].title.str);
}

void s_server_window_title (int id, char *title)
{
	int i;
        int v;
        int h;
        int yo;
	s_font_t *font;
	s_surface_t *srf;

        if ((id < 0) ||
	    (title == NULL) ||
	    (xynth_server->client[id].type & WINDOW_TYPE_NOFORM)) {
		return;
	}
	
        for (v = 0; v < 2; v++) {
		font = xynth_server->theme.font[v];
		s_font_set_str(font, title);
		s_font_get_glyph(font);
		s_image_get_mat(font->glyph.img);
		s_image_get_handler(font->glyph.img);

		font->glyph.img->buf = (char *) s_calloc(1, font->glyph.img->w * font->glyph.img->h * xynth_server->window->surface->bytesperpixel);
		if (s_surface_create_from_data(&srf, font->glyph.img->w, font->glyph.img->h, xynth_server->window->surface->bitsperpixel, font->glyph.img->buf)) {
			goto out;
		}

		if ((i = font->glyph.img->w / xynth_server->theme.form[v][TOP_3].w) > 0) {
			while (i--) {
				yo = xynth_server->theme.text_v_off[v] - font->glyph.img->handler->y + (xynth_server->theme.form[v][TOP_3].handler->h - font->glyph.img->handler->h) / 2;
				if ((font->glyph.img->h + yo) > xynth_server->theme.form[v][TOP_3].h) {
					h = xynth_server->theme.form[v][TOP_3].h - yo;
				} else {
					h = font->glyph.img->h;
				}
				s_putboxpart(srf, i * xynth_server->theme.form[v][TOP_3].w, 0, xynth_server->theme.form[v][TOP_3].w, h, xynth_server->theme.form[v][TOP_3].w, xynth_server->theme.form[v][TOP_3].h, xynth_server->theme.form[v][TOP_3].buf, 0, yo);
			}
		}
		if ((i = font->glyph.img->w % xynth_server->theme.form[v][TOP_3].w) > 0) {
				s_putboxpart(srf, font->glyph.img->w - xynth_server->theme.form[v][TOP_3].w, 0, xynth_server->theme.form[v][TOP_3].w, font->glyph.img->h, xynth_server->theme.form[v][TOP_3].w, xynth_server->theme.form[v][TOP_3].h, xynth_server->theme.form[v][TOP_3].buf, 0, xynth_server->theme.text_v_off[v] - font->glyph.img->handler->y + (xynth_server->theme.form[v][TOP_3].handler->h - font->glyph.img->handler->h) / 2);
		}
		s_putboxrgba(srf, 0, 0, font->glyph.img->w, font->glyph.img->h, font->glyph.img->rgba);

		s_free(xynth_server->client[id].title.img[v].mat);
		s_free(xynth_server->client[id].title.img[v].buf);
		xynth_server->client[id].title.img[v].mat = (unsigned char *) s_malloc(font->glyph.img->w * font->glyph.img->h);
		xynth_server->client[id].title.img[v].buf = (char *) s_malloc(font->glyph.img->w * font->glyph.img->h * xynth_server->window->surface->bytesperpixel);
		xynth_server->client[id].title.img[v].w = font->glyph.img->w;
		xynth_server->client[id].title.img[v].h = font->glyph.img->h;
		xynth_server->client[id].title.hy[v] = font->glyph.img->handler->y;
		xynth_server->client[id].title.hh[v] = font->glyph.img->handler->h;
		memcpy(xynth_server->client[id].title.img[v].mat, font->glyph.img->mat, font->glyph.img->w * font->glyph.img->h);
		memcpy(xynth_server->client[id].title.img[v].buf, font->glyph.img->buf, font->glyph.img->w * font->glyph.img->h * xynth_server->window->surface->bytesperpixel);
		s_surface_destroy(srf);
out:		s_image_uninit(font->glyph.img);
		s_image_init(&(font->glyph.img));
	}
}

void s_server_putbox (s_window_t *window, int id, s_rect_t *coor, int x, int y, s_image_t *img)
{
        int add;
	s_rect_t to;
	s_rect_t intr;
	s_rect_t icoor;

	icoor.x = x;
	icoor.y = y;
	icoor.w = img->w;
	icoor.h = img->h;

	if (s_region_rect_intersect(coor, &icoor, &intr)) {
		return;
	}
	if (s_region_rect_intersect(window->surface->buf, &intr, &to)) {
		return;
	}
	add = (to.y - icoor.y) * img->w + (to.x - icoor.x);
	bpp_putbox_mask_o(window->surface, id, to.x, to.y, to.w, to.h, img->buf + add * window->surface->bytesperpixel, img->mat + add, img->w);
}

void s_server_putmat (s_window_t *window, int id, s_rect_t *coor, int x, int y, s_image_t *img)
{
        s_rect_t mcoor;
	mcoor.x = x;
	mcoor.y = y;
	mcoor.w = img->w;
	mcoor.h = img->h;
	s_server_surface_matrix_add_this(id, coor, &mcoor, img->mat);
}

int s_server_window_form_mat_verbose (int id)
{
	int i = s_server_pri_id(0);
	if ((id < 0) ||
	    (xynth_server->client[id].type & WINDOW_TYPE_NOFORM) ||
	    (s_server_id_pri(id) < 0)) {
		return -1;
	}
	if ((s_server_id_pri(id) == 0) ||
	    ((i >= 0) &&
	     (xynth_server->client[i].type & WINDOW_TYPE_TEMP) &&
	     (s_server_window_is_parent_temp(id, i)))) {
		return 1;
        }
        return 0;
}

void s_server_window_form_mat (int v, int id, s_rect_t *coor, void (*func) (s_window_t *, int, s_rect_t *, int, int, s_image_t *))
{
	int i;
	/* top */
	func(xynth_server->window, id, coor, xynth_server->client[id].form[TOP_L].x, xynth_server->client[id].form[TOP_L].y, &xynth_server->theme.form[v][TOP_L]);
	for (i = 0; i < xynth_server->client[id].form[TOP_1].w; i += xynth_server->theme.form[v][TOP_1].w) {
		func(xynth_server->window, id, coor, xynth_server->client[id].form[TOP_1].x + i, xynth_server->client[id].form[TOP_1].y, &xynth_server->theme.form[v][TOP_1]);
	}
	if (xynth_server->client[id].win.w >= xynth_server->theme.form_min.w) {
		func(xynth_server->window, id, coor, xynth_server->client[id].form[TOP_2].x, xynth_server->client[id].form[TOP_2].y, &xynth_server->theme.form[v][TOP_2]);
		if ((i = xynth_server->client[id].form[TOP_3].w / xynth_server->theme.form[v][TOP_3].w) > 0) {
			while (i--) {
				func(xynth_server->window, id, coor, xynth_server->client[id].form[TOP_3].x + i * xynth_server->theme.form[v][TOP_3].w, xynth_server->client[id].form[TOP_3].y, &xynth_server->theme.form[v][TOP_3]);
			}
		}
		if (xynth_server->client[id].form[TOP_3].w % xynth_server->theme.form[v][TOP_3].w) {
			func(xynth_server->window, id, coor, xynth_server->client[id].form[TOP_4].x - xynth_server->theme.form[v][TOP_3].w, xynth_server->client[id].form[TOP_3].y, &xynth_server->theme.form[v][TOP_3]);
		}
		func(xynth_server->window, id, coor, xynth_server->client[id].form[TOP_4].x, xynth_server->client[id].form[TOP_4].y, &xynth_server->theme.form[v][TOP_4]);
	}
	if ((i = xynth_server->client[id].form[TOP_5].w / xynth_server->theme.form[v][TOP_5].w) > 0) {
		while (i--) {
			func(xynth_server->window, id, coor, xynth_server->client[id].form[TOP_5].x + i * xynth_server->theme.form[v][TOP_5].w, xynth_server->client[id].form[TOP_5].y, &xynth_server->theme.form[v][TOP_5]);
		}
	}
	if (xynth_server->client[id].form[TOP_5].w % xynth_server->theme.form[v][TOP_5].w) {
		func(xynth_server->window, id, coor, xynth_server->client[id].form[TOP_R].x - xynth_server->theme.form[v][TOP_5].w, xynth_server->client[id].form[TOP_5].y, &xynth_server->theme.form[v][TOP_5]);
	}
	func(xynth_server->window, id, coor, xynth_server->client[id].form[TOP_R].x, xynth_server->client[id].form[TOP_R].y, &xynth_server->theme.form[v][TOP_R]);
	/* left */
	if ((i = xynth_server->client[id].form[LEFT].h / xynth_server->theme.form[v][LEFT].h) > 0) {
		while (i--) {
			func(xynth_server->window, id, coor, xynth_server->client[id].form[LEFT].x, xynth_server->client[id].form[LEFT].y + i * xynth_server->theme.form[v][LEFT].h, &xynth_server->theme.form[v][LEFT]);
		}
	}
	if (xynth_server->client[id].form[LEFT].h % xynth_server->theme.form[v][LEFT].h) {
		func(xynth_server->window, id, coor, xynth_server->client[id].form[LEFT].x, xynth_server->client[id].form[BTM_L].y - xynth_server->theme.form[v][LEFT].h, &xynth_server->theme.form[v][LEFT]);
	}
        /* right */
	if ((i = xynth_server->client[id].form[RIGHT].h / xynth_server->theme.form[v][RIGHT].h) > 0) {
		while (i--) {
			func(xynth_server->window, id, coor, xynth_server->client[id].form[RIGHT].x, xynth_server->client[id].form[RIGHT].y + i * xynth_server->theme.form[v][RIGHT].h, &xynth_server->theme.form[v][RIGHT]);
		}
	}
	if (xynth_server->client[id].form[RIGHT].h % xynth_server->theme.form[v][RIGHT].h) {
		func(xynth_server->window, id, coor, xynth_server->client[id].form[RIGHT].x, xynth_server->client[id].form[BTM_R].y - xynth_server->theme.form[v][RIGHT].h, &xynth_server->theme.form[v][RIGHT]);
	}
        /* buttom */
        func(xynth_server->window, id, coor, xynth_server->client[id].form[BTM_L].x, xynth_server->client[id].form[BTM_L].y, &xynth_server->theme.form[v][BTM_L]);
        if ((i = xynth_server->client[id].form[BTM].w / xynth_server->theme.form[v][BTM].w) > 0) {
		while (i--) {
			func(xynth_server->window, id, coor, xynth_server->client[id].form[BTM].x + i * xynth_server->theme.form[v][BTM].w, xynth_server->client[id].form[BTM].y, &xynth_server->theme.form[v][BTM]);
		}
	}
	if (xynth_server->client[id].form[BTM].w % xynth_server->theme.form[v][BTM].w) {
		func(xynth_server->window, id, coor, xynth_server->client[id].form[BTM_R].x - xynth_server->theme.form[v][BTM].w, xynth_server->client[id].form[BTM].y, &xynth_server->theme.form[v][BTM]);
	}
        func(xynth_server->window, id, coor, xynth_server->client[id].form[BTM_R].x, xynth_server->client[id].form[BTM_R].y, &xynth_server->theme.form[v][BTM_R]);
}

void s_server_window_form (int id, s_rect_t *_coor_)
{
	int v = 0;
	s_rect_t coort;
	s_rect_t intersect;

	s_rect_t coor;
	s_rect_t coor_ = xynth_server->client[id].win;

	v = s_server_window_form_mat_verbose(id);
	if (v < 0) {
		return;
	}

	if (s_region_rect_intersect(_coor_, &coor_, &coor)) {
		return;
	}

        coort.x = xynth_server->client[id].form[TOP_3].x;
        coort.y = xynth_server->client[id].form[TOP_3].y;
        coort.w = xynth_server->client[id].form[TOP_3].w;
        coort.h = xynth_server->theme.form[v][TOP_3].handler->h;

	s_server_window_form_mat(v, id, &coor, s_server_putbox);

	/* buttons */
	s_server_putbox(xynth_server->window, id, &coor, xynth_server->client[id].button[MENU].x, xynth_server->client[id].button[MENU].y, &xynth_server->theme.button[v][MENU]);
	s_server_putbox(xynth_server->window, id, &coor, xynth_server->client[id].button[HIDE].x, xynth_server->client[id].button[HIDE].y, &xynth_server->theme.button[v][HIDE]);
	s_server_putbox(xynth_server->window, id, &coor, xynth_server->client[id].button[CLOSE].x, xynth_server->client[id].button[CLOSE].y, &xynth_server->theme.button[v][CLOSE]);
	if (xynth_server->client[id].resizeable == 1) {
		s_server_putbox(xynth_server->window, id, &coor, xynth_server->client[id].button[MAXIMIZE].x, xynth_server->client[id].button[MAXIMIZE].y, &xynth_server->theme.button[v][MAXIMIZE]);
	}

	/* title */
	if ((!s_region_rect_intersect(&coor, &coort, &intersect)) && (xynth_server->client[id].win.w > xynth_server->theme.form_min.w)) {
		s_server_putbox(xynth_server->window, id, &intersect, coort.x, coort.y + xynth_server->theme.text_v_off[v] - xynth_server->client[id].title.hy[v] + (xynth_server->theme.form[v][TOP_3].handler->h - xynth_server->client[id].title.hh[v]) / 2, &xynth_server->client[id].title.img[v]);
	}
}

void s_server_window_matrix_add (int id, s_rect_t *_coor_)
{
	int v = 0;
	s_rect_t coor;
	s_rect_t coor_ = xynth_server->client[id].win;

	v = s_server_window_form_mat_verbose(id);
	if (v < 0) {
		return;
	}

	if (s_region_rect_intersect(_coor_, &coor_, &coor)) {
		return;
	}

	s_server_window_form_mat(v, id, &coor, s_server_putmat);
}

void s_server_window_calculate (int id)
{
        int i;
	int v = 0;
	int title_len;
	int btns_w;

	v = s_server_window_form_mat_verbose(id);
	if (v < 0) {
		return;
	}

        title_len = xynth_server->client[id].title.img[v].w;
        btns_w = xynth_server->theme.button[v][HIDE].w + xynth_server->theme.button[v][CLOSE].w;
        if (xynth_server->client[id].resizeable == 1) {
		btns_w += xynth_server->theme.button[v][MAXIMIZE].w;
	}

	xynth_server->client[id].win.x = xynth_server->client[id].buf.x - xynth_server->theme.form[v][LEFT].w;
	xynth_server->client[id].win.y = xynth_server->client[id].buf.y - xynth_server->theme.form[v][TOP_1].h;
	if ((xynth_server->client[id].win.w = xynth_server->client[id].buf.w + xynth_server->theme.form[v][LEFT].w + xynth_server->theme.form[v][RIGHT].w) < xynth_server->theme.form_min.w_) {
		xynth_server->client[id].buf.w += xynth_server->theme.form_min.w_ - xynth_server->client[id].win.w;
		xynth_server->client[id].win.w += xynth_server->theme.form_min.w_ - xynth_server->client[id].win.w;
	}
	if ((xynth_server->client[id].win.h = xynth_server->client[id].buf.h + xynth_server->theme.form[v][TOP_1].h + xynth_server->theme.form[v][BTM].h) < xynth_server->theme.form_min.h) {
		xynth_server->client[id].buf.h += xynth_server->theme.form_min.h - xynth_server->client[id].win.h;
		xynth_server->client[id].win.h += xynth_server->theme.form_min.h - xynth_server->client[id].win.h;
	}

        /* window form */
        /* top */
	xynth_server->client[id].form[TOP_L].x = xynth_server->client[id].win.x;
	xynth_server->client[id].form[TOP_L].y = xynth_server->client[id].win.y;
	xynth_server->client[id].form[TOP_L].w = xynth_server->theme.form[v][TOP_L].w;
	xynth_server->client[id].form[TOP_L].h = xynth_server->theme.form[v][TOP_L].h;

	xynth_server->client[id].form[TOP_R].x = xynth_server->client[id].win.x + xynth_server->client[id].win.w - xynth_server->theme.form[v][TOP_R].w;
	xynth_server->client[id].form[TOP_R].y = xynth_server->client[id].win.y;
	xynth_server->client[id].form[TOP_R].w = xynth_server->theme.form[v][TOP_R].w;
	xynth_server->client[id].form[TOP_R].h = xynth_server->theme.form[v][TOP_R].h;

        xynth_server->client[id].form[TOP_1].x = xynth_server->client[id].win.x + xynth_server->theme.form[v][TOP_L].w;
        xynth_server->client[id].form[TOP_1].y = xynth_server->client[id].win.y;
        xynth_server->client[id].form[TOP_1].w = xynth_server->theme.button[v][MENU].w;
        xynth_server->client[id].form[TOP_1].h = xynth_server->theme.form[v][TOP_1].h;

        xynth_server->client[id].form[TOP_2].x = xynth_server->client[id].form[TOP_1].x + xynth_server->client[id].form[TOP_1].w;
        xynth_server->client[id].form[TOP_2].y = xynth_server->client[id].win.y;
        xynth_server->client[id].form[TOP_2].w = xynth_server->theme.form[v][TOP_2].w;
        xynth_server->client[id].form[TOP_2].h = xynth_server->theme.form[v][TOP_2].h;

        xynth_server->client[id].form[TOP_3].x = xynth_server->client[id].form[TOP_2].x + xynth_server->client[id].form[TOP_2].w;
        xynth_server->client[id].form[TOP_3].y = xynth_server->client[id].win.y;
        i = xynth_server->client[id].win.w - xynth_server->client[id].form[TOP_L].w - xynth_server->client[id].form[TOP_R].w -
            xynth_server->client[id].form[TOP_1].w - xynth_server->client[id].form[TOP_2].w - xynth_server->theme.form[v][TOP_4].w - btns_w;
        if (xynth_server->theme.title_full) {
		xynth_server->client[id].form[TOP_3].w = i;
	} else {
		xynth_server->client[id].form[TOP_3].w = (i < title_len) ? i : title_len;
	}
        xynth_server->client[id].form[TOP_3].h = xynth_server->theme.form[v][TOP_3].h;

        if (xynth_server->theme.title_full) {
	        xynth_server->client[id].form[TOP_4].x = xynth_server->client[id].win.x + xynth_server->client[id].win.w - xynth_server->theme.form[v][TOP_4].w -
	        			           btns_w - xynth_server->client[id].form[TOP_R].w;
	} else {
	        xynth_server->client[id].form[TOP_4].x = xynth_server->client[id].form[TOP_3].x + xynth_server->client[id].form[TOP_3].w;
	}
        xynth_server->client[id].form[TOP_4].y = xynth_server->client[id].win.y;
        xynth_server->client[id].form[TOP_4].w = xynth_server->theme.form[v][TOP_4].w;
        xynth_server->client[id].form[TOP_4].h = xynth_server->theme.form[v][TOP_4].h;

	if (xynth_server->client[id].win.w < xynth_server->theme.form_min.w) {
		xynth_server->client[id].form[TOP_5].x = xynth_server->client[id].form[TOP_2].x;
	} else {
		xynth_server->client[id].form[TOP_5].x = xynth_server->client[id].form[TOP_4].x + xynth_server->client[id].form[TOP_4].w;
	}
        xynth_server->client[id].form[TOP_5].y = xynth_server->client[id].win.y;
        xynth_server->client[id].form[TOP_5].w = xynth_server->client[id].form[TOP_R].x - xynth_server->client[id].form[TOP_5].x;
        xynth_server->client[id].form[TOP_5].h = xynth_server->theme.form[v][TOP_5].h;

	/* left */
	xynth_server->client[id].form[LEFT].x = xynth_server->client[id].win.x;
	xynth_server->client[id].form[LEFT].y = xynth_server->client[id].win.y + xynth_server->theme.form[v][TOP_L].h;
	xynth_server->client[id].form[LEFT].w = xynth_server->theme.form[v][LEFT].w;
	xynth_server->client[id].form[LEFT].h = xynth_server->client[id].win.h - xynth_server->theme.form[v][TOP_L].h - xynth_server->theme.form[v][BTM_L].h;

	/* right */
	xynth_server->client[id].form[RIGHT].x = xynth_server->client[id].win.x + xynth_server->client[id].win.w - xynth_server->theme.form[v][RIGHT].w;
	xynth_server->client[id].form[RIGHT].y = xynth_server->client[id].win.y + xynth_server->theme.form[v][TOP_R].h;
	xynth_server->client[id].form[RIGHT].w = xynth_server->theme.form[v][RIGHT].w;
	xynth_server->client[id].form[RIGHT].h = xynth_server->client[id].win.h - xynth_server->theme.form[v][TOP_R].h - xynth_server->theme.form[v][BTM_R].h;

	/* bottom */
	xynth_server->client[id].form[BTM_L].x = xynth_server->client[id].win.x;
	xynth_server->client[id].form[BTM_L].y = xynth_server->client[id].win.y + xynth_server->client[id].win.h - xynth_server->theme.form[v][BTM_L].h;
	xynth_server->client[id].form[BTM_L].w = xynth_server->theme.form[v][BTM_L].w;
	xynth_server->client[id].form[BTM_L].h = xynth_server->theme.form[v][BTM_L].h;

        xynth_server->client[id].form[BTM].x = xynth_server->client[id].win.x + xynth_server->client[id].form[BTM_L].w;
        xynth_server->client[id].form[BTM].y = xynth_server->client[id].win.y + xynth_server->client[id].win.h - xynth_server->theme.form[v][BTM].h;
        xynth_server->client[id].form[BTM].w = xynth_server->client[id].win.w - xynth_server->client[id].form[BTM_L].w - xynth_server->theme.form[v][BTM_R].w;
        xynth_server->client[id].form[BTM].h = xynth_server->theme.form[v][BTM].h;

	xynth_server->client[id].form[BTM_R].x = xynth_server->client[id].win.x + xynth_server->client[id].win.w - xynth_server->theme.form[v][BTM_R].w;
	xynth_server->client[id].form[BTM_R].y = xynth_server->client[id].win.y + xynth_server->client[id].win.h - xynth_server->theme.form[v][BTM_R].h;
	xynth_server->client[id].form[BTM_R].w = xynth_server->theme.form[v][BTM_R].w;
	xynth_server->client[id].form[BTM_R].h = xynth_server->theme.form[v][BTM_R].h;

	/* buttons */
	/* menu */
	xynth_server->client[id].button[MENU].x = xynth_server->client[id].form[TOP_1].x;
	xynth_server->client[id].button[MENU].y = xynth_server->client[id].form[TOP_1].y;
	xynth_server->client[id].button[MENU].w = xynth_server->theme.button[v][MENU].w;
	xynth_server->client[id].button[MENU].h = xynth_server->theme.button[v][MENU].h;

	/* close */
	xynth_server->client[id].button[CLOSE].x = xynth_server->client[id].form[TOP_R].x - xynth_server->theme.button[v][CLOSE].w;
	xynth_server->client[id].button[CLOSE].y = xynth_server->client[id].form[TOP_5].y;
	xynth_server->client[id].button[CLOSE].w = xynth_server->theme.button[v][CLOSE].w;
	xynth_server->client[id].button[CLOSE].h = xynth_server->theme.button[v][CLOSE].h;

	/* maximize */
	xynth_server->client[id].button[MAXIMIZE].x = xynth_server->client[id].button[CLOSE].x - xynth_server->theme.button[v][MAXIMIZE].w;
	xynth_server->client[id].button[MAXIMIZE].y = xynth_server->client[id].form[TOP_5].y;
	xynth_server->client[id].button[MAXIMIZE].w = xynth_server->theme.button[v][MAXIMIZE].w;
	xynth_server->client[id].button[MAXIMIZE].h = xynth_server->theme.button[v][MAXIMIZE].h;

	/* hide */
	if (xynth_server->client[id].resizeable == 1) {
		xynth_server->client[id].button[HIDE].x = xynth_server->client[id].button[MAXIMIZE].x - xynth_server->theme.button[v][HIDE].w;
	} else {
		xynth_server->client[id].button[HIDE].x = xynth_server->client[id].button[CLOSE].x - xynth_server->theme.button[v][HIDE].w;
	}
	xynth_server->client[id].button[HIDE].y = xynth_server->client[id].form[TOP_5].y;
	xynth_server->client[id].button[HIDE].w = xynth_server->theme.button[v][HIDE].w;
	xynth_server->client[id].button[HIDE].h = xynth_server->theme.button[v][HIDE].h;
}

int s_server_window_is_parent_temp (int pid, int cid)
{
	int i = cid;

	while (i >= 0) {
		if (xynth_server->client[i].type & (WINDOW_TYPE_MAIN | WINDOW_TYPE_CHILD)) {
			return 0;
		}
		i = xynth_server->client[i].pid;
		if (i < 0) {
			return 0;
		}
		if (i == pid) {
			return 1;
		}
	}

	return 0;
}

int s_server_window_temp_parent (int cid)
{
	int i = cid;

	while (i >= 0) {
		if (xynth_server->client[i].type & (WINDOW_TYPE_MAIN | WINDOW_TYPE_CHILD)) {
			return i;
		}
		i = xynth_server->client[i].pid;
		if (i < 0) {
			return -1;
		}
	}

	return -1;
}

void s_server_window_close_temps (int id)
{
	int i;

	if (id < 0) {
		return;
	}
	for (i = 0; i < S_CLIENTS_MAX; i++) {
		if ((xynth_server->client[i].pid == id) && (xynth_server->client[i].type & WINDOW_TYPE_TEMP)) {
			s_server_window_close_id(i);
		}
	}
}

void s_server_window_close_id (int id)
{
	if (id < 0) {
		return;
	}
	s_server_socket_request(SOC_DATA_CLOSE, id);
}

void s_server_window_hide_id (int id)
{
        int i;
	int p;
        s_rect_t c;

        if (id < 0) {
		return;
	}
	p = s_server_id_pri(id);
	if (p < 0) {
		return;
	}

	if (xynth_server->client[id].win.w < 0) {
		xynth_server->client[id].win.w = 0;
	}
        if (xynth_server->client[id].win.h < 0) {
		xynth_server->client[id].win.h = 0;
	}
	c = xynth_server->client[id].win;

	if (p >= 0) {
		for (i = p; i < (S_CLIENTS_MAX - 1); i++) {
			xynth_server->pri[i] = xynth_server->pri[i + 1];
		}
		xynth_server->pri[S_CLIENTS_MAX - 1] = -1;
	}

	s_server_pri_set(SURFACE_CLOSE, &c);

	{
		s_rect_t coor = {0, 0, 0, 0};
		s_server_socket_request(SOC_DATA_EXPOSE, id, &coor);
	}
}

void s_server_window_close (s_window_t *window)
{
	int id = s_server_pri_id(0);
	if (id < 0) {
		return;
	}
	s_server_window_close_id(id);
}

void s_server_window_move_resize (int id, s_rect_t *new)
{
        int mw;
	int mh;
	s_rect_t old = xynth_server->client[id].win;
        int w = xynth_server->client[id].win.w - xynth_server->client[id].buf.w;
        int h = xynth_server->client[id].win.h - xynth_server->client[id].buf.h;

        if (xynth_server->client[id].type & WINDOW_TYPE_NOFORM) {
		mw = 0;
		mh = 0;
	} else {
		mw = xynth_server->theme.form_min.w_ - w;
		mh = xynth_server->theme.form_min.h - h;
	}

	if ((new->x < 0) && (new->y < 0) && (s_server_id_pri(id) < 0)) {
		new->x += w;
		new->y += h;
	}

	if (new->w < (mw + w)) {
		new->w = mw + w;
	}
	if (new->h < (mh + h)) {
		new->h = mh + h;
	}
	if ((new->x + new->w) <= 5) {
		new->x = 5 - new->w;
	}
	if ((new->y + new->h) <= 5) {
		new->y = 5 - new->h;
	}
	if (new->x >= (xynth_server->window->surface->width - 5)) {
		new->x = xynth_server->window->surface->width - 5;
	}
	if (new->y >= (xynth_server->window->surface->height - 5)) {
		new->y = xynth_server->window->surface->height - 5;
	}

	xynth_server->client[id].buf.x += new->x - old.x;
	xynth_server->client[id].buf.y += new->y - old.y;
	xynth_server->client[id].buf.w += new->w - old.w;
	xynth_server->client[id].buf.h += new->h - old.h;
	xynth_server->client[id].win = *new;

	s_server_pri_set(SURFACE_CHANGED, id, &old, new);
}

void s_server_window_maximize (s_window_t *window)
{
	int id;
        s_rect_t new;
	new.x = 0;
	new.y = 0;
	new.w = window->surface->width;
	new.h = window->surface->height;
	id = s_server_pri_id(0);
	if (id < 0) {
		return;
	}
	s_server_window_move_resize(id, &new);
}
