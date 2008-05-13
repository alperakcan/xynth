/***************************************************************************
    begin                : Fri Feb 21 2003
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

void s_server_pri_set (S_SURFACE_CHNGF flag, ...)
{
        int i;
	va_list ap;
        int id = 0;
  	s_rect_t *c0 = NULL;
	s_rect_t *c1 = NULL;
	
	va_start(ap, flag);

	switch (flag) {
		case SURFACE_FOCUS:
			id = (int) va_arg(ap, int);
			break;
		case SURFACE_CLOSE:
		case SURFACE_REDRAW:
		case SURFACE_REFRESH:
			/* c0 will be updated. */
			c0 = (s_rect_t *) va_arg(ap, s_rect_t *);
			break;
		case SURFACE_CHANGED:
			/* c0 : old window rectangle
			   c1 : new window rectangle
			 */
			id = (int) va_arg(ap, int);
			c0 = (s_rect_t *) va_arg(ap, s_rect_t *);
			c1 = (s_rect_t *) va_arg(ap, s_rect_t *);
			break;
	}
	va_end(ap);

	if (id < 0) {
		debugf(DSER | DFAT, "What an id [%d] ?!!", id);
	}

	if (flag == SURFACE_CHANGED) {
		if (s_server_id_pri(id) < 0) {
			return;
		}
	}

	i = s_server_pri_id(0);
	if ((i >= 0) &&
	    (server->client[i].type & WINDOW_TYPE_TEMP)) {
		int ptmp = s_server_window_temp_parent(i);
		if (flag == SURFACE_FOCUS) {
			if (s_server_window_is_parent_temp(id, i)) {
				s_server_window_close_temps(id);
			} else {
				if ((server->client[id].type & WINDOW_TYPE_TEMP) &&
				    (server->client[id].pid == i)) {
				} else {
					if (ptmp >= 0) {
						s_server_pri_set(SURFACE_FOCUS, ptmp);
					}
				}
			}
		}
	}

	s_server_pri_set_(flag, id, c0, c1);

        switch (flag) {
		case SURFACE_FOCUS:
		case SURFACE_CLOSE:
		case SURFACE_CHANGED:
		case SURFACE_REFRESH:
			if (server->mh) {
				server->mh = 0;
			}
			i = s_server_pri_id(0);
			if (i >= 0) {
				s_server_mouse_setcursor(server->client[i].cursor);
			} else {
				s_server_mouse_setcursor(CURSOR_TYPE_ARROW);
			}
			break;
		case SURFACE_REDRAW:
			break;
	}

        switch (flag) {
		case SURFACE_FOCUS:
		case SURFACE_CLOSE:
			if (!(server->client[id].type & WINDOW_TYPE_TEMP)) {
				for (i = 0; i < S_CLIENTS_MAX; i++) {
					if (server->client[i].type & WINDOW_TYPE_DESKTOP) {
						if (!((flag == SURFACE_CLOSE) &&
						      (id == i))) {
							s_server_socket_request(SOC_DATA_DESKTOP, i);
						}
					}
				}
			}
			break;
		case SURFACE_CHANGED:
		case SURFACE_REFRESH:
		case SURFACE_REDRAW:
			break;
	}
}

/*
 * This function MUST NOT call itself recursively, or any other function that calls
 * this function. But you can call it recursively. If you need something like that,
 * look above. s_server_pri_set() calls s_server_pri_set_() recursively via
 * s_server_window_close_temps*() functions.
 * This is not, and will never be a bug.
 */
