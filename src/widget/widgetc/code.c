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
#include "xml.h"
#include "code.h"

static char *g_name;
static FILE *g_source;

void code_generate_style (s_xml_node_t *node, s_xml_node_t *parent, char *prefix)
{
	s_xml_node_t *shape = s_xml_node_get_path(node, "shape");
	s_xml_node_t *shadow = s_xml_node_get_path(node, "shadow");
	s_xml_node_attr_t *id = s_xml_node_get_attr(parent, "id");
	s_xml_node_attr_t *type = s_xml_node_get_attr(parent, "type");
	fprintf(g_source, "w_%s_set_%sstyle(%s->object, %s, %s);\n", type->value, (prefix) ? prefix : "", id->value, (shape) ? shape->value : "0", (shadow) ? shadow->value : "0");
	if (shape) shape->dontparse = 1;
	if (shadow) shadow->dontparse = 1;
}

void code_generate_image (s_xml_node_t *node, s_xml_node_t *parent, char *prefix)
{
	int i;
	int count;
	char *pid;
	char *ptype;
	char *shape;
	char *shadow;
	char *rotate;
	char *cntstr;
	char *imgvar;
	s_xml_node_t *tmp;
	cntstr = (char *) malloc(sizeof(char *) * 255);
	imgvar = (char *) malloc(sizeof(char *) * 255);
	sprintf(imgvar, "__imgvar__%u", rand());
	shape = s_xml_node_get_path_value(node, "style/shape");
	shadow = s_xml_node_get_path_value(node, "style/shadow");
	rotate = s_xml_node_get_path_value(node, "rotate");
	count = atoi(s_xml_node_get_path_value(node, "count"));
	pid = s_xml_node_get_attr_value(parent, "id");
	ptype = s_xml_node_get_attr_value(parent, "type");
	fprintf(g_source, "{\n");
	fprintf(g_source, "  char *%s[%d] = {\n", imgvar, count);
	for (i = 0; i < count; i++) {
		sprintf(cntstr, "image%d", i);
		fprintf(g_source, "    \"%s\",\n", s_xml_node_get_path_value(node, cntstr));
		tmp = s_xml_node_get_path(node, cntstr);
		tmp->dontparse = 1;
	}
	fprintf(g_source, "  };\n");
	fprintf(g_source, "  w_%s_set_%simage(%s->object, %s | %s, %s, %d, %s);\n", ptype, (prefix) ? prefix : "", pid, (shape) ? shape : "0", (shadow) ? shadow : "0", (rotate) ? rotate : "0", count, imgvar);
	fprintf(g_source, "}\n");
	if ((tmp = s_xml_node_get_path(node, "style")) != NULL) { tmp->dontparse = 1; }
	if ((tmp = s_xml_node_get_path(node, "style/shape")) != NULL) { tmp->dontparse = 1; }
	if ((tmp = s_xml_node_get_path(node, "style/shadow")) != NULL) { tmp->dontparse = 1; }
	if ((tmp = s_xml_node_get_path(node, "count")) != NULL) { tmp->dontparse = 1; }
	if ((tmp = s_xml_node_get_path(node, "rotate")) != NULL) { tmp->dontparse = 1; }
	free(cntstr);
	free(imgvar);
}

void code_generate_move (s_xml_node_t *node)
{
	s_xml_node_t *tmp;
	char *x = s_xml_node_get_path_value(node, "x");
	char *y = s_xml_node_get_path_value(node, "y");
	char *w = s_xml_node_get_path_value(node, "w");
	char *h = s_xml_node_get_path_value(node, "h");
	char *pid = s_xml_node_get_attr_value(node->parent, "id");
	if (strcmp(node->parent->name, "window") == 0) {
		fprintf(g_source, "w_window_set_coor(%s, %s, %s, %s, %s);\n", pid, (x) ? x : "0", (y) ? y : "0", (w) ? w : "w", (h) ? h : "0");
	} else if (strcmp(node->parent->name, "object") == 0) {
		fprintf(g_source, "w_object_move(%s->object, %s, %s, %s, %s);\n", pid, (x) ? x : "0", (y) ? y : "0", (w) ? w : "w", (h) ? h : "0");
	}
	node->dontparse = 1;
	if ((tmp = s_xml_node_get_path(node, "x")) != NULL) { tmp->dontparse = 1; }
	if ((tmp = s_xml_node_get_path(node, "y")) != NULL) { tmp->dontparse = 1; }
	if ((tmp = s_xml_node_get_path(node, "w")) != NULL) { tmp->dontparse = 1; }
	if ((tmp = s_xml_node_get_path(node, "h")) != NULL) { tmp->dontparse = 1; }
}

