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

typedef struct list_s list_t;
typedef struct list_node_s list_node_t;

struct list_node_s {
        void *next;
        void *element;
};

struct list_s {
        int nb_elt;
        list_node_t *node;
};

typedef struct node_s {
	char *name;
	char *id;
	char *type;
	char *value;
	list_t *nodes;
	struct node_s *parent;
	int dontparse;
} node_t;

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

static int list_init (list_t **li)
{
	(*li) = (list_t *) malloc(sizeof(list_t));
	if ((*li) == NULL) {
		return -1;
	}
	(*li)->nb_elt = 0;
	return 0;
}

static int list_uninit (list_t *li)
{
	free(li);
	return 0;
}

static int list_eol (list_t *li, int i)
{
        if (li == NULL) {
		return 1;
	}

	if ((i >= 0) && (i < li->nb_elt)) {
		return 0;
	}
	/* end of list */
	return 1;
}

static void * list_get (list_t *li, int pos)
{
	int i = 0;
	list_node_t *ntmp;
	if ((li == NULL) || (pos < 0) || (pos >= li->nb_elt)) {
		/* element does not exist */
		return NULL;
	}
	/* exist because nb_elt > 0 */
	ntmp = li->node;
	while (pos > i) {
		i++;
		ntmp = (list_node_t *) ntmp->next;
	}
	return ntmp->element;
}

static int list_remove (list_t *li, int pos)
{
	int i = 0;
	list_node_t *ntmp;
	if ((li == NULL) || (pos < 0) || (pos >= li->nb_elt)) {
		/* element does not exist */
		return -1;
	}
	/* exist because nb_elt > 0 */
	ntmp = li->node;
	if ((pos == 0)) {
		/* special case  */
		li->node = (list_node_t *) ntmp->next;
		li->nb_elt--;
		free(ntmp);
		return li->nb_elt;
	}
	while (pos > (i + 1)) {
		i++;
		ntmp = (list_node_t *) ntmp->next;
	}
	/* insert new node */
	{
		list_node_t *remnode;
		remnode = (list_node_t *) ntmp->next;
		ntmp->next = ((list_node_t *) ntmp->next)->next;
		free(remnode);
		li->nb_elt--;
	}
	return li->nb_elt;
}

static int list_add (list_t *li, void *el, int pos)
{
	int i = 0;
	list_node_t *ntmp;
	if (li == NULL) {
		return -1;
	}
	if ((pos == -1) || (pos >= li->nb_elt)) {
		/* insert at the end  */
		pos = li->nb_elt;
	}
	if (li->nb_elt == 0) {
		li->node = (list_node_t *) malloc(sizeof(list_node_t));
		li->node->element = el;
		li->nb_elt++;
		return li->nb_elt;
	}
	/* exist because nb_elt > 0  */
	ntmp = li->node;
	if (pos == 0) {
		li->node = (list_node_t *) malloc(sizeof(list_node_t));
		li->node->element = el;
		li->node->next = ntmp;
		li->nb_elt++;
		return li->nb_elt;
	}
	/* pos = 0 insert before first elt  */
	while (pos > (i + 1)) {
		i++;
		/* when pos > i next node exist  */
		ntmp = (list_node_t *) ntmp->next;
	}
	/* if pos == nb_elt next node does not exist  */
	if (pos == li->nb_elt) {
		ntmp->next = (list_node_t *) malloc(sizeof(list_node_t));
		ntmp = (list_node_t *) ntmp->next;
		ntmp->element = el;
		li->nb_elt++;
		return li->nb_elt;
	}
	/* here pos == i so next node is where we want to insert new node */
	{
		list_node_t *nextnode = (list_node_t *) ntmp->next;
		ntmp->next = (list_node_t *) malloc(sizeof(list_node_t));
		ntmp = (list_node_t *) ntmp->next;
		ntmp->element = el;
		ntmp->next = nextnode;
		li->nb_elt++;
	}
	return li->nb_elt;
}

static char * node_strdup (char *str)
{
	if (str == NULL) {
		return NULL;
	} else {
		return strdup(str);
	}
}

