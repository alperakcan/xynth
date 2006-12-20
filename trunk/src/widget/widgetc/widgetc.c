/***************************************************************************
    begin                : Tue Sep 12 2006
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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <expat.h>

#include "list.h"
#include "node.h"
#include "lang.h"
#include "gen_code.h"
#include "gen_header.h"

static int localization = 0;
static int sources      = 0;

static FILE *g_input = NULL;
static FILE *g_source = NULL;
static FILE *g_header = NULL;
static char *g_name = NULL;
static char *g_source_name = NULL;
static char *g_header_name = NULL;

static node_t *g_node = NULL;
static node_t *s_node = NULL;
static int g_depth = 0;
static char *g_path = NULL;
static node_t *g_active = NULL;

static void node_generate_element (node_t *node);
static void node_generate_localization (node_t *node);
static void node_generate_sources (node_t *node);
static void start (void *xdata, const char *el, const char **attr);
static void end (void *xdata, const char *el);
static void char_hndl (void *xdata, const char *txt, int txtlen);

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

static void node_generate_element (node_t *node)
{
	int p;
	node_t *tmp;
	node_t *dmp;
	node_t *chl;
	if (s_node == NULL) {
		return;
	}
	for (p = 0; !list_eol(node->nodes, p); p++) {
    		tmp = (node_t *) list_get(node->nodes, p);
    		node_generate_element(tmp);
	}
	if (strcmp(node->name, "element") == 0 &&
	    node->type != NULL) {
	    	for (p = 0; !list_eol(s_node->nodes, p); p++) {
	    		tmp = (node_t *) list_get(s_node->nodes, p);
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

static void start (void *xdata, const char *el, const char **attr)
{
	free(g_path);
	g_path = strdup(el);
	g_depth++;
	
	{
		int p;
		node_t *node;
		node_init(&node);
		node->name = strdup(el);
		for (p = 0; attr[p]; p += 2) {
			if (strcmp(attr[p], "type") == 0) {
				node->type = strdup(attr[p + 1]);
			} else if (strcmp(attr[p], "id") == 0) {
				node->id = strdup(attr[p + 1]);
			}
		}
		node->parent = g_active;
		g_active = node;
		if (strcmp(node->name, "stylesheet") == 0) {
			s_node = node;
			if (node->parent) {
				list_add(node->parent->nodes, node, -1);
			}
		} else {
			if (node->parent) {
				list_add(node->parent->nodes, node, -1);
			} else {
				g_node = node;
			}
		}
	}
}

static void end (void *xdata, const char *el)
{
	g_depth--;
	free(g_path);
	g_path = NULL;
	g_active = g_active->parent;
}

static void char_hndl_fixup (char *out)
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

static void char_hndl (void *xdata, const char *txt, int txtlen)
{
	char *str;
	unsigned int total = 0;
	unsigned int total_old = 0;
	if (txtlen > 0 &&
	    txt &&
	    g_path) {
	} else {
	    return;
	}
	if (g_active == NULL) {
		return;
	}
	if (g_active->value != NULL) {
		total_old = strlen(g_active->value);
	}
	total = (total_old + txtlen + 1) * sizeof(char *);
	g_active->value = (char *) realloc(g_active->value, total);
	if (total_old == 0) {
		g_active->value[0] = '\0';
	}
	strncat(g_active->value, txt, txtlen); 
	str = g_active->value;
	if (g_path) {
	    	if (g_active && g_active->value) {
	    		char_hndl_fixup(g_active->value);
	    	}
	}
}

int main (int argc, char **argv)
{
	int c;
	int l = 0;
	node_t *root;
	FILE *stylesheet;
	char *tmp = NULL;
	char *buf = NULL;
	char *varo = NULL;
	char *varf = NULL;
	char *vars = NULL;
	struct stat stbuf;
	
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
	
	if (vars != NULL) {
		stat(vars, &stbuf);
		tmp = (char *) malloc(stbuf.st_size + 1);
		stylesheet = fopen(vars, "r");
		fread(tmp, 1, stbuf.st_size, stylesheet);
		l = stbuf.st_size;
		XML_Parser p = XML_ParserCreate(NULL);
		if (!p) {
			fprintf(stderr, "Couldn't allocate memory for parser\n");
			exit(-1);
		}
		XML_SetElementHandler(p, start, end);
		XML_SetCharacterDataHandler(p, char_hndl);
		if (!XML_Parse(p, tmp, l, 1)) {
			fprintf(stderr, "Parse error at line %d:\n%s\n", XML_GetCurrentLineNumber(p), XML_ErrorString(XML_GetErrorCode(p)));
			exit(-1);
		}
		XML_ParserFree(p);
		free(tmp);
		fclose(stylesheet);
	} 
	
	if (varf != NULL) {
		stat(varf, &stbuf);
		buf = (char *) malloc(stbuf.st_size + 1);
		g_input = fopen(varf, "r");
		fread(buf, 1, stbuf.st_size, g_input);
		l = stbuf.st_size;
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
	
	XML_Parser p = XML_ParserCreate(NULL);
	if (!p) {
		fprintf(stderr, "Couldn't allocate memory for parser\n");
		exit(-1);
	}
	XML_SetElementHandler(p, start, end);
	XML_SetCharacterDataHandler(p, char_hndl);
	if (!XML_Parse(p, buf, l, 1)) {
		fprintf(stderr, "Parse error at line %d:\n%s\n", XML_GetCurrentLineNumber(p), XML_ErrorString(XML_GetErrorCode(p)));
		exit(-1);
	}
	XML_ParserFree(p);

	node_generate_element(g_node);
	for (root = s_node; root && root->parent; root = root->parent);
	node_uninit(root);

	if (localization)
		node_generate_localization(g_node);
	if (sources)
		node_generate_sources(g_node);

	for (root = g_node; root && root->parent; root = root->parent);
	node_uninit(root);
	fclose(g_input);
	if (sources) {
		fclose(g_source);
		fclose(g_header);
	}
	free(g_name);
	free(g_source_name);
	free(g_header_name);
	free(buf);
	
	return 0;
}