void code_generate_window (s_xml_node_t *node)
{
	s_xml_node_t *tmp;
	char *nid = s_xml_node_get_attr_value(node, "id");
	char *ntype = s_xml_node_get_attr_value(node, "type");
	fprintf(g_source, "w_window_init(&%s, %s, NULL);\n", nid, ntype);
	fprintf(g_source, "s_setlocale(%s->window, 0, \"\");\n"
	                  "s_bindtextdomain(%s->window, \"%s\", \"/usr/local/share/xynth/messages/\");\n"
	                  "s_textdomain(%s->window, \"%s\");\n", nid, nid, g_name, nid, g_name);
	if ((tmp = s_xml_node_get_path(node, "language")) != NULL) {
		fprintf(g_source, "s_setlocale(%s->window, 0, %s);\n", nid, tmp->value);
		tmp->dontparse = 1;
	}
	if ((tmp = s_xml_node_get_path(node, "title")) != NULL) {
		fprintf(g_source, "s_window_set_title(%s->window, \"%s\");\n", nid, tmp->value);
		tmp->dontparse = 1;
	}
	if ((tmp = s_xml_node_get_path(node, "move")) != NULL) {
		code_generate_move(tmp);
		tmp->dontparse = 1;
	}
}

void code_generate_object_frame (s_xml_node_t *node)
{
	s_xml_node_t *tmp;
	char *nid = s_xml_node_get_attr_value(node, "id");
	char *pid = s_xml_node_get_attr_value(node->parent, "id");
	char *wid = s_xml_node_get_attr_value(s_xml_node_get_parent(node, "window"), "id");
	fprintf(g_source, "w_frame_init(%s, &%s, %s, %s->object);\n", wid, nid, "0", pid);
	while ((tmp = s_xml_node_get_path(node, "style")) != NULL) {
		code_generate_style(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "image")) != NULL) {
		code_generate_image(tmp, node, NULL);
		tmp->dontparse = 1;
	}
}

