/***************************************************************************
    begin                : Wed Dec 20 2006
    copyright            : (C) 2006 by Alper Akcan
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

#include "table.h"

void code_parse_element (node_t *node, node_t *elem);
void code_parse (w_table_t *table, unsigned int depth, unsigned int mask, node_t *node, node_t *elem);

#endif /*CODE_H_*/
