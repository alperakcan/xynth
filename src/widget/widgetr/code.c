
#include "xynth_.h"
#include "widget.h"

#include "node.h"
#include "code.h"
#include "table.h"

typedef struct ctable_s {
	unsigned int depth;
	unsigned int mask;
	w_table_t *table;
} ctable_t;
 
void code_parse_element (node_t *node, node_t *elem)
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
    		code_parse_element(tmp, elem);
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

void code_generate_move (ctable_t *ctable, node_t *node)
{
	node_t *tmp;
	int xi = 0;
	int yi = 0;
	int wi = 0;
	int hi = 0;
	w_window_t *window;
	char *x = node_get_value(node, "x");
	char *y = node_get_value(node, "y");
	char *w = node_get_value(node, "w");
	char *h = node_get_value(node, "h");
	if (x) xi = atoi(x);
	if (y) yi = atoi(y);
	if (w) wi = atoi(w);
	if (h) hi = atoi(h);
	if (strcmp(node->parent->name, "window") == 0) {
		if ((window = (w_window_t *) table_get_data(ctable->table, ctable->depth, ctable->mask, node->parent->id)) != NULL) {
			w_window_set_coor(window, xi, yi, wi, hi);
		}
	}
	node->dontparse = 1;
	if ((tmp = node_get_node(node, "x")) != NULL) { tmp->dontparse = 1; }
	if ((tmp = node_get_node(node, "y")) != NULL) { tmp->dontparse = 1; }
	if ((tmp = node_get_node(node, "w")) != NULL) { tmp->dontparse = 1; }
	if ((tmp = node_get_node(node, "h")) != NULL) { tmp->dontparse = 1; }
}

void code_generate_window (ctable_t *ctable, node_t *node)
{
	node_t *tmp;
	w_window_t *window;
	w_window_init(&window, WINDOW_MAIN, NULL);
	table_add(ctable->table, ctable->depth, ctable->mask, node->id, window);
	if ((tmp = node_get_node(node, "title")) != NULL) {
		s_window_set_title(window->window, tmp->value);
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "move")) != NULL) {
		code_generate_move(ctable, tmp);
		tmp->dontparse = 1;
	}
}

void code_generate_show (ctable_t *ctable, node_t *node)
{
	w_window_t *window;
	if (strcmp(node->parent->name, "window") == 0) {
		if ((window = (w_window_t *) table_get_data(ctable->table, ctable->depth, ctable->mask, node_get_parent(node, "window")->id)) != NULL) {
			w_object_show(window->object);
			s_window_show(window->window);
			s_client_main(window->window);
		}
	}
}

void code_parse_generate (ctable_t *ctable, node_t *node)
{
	int p;
	node_t *tmp;
	if (node->dontparse != 0) {
		return;
	}
	if (strcmp(node->name, "window") == 0) {
		code_generate_window(ctable, node);
	} else if (strcmp(node->name, "show") == 0) {
		code_generate_show(ctable, node);
	}
	p = 0;
	while (!list_eol(node->nodes, p)) {
		tmp = (node_t *) list_get(node->nodes, p);
		code_parse_generate(ctable, tmp);
		p++;
	}
}

void code_parse (w_table_t *table, unsigned int depth, unsigned int mask, node_t *node, node_t *elem)
{
	ctable_t ctable;
	ctable.table = table;
	ctable.depth = depth;
	ctable.mask = mask;
	code_parse_element(node, elem);
	code_parse_generate(&ctable, node);
}
