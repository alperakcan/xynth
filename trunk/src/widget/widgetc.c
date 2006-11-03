
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <expat.h>
#include <xynth.h>

typedef struct node_s {
	char *name;
	char *id;
	char *type;
	char *value;
	s_list_t *nodes;
	struct node_s *parent;
	int dontparse;
} node_t;

static FILE *g_input = NULL;
static FILE *g_source = NULL;
static FILE *g_header = NULL;
static char *g_source_name = NULL;
static char *g_header_name = NULL;

static node_t *g_node = NULL;
static int g_depth = 0;
static char *g_path = NULL;
static node_t *g_active = NULL;

int s_list_init (s_list_t **li)
{
	(*li) = (s_list_t *) malloc(sizeof(s_list_t));
	if ((*li) == NULL) {
		return -1;
	}
	(*li)->nb_elt = 0;
	return 0;
}

int s_list_uninit (s_list_t *li)
{
	free(li);
	return 0;
}

int s_list_eol (s_list_t *li, int i)
{
        if (li == NULL) {
		return 1;
	}

	if ((i >= 0) && (i < li->nb_elt)) {
		return 0;
	}
	/* end of list */
	return 1;
}

void * s_list_get (s_list_t *li, int pos)
{
	int i = 0;
	s_list_node_t *ntmp;
	if ((li == NULL) || (pos < 0) || (pos >= li->nb_elt)) {
		/* element does not exist */
		return NULL;
	}
	/* exist because nb_elt > 0 */
	ntmp = li->node;
	while (pos > i) {
		i++;
		ntmp = (s_list_node_t *) ntmp->next;
	}
	return ntmp->element;
}

int s_list_remove (s_list_t *li, int pos)
{
	int i = 0;
	s_list_node_t *ntmp;
	if ((li == NULL) || (pos < 0) || (pos >= li->nb_elt)) {
		/* element does not exist */
		return -1;
	}
	/* exist because nb_elt > 0 */
	ntmp = li->node;
	if ((pos == 0)) {
		/* special case  */
		li->node = (s_list_node_t *) ntmp->next;
		li->nb_elt--;
		free(ntmp);
		return li->nb_elt;
	}
	while (pos > (i + 1)) {
		i++;
		ntmp = (s_list_node_t *) ntmp->next;
	}
	/* insert new node */
	{
		s_list_node_t *remnode;
		remnode = (s_list_node_t *) ntmp->next;
		ntmp->next = ((s_list_node_t *) ntmp->next)->next;
		free(remnode);
		li->nb_elt--;
	}
	return li->nb_elt;
}

