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

#ifndef GEN_CODE_H_
#define GEN_CODE_H_

void code_generate_style (s_xml_node_t *node, s_xml_node_t *parent, char *prefix);
void code_generate_image (s_xml_node_t *node, s_xml_node_t *parent, char *prefix);
void code_generate_move (s_xml_node_t *node);
void code_generate_window (s_xml_node_t *node);
void code_generate_object_frame (s_xml_node_t *node);
void code_generate_object_button (s_xml_node_t *node);
void code_generate_object_textbox (s_xml_node_t *node);
void code_generate_object_editbox (s_xml_node_t *node);
void code_generate_object_checkbox (s_xml_node_t *node);
void code_generate_object_progressbar (s_xml_node_t *node);
void code_generate_object_scrollbuffer (s_xml_node_t *node);
void code_generate_effect (s_xml_node_t *node);
void code_generate_show (s_xml_node_t *node);
void code_generate_draw (s_xml_node_t *node);
void code_generate_object (s_xml_node_t *node);
void code_generate (s_xml_node_t *node, FILE *g_source, char *g_name);

#endif /*GEN_CODE_H_*/
