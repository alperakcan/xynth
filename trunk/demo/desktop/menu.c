/***************************************************************************
    begin                : Tue Oct 5 2004
    copyright            : (C) 2004 - 2007 by Alper Akcan
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

#include "desktop.h"

void start_menu_atexit (s_window_t *window)
{
	tbar_data_t *tbar_data;
	tbar_data = (tbar_data_t *) window->data;
	if (window->parent->type & WINDOW_CHILD) {
		taskbar_start_menu_handler_rh(window->parent, NULL, NULL);
	}
	tbar_data->tbar_smenu->running = 0;
}

smenu_prog_t * start_menu_list_find_menu (smenu_prog_t *prog, s_list_t *list)
{
	int p = 0;
	smenu_prog_t *s;
	smenu_prog_t *sp;
	
	while (!s_list_eol(list, p)) {
		sp = (smenu_prog_t *) s_list_get(list, p++);
		if (sp->type == SMENU_MENU) {
			if (strcmp(sp->name, prog->menu) == 0) {
				return sp;
			}
			if ((s = start_menu_list_find_menu(prog, sp->progs)) != NULL) {
				return s;
			}
		}
	}

	return NULL;
}

void start_menu_handler (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	smenu_prog_t *sprog;
	tbar_data_t *tbar_data;

	sprog = (smenu_prog_t *) handler->data;
	tbar_data = (tbar_data_t *) window->data;
	
	if (sprog->type == SMENU_PROG) {
		desktop_self_system(sprog->exec);
		return;
	} else if (sprog->type == SMENU_MENU) {
		start_menu_start(window, sprog->progs, window->surface->buf->x + window->surface->buf->w - 12,
		                                       window->surface->buf->y + handler->mouse.y + handler->mouse.h);
	}
}

void start_menu_start (s_window_t *pwindow, s_list_t *progs, int wx, int wy)
{
	int p;
	int fy;
	int fx;
	int fw;
	int fh;
	char *file;
	char *tbuf;
	s_surface_t *s;
	s_font_t *font;
	s_image_t *img;
	s_window_t *temp;
	s_handler_t *hndl;
	smenu_prog_t *sprog;
        tbar_data_t *tbar_data;

	fx = 30;
	fy = 22;
	fw = 160;
	fh = progs->nb_elt * fy + 13;
        tbar_data = (tbar_data_t *) pwindow->data;

	tbar_data->tbar_smenu->running = 1;

	s_window_init(&temp);
        s_window_new(temp, WINDOW_TEMP | WINDOW_NOFORM, pwindow);
        s_window_set_coor(temp, 0, wx, wy - fh - 1, fw, fh);

	s_fillbox(temp->surface, 0, 0, temp->surface->buf->w, temp->surface->buf->h, s_rgbcolor(temp->surface, 0, 0, 0));
	s_fillbox(temp->surface, 1, 1, temp->surface->buf->w - 2, temp->surface->buf->h - 2, s_rgbcolor(temp->surface, 255, 255, 255));
	s_fillbox(temp->surface, 1, 1, 24, temp->surface->buf->h - 2, s_rgbcolor(temp->surface, 197, 198, 189));
	
	s_font_init(&font, "arial.ttf");
	s_font_set_size(font, 13);

	p = 0;
	while (!s_list_eol(progs, p)) {
		sprog = (smenu_prog_t *) s_list_get(progs, p++);
		if ((sprog->icon != NULL) && (*(sprog->icon) != '\0')) {
			s_image_init(&img);
			file = (char *) s_malloc(sizeof(char) * (strlen(DESKTOPDIR "/img/icons/") + strlen(sprog->icon) + 1));
			sprintf(file, "%s/img/icons/%s", DESKTOPDIR, sprog->icon);
			s_image_img(file, img);
			s_image_get_handler(img);
			tbuf = (char *) s_malloc(temp->surface->bytesperpixel * 18 * 18);
			if (s_surface_create(&s, img->w, img->h, temp->surface->bitsperpixel)) {
				goto out;
			}
			s_fillbox(s, 0, 0, img->w, img->h, s_rgbcolor(s, 197, 198, 199));
			s_putboxrgba(s, 0, 0, img->w, img->h, img->rgba);
			s_scalebox(temp->surface, img->w, img->h, s->vbuf, 18, 18, tbuf);
			s_putbox(temp->surface, 4, 2 + fy - 16, 18, 18, tbuf);
			s_surface_destroy(s);
out:			s_image_uninit(img);
			s_free(file);
			s_free(tbuf);
		}

		s_font_set_str(font, sprog->name);
		s_font_get_glyph(font);
		s_image_get_handler(font->glyph.img);
		s_putboxrgba(temp->surface, fx, fy - font->glyph.yMax, font->glyph.img->w, font->glyph.img->h, font->glyph.img->rgba);

		s_handler_init(&hndl);
		hndl->type = MOUSE_HANDLER;
		hndl->mouse.x = fx;
		hndl->mouse.y = fy - font->glyph.img->h + font->glyph.img->handler->y;
		hndl->mouse.w = fw - 50;
		hndl->mouse.h = font->glyph.img->h;
		hndl->mouse.p = start_menu_handler;
		hndl->mouse.button = MOUSE_LEFTBUTTON;
		hndl->data = sprog;
		s_handler_add(temp, hndl);

		if (sprog->type == SMENU_MENU) {
			s_font_set_str(font, ">");
			s_font_get_glyph(font);
			s_image_get_handler(font->glyph.img);
			s_putboxrgba(temp->surface, fw - font->glyph.img->w - 10, fy - font->glyph.yMax, font->glyph.img->w, font->glyph.img->h, font->glyph.img->rgba);
		}

		fy += 22;
	}

	s_font_uninit(font);

	s_window_atexit(temp, start_menu_atexit);
	temp->data = tbar_data;

	s_window_show(temp);
	s_window_main(temp);
}

void start_menu_setup (s_window_t *twindow, s_config_t *cfg)
{
	int i;
	int j;
	char *str;
	char *ptr;
	char *tmp;
	s_config_cat_t *cat;
	s_config_var_t *var;
	smenu_prog_t *sprog;
        tbar_data_t *tbar_data;

        tbar_data = (tbar_data_t *) twindow->data;

	i = 0;
	while (!s_list_eol(cfg->category, i)) {
		cat = (s_config_cat_t *) s_list_get(cfg->category, i++);
		if (strcmp(cat->name, "taskbar_prog") == 0) {
			j = 0;
			while (!s_list_eol(cat->variable, j)) {
				sprog = (smenu_prog_t *) s_calloc(1, sizeof(smenu_prog_t));
				s_list_init(&(sprog->progs));

				var = (s_config_var_t *) s_list_get(cat->variable, j++);

				sprog->name = strdup(var->name);
				tmp = strdup(var->value);
				str = tmp;
				ptr = strchr(str, '|');
				*ptr = '\0';
				sprog->icon = strdup(str);
				str = ptr + 1;
				ptr = strchr(str, '|');
				*ptr = '\0';
				sprog->exec = strdup(str);
				str = ptr + 1;
				sprog->menu = strdup(str);
				s_free(tmp);

				if ((sprog->name != NULL) && (*(sprog->name) != '\0')) {
					if ((sprog->exec != NULL) && (*(sprog->exec) != '\0')) {
						sprog->type = SMENU_PROG;
					} else {
						sprog->type = SMENU_MENU;
					}
					if ((sprog->menu != NULL) && (*(sprog->menu) != '\0')) {
						smenu_prog_t *sp;
						if ((sp = start_menu_list_find_menu(sprog, tbar_data->tbar_smenu->progs)) != NULL) {
							s_list_add(sp->progs, sprog, -1);
						} else {
							goto add_top;
						}
					} else {
add_top:					s_list_add(tbar_data->tbar_smenu->progs, sprog, -1);
					}
				} else {
					s_free(sprog->name);
					s_free(sprog->icon);
					s_free(sprog->exec);
					s_free(sprog->menu);
					s_free(sprog->progs);
					s_free(sprog);
				}
			}
		}
	}
}
