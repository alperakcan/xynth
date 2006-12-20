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

#include "gen_code.h"

static FILE *g_source;
static char *g_name;

void node_generate_code_style (node_t *node, node_t *parent, char *prefix)
{
	node_t *shape = node_get_node(node, "shape");
	node_t *shadow = node_get_node(node, "shadow");
	fprintf(g_source, "w_%s_set_%sstyle(%s->object, %s, %s);\n",parent->type, (prefix) ? prefix : "", parent->id, (shape) ? shape->value : "0", (shadow) ? shadow->value : "0");
	if (shape) shape->dontparse = 1;
	if (shadow) shadow->dontparse = 1;
}

void node_generate_code_image (node_t *node, node_t *parent, char *prefix)
{
	int i;
	int count;
	char *shape;
	char *shadow;
	char *rotate;
	char *cntstr;
	char *imgvar;
	node_t *tmp;
	cntstr = (char *) malloc(sizeof(char *) * 255);
	imgvar = (char *) malloc(sizeof(char *) * 255);
	sprintf(imgvar, "__imgvar__%u", rand());
	shape = node_get_value(node, "style/shape");
	shadow = node_get_value(node, "style/shadow");
	rotate = node_get_value(node, "rotate");
	count = atoi(node_get_value(node, "count"));
	fprintf(g_source, "{\n");
	fprintf(g_source, "  char *%s[%d] = {\n", imgvar, count);
	for (i = 0; i < count; i++) {
		sprintf(cntstr, "image%d", i);
		fprintf(g_source, "    \"%s\",\n", node_get_value(node, cntstr));
		tmp = node_get_node(node, cntstr);
		tmp->dontparse = 1;
	}
	fprintf(g_source, "  };\n");
	fprintf(g_source, "  w_%s_set_%simage(%s->object, %s | %s, %s, %d, %s);\n", parent->type, (prefix) ? prefix : "", parent->id, (shape) ? shape : "0", (shadow) ? shadow : "0", (rotate) ? rotate : "0", count, imgvar);
	fprintf(g_source, "}\n");
	if ((tmp = node_get_node(node, "style")) != NULL) { tmp->dontparse = 1; }
	if ((tmp = node_get_node(node, "style/shape")) != NULL) { tmp->dontparse = 1; }
	if ((tmp = node_get_node(node, "style/shadow")) != NULL) { tmp->dontparse = 1; }
	if ((tmp = node_get_node(node, "count")) != NULL) { tmp->dontparse = 1; }
	if ((tmp = node_get_node(node, "rotate")) != NULL) { tmp->dontparse = 1; }
	free(cntstr);
	free(imgvar);
}

void node_generate_code_move (node_t *node)
{
	node_t *tmp;
	char *x = node_get_value(node, "x");
	char *y = node_get_value(node, "y");
	char *w = node_get_value(node, "w");
	char *h = node_get_value(node, "h");
	if (strcmp(node->parent->name, "window") == 0) {
		fprintf(g_source, "w_window_set_coor(%s, %s, %s, %s, %s);\n", node->parent->id, (x) ? x : "0", (y) ? y : "0", (w) ? w : "w", (h) ? h : "0");
	} else if (strcmp(node->parent->name, "object") == 0) {
		fprintf(g_source, "w_object_move(%s->object, %s, %s, %s, %s);\n", node->parent->id, (x) ? x : "0", (y) ? y : "0", (w) ? w : "w", (h) ? h : "0");
	}
	node->dontparse = 1;
	if ((tmp = node_get_node(node, "x")) != NULL) { tmp->dontparse = 1; }
	if ((tmp = node_get_node(node, "y")) != NULL) { tmp->dontparse = 1; }
	if ((tmp = node_get_node(node, "w")) != NULL) { tmp->dontparse = 1; }
	if ((tmp = node_get_node(node, "h")) != NULL) { tmp->dontparse = 1; }
}

