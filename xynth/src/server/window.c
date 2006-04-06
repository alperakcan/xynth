/***************************************************************************
    begin                : Sun Feb 23 2003
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

#include "../lib/xynth_.h"
#include "server.h"

void s_server_window_new (int id)
{
	int v = (s_server_id_pri(id) <= 0) ? 1 : 0;

	server->client[id].win.x = server->client[id].buf.x;
	server->client[id].win.y = server->client[id].buf.y;

	if (server->client[id].type & WINDOW_NOFORM) {
		server->client[id].win.w = server->client[id].buf.w;
		server->client[id].win.h = server->client[id].buf.h;
	} else {
		server->client[id].win.w = server->client[id].buf.w + (server->theme.form[v][LEFT].w + server->theme.form[v][RIGHT].w);
		server->client[id].win.h = server->client[id].buf.h + (server->theme.form[v][TOP_1].h + server->theme.form[v][BTM].h);
	}

        /* if the caller does not know what he/she is doing we will correct his/her shit */
	server->client[id].win.x = (server->client[id].win.x < 0) ? 0 : server->client[id].win.x;
	server->client[id].win.y = (server->client[id].win.y < 0) ? 0 : server->client[id].win.y;

        if (server->client[id].type & WINDOW_NOFORM) {
		if (server->client[id].win.w < 0) {
			server->client[id].win.w = 0;
		}
		if (server->client[id].win.h < 0) {
			server->client[id].win.h = 0;
		}
	} else {
		if (server->client[id].win.w < server->theme.form_min.w_) {
			server->client[id].win.w = server->theme.form_min.w_;
		}
		if (server->client[id].win.h < server->theme.form_min.h) {
			server->client[id].win.h = server->theme.form_min.h;
		}
	}

        if (server->client[id].type & WINDOW_NOFORM) {
		server->client[id].win.x = (server->client[id].win.x > server->window->surface->width) ? 0 : server->client[id].win.x;
		server->client[id].win.y = (server->client[id].win.y > server->window->surface->height) ? 0 : server->client[id].win.y;
	} else {
		server->client[id].win.x = (server->client[id].win.x > server->window->surface->width - server->theme.form_min.w) ? 0 : server->client[id].win.x;
		server->client[id].win.y = (server->client[id].win.y > server->window->surface->height - server->theme.form_min.h) ? 0 : server->client[id].win.y;
	}

	/* coor&dim correcting, done. */

        if (server->client[id].type & WINDOW_NOFORM) {
		server->client[id].buf.x = server->client[id].win.x;
		server->client[id].buf.y = server->client[id].win.y;
		server->client[id].buf.w = server->client[id].win.w;
		server->client[id].buf.h = server->client[id].win.h;
	} else {
		server->client[id].buf.x = server->client[id].win.x + server->theme.form[v][LEFT].w;
		server->client[id].buf.y = server->client[id].win.y + server->theme.form[v][TOP_1].h;
		server->client[id].buf.w = server->client[id].win.w - server->theme.form[v][LEFT].w - server->theme.form[v][RIGHT].w;
		server->client[id].buf.h = server->client[id].win.h - server->theme.form[v][TOP_1].h - server->theme.form[v][BTM].h;
	}

	server->client[id].title.str = (char *) s_malloc(sizeof(char) * 23);
	sprintf(server->client[id].title.str, "Xynth Windowing System");
	s_server_window_title(id, server->client[id].title.str);
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
	    (server->client[id].type & WINDOW_NOFORM)) {
		return;
	}
	
	srf = (s_surface_t *) s_malloc(sizeof(s_surface_t));

        for (v = 0; v < 2; v++) {
		font = server->theme.font[v];
		s_font_set_str(font, title);
		s_font_get_glyph(font);
		s_image_get_mat(font->img);
		s_image_get_handler(font->img);

		font->img->buf = (char *) s_calloc(1, font->img->w * font->img->h * server->window->surface->bytesperpixel);
                s_getsurfacevirtual(srf, font->img->w, font->img->h, server->window->surface->bitsperpixel, font->img->buf);

		if ((i = font->img->w / server->theme.form[v][TOP_3].w) > 0) {
			while (i--) {
				yo = server->theme.text_v_off[v] - font->img->handler.y + (server->theme.form[v][TOP_3].handler.h - font->img->handler.h) / 2;
				if ((font->img->h + yo) > server->theme.form[v][TOP_3].h) {
					h = server->theme.form[v][TOP_3].h - yo;
				} else {
					h = font->img->h;
				}
				s_putboxpart(srf, i * server->theme.form[v][TOP_3].w, 0, server->theme.form[v][TOP_3].w, h, server->theme.form[v][TOP_3].w, server->theme.form[v][TOP_3].h, server->theme.form[v][TOP_3].buf, 0, yo);
			}
		}
		if ((i = font->img->w % server->theme.form[v][TOP_3].w) > 0) {
				s_putboxpart(srf, font->img->w - server->theme.form[v][TOP_3].w, 0, server->theme.form[v][TOP_3].w, font->img->h, server->theme.form[v][TOP_3].w, server->theme.form[v][TOP_3].h, server->theme.form[v][TOP_3].buf, 0, server->theme.text_v_off[v] - font->img->handler.y + (server->theme.form[v][TOP_3].handler.h - font->img->handler.h) / 2);
		}
		s_putboxrgba(srf, 0, 0, font->img->w, font->img->h, font->img->rgba);

		s_free(server->client[id].title.img[v].mat);
		s_free(server->client[id].title.img[v].buf);
		server->client[id].title.img[v].mat = (unsigned char *) s_malloc(font->img->w * font->img->h);
		server->client[id].title.img[v].buf = (char *) s_malloc(font->img->w * font->img->h * server->window->surface->bytesperpixel);
		server->client[id].title.img[v].w = font->img->w;
		server->client[id].title.img[v].h = font->img->h;
		server->client[id].title.hy[v] = font->img->handler.y;
		server->client[id].title.hh[v] = font->img->handler.h;
		memcpy(server->client[id].title.img[v].mat, font->img->mat, font->img->w * font->img->h);
		memcpy(server->client[id].title.img[v].buf, font->img->buf, font->img->w * font->img->h * server->window->surface->bytesperpixel);
		s_image_uninit(font->img);
		s_image_init(&(font->img));
	}
	s_free(srf);
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

	if (s_rect_intersect(coor, &icoor, &intr)) {
		return;
	}
	if (s_rect_clip_virtual(window->surface, intr.x, intr.y, intr.w, intr.h, &to)) {
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
	if ((id < 0) ||
	    (server->client[id].type & WINDOW_NOFORM) ||
	    (s_server_id_pri(id) < 0)) {
		return -1;
	}
	if ((s_server_id_pri(id) == 0) ||
	    ((server->client[s_server_pri_id(0)].type & WINDOW_TEMP) &&
	     (s_server_window_is_parent_temp(id, s_server_pri_id(0))))) {
		return 1;
        }
        return 0;
}

