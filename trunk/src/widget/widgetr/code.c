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
	tok = (char **) s_malloc(sizeof(char **) * (count + 1));
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

void code_get_enum (s_hashtable_t *htable, char *val, unsigned int *prop)
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
		*prop |= (unsigned int) s_hashtable_get_data(htable, code_trim_space(tok_vals[i]));
	}
	s_free(tok_vals);
}

void code_get_effect (s_hashtable_t *htable, s_xml_node_t *node, EFFECT *effect)
{
	s_xml_node_t *t_effect = s_xml_node_get_path(node, "effect");
	*effect = EFFECT_NONE;
	if (node == NULL) {
		return;
	}
	if (t_effect) {
		code_get_enum(htable, t_effect->value, effect);
		t_effect->dontparse = 1;
	}
}

void code_get_style (s_hashtable_t *htable, s_xml_node_t *node, FRAME_SHAPE *fshape, FRAME_SHADOW *fshadow)
{
	s_xml_node_t *shape = s_xml_node_get_path(node, "shape");
	s_xml_node_t *shadow = s_xml_node_get_path(node, "shadow");
	*fshape = FRAME_NOFRAME;
	*fshadow = FRAME_PLAIN;
	if (node == NULL) {
		return;
	}
	if (shape) {
		code_get_enum(htable, shape->value, fshape);
		shape->dontparse = 1;
	}
	if (shadow) {
		code_get_enum(htable, shadow->value, fshadow);
		shadow->dontparse = 1;
	}
}

void code_get_image (s_hashtable_t *htable, s_xml_node_t *node, unsigned int *istyle, unsigned int *irotate, unsigned int *icount, char ***ivar)
{
	int i;
	int count;
	char **var;
	char *cntstr;
	s_xml_node_t *tmp;
	FRAME_SHAPE shape;
	FRAME_SHADOW shadow;
	FRAME_IMAGE_ROTATION rotate;
	*istyle = 0;
	*irotate = 0;
	*icount = 0;
	*ivar = NULL;
	count = atoi(s_xml_node_get_path_value(node, "count"));
	if (count == 0) {
		return;
	}
	code_get_enum(htable, s_xml_node_get_path_value(node, "rotate"), &rotate);
	code_get_style(htable, s_xml_node_get_path(node, "style"), &shape, &shadow);
	cntstr = (char *) s_malloc(sizeof(char *) * 255);
	var = (char **) s_malloc(sizeof(char **) * count);
	for (i = 0; i < count; i++) {
		sprintf(cntstr, "image%d", i);
		tmp = s_xml_node_get_path(node, cntstr);
		var[i] = strdup(tmp->value);
		tmp->dontparse = 1;
	}
	if ((tmp = s_xml_node_get_path(node, "style")) != NULL) { tmp->dontparse = 1; }
	if ((tmp = s_xml_node_get_path(node, "style/shape")) != NULL) { tmp->dontparse = 1; }
	if ((tmp = s_xml_node_get_path(node, "style/shadow")) != NULL) { tmp->dontparse = 1; }
	if ((tmp = s_xml_node_get_path(node, "count")) != NULL) { tmp->dontparse = 1; }
	if ((tmp = s_xml_node_get_path(node, "rotate")) != NULL) { tmp->dontparse = 1; }
	s_free(cntstr);
	*istyle = shape | shadow;
	*irotate = rotate;
	*icount = count;
	*ivar = var;
}

