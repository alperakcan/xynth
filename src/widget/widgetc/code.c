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
#include "code.h"

static char *g_name;
static FILE *g_source;

void code_generate_style (node_t *node, node_t *parent, char *prefix)
{
	node_t *shape = node_get_node(node, "shape");
	node_t *shadow = node_get_node(node, "shadow");
	fprintf(g_source, "w_%s_set_%sstyle(%s->object, %s, %s);\n",parent->type, (prefix) ? prefix : "", parent->id, (shape) ? shape->value : "0", (shadow) ? shadow->value : "0");
	if (shape) shape->dontparse = 1;
	if (shadow) shadow->dontparse = 1;
}

void code_generate_image (node_t *node, node_t *parent, char *prefix)
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

void code_generate_move (node_t *node)
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

void code_generate_window (node_t *node)
{
	node_t *tmp;
	fprintf(g_source, "w_window_init(&%s, %s, NULL);\n", node->id, node->type);
	fprintf(g_source, "s_setlocale(%s->window, 0, \"\");\n"
	                  "s_bindtextdomain(%s->window, \"%s\", \"/usr/local/share/xynth/messages/\");\n"
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
		code_generate_move(tmp);
		tmp->dontparse = 1;
	}
}

void code_generate_object_frame (node_t *node)
{
	node_t *tmp;
	node_t *window = node_get_parent(node, "window");
	fprintf(g_source, "w_frame_init(%s, &%s, %s, %s->object);\n", window->id, node->id, "0", node->parent->id);
	while ((tmp = node_get_node(node, "style")) != NULL) {
		code_generate_style(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "image")) != NULL) {
		code_generate_image(tmp, node, NULL);
		tmp->dontparse = 1;
	}
}

void code_generate_object_button (node_t *node)
{
	node_t *tmp;
	node_t *window = node_get_parent(node, "window");
	fprintf(g_source, "w_button_init(%s, &%s, %s->object);\n", window->id, node->id, node->parent->id);
	while ((tmp = node_get_node(node, "style")) != NULL) {
		code_generate_style(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "image")) != NULL) {
		code_generate_image(tmp, node, NULL);
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

void code_generate_object_textbox (node_t *node)
{
	node_t *tmp;
	fprintf(g_source, "w_textbox_init(%s, &%s, %s->object);\n", node_get_parent(node, "window")->id, node->id, node->parent->id);
	while ((tmp = node_get_node(node, "style")) != NULL) {
		code_generate_style(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "image")) != NULL) {
		code_generate_image(tmp, node, NULL);
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

void code_generate_object_editbox (node_t *node)
{
	node_t *tmp;
	fprintf(g_source, "w_editbox_init(%s, &%s, %s->object);\n", node_get_parent(node, "window")->id, node->id, node->parent->id);
	while ((tmp = node_get_node(node, "style")) != NULL) {
		code_generate_style(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "image")) != NULL) {
		code_generate_image(tmp, node, NULL);
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

void code_generate_object_checkbox (node_t *node)
{
	node_t *tmp;
	fprintf(g_source, "w_checkbox_init(%s, &%s, %s->object);\n", node_get_parent(node, "window")->id, node->id, node->parent->id);
	while ((tmp = node_get_node(node, "style")) != NULL) {
		code_generate_style(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "boxstyle")) != NULL) {
		code_generate_style(tmp, node, "box");
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
		code_generate_image(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "boximage")) != NULL) {
		code_generate_image(tmp, node, "box");
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
	while ((tmp = node_get_node(node, "state")) != NULL) {
		fprintf(g_source, "w_checkbox_state(%s->object, %s);\n", node->id, tmp->value);
		tmp->dontparse = 1;
	}
}

void code_generate_object_progressbar (node_t *node)
{
	node_t *tmp;
	fprintf(g_source, "w_progressbar_init(%s, &%s, %s->object);\n", node_get_parent(node, "window")->id, node->id, node->parent->id);
	while ((tmp = node_get_node(node, "style")) != NULL) {
		code_generate_style(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "boxstyle")) != NULL) {
		code_generate_style(tmp, node, "box");
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "image")) != NULL) {
		code_generate_image(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "boximage")) != NULL) {
		code_generate_image(tmp, node, "box");
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "changed")) != NULL) {
		fprintf(g_source, "w_progressbar_set_changed(%s->object, %s);\n", node->id, tmp->value);
		tmp->dontparse = 1;
	}
}

