/***************************************************************************
    begin                : Wed Dec 20 2006
    copyright            : (C) 2006 - 2008 by Alper Akcan
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

typedef struct s_list_s s_list_t;
typedef struct s_list_node_s s_list_node_t;

struct s_list_node_s {
        void *next;
        void *element;
};

struct s_list_s {
        int nb_elt;
        s_list_node_t *node;
};

int s_list_init (s_list_t **li);
int s_list_uninit (s_list_t *li);
int s_list_eol (s_list_t *li, int i);
void * s_list_get (s_list_t *li, int pos);
int s_list_remove (s_list_t *li, int pos);
int s_list_add (s_list_t *li, void *el, int pos);

#endif /*LIST_H_*/
