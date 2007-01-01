/***************************************************************************
    begin                : Mon Aug 9 2004
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

#include "../lib/xynth_.h"
#include "server.h"

#define s_server_window_btn_y(a)\
	int id = s_server_pri_id(0);\
	if (id < 0) {\
		return;\
	}\
	if (id == server->cursor.xyid) {\
		s_server_mouse_setcursor(a);\
	} else {\
		s_server_mouse_setcursor(server->client[id].cursor);\
	}

void s_server_window_btn_resize_oh (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	int id = s_server_pri_id(0);
	if (id < 0) {
		s_server_mouse_setcursor(MOUSE_CURSOR_ARROW);
	} else {
		s_server_mouse_setcursor(server->client[id].cursor);
	}
}
void s_server_window_btn_resize_u_o (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_window_btn_y(MOUSE_CURSOR_SIZEV);
}
void s_server_window_btn_resize_ur_o (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_window_btn_y(MOUSE_CURSOR_SIZES);
}
void s_server_window_btn_resize_r_o (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_window_btn_y(MOUSE_CURSOR_SIZEH);
}
void s_server_window_btn_resize_dr_o (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_window_btn_y(MOUSE_CURSOR_SIZEB);
}
void s_server_window_btn_resize_d_o (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_window_btn_y(MOUSE_CURSOR_SIZEV);
}
void s_server_window_btn_resize_dl_o (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_window_btn_y(MOUSE_CURSOR_SIZES);
}
void s_server_window_btn_resize_l_o (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_window_btn_y(MOUSE_CURSOR_SIZEH);
}
void s_server_window_btn_resize_ul_o (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_window_btn_y(MOUSE_CURSOR_SIZEB);
}

#define s_server_window_btn_x(a, b)\
	int id;\
	s_rect_t coor;\
	id = s_server_pri_id(0);\
	if (id < 0) {\
		return;\
	}\
	coor.x = server->client[id].button[b].x;\
	coor.y = server->client[id].button[b].y;\
	coor.w = server->client[id].button[b].w;\
	coor.h = server->client[id].button[b].h;\
	s_server_putbox(window, id, &coor, coor.x, coor.y, &server->theme.button[a][b]);\
	s_server_surface_update(&coor);

#define s_server_window_btn_x_(a, b)\
	if ((event->mouse->px >= handler->mouse.x) &&\
	    (event->mouse->px <= (handler->mouse.x + handler->mouse.w)) &&\
	    (event->mouse->py >= handler->mouse.y) &&\
	    (event->mouse->py <= (handler->mouse.y + handler->mouse.h))) {\
		s_server_window_btn_x(a, b);\
	}

void s_server_window_btn_menu_p (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_window_btn_x(2, MENU);
}
void s_server_window_btn_menu_ho (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_window_btn_x_(2, MENU);
}
void s_server_window_btn_menu_oh (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_window_btn_x(1, MENU);
}
void s_server_window_btn_menu_r (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_window_btn_menu_oh(window, event, handler);
}

void s_server_window_btn_hide_p (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_window_btn_x(2, HIDE);
}
void s_server_window_btn_hide_ho (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_window_btn_x_(2, HIDE);
}
void s_server_window_btn_hide_oh (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_window_btn_x(1, HIDE);
}
void s_server_window_btn_hide_r (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_window_btn_hide_oh(window, event, handler);
}

void s_server_window_btn_maximize_p (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_window_btn_x(2, MAXIMIZE);
}
void s_server_window_btn_maximize_ho (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_window_btn_x_(2, MAXIMIZE);
}
void s_server_window_btn_maximize_oh (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_window_btn_x(1, MAXIMIZE);
}
void s_server_window_btn_maximize_r (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_window_btn_maximize_oh(window, event, handler);
	s_server_window_maximize(window);
}

void s_server_window_btn_close_p (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_window_btn_x(2, CLOSE);
}
void s_server_window_btn_close_ho (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_window_btn_x_(2, CLOSE);
}
void s_server_window_btn_close_oh (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_window_btn_x(1, CLOSE);
}
void s_server_window_btn_close_r (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_window_close(window);
}
void s_server_window_btn_move (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_window_move(window);
}
void s_server_window_btn_resize_up (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_window_resize_up(window);
}
void s_server_window_btn_resize_up_left (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_window_resize_up_left(window);
}
void s_server_window_btn_resize_up_right (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_window_resize_up_right(window);
}
void s_server_window_btn_resize_left (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_window_resize_left(window);
}
void s_server_window_btn_resize_right (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_window_resize_right(window);
}
void s_server_window_btn_resize_down (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_window_resize_down(window);
}
void s_server_window_btn_resize_down_left (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_window_resize_down_left(window);
}
void s_server_window_btn_resize_down_right (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	s_server_window_resize_down_right(window);
}

void s_server_window_handlers_del_mouse (void)
{
	int i;
	for (i = 0; i < 20; i++) {
		s_handler_del(server->window, server->whndl[i]);
	}
}

void s_server_window_handlers_add_mouse (int id)
{
        int v = 1;
	s_handler_t *hndl;

	/* menu */
	hndl = server->whndl[0];
	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = server->client[id].button[MENU].x + server->theme.button[v][MENU].handler->x;
	hndl->mouse.y = server->client[id].button[MENU].y + server->theme.button[v][MENU].handler->y;
	hndl->mouse.w = server->theme.button[v][MENU].handler->w;
	hndl->mouse.h = server->theme.button[v][MENU].handler->h;
	hndl->mouse.button = MOUSE_LEFTBUTTON;
	hndl->mouse.p = s_server_window_btn_menu_p;
	hndl->mouse.r = s_server_window_btn_menu_r;
	hndl->mouse.oh = s_server_window_btn_menu_oh;
	hndl->mouse.ho = s_server_window_btn_menu_ho;
	hndl->mouse.hoh = s_server_window_btn_menu_oh;
	s_handler_add(server->window, hndl);

	/* hide */
	hndl = server->whndl[1];
	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = server->client[id].button[HIDE].x + server->theme.button[v][HIDE].handler->x;
	hndl->mouse.y = server->client[id].button[HIDE].y + server->theme.button[v][HIDE].handler->y;
	hndl->mouse.w = server->theme.button[v][HIDE].handler->w;
	hndl->mouse.h = server->theme.button[v][HIDE].handler->h;
	hndl->mouse.button = MOUSE_LEFTBUTTON;
	hndl->mouse.p = s_server_window_btn_hide_p;
	hndl->mouse.r = s_server_window_btn_hide_r;
	hndl->mouse.oh = s_server_window_btn_hide_oh;
	hndl->mouse.ho = s_server_window_btn_hide_ho;
	hndl->mouse.hoh = s_server_window_btn_hide_oh;
	s_handler_add(server->window, hndl);

	/* close */
	hndl = server->whndl[2];
	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = server->client[id].button[CLOSE].x + server->theme.button[v][CLOSE].handler->x;
	hndl->mouse.y = server->client[id].button[CLOSE].y + server->theme.button[v][CLOSE].handler->y;
	hndl->mouse.w = server->theme.button[v][CLOSE].handler->w;
	hndl->mouse.h = server->theme.button[v][CLOSE].handler->h;
	hndl->mouse.button = MOUSE_LEFTBUTTON;
	hndl->mouse.p = s_server_window_btn_close_p;
	hndl->mouse.r = s_server_window_btn_close_r;
	hndl->mouse.oh = s_server_window_btn_close_oh;
	hndl->mouse.ho = s_server_window_btn_close_ho;
	hndl->mouse.hoh = s_server_window_btn_close_oh;
	s_handler_add(server->window, hndl);

	if (server->client[id].resizeable == 1) {
		/* maximize */
	        hndl = server->whndl[3];
	        hndl->type = MOUSE_HANDLER;
		hndl->mouse.x = server->client[id].button[MAXIMIZE].x + server->theme.button[v][MAXIMIZE].handler->x;
		hndl->mouse.y = server->client[id].button[MAXIMIZE].y + server->theme.button[v][MAXIMIZE].handler->y;
		hndl->mouse.w = server->theme.button[v][MAXIMIZE].handler->w;
		hndl->mouse.h = server->theme.button[v][MAXIMIZE].handler->h;
		hndl->mouse.button = MOUSE_LEFTBUTTON;
		hndl->mouse.p = s_server_window_btn_maximize_p;
		hndl->mouse.r = s_server_window_btn_maximize_r;
		hndl->mouse.oh = s_server_window_btn_maximize_oh;
		hndl->mouse.ho = s_server_window_btn_maximize_ho;
		hndl->mouse.hoh = s_server_window_btn_maximize_oh;
		s_handler_add(server->window, hndl);
	}

        if (server->client[id].win.w > server->theme.form_min.w) {
		/* move 2 */
	        hndl = server->whndl[4];
	        hndl->type = MOUSE_HANDLER;
		hndl->mouse.x = server->client[id].form[TOP_2].x;
		hndl->mouse.y = server->client[id].form[TOP_2].y + server->theme.form[v][TOP_2].handler->y + 4;
		hndl->mouse.w = server->client[id].form[TOP_2].w;
		hndl->mouse.h = server->theme.form[v][TOP_2].handler->h - 4;
		hndl->mouse.button = MOUSE_LEFTBUTTON;
		hndl->mouse.p = s_server_window_btn_move;
		s_handler_add(server->window, hndl);

                /* move 3 */
	        hndl = server->whndl[5];
	        hndl->type = MOUSE_HANDLER;
		hndl->mouse.x = server->client[id].form[TOP_3].x;
		hndl->mouse.y = server->client[id].form[TOP_3].y + server->theme.form[v][TOP_3].handler->y + 4;
		hndl->mouse.w = server->client[id].form[TOP_3].w;
		hndl->mouse.h = server->theme.form[v][TOP_3].handler->h - 4;
		hndl->mouse.button = MOUSE_LEFTBUTTON;
		hndl->mouse.p = s_server_window_btn_move;
		s_handler_add(server->window, hndl);


                /* move 4 */
	        hndl = server->whndl[6];
	        hndl->type = MOUSE_HANDLER;
		hndl->mouse.x = server->client[id].form[TOP_4].x;
		hndl->mouse.y = server->client[id].form[TOP_4].y + server->theme.form[v][TOP_4].handler->y + 4;
		hndl->mouse.w = server->client[id].form[TOP_4].w;
		hndl->mouse.h = server->theme.form[v][TOP_4].handler->h - 4;
		hndl->mouse.button = MOUSE_LEFTBUTTON;
		hndl->mouse.p = s_server_window_btn_move;
		s_handler_add(server->window, hndl);
	}

	/* move 5*/
	hndl = server->whndl[7];
	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = server->client[id].form[TOP_5].x;
	hndl->mouse.y = server->client[id].form[TOP_5].y + server->theme.form[v][TOP_5].handler->y + 4;
	hndl->mouse.w = server->client[id].form[TOP_5].w;
	hndl->mouse.h = server->theme.form[v][TOP_5].handler->h - 4;
	hndl->mouse.button = MOUSE_LEFTBUTTON;
	hndl->mouse.p = s_server_window_btn_move;
	s_handler_add(server->window, hndl);

	if (server->client[id].resizeable == 0) {
		return;
	}

	/* resize u1 */
	hndl = server->whndl[8];
	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = server->client[id].form[TOP_1].x;
	hndl->mouse.y = server->client[id].form[TOP_1].y + server->theme.form[v][TOP_1].handler->y;
	hndl->mouse.w = server->client[id].form[TOP_1].w;
	hndl->mouse.h = 4;
	hndl->mouse.button = MOUSE_LEFTBUTTON;
	hndl->mouse.p = s_server_window_btn_resize_up;
	hndl->mouse.o = s_server_window_btn_resize_u_o;
	hndl->mouse.oh = s_server_window_btn_resize_oh;
	s_handler_add(server->window, hndl);

        if (server->client[id].win.w > server->theme.form_min.w) {
		/* resize u2 */
	        hndl = server->whndl[9];
        	hndl->type = MOUSE_HANDLER;
		hndl->mouse.x = server->client[id].form[TOP_2].x;
		hndl->mouse.y = server->client[id].form[TOP_2].y + server->theme.form[v][TOP_2].handler->y;
		hndl->mouse.w = server->client[id].form[TOP_2].w;
		hndl->mouse.h = 4;
		hndl->mouse.button = MOUSE_LEFTBUTTON;
		hndl->mouse.p = s_server_window_btn_resize_up;
		hndl->mouse.o = s_server_window_btn_resize_u_o;
		hndl->mouse.oh = s_server_window_btn_resize_oh;
		s_handler_add(server->window, hndl);

		/* resize u3 */
	        hndl = server->whndl[10];
        	hndl->type = MOUSE_HANDLER;
		hndl->mouse.x = server->client[id].form[TOP_3].x;
		hndl->mouse.y = server->client[id].form[TOP_3].y + server->theme.form[v][TOP_3].handler->y;
		hndl->mouse.w = server->client[id].form[TOP_3].w;
		hndl->mouse.h = 4;
		hndl->mouse.button = MOUSE_LEFTBUTTON;
		hndl->mouse.p = s_server_window_btn_resize_up;
		hndl->mouse.o = s_server_window_btn_resize_u_o;
		hndl->mouse.oh = s_server_window_btn_resize_oh;
		s_handler_add(server->window, hndl);

                /* resize u4 */
	        hndl = server->whndl[11];
        	hndl->type = MOUSE_HANDLER;
		hndl->mouse.x = server->client[id].form[TOP_4].x;
		hndl->mouse.y = server->client[id].form[TOP_4].y + server->theme.form[v][TOP_4].handler->y;
		hndl->mouse.w = server->client[id].form[TOP_4].w;
		hndl->mouse.h = 4;
		hndl->mouse.button = MOUSE_LEFTBUTTON;
		hndl->mouse.p = s_server_window_btn_resize_up;
		hndl->mouse.o = s_server_window_btn_resize_u_o;
		hndl->mouse.oh = s_server_window_btn_resize_oh;
		s_handler_add(server->window, hndl);
	}

	/* resize u5 */
        hndl = server->whndl[12];
       	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = server->client[id].form[TOP_5].x;
	hndl->mouse.y = server->client[id].form[TOP_5].y + server->theme.form[v][TOP_5].handler->y;
	hndl->mouse.w = server->client[id].form[TOP_5].w;
	hndl->mouse.h = 4;
	hndl->mouse.button = MOUSE_LEFTBUTTON;
	hndl->mouse.p = s_server_window_btn_resize_up;
	hndl->mouse.o = s_server_window_btn_resize_u_o;
	hndl->mouse.oh = s_server_window_btn_resize_oh;
	s_handler_add(server->window, hndl);

        /* resize ul */
        hndl = server->whndl[13];
       	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = server->client[id].form[TOP_L].x + server->theme.form[v][TOP_L].handler->x;
	hndl->mouse.y = server->client[id].form[TOP_L].y + server->theme.form[v][TOP_L].handler->y;
	hndl->mouse.w = server->theme.form[v][TOP_L].handler->w;
	hndl->mouse.h = server->theme.form[v][TOP_L].handler->h;
	hndl->mouse.button = MOUSE_LEFTBUTTON;
	hndl->mouse.p = s_server_window_btn_resize_up_left;
	hndl->mouse.o = s_server_window_btn_resize_ul_o;
	hndl->mouse.oh = s_server_window_btn_resize_oh;
	s_handler_add(server->window, hndl);

	/* resize ur */
	hndl = server->whndl[14];
	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = server->client[id].form[TOP_R].x;
	hndl->mouse.y = server->client[id].form[TOP_R].y + server->theme.form[v][TOP_R].handler->y;
	hndl->mouse.w = server->theme.form[v][TOP_R].handler->w;
	hndl->mouse.h = server->theme.form[v][TOP_R].handler->h;
	hndl->mouse.button = MOUSE_LEFTBUTTON;
	hndl->mouse.p = s_server_window_btn_resize_up_right;
	hndl->mouse.o = s_server_window_btn_resize_ur_o;
	hndl->mouse.oh = s_server_window_btn_resize_oh;
	s_handler_add(server->window, hndl);

	/* resize l */
	hndl = server->whndl[15];
	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = server->client[id].form[LEFT].x + server->theme.form[v][LEFT].handler->x;
	hndl->mouse.y = server->client[id].form[LEFT].y;
	hndl->mouse.w = server->theme.form[v][LEFT].handler->w;
	hndl->mouse.h = server->client[id].form[LEFT].h;
	hndl->mouse.button = MOUSE_LEFTBUTTON;
	hndl->mouse.p = s_server_window_btn_resize_left;
	hndl->mouse.o = s_server_window_btn_resize_l_o;
	hndl->mouse.oh = s_server_window_btn_resize_oh;
	s_handler_add(server->window, hndl);

	/* resize r */
	hndl = server->whndl[16];
	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = server->client[id].form[RIGHT].x;
	hndl->mouse.y = server->client[id].form[RIGHT].y;
	hndl->mouse.w = server->theme.form[v][RIGHT].handler->w;
	hndl->mouse.h = server->client[id].form[RIGHT].h;
	hndl->mouse.button = MOUSE_LEFTBUTTON;
	hndl->mouse.p = s_server_window_btn_resize_right;
	hndl->mouse.o = s_server_window_btn_resize_r_o;
	hndl->mouse.oh = s_server_window_btn_resize_oh;
	s_handler_add(server->window, hndl);

	/* resize d */
	hndl = server->whndl[17];
	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = server->client[id].form[BTM].x;
	hndl->mouse.y = server->client[id].form[BTM].y;
	hndl->mouse.w = server->client[id].form[BTM].w;
	hndl->mouse.h = server->theme.form[v][BTM].handler->h;
	hndl->mouse.button = MOUSE_LEFTBUTTON;
	hndl->mouse.p = s_server_window_btn_resize_down;
	hndl->mouse.o = s_server_window_btn_resize_d_o;
	hndl->mouse.oh = s_server_window_btn_resize_oh;
	s_handler_add(server->window, hndl);

	/* resize dl */
	hndl = server->whndl[18];
	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = server->client[id].form[BTM_L].x + server->theme.form[v][BTM_L].handler->x;
	hndl->mouse.y = server->client[id].form[BTM_L].y;
	hndl->mouse.w = server->theme.form[v][BTM_L].handler->w;
	hndl->mouse.h = server->theme.form[v][BTM_L].handler->h;
	hndl->mouse.button = MOUSE_LEFTBUTTON;
	hndl->mouse.p = s_server_window_btn_resize_down_left;
	hndl->mouse.o = s_server_window_btn_resize_dl_o;
	hndl->mouse.oh = s_server_window_btn_resize_oh;
	s_handler_add(server->window, hndl);

	/* resize dr */
	hndl = server->whndl[19];
	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = server->client[id].form[BTM_R].x;
	hndl->mouse.y = server->client[id].form[BTM_R].y;
	hndl->mouse.w = server->theme.form[v][BTM_R].handler->w;
	hndl->mouse.h = server->theme.form[v][BTM_R].handler->h;
	hndl->mouse.button = MOUSE_LEFTBUTTON;
	hndl->mouse.p = s_server_window_btn_resize_down_right;
	hndl->mouse.o = s_server_window_btn_resize_dr_o;
	hndl->mouse.oh = s_server_window_btn_resize_oh;
	s_handler_add(server->window, hndl);
}
