#ifndef GEN_HEADER_H_
#define GEN_HEADER_H_

#include "node.h"

void node_generate_header (node_t *node, FILE *g_header);
void node_generate_function (node_t *node, FILE *g_header);

#endif /*GEN_HEADER_H_*/