int s_list_add (s_list_t *li, void *el, int pos)
{
	int i = 0;
	s_list_node_t *ntmp;
	if (li == NULL) {
		return -1;
	}
	if ((pos == -1) || (pos >= li->nb_elt)) {
		/* insert at the end  */
		pos = li->nb_elt;
	}
	if (li->nb_elt == 0) {
		li->node = (s_list_node_t *) malloc(sizeof(s_list_node_t));
		li->node->element = el;
		li->nb_elt++;
		return li->nb_elt;
	}
	/* exist because nb_elt > 0  */
	ntmp = li->node;
	if (pos == 0) {
		li->node = (s_list_node_t *) malloc(sizeof(s_list_node_t));
		li->node->element = el;
		li->node->next = ntmp;
		li->nb_elt++;
		return li->nb_elt;
	}
	/* pos = 0 insert before first elt  */
	while (pos > (i + 1)) {
		i++;
		/* when pos > i next node exist  */
		ntmp = (s_list_node_t *) ntmp->next;
	}
	/* if pos == nb_elt next node does not exist  */
	if (pos == li->nb_elt) {
		ntmp->next = (s_list_node_t *) malloc(sizeof(s_list_node_t));
		ntmp = (s_list_node_t *) ntmp->next;
		ntmp->element = el;
		li->nb_elt++;
		return li->nb_elt;
	}
	/* here pos == i so next node is where we want to insert new node */
	{
		s_list_node_t *nextnode = (s_list_node_t *) ntmp->next;
		ntmp->next = (s_list_node_t *) malloc(sizeof(s_list_node_t));
		ntmp = (s_list_node_t *) ntmp->next;
		ntmp->element = el;
		ntmp->next = nextnode;
		li->nb_elt++;
	}
	return li->nb_elt;
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
node_t * node_get_node_ (node_t *node, char *path)
{
	int p;
	char *ptr;
	char *str;
	node_t *res;
	node_t *tmp;
	p = 0;
	res = NULL;
	while (!s_list_eol(node->nodes, p)) {
		tmp = (node_t *) s_list_get(node->nodes, p);
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

void node_generate_code_window (node_t *node)
{
	fprintf(g_source, "w_window_init(&%s, %s, NULL);\n", node->id, node->type);
}

void node_generate_code_title (node_t *node)
{
	fprintf(g_source, "s_window_set_title(%s->window, \"%s\");\n", node_get_parent(node, "window")->id, node->value);
}

void node_generate_code_move (node_t *node)
{
	node_t *tmp;
	char *x = node_get_value(node, "x");
	char *y = node_get_value(node, "y");
	char *w = node_get_value(node, "w");
	char *h = node_get_value(node, "h");
	if (strcmp(node->parent->name, "window") == 0) {
		fprintf(g_source, "w_window_set_coor(%s, %s, %s, %s, %s);\n", node_get_parent(node, "window")->id, (x) ? x : "0", (y) ? y : "0", (w) ? w : "w", (h) ? h : "0");
	} else if (strcmp(node->parent->name, "object") == 0) {
		fprintf(g_source, "w_object_move(%s->object, %s, %s, %s, %s);\n", node_get_parent(node, "object")->id, (x) ? x : "0", (y) ? y : "0", (w) ? w : "w", (h) ? h : "0");
	}
	if ((tmp = node_get_node(node, "x")) != NULL) { tmp->dontparse = 1; }
	if ((tmp = node_get_node(node, "y")) != NULL) { tmp->dontparse = 1; }
	if ((tmp = node_get_node(node, "w")) != NULL) { tmp->dontparse = 1; }
	if ((tmp = node_get_node(node, "h")) != NULL) { tmp->dontparse = 1; }
}

void node_generate_code_effect (node_t *node)
{
	node_t *tmp;
	char *effect = node_get_value(node, "effect");
	fprintf(g_source, "%s->object->effect->effect = %s;\n", node->parent->id, (effect) ? effect : "0");
	if ((tmp = node_get_node(node, "effect")) != NULL) { tmp->dontparse = 1; }
}

void node_generate_code_style (node_t *node)
{
	node_t *tmp;
	char *shape = node_get_value(node, "shape");
	char *shadow = node_get_value(node, "shadow");
	if (strcmp(node->parent->type, "frame") == 0) {
		fprintf(g_source, "%s->style = %s | %s;\n", node->parent->id, (shape) ? shape : "0" , (shadow) ? shadow : "0");
	} else if (strcmp(node->parent->type, "textbox") == 0) {
		fprintf(g_source, "%s->frame->style = %s | %s;\n", node->parent->id, (shape) ? shape : "0" , (shadow) ? shadow : "0");
	}
	if ((tmp = node_get_node(node, "shape")) != NULL) { tmp->dontparse = 1; }
	if ((tmp = node_get_node(node, "shadow")) != NULL) { tmp->dontparse = 1; }
}

void node_generate_code_string (node_t *node)
{
	if (strcmp(node->parent->type, "textbox") == 0 ||
	    strcmp(node->parent->type, "editbox") == 0) {
		fprintf(g_source, "w_textbox_set_str(%s->object, \"%s\");\n", node->parent->id, node->value);
	} else if (strcmp(node->parent->type, "checkbox") == 0) {
		fprintf(g_source, "w_textbox_set_str(%s->text->object, \"%s\");\n", node->parent->id, node->value);
	}
}

void node_generate_code_show (node_t *node)
{
	if (strcmp(node->parent->name, "window") == 0) {
		fprintf(g_source, "w_object_show(%s->object);\n", node_get_parent(node, "window")->id);
		fprintf(g_source, "s_window_show(%s->window);\n", node_get_parent(node, "window")->id);
		fprintf(g_source, "s_client_main(%s->window);\n", node_get_parent(node, "window")->id);
	} else if (strcmp(node->parent->name, "object") == 0) {
		fprintf(g_source, "w_object_show(%s->object);\n", node_get_parent(node, "object")->id);
	}
}

void node_generate_code_draw (node_t *node)
{
	fprintf(g_source, "%s->object->draw = %s;\n", node->parent->id, node->value);
}

void node_generate_code_pressed (node_t *node)
{
	fprintf(g_source, "%s->pressed = %s;\n", node->parent->id, node->value);
}

void node_generate_code_released (node_t *node)
{
	fprintf(g_source, "%s->released = %s;\n", node->parent->id, node->value);
}

void node_generate_code_changed (node_t *node)
{
	fprintf(g_source, "%s->changed = %s;\n", node->parent->id, node->value);
}

void node_generate_code_image (node_t *node, char *to)
{
	int i;
	int count;
	char *parent;
	char *shape;
	char *shadow;
	char *rotate;
	char *cntstr;
	node_t *tmp;
	shape = node_get_value(node, "style/shape");
	shadow = node_get_value(node, "style/shadow");
	rotate = node_get_value(node, "rotate");
	count = atoi(node_get_value(node, "count"));
	if (to) {
		parent = to;
	} else if (node_get_parent(node, "object")) {
		parent = node_get_parent(node, "object")->id;
	} else {
		return;
	}
	fprintf(g_source, "w_frame_set_image(%s->object, %s | %s, %s, %d", parent, (shape) ? shape : "0", (shadow) ? shadow : "0", (rotate) ? rotate : "0", count);
	cntstr = (char *) malloc(sizeof(char *) * 255);
	for (i = 0; i < count; i++) {
		sprintf(cntstr, "image%d", i);
		fprintf(g_source, ", \"%s\"", node_get_value(node, cntstr));
		tmp = node_get_node(node, cntstr);
		tmp->dontparse = 1;
	}
	free(cntstr);
	fprintf(g_source, ");\n");
	if ((tmp = node_get_node(node, "style")) != NULL) { tmp->dontparse = 1; }
	if ((tmp = node_get_node(node, "style/shape")) != NULL) { tmp->dontparse = 1; }
	if ((tmp = node_get_node(node, "style/shadow")) != NULL) { tmp->dontparse = 1; }
	if ((tmp = node_get_node(node, "count")) != NULL) { tmp->dontparse = 1; }
	if ((tmp = node_get_node(node, "rotate")) != NULL) { tmp->dontparse = 1; }
}

void node_generate_code_object (node_t *node)
{
	node_t *tmp;
	if (strcmp(node->type, "frame") == 0) {
		fprintf(g_source, "w_frame_init(%s, &%s, %s, %s->object);\n", node_get_parent(node, "window")->id, node->id, "0", node->parent->id);
	} else if (strcmp(node->type, "button") == 0) {
		fprintf(g_source, "w_button_init(%s, &%s, %s->object);\n", node_get_parent(node, "window")->id, node->id, node->parent->id);
	} else if (strcmp(node->type, "textbox") == 0) {
		fprintf(g_source, "w_textbox_init(%s, &%s, %s->object);\n", node_get_parent(node, "window")->id, node->id, node->parent->id);
		if ((tmp = node_get_node(node, "properties")) != NULL) {
			fprintf(g_source, "%s->properties = %s;\n", tmp->parent->id, tmp->value);
			tmp->dontparse = 1;
		}
		if ((tmp = node_get_node(node, "size")) != NULL) {
			fprintf(g_source, "w_textbox_set_size(%s->object, %s);\n", tmp->parent->id, tmp->value);
			tmp->dontparse = 1;
		}
		if ((tmp = node_get_node(node, "color")) != NULL) {
			fprintf(g_source, "w_textbox_set_rgb(%s->object, %s, %s, %s);\n", tmp->parent->id, node_get_value(tmp, "red"), node_get_value(tmp, "green"), node_get_value(tmp, "blue"));
			tmp->dontparse = 1;
			if ((tmp = node_get_node(node, "color/red")) != NULL) { tmp->dontparse = 1; }
			if ((tmp = node_get_node(node, "color/green")) != NULL) { tmp->dontparse = 1; }
			if ((tmp = node_get_node(node, "color/blue")) != NULL) { tmp->dontparse = 1; }
		}
	} else if (strcmp(node->type, "checkbox") == 0) {
		fprintf(g_source, "w_checkbox_init(%s, &%s, %s->object);\n", node_get_parent(node, "window")->id, node->id, node->parent->id);
		if ((tmp = node_get_node(node, "properties")) != NULL) {
			fprintf(g_source, "%s->text->properties = %s;\n", tmp->parent->id, tmp->value);
			tmp->dontparse = 1;
		}
		if ((tmp = node_get_node(node, "size")) != NULL) {
			fprintf(g_source, "w_textbox_set_size(%s->text->object, %s);\n", tmp->parent->id, tmp->value);
			tmp->dontparse = 1;
		}
		if ((tmp = node_get_node(node, "color")) != NULL) {
			fprintf(g_source, "w_textbox_set_rgb(%s->text->object, %s, %s, %s);\n", tmp->parent->id, node_get_value(tmp, "red"), node_get_value(tmp, "green"), node_get_value(tmp, "blue"));
			tmp->dontparse = 1;
			if ((tmp = node_get_node(node, "color/red")) != NULL) { tmp->dontparse = 1; }
			if ((tmp = node_get_node(node, "color/green")) != NULL) { tmp->dontparse = 1; }
			if ((tmp = node_get_node(node, "color/blue")) != NULL) { tmp->dontparse = 1; }
		}
		while ((tmp = node_get_node(node, "image")) != NULL) {
			char str[255];
			sprintf(str, "%s->box", node->id);
			node_generate_code_image(tmp, str);
			tmp->dontparse = 1;
		}
		if ((tmp = node_get_node(node, "changed")) != NULL) {
			node_generate_code_changed(tmp);
			tmp->dontparse = 1;
		}
	} else if (strcmp(node->type, "editbox") == 0) {
		fprintf(g_source, "w_editbox_init(%s, &%s, %s->object);\n", node_get_parent(node, "window")->id, node->id, node->parent->id);
	} else if (strcmp(node->type, "progressbar") == 0) {
		fprintf(g_source, "w_progressbar_init(%s, &%s, %s->object);\n", node_get_parent(node, "window")->id, node->id, node->parent->id);
		while ((tmp = node_get_node(node, "image")) != NULL) {
			node_generate_code_image(tmp, node->id);
			tmp->dontparse = 1;
		}
		if ((tmp = node_get_node(node, "box")) != NULL) {
			node_t *btmp;
			while ((btmp = node_get_node(tmp, "image")) != NULL) {
				char str[255];
				sprintf(str, "%s->box", node->id);
				node_generate_code_image(btmp, str);
				btmp->dontparse = 1;
			}
		}
	}
	if ((tmp = node_get_node(node, "style")) != NULL) {
		node_generate_code_style(tmp);
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "effect")) != NULL) {
		node_generate_code_effect(tmp);
		tmp->dontparse = 1;
	}
	if ((tmp = node_get_node(node, "draw")) != NULL) {
		node_generate_code_draw(tmp);
		tmp->dontparse = 1;
	}
	while ((tmp = node_get_node(node, "image")) != NULL) {
		node_generate_code_image(tmp, node->id);
		tmp->dontparse = 1;
	}
}

void node_generate_code (node_t *node)
{
	int p;
	node_t *tmp;
	if (node->dontparse != 0) {
		return;
	}
	if (strcmp(node->name, "window") == 0) {
		node_generate_code_window(node);
	} else if (strcmp(node->name, "object") == 0) {
		node_generate_code_object(node);
	} else if (strcmp(node->name, "title") == 0) {
		node_generate_code_title(node);
	} else if (strcmp(node->name, "move") == 0) {
		node_generate_code_move(node);
	} else if (strcmp(node->name, "effect") == 0) {
		node_generate_code_effect(node);
	} else if (strcmp(node->name, "style") == 0) {
		node_generate_code_style(node);
	} else if (strcmp(node->name, "string") == 0) {
		node_generate_code_string(node);
	} else if (strcmp(node->name, "show") == 0) {
		node_generate_code_show(node);
	} else if (strcmp(node->name, "draw") == 0) {
		node_generate_code_draw(node);
	} else if (strcmp(node->name, "pressed") == 0) {
		node_generate_code_pressed(node);
	} else if (strcmp(node->name, "released") == 0) {
		node_generate_code_released(node);
	} else if (strcmp(node->name, "image") == 0) {
		node_generate_code_image(node, NULL);
	}
	g_depth++;
	p = 0;
	while (!s_list_eol(node->nodes, p)) {
		tmp = (node_t *) s_list_get(node->nodes, p);
		node_generate_code(tmp);
		p++;
	}
	g_depth--;
}

void node_generate_header (node_t *node)
{
	int p;
	node_t *tmp;
	if (strcmp(node->name, "window") == 0) {
		fprintf(g_header, "w_window_t *%s;\n", node->id);
	} else if (strcmp(node->name, "object") == 0) {
		if (strcmp(node->type, "frame") == 0) {
			fprintf(g_header, "w_frame_t *%s;\n", node->id);
		} else if (strcmp(node->type, "button") == 0) {
			fprintf(g_header, "w_button_t *%s;\n", node->id);
		} else if (strcmp(node->type, "textbox") == 0) {
			fprintf(g_header, "w_textbox_t *%s;\n", node->id);
		} else if (strcmp(node->type, "checkbox") == 0) {
			fprintf(g_header, "w_checkbox_t *%s;\n", node->id);
		} else if (strcmp(node->type, "editbox") == 0) {
			fprintf(g_header, "w_editbox_t *%s;\n", node->id);
		} else if (strcmp(node->type, "progressbar") == 0) {
			fprintf(g_header, "w_progressbar_t *%s;\n", node->id);
		}
	}
	g_depth++;
	p = 0;
	while (!s_list_eol(node->nodes, p)) {
		tmp = (node_t *) s_list_get(node->nodes, p);
		node_generate_header(tmp);
		p++;
	}
	g_depth--;
}

void node_generate_function (node_t *node)
{
	int p;
	node_t *tmp;
	if (strcmp(node->name, "draw") == 0) {
		fprintf(g_header, "void %s (w_object_t *object);\n", node->value);
	} else if (strcmp(node->name, "pressed") == 0) {
		fprintf(g_header, "void %s (w_object_t *object, int button);\n", node->value);
	} else if (strcmp(node->name, "released") == 0) {
		fprintf(g_header, "void %s (w_object_t *object, int button);\n", node->value);
	} else if (strcmp(node->name, "changed") == 0) {
		fprintf(g_header, "void %s (w_object_t *object, int state);\n", node->value);
	}
	g_depth++;
	p = 0;
	while (!s_list_eol(node->nodes, p)) {
		tmp = (node_t *) s_list_get(node->nodes, p);
		node_generate_function(tmp);
		p++;
	}
	g_depth--;
}

void node_generate (node_t *node)
{
	fprintf(g_header,
	        "\n"
	        "#include <stdio.h>\n"
	        "#include <stdlib.h>\n"
	        "#include <time.h>\n"
	        "#include <xynth.h>\n"
	        "#include <widget.h>\n"
	        "\n");
	node_generate_header(node);
	fprintf(g_header, "\n");
	node_generate_function(node);
	fprintf(g_source,
	        "\n"
	        "#include \"%s\"\n"
	        "\n"
	        "int main (int argc, char *argv[])\n"
	        "{\n"
	        "srand(time(NULL));\n",
	        g_header_name);
	node_generate_code(node);
	fprintf(g_source,
	        "return 0;\n"
	        "}\n");
}

void node_print (node_t *node)
{
	int i;
	for (i = 0; i < g_depth; i++) {
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
	g_depth++;
	p = 0;
	while (!s_list_eol(node->nodes, p)) {
		tmp = (node_t *) s_list_get(node->nodes, p);
		node_parse(tmp);
		p++;
	}
	g_depth--;
}

void node_init (node_t **node)
{
	node_t *n;
	n = (node_t *) malloc(sizeof(node_t));
	memset(n, 0, sizeof(node_t));
	s_list_init(&(n->nodes));
	*node = n;
}

void node_uninit (node_t *node)
{
	node_t *tmp;
	while (!s_list_eol(node->nodes, 0)) {
		tmp = (node_t *) s_list_get(node->nodes, 0);
		s_list_remove(node->nodes, 0);
		node_uninit(tmp);
	}
	s_list_uninit(node->nodes);
	free(node->id);
	free(node->name);
	free(node->type);
	free(node->value);
	free(node);
}

void start (void *xdata, const char *el, const char **attr)
{
	free(g_path);
	g_path = strdup(el);
	g_depth++;
	
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
		node->parent = g_active;
		g_active = node;
		if (node->parent) {
			s_list_add(node->parent->nodes, node, -1);
		} else {
			g_node = node;
		}
	}
}

void end (void *xdata, const char *el)
{
	g_depth--;
	free(g_path);
	g_path = NULL;
	g_active = g_active->parent;
}

void char_hndl (void *xdata, const char *txt, int txtlen)
{
	char *str;
	char *ptr;
	char *end;
	if (txtlen > 0 &&
	    txt &&
	    g_path) {
	} else {
	    return;
	}
	end = (char *) malloc(sizeof(char) * (strlen(g_path) + 3 + 1));
	sprintf(end, "</%s>", g_path);
	str = strdup(txt);
	ptr = str + txtlen;
	if (g_path &&
	    strncmp(ptr, end, strlen(end)) == 0) {
	    	str[txtlen] = '\0';
	    	if (g_active) {
	    		g_active->value = strdup(str);
	    	}
	}
	free(str);
	free(end);
}

int main (int argc, char **argv)
{
	int c;
	int l = 0;
	char *buf = NULL;
	struct stat stbuf;
	
	while ((c = getopt(argc, argv, "f:o:h")) != -1) {
		switch (c) {
			case 'f':
				g_input = fopen(optarg, "r");
				stat(optarg, &stbuf);
				l = stbuf.st_size;
				buf = (char *) malloc(stbuf.st_size + 1);
				fread(buf, 1, stbuf.st_size, g_input);
				break;
			case 'o':
				g_source_name = (char *) malloc(strlen(optarg) + 20);
				g_header_name = (char *) malloc(strlen(optarg) + 20);
				sprintf(g_source_name, "%s_xml.c", optarg);
				sprintf(g_header_name, "%s_xml.h", optarg);
				g_source = fopen(g_source_name, "w+");
				g_header = fopen(g_header_name, "w+");
				break;
usage:			case 'h':
			default:
				printf("%s -f input_file -o output_name\n", argv[0]);
				exit(1);
				break;
		}
	}
	
	if (buf == NULL ||
	    g_input == NULL ||
	    g_source == NULL ||
	    g_header == NULL) {
	    	goto usage;
	}
	
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
	
	XML_ParserFree(p);

	node_parse(g_node);
	node_generate(g_node);

	node_uninit(g_node);
	fclose(g_input);
	fclose(g_source);
	fclose(g_header);
	free(g_source_name);
	free(g_header_name);
	free(buf);
	
	return 0;
}