void s_server_window_form_mat (int v, int id, int mi, s_rect_t *coor, void (*func) (s_window_t *, int, s_rect_t *, int, int, s_image_t *))
{
	int i;
	/* top */
	func(server->window, mi, coor, server->client[id].form[TOP_L].x, server->client[id].form[TOP_L].y, &server->theme.form[v][TOP_L]);
	for (i = 0; i < server->client[id].form[TOP_1].w; i += server->theme.form[v][TOP_1].w) {
		func(server->window, mi, coor, server->client[id].form[TOP_1].x + i, server->client[id].form[TOP_1].y, &server->theme.form[v][TOP_1]);
	}
	if (server->client[id].win.w >= server->theme.form_min.w) {
		func(server->window, mi, coor, server->client[id].form[TOP_2].x, server->client[id].form[TOP_2].y, &server->theme.form[v][TOP_2]);
		if ((i = server->client[id].form[TOP_3].w / server->theme.form[v][TOP_3].w) > 0) {
			while (i--) {
				func(server->window, mi, coor, server->client[id].form[TOP_3].x + i * server->theme.form[v][TOP_3].w, server->client[id].form[TOP_3].y, &server->theme.form[v][TOP_3]);
			}
		}
		if (server->client[id].form[TOP_3].w % server->theme.form[v][TOP_3].w) {
			func(server->window, mi, coor, server->client[id].form[TOP_4].x - server->theme.form[v][TOP_3].w, server->client[id].form[TOP_3].y, &server->theme.form[v][TOP_3]);
		}
		func(server->window, mi, coor, server->client[id].form[TOP_4].x, server->client[id].form[TOP_4].y, &server->theme.form[v][TOP_4]);
	}
	if ((i = server->client[id].form[TOP_5].w / server->theme.form[v][TOP_5].w) > 0) {
		while (i--) {
			func(server->window, mi, coor, server->client[id].form[TOP_5].x + i * server->theme.form[v][TOP_5].w, server->client[id].form[TOP_5].y, &server->theme.form[v][TOP_5]);
		}
	}
	if (server->client[id].form[TOP_5].w % server->theme.form[v][TOP_5].w) {
		func(server->window, mi, coor, server->client[id].form[TOP_R].x - server->theme.form[v][TOP_5].w, server->client[id].form[TOP_5].y, &server->theme.form[v][TOP_5]);
	}
	func(server->window, mi, coor, server->client[id].form[TOP_R].x, server->client[id].form[TOP_R].y, &server->theme.form[v][TOP_R]);
	/* left */
	if ((i = server->client[id].form[LEFT].h / server->theme.form[v][LEFT].h) > 0) {
		while (i--) {
			func(server->window, mi, coor, server->client[id].form[LEFT].x, server->client[id].form[LEFT].y + i * server->theme.form[v][LEFT].h, &server->theme.form[v][LEFT]);
		}
	}
	if (server->client[id].form[LEFT].h % server->theme.form[v][LEFT].h) {
		func(server->window, mi, coor, server->client[id].form[LEFT].x, server->client[id].form[BTM_L].y - server->theme.form[v][LEFT].h, &server->theme.form[v][LEFT]);
	}
        /* right */
	if ((i = server->client[id].form[RIGHT].h / server->theme.form[v][RIGHT].h) > 0) {
		while (i--) {
			func(server->window, mi, coor, server->client[id].form[RIGHT].x, server->client[id].form[RIGHT].y + i * server->theme.form[v][RIGHT].h, &server->theme.form[v][RIGHT]);
		}
	}
	if (server->client[id].form[RIGHT].h % server->theme.form[v][RIGHT].h) {
		func(server->window, mi, coor, server->client[id].form[RIGHT].x, server->client[id].form[BTM_R].y - server->theme.form[v][RIGHT].h, &server->theme.form[v][RIGHT]);
	}
        /* buttom */
        func(server->window, mi, coor, server->client[id].form[BTM_L].x, server->client[id].form[BTM_L].y, &server->theme.form[v][BTM_L]);
        if ((i = server->client[id].form[BTM].w / server->theme.form[v][BTM].w) > 0) {
		while (i--) {
			func(server->window, mi, coor, server->client[id].form[BTM].x + i * server->theme.form[v][BTM].w, server->client[id].form[BTM].y, &server->theme.form[v][BTM]);
		}
	}
	if (server->client[id].form[BTM].w % server->theme.form[v][BTM].w) {
		func(server->window, mi, coor, server->client[id].form[BTM_R].x - server->theme.form[v][BTM].w, server->client[id].form[BTM].y, &server->theme.form[v][BTM]);
	}
        func(server->window, mi, coor, server->client[id].form[BTM_R].x, server->client[id].form[BTM_R].y, &server->theme.form[v][BTM_R]);
}

