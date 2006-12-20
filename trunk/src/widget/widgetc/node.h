#ifndef NODE_H_
#define NODE_H_

#include "list.h"

typedef struct node_s {
	char *name;
	char *id;
	char *type;
	char *value;
	list_t *nodes;
	struct node_s *parent;
	int dontparse;
} node_t;

char * node_strdup (char *str);
char * node_string_normalize (char *str_param);
int node_path_normalize (char *out, int len);
void node_init (node_t **node);
void node_uninit (node_t *node);
void node_dublicate_ (node_t *node, node_t *dub);
void node_dublicate (node_t *node, node_t **dub);
node_t * node_get_parent (node_t *node, char *name);
node_t * node_get_node_ (node_t *node, char *path);
node_t * node_get_node (node_t *node, char *path);
char * node_get_value (node_t *node, char *path);
void node_print_node (node_t *node, unsigned int *depth);
void node_print_ (node_t *node, unsigned int *depth);
void node_print (node_t *node);

#endif /*NODE_H_*/
