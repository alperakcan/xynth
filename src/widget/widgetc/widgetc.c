/***************************************************************************
    begin                : Tue Sep 12 2006
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
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "list.h"
#include "node.h"
#include "lang.h"
#include "gen_code.h"
#include "gen_header.h"
#include "parse.h"

static FILE *g_source = NULL;
static FILE *g_header = NULL;
static char *g_name = NULL;
static char *g_source_name = NULL;
static char *g_header_name = NULL;

static void node_generate_element (node_t *node, node_t *s_node);
static void node_generate_localization (node_t *node);
static void node_generate_sources (node_t *node);

static void node_generate_localization (node_t *node)
{
	node_t *lang;
	node_t *local;
	while ((local = node_get_node(node, "localization")) != NULL) {
		while ((lang = node_get_node(local, "language")) != NULL) {
			lang_generate(lang, g_name);
			lang->dontparse = 1;
		}
		local->dontparse = 1;
	}
}

static void node_generate_sources (node_t *node)
{
	fprintf(g_header,
	        "\n"
	        "#include <stdio.h>\n"
	        "#include <stdlib.h>\n"
	        "#include <time.h>\n"
	        "#include <xynth.h>\n"
	        "#include <widget.h>\n"
	        "\n");
	node_generate_header(node, g_header);
	fprintf(g_header, "\n");
	node_generate_function(node, g_header);
	fprintf(g_source,
	        "\n"
	        "#include \"%s\"\n"
	        "\n"
	        "int main (int argc, char *argv[])\n"
	        "{\n"
	        "srand(time(NULL));\n",
	        g_header_name);
	node_generate_code(node, g_source, g_name);
	fprintf(g_source,
	        "return 0;\n"
	        "}\n");
}

static void node_generate_element (node_t *node, node_t *elem)
{
	int p;
	node_t *tmp;
	node_t *dmp;
	node_t *chl;
	if (elem == NULL) {
		return;
	}
	for (p = 0; !list_eol(node->nodes, p); p++) {
    		tmp = (node_t *) list_get(node->nodes, p);
    		node_generate_element(tmp, elem);
	}
	if (strcmp(node->name, "element") == 0 &&
	    node->type != NULL) {
	    	for (p = 0; !list_eol(elem->nodes, p); p++) {
	    		tmp = (node_t *) list_get(elem->nodes, p);
	    		if (strcmp(tmp->name, "element") == 0 &&
	    		    strcmp(tmp->id, node->type) == 0) {
	    		    	node_dublicate(tmp, &dmp);
	    		    	free(node->name);
	    		    	free(node->type);
	    		    	node->name = node_strdup("object");
	    		    	node->type = node_strdup(dmp->type);
	    		    	while (!list_eol(dmp->nodes, 0)) {
	    		    		chl = (node_t *) list_get(dmp->nodes, dmp->nodes->nb_elt - 1);
	    		    		chl->parent = node;
	    		    		list_add(node->nodes, chl, 0);
	    		    		list_remove(dmp->nodes, dmp->nodes->nb_elt - 1);
	    		    	}
	    		    	node_uninit(dmp);
	    		}
	    	}
	}
}
	
int main (int argc, char **argv)
{
	int c;
	node_t *root;
	char *varo = NULL;
	char *varf = NULL;
	char *vars = NULL;
	xml_data_t *xdata;
	int localization = 0;
	int sources      = 0;
	
	while ((c = getopt(argc, argv, "s:f:o:clh")) != -1) {
		switch (c) {
			case 'f':
				varf = optarg;
				break;
			case 'o':
				varo = optarg;
				break;
			case 's':
				vars = optarg;
				break;
			case 'l':
				localization = 1;
				break;
			case 'c':
				sources = 1;
				break;
usage:			case 'h':
			default:
				printf("%s -f input_file -o output_name -c -l\n"
				       "\t-f : input file"
				       "\t-o : output name"
				       "\t-c : output source file"
				       "\t-l : output localization file", argv[0]);
				exit(1);
				break;
		}
	}
	
	if (sources == 0 && localization == 0) {
		goto usage;
	}
	
	xdata = (xml_data_t *) malloc(sizeof(xml_data_t));
	memset(xdata, 0, sizeof(xml_data_t));

	if (vars != NULL) {
		if (parse_xml_file(xdata, vars)) {
			exit(1);
		}
	} 
	
	if (varf != NULL) {
		if (parse_xml_file(xdata, varf)) {
			exit(1);
		}
	} else {
		goto usage;
	}

	if (varo != NULL) {
		g_source_name = (char *) malloc(strlen(varo) + 20);
		g_header_name = (char *) malloc(strlen(varo) + 20);
		g_name = strdup(varo);
		if (sources == 1) {
			sprintf(g_source_name, "%s_xml.c", varo);
			sprintf(g_header_name, "%s_xml.h", varo);
			g_source = fopen(g_source_name, "w+");
			g_header = fopen(g_header_name, "w+");
		}
	} else {
		goto usage;
	}
	
	node_generate_element(xdata->root, xdata->elem);

	if (localization)
		node_generate_localization(xdata->root);
	if (sources)
		node_generate_sources(xdata->root);

	for (root = xdata->root; root && root->parent; root = root->parent);
	node_uninit(root);
	for (root = xdata->elem; root && root->parent; root = root->parent);
	node_uninit(root);

	if (sources) {
		fclose(g_source);
		fclose(g_header);
	}
	
	free(xdata);
	free(g_name);
	free(g_source_name);
	free(g_header_name);
	
	return 0;
}
