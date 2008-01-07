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

#ifndef GEN_HEADER_H_
#define GEN_HEADER_H_

void header_generate (s_xml_node_t *node, FILE *g_header);
void header_generate_function (s_xml_node_t *node, FILE *g_header);

#endif /*GEN_HEADER_H_*/