void code_generate_object_button (s_xml_node_t *node)
{
	s_xml_node_t *tmp;
	char *nid = s_xml_node_get_attr_value(node, "id");
	char *pid = s_xml_node_get_attr_value(node->parent, "id");
	char *wid = s_xml_node_get_attr_value(s_xml_node_get_parent(node, "window"), "id");
	fprintf(g_source, "w_button_init(%s, &%s, %s->object);\n", wid, nid, pid);
	while ((tmp = s_xml_node_get_path(node, "style")) != NULL) {
		code_generate_style(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "image")) != NULL) {
		code_generate_image(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	if ((tmp = s_xml_node_get_path(node, "pressed")) != NULL) {
		fprintf(g_source, "w_button_set_pressed(%s->object, %s);\n", nid, tmp->value);
		tmp->dontparse = 1;
	}
	if ((tmp = s_xml_node_get_path(node, "released")) != NULL) {
		fprintf(g_source, "w_button_set_released(%s->object, %s);\n", nid, tmp->value);
		tmp->dontparse = 1;
	}
	if ((tmp = s_xml_node_get_path(node, "clicked")) != NULL) {
		fprintf(g_source, "w_button_set_clicked(%s->object, %s);\n", nid, tmp->value);
		tmp->dontparse = 1;
	}
}

void code_generate_object_textbox (s_xml_node_t *node)
{
	s_xml_node_t *tmp;
	char *nid = s_xml_node_get_attr_value(node, "id");
	char *pid = s_xml_node_get_attr_value(node->parent, "id");
	char *wid = s_xml_node_get_attr_value(s_xml_node_get_parent(node, "window"), "id");
	fprintf(g_source, "w_textbox_init(%s, &%s, %s->object);\n", wid, nid, pid);
	while ((tmp = s_xml_node_get_path(node, "style")) != NULL) {
		code_generate_style(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "image")) != NULL) {
		code_generate_image(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "properties")) != NULL) {
		fprintf(g_source, "w_textbox_set_properties(%s->object, %s);\n", nid, tmp->value);
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "size")) != NULL) {
		fprintf(g_source, "w_textbox_set_size(%s->object, %s);\n", nid, tmp->value);
		tmp->dontparse = 1;
	}
	if ((tmp = s_xml_node_get_path(node, "color")) != NULL) {
		fprintf(g_source, "w_textbox_set_rgb(%s->object, %s, %s, %s);\n", nid, s_xml_node_get_path_value(tmp, "red"), s_xml_node_get_path_value(tmp, "green"), s_xml_node_get_path_value(tmp, "blue"));
		if ((tmp = s_xml_node_get_path(node, "color/red")) != NULL) { tmp->dontparse = 1; }
		if ((tmp = s_xml_node_get_path(node, "color/green")) != NULL) { tmp->dontparse = 1; }
		if ((tmp = s_xml_node_get_path(node, "color/blue")) != NULL) { tmp->dontparse = 1; }
		tmp->dontparse = 1;
	}
	if ((tmp = s_xml_node_get_path(node, "string")) != NULL) {
		fprintf(g_source, "w_textbox_set_str(%s->object, %s);\n", nid, tmp->value);
		tmp->dontparse = 1;
	}
}

void code_generate_object_editbox (s_xml_node_t *node)
{
	s_xml_node_t *tmp;
	char *nid = s_xml_node_get_attr_value(node, "id");
	char *pid = s_xml_node_get_attr_value(node->parent, "id");
	char *wid = s_xml_node_get_attr_value(s_xml_node_get_parent(node, "window"), "id");
	fprintf(g_source, "w_editbox_init(%s, &%s, %s->object);\n", wid, nid, pid);
	while ((tmp = s_xml_node_get_path(node, "style")) != NULL) {
		code_generate_style(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "image")) != NULL) {
		code_generate_image(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "properties")) != NULL) {
		fprintf(g_source, "w_editbox_set_properties(%s->object, %s);\n", nid, tmp->value);
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "size")) != NULL) {
		fprintf(g_source, "w_editbox_set_size(%s->object, %s);\n", nid, tmp->value);
		tmp->dontparse = 1;
	}
	if ((tmp = s_xml_node_get_path(node, "color")) != NULL) {
		fprintf(g_source, "w_editbox_set_rgb(%s->object, %s, %s, %s);\n", nid, s_xml_node_get_path_value(tmp, "red"), s_xml_node_get_path_value(tmp, "green"), s_xml_node_get_path_value(tmp, "blue"));
		if ((tmp = s_xml_node_get_path(node, "color/red")) != NULL) { tmp->dontparse = 1; }
		if ((tmp = s_xml_node_get_path(node, "color/green")) != NULL) { tmp->dontparse = 1; }
		if ((tmp = s_xml_node_get_path(node, "color/blue")) != NULL) { tmp->dontparse = 1; }
		tmp->dontparse = 1;
	}
	if ((tmp = s_xml_node_get_path(node, "string")) != NULL) {
		fprintf(g_source, "w_editbox_set_str(%s->object, %s);\n", nid, tmp->value);
		tmp->dontparse = 1;
	}
}

