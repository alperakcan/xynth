
#include "xynth_.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <expat.h>

int s_xml_node_path_normalize (char *out, int len)
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

s_xml_node_t * s_xml_node_get_path_ (s_xml_node_t *node, char *path)
{
	int p;
	char *ptr;
	char *str;
	s_xml_node_t *res;
	s_xml_node_t *tmp;
	p = 0;
	res = NULL;
	if (path[0] == '/') {
		path++;
		node = node->root;
		str = strchr(path, '/');
		if (str == NULL &&
		    strcmp(node->name, path) == 0) {
			path = str + 1;
			goto ok;
		}
		if (str != NULL &&
		    strncmp(node->name, path, str - path) == 0 &&
		    strlen(node->name) == str - path) {
			path = str + 1;
			goto ok;
		}
		goto end;
	}
ok:
	while (!s_list_eol(node->nodes, p)) {
		tmp = (s_xml_node_t *) s_list_get(node->nodes, p);
		str = strchr(path,  '/');
		if (str == NULL) {
			if (strcmp(tmp->name, path) == 0) {
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
			    	if ((res = s_xml_node_get_path_(tmp, ptr)) != NULL) {
			    		break;
			    	}
			}
		}
		p++;
	}
end:	return res;
}

s_xml_node_t * s_xml_node_get_path (s_xml_node_t *node, char *path)
{
	int len;
	char *str;
	s_xml_node_t *res;
	len = strlen(path);
	str = (char *) s_malloc(len + 10 + 1);
	memset(str, 0, len + 10 + 1);
	memcpy(str, path, len);
	s_xml_node_path_normalize(str, len + 10);
	res = s_xml_node_get_path_(node, str);
	s_free(str);
	return res;
}

char * s_xml_node_get_value (s_xml_node_t *node)
{
	return node->value;
}

char * s_xml_node_get_path_value (s_xml_node_t *node, char *path)
{
	s_xml_node_t *res;
	res = s_xml_node_get_path(node, path);
	if (res == NULL) {
		return NULL;
	}
	return s_xml_node_get_value(res);
}

int s_xml_node_attr_init (s_xml_node_attr_t **attr)
{
	s_xml_node_attr_t *a;
	a = (s_xml_node_attr_t *) s_malloc(sizeof(s_xml_node_attr_t));
	memset(a, 0, sizeof(s_xml_node_attr_t));
	*attr = a;
	return 0;
}

int s_xml_node_attr_uninit (s_xml_node_attr_t *attr)
{
	s_free(attr->name);
	s_free(attr->value);
	s_free(attr);
	return 0;
}

int s_xml_node_init (s_xml_node_t **node)
{
	s_xml_node_t *n;
	n = (s_xml_node_t *) s_malloc(sizeof(s_xml_node_t));
	memset(n, 0, sizeof(s_xml_node_t));
	s_list_init(&n->attrs);
	s_list_init(&n->nodes);
	*node = n;
	return 0;
}

int s_xml_node_uninit (s_xml_node_t *node)
{
	s_xml_node_t *n;
	s_xml_node_attr_t *a;
	if (node == NULL) {
		return 0;
	}
	while (!s_list_eol(node->nodes, 0)) {
		n = (s_xml_node_t *) s_list_get(node->nodes, 0);
		s_list_remove(node->nodes, 0);
		s_xml_node_uninit(n);
	}
	s_list_uninit(node->nodes);
	while (!s_list_eol(node->attrs, 0)) {
		a = (s_xml_node_attr_t *) s_list_get(node->attrs, 0);
		s_list_remove(node->attrs, 0);
		s_xml_node_attr_uninit(a);
	}
	s_list_uninit(node->attrs);
	s_free(node->name);
	s_free(node->value);
	s_free(node);
	return 0;
}

