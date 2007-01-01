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

#ifndef GEN_CODE_H_
#define GEN_CODE_H_

void node_generate_code_style (node_t *node, node_t *parent, char *prefix);
void node_generate_code_image (node_t *node, node_t *parent, char *prefix);
void node_generate_code_move (node_t *node);
void node_generate_code_window (node_t *node);
void node_generate_code_object_frame (node_t *node);
void node_generate_code_object_button (node_t *node);
void node_generate_code_object_textbox (node_t *node);
void node_generate_code_object_editbox (node_t *node);
void node_generate_code_object_checkbox (node_t *node);
void node_generate_code_object_progressbar (node_t *node);
void node_generate_code_object_scrollbuffer (node_t *node);
void node_generate_code_effect (node_t *node);
void node_generate_code_show (node_t *node);
void node_generate_code_draw (node_t *node);
void node_generate_code_object (node_t *node);
void node_generate_code (node_t *node, FILE *g_source, char *g_name);

#endif /*GEN_CODE_H_*/
