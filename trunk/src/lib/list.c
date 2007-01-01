/***************************************************************************
    begin                : Tue May 25 2004
    copyright            : (C) 2004 - 2007 by Alper Akcan
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

#include <stdlib.h>
#include "xynth_.h"

int s_list_init (s_list_t **li)
{
	(*li) = (s_list_t *) s_malloc(sizeof(s_list_t));
	if ((*li) == NULL) {
		return -1;
	}
	(*li)->nb_elt = 0;
	return 0;
}

int s_list_uninit (s_list_t *li)
{
	s_free(li);
	return 0;
}

int s_list_eol (s_list_t *li, int i)
{
        if (li == NULL) {
		return 1;
	}

	if ((i >= 0) && (i < li->nb_elt)) {
		return 0;
	}

	/* end of list */
	return 1;
}

void * s_list_get (s_list_t *li, int pos)
{
	int i = 0;
	s_list_node_t *ntmp;

	if ((li == NULL) || (pos < 0) || (pos >= li->nb_elt)) {
		/* element does not exist */
		return NULL;
	}

	/* exist because nb_elt > 0 */
	ntmp = li->node;

	while (pos > i) {
		i++;
		ntmp = (s_list_node_t *) ntmp->next;
	}

	return ntmp->element;
}

int s_list_remove (s_list_t *li, int pos)
{
	int i = 0;
	s_list_node_t *ntmp;

	if ((li == NULL) || (pos < 0) || (pos >= li->nb_elt)) {
		/* element does not exist */
		return -1;
	}

	/* exist because nb_elt > 0 */
	ntmp = li->node;

	if ((pos == 0)) {
		/* special case  */
		li->node = (s_list_node_t *) ntmp->next;
		li->nb_elt--;
		s_free(ntmp);
		return li->nb_elt;
	}

	while (pos > (i + 1)) {
		i++;
		ntmp = (s_list_node_t *) ntmp->next;
	}

	/* insert new node */
	{
		s_list_node_t *remnode;

		remnode = (s_list_node_t *) ntmp->next;
		ntmp->next = ((s_list_node_t *) ntmp->next)->next;
		s_free(remnode);
		li->nb_elt--;
	}

	return li->nb_elt;
}

int s_list_add (s_list_t *li, void *el, int pos)
{
	int i = 0;
	s_list_node_t *ntmp;
	
	if (li == NULL) {
		return -1;
	}

	if ((pos == -1) || (pos >= li->nb_elt)) {
		/* insert at the end  */
		pos = li->nb_elt;
	}

	if (li->nb_elt == 0) {
		li->node = (s_list_node_t *) s_malloc(sizeof(s_list_node_t));
		li->node->element = el;
		li->nb_elt++;
		return li->nb_elt;
	}

	/* exist because nb_elt > 0  */
	ntmp = li->node;

	if (pos == 0) {
		li->node = (s_list_node_t *) s_malloc(sizeof(s_list_node_t));
		li->node->element = el;
		li->node->next = ntmp;
		li->nb_elt++;
		return li->nb_elt;
	}

	/* pos = 0 insert before first elt  */
	while (pos > (i + 1)) {
		i++;
		/* when pos > i next node exist  */
		ntmp = (s_list_node_t *) ntmp->next;
	}

	/* if pos == nb_elt next node does not exist  */
	if (pos == li->nb_elt) {
		ntmp->next = (s_list_node_t *) s_malloc(sizeof(s_list_node_t));
		ntmp = (s_list_node_t *) ntmp->next;
		ntmp->element = el;
		li->nb_elt++;
		return li->nb_elt;
	}

	/* here pos == i so next node is where we want to insert new node */
	{
		s_list_node_t *nextnode = (s_list_node_t *) ntmp->next;
		ntmp->next = (s_list_node_t *) s_malloc(sizeof(s_list_node_t));
		ntmp = (s_list_node_t *) ntmp->next;
		ntmp->element = el;
		ntmp->next = nextnode;
		li->nb_elt++;
	}

	return li->nb_elt;
}

void * s_list_find (s_list_t *list, void *node, int (*cmp_func) (void *, void *))
{
	int pos = 0;
	void *tmp = NULL;

	while (!s_list_eol(list, pos)) {
		void *node_;
		node_ = s_list_get(list, pos);
		if (cmp_func(node, node_) == 0) {
			tmp = node_;
			break;
		}
		pos++;
	}

	return tmp;
}

int s_list_get_pos (s_list_t *list, void *node)
{
	int pos = 0;

	while (!s_list_eol(list, pos)) {
		void *node_;
		node_ = s_list_get(list, pos);
		if (node_ == node) {
			return pos;
		}
		pos++;
	}

	return -1;
}
