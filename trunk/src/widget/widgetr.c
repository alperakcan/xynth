
#include "../lib/xynth_.h"
#include "widget.h"
#include <getopt.h>

typedef struct w_node_s w_node_t;
typedef struct w_table_s w_table_t;
typedef struct w_widgetr_s w_widgetr_t;

struct w_node_s {
	unsigned long int key;
	char *name;
	w_node_t *next;
	w_node_t *prev;
	w_table_t *table;
	void *data;
};

struct w_table_s {
	w_table_t *table;
	w_node_t *node;
};

struct w_widgetr_s {
	unsigned int depth;
	unsigned int mask;
	w_table_t *table;
};

int axtoi (char *hex)
{
	int n = 0;
	int m = 0;
	int count;
	int intvalue = 0;
	int digit[5];
	while (n < 4) {
		if (hex[n]=='\0')
			break;
		if (hex[n] > 0x29 && hex[n] < 0x40)
			digit[n] = hex[n] & 0x0f;
		else if (hex[n] >='a' && hex[n] <= 'f')
			digit[n] = (hex[n] & 0x0f) + 9;
		else if (hex[n] >='A' && hex[n] <= 'F')
			digit[n] = (hex[n] & 0x0f) + 9;
		else
			break;
		n++;
	}
	count = n;
	m = n - 1;
	n = 0;
	while(n < count) {
		intvalue = intvalue | (digit[n] << (m << 2));
		m--;
		n++;
	}
	return (intvalue);
}

int cmp_string (char *str, char *ptr)
{
	while (*str && *ptr) {
		if (*str++ != *ptr++) {
			return -1;
		}
	}
	if (*str || *ptr) {
		return -1;
	}
	return 0;
}

unsigned long int hash_string (const char *str_param)
{
	#define HASHWORDBITS 32
	unsigned long int hval, g;
	const char *str = str_param;
	hval = 0;
	while (*str != '\0') {
		hval <<= 4;
		hval += (unsigned char) *str++;
		g = hval & ((unsigned long int) 0xf << (HASHWORDBITS - 4));
		if (g != 0) {
			hval ^= g >> (HASHWORDBITS - 8);
			hval ^= g;
		}
	}
	return hval;
}

int table_add (w_table_t *table, unsigned int tlen, unsigned int mask, char *name, void *data)
{
	w_node_t *node;
	w_table_t *tmp;
	unsigned int o;
	unsigned int l;
	unsigned int mlen;
	unsigned long int h;
	for (l = 32; l > 0; l--) {
		if (mask & (1 << (l - 1))) {
			break;
		}
	}
	mlen = l;
	h = hash_string(name);
	tmp = table;
	for (l = 0; l < tlen; l++) {
		o = h & mask;
		if (tmp[o].table == NULL) {
			tmp[o].table = (w_table_t *) s_malloc(sizeof(w_table_t) * (mask + 1));
			memset(tmp[o].table, 0, sizeof(w_table_t) * (mask + 1));
		}
		tmp = tmp[o].table;
		h >>= mlen;
		if (h == 0) {
			break;
		}
	}
	node = (w_node_t *) s_malloc(sizeof(w_node_t));
	memset(node, 0, sizeof(w_node_t));
	node->key = h;
	h &= mask;
	node->name = strdup(name);
	node->data = data;
	node->table = &tmp[h];
	node->next = tmp[h].node;
	if (node->next) {
		node->next->prev = node;
	}
	tmp[h].node = node;
	return 0;
}

w_node_t * table_get_node (w_table_t *table, unsigned int tlen, unsigned int mask, char *name)
{
	w_node_t *node;
	w_table_t *tmp;
	unsigned int o;
	unsigned int l;
	unsigned int mlen;
	unsigned long int h;
	for (l = 32; l > 0; l--) {
		if (mask & (1 << (l - 1))) {
			break;
		}
	}
	mlen = l;
	h = hash_string(name);
	tmp = table;
	for (l = 0; l < tlen; l++) {
		o = h & mask;
		if (tmp[o].table == NULL) {
			return NULL;
		}
		tmp = tmp[o].table;
		h >>= mlen;
		if (h == 0) {
			break;
		}
	}
	node = tmp[h & mask].node;
	while (node) {
		if (node->key == h &&
		    cmp_string(node->name, name) == 0) {
			return node;
		}
		node = node->next;
	}
	return NULL;
}

