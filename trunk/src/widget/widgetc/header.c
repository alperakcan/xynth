/***************************************************************************
    begin                : Wed Dec 20 2006
    copyright            : (C) 2006 - 2008 by Alper Akcan
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
#include "xml.h"
#include "header.h"

void header_generate (s_xml_node_t *node, FILE *g_header)
{
	int p;
	char *nid;
	char *ntype;
	s_xml_node_t *tmp;
	nid = s_xml_node_get_attr_value(node, "id");
	ntype = s_xml_node_get_attr_value(node, "type");
	if (strcmp(node->name, "window") == 0) {
		fprintf(g_header, "w_window_t *%s;\n", nid);
	} else if (strcmp(node->name, "object") == 0) {
		if (strcmp(ntype, "frame") == 0) {
			fprintf(g_header, "w_frame_t *%s;\n", nid);
		} else if (strcmp(ntype, "button") == 0) {
			fprintf(g_header, "w_button_t *%s;\n", nid);
		} else if (strcmp(ntype, "textbox") == 0) {
			fprintf(g_header, "w_textbox_t *%s;\n", nid);
		} else if (strcmp(ntype, "checkbox") == 0) {
			fprintf(g_header, "w_checkbox_t *%s;\n", nid);
		} else if (strcmp(ntype, "editbox") == 0) {
			fprintf(g_header, "w_editbox_t *%s;\n", nid);
		} else if (strcmp(ntype, "progressbar") == 0) {
			fprintf(g_header, "w_progressbar_t *%s;\n", nid);
		} else if (strcmp(ntype, "scrollbuffer") == 0) {
			fprintf(g_header, "w_scrollbuffer_t *%s;\n", nid);
		} else if (strcmp(ntype, "scrollbar") == 0) {
			fprintf(g_header, "w_scrollbar_t *%s;\n", nid);
		} else if (strcmp(ntype, "combobox") == 0) {
			fprintf(g_header, "w_combobox_t *%s;\n", nid);
		} else if (strcmp(ntype, "listbox") == 0) {
			fprintf(g_header, "w_listbox_t *%s;\n", nid);
		} else if (strcmp(ntype, "clistbox") == 0) {
			fprintf(g_header, "w_clistbox_t *%s;\n", nid);
		}
	}
	p = 0;
	while (!s_list_eol(node->nodes, p)) {
		tmp = (s_xml_node_t *) s_list_get(node->nodes, p);
		header_generate(tmp, g_header);
		p++;
	}
}

void header_generate_function (s_xml_node_t *node, FILE *g_header)
{
	int p;
	s_xml_node_t *tmp;
	if (strcmp(node->name, "draw") == 0) {
		fprintf(g_header, "void %s (w_object_t *object);\n", node->value);
	} else if (strcmp(node->name, "event") == 0) {
		fprintf(g_header, "void %s (w_object_t *object, s_event_t *event);\n", node->value);
	} else if (strcmp(node->name, "onload") == 0) {
		fprintf(g_header, "void %s (w_object_t *object);\n", node->value);
	} else if (strcmp(node->name, "unload") == 0) {
		fprintf(g_header, "void %s (w_object_t *object);\n", node->value);
	} else if (strcmp(node->name, "pressed") == 0) {
		fprintf(g_header, "void %s (w_object_t *object, int button);\n", node->value);
	} else if (strcmp(node->name, "released") == 0) {
		fprintf(g_header, "void %s (w_object_t *object, int button);\n", node->value);
	} else if (strcmp(node->name, "changed") == 0) {
		fprintf(g_header, "void %s (w_object_t *object, int state);\n", node->value);
	}
	p = 0;
	while (!s_list_eol(node->nodes, p)) {
		tmp = (s_xml_node_t *) s_list_get(node->nodes, p);
		header_generate_function(tmp, g_header);
		p++;
	}
}