void code_generate_object_scrollbuffer (node_t *node)
{
	node_t *tmp;
	fprintf(g_source, "w_scrollbuffer_init(%s, &%s, %s->object);\n", node_get_parent(node, "window")->id, node->id, node->parent->id);
	while ((tmp = node_get_node(node, "object")) != NULL) {
		code_generate(tmp, g_source, g_name);
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
	while ((tmp = node_get_node(node, "border")) != NULL) {
		fprintf(g_source, "w_scrollbuffer_set_border(%s->object, %s);\n", node->id, tmp->value);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "style")) != NULL) {
		code_generate_style(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "image")) != NULL) {
		code_generate_image(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "boxstyle")) != NULL) {
		code_generate_style(tmp, node, "box");
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "boximage")) != NULL) {
		code_generate_image(tmp, node, "box");
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "addstyle")) != NULL) {
		code_generate_style(tmp, node, "add");
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "addimage")) != NULL) {
		code_generate_image(tmp, node, "add");
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "substyle")) != NULL) {
		code_generate_style(tmp, node, "sub");
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "subimage")) != NULL) {
		code_generate_image(tmp, node, "sub");
		tmp->dontparse = 1;
	}
}

void code_generate_object_listbox (node_t *node)
{
	node_t *tmp;
	fprintf(g_source, "w_listbox_init(%s, &%s, %s->object);\n", node_get_parent(node, "window")->id, node->id, node->parent->id);
	while ((tmp = node_get_node(node, "style")) != NULL) {
		code_generate_style(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "image")) != NULL) {
		code_generate_image(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "changed")) != NULL) {
		fprintf(g_source, "w_listbox_changed(%s->object, %s);\n", node->id, tmp->value);
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "add")) != NULL) {
		fprintf(g_source, "w_listbox_add(%s->object, %s);\n", node->id, tmp->value);
		tmp->dontparse = 1;
	}
	while((tmp = node_get_node(node, "scrollbarupimage")) != NULL) {
		code_generate_image(tmp,node,"scrollbarup");
		tmp->dontparse = 1;
	}
	while((tmp = node_get_node(node, "scrollbardownimage")) != NULL) {
		code_generate_image(tmp,node,"scrollbardown");
		tmp->dontparse = 1;
	}
	while((tmp = node_get_node(node, "scrollbarmiddleimage")) != NULL) {
		code_generate_image(tmp,node,"scrollbarmiddle");
		tmp->dontparse = 1;
	}
	while((tmp = node_get_node(node, "scrollbarbackgroundimage")) != NULL) {
		code_generate_image(tmp,node,"scrollbarbackground");
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "clearlist")) != NULL) {
		fprintf(g_source, "w_listbox_clearlist(%s->object);\n", node->id);
		tmp->dontparse = 1;
	}
}

