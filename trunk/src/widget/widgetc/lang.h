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

#ifndef LANG_H_
#define LANG_H_

typedef struct lang_header_s {
	char magic[8];
	unsigned long int count;
} lang_header_t;

typedef struct lang_s {
	unsigned long int hash;
	unsigned long int id_len;
	unsigned long int str_len;
	unsigned long int id_offset;
	unsigned long int str_offset;
} lang_t;

typedef struct lmsg_s {
	char *id;
	char *str;
} lmsg_t;

void lang_generate (node_t *node, char *file);

#endif /*LANG_H_*/
