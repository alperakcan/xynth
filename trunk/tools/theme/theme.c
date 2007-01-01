/***************************************************************************
    begin                : Sun Feb 23 2003
    copyright            : (C) 2003 - 2007 by Alper Akcan
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
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "xynth_.h"
#include "server.h"

#define MAX(a, b)	((a) > (b) ? (a) : (b))

s_theme_t s_theme;

char *f_names[FORM_MAX][2] = {
	{"top-left-inactive.xpm", "top-left-active.xpm"},
	{"title-1-inactive.xpm", "title-1-active.xpm"},
	{"title-2-inactive.xpm", "title-2-active.xpm"},
	{"title-3-inactive.xpm", "title-3-active.xpm"},
	{"title-4-inactive.xpm", "title-4-active.xpm"},
	{"title-5-inactive.xpm", "title-5-active.xpm"},
	{"top-right-inactive.xpm", "top-right-active.xpm"},
	{"left-inactive.xpm", "left-active.xpm"},
	{"right-inactive.xpm", "right-active.xpm"},
	{"bottom-left-inactive.xpm", "bottom-left-active.xpm"},
	{"bottom-inactive.xpm", "bottom-active.xpm"},
	{"bottom-right-inactive.xpm", "bottom-right-active.xpm"},
	};
/*	{"top-left-active.xpm", "top-left-inactive.xpm"},
	{"title-1-active.xpm", "title-1-inactive.xpm"},
	{"title-2-active.xpm", "title-2-inactive.xpm"},
	{"title-3-active.xpm", "title-3-inactive.xpm"},
	{"title-4-active.xpm", "title-4-inactive.xpm"},
	{"title-5-active.xpm", "title-5-inactive.xpm"},
	{"top-right-active.xpm", "top-right-inactive.xpm"},
	{"left-active.xpm", "left-inactive.xpm"},
	{"right-active.xpm", "right-inactive.xpm"},
	{"bottom-left-active.xpm", "bottom-left-inactive.xpm"},
	{"bottom-active.xpm", "bottom-inactive.xpm"},
	{"bottom-right-active.xpm", "bottom-right-inactive.xpm"},
	};
*/

char *b_names[BTNS_MAX][3] = {
	{"close-inactive.xpm", "close-active.xpm", "close-pressed.xpm"},
	{"maximize-inactive.xpm", "maximize-active.xpm", "maximize-pressed.xpm"},
	{"hide-inactive.xpm", "hide-active.xpm", "hide-pressed.xpm"},
	{"menu-inactive.xpm", "menu-active.xpm", "menu-pressed.xpm"},
	};

void print_func (char *file)
{
        int i;
        int len = strlen(file);
        char *name = (char *) s_malloc(len * sizeof(char) + 1);

        memcpy(name, file, len);

	for (i = 0; i < len; i++) {
		if ((name[i] == '-') ||
		    (name[i] == '.')) {
			name[i] = '_';
		}
	}
	name[i] = '\0';
	
        printf("%s", name);
        s_free(name);
}

void init_buf (char *file, s_image_t *img)
{
        int y;
        int x;
	unsigned int *tmp_d;

	s_image_img(file, img);
	s_image_get_handler(img);

	printf("\n"
	       "/*\n"
               " * %s\n"
               " */\n", file);

	printf("static void ");
        print_func(file);
	printf(" (s_image_t *img)\n");

        printf("{\n");
        printf("\tunsigned int rgba[%d] = {\n", img->w * img->h);

        tmp_d = img->rgba;
	for (y = 0; y < img->h; y++) {
		printf("\t");
		for (x = 0; x < img->w; x++) {
			printf("0x%08x, ", *tmp_d);
			tmp_d++;
		}
		printf("\n");
	}

	printf("\t};\n");
	printf("\n");
	printf("\ttheme_set(img, rgba, %d, %d);\n", img->w, img->h);
        printf("}\n");
}

void init_rc (void)
{
	FILE *f_rc;
	char buf[400];

	if ((f_rc = fopen("themerc", "r")) == NULL) {
		printf("themerc file is missing for the desired theme\n");
		exit(1);
	}

	while (!feof(f_rc)) {
		fgets(buf, 400, f_rc);
		if (strncmp(buf, "active_text_color", 17) == 0) {
			s_theme.text_color[1] |= (s_image_hex2int(buf + 19)) << 16;
			s_theme.text_color[1] |= (s_image_hex2int(buf + 21)) << 8;
			s_theme.text_color[1] |= (s_image_hex2int(buf + 23)) << 0;
		}
		if (strncmp(buf, "inactive_text_color", 19) == 0) {
			s_theme.text_color[0] |= (s_image_hex2int(buf + 21)) << 16;
			s_theme.text_color[0] |= (s_image_hex2int(buf + 23)) << 8;
			s_theme.text_color[0] |= (s_image_hex2int(buf + 25)) << 0;
		}
		if (strncmp(buf, "title_vertical_offset_active", 28) == 0) {
			sscanf(buf + 29, "%d", &s_theme.text_v_off[1]);
		}
		if (strncmp(buf, "title_vertical_offset_inactive", 30) == 0) {
			sscanf(buf + 31, "%d", &s_theme.text_v_off[0]);
		}
	}

	fclose(f_rc);
}