void code_generate_object_ctrllistbox (node_t *node)
{
	node_t *tmp;
	fprintf(g_source, "w_ctrllistbox_init(%s, &%s, %s->object);\n", node_get_parent(node, "window")->id, node->id, node->parent->id);
	while ((tmp = node_get_node(node, "style")) != NULL) {
		code_generate_style(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "line")) != NULL) {
		code_generate_image(tmp, node, "line");
		tmp->dontparse = 1;
	}
	
	while ((tmp = node_get_node(node, "progressbar")) != NULL) {
		code_generate_image(tmp, node, "progressbar");
		tmp->dontparse = 1;
	}

	while ((tmp = node_get_node(node, "progressboxbar")) != NULL) {
		code_generate_image(tmp, node, "progressboxbar");
		tmp->dontparse = 1;
	}
		
	if ((tmp = node_get_node(node, "add")) != NULL) {
		fprintf(g_source, "w_ctrllistbox_add(%s->object, %s);\n", node->id, tmp->value);
		tmp->dontparse = 1;
	}
	while((tmp = node_get_node(node, "scrollbarupimage")) != NULL) {
		code_generate_image(tmp,node,"scrollbarup");
		tmp->dontparse = 1;
	}
	while((tmp = node_get_node(node, "scrollbardownimage")) != NULL) {
		code_generate_image(tmp,node,"scrollbardown");
		tmp->dontparse = 1;
	}
	while((tmp = node_get_node(node, "scrollbarmiddleimage")) != NULL) {
		code_generate_image(tmp,node,"scrollbarmiddle");
		tmp->dontparse = 1;
	}
	while((tmp = node_get_node(node, "scrollbarbackgroundimage")) != NULL) {
		code_generate_image(tmp,node,"scrollbarbackground");
		tmp->dontparse = 1;
	}
}

void code_generate_object_nlistbox (node_t *node)
{
	node_t *tmp;
	node_t *tnm;
	fprintf(g_source, "w_nlistbox_init(%s, &%s, %s->object);\n", node_get_parent(node, "window")->id, node->id, node->parent->id);
	while ((tmp = node_get_node(node, "style")) != NULL) {
		code_generate_style(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "changed")) != NULL) {
		fprintf(g_source, "%s->changed = %s;\n", node->id, tmp->value);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "itemheight")) != NULL) {
		fprintf(g_source, "w_nlistbox_set_item_height(%s->object, %s);\n", node->id, tmp->value);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "itemimage")) != NULL) {
		code_generate_image(tmp, node, "item");
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "item")) != NULL) {
		fprintf(g_source, "w_nlistbox_item_init(%s->object, &%s);\n", node->id, tmp->id);
		while ((tnm = node_get_node(tmp, "name")) != NULL) {
			fprintf(g_source, "w_nlistbox_item_name_set(%s->object, %s, %s);\n", node->id, tmp->id, tnm->value);
			tnm->dontparse = 1;
		}
		fprintf(g_source, "w_nlistbox_item_add(%s->object, %s);\n", node->id, tmp->id);
		tmp->dontparse = 1; 
	}
	while ((tmp = node_get_node(node, "scrollbuffer")) != NULL) {
		fprintf(g_source, "w_nlistbox_scrollbuffer_set(%s->object, %s->object);\n", node->id, tmp->value);
		tmp->dontparse = 1;
	}
}

void code_generate_object_clistbox (node_t *node)
{
	node_t *tmp;
	node_t *tnm;
	fprintf(g_source, "w_clistbox_init(%s, &%s, %s->object);\n", node_get_parent(node, "window")->id, node->id, node->parent->id);
	while ((tmp = node_get_node(node, "style")) != NULL) {
		code_generate_style(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "changed")) != NULL) {
		fprintf(g_source, "%s->changed = %s;\n", node->id, tmp->value);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "itemheight")) != NULL) {
		fprintf(g_source, "w_clistbox_set_item_height(%s->object, %s);\n", node->id, tmp->value);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "itemimage")) != NULL) {
		code_generate_image(tmp, node, "item");
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "item")) != NULL) {
		fprintf(g_source, "w_clistbox_item_init(%s->object, &%s);\n", node->id, tmp->id);
		while ((tnm = node_get_node(tmp, "name")) != NULL) {
			fprintf(g_source, "w_clistbox_item_name_set(%s->object, %s, %s);\n", node->id, tmp->id, tnm->value);
			tnm->dontparse = 1;
		}
		fprintf(g_source, "w_clistbox_item_add(%s->object, %s);\n", node->id, tmp->id);
		tmp->dontparse = 1; 
	}
	while ((tmp = node_get_node(node, "scrollbuffer")) != NULL) {
		fprintf(g_source, "w_clistbox_scrollbuffer_set(%s->object, %s->object);\n", node->id, tmp->value);
		tmp->dontparse = 1;
	}
}

