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

#ifndef IRR_H_
#define IRR_H_

/* irr.c */
int s_video_helper_irr_init (s_server_conf_t *cfg);
void s_video_helper_irr_uninit (void);
int s_video_helper_irr_update (s_video_input_data_t *keybd);
int s_server_irr_add_code (char *key, char *code);
int s_server_irr_uninit (s_window_t *window, s_pollfd_t *pfd);
int s_server_irr_update (s_window_t *window, s_pollfd_t *pfd);
void s_server_irr_init (s_server_conf_t *cfg, s_video_input_t *irr);

#endif /*IRR_H_*/
