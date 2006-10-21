
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <expat.h>
#include <xynth.h>
#include <widget.h>

#define buffSIZE	8192

char buff[buffSIZE];

typedef struct node_s {
	char *name;
	char *id;
	char *type;
	char *value;
	s_list_t *nodes;
	struct node_s *parent;
} node_t;

typedef struct data_s {
	node_t *node;
	int depth;
	unsigned int objs[OBJECT_OBJECTS];
	char *path;
	node_t *active;
} data_t;

data_t *data;

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

char * node_get_value (node_t *node, char *path)
{
	int p;
	node_t *tmp;
	p = 0;
	while (!s_list_eol(node->nodes, p)) {
		tmp = (node_t *) s_list_get(node->nodes, p);
		if (strcmp(tmp->name, path) == 0) {
			return tmp->value;
		}
		p++;
	}
	return NULL;
}

void node_generate_code (node_t *node)
{
	int p;
	node_t *tmp;
	if (strcmp(node->name, "window") == 0) {
		printf("w_window_init(&%s, %s, NULL);\n", node->id, node->type);
	} else if (strcmp(node->name, "object") == 0) {
		if (strcmp(node->type, "frame") == 0) {
			printf("w_frame_init(%s, &%s, %s, %s->object);\n", node_get_parent(node, "window")->id, node->id, "0", node->parent->id);
		} else if (strcmp(node->type, "button") == 0) {
			printf("w_button_init(%s, &%s, %s->object);\n", node_get_parent(node, "window")->id, node->id, node->parent->id);
		} else if (strcmp(node->type, "textbox") == 0) {
			printf("w_textbox_init(%s, &%s, %s->object);\n", node_get_parent(node, "window")->id, node->id, node->parent->id);
		} else if (strcmp(node->type, "checkbox") == 0) {
			printf("w_checkbox_init(%s, &%s, %s->object);\n", node_get_parent(node, "window")->id, node->id, node->parent->id);
		}
	} else if (strcmp(node->name, "title") == 0) {
		printf("s_window_set_title(%s->window, \"%s\");\n", node_get_parent(node, "window")->id, node->value);
	} else if (strcmp(node->name, "move") == 0) {
		char *x = node_get_value(node, "x");
		char *y = node_get_value(node, "y");
		char *w = node_get_value(node, "w");
		char *h = node_get_value(node, "h");
		if (strcmp(node->parent->name, "window") == 0) {
			printf("w_window_set_coor(%s, %s, %s, %s, %s);\n", node_get_parent(node, "window")->id, (x) ? x : "0", (y) ? y : "0", (w) ? w : "w", (h) ? h : "0");
		} else if (strcmp(node->parent->name, "object") == 0) {
			printf("w_object_move(%s->object, %s, %s, %s, %s);\n", node_get_parent(node, "object")->id, (x) ? x : "0", (y) ? y : "0", (w) ? w : "w", (h) ? h : "0");
		}
	} else if (strcmp(node->name, "style") == 0) {
		char *shape = node_get_value(node, "shape");
		char *shadow = node_get_value(node, "shadow");
		if (strcmp(node->parent->type, "frame") == 0) {
			printf("%s->style = %s | %s;\n", node->parent->id, (shape) ? shape : "0" , (shadow) ? shadow : "0");
		} else if (strcmp(node->parent->type, "textbox") == 0) {
			printf("%s->frame->style = %s | %s;\n", node->parent->id, (shape) ? shape : "0" , (shadow) ? shadow : "0");
		}
	} else if (strcmp(node->name, "string") == 0) {
		if (strcmp(node->parent->type, "textbox") == 0) {
			printf("w_textbox_set_str(%s->object, \"%s\");\n", node->parent->id, node->value);
		} else if (strcmp(node->parent->type, "checkbox") == 0) {
			printf("w_textbox_set_str(%s->text->object, \"%s\");\n", node->parent->id, node->value);
		}
	} else if (strcmp(node->name, "show") == 0) {
		if (strcmp(node->parent->name, "window") == 0) {
			printf("w_object_show(%s->object);\n", node_get_parent(node, "window")->id);
			printf("s_window_show(%s->window);\n", node_get_parent(node, "window")->id);
			printf("s_client_main(%s->window);\n", node_get_parent(node, "window")->id);
		} else if (strcmp(node->parent->name, "object") == 0) {
			printf("w_object_show(%s->object);\n", node_get_parent(node, "object")->id);
		}
	} else if (strcmp(node->name, "draw") == 0) {
		printf("%s->object->draw = %s;\n", node->parent->id, node->value);
	} else if (strcmp(node->name, "pressed") == 0) {
		printf("%s->pressed = %s;\n", node->parent->id, node->value);
	}
	data->depth++;
	p = 0;
	while (!s_list_eol(node->nodes, p)) {
		tmp = (node_t *) s_list_get(node->nodes, p);
		node_generate_code(tmp);
		p++;
	}
	data->depth--;
}

