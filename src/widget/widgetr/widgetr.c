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

#include "xynth_.h"
#include "widget/widget.h"
#include <getopt.h>

#include "list.h"
#include "node.h"
#include "parse.h"
#include "table.h"
#include "widgetr.h"
#include "code.h"

int axtoi (char *hex)
{
	int n = 0;
	int m = 0;
	int count;
	int intvalue = 0;
	int digit[8];
	while (n < 8) {
		if (hex[n]=='\0')
			break;
		if (hex[n] > 0x29 && hex[n] < 0x40)
			digit[n] = hex[n] & 0x0f;
		else if (hex[n] >='a' && hex[n] <= 'f')
			digit[n] = (hex[n] & 0x0f) + 9;
		else if (hex[n] >='A' && hex[n] <= 'F')
			digit[n] = (hex[n] & 0x0f) + 9;
		else
			break;
		n++;
	}
	count = n;
	m = n - 1;
	n = 0;
	while (n < count) {
		intvalue = intvalue | (digit[n] << (m << 2));
		m--;
		n++;
	}
	return (intvalue);
}

int main (int argc, char *argv[])
{
	char c;
	char *hex;
	node_t *root;
	unsigned int i;
	char *varf = NULL;
	char *vars = NULL;
	char *varc = NULL;
	char *vare = NULL;
	xml_data_t *xdata;
	ctable_t *widgetr;
	unsigned int option_index = 0;
	struct option long_options[] = {
		{"depth", 1, 0, 0},
		{"mask", 1, 0, 0},
		{"file", 1, 0, 0},
		{"style", 1, 0, 0},
		{"script", 1, 0, 0},
		{"engine", 1, 0, 0},
		{"help", 0, 0, 0},
		{0, 0, 0, 0},
	};

	widgetr = (ctable_t *) s_malloc(sizeof(ctable_t));
	memset(widgetr, 0, sizeof(ctable_t));
	
	while ((c = getopt_long(argc, argv, "e:c:s:f:d:m:h", long_options, &option_index)) != -1) {
		switch (c) {
			case 0:
				if (strcmp("depth", long_options[option_index].name) == 0) {
					goto option_tables;
				} else if (strcmp("mask", long_options[option_index].name) == 0) {
					goto option_mask;
				} else if (strcmp("file", long_options[option_index].name) == 0) {
					goto option_file;
				} else if (strcmp("style", long_options[option_index].name) == 0) {
					goto option_style;
				} else if (strcmp("script", long_options[option_index].name) == 0) {
					goto option_script;
				} else if (strcmp("engine", long_options[option_index].name) == 0) {
					goto option_engine;
				} else if (strcmp("help", long_options[option_index].name) == 0) {
					goto option_help;
				}
				break;
			case 'e':
option_engine:
				vare = optarg;
				break;
			case 'c':
option_script:
				varc = optarg;
				break;
			case 's':
option_style:
				vars = optarg;
				break;
			case 'f':
option_file:
				varf = optarg;
				break;
			case 'm':
option_mask:
				if ((hex = strstr(optarg, "0x")) != NULL) {
					widgetr->mask = axtoi(hex + 2);
				} else {
					widgetr->mask = atoi(optarg);
				}
				break;
			case 'd':
option_tables:
				widgetr->depth = atoi(optarg);
				break;
			case 'h':
option_help:
				printf("%s usage;\n"
				       "\t-f / --file   : xml file to parse\n"
				       "\t-t / --tables : tables depth\n"
				       "\t-m / --mask   : bit mask\n",
				       argv[0]);
				exit(1);
		}
	}
	
	if (widgetr->depth == 0) {
		widgetr->depth = 4;
	}
	if (widgetr->mask == 0) {
		widgetr->mask = 0x0f;
	}
	
	for (i = 32; i > 0; i--) {
		if (widgetr->mask & (1 << (i - 1))) {
			break;
		}
	}
	printf("%s:\n"
	       "\ttable depth : %u\n"
	       "\ttable mask  : 0x%08x\n"
	       "\tmask length : %u\n",
	       argv[0], widgetr->depth, widgetr->mask, i);
	
	table_init(&widgetr->table, widgetr->mask + 1);

	if (varf != NULL) {
		xdata = (xml_data_t *) s_malloc(sizeof(xml_data_t));
		memset(xdata, 0, sizeof(xml_data_t));
		if (vars != NULL) {
			if (parse_xml_file(xdata, vars)) {
				exit(1);
			}
		}
		if (parse_xml_file(xdata, varf)) {
			exit(1);
		}
		code_parse(widgetr->table, widgetr->depth, widgetr->mask, xdata->root, xdata->elem, varc, vare);
		for (root = xdata->root; root && root->parent; root = root->parent);
		node_uninit(root);
		for (root = xdata->elem; root && root->parent; root = root->parent);
		node_uninit(root);
		s_free(xdata);
	}

	table_uninit(widgetr->table, widgetr->mask + 1);
	s_free(widgetr);
	
	return 0;
}