void s_xml_parse_start (void *xdata, const char *el, const char **xattr)
{
	int p;
	s_xml_data_t *data;
	s_xml_node_t *node;
	s_xml_node_attr_t *attr;
	data = (s_xml_data_t *) xdata;
	s_free(data->path);
	data->path = (char *) strdup(el);
	s_xml_node_init(&node);
	node->name = (char *) strdup(el);
	for (p = 0; xattr[p] && xattr[p + 1]; p += 2) {
		s_xml_node_attr_init(&attr);
		attr->name = (char *) strdup(xattr[p]);
		attr->value = (char *) strdup(xattr[p + 1]);
		s_list_add(node->attrs, attr, -1);
	}
	if (data->active) {
		s_list_add(data->active->nodes, node, -1);
	} else {
		data->root = node;
	}
	node->parent = data->active;
	node->root = data->root;
	data->active = node;
}

void s_xml_parse_end (void *xdata, const char *el)
{
	s_xml_data_t *data;
	data = (s_xml_data_t *) xdata;
	s_free(data->path);
	data->path = NULL;
	data->active = data->active->parent;
}

void s_xml_parse_character_fixup (char *out)
{
	int i;
	int j;
	/* Convert "&amp;" to "&" */
	for (i = 0; out[i] && out[i + 1] && out[i + 2] && out[i + 3] && out[i + 4]; i++) {
		if (out[i] == '&' && out[i + 1] == 'a' && out[i + 2] == 'm' && out[i + 3] == 'p' && out[i + 4] == ';') {
			for (j = i + 1; out[j]; j++)
				out[j] = out[j + 4];
			i--;
		}
	}
}

void s_xml_parse_character (void *xdata, const char *txt, int txtlen)
{
	char *str;
	s_xml_data_t *data;
	unsigned int total = 0;
	unsigned int total_old = 0;
	data = (s_xml_data_t *) xdata;
	if (txtlen > 0 && txt && data->path) {
	} else {
		return;
	}
	if (data->active == NULL) {
		return;
	}
	if (data->active->value != NULL) {
		total_old = strlen(data->active->value);
	}
	total = (total_old + txtlen + 1) * sizeof(char *);
	data->active->value = (char *) s_realloc(data->active->value, total);
	if (total_old == 0) {
		data->active->value[0] = '\0';
	}
	strncat(data->active->value, txt, txtlen); 
	str = data->active->value;
	if (data->path) {
	    	if (data->active && data->active->value) {
	    		s_xml_parse_character_fixup(data->active->value);
	    	}
	}
}

int s_xml_parse_buffer (s_xml_node_t **node, char *buffer, unsigned int len)
{
	int ret;
	XML_Parser p;
	s_xml_data_t *data;
	ret = 0;
	data = (s_xml_data_t *) s_malloc(sizeof(s_xml_data_t));
	memset(data, 0, sizeof(s_xml_data_t));
	p = XML_ParserCreate(NULL);
	XML_SetUserData(p, data);
	XML_SetElementHandler(p, s_xml_parse_start, s_xml_parse_end);
	XML_SetCharacterDataHandler(p, s_xml_parse_character);
	if (!XML_Parse(p, buffer, len, 1)) {
		debugf(0, "Parse error at line %d:%s", XML_GetCurrentLineNumber(p), XML_ErrorString(XML_GetErrorCode(p)));
		ret = -1;
	}
	XML_ParserFree(p);
	*node = data->root;
	s_free(data);
	return ret;
}

int s_xml_parse_file (s_xml_node_t **node, char *file)
{
	int ret;
	FILE *fp;
	char *buffer;
	unsigned int len;
	struct stat stbuf;
	*node = NULL;
	if (stat(file, &stbuf)) {
		debugf(0, "No such file : %s", file);
		return -1;
	}
	ret = 0;
	len = stbuf.st_size;
	buffer = (char *) s_malloc(sizeof(char) * (len + 1));
	fp = fopen(file, "r");
	fread(buffer, 1, len, fp);
	if (s_xml_parse_buffer(node, buffer, len)) {
		debugf(0, "s_xml_parse_buffer failed");
		ret = -1;
	}
	fclose(fp);
	s_free(buffer);
	return ret;
}
