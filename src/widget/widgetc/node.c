
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"

char * node_strdup (char *str)
{
	if (str == NULL) {
		return NULL;
	} else {
		return strdup(str);
	}
}

char * node_string_normalize (char *str_param)
{
	int i;
	int j;
	int s;
	char *out = str_param;
	for (s = 0, i = 0; out[i]; i++){
		if (out[i] == '"') {
			if (i == 0 ||
			    out[i - 1] != '\\') {
			    	s = (s + 1) % 2;
			    	if (s == 0) {
			    		while (out[i + 1] && out[i + 1] != '"') {
						for (j = i + 1; out[j]; j++) {
							out[j] = out[j + 1];
						}
			    		}
			    	}
			}
		}
	}
	/* Remove leading " */
	while (out[0] == '"') {
		for (j = 0; out[j]; j++) {
			out[j] = out[j + 1];
		}
	}
	/* Remove lonely " 's */
	for (i = 0; out[i + 1]; i++) {
		if (out[i] != '\\' && out[i + 1] == '"') {
			for (j = i + 1; out[j]; j++)
				out[j] = out[j + 1];
			i--;
		}
	}
	/* Convert \" 's to " */
	for (i = 0; out[i + 1]; i++) {
		if (out[i] == '\\' && out[i + 1] == '"') {
			for (j = i; out[j]; j++)
				out[j] = out[j + 1];
			i--;
		}
	}
	return out;
}

int node_path_normalize (char *out, int len)
{
	int i;
	int j;
	int first;
	int next;
	
	/* First append "/" to make the rest easier */
	if (!strncat(out,"/",len))
		return -10;

	/* Convert "//" to "/" */
	for (i = 0; out[i + 1]; i++) {
		if (out[i] == '/' && out[i + 1] == '/') {
			for (j = i + 1; out[j]; j++)
				out[j] = out[j + 1];
			i--;
		}
	}

	/* Convert "/./" to "/" */
	for (i = 0; out[i] && out[i + 1] && out[i + 2]; i++) {
		if (out[i] == '/' && out[i + 1] == '.' && out[i + 2] == '/') {
			for (j = i + 1; out[j]; j++)
				out[j] = out[j + 2];
			i--;
		}
	}

	/* Convert "/asdf/../" to "/" until we can't anymore.  Also
	 * convert leading "/../" to "/" */
	first = next = 0;
	while (1) {
		/* If a "../" follows, remove it and the parent */
		if (out[next + 1] && out[next + 1] == '.' && 
		    out[next + 2] && out[next + 2] == '.' &&
		    out[next + 3] && out[next + 3] == '/') {
			for (j = 0; out[first + j + 1]; j++)
				out[first + j + 1] = out[next + j + 4];
			first = next = 0;
			continue;
		}

		/* Find next slash */
		first = next;
		for (next = first + 1; out[next] && out[next] != '/'; next++)
			continue;
		if (!out[next])
			break;
	}
	
	/* Remove trailing "/" */
	for (i = 1; out[i]; i++)
		continue;
	if (i >= 1 && out[i - 1] == '/') 
		out[i - 1] = 0;

	return 0;
}

void node_init (node_t **node)
{
	node_t *n;
	n = (node_t *) malloc(sizeof(node_t));
	memset(n, 0, sizeof(node_t));
	list_init(&(n->nodes));
	*node = n;
}

void node_uninit (node_t *node)
{
	node_t *tmp;
	if (node == NULL) {
		return;
	}
	while (!list_eol(node->nodes, 0)) {
		tmp = (node_t *) list_get(node->nodes, 0);
		list_remove(node->nodes, 0);
		node_uninit(tmp);
	}
	list_uninit(node->nodes);
	free(node->id);
	free(node->name);
	free(node->type);
	free(node->value);
	free(node);
}

void node_dublicate_ (node_t *node, node_t *dub)
{
	int p;
	node_t *tmp;
	node_t *dmp;
	dub->id = node_strdup(node->id);
	dub->name = node_strdup(node->name);
	dub->type = node_strdup(node->type);
	dub->value = node_strdup(node->value);
	for (p = 0; !list_eol(node->nodes, p); p++) {
    		tmp = (node_t *) list_get(node->nodes, p);
    		node_init(&dmp);
    		node_dublicate_(tmp, dmp);
    		list_add(dub->nodes, dmp, -1);
    		dmp->parent = dub;
	}
}

void node_dublicate (node_t *node, node_t **dub)
{
	node_init(dub);
	node_dublicate_(node, *dub);
}

node_t * node_get_parent (node_t *node, char *name)
{
	while (node->parent) {
		if (strcmp(node->parent->name, name) == 0) {
			return node->parent;
		}
		node = node->parent;
	}
	return NULL;
}

node_t * node_get_node_ (node_t *node, char *path)
{
	int p;
	char *ptr;
	char *str;
	node_t *res;
	node_t *tmp;
	p = 0;
	res = NULL;
	while (!list_eol(node->nodes, p)) {
		tmp = (node_t *) list_get(node->nodes, p);
		str = strchr(path,  '/');
		if (str == NULL) {
			if (strcmp(tmp->name, path) == 0 &&
			    tmp->dontparse != 1) {
				res = tmp;
				break;
			}
		} else {
			if (strncmp(tmp->name, path, str - path) == 0 &&
			    strlen(tmp->name) == str - path) {
			    	ptr = strchr(path, '/');
			    	if (ptr != NULL) {
			    		ptr++;
			    	}
			    	if ((res = node_get_node_(tmp, ptr)) != NULL) {
			    		break;
			    	}
			}
		}
		p++;
	}
	return res;
}

node_t * node_get_node (node_t *node, char *path)
{
	int len;
	char *str;
	node_t *res;
	len = strlen(path);
	str = (char *) malloc(len + 10 + 1);
	memset(str, 0, len + 10 + 1);
	memcpy(str, path, len);
	node_path_normalize(str, len + 10);
	if (str[0] == '/') {
		while (node->parent) {
			node = node->parent;
		}
	}
	res = node_get_node_(node, str);
	free(str);
	return res;
}

char * node_get_value (node_t *node, char *path)
{
	node_t *res;
	res = node_get_node(node, path);
	if (res != NULL) {
		return res->value;
	}
	return NULL;
}

void node_print_node (node_t *node, unsigned int *depth)
{
	int i;
	for (i = 0; i < *depth; i++) {
		printf("  ");
	}
	printf("%s : ", node->name);
	if (node->value) {
		printf("%s ", node->value);
	}
	if (node->id) {
		printf("[%s] ", node->id);
	}
	if (node->type) {
		printf("[%s] ", node->type);
	}
	printf("\n");
}

void node_print_ (node_t *node, unsigned int *depth)
{
	int p;
	node_t *tmp;
	node_print_node(node, depth);
	(*depth)++;
	p = 0;
	while (!list_eol(node->nodes, p)) {
		tmp = (node_t *) list_get(node->nodes, p);
		node_print_(tmp, depth);
		p++;
	}
	(*depth)--;
}

void node_print (node_t *node)
{
	unsigned int depth = 0;
	node_print_(node, &depth);
}