void s_server_window_form (int id, s_rect_t *_coor_)
{
	int v = 0;
	int mi = id;
	s_rect_t coort;
	s_rect_t intersect;

	s_rect_t coor;
	s_rect_t coor_ = server->client[id].win;

	v = s_server_window_form_mat_verbose(id);
	if (v < 0) {
		return;
	}
        
	if (s_rect_intersect(_coor_, &coor_, &coor)) {
		return;
	}

        coort.x = server->client[id].form[TOP_3].x;
        coort.y = server->client[id].form[TOP_3].y;
        coort.w = server->client[id].form[TOP_3].w;
        coort.h = server->theme.form[v][TOP_3].handler.h;

	s_server_window_form_mat(v, id, mi, &coor, s_server_putbox);
	
	/* buttons */
	s_server_putbox(server->window, id, &coor, server->client[id].button[MENU].x, server->client[id].button[MENU].y, &server->theme.button[v][MENU]);
	s_server_putbox(server->window, id, &coor, server->client[id].button[HIDE].x, server->client[id].button[HIDE].y, &server->theme.button[v][HIDE]);
	s_server_putbox(server->window, id, &coor, server->client[id].button[CLOSE].x, server->client[id].button[CLOSE].y, &server->theme.button[v][CLOSE]);
	if (server->client[id].resizeable == 1) {
		s_server_putbox(server->window, id, &coor, server->client[id].button[MAXIMIZE].x, server->client[id].button[MAXIMIZE].y, &server->theme.button[v][MAXIMIZE]);
	}

        /* title */
       	if ((!s_rect_intersect(&coor, &coort, &intersect)) && (server->client[id].win.w > server->theme.form_min.w)) {
		s_server_putbox(server->window, id, &intersect, coort.x, coort.y + server->theme.text_v_off[v] - server->client[id].title.hy[v] + (server->theme.form[v][TOP_3].handler.h - server->client[id].title.hh[v]) / 2, &server->client[id].title.img[v]);
	}
}