void code_generate_effect (node_t *node)
{
	node_t *tmp;
	char *effect = node_get_value(node, "effect");
	fprintf(g_source, "%s->object->effect->effect = %s;\n", node->parent->id, (effect) ? effect : "0");
	if ((tmp = node_get_node(node, "effect")) != NULL) { tmp->dontparse = 1; }
}

void code_generate_show (node_t *node)
{
	if (strcmp(node->parent->name, "window") == 0) {
		fprintf(g_source, "w_object_show(%s->object);\n", node_get_parent(node, "window")->id);
		fprintf(g_source, "s_window_show(%s->window);\n", node_get_parent(node, "window")->id);
		fprintf(g_source, "s_window_main(%s->window);\n", node_get_parent(node, "window")->id);
	} else if (strcmp(node->parent->name, "object") == 0) {
		fprintf(g_source, "w_object_show(%s->object);\n", node_get_parent(node, "object")->id);
	}
}

void code_generate_draw (node_t *node)
{
	fprintf(g_source, "%s->object->draw = %s;\n", node->parent->id, node->value);
}

void code_generate_event (node_t *node)
{
	fprintf(g_source, "%s->object->event = %s;\n", node->parent->id, node->value);
}

void code_generate_onload (node_t *node)
{
	fprintf(g_source, "%s->object->onload = %s;\n", node->parent->id, node->value);
}

void code_generate_unload (node_t *node)
{
	fprintf(g_source, "%s->object->unload = %s;\n", node->parent->id, node->value);
}

void code_generate_object (node_t *node)
{
	node_t *tmp;
	if (strcmp(node->type, "frame") == 0) {
		code_generate_object_frame(node);
	} else if (strcmp(node->type, "button") == 0) {
		code_generate_object_button(node);
	} else if (strcmp(node->type, "textbox") == 0) {
		code_generate_object_textbox(node);
	} else if (strcmp(node->type, "checkbox") == 0) {
		code_generate_object_checkbox(node);
	} else if (strcmp(node->type, "editbox") == 0) {
		code_generate_object_editbox(node);
	} else if (strcmp(node->type, "progressbar") == 0) {
		code_generate_object_progressbar(node);
	} else if (strcmp(node->type, "scrollbuffer") == 0) {
		code_generate_object_scrollbuffer(node);
	} else if (strcmp(node->type, "listbox") == 0) {
		code_generate_object_listbox(node);
	} else if (strcmp(node->type, "ctrllistbox") == 0) {
		code_generate_object_ctrllistbox(node);
	} else if (strcmp(node->type, "nlistbox") == 0) {
		code_generate_object_nlistbox(node);
	} else if (strcmp(node->type, "clistbox") == 0) {
		code_generate_object_clistbox(node);
	}
	while ((tmp = node_get_node(node, "effect")) != NULL) {
		code_generate_effect(tmp);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "draw")) != NULL) {
		code_generate_draw(tmp);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "event")) != NULL) {
		code_generate_event(tmp);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "onload")) != NULL) {
		code_generate_onload(tmp);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "unload")) != NULL) {
		code_generate_unload(tmp);
		tmp->dontparse = 1;
	}
}

void code_generate (node_t *node, FILE *source, char *name)
{
	int p;
	node_t *tmp;
	if (node->dontparse != 0) {
		return;
	}
	g_name = name;
	g_source = source;
	if (strcmp(node->name, "window") == 0) {
		code_generate_window(node);
	} else if (strcmp(node->name, "object") == 0) {
		code_generate_object(node);
	} else if (strcmp(node->name, "move") == 0) {
		code_generate_move(node);
	} else if (strcmp(node->name, "show") == 0) {
		code_generate_show(node);
	}
	p = 0;
	while (!s_list_eol(node->nodes, p)) {
		tmp = (node_t *) s_list_get(node->nodes, p);
		code_generate(tmp, source, name);
		p++;
	}
}
