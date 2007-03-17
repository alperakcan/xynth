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
	int (*init) (ctable_t *ctable, char *script);
	void (*uninit) (void);
} code_script_t;

char * code_trim_quota (char *value);
char * code_trim_space (char *value);
void code_tokenize (char *value, char token, int *n, char ***tokens);
void code_get_enum (ctable_t *ctable, char *val, unsigned int *prop);
void code_get_style (ctable_t *ctable, s_xml_node_t *node, FRAME_SHAPE *fshape, FRAME_SHADOW *fshadow);
void code_get_image (ctable_t *ctable, s_xml_node_t *node, unsigned int *istyle, unsigned int *irotate, unsigned int *icount, char ***ivar);
void code_parse_element (s_xml_node_t *file, s_xml_node_t *style);
void code_generate_move (ctable_t *ctable, s_xml_node_t *node);
void code_generate_window (ctable_t *ctable, s_xml_node_t *node);
void code_generate_show (ctable_t *ctable, s_xml_node_t *node);
void code_generate_object_frame (ctable_t *ctable, s_xml_node_t *node);
void code_generate_object_button (ctable_t *ctable, s_xml_node_t *node);
void code_generate_object_textbox (ctable_t *ctable, s_xml_node_t *node);
void code_generate_object_editbox (ctable_t *ctable, s_xml_node_t *node);
void code_generate_object_checkbox (ctable_t *ctable, s_xml_node_t *node);
void code_generate_object_progressbar (ctable_t *ctable, s_xml_node_t *node);
void code_generate_object_scrollbuffer (ctable_t *ctable, s_xml_node_t *node);
void code_generate_object (ctable_t *ctable, s_xml_node_t *node);
void code_parse_generate (ctable_t *ctable, s_xml_node_t *node);
void code_parse (w_table_t *table, unsigned int depth, unsigned int mask, s_xml_node_t *file, s_xml_node_t *style, char *script, char *engine);

#endif /*CODE_H_*/