void * table_get_data (w_table_t *table, unsigned int tlen, unsigned int mask, char *name)
{
	w_node_t *node;
	node = table_get_node(table, tlen, mask, name);
	if (node) {
		return node->data;
	} else {
		return NULL;
	} 
}

int table_del (w_table_t *table, unsigned int tlen, unsigned int mask, char *name)
{
	w_node_t *node;
	node = table_get_node(table, tlen, mask, name);
	if (node == NULL) {
		return -1;
	}
	if (node->prev) {
		node->prev->next = node->next;
	} else {
		node->table->node = node->next;
	}
	s_free(node->name);
	s_free(node);
	return 0;
}

int table_print (w_table_t *table, unsigned int len)
{
	unsigned int i;
	w_node_t *node;
	if (table == NULL) {
		return -1;
	}
	for (i = 0; i < len; i++) {
		table_print(table[i].table, len);
		node = table[i].node;
		while (node) {
			printf("%s\n", node->name);
			node = node->next;
		}
	}
	return 0;
}

int table_init (w_table_t **table, unsigned int size)
{
	w_table_t *tbl;
	tbl = (w_table_t *) s_malloc(sizeof(w_table_t) * size);
	memset(tbl, 0, sizeof(w_table_t) * size);
	*table = tbl;
	return 0;
}

int table_uninit (w_table_t *table, unsigned int len)
{
	w_node_t *tmp;
	w_node_t *node;
	unsigned int i;
	if (table == NULL) {
		return -1;
	}
	for (i = 0; i < len; i++) {
		table_uninit(table[i].table, len);
		node = table[i].node;
		while (node) {
			tmp = node;
			node = node->next;
			s_free(tmp->name);
			s_free(tmp);
		}
	}
	s_free(table);
	return 0;
}

int main (int argc, char *argv[])
{
	unsigned int i;
	w_widgetr_t *widgetr;

	widgetr = (w_widgetr_t *) s_malloc(sizeof(w_widgetr_t));
	memset(widgetr, 0, sizeof(w_widgetr_t));
	
	{
		char c;
		char *hex;
		unsigned int option_index = 0;
		struct option long_options[] = {
			{"tables", 1, 0, 0},
			{"mask", 1, 0, 0},
			{"help", 0, 0, 0},
			{0, 0, 0, 0},
		};
		while ((c = getopt_long(argc, argv, "t:m:h", long_options, &option_index)) != -1) {
			switch (c) {
				case 0:
					if (strcmp("tables", long_options[option_index].name) == 0) {
						goto option_tables;
					} else if (strcmp("mask", long_options[option_index].name) == 0) {
						goto option_mask;
					} else if (strcmp("help", long_options[option_index].name) == 0) {
						goto option_help;
					}
					break;
				case 'm':
option_mask:
					if ((hex = strstr(optarg, "0x")) != NULL) {
						widgetr->mask = axtoi(hex + 2);
					} else {
						widgetr->mask = atoi(optarg);
					}
					break;
				case 't':
option_tables:
					widgetr->depth = atoi(optarg);
					break;
				case 'h':
option_help:
					printf("%s usage;\n"
					       "\t-t / --tables : tables depth\n"
					       "\t-m / --mask   : bit mask\n",
					       argv[0]);
					exit(1);
			}
		}
	}
	
	if (widgetr->depth == 0) {
		widgetr->depth = 4;
	}
	if (widgetr->mask == 0) {
		widgetr->mask = 0x0f;
	}
	
	for (i = 32; i > 0; i--) {
		if (widgetr->mask & (1 << (i - 1))) {
			break;
		}
	}
	printf("%s:\n"
	       "\ttable depth : %u\n"
	       "\ttable mask  : 0x%08x\n"
	       "\tmask length : %u\n",
	       argv[0], widgetr->depth, widgetr->mask, i);
	
	table_init(&widgetr->table, widgetr->mask + 1);

	for (i = 1; i < argc; i++) {
		table_add(widgetr->table, widgetr->depth, widgetr->mask, argv[i], NULL);
	}
	
	table_print(widgetr->table, widgetr->mask + 1);
	
	table_get_data(widgetr->table, widgetr->depth, widgetr->mask, "to");
	table_del(widgetr->table, widgetr->depth, widgetr->mask, "alper");
	table_get_data(widgetr->table, widgetr->depth, widgetr->mask, "to");
	
	table_uninit(widgetr->table, widgetr->mask + 1);
	s_free(widgetr);
	
	return 0;
}