void code_parse_element (s_xml_node_t *node, s_xml_node_t *style)
{
	int p;
	s_xml_node_t *tmp;
	s_xml_node_t *dmp;
	s_xml_node_t *chl;
	s_xml_node_attr_t *sid;
	s_xml_node_attr_t *stype;
	s_xml_node_attr_t *ntype;
	if (style == NULL) {
		return;
	}
	for (p = 0; !s_list_eol(node->nodes, p); p++) {
    		tmp = (s_xml_node_t *) s_list_get(node->nodes, p);
    		code_parse_element(tmp, style);
	}
	ntype = s_xml_node_get_attr(node, "type");
	if (strcmp(node->name, "element") == 0 && ntype && ntype->value != NULL) {
	    	for (p = 0; !s_list_eol(style->nodes, p); p++) {
	    		tmp = (s_xml_node_t *) s_list_get(style->nodes, p);
	    		sid = s_xml_node_get_attr(tmp, "id");
	    		stype = s_xml_node_get_attr(tmp, "type");
	    		if (strcmp(tmp->name, "element") == 0 && sid && strcmp(sid->value, ntype->value) == 0) {
	    		    	s_xml_node_dublicate(tmp, &dmp);
	    		    	s_free(node->name);
	    		    	s_free(ntype->value);
	    		    	node->name = strdup("object");
	    		    	ntype->value = (stype->value) ? strdup(stype->value) : NULL;
	    		    	while (!s_list_eol(dmp->nodes, 0)) {
	    		    		chl = (s_xml_node_t *) s_list_get(dmp->nodes, dmp->nodes->nb_elt - 1);
	    		    		chl->parent = node;
	    		    		s_list_add(node->nodes, chl, 0);
	    		    		s_list_remove(dmp->nodes, dmp->nodes->nb_elt - 1);
	    		    	}
	    		    	s_xml_node_uninit(dmp);
	    		}
	    	}
	}
}

void code_generate_move (s_hashtable_t *htable, s_xml_node_t *node)
{
	int xi = 0;
	int yi = 0;
	int wi = 0;
	int hi = 0;
	s_xml_node_t *tmp;
	w_object_t *object;
	char *x = s_xml_node_get_path_value(node, "x");
	char *y = s_xml_node_get_path_value(node, "y");
	char *w = s_xml_node_get_path_value(node, "w");
	char *h = s_xml_node_get_path_value(node, "h");
	if (x) xi = atoi(x);
	if (y) yi = atoi(y);
	if (w) wi = atoi(w);
	if (h) hi = atoi(h);
	if (strcmp(node->parent->name, "window") == 0) {
		if ((object = (w_object_t *) s_hashtable_get_data(htable, s_xml_node_get_attr_value(node->parent, "id"))) != NULL) {
			w_window_set_coor(object->window, xi, yi, wi, hi);
		}
	} else if (strcmp(node->parent->name, "object") == 0) {
		if ((object = (w_object_t *) s_hashtable_get_data(htable, s_xml_node_get_attr_value(node->parent, "id"))) != NULL) {
			w_object_move(object, xi, yi, wi, hi);
		}
	}
	node->dontparse = 1;
	if ((tmp = s_xml_node_get_path(node, "x")) != NULL) { tmp->dontparse = 1; }
	if ((tmp = s_xml_node_get_path(node, "y")) != NULL) { tmp->dontparse = 1; }
	if ((tmp = s_xml_node_get_path(node, "w")) != NULL) { tmp->dontparse = 1; }
	if ((tmp = s_xml_node_get_path(node, "h")) != NULL) { tmp->dontparse = 1; }
}

void code_generate_window (s_hashtable_t *htable, s_xml_node_t *node)
{
	S_WINDOW prop;
	s_xml_node_t *tmp;
	w_window_t *window;
	code_get_enum(htable, s_xml_node_get_attr_value(node, "type"), &prop);
	w_window_init(&window, prop, NULL);
	s_hashtable_add(htable, s_xml_node_get_attr_value(node, "id"), window->object);
	if ((tmp = s_xml_node_get_path(node, "title")) != NULL) {
		s_window_set_title(window->window, tmp->value);
		tmp->dontparse = 1;
	}
	if ((tmp = s_xml_node_get_path(node, "move")) != NULL) {
		code_generate_move(htable, tmp);
		tmp->dontparse = 1;
	}
}

