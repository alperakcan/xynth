/***************************************************************************
    begin                : Sun Feb 16 2003
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

#ifndef WINDOW_HANDLER_H_
#define WINDOW_HANDLER_H_

/* window_handler.c */
void s_server_window_btn_resize_oh (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_resize_u_o (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_resize_ur_o (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_resize_r_o (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_resize_dr_o (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_resize_d_o (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_resize_dl_o (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_resize_l_o (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_resize_ul_o (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_menu_p (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_menu_oh (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_menu_ho (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_menu_r (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_hide_p (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_hide_oh (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_hide_ho (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_hide_r (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_maximize_p (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_maximize_oh (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_maximize_ho (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_maximize_r (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_close_p (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_close_oh (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_close_ho (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_close_r (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_move (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_resize_up (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_resize_up_left (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_resize_up_right (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_resize_left (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_resize_right (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_resize_down (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_resize_down_left (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_btn_resize_down_right (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_window_handlers_del_mouse (void);
void s_server_window_handlers_add_mouse (int id);

#endif /*WINDOW_HANDLER_H_*/