void node_generate_code_window (node_t *node)
{
	node_t *tmp;
	fprintf(g_source, "w_window_init(&%s, %s, NULL);\n", node->id, node->type);
	fprintf(g_source, "s_setlocale(%s->window, 0, \"\");\n"
	                  "s_bindtextdomain(%s->window, \"%s\", \"./lang\");\n"
	                  "s_textdomain(%s->window, \"%s\");\n", node->id, node->id, g_name, node->id, g_name);
	if ((tmp = node_get_node(node, "language")) != NULL) {
		fprintf(g_source, "s_setlocale(%s->window, 0, %s);\n", node->id, tmp->value);
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "title")) != NULL) {
		fprintf(g_source, "s_window_set_title(%s->window, \"%s\");\n", node->id, tmp->value);
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "move")) != NULL) {
		node_generate_code_move(tmp);
		tmp->dontparse = 1;
	}
}

void node_generate_code_object_frame (node_t *node)
{
	node_t *tmp;
	node_t *window = node_get_parent(node, "window");
	fprintf(g_source, "w_frame_init(%s, &%s, %s, %s->object);\n", window->id, node->id, "0", node->parent->id);
	while ((tmp = node_get_node(node, "style")) != NULL) {
		node_generate_code_style(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "image")) != NULL) {
		node_generate_code_image(tmp, node, NULL);
		tmp->dontparse = 1;
	}
}

void node_generate_code_object_button (node_t *node)
{
	node_t *tmp;
	node_t *window = node_get_parent(node, "window");
	fprintf(g_source, "w_button_init(%s, &%s, %s->object);\n", window->id, node->id, node->parent->id);
	while ((tmp = node_get_node(node, "style")) != NULL) {
		node_generate_code_style(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "image")) != NULL) {
		node_generate_code_image(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "pressed")) != NULL) {
		fprintf(g_source, "w_button_set_pressed(%s->object, %s);\n", node->id, tmp->value);
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "released")) != NULL) {
		fprintf(g_source, "w_button_set_released(%s->object, %s);\n", node->id, tmp->value);
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "clicked")) != NULL) {
		fprintf(g_source, "w_button_set_clicked(%s->object, %s);\n", node->id, tmp->value);
		tmp->dontparse = 1;
	}
}

void node_generate_code_object_textbox (node_t *node)
{
	node_t *tmp;
	fprintf(g_source, "w_textbox_init(%s, &%s, %s->object);\n", node_get_parent(node, "window")->id, node->id, node->parent->id);
	while ((tmp = node_get_node(node, "style")) != NULL) {
		node_generate_code_style(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "image")) != NULL) {
		node_generate_code_image(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "properties")) != NULL) {
		fprintf(g_source, "w_textbox_set_properties(%s->object, %s);\n", node->id, tmp->value);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "size")) != NULL) {
		fprintf(g_source, "w_textbox_set_size(%s->object, %s);\n", node->id, tmp->value);
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "color")) != NULL) {
		fprintf(g_source, "w_textbox_set_rgb(%s->object, %s, %s, %s);\n", node->id, node_get_value(tmp, "red"), node_get_value(tmp, "green"), node_get_value(tmp, "blue"));
		if ((tmp = node_get_node(node, "color/red")) != NULL) { tmp->dontparse = 1; }
		if ((tmp = node_get_node(node, "color/green")) != NULL) { tmp->dontparse = 1; }
		if ((tmp = node_get_node(node, "color/blue")) != NULL) { tmp->dontparse = 1; }
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "string")) != NULL) {
		fprintf(g_source, "w_textbox_set_str(%s->object, %s);\n", node->id, tmp->value);
		tmp->dontparse = 1;
	}
}