void s_server_pri_set_ (S_SURFACE_CHNGF flag, int id, s_rect_t *c0, s_rect_t *c1)
{
	int i;
	int p;
	int p_old;
	int ontop;
	
	int n;
	s_rect_t *r;
	s_region_t *region;
	
	int ri;
	int rp;
	int rj;
	int rk;
	int rn;
	s_rect_t *rr;

	/* static declerations, will be fixed. these regions should be a part
	   of client class.
	 */
	static s_region_t reg;
	static s_region_t regsb[S_CLIENTS_MAX];
	static s_region_t regsw[S_CLIENTS_MAX];

	s_region_create(&region);

	p = s_server_id_pri(id);
	p_old = s_server_pri_id(0);

	/* closed one is already deleted from the priority list.
	   add opened one to the priority list (id)
	   change priority of window (id)
	 */
        if (flag == SURFACE_FOCUS) {
		if (p != -1) {
			i = p;
		} else {
			i = (S_CLIENTS_MAX - 1);
		}
		for (; i > 0; i--) {
			server->pri[i] = server->pri[i - 1];
		}
		server->pri[0] = id;
	}

	switch (flag) {
		case SURFACE_FOCUS:
			if ((i = s_server_pri_id(1)) >= 0) {
				s_server_window_calculate(i);
			}
			/* no break */
		case SURFACE_CLOSE:
			if ((i = s_server_pri_id(0)) >= 0) {
				s_server_window_calculate(i);
			}
			break;
		case SURFACE_CHANGED:
			s_server_window_calculate(id);
			break;
		case SURFACE_REFRESH:
			for (i = 0; i < S_CLIENTS_MAX; i++) {
				if (server->id[i] >= 0) {
					s_server_window_title(i, server->client[i].title.str);
					s_server_window_calculate(i);
				}
			}
			break;
		case SURFACE_REDRAW:
			break;
	}

	switch (flag) {
		case SURFACE_CLOSE:
			/* CLOSED;
			   closed window`s rectangle
			   window form area of the one`s that will get the priority (pri == 0)
			 */
			if ((i = s_server_pri_id(0)) >= 0) {
				s_region_rect_substract(&server->client[i].win, &server->client[i].buf, region);
			}
			s_region_subrect(region, c0);
			s_region_addrect(region, c0);
			break;
		case SURFACE_FOCUS:
			/* FOCUSED;
			   window form area of the one`s that lost priority (pri == 1)
			   priority gained window`s rectangle (pri == 0)
			 */
			if ((i = s_server_pri_id(1)) >= 0) {
				s_region_rect_substract(&server->client[i].win, &server->client[i].buf, region);
			}
			s_region_subrect(region, &server->client[id].win);
			s_region_addrect(region, &server->client[id].win);
			break;
		case SURFACE_CHANGED:
			/* CHANGED;
			   changed ones old rectangle (win, c0)
			   changed ones new rectangle (win, c1)
			 */
			s_region_rect_substract(c0, c1, region);
			s_region_addrect(region, c1);
			break;
		case SURFACE_REDRAW:
		case SURFACE_REFRESH:
			s_region_addrect(region, c0);
			break;
		default:
			debugf(DSER | DFAT, "Unknown surface changed flag");
	}
	
#if 0
	if ((flag == SURFACE_FOCUS) ||
	    (flag == SURFACE_REFRESH)||
	    (flag == SURFACE_REDRAW) ||
	    (flag == SURFACE_CLOSE) ||
	    (flag == SURFACE_CHANGED)) {
//	if (flag == SURFACE_REDRAW) {
		if (flag == SURFACE_CHANGED) {
			debugf(DSER, "SURFACE_CHANGED %d %d %d %d -> %d %d %d %d", c0->x, c0->y, c0->w, c0->h,
										   c1->x, c1->y, c1->w, c1->h);
		} else if (flag == SURFACE_FOCUS) {
			debugf(DSER, "SURFACE_FOCUS %d %d %d %d", server->client[id].win.x, server->client[id].win.y,
			                                          server->client[id].win.w, server->client[id].win.h);
		} else {
			debugf(DSER, "SURFACE_%s %d %d %d %d", (flag == SURFACE_FOCUS) ? "FOCUS" :
			                                       (flag == SURFACE_CLOSE) ? "CLOSED" :
			                                       (flag == SURFACE_CHANGED) ? "CHANGED" :
							       (flag == SURFACE_REDRAW) ? "REDRAW" :
							       (flag == SURFACE_REFRESH) ? "REFRESH" :
							       "UNKNOWN",
							       c0->x, c0->y, c0->w, c0->h);
		}
		r = s_region_rectangles(region);
		n = s_region_num_rectangles(region);
		while (n--) {
			debugf(DSER, "\t%d %d %d %d", r->x, r->y, r->w, r->h);
			r++;
		}
	}
#endif
	
	s_server_window_handlers_del_mouse();
	if ((i = s_server_pri_id(0)) >= 0) {
		s_server_window_handlers_add_mouse(i);
	}

	r = s_region_rectangles(region);
	n = s_region_num_rectangles(region);
	while (n--) {
		s_server_surface_matrix_del_coor(r);
		r++;
	}

	memset(&reg, 0, sizeof(s_region_t));
	memset(regsb, 0, sizeof(s_region_t) * S_CLIENTS_MAX);
	memset(regsw, 0, sizeof(s_region_t) * S_CLIENTS_MAX);
	
	for (rp = S_CLIENTS_MAX - 1; rp >= 0; rp--) {
		ri = s_server_pri_id(rp);
		if (ri < 0) {
			continue;
		}
		s_region_clear(&regsb[ri]);
		s_region_addrect(&regsb[ri], &server->client[ri].buf);
		for (rj = rp - 1; rj >= 0; rj--) {
			rk = s_server_pri_id(rj);
			if (rk < 0) {
				continue;
			}
			if (server->client[rk].alwaysontop != server->client[ri].alwaysontop) {
				continue;
			}
			s_region_subrect(&regsb[ri], &server->client[rk].buf);
		}
		for (ontop = server->client[ri].alwaysontop + 1; ontop <= 1; ontop++) {
			for (rj = S_CLIENTS_MAX - 1; rj >= 0; rj--) {
				rk = s_server_pri_id(rj);
				if (rk < 0) {
					continue;
				}
				if (server->client[rk].alwaysontop != ontop) {
					continue;
				}
				s_region_subrect(&regsb[ri], &server->client[rk].buf);
			}
		}
	}
	
	for (rp = S_CLIENTS_MAX - 1; rp >= 0; rp--) {
		ri = s_server_pri_id(rp);
		if (ri < 0) {
			continue;
		}
		s_region_clear(&regsw[ri]);
		s_region_addrect(&regsw[ri], &server->client[ri].win);
		for (rj = rp - 1; rj >= 0; rj--) {
			rk = s_server_pri_id(rj);
			if (rk < 0) {
				continue;
			}
			if (server->client[rk].alwaysontop != server->client[ri].alwaysontop) {
				continue;
			}
			s_region_subrect(&regsw[ri], &server->client[rk].buf);
		}
		for (ontop = server->client[ri].alwaysontop + 1; ontop <= 1; ontop++) {
			for (rj = S_CLIENTS_MAX - 1; rj >= 0; rj--) {
				rk = s_server_pri_id(rj);
				if (rk < 0) {
					continue;
				}
				if (server->client[rk].alwaysontop != ontop) {
					continue;
				}
				s_region_subrect(&regsw[ri], &server->client[rk].buf);
			}
		}
	}
	
	for (rp = S_CLIENTS_MAX - 1; rp >= 0; rp--) {
		ri = s_server_pri_id(rp);
		if (ri < 0) {
			continue;
		}
		s_region_clear(&reg);
		s_region_intregion(&regsb[ri], region, &reg);
		rr = s_region_rectangles(&reg);
		rn = s_region_num_rectangles(&reg);
		while (rn--) {
			s_server_surface_matrix_add(ri, rr);
			s_server_socket_request(SOC_DATA_EXPOSE, ri, rr);
			if (ri == p_old) {
				p_old = -1;
			}
			rr++;
		}
	}

	for (ontop = -1; ontop <= 1; ontop++) {	
		for (rp = S_CLIENTS_MAX - 1; rp >= 0; rp--) {
			ri = s_server_pri_id(rp);
			if (ri < 0) {
				continue;
			}
			if (server->client[ri].alwaysontop != ontop) {
				continue;
			}
			s_region_clear(&reg);
			s_region_intregion(&regsw[ri], region, &reg);
			rr = s_region_rectangles(&reg);
			rn = s_region_num_rectangles(&reg);
			while (rn--) {
				s_server_window_matrix_add(ri, rr);
				s_server_window_form(ri, rr);
				if (ri == p_old) {
					p_old = -1;
				}
				rr++;
			}
			s_region_clear(&reg);
		}
	}
	
	s_server_cursor_matrix_add();
	s_server_cursor_draw();
	
	r = s_region_rectangles(region);
	n = s_region_num_rectangles(region);
	while (n--) {
		s_server_surface_clean(r);
		s_server_surface_update(r);
		r++;
	}

	/* tell the priority (only the focus) change if not told */
	if ((p_old >= 0) &&
	    (p_old != s_server_id_pri(id))) {
		s_rect_t rfake;
		rfake.x = 0; rfake.y = 0; rfake.w = 0; rfake.h = 0;
		s_server_socket_request(SOC_DATA_EXPOSE, p_old, &rfake);
	}
	
	s_region_clear(&reg);
	for (ri = 0; ri < S_CLIENTS_MAX; ri++) {
		s_region_clear(&regsb[ri]);
		s_region_clear(&regsw[ri]);
	}

	s_region_destroy(region);
}

int s_server_id_pri (int id)
{
	int i;

	if (id < 0) {
		return -1;
	}
	for (i = 0; i < S_CLIENTS_MAX; i++) {
		if (server->pri[i] == id) {
			return i;
		}
	}
	return -1;
}

int s_server_pri_id (int pri)
{
        if (pri >= 0) {
		return server->pri[pri];
	}
	return -1;
}

void s_server_pri_del (int id)
{
	int i;
	int p;
	int v;
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

	s_free(server->client[id].title.str);
	for (v = 0; v < 2; v++) {
		s_free(server->client[id].title.img[v].buf);
		s_free(server->client[id].title.img[v].mat);
	}
	memset(&(server->client[id]), 0, sizeof(s_clients_t));

	server->client[id].pid = -1;
	server->client[id].soc = -1;
	server->client[id].type = 0;
	server->client[id].resizeable = 1;

	if (p >= 0) {
		for (i = p; i < (S_CLIENTS_MAX - 1); i++) {
			server->pri[i] = server->pri[i + 1];
		}
		server->pri[S_CLIENTS_MAX - 1] = -1;
	}

	s_server_pri_set(SURFACE_CLOSE, &c);
}
