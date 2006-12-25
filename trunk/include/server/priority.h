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

#ifndef PRIORITY_H_
#define PRIORITY_H_

/* priority.c */
void s_server_pri_set (S_SURFACE_CHNGF flag, ...);
void s_server_pri_set_ (S_SURFACE_CHNGF flag, int id, s_rect_t *c0, s_rect_t *c1);
int s_server_id_pri (int id);
int s_server_pri_id (int pri);
void s_server_pri_del (int id);

#endif /*PRIORITY_H_*/