void node_generate_code_object_editbox (node_t *node)
{
	node_t *tmp;
	fprintf(g_source, "w_editbox_init(%s, &%s, %s->object);\n", node_get_parent(node, "window")->id, node->id, node->parent->id);
	while ((tmp = node_get_node(node, "style")) != NULL) {
		node_generate_code_style(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "image")) != NULL) {
		node_generate_code_image(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "properties")) != NULL) {
		fprintf(g_source, "w_editbox_set_properties(%s->object, %s);\n", node->id, tmp->value);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "size")) != NULL) {
		fprintf(g_source, "w_editbox_set_size(%s->object, %s);\n", node->id, tmp->value);
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "color")) != NULL) {
		fprintf(g_source, "w_editbox_set_rgb(%s->object, %s, %s, %s);\n", node->id, node_get_value(tmp, "red"), node_get_value(tmp, "green"), node_get_value(tmp, "blue"));
		if ((tmp = node_get_node(node, "color/red")) != NULL) { tmp->dontparse = 1; }
		if ((tmp = node_get_node(node, "color/green")) != NULL) { tmp->dontparse = 1; }
		if ((tmp = node_get_node(node, "color/blue")) != NULL) { tmp->dontparse = 1; }
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "string")) != NULL) {
		fprintf(g_source, "w_editbox_set_str(%s->object, %s);\n", node->id, tmp->value);
		tmp->dontparse = 1;
	}
}

void node_generate_code_object_checkbox (node_t *node)
{
	node_t *tmp;
	fprintf(g_source, "w_checkbox_init(%s, &%s, %s->object);\n", node_get_parent(node, "window")->id, node->id, node->parent->id);
	while ((tmp = node_get_node(node, "style")) != NULL) {
		node_generate_code_style(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "boxstyle")) != NULL) {
		node_generate_code_style(tmp, node, "box");
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "properties")) != NULL) {
		fprintf(g_source, "w_checkbox_set_properties(%s->object, %s);\n", node->id, tmp->value);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "size")) != NULL) {
		fprintf(g_source, "w_checkbox_set_size(%s->object, %s);\n", node->id, tmp->value);
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "color")) != NULL) {
		fprintf(g_source, "w_checkbox_set_rgb(%s->object, %s, %s, %s);\n", node->id, node_get_value(tmp, "red"), node_get_value(tmp, "green"), node_get_value(tmp, "blue"));
		if ((tmp = node_get_node(node, "color/red")) != NULL) { tmp->dontparse = 1; }
		if ((tmp = node_get_node(node, "color/green")) != NULL) { tmp->dontparse = 1; }
		if ((tmp = node_get_node(node, "color/blue")) != NULL) { tmp->dontparse = 1; }
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "image")) != NULL) {
		node_generate_code_image(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "boximage")) != NULL) {
		node_generate_code_image(tmp, node, "box");
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "changed")) != NULL) {
		fprintf(g_source, "w_checkbox_set_changed(%s->object, %s);\n", node->id, tmp->value);
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "string")) != NULL) {
		fprintf(g_source, "w_checkbox_set_str(%s->object, %s);\n", node->id, tmp->value);
		tmp->dontparse = 1;
	}
}

void node_generate_code_object_progressbar (node_t *node)
{
	node_t *tmp;
	fprintf(g_source, "w_progressbar_init(%s, &%s, %s->object);\n", node_get_parent(node, "window")->id, node->id, node->parent->id);
	while ((tmp = node_get_node(node, "style")) != NULL) {
		node_generate_code_style(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "boxstyle")) != NULL) {
		node_generate_code_style(tmp, node, "box");
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "image")) != NULL) {
		node_generate_code_image(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "boximage")) != NULL) {
		node_generate_code_image(tmp, node, "box");
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "changed")) != NULL) {
		fprintf(g_source, "w_progressbar_set_changed(%s->object, %s);\n", node->id, tmp->value);
		tmp->dontparse = 1;
	}
}