void node_generate_header (node_t *node)
{
	int p;
	node_t *tmp;
	if (strcmp(node->name, "window") == 0) {
		printf("w_window_t *%s;\n", node->id);
	} else if (strcmp(node->name, "object") == 0) {
		if (strcmp(node->type, "frame") == 0) {
			printf("w_frame_t *%s;\n", node->id);
		} else if (strcmp(node->type, "button") == 0) {
			printf("w_button_t *%s;\n", node->id);
		} else if (strcmp(node->type, "textbox") == 0) {
			printf("w_textbox_t *%s;\n", node->id);
		} else if (strcmp(node->type, "checkbox") == 0) {
			printf("w_checkbox_t *%s;\n", node->id);
		}
	}
	data->depth++;
	p = 0;
	while (!s_list_eol(node->nodes, p)) {
		tmp = (node_t *) s_list_get(node->nodes, p);
		node_generate_header(tmp);
		p++;
	}
	data->depth--;
}

void node_generate_function (node_t *node)
{
	int p;
	node_t *tmp;
	if (strcmp(node->name, "draw") == 0) {
		printf("void %s (w_object_t *object);\n", node->value);
	} else if (strcmp(node->name, "pressed") == 0) {
		printf("void %s (w_object_t *object, int button);\n", node->value);
	}
	data->depth++;
	p = 0;
	while (!s_list_eol(node->nodes, p)) {
		tmp = (node_t *) s_list_get(node->nodes, p);
		node_generate_function(tmp);
		p++;
	}
	data->depth--;
}

void node_generate (node_t *node)
{
	printf("#include <stdio.h>\n"
	       "#include <stdlib.h>\n"
	       "#include <time.h>\n"
	       "#include <xynth.h>\n"
	       "#include <widget.h>\n"
	       "\n");
	node_generate_header(node);
	printf("\n");
	node_generate_function(node);
	printf("\n"
	       "int main (int argc, char *argv[])\n"
	       "{\n"
	       "srand(time(NULL));\n");
	node_generate_code(node);
	printf("return 0;\n"
	       "}\n");
}

void node_print (node_t *node)
{
	int i;
	for (i = 0; i < data->depth; i++) {
		printf("  ");
	}
	printf("%s : ", node->name);
	if (node->value) {
		printf("%s ", node->value);
	}
	if (node->type) {
		printf("[%s] ", node->type);
	}
	printf("\n");
}

void node_parse (node_t *node)
{
	int p;
	node_t *tmp;
	node_print(node);
	data->depth++;
	p = 0;
	while (!s_list_eol(node->nodes, p)) {
		tmp = (node_t *) s_list_get(node->nodes, p);
		node_parse(tmp);
		p++;
	}
	data->depth--;
}

void node_init (node_t **node)
{
	node_t *n;
	n = (node_t *) malloc(sizeof(node_t));
	memset(n, 0, sizeof(node_t));
	s_list_init(&(n->nodes));
	*node = n;
}

void start (void *xdata, const char *el, const char **attr)
{
	free(data->path);
	data->path = strdup(el);
	data->depth++;
	
	{
		int p;
		node_t *node;
		node_init(&node);
		node->name = strdup(el);
		for (p = 0; attr[p]; p += 2) {
			if (strcmp(attr[p], "type") == 0) {
				node->type = strdup(attr[p + 1]);
			} else if (strcmp(attr[p], "id") == 0) {
				node->id = strdup(attr[p + 1]);
			}
		}
		node->parent = data->active;
		data->active = node;
		if (node->parent) {
			s_list_add(node->parent->nodes, node, -1);
		} else {
			data->node = node;
		}
	}
}

void end (void *xdata, const char *el)
{
	data->depth--;
	free(data->path);
	data->path = NULL;
	data->active = data->active->parent;
}

void char_hndl (void *xdata, const char *txt, int txtlen)
{
	char *str;
	char *ptr;
	char *end;
	if (txtlen > 0 &&
	    txt &&
	    data->path) {
	} else {
	    return;
	}
	end = (char *) malloc(sizeof(char) * (strlen(data->path) + 3 + 1));
	sprintf(end, "</%s>", data->path);
	str = strdup(txt);
	ptr = str + txtlen;
	if (data->path &&
	    strncmp(ptr, end, strlen(end)) == 0) {
	    	str[txtlen] = '\0';
	    	if (data->active) {
	    		data->active->value = strdup(str);
	    	}
	}
	free(str);
}

int main (int argc, char **argv)
{
	data = (data_t *) malloc(sizeof(data_t));
	memset(data, 0, sizeof(data_t));
	memset(buff, 0, buffSIZE);
	
	XML_Parser p = XML_ParserCreate(NULL);
	if (!p) {
		fprintf(stderr, "Couldn't allocate memory for parser\n");
		exit(-1);
	}
	XML_SetElementHandler(p, start, end);
	XML_SetCharacterDataHandler(p, char_hndl);
	for (;;) {
		int done;
		int len;
		len = fread(buff, 1, buffSIZE, stdin);
		if (ferror(stdin)) {
			fprintf(stderr, "Read error\n");
			exit(-1);
		}
		done = feof(stdin);
		if (!XML_Parse(p, buff, len, done)) {
			fprintf(stderr, "Parse error at line %d:\n%s\n", XML_GetCurrentLineNumber(p), XML_ErrorString(XML_GetErrorCode(p)));
			exit(-1);
		}
		if (done)
			break;
	}
//	node_parse(data->node);
	node_generate(data->node);
	return 0;
}