void code_generate_object_checkbox (s_xml_node_t *node)
{
	s_xml_node_t *tmp;
	char *nid = s_xml_node_get_attr_value(node, "id");
	char *pid = s_xml_node_get_attr_value(node->parent, "id");
	char *wid = s_xml_node_get_attr_value(s_xml_node_get_parent(node, "window"), "id");
	fprintf(g_source, "w_checkbox_init(%s, &%s, %s->object);\n", wid, nid, pid);
	while ((tmp = s_xml_node_get_path(node, "style")) != NULL) {
		code_generate_style(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "boxstyle")) != NULL) {
		code_generate_style(tmp, node, "box");
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "properties")) != NULL) {
		fprintf(g_source, "w_checkbox_set_properties(%s->object, %s);\n", nid, tmp->value);
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "size")) != NULL) {
		fprintf(g_source, "w_checkbox_set_size(%s->object, %s);\n", nid, tmp->value);
		tmp->dontparse = 1;
	}
	if ((tmp = s_xml_node_get_path(node, "color")) != NULL) {
		fprintf(g_source, "w_checkbox_set_rgb(%s->object, %s, %s, %s);\n", nid, s_xml_node_get_path_value(tmp, "red"), s_xml_node_get_path_value(tmp, "green"), s_xml_node_get_path_value(tmp, "blue"));
		if ((tmp = s_xml_node_get_path(node, "color/red")) != NULL) { tmp->dontparse = 1; }
		if ((tmp = s_xml_node_get_path(node, "color/green")) != NULL) { tmp->dontparse = 1; }
		if ((tmp = s_xml_node_get_path(node, "color/blue")) != NULL) { tmp->dontparse = 1; }
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "image")) != NULL) {
		code_generate_image(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "boximage")) != NULL) {
		code_generate_image(tmp, node, "box");
		tmp->dontparse = 1;
	}
	if ((tmp = s_xml_node_get_path(node, "changed")) != NULL) {
		fprintf(g_source, "w_checkbox_set_changed(%s->object, %s);\n", nid, tmp->value);
		tmp->dontparse = 1;
	}
	if ((tmp = s_xml_node_get_path(node, "string")) != NULL) {
		fprintf(g_source, "w_checkbox_set_str(%s->object, %s);\n", nid, tmp->value);
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "state")) != NULL) {
		fprintf(g_source, "w_checkbox_state(%s->object, %s);\n", nid, tmp->value);
		tmp->dontparse = 1;
	}
}

void code_generate_object_progressbar (s_xml_node_t *node)
{
	s_xml_node_t *tmp;
	char *nid = s_xml_node_get_attr_value(node, "id");
	char *pid = s_xml_node_get_attr_value(node->parent, "id");
	char *wid = s_xml_node_get_attr_value(s_xml_node_get_parent(node, "window"), "id");
	fprintf(g_source, "w_progressbar_init(%s, &%s, %s->object);\n", wid, nid, pid);
	while ((tmp = s_xml_node_get_path(node, "style")) != NULL) {
		code_generate_style(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	if ((tmp = s_xml_node_get_path(node, "boxstyle")) != NULL) {
		code_generate_style(tmp, node, "box");
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "image")) != NULL) {
		code_generate_image(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "boximage")) != NULL) {
		code_generate_image(tmp, node, "box");
		tmp->dontparse = 1;
	}
	if ((tmp = s_xml_node_get_path(node, "changed")) != NULL) {
		fprintf(g_source, "w_progressbar_set_changed(%s->object, %s);\n", nid, tmp->value);
		tmp->dontparse = 1;
	}
}

void code_generate_object_scrollbuffer (s_xml_node_t *node)
{
	s_xml_node_t *tmp;
	char *nid = s_xml_node_get_attr_value(node, "id");
	char *pid = s_xml_node_get_attr_value(node->parent, "id");
	char *wid = s_xml_node_get_attr_value(s_xml_node_get_parent(node, "window"), "id");
	fprintf(g_source, "w_scrollbuffer_init(%s, &%s, %s->object);\n", wid, nid, pid);
	while ((tmp = s_xml_node_get_path(node, "object")) != NULL) {
		code_generate(tmp, g_source, g_name);
		tmp->dontparse = 1;
	}
	if ((tmp = s_xml_node_get_path(node, "child")) != NULL) {
		fprintf(g_source, "w_scrollbuffer_set_child(%s->object, %s->object);\n", nid, tmp->value);
		tmp->dontparse = 1;
	}
	if ((tmp = s_xml_node_get_path(node, "slide")) != NULL) {
		fprintf(g_source, "w_scrollbuffer_set_slide(%s->object, %s);\n", nid, tmp->value);
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "border")) != NULL) {
		fprintf(g_source, "w_scrollbuffer_set_border(%s->object, %s);\n", nid, tmp->value);
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "style")) != NULL) {
		code_generate_style(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "image")) != NULL) {
		code_generate_image(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "boxstyle")) != NULL) {
		code_generate_style(tmp, node, "box");
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "boximage")) != NULL) {
		code_generate_image(tmp, node, "box");
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "addstyle")) != NULL) {
		code_generate_style(tmp, node, "add");
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "addimage")) != NULL) {
		code_generate_image(tmp, node, "add");
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "substyle")) != NULL) {
		code_generate_style(tmp, node, "sub");
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "subimage")) != NULL) {
		code_generate_image(tmp, node, "sub");
		tmp->dontparse = 1;
	}
}