void print_head (char *name)
{
	printf(
	"/*\n"
	" * Theme %s Plugin\n"
	" */\n"
	"\n"
	"#include <stdio.h>\n"
	"#include <stdlib.h>\n"
	"#include <string.h>\n"
	"\n"
	"#include \"xynth_.h\"\n"
	"#include \"server.h\"\n",
	name);
	printf("\n");
	printf(
	"static void theme_set (s_image_t *img, unsigned int *data, int a, int b)\n"
	"{\n"
	"\timg->w = a;\n"
	"\timg->h = b;\n"
	"\timg->rgba = (unsigned int *) s_malloc(a * b * sizeof(unsigned int) + 1);\n"
	"\tmemcpy(img->rgba, data, a * b * sizeof(unsigned int));\n");
	printf("}\n");
}

void print_main (char *name)
{
        int f;
        int b;
        int v;
        
	printf(
	"\n"
	"void theme_init (s_theme_t *theme)\n"
	"{\n"
	"\ttheme->title_full = 0;\n\n");

	for (f = 0; f < FORM_MAX; f++) {
		for (v = 0; v < 2; v++) {
			printf("\t");
			print_func(f_names[f][v]);
			printf("(&theme->form[%d][%d]);\n", v, f);
		}
		printf("\n");
	}
	for (b = 0; b < BTNS_MAX; b++) {
		for (v = 0; v < 3; v++) {
			printf("\t");
			print_func(b_names[b][v]);
			printf("(&theme->button[%d][%d]);\n", v, b);
		}
		printf("\n");
	}

	printf("\ttheme->form_min.w = %d;\n", s_theme.form_min.w);
	printf("\ttheme->form_min.w_ = %d;\n", s_theme.form_min.w_);
	printf("\ttheme->form_min.h = %d;\n", s_theme.form_min.h);
	printf("\ttheme->text_color[1] = 0x%06x;\n", s_theme.text_color[1]);
	printf("\ttheme->text_color[0] = 0x%06x;\n", s_theme.text_color[0]);
	printf("\ttheme->text_v_off[1] = %d;\n", s_theme.text_v_off[1]);
	printf("\ttheme->text_v_off[0] = %d;\n", s_theme.text_v_off[0]);

	printf(
	"}\n");
}

int main (int argc, char *argv[])
{
	int f;
	int b;
	int v;
        int i;
        int j;
        int k;
        int wh[2];

	if (argc < 3) {
		printf("%s name dir\n", argv[0]);
		return 0;
	}

	chdir(argv[2]);
	print_head(argv[1]);

        for (f = 0; f < FORM_MAX; f++) {
		for (v = 0; v < 2; v++) {
			init_buf(f_names[f][v], &s_theme.form[v][f]);
		}
	}
        for (b = 0; b < BTNS_MAX; b++) {
		for (v = 0; v < 3; v++) {
			init_buf(b_names[b][v], &s_theme.button[v][b]);
		}
	}

        for (i = 1; i >= 0; i--) {
		wh[i] = s_theme.form[i][TOP_L].w +
		        s_theme.form[i][TOP_R].w +
		        s_theme.button[i][MENU].w +
		        s_theme.form[i][TOP_2].w +
		        s_theme.form[i][TOP_3].w +
		        s_theme.form[i][TOP_4].w +
		        s_theme.button[i][HIDE].w +
		        s_theme.button[i][CLOSE].w +
		        s_theme.button[i][MAXIMIZE].w;
	}
	s_theme.form_min.w = MAX(wh[1], wh[0]);
        for (i = 1; i >= 0; i--) {
		wh[i] = s_theme.form[i][TOP_L].w +
		        s_theme.form[i][TOP_R].w +
		        s_theme.button[i][MENU].w +
		        s_theme.button[i][HIDE].w +
		        s_theme.button[i][CLOSE].w +
		        s_theme.button[i][MAXIMIZE].w;
	}
	s_theme.form_min.w_ = MAX(wh[1], wh[0]);

        for (i = 1; i >= 0; i--) {
		j = MAX(s_theme.form[i][TOP_L].h, MAX(s_theme.form[i][TOP_R].h, s_theme.form[i][TOP_1].h));
		k = MAX(s_theme.form[i][BTM_L].h, MAX(s_theme.form[i][BTM_R].h, s_theme.form[i][BTM].h));
		wh[i] = j + k + MAX(s_theme.form[i][LEFT].h, s_theme.form[i][RIGHT].h);
	}
	s_theme.form_min.h = MAX(wh[1], wh[0]);

	init_rc();

	print_main(argv[1]);
        
	return 0;
}
