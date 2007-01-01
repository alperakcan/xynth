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

#include "xynth_.h"
#include "widget.h"

#include "list.h"
#include "node.h"
#include "table.h"
#include "widgetr.h"
#include "code.h"

#ifdef WIDGET_SCRIPT_ENGINE_JS
extern code_script_t js_script;
#endif

code_script_t *code_scripts[] = {
#ifdef WIDGET_SCRIPT_ENGINE_JS
	&js_script,
#endif
	NULL,
};

code_script_t *g_engine = NULL;

#define TAD(__key, __data) table_add(ctable->table, ctable->depth, ctable->mask, __key, __data);
#define TGD(__key) table_get_data(ctable->table, ctable->depth, ctable->mask, __key)

char * code_trim_quota (char *value)
{
	char *ptr = value;
	/* strip trailing white space, returns, etc */
	while ((*ptr != '\0') && (ptr[strlen(ptr) - 1] == '"')) {
		ptr[strlen(ptr) - 1] = '\0';
	}
	/* strip leading white space, returns, etc */
	while (*ptr && (*ptr == '"')) {
		ptr++;
	}
	return ptr;
}

char * code_trim_space (char *value)
{
	char *ptr = value;
	/* strip trailing white space, returns, etc */
	while ((*ptr != '\0') && (ptr[strlen(ptr) - 1] < 33)) {
		ptr[strlen(ptr) - 1] = '\0';
	}
	/* strip leading white space, returns, etc */
	while (*ptr && (*ptr < 33)) {
		ptr++;
	}
	return ptr;
}

void code_tokenize (char *value, char token, int *n, char ***tokens)
{
	char *tmp;
	char **tok;
	unsigned int nt;
	unsigned int count;
	for (count = 0, tmp = value; *tmp != 0; tmp++) {
		if (*tmp == token) {
			count++;
		}
	}
	tok = (char **) malloc(sizeof(char **) * (count + 1));
	if (tok == NULL) {
		*n = 0;
		return;
	}
	nt = 0;
	tmp = value;
	tok[nt] = tmp;
	for (; *tmp != 0; tmp++) {
		if (*tmp == token) {
			*tmp++ = '\0';
			if (*tmp == 0) {
				break;
			} else {
				tok[++nt] = tmp;
			}
		}
	}
	*n = nt + 1;
	*tokens = tok;
	
	return;
}

void code_get_enum (ctable_t *ctable, char *val, unsigned int *prop)
{
	int i;
	int tok_count;
	char **tok_vals;
	*prop = 0;
	if (val == NULL) {
		return;
	}
	code_tokenize(val, '|', &tok_count, &tok_vals);
	for (i = 0; i < tok_count; i++) {
		*prop |= (unsigned int) TGD(code_trim_space(tok_vals[i]));
	}
	s_free(tok_vals);
}

void code_get_effect (ctable_t *ctable, node_t *node, EFFECT *effect)
{
	node_t *t_effect = node_get_node(node, "effect");
	*effect = EFFECT_NONE;
	if (node == NULL) {
		return;
	}
	if (t_effect) {
		code_get_enum(ctable, t_effect->value, effect);
		t_effect->dontparse = 1;
	}
}

void code_get_style (ctable_t *ctable, node_t *node, FRAME_SHAPE *fshape, FRAME_SHADOW *fshadow)
{
	node_t *shape = node_get_node(node, "shape");
	node_t *shadow = node_get_node(node, "shadow");
	*fshape = FRAME_NOFRAME;
	*fshadow = FRAME_PLAIN;
	if (node == NULL) {
		return;
	}
	if (shape) {
		code_get_enum(ctable, shape->value, fshape);
		shape->dontparse = 1;
	}
	if (shadow) {
		code_get_enum(ctable, shadow->value, fshadow);
		shadow->dontparse = 1;
	}
}

