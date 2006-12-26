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

#include <stdio.h>
#include <stdlib.h>
#include "xynth_.h"
#include "widget.h"

#include "table.h"

static int table_cmp_string (char *str, char *ptr)
{
	while (*str && *ptr) {
		if (*str++ != *ptr++) {
			return -1;
		}
	}
	if (*str || *ptr) {
		return -1;
	}
	return 0;
}

static unsigned long int table_hash_string (const char *str_param)
{
	#define HASHWORDBITS 32
	unsigned long int hval, g;
	const char *str = str_param;
	hval = 0;
	while (*str != '\0') {
		hval <<= 4;
		hval += (unsigned char) *str++;
		g = hval & ((unsigned long int) 0xf << (HASHWORDBITS - 4));
		if (g != 0) {
			hval ^= g >> (HASHWORDBITS - 8);
			hval ^= g;
		}
	}
	return hval;
}

int table_add (w_table_t *table, unsigned int tlen, unsigned int mask, char *name, void *data)
{
	w_node_t *node;
	w_table_t *tmp;
	unsigned int o;
	unsigned int l;
	unsigned int mlen;
	unsigned long int h;
	for (l = 32; l > 0; l--) {
		if (mask & (1 << (l - 1))) {
			break;
		}
	}
	mlen = l;
	h = table_hash_string(name);
	tmp = table;
	for (l = 0; l < tlen; l++) {
		o = h & mask;
		if (tmp[o].table == NULL) {
			tmp[o].table = (w_table_t *) s_malloc(sizeof(w_table_t) * (mask + 1));
			memset(tmp[o].table, 0, sizeof(w_table_t) * (mask + 1));
		}
		tmp = tmp[o].table;
		h >>= mlen;
		if (h == 0) {
			break;
		}
	}
	node = (w_node_t *) s_malloc(sizeof(w_node_t));
	memset(node, 0, sizeof(w_node_t));
	node->key = h;
	h &= mask;
	node->name = strdup(name);
	node->data = data;
	node->table = &tmp[h];
	node->next = tmp[h].node;
	if (node->next) {
		node->next->prev = node;
	}
	tmp[h].node = node;
	return 0;
}

w_node_t * table_get_node (w_table_t *table, unsigned int tlen, unsigned int mask, char *name)
{
	w_node_t *node;
	w_table_t *tmp;
	unsigned int o;
	unsigned int l;
	unsigned int mlen;
	unsigned long int h;
	for (l = 32; l > 0; l--) {
		if (mask & (1 << (l - 1))) {
			break;
		}
	}
	mlen = l;
	h = table_hash_string(name);
	tmp = table;
	for (l = 0; l < tlen; l++) {
		o = h & mask;
		if (tmp[o].table == NULL) {
			return NULL;
		}
		tmp = tmp[o].table;
		h >>= mlen;
		if (h == 0) {
			break;
		}
	}
	node = tmp[h & mask].node;
	while (node) {
		if (node->key == h &&
		    table_cmp_string(node->name, name) == 0) {
			return node;
		}
		node = node->next;
	}
	return NULL;
}

void * table_get_data (w_table_t *table, unsigned int tlen, unsigned int mask, char *name)
{
	w_node_t *node;
	node = table_get_node(table, tlen, mask, name);
	if (node) {
		return node->data;
	} else {
		return NULL;
	} 
}

int table_del (w_table_t *table, unsigned int tlen, unsigned int mask, char *name)
{
	w_node_t *node;
	node = table_get_node(table, tlen, mask, name);
	if (node == NULL) {
		return -1;
	}
	if (node->prev) {
		node->prev->next = node->next;
	} else {
		node->table->node = node->next;
	}
	s_free(node->name);
	s_free(node);
	return 0;
}

int table_print (w_table_t *table, unsigned int len)
{
	unsigned int i;
	w_node_t *node;
	if (table == NULL) {
		return -1;
	}
	for (i = 0; i < len; i++) {
		table_print(table[i].table, len);
		node = table[i].node;
		while (node) {
			printf("%s\n", node->name);
			node = node->next;
		}
	}
	return 0;
}

int table_init (w_table_t **table, unsigned int size)
{
	w_table_t *tbl;
	tbl = (w_table_t *) s_malloc(sizeof(w_table_t) * size);
	memset(tbl, 0, sizeof(w_table_t) * size);
	*table = tbl;
	return 0;
}

int table_uninit (w_table_t *table, unsigned int len)
{
	w_node_t *tmp;
	w_node_t *node;
	unsigned int i;
	if (table == NULL) {
		return -1;
	}
	for (i = 0; i < len; i++) {
		table_uninit(table[i].table, len);
		node = table[i].node;
		while (node) {
			tmp = node;
			node = node->next;
			s_free(tmp->name);
			s_free(tmp);
		}
	}
	s_free(table);
	return 0;
}
