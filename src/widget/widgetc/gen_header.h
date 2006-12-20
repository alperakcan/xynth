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

#ifndef GEN_HEADER_H_
#define GEN_HEADER_H_

#include "node.h"

void node_generate_header (node_t *node, FILE *g_header);
void node_generate_function (node_t *node, FILE *g_header);

#endif /*GEN_HEADER_H_*/