void code_generate_show (s_hashtable_t *htable, s_xml_node_t *node)
{
	w_object_t *object;
	if (strcmp(node->parent->name, "window") == 0) {
		if ((object = (w_object_t *) s_hashtable_get_data(htable, s_xml_node_get_attr_value(node->parent, "id"))) != NULL) {
			w_object_show(object);
			s_window_show(object->window->window);
			s_window_main(object->window->window);
		}
	} else if (strcmp(node->parent->name, "object") == 0) {
		if ((object = (w_object_t *) s_hashtable_get_data(htable, s_xml_node_get_attr_value(node->parent, "id"))) != NULL) {
			w_object_show(object);
		}
	}
}

void code_generate_object_frame (s_hashtable_t *htable, s_xml_node_t *node)
{
	w_frame_t *frame;
	s_xml_node_t *tmp;
	code_priv_t *priv;
	w_object_t *pobject;
	w_object_t *wobject;
	s_xml_node_t *window = s_xml_node_get_parent(node, "window");
	wobject = (w_object_t *) s_hashtable_get_data(htable, s_xml_node_get_attr_value(window, "id"));
	pobject = (w_object_t *) s_hashtable_get_data(htable, s_xml_node_get_attr_value(node->parent, "id"));
	w_frame_init(wobject->window, &frame, 0, pobject);
	s_hashtable_add(htable, s_xml_node_get_attr_value(node, "id"), frame->object);
	while ((tmp = s_xml_node_get_path(node, "style")) != NULL) {
		FRAME_SHAPE fshape;
		FRAME_SHADOW fshadow;
		code_get_style(htable, tmp, &fshape, &fshadow);
		w_frame_set_style(frame->object, fshape, fshadow); 
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "image")) != NULL) {
		char **var;
		unsigned int count;
		unsigned int style;
		unsigned int rotate;
		code_get_image(htable, tmp, &style, &rotate, &count, &var);
		if (var != NULL) {
			w_frame_set_image(frame->object, style, rotate, count, var);
			while (count--) s_free(var[count]);
			s_free(var);
		}
		tmp->dontparse = 1;
	}
	priv = (code_priv_t *) s_malloc(sizeof(code_priv_t));
	memset(priv, 0, sizeof(code_priv_t));
	if ((tmp = s_xml_node_get_path(node, "effect")) != NULL) {
		EFFECT effect;
		code_get_effect(htable, tmp, &effect);
		frame->object->effect->effect = effect;
		tmp->dontparse = 1;
	}
	if ((tmp = s_xml_node_get_path(node, "draw")) != NULL) {
		priv->draw = strdup(tmp->value);
		if (g_engine) frame->object->draw = g_engine->object_draw;
		tmp->dontparse = 1;
	}
	frame->object->priv = priv;
}

void code_generate_object_button (s_hashtable_t *htable, s_xml_node_t *node)
{
	s_xml_node_t *tmp;
	code_priv_t *priv;
	w_button_t *button;
	w_object_t *pobject;
	w_object_t *wobject;
	s_xml_node_t *window = s_xml_node_get_parent(node, "window");
	wobject = (w_object_t *) s_hashtable_get_data(htable, s_xml_node_get_attr_value(window, "id"));
	pobject = (w_object_t *) s_hashtable_get_data(htable, s_xml_node_get_attr_value(node->parent, "id"));
	w_button_init(wobject->window, &button, pobject);
	s_hashtable_add(htable, s_xml_node_get_attr_value(node, "id"), button->object);
	while ((tmp = s_xml_node_get_path(node, "style")) != NULL) {
		FRAME_SHAPE fshape;
		FRAME_SHADOW fshadow;
		code_get_style(htable, tmp, &fshape, &fshadow);
		w_button_set_style(button->object, fshape, fshadow); 
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "image")) != NULL) {
		char **var;
		unsigned int count;
		unsigned int style;
		unsigned int rotate;
		code_get_image(htable, tmp, &style, &rotate, &count, &var);
		if (var != NULL) {
			w_button_set_image(button->object, style, rotate, count, var);
			while (count--) s_free(var[count]);
			s_free(var);
		}
		tmp->dontparse = 1;
	}
	priv = (code_priv_t *) s_malloc(sizeof(code_priv_t));
	memset(priv, 0, sizeof(code_priv_t));
	if ((tmp = s_xml_node_get_path(node, "pressed")) != NULL) {
		priv->pressed = strdup(tmp->value);
		if (g_engine) w_button_set_pressed(button->object, g_engine->button_pressed);
		tmp->dontparse = 1;
	}
	if ((tmp = s_xml_node_get_path(node, "released")) != NULL) {
		priv->released = strdup(tmp->value);
		if (g_engine) w_button_set_released(button->object, g_engine->button_released);
		tmp->dontparse = 1;
	}
	if ((tmp = s_xml_node_get_path(node, "clicked")) != NULL) {
		priv->clicked = strdup(tmp->value);
		if (g_engine) w_button_set_clicked(button->object, g_engine->button_clicked);
		tmp->dontparse = 1;
	}
	if ((tmp = s_xml_node_get_path(node, "draw")) != NULL) {
		priv->draw = strdup(tmp->value);
		if (g_engine) button->object->draw = g_engine->object_draw;
		tmp->dontparse = 1;
	}
	button->object->priv = priv;
}