void code_get_image (ctable_t *ctable, node_t *node, unsigned int *istyle, unsigned int *irotate, unsigned int *icount, char ***ivar)
{
	int i;
	int count;
	char **var;
	char *cntstr;
	node_t *tmp;
	FRAME_SHAPE shape;
	FRAME_SHADOW shadow;
	FRAME_IMAGE_ROTATION rotate;
	*istyle = 0;
	*irotate = 0;
	*icount = 0;
	*ivar = NULL;
	count = atoi(node_get_value(node, "count"));
	if (count == 0) {
		return;
	}
	code_get_enum(ctable, node_get_value(node, "rotate"), &rotate);
	code_get_style(ctable, node_get_node(node, "style"), &shape, &shadow);
	cntstr = (char *) s_malloc(sizeof(char *) * 255);
	var = (char **) s_malloc(sizeof(char **) * count);
	for (i = 0; i < count; i++) {
		sprintf(cntstr, "image%d", i);
		tmp = node_get_node(node, cntstr);
		var[i] = strdup(tmp->value);
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "style")) != NULL) { tmp->dontparse = 1; }
	if ((tmp = node_get_node(node, "style/shape")) != NULL) { tmp->dontparse = 1; }
	if ((tmp = node_get_node(node, "style/shadow")) != NULL) { tmp->dontparse = 1; }
	if ((tmp = node_get_node(node, "count")) != NULL) { tmp->dontparse = 1; }
	if ((tmp = node_get_node(node, "rotate")) != NULL) { tmp->dontparse = 1; }
	s_free(cntstr);
	*istyle = shape | shadow;
	*irotate = rotate;
	*icount = count;
	*ivar = var;
}

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
		if ((object = (w_object_t *) TGD(node->parent->id)) != NULL) {
			w_window_set_coor(object->window, xi, yi, wi, hi);
		}
	} else if (strcmp(node->parent->name, "object") == 0) {
		if ((object = (w_object_t *) TGD(node->parent->id)) != NULL) {
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
	S_WINDOW prop;
	w_window_t *window;
	code_get_enum(ctable, node->type, &prop);
	w_window_init(&window, prop, NULL);
	TAD(node->id, window->object);
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
		if ((object = (w_object_t *) TGD(node_get_parent(node, "window")->id)) != NULL) {
			w_object_show(object);
			s_window_show(object->window->window);
			s_window_main(object->window->window);
		}
	} else if (strcmp(node->parent->name, "object") == 0) {
		if ((object = (w_object_t *) TGD(node_get_parent(node, "object")->id)) != NULL) {
			w_object_show(object);
		}
	}
}

void code_generate_object_frame (ctable_t *ctable, node_t *node)
{
	node_t *tmp;
	w_frame_t *frame;
	code_priv_t *priv;
	w_object_t *pobject;
	w_object_t *wobject;
	node_t *window = node_get_parent(node, "window");
	wobject = (w_object_t *) TGD(window->id);
	pobject = (w_object_t *) TGD(node->parent->id);
	w_frame_init(wobject->window, &frame, 0, pobject);
	TAD(node->id, frame->object);
	while ((tmp = node_get_node(node, "style")) != NULL) {
		FRAME_SHAPE fshape;
		FRAME_SHADOW fshadow;
		code_get_style(ctable, tmp, &fshape, &fshadow);
		w_frame_set_style(frame->object, fshape, fshadow); 
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "image")) != NULL) {
		char **var;
		unsigned int count;
		unsigned int style;
		unsigned int rotate;
		code_get_image(ctable, tmp, &style, &rotate, &count, &var);
		if (var != NULL) {
			w_frame_set_image(frame->object, style, rotate, count, var);
			while (count--) s_free(var[count]);
			s_free(var);
		}
		tmp->dontparse = 1;
	}
	priv = (code_priv_t *) s_malloc(sizeof(code_priv_t));
	memset(priv, 0, sizeof(code_priv_t));
	if ((tmp = node_get_node(node, "effect")) != NULL) {
		EFFECT effect;
		code_get_effect(ctable, tmp, &effect);
		frame->object->effect->effect = effect;
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "draw")) != NULL) {
		priv->draw = strdup(tmp->value);
		if (g_engine) frame->object->draw = g_engine->object_draw;
		tmp->dontparse = 1;
	}
	frame->object->priv = priv;
}

