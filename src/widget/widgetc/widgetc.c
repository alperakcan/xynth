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

static void node_generate_code_style (node_t *node, node_t *parent, char *prefix);
static void node_generate_code_image (node_t *node, node_t *parent, char *prefix);
static void node_generate_code_move (node_t *node);
static void node_generate_code_window (node_t *node);
static void node_generate_code_object_frame (node_t *node);
static void node_generate_code_object_button (node_t *node);
static void node_generate_code_object_textbox (node_t *node);
static void node_generate_code_object_editbox (node_t *node);
static void node_generate_code_object_checkbox (node_t *node);
static void node_generate_code_object_progressbar (node_t *node);
static void node_generate_code_object_scrollbuffer (node_t *node);
static void node_generate_code_effect (node_t *node);
static void node_generate_code_show (node_t *node);
static void node_generate_code_draw (node_t *node);
static void node_generate_code_object (node_t *node);
static void node_generate_code (node_t *node);
static void node_generate_header (node_t *node);
static void node_generate_function (node_t *node);
static void node_generate_element (node_t *node);
static void node_generate_localization (node_t *node);
static void node_generate_sources (node_t *node);
static void start (void *xdata, const char *el, const char **attr);
static void end (void *xdata, const char *el);
static void char_hndl (void *xdata, const char *txt, int txtlen);

static void node_generate_code_style (node_t *node, node_t *parent, char *prefix)
{
	node_t *shape = node_get_node(node, "shape");
	node_t *shadow = node_get_node(node, "shadow");
	fprintf(g_source, "w_%s_set_%sstyle(%s->object, %s, %s);\n",parent->type, (prefix) ? prefix : "", parent->id, (shape) ? shape->value : "0", (shadow) ? shadow->value : "0");
	if (shape) shape->dontparse = 1;
	if (shadow) shadow->dontparse = 1;
}

static void node_generate_code_image (node_t *node, node_t *parent, char *prefix)
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

static void node_generate_code_move (node_t *node)
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

static void node_generate_code_window (node_t *node)
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

static void node_generate_code_object_frame (node_t *node)
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

static void node_generate_code_object_button (node_t *node)
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

static void node_generate_code_object_textbox (node_t *node)
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

static void node_generate_code_object_editbox (node_t *node)
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

static void node_generate_code_object_checkbox (node_t *node)
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

static void node_generate_code_object_progressbar (node_t *node)
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

static void node_generate_code_object_scrollbuffer (node_t *node)
{
	node_t *tmp;
	fprintf(g_source, "w_scrollbuffer_init(%s, &%s, %s->object);\n", node_get_parent(node, "window")->id, node->id, node->parent->id);
	while ((tmp = node_get_node(node, "object")) != NULL) {
		node_generate_code(tmp);
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

static void node_generate_code_effect (node_t *node)
{
	node_t *tmp;
	char *effect = node_get_value(node, "effect");
	fprintf(g_source, "%s->object->effect->effect = %s;\n", node->parent->id, (effect) ? effect : "0");
	if ((tmp = node_get_node(node, "effect")) != NULL) { tmp->dontparse = 1; }
}

static void node_generate_code_show (node_t *node)
{
	if (strcmp(node->parent->name, "window") == 0) {
		fprintf(g_source, "w_object_show(%s->object);\n", node_get_parent(node, "window")->id);
		fprintf(g_source, "s_window_show(%s->window);\n", node_get_parent(node, "window")->id);
		fprintf(g_source, "s_client_main(%s->window);\n", node_get_parent(node, "window")->id);
	} else if (strcmp(node->parent->name, "object") == 0) {
		fprintf(g_source, "w_object_show(%s->object);\n", node_get_parent(node, "object")->id);
	}
}

static void node_generate_code_draw (node_t *node)
{
	fprintf(g_source, "%s->object->draw = %s;\n", node->parent->id, node->value);
}

static void node_generate_code_object (node_t *node)
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

static void node_generate_code (node_t *node)
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
	g_depth++;
	p = 0;
	while (!list_eol(node->nodes, p)) {
		tmp = (node_t *) list_get(node->nodes, p);
		node_generate_code(tmp);
		p++;
	}
	g_depth--;
}

static void node_generate_header (node_t *node)
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
		}
	}
	g_depth++;
	p = 0;
	while (!list_eol(node->nodes, p)) {
		tmp = (node_t *) list_get(node->nodes, p);
		node_generate_header(tmp);
		p++;
	}
	g_depth--;
}

static void node_generate_function (node_t *node)
{
	int p;
	node_t *tmp;
	if (strcmp(node->name, "draw") == 0) {
		fprintf(g_header, "void %s (w_object_t *object);\n", node->value);
	} else if (strcmp(node->name, "pressed") == 0) {
		fprintf(g_header, "void %s (w_object_t *object, int button);\n", node->value);
	} else if (strcmp(node->name, "released") == 0) {
		fprintf(g_header, "void %s (w_object_t *object, int button);\n", node->value);
	} else if (strcmp(node->name, "changed") == 0) {
		fprintf(g_header, "void %s (w_object_t *object, int state);\n", node->value);
	}
	g_depth++;
	p = 0;
	while (!list_eol(node->nodes, p)) {
		tmp = (node_t *) list_get(node->nodes, p);
		node_generate_function(tmp);
		p++;
	}
	g_depth--;
}

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
	node_generate_header(node);
	fprintf(g_header, "\n");
	node_generate_function(node);
	fprintf(g_source,
	        "\n"
	        "#include \"%s\"\n"
	        "\n"
	        "int main (int argc, char *argv[])\n"
	        "{\n"
	        "srand(time(NULL));\n",
	        g_header_name);
	node_generate_code(node);
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
