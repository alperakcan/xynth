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
#include <string.h>

#include "list.h"
#include "node.h"
#include "lang.h"

static unsigned long int hash_string (const char *str_param)
{
	#define HASHWORDBITS 32
	unsigned long int hval, g;
	const char *str = str_param;
	/* Compute the hash value for the given string.  */
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

static void sort_strings (list_t *list)
{
	int i;
	int j;
	lang_t *ing;
	lang_t *jng;
again:
	for (i = 0; !list_eol(list, i); i++) {
		ing = (lang_t *) list_get(list, i);
		for (j = i; !list_eol(list, j); j++) {
			jng = (lang_t *) list_get(list, j);
			if (jng->hash < ing->hash) {
				list_remove(list, j);
				list_add(list, jng, i);
				goto again;
			}
		}
	}
}

void lang_generate (node_t *node, char *file)
{
	int i;
	unsigned long int offset;
	node_t *id;
	node_t *str;
	node_t *msg;
	lmsg_t *lmsg;
	lang_t *lang;
	list_t *llang;
	list_t *llmsg;
	char *fname;
	FILE *flang;
	lang_header_t lheader = {{"xynth.xo"}, 3};
	fname = (char *) malloc(sizeof(char) * (strlen(file) + 20));
	sprintf(fname, "%s.%s.xo", file, node->id);
	flang = fopen(fname, "w+");
	list_init(&llang);
	list_init(&llmsg);
	while ((msg = node_get_node(node, "message")) != NULL) {
		id = node_get_node(msg, "id");
		str = node_get_node(msg, "str");
		if (id && str && id->value && str->value) {
			lmsg = (lmsg_t *) malloc(sizeof(lmsg_t));
			lang = (lang_t *) malloc(sizeof(lang_t));
			memset(lmsg, 0, sizeof(lmsg_t));
			memset(lang, 0, sizeof(lang_t));
			node_string_normalize(id->value);
			node_string_normalize(str->value);
			lang->hash = hash_string(id->value);
			lmsg->id = id->value;
			lmsg->str = str->value;
			lang->id_len = strlen(lmsg->id);
			lang->str_len = strlen(lmsg->str);
			list_add(llang, lang, -1);
			list_add(llmsg, lmsg, -1);
		}
		msg->dontparse = 1;
		if (id) id->dontparse = 1;
		if (str) str->dontparse = 1;
	}
	lheader.count = llang->nb_elt;
	offset = sizeof(lang_header_t);
	offset += (sizeof(lang_t) * lheader.count);
	for (i = 0; !list_eol(llmsg, i); i++) {
		lmsg = (lmsg_t *) list_get(llmsg, i);
		lang = (lang_t *) list_get(llang, i);
		lang->id_offset = offset;
		offset += (strlen(lmsg->id) + 1);
	}
	for (i = 0; !list_eol(llmsg, i); i++) {
		lmsg = (lmsg_t *) list_get(llmsg, i);
		lang = (lang_t *) list_get(llang, i);
		lang->str_offset = offset;
		offset += (strlen(lmsg->str) + 1);
	}
	fwrite(&lheader, sizeof(lang_header_t), 1, flang);
	sort_strings(llang);
	for (i = 0; !list_eol(llang, i); i++) {
		lang = (lang_t *) list_get(llang, i);
		fwrite(lang, sizeof(lang_t), 1, flang);
	}
	for (i = 0; !list_eol(llmsg, i); i++) {
		lmsg = (lmsg_t *) list_get(llmsg, i);
		fwrite(lmsg->id, 1, strlen(lmsg->id) + 1, flang);
	}
	for (i = 0; !list_eol(llmsg, i); i++) {
		lmsg = (lmsg_t *) list_get(llmsg, i);
		fwrite(lmsg->str, 1, strlen(lmsg->str) + 1, flang);
	}
	free(fname);
	fclose(flang);
}
