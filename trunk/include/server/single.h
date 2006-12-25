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

#ifndef SINGLE_H_
#define SINGLE_H_

/* single.c */
#if defined(SINGLE_APP)
void * s_server_single_app_start (void *arg);
void s_server_single_start (void);
void s_server_single_stop (void);
#endif

#endif /*SINGLE_H_*/
