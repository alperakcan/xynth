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
#include <string.h>

#include "list.h"
#include "node.h"
#include "gen_header.h"

void node_generate_header (node_t *node, FILE *g_header)
{
	int p;
	node_t *tmp;
	if (strcmp(node->name, "window") == 0) {
		fprintf(g_header, "w_window_t *%s;\n", node->id);
	} else if (strcmp(node->name, "object") == 0) {
		if (strcmp(node->type, "frame") == 0) {
			fprintf(g_header, "w_frame_t *%s;\n", node->id);
		} else if (strcmp(node->type, "button") == 0) {
			fprintf(g_header, "w_button_t *%s;\n", node->id);
		} else if (strcmp(node->type, "textbox") == 0) {
			fprintf(g_header, "w_textbox_t *%s;\n", node->id);
		} else if (strcmp(node->type, "checkbox") == 0) {
			fprintf(g_header, "w_checkbox_t *%s;\n", node->id);
		} else if (strcmp(node->type, "editbox") == 0) {
			fprintf(g_header, "w_editbox_t *%s;\n", node->id);
		} else if (strcmp(node->type, "progressbar") == 0) {
			fprintf(g_header, "w_progressbar_t *%s;\n", node->id);
		} else if (strcmp(node->type, "scrollbuffer") == 0) {
			fprintf(g_header, "w_scrollbuffer_t *%s;\n", node->id);
		} else if (strcmp(node->type, "listbox") == 0) {
			fprintf(g_header, "w_listbox_t *%s;\n", node->id);
			p = 0;
			while (!s_list_eol(node->nodes, p)) {
				tmp = (node_t *) s_list_get(node->nodes, p);
				fprintf(g_header, "w_listbox_item_t *%s;\n", tmp->id);
				p++;
			}
		}
	}
	p = 0;
	while (!s_list_eol(node->nodes, p)) {
		tmp = (node_t *) s_list_get(node->nodes, p);
		node_generate_header(tmp, g_header);
		p++;
	}
}

void node_generate_function (node_t *node, FILE *g_header)
{
	int p;
	node_t *tmp;
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
		tmp = (node_t *) s_list_get(node->nodes, p);
		node_generate_function(tmp, g_header);
		p++;
	}
}
