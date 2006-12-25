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

#ifndef S_EVENT_H_
#define S_EVENT_H_

/* event.c */
void s_server_event_parse_keyboard (s_video_input_data_keybd_t *keybd);
int s_server_event_parse_mouse (s_video_input_data_mouse_t *mouse);
int s_event_changed_ (s_window_t *window);
void s_server_event_changed (void);

#endif /* S_EVENT_H_ */
