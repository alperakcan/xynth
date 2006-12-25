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

#ifndef SURFACE_H_
#define SURFACE_H_

/* surface.c */
void s_server_surface_matrix_find (s_rect_t *coor, int *dm);
void s_server_surface_matrix_add_this (int id, s_rect_t *coor, s_rect_t *mcoor, unsigned char *mat);
void s_server_surface_matrix_add_id (int id, s_rect_t *coor);
void s_server_surface_matrix_add (int id, s_rect_t *coor);
void s_server_surface_matrix_del (int id);
void s_server_surface_matrix_del_coor (s_rect_t *coor);
void s_server_surface_clean (s_rect_t *coor);
void s_server_surface_background (s_rect_t *coor);
void s_server_surface_lock_real (void);
void s_server_surface_refresh (void);

#endif /*SURFACE_H_*/
