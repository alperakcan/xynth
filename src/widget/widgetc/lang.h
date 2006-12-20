#ifndef LANG_H_
#define LANG_H_

#include "node.h"

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
