/***************************************************************************
    begin                : Tue Sep 22 2007
    copyright            : (C) 2007 by Alper Akcan
    email                : distchx@yahoo.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "xdialog.h"

extern xd_widget_t xd_msgbox;
extern xd_widget_t xd_yesno;

xd_widget_t *xd_widgets[] = {
	&xd_msgbox,
	&xd_yesno,
};

struct option xd_options[] = {
	{"help", 0, NULL, 0x1},
};

int xd_help (void)
{
	unsigned int i;
	unsigned int xd_widgets_size;
	
	xd_widgets_size = sizeof(xd_widgets) / sizeof(*xd_widgets);
	for (i = 0; i < xd_widgets_size; i++) {
		printf("    --%-10s %s\n", xd_widgets[i]->name, xd_widgets[i]->help);
	}
	
	return 0;
}

int main (int argc, char *argv[])
{
	int a;
	int c;
	int r;
	int oindex = 0;
	unsigned int i;
	unsigned int o;
	struct option *options = NULL;
	unsigned int options_size;
	unsigned int xd_options_size;
	unsigned int xd_widgets_size;
	
	xd_options_size = sizeof(xd_options) / sizeof(struct option);
	xd_widgets_size = sizeof(xd_widgets) / sizeof(*xd_widgets);
	options_size = xd_options_size + xd_widgets_size + 1;

	options = (struct option *) malloc(sizeof(struct option) * options_size);
	if (options == NULL) {
		printf("not enough memory.\n");
		return -1;
	}
	memset(options, 0, sizeof(struct option) * options_size);
	
	for (o = 0, i = 0; i < xd_widgets_size; i++, o++) {
		options[o].name = xd_widgets[i]->name;
	}
	for (i = 0; i < xd_options_size; i++, o++) {
		options[o].name = xd_options[i].name;
		options[o].has_arg = xd_options[i].has_arg;
		options[o].flag = xd_options[i].flag;
		options[o].val = xd_options[i].val;
	}
	
	r = -2;
	a = 1;
	while ((c = getopt_long(argc, argv, "", options, &oindex)) != -1) {
		a++;
		switch (c) {
			case 00:
				for (i = 0; i < xd_widgets_size; i++) {
					if (strcmp(options[oindex].name, xd_widgets[i]->name) == 0) {
						if ((argc - a) == xd_widgets[i]->nargs) {
							if (xd_widgets[i]->func != NULL) {
								r = xd_widgets[i]->func(argc - a, &argv[a]);
								break;
							}
						} else {
						}
					}
				}
				break;
			case 0x1:
			default:
				xd_help();
				goto out;
		}
	}
	
	if (r == -2) {
		xd_help();
		r = -1;
	}
out:	
	free(options);

	return r;
}

#if defined(CONFIG_SINGLE_APPLICATION)
s_single_app_t single_xdialog = {
	xdialog_main,
	1,
	{"xdialog"}
};
#endif