void s_server_window_matrix_add (int id, s_rect_t *_coor_)
{
	s_server_window_matrix(id, id, _coor_);
}

void s_server_window_matrix_del (int id, s_rect_t *_coor_)
{
	s_server_window_matrix(id, S_MATRIX_DELETED, _coor_);
}

void s_server_window_matrix (int id, int mi, s_rect_t *_coor_)
{
	int v = 0;
	s_rect_t coor;
	s_rect_t coor_ = server->client[id].win;

	v = s_server_window_form_mat_verbose(id);
	if (v < 0) {
		return;
	}

	if (s_rect_intersect(_coor_, &coor_, &coor)) {
		return;
	}

	s_server_window_form_mat(v, id, mi, &coor, s_server_putmat);
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

        title_len = server->client[id].title.img[v].w;
        btns_w = server->theme.button[v][HIDE].w + server->theme.button[v][CLOSE].w;
        if (server->client[id].resizeable == 1) {
		btns_w += server->theme.button[v][MAXIMIZE].w;
	}

	server->client[id].win.x = server->client[id].buf.x - server->theme.form[v][LEFT].w;
	server->client[id].win.y = server->client[id].buf.y - server->theme.form[v][TOP_1].h;
	if ((server->client[id].win.w = server->client[id].buf.w + server->theme.form[v][LEFT].w + server->theme.form[v][RIGHT].w) < server->theme.form_min.w_) {
		server->client[id].buf.w += server->theme.form_min.w_ - server->client[id].win.w;
		server->client[id].win.w += server->theme.form_min.w_ - server->client[id].win.w;
	}
	if ((server->client[id].win.h = server->client[id].buf.h + server->theme.form[v][TOP_1].h + server->theme.form[v][BTM].h) < server->theme.form_min.h) {
		server->client[id].buf.h += server->theme.form_min.h - server->client[id].win.h;
		server->client[id].win.h += server->theme.form_min.h - server->client[id].win.h;
	}

        /* window form */
        /* top */
	server->client[id].form[TOP_L].x = server->client[id].win.x;
	server->client[id].form[TOP_L].y = server->client[id].win.y;
	server->client[id].form[TOP_L].w = server->theme.form[v][TOP_L].w;
	server->client[id].form[TOP_L].h = server->theme.form[v][TOP_L].h;

	server->client[id].form[TOP_R].x = server->client[id].win.x + server->client[id].win.w - server->theme.form[v][TOP_R].w;
	server->client[id].form[TOP_R].y = server->client[id].win.y;
	server->client[id].form[TOP_R].w = server->theme.form[v][TOP_R].w;
	server->client[id].form[TOP_R].h = server->theme.form[v][TOP_R].h;

        server->client[id].form[TOP_1].x = server->client[id].win.x + server->theme.form[v][TOP_L].w;
        server->client[id].form[TOP_1].y = server->client[id].win.y;
        server->client[id].form[TOP_1].w = server->theme.button[v][MENU].w;
        server->client[id].form[TOP_1].h = server->theme.form[v][TOP_1].h;

        server->client[id].form[TOP_2].x = server->client[id].form[TOP_1].x + server->client[id].form[TOP_1].w;
        server->client[id].form[TOP_2].y = server->client[id].win.y;
        server->client[id].form[TOP_2].w = server->theme.form[v][TOP_2].w;
        server->client[id].form[TOP_2].h = server->theme.form[v][TOP_2].h;

        server->client[id].form[TOP_3].x = server->client[id].form[TOP_2].x + server->client[id].form[TOP_2].w;
        server->client[id].form[TOP_3].y = server->client[id].win.y;
        i = server->client[id].win.w - server->client[id].form[TOP_L].w - server->client[id].form[TOP_R].w -
            server->client[id].form[TOP_1].w - server->client[id].form[TOP_2].w - server->theme.form[v][TOP_4].w - btns_w;
        if (server->theme.title_full) {
		server->client[id].form[TOP_3].w = i;
	} else {
		server->client[id].form[TOP_3].w = (i < title_len) ? i : title_len;
	}
        server->client[id].form[TOP_3].h = server->theme.form[v][TOP_3].h;

        if (server->theme.title_full) {
	        server->client[id].form[TOP_4].x = server->client[id].win.x + server->client[id].win.w - server->theme.form[v][TOP_4].w -
	        			           btns_w - server->client[id].form[TOP_R].w;
	} else {
	        server->client[id].form[TOP_4].x = server->client[id].form[TOP_3].x + server->client[id].form[TOP_3].w;
	}
        server->client[id].form[TOP_4].y = server->client[id].win.y;
        server->client[id].form[TOP_4].w = server->theme.form[v][TOP_4].w;
        server->client[id].form[TOP_4].h = server->theme.form[v][TOP_4].h;

	if (server->client[id].win.w < server->theme.form_min.w) {
		server->client[id].form[TOP_5].x = server->client[id].form[TOP_2].x;
	} else {
		server->client[id].form[TOP_5].x = server->client[id].form[TOP_4].x + server->client[id].form[TOP_4].w;
	}
        server->client[id].form[TOP_5].y = server->client[id].win.y;
        server->client[id].form[TOP_5].w = server->client[id].form[TOP_R].x - server->client[id].form[TOP_5].x;
        server->client[id].form[TOP_5].h = server->theme.form[v][TOP_5].h;

	/* left */
	server->client[id].form[LEFT].x = server->client[id].win.x;
	server->client[id].form[LEFT].y = server->client[id].win.y + server->theme.form[v][TOP_L].h;
	server->client[id].form[LEFT].w = server->theme.form[v][LEFT].w;
	server->client[id].form[LEFT].h = server->client[id].win.h - server->theme.form[v][TOP_L].h - server->theme.form[v][BTM_L].h;

	/* right */
	server->client[id].form[RIGHT].x = server->client[id].win.x + server->client[id].win.w - server->theme.form[v][RIGHT].w;
	server->client[id].form[RIGHT].y = server->client[id].win.y + server->theme.form[v][TOP_R].h;
	server->client[id].form[RIGHT].w = server->theme.form[v][RIGHT].w;
	server->client[id].form[RIGHT].h = server->client[id].win.h - server->theme.form[v][TOP_R].h - server->theme.form[v][BTM_R].h;

	/* bottom */
	server->client[id].form[BTM_L].x = server->client[id].win.x;
	server->client[id].form[BTM_L].y = server->client[id].win.y + server->client[id].win.h - server->theme.form[v][BTM_L].h;
	server->client[id].form[BTM_L].w = server->theme.form[v][BTM_L].w;
	server->client[id].form[BTM_L].h = server->theme.form[v][BTM_L].h;

        server->client[id].form[BTM].x = server->client[id].win.x + server->client[id].form[BTM_L].w;
        server->client[id].form[BTM].y = server->client[id].win.y + server->client[id].win.h - server->theme.form[v][BTM].h;
        server->client[id].form[BTM].w = server->client[id].win.w - server->client[id].form[BTM_L].w - server->theme.form[v][BTM_R].w;
        server->client[id].form[BTM].h = server->theme.form[v][BTM].h;

	server->client[id].form[BTM_R].x = server->client[id].win.x + server->client[id].win.w - server->theme.form[v][BTM_R].w;
	server->client[id].form[BTM_R].y = server->client[id].win.y + server->client[id].win.h - server->theme.form[v][BTM_R].h;
	server->client[id].form[BTM_R].w = server->theme.form[v][BTM_R].w;
	server->client[id].form[BTM_R].h = server->theme.form[v][BTM_R].h;

	/* buttons */
	/* menu */
	server->client[id].button[MENU].x = server->client[id].form[TOP_1].x;
	server->client[id].button[MENU].y = server->client[id].form[TOP_1].y;
	server->client[id].button[MENU].w = server->theme.button[v][MENU].w;
	server->client[id].button[MENU].h = server->theme.button[v][MENU].h;

	/* close */
	server->client[id].button[CLOSE].x = server->client[id].form[TOP_R].x - server->theme.button[v][CLOSE].w;
	server->client[id].button[CLOSE].y = server->client[id].form[TOP_5].y;
	server->client[id].button[CLOSE].w = server->theme.button[v][CLOSE].w;
	server->client[id].button[CLOSE].h = server->theme.button[v][CLOSE].h;

	/* maximize */
	server->client[id].button[MAXIMIZE].x = server->client[id].button[CLOSE].x - server->theme.button[v][MAXIMIZE].w;
	server->client[id].button[MAXIMIZE].y = server->client[id].form[TOP_5].y;
	server->client[id].button[MAXIMIZE].w = server->theme.button[v][MAXIMIZE].w;
	server->client[id].button[MAXIMIZE].h = server->theme.button[v][MAXIMIZE].h;

	/* hide */
	if (server->client[id].resizeable == 1) {
		server->client[id].button[HIDE].x = server->client[id].button[MAXIMIZE].x - server->theme.button[v][HIDE].w;
	} else {
		server->client[id].button[HIDE].x = server->client[id].button[CLOSE].x - server->theme.button[v][HIDE].w;
	}
	server->client[id].button[HIDE].y = server->client[id].form[TOP_5].y;
	server->client[id].button[HIDE].w = server->theme.button[v][HIDE].w;
	server->client[id].button[HIDE].h = server->theme.button[v][HIDE].h;
}