void node_generate_code_object_scrollbuffer (node_t *node)
{
	node_t *tmp;
	fprintf(g_source, "w_scrollbuffer_init(%s, &%s, %s->object);\n", node_get_parent(node, "window")->id, node->id, node->parent->id);
	while ((tmp = node_get_node(node, "object")) != NULL) {
		node_generate_code(tmp, g_source, g_name);
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "child")) != NULL) {
		fprintf(g_source, "w_scrollbuffer_set_child(%s->object, %s->object);\n", node->id, tmp->value);
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "slide")) != NULL) {
		fprintf(g_source, "w_scrollbuffer_set_slide(%s->object, %s);\n", node->id, tmp->value);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "boxstyle")) != NULL) {
		node_generate_code_style(tmp, node, "box");
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "boximage")) != NULL) {
		node_generate_code_image(tmp, node, "box");
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "addstyle")) != NULL) {
		node_generate_code_style(tmp, node, "add");
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "addimage")) != NULL) {
		node_generate_code_image(tmp, node, "add");
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "substyle")) != NULL) {
		node_generate_code_style(tmp, node, "sub");
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "subimage")) != NULL) {
		node_generate_code_image(tmp, node, "sub");
		tmp->dontparse = 1;
	}
}

void node_generate_code_effect (node_t *node)
{
	node_t *tmp;
	char *effect = node_get_value(node, "effect");
	fprintf(g_source, "%s->object->effect->effect = %s;\n", node->parent->id, (effect) ? effect : "0");
	if ((tmp = node_get_node(node, "effect")) != NULL) { tmp->dontparse = 1; }
}

void node_generate_code_show (node_t *node)
{
	if (strcmp(node->parent->name, "window") == 0) {
		fprintf(g_source, "w_object_show(%s->object);\n", node_get_parent(node, "window")->id);
		fprintf(g_source, "s_window_show(%s->window);\n", node_get_parent(node, "window")->id);
		fprintf(g_source, "s_client_main(%s->window);\n", node_get_parent(node, "window")->id);
	} else if (strcmp(node->parent->name, "object") == 0) {
		fprintf(g_source, "w_object_show(%s->object);\n", node_get_parent(node, "object")->id);
	}
}

void node_generate_code_draw (node_t *node)
{
	fprintf(g_source, "%s->object->draw = %s;\n", node->parent->id, node->value);
}

void node_generate_code_object (node_t *node)
{
	node_t *tmp;
	if (strcmp(node->type, "frame") == 0) {
		node_generate_code_object_frame(node);
	} else if (strcmp(node->type, "button") == 0) {
		node_generate_code_object_button(node);
	} else if (strcmp(node->type, "textbox") == 0) {
		node_generate_code_object_textbox(node);
	} else if (strcmp(node->type, "checkbox") == 0) {
		node_generate_code_object_checkbox(node);
	} else if (strcmp(node->type, "editbox") == 0) {
		node_generate_code_object_editbox(node);
	} else if (strcmp(node->type, "progressbar") == 0) {
		node_generate_code_object_progressbar(node);
	} else if (strcmp(node->type, "scrollbuffer") == 0) {
		node_generate_code_object_scrollbuffer(node);
	}
	if ((tmp = node_get_node(node, "effect")) != NULL) {
		node_generate_code_effect(tmp);
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "draw")) != NULL) {
		node_generate_code_draw(tmp);
		tmp->dontparse = 1;
	}
}

void node_generate_code (node_t *node, FILE *g_source, char *g_name)
{
	int p;
	node_t *tmp;
	if (node->dontparse != 0) {
		return;
	}
	if (strcmp(node->name, "window") == 0) {
		node_generate_code_window(node);
	} else if (strcmp(node->name, "object") == 0) {
		node_generate_code_object(node);
	} else if (strcmp(node->name, "move") == 0) {
		node_generate_code_move(node);
	} else if (strcmp(node->name, "show") == 0) {
		node_generate_code_show(node);
	}
	p = 0;
	while (!list_eol(node->nodes, p)) {
		tmp = (node_t *) list_get(node->nodes, p);
		node_generate_code(tmp, g_source, g_name);
		p++;
	}
}
