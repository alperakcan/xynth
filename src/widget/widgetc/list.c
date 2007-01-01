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

#include <stdio.h>
#include <stdlib.h>

#include "list.h"

int list_init (list_t **li)
{
	(*li) = (list_t *) malloc(sizeof(list_t));
	if ((*li) == NULL) {
		return -1;
	}
	(*li)->nb_elt = 0;
	return 0;
}

int list_uninit (list_t *li)
{
	free(li);
	return 0;
}

int list_eol (list_t *li, int i)
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

void * list_get (list_t *li, int pos)
{
	int i = 0;
	list_node_t *ntmp;
	if ((li == NULL) || (pos < 0) || (pos >= li->nb_elt)) {
		/* element does not exist */
		return NULL;
	}
	/* exist because nb_elt > 0 */
	ntmp = li->node;
	while (pos > i) {
		i++;
		ntmp = (list_node_t *) ntmp->next;
	}
	return ntmp->element;
}

int list_remove (list_t *li, int pos)
{
	int i = 0;
	list_node_t *ntmp;
	if ((li == NULL) || (pos < 0) || (pos >= li->nb_elt)) {
		/* element does not exist */
		return -1;
	}
	/* exist because nb_elt > 0 */
	ntmp = li->node;
	if ((pos == 0)) {
		/* special case  */
		li->node = (list_node_t *) ntmp->next;
		li->nb_elt--;
		free(ntmp);
		return li->nb_elt;
	}
	while (pos > (i + 1)) {
		i++;
		ntmp = (list_node_t *) ntmp->next;
	}
	/* insert new node */
	{
		list_node_t *remnode;
		remnode = (list_node_t *) ntmp->next;
		ntmp->next = ((list_node_t *) ntmp->next)->next;
		free(remnode);
		li->nb_elt--;
	}
	return li->nb_elt;
}

int list_add (list_t *li, void *el, int pos)
{
	int i = 0;
	list_node_t *ntmp;
	if (li == NULL) {
		return -1;
	}
	if ((pos == -1) || (pos >= li->nb_elt)) {
		/* insert at the end  */
		pos = li->nb_elt;
	}
	if (li->nb_elt == 0) {
		li->node = (list_node_t *) malloc(sizeof(list_node_t));
		li->node->element = el;
		li->nb_elt++;
		return li->nb_elt;
	}
	/* exist because nb_elt > 0  */
	ntmp = li->node;
	if (pos == 0) {
		li->node = (list_node_t *) malloc(sizeof(list_node_t));
		li->node->element = el;
		li->node->next = ntmp;
		li->nb_elt++;
		return li->nb_elt;
	}
	/* pos = 0 insert before first elt  */
	while (pos > (i + 1)) {
		i++;
		/* when pos > i next node exist  */
		ntmp = (list_node_t *) ntmp->next;
	}
	/* if pos == nb_elt next node does not exist  */
	if (pos == li->nb_elt) {
		ntmp->next = (list_node_t *) malloc(sizeof(list_node_t));
		ntmp = (list_node_t *) ntmp->next;
		ntmp->element = el;
		li->nb_elt++;
		return li->nb_elt;
	}
	/* here pos == i so next node is where we want to insert new node */
	{
		list_node_t *nextnode = (list_node_t *) ntmp->next;
		ntmp->next = (list_node_t *) malloc(sizeof(list_node_t));
		ntmp = (list_node_t *) ntmp->next;
		ntmp->element = el;
		ntmp->next = nextnode;
		li->nb_elt++;
	}
	return li->nb_elt;
}
