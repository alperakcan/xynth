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

#ifndef KBD_H_
#define KBD_H_

/* kbd.c */
void s_server_kbd_switch_handler (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_kbd_window_close_handler (s_window_t *window, s_event_t *event, s_handler_t *handler);
void s_server_kbd_server_quit_handler (s_window_t *window, s_event_t *event, s_handler_t *handler);
int s_server_kbd_update (s_window_t *window, s_pollfd_t *pfd);
void s_server_kbd_init (s_server_conf_t *cfg, s_video_input_t *keybd);
int s_server_kbd_uninit (s_window_t *window, s_pollfd_t *pfd);
S_KEYCODE_CODE s_server_keyname_to_keycode (char *name);

#endif /*KBD_H_*/