int s_server_window_is_parent_temp (int pid, int cid)
{
	int i = cid;

	while (i >= 0) {
		if (server->client[i].type & (WINDOW_MAIN | WINDOW_CHILD)) {
			return 0;
		}
		i = server->client[i].pid;
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
		if (server->client[i].type & (WINDOW_MAIN | WINDOW_CHILD)) {
			return i;
		}
		i = server->client[i].pid;
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
		if ((server->client[i].pid == id) && (server->client[i].type & WINDOW_TEMP)) {
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

	if (server->client[id].win.w < 0) {
		server->client[id].win.w = 0;
	}
        if (server->client[id].win.h < 0) {
		server->client[id].win.h = 0;
	}
	c = server->client[id].win;

	if (p >= 0) {
		for (i = p; i < (S_CLIENTS_MAX - 1); i++) {
			server->pri[i] = server->pri[i + 1];
		}
		server->pri[S_CLIENTS_MAX - 1] = -1;
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
	s_rect_t old = server->client[id].win;
        int w = server->client[id].win.w - server->client[id].buf.w;
        int h = server->client[id].win.h - server->client[id].buf.h;

        if (server->client[id].type & WINDOW_NOFORM) {
		mw = 0;
		mh = 0;
	} else {
		mw = server->theme.form_min.w_ - w;
		mh = server->theme.form_min.h - h;
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
	if (new->x >= (server->window->surface->width - 5)) {
		new->x = server->window->surface->width - 5;
	}
	if (new->y >= (server->window->surface->height - 5)) {
		new->y = server->window->surface->height - 5;
	}

	server->client[id].buf.x += new->x - old.x;
	server->client[id].buf.y += new->y - old.y;
	server->client[id].buf.w += new->w - old.w;
	server->client[id].buf.h += new->h - old.h;
	server->client[id].win = *new;

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
