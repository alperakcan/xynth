/***************************************************************************
    begin                : Fri Feb 11 2005
    copyright            : (C) 2005 - 2007 by Alper Akcan
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

#include "xynth_.h"

char * s_config_strip (char *buf)
{
	char *start;

	/* Strip off trailing whitespace, returns, etc */
	while ((*buf != '\0') && (buf[strlen(buf) - 1] < 33)) {
		buf[strlen(buf) - 1] = '\0';
	}
	start = buf;

	/* Strip off leading whitespace, returns, etc */
	while (*start && (*start < 33)) {
		start++;
	}
	
	return start;
}

int s_config_parse (s_config_t *cfg, char *name)
{
        FILE *fp;
        char *buf;
        s_config_cat_t *category;
        s_config_var_t *variable;

	buf = (char *) s_malloc(sizeof(char) * (8192 + 1));
	
        fp = fopen(name, "r");
        if (fp == NULL) {
		debugf(DSYS, "Unable to open file : %s", name);
		goto err0;
	}

	while (!feof(fp)) {
		if (fgets(buf, 8192, fp)) {
			char *c;
			char *cur;

			/* strip comments */
			c = strchr(buf, ';');
			while (c) {
				if ((c == buf) || (*(c - 1) != '\\')) {
					*c = '\0';
				} else {
					*(c - 1) = ';';
					memmove(c, c + 1, strlen(c + 1));
				}
				c = strchr(c + 1, ';');
			}

			cur = s_config_strip(buf);
			if (*buf != '\0') {
				if (cur[0] == '[') {
					/* A category header */
					c = strchr(cur, ']');
					if (c) {
						*c = 0;
						if (!s_config_category_init(&category, cur + 1)) {
							s_list_add(cfg->category, category, -1);
						}
					} else {
						debugf(0, "parse error: no closing ']' for category %s of %s", cur + 1, name);
						goto err1;
					}
				} else if (cur[0] == '#') {
					/* A directive */
				} else {
					/* variable = value */
					category = (s_config_cat_t *) s_list_get(cfg->category, cfg->category->nb_elt - 1);
					if (category == NULL) {
						debugf(0, "parse error: There is no category context for %s", name);
						goto err1;
					}
					c = strchr(cur, '=');
					if (c) {
						*c = 0;
						c++;
						if (!s_config_variable_init(&variable, s_config_strip(cur), s_config_strip(c))) {
							s_list_add(category->variable, variable, -1);
						}
					} else {
						debugf(0, "parse error: No '=' (equal sign) in %s of %s", cur, name);
						goto err1;
					}
				}
			} else {
			}
		}
	}
	
	s_free(buf);
	fclose(fp);
	return 0;

err1:	fclose(fp);
err0:	s_free(buf);
	return -1;
}

int s_config_init (s_config_t **cfg)
{
	(*cfg) = (s_config_t *) s_malloc(sizeof(s_config_t));
	if (s_list_init(&((*cfg)->category))) {
		goto err0;
	}
	return 0;
err0:	s_free(*cfg);
	return -1;
}

int s_config_category_init (s_config_cat_t **cat, char *name)
{
	(*cat) = (s_config_cat_t *) s_malloc(sizeof(s_config_cat_t));
	(*cat)->name = strdup(name);
	if (s_list_init(&((*cat)->variable))) {
		goto err0;
	}
	return 0;
err0:	s_free((*cat)->name);
	s_free(*cat);
	return -1;
}

int s_config_variable_init (s_config_var_t **var, char *name, char *value)
{
	(*var) = (s_config_var_t *) s_malloc(sizeof(s_config_var_t));
	(*var)->name = strdup(name);
	(*var)->value = strdup(value);
	return 0;
}

int s_config_variable_uninit (s_config_var_t *var)
{
	s_free(var->name);
	s_free(var->value);
	s_free(var);
	return 0;
}

int s_config_category_uninit (s_config_cat_t *cat)
{
        while (!s_list_eol(cat->variable, 0)) {
		s_config_var_t *var = (s_config_var_t *) s_list_get(cat->variable, 0);
		s_list_remove(cat->variable, 0);
		s_config_variable_uninit(var);
	}
        s_list_uninit(cat->variable);
        s_free(cat->name);
	s_free(cat);
	return 0;
}

int s_config_uninit (s_config_t *cfg)
{
        while (!s_list_eol(cfg->category, 0)) {
		s_config_cat_t *cat = (s_config_cat_t *) s_list_get(cfg->category, 0);
		s_list_remove(cfg->category, 0);
		s_config_category_uninit(cat);
	}
        s_list_uninit(cfg->category);
	s_free(cfg);
	return 0;
}