void code_generate_object_textbox (s_hashtable_t *htable, s_xml_node_t *node)
{
	s_xml_node_t *tmp;
	w_textbox_t *textbox;
	w_object_t *pobject;
	w_object_t *wobject;
	s_xml_node_t *window = s_xml_node_get_parent(node, "window");
	wobject = (w_object_t *) s_hashtable_get_data(htable, s_xml_node_get_attr_value(window, "id"));
	pobject = (w_object_t *) s_hashtable_get_data(htable, s_xml_node_get_attr_value(node->parent, "id"));
	w_textbox_init(wobject->window, &textbox, pobject);
	s_hashtable_add(htable, s_xml_node_get_attr_value(node, "id"), textbox->object);
	while ((tmp = s_xml_node_get_path(node, "style")) != NULL) {
		FRAME_SHAPE fshape;
		FRAME_SHADOW fshadow;
		code_get_style(htable, tmp, &fshape, &fshadow);
		w_textbox_set_style(textbox->object, fshape, fshadow); 
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "image")) != NULL) {
		char **var;
		unsigned int count;
		unsigned int style;
		unsigned int rotate;
		code_get_image(htable, tmp, &style, &rotate, &count, &var);
		if (var != NULL) {
			w_textbox_set_image(textbox->object, style, rotate, count, var);
			while (count--) s_free(var[count]);
			s_free(var);
		}
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "properties")) != NULL) {
		TEXTBOX_PROPERTIES prop;
		code_get_enum(htable, tmp->value, &prop);
		w_textbox_set_properties(textbox->object, prop);
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "size")) != NULL) {
		int size = atoi(tmp->value);
		w_textbox_set_size(textbox->object, size);
		tmp->dontparse = 1;
	}
	if ((tmp = s_xml_node_get_path(node, "color")) != NULL) {
		int cr = atoi(s_xml_node_get_path_value(tmp, "red"));
		int cg = atoi(s_xml_node_get_path_value(tmp, "green"));
		int cb = atoi(s_xml_node_get_path_value(tmp, "blue"));
		w_textbox_set_rgb(textbox->object, cr, cg, cb);
		if ((tmp = s_xml_node_get_path(node, "color/red")) != NULL) { tmp->dontparse = 1; }
		if ((tmp = s_xml_node_get_path(node, "color/green")) != NULL) { tmp->dontparse = 1; }
		if ((tmp = s_xml_node_get_path(node, "color/blue")) != NULL) { tmp->dontparse = 1; }
		tmp->dontparse = 1;
	}
	if ((tmp = s_xml_node_get_path(node, "string")) != NULL) {
		w_textbox_set_str(textbox->object, code_trim_quota(tmp->value));
		tmp->dontparse = 1;
	}
}

