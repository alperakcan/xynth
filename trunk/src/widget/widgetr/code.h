#ifndef CODE_H_
#define CODE_H_

#include "table.h"

void code_parse_element (node_t *node, node_t *elem);
void code_parse (w_table_t *table, unsigned int depth, unsigned int mask, node_t *node, node_t *elem);

#endif /*CODE_H_*/