void code_generate_object_listbox (s_xml_node_t *node)
{
	s_xml_node_t *tmp;
	s_xml_node_t *tnm;
	char *nid = s_xml_node_get_attr_value(node, "id");
	char *pid = s_xml_node_get_attr_value(node->parent, "id");
	char *wid = s_xml_node_get_attr_value(s_xml_node_get_parent(node, "window"), "id");
	fprintf(g_source, "w_listbox_init(%s, &%s, %s->object);\n", wid, nid, pid);
	while ((tmp = s_xml_node_get_path(node, "style")) != NULL) {
		code_generate_style(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "changed")) != NULL) {
		fprintf(g_source, "%s->changed = %s;\n", nid, tmp->value);
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "itemheight")) != NULL) {
		fprintf(g_source, "w_listbox_set_item_height(%s->object, %s);\n", nid, tmp->value);
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "itemimage")) != NULL) {
		code_generate_image(tmp, node, "item");
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "item")) != NULL) {
		char *tid = s_xml_node_get_attr_value(tmp, "id");
		fprintf(g_source, "w_listbox_item_init(%s->object, &%s);\n", nid, tid);
		while ((tnm = s_xml_node_get_path(tmp, "name")) != NULL) {
			fprintf(g_source, "w_listbox_item_name_set(%s->object, %s, %s);\n", nid, tid, tnm->value);
			tnm->dontparse = 1;
		}
		fprintf(g_source, "w_listbox_item_add(%s->object, %s);\n", nid, tid);
		tmp->dontparse = 1; 
	}
	while ((tmp = s_xml_node_get_path(node, "scrollbuffer")) != NULL) {
		fprintf(g_source, "w_listbox_scrollbuffer_set(%s->object, %s->object);\n", nid, tmp->value);
		tmp->dontparse = 1;
	}
}

void code_generate_object_clistbox (s_xml_node_t *node)
{
	s_xml_node_t *tmp;
	s_xml_node_t *tnm;
	char *nid = s_xml_node_get_attr_value(node, "id");
	char *pid = s_xml_node_get_attr_value(node->parent, "id");
	char *wid = s_xml_node_get_attr_value(s_xml_node_get_parent(node, "window"), "id");
	fprintf(g_source, "w_clistbox_init(%s, &%s, %s->object);\n", wid, nid, pid);
	while ((tmp = s_xml_node_get_path(node, "style")) != NULL) {
		code_generate_style(tmp, node, NULL);
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "changed")) != NULL) {
		fprintf(g_source, "%s->changed = %s;\n", nid, tmp->value);
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "itemheight")) != NULL) {
		fprintf(g_source, "w_clistbox_set_item_height(%s->object, %s);\n", nid, tmp->value);
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "itemimage")) != NULL) {
		code_generate_image(tmp, node, "item");
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "item")) != NULL) {
		char *tid = s_xml_node_get_attr_value(tmp, "id");
		fprintf(g_source, "w_clistbox_item_init(%s->object, &%s);\n", nid, tid);
		while ((tnm = s_xml_node_get_path(tmp, "name")) != NULL) {
			fprintf(g_source, "w_clistbox_item_name_set(%s->object, %s, %s);\n", nid, tid, tnm->value);
			tnm->dontparse = 1;
		}
		fprintf(g_source, "w_clistbox_item_add(%s->object, %s);\n", nid, tid);
		tmp->dontparse = 1; 
	}
	while ((tmp = s_xml_node_get_path(node, "scrollbuffer")) != NULL) {
		fprintf(g_source, "w_clistbox_scrollbuffer_set(%s->object, %s->object);\n", nid, tmp->value);
		tmp->dontparse = 1;
	}
}