void code_generate_object_button (ctable_t *ctable, node_t *node)
{
	node_t *tmp;
	code_priv_t *priv;
	w_button_t *button;
	w_object_t *pobject;
	w_object_t *wobject;
	node_t *window = node_get_parent(node, "window");
	wobject = (w_object_t *) TGD(window->id);
	pobject = (w_object_t *) TGD(node->parent->id);
	w_button_init(wobject->window, &button, pobject);
	TAD(node->id, button->object);
	while ((tmp = node_get_node(node, "style")) != NULL) {
		FRAME_SHAPE fshape;
		FRAME_SHADOW fshadow;
		code_get_style(ctable, tmp, &fshape, &fshadow);
		w_button_set_style(button->object, fshape, fshadow); 
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "image")) != NULL) {
		char **var;
		unsigned int count;
		unsigned int style;
		unsigned int rotate;
		code_get_image(ctable, tmp, &style, &rotate, &count, &var);
		if (var != NULL) {
			w_button_set_image(button->object, style, rotate, count, var);
			while (count--) s_free(var[count]);
			s_free(var);
		}
		tmp->dontparse = 1;
	}
	priv = (code_priv_t *) s_malloc(sizeof(code_priv_t));
	memset(priv, 0, sizeof(code_priv_t));
	if ((tmp = node_get_node(node, "pressed")) != NULL) {
		priv->pressed = strdup(tmp->value);
		if (g_engine) w_button_set_pressed(button->object, g_engine->button_pressed);
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "released")) != NULL) {
		priv->released = strdup(tmp->value);
		if (g_engine) w_button_set_released(button->object, g_engine->button_released);
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "clicked")) != NULL) {
		priv->clicked = strdup(tmp->value);
		if (g_engine) w_button_set_clicked(button->object, g_engine->button_clicked);
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "draw")) != NULL) {
		priv->draw = strdup(tmp->value);
		if (g_engine) button->object->draw = g_engine->object_draw;
		tmp->dontparse = 1;
	}
	button->object->priv = priv;
}

void code_generate_object_textbox (ctable_t *ctable, node_t *node)
{
	node_t *tmp;
	w_textbox_t *textbox;
	w_object_t *pobject;
	w_object_t *wobject;
	node_t *window = node_get_parent(node, "window");
	wobject = (w_object_t *) TGD(window->id);
	pobject = (w_object_t *) TGD(node->parent->id);
	w_textbox_init(wobject->window, &textbox, pobject);
	TAD(node->id, textbox->object);
	while ((tmp = node_get_node(node, "style")) != NULL) {
		FRAME_SHAPE fshape;
		FRAME_SHADOW fshadow;
		code_get_style(ctable, tmp, &fshape, &fshadow);
		w_textbox_set_style(textbox->object, fshape, fshadow); 
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "image")) != NULL) {
		char **var;
		unsigned int count;
		unsigned int style;
		unsigned int rotate;
		code_get_image(ctable, tmp, &style, &rotate, &count, &var);
		if (var != NULL) {
			w_textbox_set_image(textbox->object, style, rotate, count, var);
			while (count--) s_free(var[count]);
			s_free(var);
		}
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "properties")) != NULL) {
		TEXTBOX_PROPERTIES prop;
		code_get_enum(ctable, tmp->value, &prop);
		w_textbox_set_properties(textbox->object, prop);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "size")) != NULL) {
		int size = atoi(tmp->value);
		w_textbox_set_size(textbox->object, size);
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "color")) != NULL) {
		int cr = atoi(node_get_value(tmp, "red"));
		int cg = atoi(node_get_value(tmp, "green"));
		int cb = atoi(node_get_value(tmp, "blue"));
		w_textbox_set_rgb(textbox->object, cr, cg, cb);
		if ((tmp = node_get_node(node, "color/red")) != NULL) { tmp->dontparse = 1; }
		if ((tmp = node_get_node(node, "color/green")) != NULL) { tmp->dontparse = 1; }
		if ((tmp = node_get_node(node, "color/blue")) != NULL) { tmp->dontparse = 1; }
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "string")) != NULL) {
		w_textbox_set_str(textbox->object, code_trim_quota(tmp->value));
		tmp->dontparse = 1;
	}
}

