/***************************************************************************
    begin                : Wed Dec 20 2006
    copyright            : (C) 2006 - 2008 by Alper Akcan
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

#ifndef CODE_H_
#define CODE_H_

typedef struct code_priv_s {
	void *draw;
	char *pressed;
	char *released;
	char *clicked;
} code_priv_t;

typedef struct code_script_s {
	char *name;
	void (*object_draw) (w_object_t *object);
	void (*button_pressed) (w_object_t *object, int button);
	void (*button_released) (w_object_t *object, int button);
	void (*button_clicked) (w_object_t *object, int button, int click);
	int (*init) (s_hashtable_t *htable, char *script);
	void (*uninit) (void);
} code_script_t;

char * code_trim_quota (char *value);
char * code_trim_space (char *value);
void code_tokenize (char *value, char token, int *n, char ***tokens);
void code_get_enum (s_hashtable_t *htable, char *val, unsigned int *prop);
void code_get_style (s_hashtable_t *htable, s_xml_node_t *node, FRAME_SHAPE *fshape, FRAME_SHADOW *fshadow);
void code_get_image (s_hashtable_t *htable, s_xml_node_t *node, unsigned int *istyle, unsigned int *irotate, unsigned int *icount, char ***ivar);
void code_parse_element (s_xml_node_t *file, s_xml_node_t *style);
void code_generate_move (s_hashtable_t *htable, s_xml_node_t *node);
void code_generate_window (s_hashtable_t *htable, s_xml_node_t *node);
void code_generate_show (s_hashtable_t *htable, s_xml_node_t *node);
void code_generate_object_frame (s_hashtable_t *htable, s_xml_node_t *node);
void code_generate_object_button (s_hashtable_t *htable, s_xml_node_t *node);
void code_generate_object_textbox (s_hashtable_t *htable, s_xml_node_t *node);
void code_generate_object_editbox (s_hashtable_t *htable, s_xml_node_t *node);
void code_generate_object_checkbox (s_hashtable_t *htable, s_xml_node_t *node);
void code_generate_object_progressbar (s_hashtable_t *htable, s_xml_node_t *node);
void code_generate_object_scrollbuffer (s_hashtable_t *htable, s_xml_node_t *node);
void code_generate_object (s_hashtable_t *htable, s_xml_node_t *node);
void code_parse_generate (s_hashtable_t *htable, s_xml_node_t *node);
void code_parse (s_hashtable_t *htable, s_xml_node_t *file, s_xml_node_t *style, char *script, char *engine);

#endif /*CODE_H_*/
