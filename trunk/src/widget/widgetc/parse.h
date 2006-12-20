#ifndef PARSE_H_
#define PARSE_H_

typedef struct xml_data_s {
	char *path;
	node_t *active;
	node_t *root;
	node_t *elem;
} xml_data_t;

void parse_xml_start (void *x_data, const char *el, const char **attr);
void parse_xml_end (void *x_data, const char *el);
void parse_xml_char_hndl_fixup (char *out);
void parse_xml_char_hndl (void *x_data, const char *txt, int txtlen);
int parse_xml_file (xml_data_t *xdata, char *name);

#endif /*PARSE_H_*/