static int node_path_normalize (char *out, int len)
{
	int i;
	int j;
	int first;
	int next;
	
	/* First append "/" to make the rest easier */
	if (!strncat(out,"/",len))
		return -10;

	/* Convert "//" to "/" */
	for (i = 0; out[i + 1]; i++) {
		if (out[i] == '/' && out[i + 1] == '/') {
			for (j = i + 1; out[j]; j++)
				out[j] = out[j + 1];
			i--;
		}
	}

	/* Convert "/./" to "/" */
	for (i = 0; out[i] && out[i + 1] && out[i + 2]; i++) {
		if (out[i] == '/' && out[i + 1] == '.' && out[i + 2] == '/') {
			for (j = i + 1; out[j]; j++)
				out[j] = out[j + 2];
			i--;
		}
	}

	/* Convert "/asdf/../" to "/" until we can't anymore.  Also
	 * convert leading "/../" to "/" */
	first = next = 0;
	while (1) {
		/* If a "../" follows, remove it and the parent */
		if (out[next + 1] && out[next + 1] == '.' && 
		    out[next + 2] && out[next + 2] == '.' &&
		    out[next + 3] && out[next + 3] == '/') {
			for (j = 0; out[first + j + 1]; j++)
				out[first + j + 1] = out[next + j + 4];
			first = next = 0;
			continue;
		}

		/* Find next slash */
		first = next;
		for (next = first + 1; out[next] && out[next] != '/'; next++)
			continue;
		if (!out[next])
			break;
	}
	
	/* Remove trailing "/" */
	for (i = 1; out[i]; i++)
		continue;
	if (i >= 1 && out[i - 1] == '/') 
		out[i - 1] = 0;

	return 0;
}

static void node_init (node_t **node)
{
	node_t *n;
	n = (node_t *) malloc(sizeof(node_t));
	memset(n, 0, sizeof(node_t));
	list_init(&(n->nodes));
	*node = n;
}

static void node_uninit (node_t *node)
{
	node_t *tmp;
	if (node == NULL) {
		return;
	}
	while (!list_eol(node->nodes, 0)) {
		tmp = (node_t *) list_get(node->nodes, 0);
		list_remove(node->nodes, 0);
		node_uninit(tmp);
	}
	list_uninit(node->nodes);
	free(node->id);
	free(node->name);
	free(node->type);
	free(node->value);
	free(node);
}

static void node_dublicate_ (node_t *node, node_t *dub)
{
	int p;
	node_t *tmp;
	node_t *dmp;
	dub->id = node_strdup(node->id);
	dub->name = node_strdup(node->name);
	dub->type = node_strdup(node->type);
	dub->value = node_strdup(node->value);
	for (p = 0; !list_eol(node->nodes, p); p++) {
    		tmp = (node_t *) list_get(node->nodes, p);
    		node_init(&dmp);
    		node_dublicate_(tmp, dmp);
    		list_add(dub->nodes, dmp, -1);
    		dmp->parent = dub;
	}
}

static void node_dublicate (node_t *node, node_t **dub)
{
	node_init(dub);
	node_dublicate_(node, *dub);
}

static node_t * node_get_parent (node_t *node, char *name)
{
	while (node->parent) {
		if (strcmp(node->parent->name, name) == 0) {
			return node->parent;
		}
		node = node->parent;
	}
	return NULL;
}

static node_t * node_get_node_ (node_t *node, char *path)
{
	int p;
	char *ptr;
	char *str;
	node_t *res;
	node_t *tmp;
	p = 0;
	res = NULL;
	while (!list_eol(node->nodes, p)) {
		tmp = (node_t *) list_get(node->nodes, p);
		str = strchr(path,  '/');
		if (str == NULL) {
			if (strcmp(tmp->name, path) == 0 &&
			    tmp->dontparse != 1) {
				res = tmp;
				break;
			}
		} else {
			if (strncmp(tmp->name, path, str - path) == 0 &&
			    strlen(tmp->name) == str - path) {
			    	ptr = strchr(path, '/');
			    	if (ptr != NULL) {
			    		ptr++;
			    	}
			    	if ((res = node_get_node_(tmp, ptr)) != NULL) {
			    		break;
			    	}
			}
		}
		p++;
	}
	return res;
}

static node_t * node_get_node (node_t *node, char *path)
{
	int len;
	char *str;
	node_t *res;
	len = strlen(path);
	str = (char *) malloc(len + 10 + 1);
	memset(str, 0, len + 10 + 1);
	memcpy(str, path, len);
	node_path_normalize(str, len + 10);
	if (str[0] == '/') {
		while (node->parent) {
			node = node->parent;
		}
	}
	res = node_get_node_(node, str);
	free(str);
	return res;
}

static char * node_get_value (node_t *node, char *path)
{
	node_t *res;
	res = node_get_node(node, path);
	if (res != NULL) {
		return res->value;
	}
	return NULL;
}

