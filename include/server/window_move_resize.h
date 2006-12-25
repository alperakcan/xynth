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

#ifndef WINDOW_MODE_RESIZE_H_
#define WINDOW_MODE_RESIZE_H_

/* window_move_resize.c */
void s_server_window_lines_draw_ (s_rect_t *coor, s_rect_t *rect, int c);
void s_server_window_lines_draw (s_rect_t *lnew);
void s_server_window_lines_clear_ (s_rect_t *told, s_rect_t *tnew);
void s_server_window_lines_clear (s_rect_t *lold, s_rect_t *lnew);
void s_server_window_while (s_rect_t *move, int flag);
void s_server_window_finish (int id, s_rect_t *move);
void s_server_window_move (s_window_t *window);
void s_server_window_resize_up (s_window_t *window);
void s_server_window_resize_up_left (s_window_t *window);
void s_server_window_resize_left (s_window_t *window);
void s_server_window_resize_down_left (s_window_t *window);
void s_server_window_resize_down (s_window_t *window);
void s_server_window_resize_down_right (s_window_t *window);
void s_server_window_resize_right (s_window_t *window);
void s_server_window_resize_up_right (s_window_t *window);

#endif /*WINDOW_MODE_RESIZE_H_*/
