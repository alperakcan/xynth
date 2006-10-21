
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <expat.h>
#include <xynth.h>
#include <widget.h>

FILE *input = NULL;
FILE *source = NULL;
FILE *header = NULL;
char *source_name = NULL;
char *header_name = NULL;

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
		fprintf(source, "w_window_init(&%s, %s, NULL);\n", node->id, node->type);
	} else if (strcmp(node->name, "object") == 0) {
		if (strcmp(node->type, "frame") == 0) {
			fprintf(source, "w_frame_init(%s, &%s, %s, %s->object);\n", node_get_parent(node, "window")->id, node->id, "0", node->parent->id);
		} else if (strcmp(node->type, "button") == 0) {
			fprintf(source, "w_button_init(%s, &%s, %s->object);\n", node_get_parent(node, "window")->id, node->id, node->parent->id);
		} else if (strcmp(node->type, "textbox") == 0) {
			fprintf(source, "w_textbox_init(%s, &%s, %s->object);\n", node_get_parent(node, "window")->id, node->id, node->parent->id);
		} else if (strcmp(node->type, "checkbox") == 0) {
			fprintf(source, "w_checkbox_init(%s, &%s, %s->object);\n", node_get_parent(node, "window")->id, node->id, node->parent->id);
		} else if (strcmp(node->type, "editbox") == 0) {
			fprintf(source, "w_editbox_init(%s, &%s, %s->object);\n", node_get_parent(node, "window")->id, node->id, node->parent->id);
		}
	} else if (strcmp(node->name, "title") == 0) {
		fprintf(source, "s_window_set_title(%s->window, \"%s\");\n", node_get_parent(node, "window")->id, node->value);
	} else if (strcmp(node->name, "move") == 0) {
		char *x = node_get_value(node, "x");
		char *y = node_get_value(node, "y");
		char *w = node_get_value(node, "w");
		char *h = node_get_value(node, "h");
		if (strcmp(node->parent->name, "window") == 0) {
			fprintf(source, "w_window_set_coor(%s, %s, %s, %s, %s);\n", node_get_parent(node, "window")->id, (x) ? x : "0", (y) ? y : "0", (w) ? w : "w", (h) ? h : "0");
		} else if (strcmp(node->parent->name, "object") == 0) {
			fprintf(source, "w_object_move(%s->object, %s, %s, %s, %s);\n", node_get_parent(node, "object")->id, (x) ? x : "0", (y) ? y : "0", (w) ? w : "w", (h) ? h : "0");
		}
	} else if (strcmp(node->name, "style") == 0) {
		char *shape = node_get_value(node, "shape");
		char *shadow = node_get_value(node, "shadow");
		if (strcmp(node->parent->type, "frame") == 0) {
			fprintf(source, "%s->style = %s | %s;\n", node->parent->id, (shape) ? shape : "0" , (shadow) ? shadow : "0");
		} else if (strcmp(node->parent->type, "textbox") == 0) {
			fprintf(source, "%s->frame->style = %s | %s;\n", node->parent->id, (shape) ? shape : "0" , (shadow) ? shadow : "0");
		}
	} else if (strcmp(node->name, "string") == 0) {
		if (strcmp(node->parent->type, "textbox") == 0 ||
		    strcmp(node->parent->type, "editbox") == 0) {
			fprintf(source, "w_textbox_set_str(%s->object, \"%s\");\n", node->parent->id, node->value);
		} else if (strcmp(node->parent->type, "checkbox") == 0) {
			fprintf(source, "w_textbox_set_str(%s->text->object, \"%s\");\n", node->parent->id, node->value);
		}
	} else if (strcmp(node->name, "show") == 0) {
		if (strcmp(node->parent->name, "window") == 0) {
			fprintf(source, "w_object_show(%s->object);\n", node_get_parent(node, "window")->id);
			fprintf(source, "s_window_show(%s->window);\n", node_get_parent(node, "window")->id);
			fprintf(source, "s_client_main(%s->window);\n", node_get_parent(node, "window")->id);
		} else if (strcmp(node->parent->name, "object") == 0) {
			fprintf(source, "w_object_show(%s->object);\n", node_get_parent(node, "object")->id);
		}
	} else if (strcmp(node->name, "draw") == 0) {
		fprintf(source, "%s->object->draw = %s;\n", node->parent->id, node->value);
	} else if (strcmp(node->name, "pressed") == 0) {
		fprintf(source, "%s->pressed = %s;\n", node->parent->id, node->value);
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
		fprintf(header, "w_window_t *%s;\n", node->id);
	} else if (strcmp(node->name, "object") == 0) {
		if (strcmp(node->type, "frame") == 0) {
			fprintf(header, "w_frame_t *%s;\n", node->id);
		} else if (strcmp(node->type, "button") == 0) {
			fprintf(header, "w_button_t *%s;\n", node->id);
		} else if (strcmp(node->type, "textbox") == 0) {
			fprintf(header, "w_textbox_t *%s;\n", node->id);
		} else if (strcmp(node->type, "checkbox") == 0) {
			fprintf(header, "w_checkbox_t *%s;\n", node->id);
		} else if (strcmp(node->type, "editbox") == 0) {
			fprintf(header, "w_editbox_t *%s;\n", node->id);
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
		fprintf(header, "void %s (w_object_t *object);\n", node->value);
	} else if (strcmp(node->name, "pressed") == 0) {
		fprintf(header, "void %s (w_object_t *object, int button);\n", node->value);
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
	fprintf(header,
	        "\n"
	        "#include <stdio.h>\n"
	        "#include <stdlib.h>\n"
	        "#include <time.h>\n"
	        "#include <xynth.h>\n"
	        "#include <widget.h>\n"
	        "\n");
	node_generate_header(node);
	fprintf(header, "\n");
	node_generate_function(node);
	fprintf(source,
	        "\n"
	        "#include \"%s\"\n"
	        "\n"
	        "int main (int argc, char *argv[])\n"
	        "{\n"
	        "srand(time(NULL));\n",
	        header_name);
	node_generate_code(node);
	fprintf(source,
	        "return 0;\n"
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
	int c;
	int l;
	char *buf = NULL;
	struct stat stbuf;
	
	while ((c = getopt(argc, argv, "f:o:h")) != -1) {
		switch (c) {
			case 'f':
				input = fopen(optarg, "r");
				stat(optarg, &stbuf);
				l = stbuf.st_size;
				buf = (char *) malloc(stbuf.st_size + 1);
				fread(buf, 1, stbuf.st_size, input);
				break;
			case 'o':
				source_name = (char *) malloc(strlen(optarg) + 20);
				header_name = (char *) malloc(strlen(optarg) + 20);
				sprintf(source_name, "%s_xml.c", optarg);
				sprintf(header_name, "%s_xml.h", optarg);
				source = fopen(source_name, "w+");
				header = fopen(header_name, "w+");
				break;
usage:			case 'h':
			default:
				printf("%s -f input_file -o output_name\n", argv[0]);
				exit(1);
				break;
		}
	}
	
	if (buf == NULL ||
	    input == NULL ||
	    source == NULL ||
	    header == NULL) {
	    	goto usage;
	}
	
	data = (data_t *) malloc(sizeof(data_t));
	memset(data, 0, sizeof(data_t));
	
	XML_Parser p = XML_ParserCreate(NULL);
	if (!p) {
		fprintf(stderr, "Couldn't allocate memory for parser\n");
		exit(-1);
	}

	XML_SetElementHandler(p, start, end);
	XML_SetCharacterDataHandler(p, char_hndl);

	if (!XML_Parse(p, buf, l, 1)) {
		fprintf(stderr, "Parse error at line %d:\n%s\n", XML_GetCurrentLineNumber(p), XML_ErrorString(XML_GetErrorCode(p)));
		exit(-1);
	}

//	node_parse(data->node);
	node_generate(data->node);
	return 0;
}