void code_generate_object_editbox (ctable_t *ctable, node_t *node)
{
	node_t *tmp;
	w_editbox_t *editbox;
	w_object_t *pobject;
	w_object_t *wobject;
	node_t *window = node_get_parent(node, "window");
	wobject = (w_object_t *) TGD(window->id);
	pobject = (w_object_t *) TGD(node->parent->id);
	w_editbox_init(wobject->window, &editbox, pobject);
	TAD(node->id, editbox->object);
	while ((tmp = node_get_node(node, "style")) != NULL) {
		FRAME_SHAPE fshape;
		FRAME_SHADOW fshadow;
		code_get_style(ctable, tmp, &fshape, &fshadow);
		w_editbox_set_style(editbox->object, fshape, fshadow); 
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "image")) != NULL) {
		char **var;
		unsigned int count;
		unsigned int style;
		unsigned int rotate;
		code_get_image(ctable, tmp, &style, &rotate, &count, &var);
		if (var != NULL) {
			w_editbox_set_image(editbox->object, style, rotate, count, var);
			while (count--) s_free(var[count]);
			s_free(var);
		}
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "properties")) != NULL) {
		TEXTBOX_PROPERTIES prop;
		code_get_enum(ctable, tmp->value, &prop);
		w_editbox_set_properties(editbox->object, prop);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "size")) != NULL) {
		int size = atoi(tmp->value);
		w_editbox_set_size(editbox->object, size);
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "color")) != NULL) {
		int cr = atoi(node_get_value(tmp, "red"));
		int cg = atoi(node_get_value(tmp, "green"));
		int cb = atoi(node_get_value(tmp, "blue"));
		w_editbox_set_rgb(editbox->object, cr, cg, cb);
		if ((tmp = node_get_node(node, "color/red")) != NULL) { tmp->dontparse = 1; }
		if ((tmp = node_get_node(node, "color/green")) != NULL) { tmp->dontparse = 1; }
		if ((tmp = node_get_node(node, "color/blue")) != NULL) { tmp->dontparse = 1; }
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "string")) != NULL) {
		w_editbox_set_str(editbox->object, code_trim_quota(tmp->value));
		tmp->dontparse = 1;
	}
}

void code_generate_object_checkbox (ctable_t *ctable, node_t *node)
{
	node_t *tmp;
	w_checkbox_t *checkbox;
	w_object_t *pobject;
	w_object_t *wobject;
	node_t *window = node_get_parent(node, "window");
	wobject = (w_object_t *) TGD(window->id);
	pobject = (w_object_t *) TGD(node->parent->id);
	w_checkbox_init(wobject->window, &checkbox, pobject);
	TAD(node->id, checkbox->object);
	while ((tmp = node_get_node(node, "style")) != NULL) {
		FRAME_SHAPE fshape;
		FRAME_SHADOW fshadow;
		code_get_style(ctable, tmp, &fshape, &fshadow);
		w_checkbox_set_style(checkbox->object, fshape, fshadow); 
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "boxstyle")) != NULL) {
		FRAME_SHAPE fshape;
		FRAME_SHADOW fshadow;
		code_get_style(ctable, tmp, &fshape, &fshadow);
		w_checkbox_set_boxstyle(checkbox->object, fshape, fshadow); 
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "image")) != NULL) {
		char **var;
		unsigned int count;
		unsigned int style;
		unsigned int rotate;
		code_get_image(ctable, tmp, &style, &rotate, &count, &var);
		if (var != NULL) {
			w_checkbox_set_image(checkbox->object, style, rotate, count, var);
			while (count--) s_free(var[count]);
			s_free(var);
		}
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "boximage")) != NULL) {
		char **var;
		unsigned int count;
		unsigned int style;
		unsigned int rotate;
		code_get_image(ctable, tmp, &style, &rotate, &count, &var);
		if (var != NULL) {
			w_checkbox_set_boximage(checkbox->object, style, rotate, count, var);
			while (count--) s_free(var[count]);
			s_free(var);
		}
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "properties")) != NULL) {
		TEXTBOX_PROPERTIES prop;
		code_get_enum(ctable, tmp->value, &prop);
		w_checkbox_set_properties(checkbox->object, prop);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "size")) != NULL) {
		int size = atoi(tmp->value);
		w_checkbox_set_size(checkbox->object, size);
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "color")) != NULL) {
		int cr = atoi(node_get_value(tmp, "red"));
		int cg = atoi(node_get_value(tmp, "green"));
		int cb = atoi(node_get_value(tmp, "blue"));
		w_checkbox_set_rgb(checkbox->object, cr, cg, cb);
		if ((tmp = node_get_node(node, "color/red")) != NULL) { tmp->dontparse = 1; }
		if ((tmp = node_get_node(node, "color/green")) != NULL) { tmp->dontparse = 1; }
		if ((tmp = node_get_node(node, "color/blue")) != NULL) { tmp->dontparse = 1; }
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "changed")) != NULL) {
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "string")) != NULL) {
		w_checkbox_set_str(checkbox->object, code_trim_quota(tmp->value));
		tmp->dontparse = 1;
	}
}

