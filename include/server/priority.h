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

#ifndef S_PRIORITY_H_
#define S_PRIORITY_H_

typedef enum {
	SURFACE_CLOSE	 = 0x1,
	SURFACE_FOCUS	 = 0x2,
	SURFACE_REDRAW	 = 0x3,
	SURFACE_REFRESH	 = 0x4,
	SURFACE_CHANGED	 = 0x5
} S_SURFACE_CHNGF;

/* priority.c */
void s_server_pri_set (S_SURFACE_CHNGF flag, ...);
void s_server_pri_set_ (S_SURFACE_CHNGF flag, int id, s_rect_t *c0, s_rect_t *c1);
int s_server_id_pri (int id);
int s_server_pri_id (int pri);
void s_server_pri_del (int id);

#endif /* S_PRIORITY_H_ */