void code_generate_object_editbox (s_hashtable_t *htable, s_xml_node_t *node)
{
	s_xml_node_t *tmp;
	w_editbox_t *editbox;
	w_object_t *pobject;
	w_object_t *wobject;
	s_xml_node_t *window = s_xml_node_get_parent(node, "window");
	wobject = (w_object_t *) s_hashtable_get_data(htable, s_xml_node_get_attr_value(window, "id"));
	pobject = (w_object_t *) s_hashtable_get_data(htable, s_xml_node_get_attr_value(node->parent, "id"));
	w_editbox_init(wobject->window, &editbox, pobject);
	s_hashtable_add(htable, s_xml_node_get_attr_value(node, "id"), editbox->object);
	while ((tmp = s_xml_node_get_path(node, "style")) != NULL) {
		FRAME_SHAPE fshape;
		FRAME_SHADOW fshadow;
		code_get_style(htable, tmp, &fshape, &fshadow);
		w_editbox_set_style(editbox->object, fshape, fshadow); 
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "image")) != NULL) {
		char **var;
		unsigned int count;
		unsigned int style;
		unsigned int rotate;
		code_get_image(htable, tmp, &style, &rotate, &count, &var);
		if (var != NULL) {
			w_editbox_set_image(editbox->object, style, rotate, count, var);
			while (count--) s_free(var[count]);
			s_free(var);
		}
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "properties")) != NULL) {
		TEXTBOX_PROPERTIES prop;
		code_get_enum(htable, tmp->value, &prop);
		w_editbox_set_properties(editbox->object, prop);
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "size")) != NULL) {
		int size = atoi(tmp->value);
		w_editbox_set_size(editbox->object, size);
		tmp->dontparse = 1;
	}
	if ((tmp = s_xml_node_get_path(node, "color")) != NULL) {
		int cr = atoi(s_xml_node_get_path_value(tmp, "red"));
		int cg = atoi(s_xml_node_get_path_value(tmp, "green"));
		int cb = atoi(s_xml_node_get_path_value(tmp, "blue"));
		w_editbox_set_rgb(editbox->object, cr, cg, cb);
		if ((tmp = s_xml_node_get_path(node, "color/red")) != NULL) { tmp->dontparse = 1; }
		if ((tmp = s_xml_node_get_path(node, "color/green")) != NULL) { tmp->dontparse = 1; }
		if ((tmp = s_xml_node_get_path(node, "color/blue")) != NULL) { tmp->dontparse = 1; }
		tmp->dontparse = 1;
	}
	if ((tmp = s_xml_node_get_path(node, "string")) != NULL) {
		w_editbox_set_str(editbox->object, code_trim_quota(tmp->value));
		tmp->dontparse = 1;
	}
}

