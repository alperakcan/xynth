/***************************************************************************
    begin                : Wed Dec 20 2006
    copyright            : (C) 2006 by Alper Akcan
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

#ifndef TABLE_H_
#define TABLE_H_

typedef struct w_node_s w_node_t;
typedef struct w_table_s w_table_t;

struct w_node_s {
	unsigned long int key;
	char *name;
	w_node_t *next;
	w_node_t *prev;
	w_table_t *table;
	void *data;
};

struct w_table_s {
	w_table_t *table;
	w_node_t *node;
};

int table_add (w_table_t *table, unsigned int tlen, unsigned int mask, char *name, void *data);
w_node_t * table_get_node (w_table_t *table, unsigned int tlen, unsigned int mask, char *name);
void * table_get_data (w_table_t *table, unsigned int tlen, unsigned int mask, char *name);
int table_del (w_table_t *table, unsigned int tlen, unsigned int mask, char *name);
int table_print (w_table_t *table, unsigned int len);
int table_init (w_table_t **table, unsigned int size);
int table_uninit (w_table_t *table, unsigned int len);

#endif /*TABLE_H_*/
