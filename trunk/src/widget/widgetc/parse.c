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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <expat.h>

#include "node.h"
#include "parse.h"

void parse_xml_start (void *x_data, const char *el, const char **attr)
{
	int p;
	node_t *node;
	xml_data_t *xdata = (xml_data_t *) x_data;

	free(xdata->path);
	xdata->path = strdup(el);
	
	node_init(&node);
	node->name = strdup(el);
	for (p = 0; attr[p]; p += 2) {
		if (strcmp(attr[p], "type") == 0) {
			node->type = strdup(attr[p + 1]);
		} else if (strcmp(attr[p], "id") == 0) {
			node->id = strdup(attr[p + 1]);
		}
	}
	node->parent = xdata->active;
	xdata->active = node;
	if (strcmp(node->name, "stylesheet") == 0) {
		xdata->elem = node;
		if (node->parent) {
			list_add(node->parent->nodes, node, -1);
		}
	} else {
		if (node->parent) {
			list_add(node->parent->nodes, node, -1);
		} else {
			xdata->root = node;
		}
	}
}

void parse_xml_end (void *x_data, const char *el)
{
	xml_data_t *xdata = (xml_data_t *) x_data;
	free(xdata->path);
	xdata->path = NULL;
	xdata->active = xdata->active->parent;
}

void parse_xml_char_hndl_fixup (char *out)
{
	int i;
	int j;
	/* Convert "&amp;" to "&" */
	for (i = 0; out[i] && out[i + 1] && out[i + 2] && out[i + 3] && out[i + 4]; i++) {
		if (out[i] == '&' && out[i + 1] == 'a' && out[i + 2] == 'm' && out[i + 3] == 'p' && out[i + 4] == ';') {
			for (j = i + 1; out[j]; j++)
				out[j] = out[j + 4];
			i--;
		}
	}
}

void parse_xml_char_hndl (void *x_data, const char *txt, int txtlen)
{
	char *str;
	unsigned int total = 0;
	unsigned int total_old = 0;
	xml_data_t *xdata = (xml_data_t *) x_data;
	if (txtlen > 0 &&
	    txt &&
	    xdata->path) {
	} else {
	    return;
	}
	if (xdata->active == NULL) {
		return;
	}
	if (xdata->active->value != NULL) {
		total_old = strlen(xdata->active->value);
	}
	total = (total_old + txtlen + 1) * sizeof(char *);
	xdata->active->value = (char *) realloc(xdata->active->value, total);
	if (total_old == 0) {
		xdata->active->value[0] = '\0';
	}
	strncat(xdata->active->value, txt, txtlen); 
	str = xdata->active->value;
	if (xdata->path) {
	    	if (xdata->active && xdata->active->value) {
	    		parse_xml_char_hndl_fixup(xdata->active->value);
	    	}
	}
}

int parse_xml_file (xml_data_t *xdata, char *name)
{
	int l = 0;
	FILE *fp;
	char *tmp;
	struct stat stbuf;
	stat(name, &stbuf);
	tmp = (char *) malloc(stbuf.st_size + 1);
	fp = fopen(name, "r");
	fread(tmp, 1, stbuf.st_size, fp);
	l = stbuf.st_size;
	XML_Parser p = XML_ParserCreate(NULL);
	if (!p) {
		fprintf(stderr, "Couldn't allocate memory for parser\n");
		return -1;
	}
	XML_SetUserData(p, xdata);
	XML_SetElementHandler(p, parse_xml_start, parse_xml_end);
	XML_SetCharacterDataHandler(p, parse_xml_char_hndl);
	if (!XML_Parse(p, tmp, l, 1)) {
		fprintf(stderr, "Parse error at line %d:\n%s\n", XML_GetCurrentLineNumber(p), XML_ErrorString(XML_GetErrorCode(p)));
		return -1;
	}
	XML_ParserFree(p);
	free(tmp);
	fclose(fp);
	return 0;
}