void code_generate_object_checkbox (s_hashtable_t *htable, s_xml_node_t *node)
{
	s_xml_node_t *tmp;
	w_checkbox_t *checkbox;
	w_object_t *pobject;
	w_object_t *wobject;
	s_xml_node_t *window = s_xml_node_get_parent(node, "window");
	wobject = (w_object_t *) s_hashtable_get_data(htable, s_xml_node_get_attr_value(window, "id"));
	pobject = (w_object_t *) s_hashtable_get_data(htable, s_xml_node_get_attr_value(node->parent, "id"));
	w_checkbox_init(wobject->window, &checkbox, pobject);
	s_hashtable_add(htable, s_xml_node_get_attr_value(node, "id"), checkbox->object);
	while ((tmp = s_xml_node_get_path(node, "style")) != NULL) {
		FRAME_SHAPE fshape;
		FRAME_SHADOW fshadow;
		code_get_style(htable, tmp, &fshape, &fshadow);
		w_checkbox_set_style(checkbox->object, fshape, fshadow); 
		tmp->dontparse = 1;
	}
	if ((tmp = s_xml_node_get_path(node, "boxstyle")) != NULL) {
		FRAME_SHAPE fshape;
		FRAME_SHADOW fshadow;
		code_get_style(htable, tmp, &fshape, &fshadow);
		w_checkbox_set_boxstyle(checkbox->object, fshape, fshadow); 
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "image")) != NULL) {
		char **var;
		unsigned int count;
		unsigned int style;
		unsigned int rotate;
		code_get_image(htable, tmp, &style, &rotate, &count, &var);
		if (var != NULL) {
			w_checkbox_set_image(checkbox->object, style, rotate, count, var);
			while (count--) s_free(var[count]);
			s_free(var);
		}
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "boximage")) != NULL) {
		char **var;
		unsigned int count;
		unsigned int style;
		unsigned int rotate;
		code_get_image(htable, tmp, &style, &rotate, &count, &var);
		if (var != NULL) {
			w_checkbox_set_boximage(checkbox->object, style, rotate, count, var);
			while (count--) s_free(var[count]);
			s_free(var);
		}
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "properties")) != NULL) {
		TEXTBOX_PROPERTIES prop;
		code_get_enum(htable, tmp->value, &prop);
		w_checkbox_set_properties(checkbox->object, prop);
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "size")) != NULL) {
		int size = atoi(tmp->value);
		w_checkbox_set_size(checkbox->object, size);
		tmp->dontparse = 1;
	}
	if ((tmp = s_xml_node_get_path(node, "color")) != NULL) {
		int cr = atoi(s_xml_node_get_path_value(tmp, "red"));
		int cg = atoi(s_xml_node_get_path_value(tmp, "green"));
		int cb = atoi(s_xml_node_get_path_value(tmp, "blue"));
		w_checkbox_set_rgb(checkbox->object, cr, cg, cb);
		if ((tmp = s_xml_node_get_path(node, "color/red")) != NULL) { tmp->dontparse = 1; }
		if ((tmp = s_xml_node_get_path(node, "color/green")) != NULL) { tmp->dontparse = 1; }
		if ((tmp = s_xml_node_get_path(node, "color/blue")) != NULL) { tmp->dontparse = 1; }
		tmp->dontparse = 1;
	}
	if ((tmp = s_xml_node_get_path(node, "changed")) != NULL) {
		tmp->dontparse = 1;
	}
	if ((tmp = s_xml_node_get_path(node, "string")) != NULL) {
		w_checkbox_set_str(checkbox->object, code_trim_quota(tmp->value));
		tmp->dontparse = 1;
	}
}

void code_generate_object_progressbar (s_hashtable_t *htable, s_xml_node_t *node)
{
	s_xml_node_t *tmp;
	w_progressbar_t *progressbar;
	w_object_t *pobject;
	w_object_t *wobject;
	s_xml_node_t *window = s_xml_node_get_parent(node, "window");
	wobject = (w_object_t *) s_hashtable_get_data(htable, s_xml_node_get_attr_value(window, "id"));
	pobject = (w_object_t *) s_hashtable_get_data(htable, s_xml_node_get_attr_value(node->parent, "id"));
	w_progressbar_init(wobject->window, &progressbar, pobject);
	s_hashtable_add(htable, s_xml_node_get_attr_value(node, "id"), progressbar->object);
	while ((tmp = s_xml_node_get_path(node, "style")) != NULL) {
		FRAME_SHAPE fshape;
		FRAME_SHADOW fshadow;
		code_get_style(htable, tmp, &fshape, &fshadow);
		w_progressbar_set_style(progressbar->object, fshape, fshadow); 
		tmp->dontparse = 1;
	}
	if ((tmp = s_xml_node_get_path(node, "boxstyle")) != NULL) {
		FRAME_SHAPE fshape;
		FRAME_SHADOW fshadow;
		code_get_style(htable, tmp, &fshape, &fshadow);
		w_progressbar_set_boxstyle(progressbar->object, fshape, fshadow); 
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "image")) != NULL) {
		char **var;
		unsigned int count;
		unsigned int style;
		unsigned int rotate;
		code_get_image(htable, tmp, &style, &rotate, &count, &var);
		if (var != NULL) {
			w_progressbar_set_image(progressbar->object, style, rotate, count, var);
			while (count--) s_free(var[count]);
			s_free(var);
		}
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "boximage")) != NULL) {
		tmp->dontparse = 1;
	}
	if ((tmp = s_xml_node_get_path(node, "changed")) != NULL) {
		tmp->dontparse = 1;
	}
}

