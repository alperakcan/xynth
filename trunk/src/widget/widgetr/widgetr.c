
#include "xynth_.h"
#include "widget.h"
#include <getopt.h>

#include "table.h"

typedef struct w_widgetr_s w_widgetr_t;

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
	while (n < 8) {
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
	while (n < count) {
		intvalue = intvalue | (digit[n] << (m << 2));
		m--;
		n++;
	}
	return (intvalue);
}

int main (int argc, char *argv[])
{
	char c;
	char *hex;
	unsigned int i;
	w_widgetr_t *widgetr;
	unsigned int option_index = 0;
	struct option long_options[] = {
		{"depth", 1, 0, 0},
		{"mask", 1, 0, 0},
		{"help", 0, 0, 0},
		{0, 0, 0, 0},
	};

	widgetr = (w_widgetr_t *) s_malloc(sizeof(w_widgetr_t));
	memset(widgetr, 0, sizeof(w_widgetr_t));
	
	while ((c = getopt_long(argc, argv, "d:m:h", long_options, &option_index)) != -1) {
		switch (c) {
			case 0:
				if (strcmp("depth", long_options[option_index].name) == 0) {
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
			case 'd':
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
