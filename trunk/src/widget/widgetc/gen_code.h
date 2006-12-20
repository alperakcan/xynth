#ifndef GEN_CODE_H_
#define GEN_CODE_H_

#include "node.h"

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
