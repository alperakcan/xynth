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

#ifndef LIST_H_
#define LIST_H_

typedef struct list_s list_t;
typedef struct list_node_s list_node_t;

struct list_node_s {
        void *next;
        void *element;
};

struct list_s {
        int nb_elt;
        list_node_t *node;
};

int list_init (list_t **li);
int list_uninit (list_t *li);
int list_eol (list_t *li, int i);
void * list_get (list_t *li, int pos);
int list_remove (list_t *li, int pos);
int list_add (list_t *li, void *el, int pos);

#endif /*LIST_H_*/