static void node_print_ (node_t *node)
{
	int i;
	for (i = 0; i < g_depth; i++) {
		printf("  ");
	}
	printf("%s : ", node->name);
	if (node->value) {
		printf("%s ", node->value);
	}
	if (node->id) {
		printf("[%s] ", node->id);
	}
	if (node->type) {
		printf("[%s] ", node->type);
	}
	printf("\n");
}

static void node_print (node_t *node)
{
	int p;
	node_t *tmp;
	node_print_(node);
	g_depth++;
	p = 0;
	while (!list_eol(node->nodes, p)) {
		tmp = (node_t *) list_get(node->nodes, p);
		node_print(tmp);
		p++;
	}
	g_depth--;
}

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
	if ((tmp = node_get_node(node, "size")) != NULL) {
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
	if ((tmp = node_get_node(node, "size")) != NULL) {
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
	if ((tmp = node_get_node(node, "size")) != NULL) {
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

static void node_generate_element (node_t *node)
{
	int p;
	node_t *tmp;
	node_t *dmp;
	node_t *chl;
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

typedef struct lang_header_s {
	char magic[8];
	unsigned long int count;
} lang_header_t;

typedef struct lang_s {
	unsigned long int hash;
	unsigned long int id_len;
	unsigned long int str_len;
	unsigned long int id_offset;
	unsigned long int str_offset;
} lang_t;

typedef struct lmsg_s {
	char *id;
	char *str;
} lmsg_t;

static unsigned long int hash_string (const char *str_param)
{
	#define HASHWORDBITS 32
	unsigned long int hval, g;
	const char *str = str_param;
	/* Compute the hash value for the given string.  */
	hval = 0;
	while (*str != '\0') {
		hval <<= 4;
		hval += (unsigned char) *str++;
		g = hval & ((unsigned long int) 0xf << (HASHWORDBITS - 4));
		if (g != 0) {
			hval ^= g >> (HASHWORDBITS - 8);
			hval ^= g;
		}
	}
	return hval;
}

static char * manipulate_string (char *str_param)
{
	int i;
	int j;
	int s;
	char *out = str_param;
	for (s = 0, i = 0; out[i]; i++){
		if (out[i] == '"') {
			if (i == 0 ||
			    out[i - 1] != '\\') {
			    	s = (s + 1) % 2;
			    	if (s == 0) {
			    		while (out[i + 1] && out[i + 1] != '"') {
						for (j = i + 1; out[j]; j++) {
							out[j] = out[j + 1];
						}
			    		}
			    	}
			}
		}
	}
	/* Remove leading " */
	while (out[0] == '"') {
		for (j = 0; out[j]; j++) {
			out[j] = out[j + 1];
		}
	}
	/* Remove lonely " 's */
	for (i = 0; out[i + 1]; i++) {
		if (out[i] != '\\' && out[i + 1] == '"') {
			for (j = i + 1; out[j]; j++)
				out[j] = out[j + 1];
			i--;
		}
	}
	/* Convert \" 's to " */
	for (i = 0; out[i + 1]; i++) {
		if (out[i] == '\\' && out[i + 1] == '"') {
			for (j = i; out[j]; j++)
				out[j] = out[j + 1];
			i--;
		}
	}
	return out;
}

static void node_generate_language (node_t *node)
{
	int i;
	unsigned long int offset;
	node_t *id;
	node_t *str;
	node_t *msg;
	lmsg_t *lmsg;
	lang_t *lang;
	list_t *llang;
	list_t *llmsg;
	char *fname;
	FILE *flang;
	lang_header_t lheader = {{"xynth.xo"}, 3};
	fname = (char *) malloc(sizeof(char) * (strlen(g_name) + 20));
	sprintf(fname, "%s.%s.xo", g_name, node->id);
	flang = fopen(fname, "w+");
	list_init(&llang);
	list_init(&llmsg);
	while ((msg = node_get_node(node, "message")) != NULL) {
		id = node_get_node(msg, "id");
		str = node_get_node(msg, "str");
		if (id && str && id->value && str->value) {
			lmsg = (lmsg_t *) malloc(sizeof(lmsg_t));
			lang = (lang_t *) malloc(sizeof(lang_t));
			manipulate_string(id->value);
			manipulate_string(str->value);
			lang->hash = hash_string(id->value);
			lmsg->id = id->value;
			lmsg->str = str->value;
			lang->id_len = strlen(lmsg->id);
			lang->str_len = strlen(lmsg->str);
			list_add(llang, lang, -1);
			list_add(llmsg, lmsg, -1);
		}
		msg->dontparse = 1;
		if (id) id->dontparse = 1;
		if (str) str->dontparse = 1;
	}
	lheader.count = llang->nb_elt;
	offset = sizeof(lang_header_t);
	offset += (sizeof(lang_t) * lheader.count);
	for (i = 0; !list_eol(llmsg, i); i++) {
		lmsg = (lmsg_t *) list_get(llmsg, i);
		lang = (lang_t *) list_get(llang, i);
		lang->id_offset = offset;
		offset += (strlen(lmsg->id) + 1);
	}
	for (i = 0; !list_eol(llmsg, i); i++) {
		lmsg = (lmsg_t *) list_get(llmsg, i);
		lang = (lang_t *) list_get(llang, i);
		lang->str_offset = offset;
		offset += (strlen(lmsg->str) + 1);
	}
	fwrite(&lheader, sizeof(lang_header_t), 1, flang);
	for (i = 0; !list_eol(llang, i); i++) {
		lang = (lang_t *) list_get(llang, i);
		fwrite(lang, sizeof(lang_t), 1, flang);
	}
	for (i = 0; !list_eol(llmsg, i); i++) {
		lmsg = (lmsg_t *) list_get(llmsg, i);
		fwrite(lmsg->id, 1, strlen(lmsg->id) + 1, flang);
	}
	for (i = 0; !list_eol(llmsg, i); i++) {
		lmsg = (lmsg_t *) list_get(llmsg, i);
		fwrite(lmsg->str, 1, strlen(lmsg->str) + 1, flang);
	}
	free(fname);
	fclose(flang);
}

static void node_generate_localization (node_t *node)
{
	node_t *lang;
	node_t *local;
	while ((local = node_get_node(node, "localization")) != NULL) {
		while ((lang = node_get_node(local, "language")) != NULL) {
			node_generate_language(lang);
			lang->dontparse = 1;
		}
		local->dontparse = 1;
	}
}

static void node_generate_sources (node_t *node)
{
	node_generate_element(node);
	fprintf(g_header,
	        "\n"
	        "#include <stdio.h>\n"
	        "#include <stdlib.h>\n"
	        "#include <time.h>\n"
	        "#include <xynth.h>\n"
	        "#include <widget.h>\n"
	        "\n"
	        "#define LC_ALL      0\n"
	        "#define LC_MESSAGES 0\n"
		"char * w_setlocale(int category, const char *locale);\n"
		"char * w_bindtextdomain (const char *domainname, const char *dirname);\n"
		"char * w_textdomain (const char *domainname);\n"
		"void w_gettext_free (void);\n"
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
	        "srand(time(NULL));\n"
	        "w_setlocale(LC_ALL, \"\");\n"
	        "w_bindtextdomain(\"%s\", \"./lang\");\n"
	        "w_textdomain(\"%s\");\n",
	        g_header_name,
	        g_name,
	        g_name);
	node_generate_code(node);
	fprintf(g_source,
		"w_gettext_free();\n"
	        "return 0;\n"
	        "}\n");
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

static void char_hndl (void *xdata, const char *txt, int txtlen)
{
	char *str;
	char *ptr;
	char *end;
	if (txtlen > 0 &&
	    txt &&
	    g_path) {
	} else {
	    return;
	}
	end = (char *) malloc(sizeof(char) * (strlen(g_path) + 3 + 1));
	sprintf(end, "</%s>", g_path);
	str = strdup(txt);
#if 0
	ptr = str + txtlen;
	if (g_path &&
	    strncmp(ptr, end, strlen(end)) == 0) {
	    	str[txtlen] = '\0';
	    	if (g_active) {
	    		g_active->value = strdup(str);
	    	}
	}
#else
	ptr = strstr(str, end);
	if (g_path &&
	    ptr) {
	    	*ptr = '\0';
	    	if (g_active && g_active->value == NULL) {
	    		g_active->value = strdup(str);
	    	}
	}
#endif
	free(str);
	free(end);
}

int main (int argc, char **argv)
{
	int c;
	int l = 0;
	char *buf = NULL;
	char *varo = NULL;
	char *varf = NULL;
	struct stat stbuf;
	
	while ((c = getopt(argc, argv, "f:o:clh")) != -1) {
		switch (c) {
			case 'f':
				varf = strdup(optarg);
				break;
			case 'o':
				varo = strdup(optarg);
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
	
	if (varf != NULL) {
		stat(varf, &stbuf);
		l = stbuf.st_size;
		buf = (char *) malloc(stbuf.st_size + 1);
		g_input = fopen(varf, "r");
		fread(buf, 1, stbuf.st_size, g_input);
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

	if (localization)
		node_generate_localization(g_node);
	if (sources)
		node_generate_sources(g_node);

	node_uninit(g_node);
	node_uninit(s_node);
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
