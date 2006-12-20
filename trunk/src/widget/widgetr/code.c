
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
	w_object_t *object;
	char *x = node_get_value(node, "x");
	char *y = node_get_value(node, "y");
	char *w = node_get_value(node, "w");
	char *h = node_get_value(node, "h");
	if (x) xi = atoi(x);
	if (y) yi = atoi(y);
	if (w) wi = atoi(w);
	if (h) hi = atoi(h);
	if (strcmp(node->parent->name, "window") == 0) {
		if ((object = (w_object_t *) table_get_data(ctable->table, ctable->depth, ctable->mask, node->parent->id)) != NULL) {
			w_window_set_coor(object->window, xi, yi, wi, hi);
		}
	} else if (strcmp(node->parent->name, "object") == 0) {
		if ((object = (w_object_t *) table_get_data(ctable->table, ctable->depth, ctable->mask, node->parent->id)) != NULL) {
			w_object_move(object, xi, yi, wi, hi);
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
	table_add(ctable->table, ctable->depth, ctable->mask, node->id, window->object);
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
	w_object_t *object;
	if (strcmp(node->parent->name, "window") == 0) {
		if ((object = (w_object_t *) table_get_data(ctable->table, ctable->depth, ctable->mask, node_get_parent(node, "window")->id)) != NULL) {
			w_object_show(object);
			s_window_show(object->window->window);
			s_client_main(object->window->window);
		}
	} else if (strcmp(node->parent->name, "object") == 0) {
		if ((object = (w_object_t *) table_get_data(ctable->table, ctable->depth, ctable->mask, node_get_parent(node, "object")->id)) != NULL) {
			w_object_show(object);
		}
	}
}

void code_generate_object_frame (ctable_t *ctable, node_t *node)
{
	node_t *tmp;
	w_frame_t *frame;
	w_object_t *pobject;
	w_object_t *wobject;
	node_t *window = node_get_parent(node, "window");
	wobject = (w_object_t *) table_get_data(ctable->table, ctable->depth, ctable->mask, window->id);
	pobject = (w_object_t *) table_get_data(ctable->table, ctable->depth, ctable->mask, node->parent->id);
	w_frame_init(wobject->window, &frame, 0, pobject);
	table_add(ctable->table, ctable->depth, ctable->mask, node->id, frame->object);
	while ((tmp = node_get_node(node, "style")) != NULL) {
		node_t *shape = node_get_node(node, "shape");
		node_t *shadow = node_get_node(node, "shadow");
		w_frame_set_style(frame->object, FRAME_PANEL, FRAME_RAISED); 
		tmp->dontparse = 1;
	}
}

void code_generate_object (ctable_t *ctable, node_t *node)
{
	if (strcmp(node->type, "frame") == 0) {
		code_generate_object_frame(ctable, node);
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
	} else if (strcmp(node->name, "object") == 0) {
		code_generate_object(ctable, node);
	} else if (strcmp(node->name, "move") == 0) {
		code_generate_move(ctable, node);
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