void code_generate_object_scrollbuffer (s_hashtable_t *htable, s_xml_node_t *node)
{
	s_xml_node_t *tmp;
	w_scrollbuffer_t *scrollbuffer;
	w_object_t *pobject;
	w_object_t *wobject;
	s_xml_node_t *window = s_xml_node_get_parent(node, "window");
	wobject = (w_object_t *) s_hashtable_get_data(htable, s_xml_node_get_attr_value(window, "id"));
	pobject = (w_object_t *) s_hashtable_get_data(htable, s_xml_node_get_attr_value(node->parent, "id"));
	w_scrollbuffer_init(wobject->window, &scrollbuffer, pobject);
	s_hashtable_add(htable, s_xml_node_get_attr_value(node, "id"), scrollbuffer->object);
	while ((tmp = s_xml_node_get_path(node, "object")) != NULL) {
		code_parse_generate(htable, tmp);
		tmp->dontparse = 1;
	}
	if ((tmp = s_xml_node_get_path(node, "child")) != NULL) {
		w_object_t *cobject = (w_object_t *) s_hashtable_get_data(htable, tmp->value);
		w_scrollbuffer_set_child(scrollbuffer->object, cobject);
		tmp->dontparse = 1;
	}
	if ((tmp = s_xml_node_get_path(node, "slide")) != NULL) {
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "boxstyle")) != NULL) {
		FRAME_SHAPE fshape;
		FRAME_SHADOW fshadow;
		code_get_style(htable, tmp, &fshape, &fshadow);
		w_scrollbuffer_set_boxstyle(scrollbuffer->object, fshape, fshadow); 
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "boximage")) != NULL) {
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "addstyle")) != NULL) {
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "addimage")) != NULL) {
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "substyle")) != NULL) {
		tmp->dontparse = 1;
	}
	while ((tmp = s_xml_node_get_path(node, "subimage")) != NULL) {
		tmp->dontparse = 1;
	}
}

void code_generate_object (s_hashtable_t *htable, s_xml_node_t *node)
{
	char *type = s_xml_node_get_attr_value(node, "type");
	if (strcmp(type, "frame") == 0) {
		code_generate_object_frame(htable, node);
	} else if (strcmp(type, "button") == 0) {
		code_generate_object_button(htable, node);
	} else if (strcmp(type, "textbox") == 0) {
		code_generate_object_textbox(htable, node);
	} else if (strcmp(type, "editbox") == 0) {
		code_generate_object_editbox(htable, node);
	} else if (strcmp(type, "checkbox") == 0) {
		code_generate_object_checkbox(htable, node);
	} else if (strcmp(type, "progressbar") == 0) {
		code_generate_object_progressbar(htable, node);
	} else if (strcmp(type, "scrollbuffer") == 0) {
		code_generate_object_scrollbuffer(htable, node);
	}
}

void code_parse_generate (s_hashtable_t *htable, s_xml_node_t *node)
{
	int p;
	s_xml_node_t *tmp;
	if (node->dontparse != 0) {
		return;
	}
	if (strcmp(node->name, "window") == 0) {
		code_generate_window(htable, node);
	} else if (strcmp(node->name, "object") == 0) {
		code_generate_object(htable, node);
	} else if (strcmp(node->name, "move") == 0) {
		code_generate_move(htable, node);
	} else if (strcmp(node->name, "show") == 0) {
		code_generate_show(htable, node);
	}
	p = 0;
	while (!s_list_eol(node->nodes, p)) {
		tmp = (s_xml_node_t *) s_list_get(node->nodes, p);
		code_parse_generate(htable, tmp);
		p++;
	}
}