void code_generate_object_progressbar (ctable_t *ctable, node_t *node)
{
	node_t *tmp;
	w_progressbar_t *progressbar;
	w_object_t *pobject;
	w_object_t *wobject;
	node_t *window = node_get_parent(node, "window");
	wobject = (w_object_t *) TGD(window->id);
	pobject = (w_object_t *) TGD(node->parent->id);
	w_progressbar_init(wobject->window, &progressbar, pobject);
	TAD(node->id, progressbar->object);
	while ((tmp = node_get_node(node, "style")) != NULL) {
		FRAME_SHAPE fshape;
		FRAME_SHADOW fshadow;
		code_get_style(ctable, tmp, &fshape, &fshadow);
		w_progressbar_set_style(progressbar->object, fshape, fshadow); 
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "boxstyle")) != NULL) {
		FRAME_SHAPE fshape;
		FRAME_SHADOW fshadow;
		code_get_style(ctable, tmp, &fshape, &fshadow);
		w_progressbar_set_boxstyle(progressbar->object, fshape, fshadow); 
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "image")) != NULL) {
		char **var;
		unsigned int count;
		unsigned int style;
		unsigned int rotate;
		code_get_image(ctable, tmp, &style, &rotate, &count, &var);
		if (var != NULL) {
			w_progressbar_set_image(progressbar->object, style, rotate, count, var);
			while (count--) s_free(var[count]);
			s_free(var);
		}
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "boximage")) != NULL) {
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "changed")) != NULL) {
		tmp->dontparse = 1;
	}
}

void code_generate_object_scrollbuffer (ctable_t *ctable, node_t *node)
{
	node_t *tmp;
	w_scrollbuffer_t *scrollbuffer;
	w_object_t *pobject;
	w_object_t *wobject;
	node_t *window = node_get_parent(node, "window");
	wobject = (w_object_t *) TGD(window->id);
	pobject = (w_object_t *) TGD(node->parent->id);
	w_scrollbuffer_init(wobject->window, &scrollbuffer, pobject);
	TAD(node->id, scrollbuffer->object);
	while ((tmp = node_get_node(node, "object")) != NULL) {
		code_parse_generate(ctable, tmp);
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "child")) != NULL) {
		w_object_t *cobject = (w_object_t *) TGD(tmp->value);
		w_scrollbuffer_set_child(scrollbuffer->object, cobject);
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "slide")) != NULL) {
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "boxstyle")) != NULL) {
		FRAME_SHAPE fshape;
		FRAME_SHADOW fshadow;
		code_get_style(ctable, tmp, &fshape, &fshadow);
		w_scrollbuffer_set_boxstyle(scrollbuffer->object, fshape, fshadow); 
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "boximage")) != NULL) {
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "addstyle")) != NULL) {
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "addimage")) != NULL) {
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "substyle")) != NULL) {
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "subimage")) != NULL) {
		tmp->dontparse = 1;
	}
}

