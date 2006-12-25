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

#ifndef S_MOUSE_H_
#define S_MOUSE_H_

struct s_cursor_s {
	int sw;
	int x;
	int y;
	int xyid;
	int xyid_old;
	s_image_t *img;
	s_image_t images[MOUSE_CURSOR_MAX];
};

/* mouse.c */
void s_server_cursor_uninit (void);
void s_server_cursor_init (void);
void s_server_cursor_image_set (int which, int c0, int c1, unsigned int *c);
void s_server_cursor_matrix_add (void);
void s_server_cursor_draw (void);
void s_server_cursor_select (S_MOUSE_CURSOR c);
void s_server_cursor_position (int x, int y);
int s_mouse_getx (void);
int s_mouse_gety (void);
void s_mouse_setxrange (s_window_t *window, int a, int b);
void s_mouse_setyrange (s_window_t *window, int a, int b);
void s_server_mouse_setcursor (S_MOUSE_CURSOR c);
void s_server_mouse_draw (void);
int s_server_mouse_uninit (s_window_t *window, s_pollfd_t *pfd);
int s_server_mouse_update (s_window_t *window, s_pollfd_t *pfd);
void s_server_mouse_init (s_server_conf_t *cfg, s_video_input_t *mouse);

#endif /* S_MOUSE_H_ */