void code_generate_effect (s_xml_node_t *node)
{
	s_xml_node_t *tmp;
	char *effect = s_xml_node_get_path_value(node, "effect");
	char *pid = s_xml_node_get_attr_value(node->parent, "id");
	fprintf(g_source, "%s->object->effect->effect = %s;\n", pid, (effect) ? effect : "0");
	if ((tmp = s_xml_node_get_path(node, "effect")) != NULL) { tmp->dontparse = 1; }
}

void code_generate_show (s_xml_node_t *node)
{
	if (strcmp(node->parent->name, "window") == 0) {
		char *pid = s_xml_node_get_attr_value(s_xml_node_get_parent(node, "window"), "id");
		fprintf(g_source, "w_object_show(%s->object);\n", pid);
		fprintf(g_source, "s_window_show(%s->window);\n", pid);
		fprintf(g_source, "s_window_main(%s->window);\n", pid);
	} else if (strcmp(node->parent->name, "object") == 0) {
		char *oid = s_xml_node_get_attr_value(s_xml_node_get_parent(node, "object"), "id");
		fprintf(g_source, "w_object_show(%s->object);\n", oid);
	}
}

void code_generate_draw (s_xml_node_t *node)
{
	char *pid = s_xml_node_get_attr_value(node->parent, "id");
	fprintf(g_source, "%s->object->draw = %s;\n", pid, node->value);
}

void code_generate_event (s_xml_node_t *node)
{
	char *pid = s_xml_node_get_attr_value(node->parent, "id");
	fprintf(g_source, "%s->object->event = %s;\n", pid, node->value);
}

void code_generate_onload (s_xml_node_t *node)
{
	char *pid = s_xml_node_get_attr_value(node->parent, "id");
	fprintf(g_source, "%s->object->onload = %s;\n", pid, node->value);
}

void code_generate_unload (s_xml_node_t *node)
{
	char *pid = s_xml_node_get_attr_value(node->parent, "id");
	fprintf(g_source, "%s->object->unload = %s;\n", pid, node->value);
}

void code_generate_object (s_xml_node_t *node)
{
	s_xml_node_t *tmp;
	char *type = s_xml_node_get_attr_value(node, "type");
	if (strcmp(type, "frame") == 0) {
		code_generate_object_frame(node);
	} else if (strcmp(type, "button") == 0) {
		code_generate_object_button(node);
	} else if (strcmp(type, "textbox") == 0) {
		code_generate_object_textbox(node);
	} else if (strcmp(type, "checkbox") == 0) {
		code_generate_object_checkbox(node);
	} else if (strcmp(type, "editbox") == 0) {
		code_generate_object_editbox(node);
	} else if (strcmp(type, "progressbar") == 0) {
		code_generate_object_progressbar(node);
	} else if (strcmp(type, "scrollbuffer") == 0) {
		code_generate_object_scrollbuffer(node);
	} else if (strcmp(type, "listbox") == 0) {
		code_generate_object_listbox(node);
	} else if (strcmp(type, "clistbox") == 0) {
		code_generate_object_clistbox(node);
	}
	while ((tmp = s_xml_node_get_path(node, "effect")) != NULL) {
		code_generate_effect(tmp);
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "draw")) != NULL) {
		code_generate_draw(tmp);
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "event")) != NULL) {
		code_generate_event(tmp);
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "onload")) != NULL) {
		code_generate_onload(tmp);
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "unload")) != NULL) {
		code_generate_unload(tmp);
		tmp->dontparse = 1;
	}
}

void code_generate (s_xml_node_t *node, FILE *source, char *name)
{
	int p;
	s_xml_node_t *tmp;
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
		tmp = (s_xml_node_t *) s_list_get(node->nodes, p);
		code_generate(tmp, source, name);
		p++;
	}
}