void code_parse (s_hashtable_t *htable, s_xml_node_t *file, s_xml_node_t *style, char *script, char *engine)
{
	code_script_t **sengine;
	
	s_hashtable_add(htable, "WINDOW_NOFORM", (void *) WINDOW_NOFORM);
	s_hashtable_add(htable, "WINDOW_MAIN", (void *) WINDOW_MAIN);
	s_hashtable_add(htable, "WINDOW_TEMP", (void *) WINDOW_TEMP);
	s_hashtable_add(htable, "WINDOW_CHILD", (void *) WINDOW_CHILD);
	s_hashtable_add(htable, "WINDOW_DESKTOP", (void *) WINDOW_DESKTOP);

	s_hashtable_add(htable, "FRAME_NOFRAME", (void *) FRAME_NOFRAME);
	s_hashtable_add(htable, "FRAME_BOX", (void *) FRAME_BOX);
	s_hashtable_add(htable, "FRAME_PANEL", (void *) FRAME_PANEL);
	s_hashtable_add(htable, "FRAME_WINPANEL", (void *) FRAME_WINPANEL);
	s_hashtable_add(htable, "FRAME_HLINE", (void *) FRAME_HLINE);
	s_hashtable_add(htable, "FRAME_VLINE", (void *) FRAME_VLINE);
	s_hashtable_add(htable, "FRAME_STYLEDPANEL", (void *) FRAME_STYLEDPANEL);
	s_hashtable_add(htable, "FRAME_POPUPPANEL", (void *) FRAME_POPUPPANEL);
	s_hashtable_add(htable, "FRAME_MENUBARPANEL", (void *) FRAME_MENUBARPANEL);
	s_hashtable_add(htable, "FRAME_TOOLBARPANEL", (void *) FRAME_TOOLBARPANEL);
	s_hashtable_add(htable, "FRAME_LINEEDITPANEL", (void *) FRAME_LINEEDITPANEL);
	s_hashtable_add(htable, "FRAME_TABWIDGETPANEL", (void *) FRAME_TABWIDGETPANEL);
	s_hashtable_add(htable, "FRAME_GROUPBOXPANEL", (void *) FRAME_GROUPBOXPANEL);
	s_hashtable_add(htable, "FRAME_EMPTY", (void *) FRAME_EMPTY);
	s_hashtable_add(htable, "FRAME_PLAIN", (void *) FRAME_PLAIN);
	s_hashtable_add(htable, "FRAME_RAISED", (void *) FRAME_RAISED);
	s_hashtable_add(htable, "FRAME_SUNKEN", (void *) FRAME_SUNKEN);
	s_hashtable_add(htable, "FRAME_FOCUSED", (void *) FRAME_FOCUSED);

	s_hashtable_add(htable, "TEXTBOX_WRAP", (void *) TEXTBOX_WRAP);
	s_hashtable_add(htable, "TEXTBOX_VCENTER", (void *) TEXTBOX_VCENTER);
	s_hashtable_add(htable, "TEXTBOX_HCENTER", (void *) TEXTBOX_HCENTER);
	
	s_hashtable_add(htable, "FRAME_IMAGE_SOLID", (void *) FRAME_IMAGE_SOLID);
	s_hashtable_add(htable, "FRAME_IMAGE_VERTICAL", (void *) FRAME_IMAGE_VERTICAL);
	s_hashtable_add(htable, "FRAME_IMAGE_HORIZONTAL", (void *) FRAME_IMAGE_HORIZONTAL);

	s_hashtable_add(htable, "EFFECT_NONE", (void *) EFFECT_NONE);
	s_hashtable_add(htable, "EFFECT_FADEIN", (void *) EFFECT_FADEIN);
	s_hashtable_add(htable, "EFFECT_FADEOUT", (void *) EFFECT_FADEOUT);
	s_hashtable_add(htable, "EFFECT_POPIN", (void *) EFFECT_POPIN);
	s_hashtable_add(htable, "EFFECT_POPOUT", (void *) EFFECT_POPOUT);

	for (sengine = code_scripts; *sengine && engine; sengine++) {
		if (strcmp((*sengine)->name, engine) == 0) {
			g_engine = *sengine;
		} 
	}
	if (g_engine) g_engine->init(htable, script);	

	code_parse_element(file, style);
	code_parse_generate(htable, file);
	
	if (g_engine) g_engine->uninit();
}
