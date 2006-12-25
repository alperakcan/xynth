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

#ifndef S_SOCKET_H_
#define S_SOCKET_H_

/* socket.c */
int s_server_socket_listen_new (int id);
int s_server_socket_listen_display (int id);
int s_server_socket_listen_configure (int id);
int s_server_socket_listen_stream (int id);
int s_server_socket_listen_close (int id);
int s_server_socket_listen_show (int id);
int s_server_socket_listen_event (int id);
int s_server_socket_listen_window_close (int soc);
int s_server_socket_listen_parse (int soc);
int s_server_socket_client_in_f (s_window_t *window, s_pollfd_t *pfd);
int s_server_socket_client_ierr_f (s_window_t *window, s_pollfd_t *pfd);
int s_server_socket_listen_accept (int soc);
int s_server_socket_request_event (int id);
int s_server_socket_request_close (int id);
int s_server_socket_request_expose (int id, s_rect_t *changed);
int s_server_socket_request_desktop (int id);
int s_server_socket_request (S_SOC_DATA req, int id, ...);
int s_server_socket_uninit (s_window_t *window, s_pollfd_t *pfd);
int s_server_socket_in_f (s_window_t *window, s_pollfd_t *pfd);
int s_server_socket_ierr_f (s_window_t *window, s_pollfd_t *pfd);
int s_server_socket_init_uds (void);
int s_server_socket_init_tcp (void);
int s_server_socket_init_pipe (void);
void s_server_socket_init (void);

#endif /* S_SOCKET_H_ */
