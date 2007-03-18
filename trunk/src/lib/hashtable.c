/***************************************************************************
    begin                : Sun Mar 18 2007
    copyright            : (C) 2007 by Alper Akcan
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


#include "xynth_.h"

static int s_hashtable_cmp_string (char *str, char *ptr)
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

static unsigned long int s_hashtable_hash_string (const char *str_param)
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

s_hashtable_node_t * s_hashtable_get_node (s_hashtable_t *htable, char *name)
{
	unsigned int o;
	unsigned int l;
	unsigned int mlen;
	unsigned int mask;
	unsigned int depth;
	unsigned long int h;
	s_hashtable_node_t *node;
	s_hashtable_table_t *tmp;
	mask = htable->mask;
	depth =htable->depth;
	for (l = 32; l > 0; l--) {
		if (mask & (1 << (l - 1))) {
			break;
		}
	}
	mlen = l;
	h = s_hashtable_hash_string(name);
	tmp = htable->table;
	for (l = 0; l < depth; l++) {
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
		    s_hashtable_cmp_string(node->name, name) == 0) {
			return node;
		}
		node = node->next;
	}
	return NULL;
}

int s_hashtable_add (s_hashtable_t *htable, char *name, void *data)
{
	unsigned int o;
	unsigned int l;
	unsigned int mlen;
	unsigned int mask;
	unsigned int depth;
	unsigned long int h;
	s_hashtable_node_t *node;
	s_hashtable_table_t *tmp;
	mask = htable->mask;
	depth =htable->depth;
	for (l = 32; l > 0; l--) {
		if (mask & (1 << (l - 1))) {
			break;
		}
	}
	mlen = l;
	h = s_hashtable_hash_string(name);
	tmp = htable->table;
	for (l = 0; l < depth; l++) {
		o = h & mask;
		if (tmp[o].table == NULL) {
			tmp[o].table = (s_hashtable_table_t *) s_malloc(sizeof(s_hashtable_t) * htable->size);
			memset(tmp[o].table, 0, sizeof(s_hashtable_table_t) * htable->size);
		}
		tmp = tmp[o].table;
		h >>= mlen;
		if (h == 0) {
			break;
		}
	}
	node = (s_hashtable_node_t *) s_malloc(sizeof(s_hashtable_node_t));
	memset(node, 0, sizeof(s_hashtable_node_t));
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

int s_hashtable_del (s_hashtable_t *htable, char *name)
{
	s_hashtable_node_t *node;
	node = s_hashtable_get_node(htable, name);
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

void * s_hashtable_get_data (s_hashtable_t *htable, char *name)
{
	s_hashtable_node_t *node;
	node = s_hashtable_get_node(htable, name);
	if (node) {
		return node->data;
	} else {
		return NULL;
	} 
}
int s_hashtable_init (s_hashtable_t **htable, unsigned int mask, unsigned int depth)
{
	s_hashtable_t *htbl;
	htbl = (s_hashtable_t *) s_malloc(sizeof(s_hashtable_t));
	htbl->size = mask + 1;
	htbl->mask = mask;
	htbl->depth = depth;
	htbl->table = (s_hashtable_table_t *) s_malloc(sizeof(s_hashtable_table_t) * htbl->size);
	memset(htbl->table, 0, sizeof(s_hashtable_table_t) * htbl->size);
	*htable = htbl;
	return 0;
}

int s_hashtable_uninit_table (s_hashtable_table_t *table, unsigned int size)
{
	s_hashtable_node_t *tmp;
	s_hashtable_node_t *node;
	unsigned int i;
	if (table == NULL) {
		return -1;
	}
	for (i = 0; i < size; i++) {
		s_hashtable_uninit_table(table[i].table, size);
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

int s_hashtable_uninit (s_hashtable_t *htable)
{
	s_hashtable_uninit_table(htable->table, htable->size);
	s_free(htable);
	return 0;
}