void code_generate_object (ctable_t *ctable, node_t *node)
{
	if (strcmp(node->type, "frame") == 0) {
		code_generate_object_frame(ctable, node);
	} else if (strcmp(node->type, "button") == 0) {
		code_generate_object_button(ctable, node);
	} else if (strcmp(node->type, "textbox") == 0) {
		code_generate_object_textbox(ctable, node);
	} else if (strcmp(node->type, "editbox") == 0) {
		code_generate_object_editbox(ctable, node);
	} else if (strcmp(node->type, "checkbox") == 0) {
		code_generate_object_checkbox(ctable, node);
	} else if (strcmp(node->type, "progressbar") == 0) {
		code_generate_object_progressbar(ctable, node);
	} else if (strcmp(node->type, "scrollbuffer") == 0) {
		code_generate_object_scrollbuffer(ctable, node);
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

void code_parse (w_table_t *table, unsigned int depth, unsigned int mask, node_t *node, node_t *elem, char *script, char *engine)
{
	ctable_t *ctable;
	code_script_t **sengine;
	
	ctable = (ctable_t *) s_malloc(sizeof(ctable_t));
	ctable->table = table;
	ctable->depth = depth;
	ctable->mask = mask;
	
	TAD("WINDOW_NOFORM", (void *) WINDOW_NOFORM);
	TAD("WINDOW_MAIN", (void *) WINDOW_MAIN);
	TAD("WINDOW_TEMP", (void *) WINDOW_TEMP);
	TAD("WINDOW_CHILD", (void *) WINDOW_CHILD);
	TAD("WINDOW_DESKTOP", (void *) WINDOW_DESKTOP);

	TAD("FRAME_NOFRAME", (void *) FRAME_NOFRAME);
	TAD("FRAME_BOX", (void *) FRAME_BOX);
	TAD("FRAME_PANEL", (void *) FRAME_PANEL);
	TAD("FRAME_WINPANEL", (void *) FRAME_WINPANEL);
	TAD("FRAME_HLINE", (void *) FRAME_HLINE);
	TAD("FRAME_VLINE", (void *) FRAME_VLINE);
	TAD("FRAME_STYLEDPANEL", (void *) FRAME_STYLEDPANEL);
	TAD("FRAME_POPUPPANEL", (void *) FRAME_POPUPPANEL);
	TAD("FRAME_MENUBARPANEL", (void *) FRAME_MENUBARPANEL);
	TAD("FRAME_TOOLBARPANEL", (void *) FRAME_TOOLBARPANEL);
	TAD("FRAME_LINEEDITPANEL", (void *) FRAME_LINEEDITPANEL);
	TAD("FRAME_TABWIDGETPANEL", (void *) FRAME_TABWIDGETPANEL);
	TAD("FRAME_GROUPBOXPANEL", (void *) FRAME_GROUPBOXPANEL);
	TAD("FRAME_EMPTY", (void *) FRAME_EMPTY);
	TAD("FRAME_PLAIN", (void *) FRAME_PLAIN);
	TAD("FRAME_RAISED", (void *) FRAME_RAISED);
	TAD("FRAME_SUNKEN", (void *) FRAME_SUNKEN);
	TAD("FRAME_FOCUSED", (void *) FRAME_FOCUSED);

	TAD("TEXTBOX_WRAP", (void *) TEXTBOX_WRAP);
	TAD("TEXTBOX_VCENTER", (void *) TEXTBOX_VCENTER);
	TAD("TEXTBOX_HCENTER", (void *) TEXTBOX_HCENTER);
	
	TAD("FRAME_IMAGE_SOLID", (void *) FRAME_IMAGE_SOLID);
	TAD("FRAME_IMAGE_VERTICAL", (void *) FRAME_IMAGE_VERTICAL);
	TAD("FRAME_IMAGE_HORIZONTAL", (void *) FRAME_IMAGE_HORIZONTAL);

	TAD("EFFECT_NONE", (void *) EFFECT_NONE);
	TAD("EFFECT_FADEIN", (void *) EFFECT_FADEIN);
	TAD("EFFECT_FADEOUT", (void *) EFFECT_FADEOUT);
	TAD("EFFECT_POPIN", (void *) EFFECT_POPIN);
	TAD("EFFECT_POPOUT", (void *) EFFECT_POPOUT);

	for (sengine = code_scripts; *sengine && engine; sengine++) {
		if (strcmp((*sengine)->name, engine) == 0) {
			g_engine = *sengine;
		} 
	}
	if (g_engine) g_engine->init(ctable, script);	

	code_parse_element(node, elem);
	code_parse_generate(ctable, node);
	
	if (g_engine) g_engine->uninit();
	
	s_free(ctable);
}
