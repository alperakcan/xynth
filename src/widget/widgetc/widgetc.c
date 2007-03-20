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
#include "xml.h"
#include "code.h"
#include "header.h"

static FILE *g_source = NULL;
static FILE *g_header = NULL;
static char *g_name = NULL;
static char *g_source_name = NULL;
static char *g_header_name = NULL;

static void node_generate_element (s_xml_node_t *node, s_xml_node_t *s_node);
static void node_generate_sources (s_xml_node_t *node);

static void node_generate_sources (s_xml_node_t *node)
{
	fprintf(g_header,
	        "\n"
	        "#include <stdio.h>\n"
	        "#include <stdlib.h>\n"
	        "#include <time.h>\n"
	        "#include <xynth.h>\n"
	        "#include <widget.h>\n"
	        "\n");
	header_generate(node, g_header);
	fprintf(g_header, "\n");
	header_generate_function(node, g_header);
	fprintf(g_source,
	        "\n"
	        "#include \"%s\"\n"
	        "\n"
	        "int main (int argc, char *argv[])\n"
	        "{\n"
	        "srand(time(NULL));\n",
	        g_header_name);
	code_generate(node, g_source, g_name);
	fprintf(g_source,
	        "return 0;\n"
	        "}\n");
}

static void node_generate_element (s_xml_node_t *node, s_xml_node_t *style)
{
	int p;
	s_xml_node_t *tmp;
	s_xml_node_t *dmp;
	s_xml_node_t *chl;
	s_xml_node_attr_t *sid;
	s_xml_node_attr_t *stype;
	s_xml_node_attr_t *ntype;
	if (style == NULL) {
		return;
	}
	for (p = 0; !s_list_eol(node->nodes, p); p++) {
    		tmp = (s_xml_node_t *) s_list_get(node->nodes, p);
    		node_generate_element(tmp, style);
	}
	ntype = s_xml_node_get_attr(node, "type");
	if (strcmp(node->name, "element") == 0 && ntype && ntype->value != NULL) {
	    	for (p = 0; !s_list_eol(style->nodes, p); p++) {
	    		tmp = (s_xml_node_t *) s_list_get(style->nodes, p);
	    		sid = s_xml_node_get_attr(tmp, "id");
	    		stype = s_xml_node_get_attr(tmp, "type");
	    		if (strcmp(tmp->name, "element") == 0 && sid && strcmp(sid->value, ntype->value) == 0) {
	    		    	s_xml_node_dublicate(tmp, &dmp);
	    		    	free(node->name);
	    		    	free(ntype->value);
	    		    	node->name = strdup("object");
	    		    	ntype->value = (stype->value) ? strdup(stype->value) : NULL;
	    		    	while (!s_list_eol(dmp->nodes, 0)) {
	    		    		chl = (s_xml_node_t *) s_list_get(dmp->nodes, dmp->nodes->nb_elt - 1);
	    		    		chl->parent = node;
	    		    		s_list_add(node->nodes, chl, 0);
	    		    		s_list_remove(dmp->nodes, dmp->nodes->nb_elt - 1);
	    		    	}
	    		    	s_xml_node_uninit(dmp);
	    		}
	    	}
	}
}
	
int main (int argc, char **argv)
{
	int c;
	char *varo = NULL;
	char *varf = NULL;
	char *vars = NULL;
	
	while ((c = getopt(argc, argv, "s:f:o:h")) != -1) {
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
			case 'h':
			default:
usage:				printf("%s -f input_file -o output_name [-s style]\n"
				       "\t-f : input file"
				       "\t-o : output name"
				       "\t-s : style sheet\n", argv[0]);
				exit(1);
				break;
		}
	}
	
	if (varf == NULL || varo == NULL) {
		goto usage;
	}
	
	g_source_name = (char *) malloc(strlen(varo) + 20);
	g_header_name = (char *) malloc(strlen(varo) + 20);
	g_name = strdup(varo);
	sprintf(g_source_name, "%s_xml.c", varo);
	sprintf(g_header_name, "%s_xml.h", varo);
	g_source = fopen(g_source_name, "w+");
	g_header = fopen(g_header_name, "w+");

	{
		s_xml_node_t *xfile = NULL;
		s_xml_node_t *xstyle = NULL;
		if (varf == NULL ||
		    s_xml_parse_file(&xfile, varf)) {
			exit(1);
		}
		if (vars == NULL ||
		    s_xml_parse_file(&xstyle, vars)) {
//		    	exit(1);
		}
		node_generate_element(xfile, s_xml_node_get_path(xstyle, "stylesheet"));
		node_generate_sources(xfile);
		s_xml_node_uninit(xfile);
		s_xml_node_uninit(xstyle);
	}
	

	fclose(g_source);
	fclose(g_header);
	
	free(g_name);
	free(g_source_name);
	free(g_header_name);
	
	return 0;
}
