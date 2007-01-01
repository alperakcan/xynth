/***************************************************************************
    begin                : Wed Dec 20 2006
    copyright            : (C) 2006 - 2007 by Alper Akcan
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

#ifndef WIDGETR_H_
#define WIDGETR_H_

typedef struct ctable_s {
	unsigned int depth;
	unsigned int mask;
	w_table_t *table;
} ctable_t;

typedef struct w_widgetr_s w_widgetr_t;
int axtoi (char *hex);

#endif /*WIDGETR_H_*/
