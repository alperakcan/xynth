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

#ifndef WINDOW_H_
#define WINDOW_H_

/* window.c */
void s_server_window_new (int id);
void s_server_window_title (int id, char *title);
void s_server_putbox (s_window_t *window, int id, s_rect_t *coor, int x, int y, s_image_t *img);
void s_server_putmat (s_window_t *window, int id, s_rect_t *coor, int x, int y, s_image_t *img);
int s_server_window_form_mat_verbose (int id);
void s_server_window_form_mat (int v, int id, int mi, s_rect_t *coor, void (*func) (s_window_t *, int, s_rect_t *, int, int, s_image_t *));
void s_server_window_form (int id, s_rect_t *_coor_);
void s_server_window_matrix (int id, int mi, s_rect_t *_coor_);
void s_server_window_matrix_add (int id, s_rect_t *_coor_);
void s_server_window_matrix_del (int id, s_rect_t *_coor_);
void s_server_window_calculate (int id);
int s_server_window_is_parent_temp (int pid, int cid);
int s_server_window_temp_parent (int cid);
void s_server_window_close_temps (int id);
void s_server_window_close_id (int id);
void s_server_window_hide_id (int id);
void s_server_window_close (s_window_t *window);
void s_server_window_move_resize (int id, s_rect_t *new);
void s_server_window_maximize (s_window_t *window);

#endif /*WINDOW_H_*/
