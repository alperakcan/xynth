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

#include "xynth_.h"
#include "widget.h"
#include <getopt.h>

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
	unsigned int i;
	char *varf = NULL;
	char *vars = NULL;
	char *varc = NULL;
	char *vare = NULL;
	unsigned int mask = 0;
	unsigned int depth = 0;
	s_hashtable_t *htable;
	int option_index = 0;
	struct option long_options[] = {
		{"depth", 1, 0, 'd'},
		{"mask", 1, 0, 'm'},
		{"file", 1, 0, 'f'},
		{"style", 1, 0, 's'},
		{"script", 1, 0, 'c'},
		{"engine", 1, 0, 'e'},
		{"help", 0, 0, 'h'},
		{0, 0, 0, 0},
	};

	while ((c = getopt_long(argc, argv, "e:c:s:f:d:m:h", long_options, &option_index)) != -1) {
		switch (c) {
			case 'e':
				vare = optarg;
				break;
			case 'c':
				varc = optarg;
				break;
			case 's':
				vars = optarg;
				break;
			case 'f':
				varf = optarg;
				break;
			case 'm':
				if ((hex = strstr(optarg, "0x")) != NULL) {
					mask = axtoi(hex + 2);
				} else {
					mask = atoi(optarg);
				}
				break;
			case 'd':
				depth = atoi(optarg);
				break;
			case 'h':
usage:				printf("%s usage;\n"
				       "\t-f / --file   : xml file to use\n"
				       "\t-s / --style  : style sheet xml file to use\n"
				       "\t-c / --script : script file to use\n"
				       "\t-e / --engine : script engine to use\n"
				       "\t-d / --depth  : tables depth\n"
				       "\t-m / --mask   : bit mask\n",
				       argv[0]);
				exit(1);
		}
	}
	if (varf == NULL) {
		goto usage;
	}
	
	if (depth == 0) {
		depth = 4;
	}
	if (mask == 0) {
		mask = 0x0f;
	}
	
	for (i = 32; i > 0; i--) {
		if (mask & (1 << (i - 1))) {
			break;
		}
	}
	printf("%s:\n"
	       "\ttable depth : %u\n"
	       "\ttable mask  : 0x%08x\n"
	       "\tmask length : %u\n"
	       "\tfile        : %s\n"
	       "\tstyle       : %s\n"
	       "\tscript      : %s\n"
	       "\tengine      : %s\n",
	       argv[0], depth, mask, i, varf, vars, varc, vare);
	
	s_hashtable_init(&htable, mask, depth);

	{
		s_xml_node_t *xfile = NULL;
		s_xml_node_t *xstyle = NULL;
		if (varf == NULL ||
		    s_xml_parse_file(&xfile, varf)) {
			exit(1);
		}
		if (vars != NULL &&
		    s_xml_parse_file(&xstyle, vars)) {
		    	exit(1);
		}
		code_parse(htable, xfile, s_xml_node_get_path(xstyle, "stylesheet"), varc, vare);
		s_xml_node_uninit(xfile);
		s_xml_node_uninit(xstyle);
	}

	s_hashtable_uninit(htable);
	
	return 0;
}
