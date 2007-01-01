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

void desktop_background (s_window_t *window, char *file)
{
        int x;
        int y;
        char *vbuf;
        s_surface_t s;
        int imagex = 0;
        int imagey = 0;
        s_image_t *img;
        dtop_data_t *dtop_data;
	
        dtop_data = (dtop_data_t *) window->data;

	s_image_init(&img);
	s_image_img(file, img);

	if (img->w < window->surface->buf->w) {
		imagex = (window->surface->buf->w - img->w) / 2;
	}
	if (img->h < window->surface->buf->h) {
		imagey = (window->surface->buf->h - img->h) / 2;
	}

        vbuf = (char *) s_malloc(window->surface->bytesperpixel * img->w * img->h + 1);
        s_getsurfacevirtual(&s, img->w, img->h, window->surface->bitsperpixel, vbuf);
        s_fillbox(&s, 0, 0, img->w, img->h, s_rgbcolor(&s, dtop_data->bg_color >> 0x10 & 0xFF,
                                                           dtop_data->bg_color >> 0x08 & 0xFF,
                                                           dtop_data->bg_color >> 0x00 & 0xFF));
        s_putboxrgba(&s, 0, 0, img->w, img->h, img->rgba);

	if (dtop_data->scale_img) {
		s_scalebox(window->surface, img->w, img->h, vbuf, window->surface->buf->w, window->surface->buf->h, window->surface->vbuf);
	} else {
		x = (window->surface->buf->w - img->w) / 2;
		y = (window->surface->buf->h - img->h) / 2;
		s_putbox(window->surface, 0, 0, img->w, img->h, vbuf);
	}

	s_free(vbuf);

	s_image_uninit(img);
}

void desktop_icon_handler (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	int p = 0;
	dtop_prog_t *dtopp;
        dtop_data_t *dtop_data;
        dtop_data = (dtop_data_t *) window->data;

#if 0
        s_window_quit(window);
        return;
#endif

        if (event->mouse->clicks != 2) {
		return;
	}
	while (!s_list_eol(dtop_data->progs, p)) {
		dtopp = (dtop_prog_t *) s_list_get(dtop_data->progs, p++);
		if (dtopp == (dtop_prog_t *) handler->data) {
			desktop_self_system(dtopp->exec);
			break;
		}
	}
}

void desktop_icon (s_window_t *window, dtop_prog_t *dtopp, s_font_t *font)
{
        char *file;
	s_image_t *img;
	s_handler_t *hndl;
        dtop_data_t *dtopd;

        dtopd = (dtop_data_t *) window->data;

	file = (char *) s_malloc(sizeof(char) * (strlen(DESKTOPDIR "/img/icons/") + strlen(dtopp->icon) + 1));
	sprintf(file, "%s/img/icons/%s", DESKTOPDIR, dtopp->icon);

	s_image_init(&img);
	s_image_img(file, img);
	s_image_get_handler(img);
	s_putboxrgba(window->surface, dtopp->x, dtopp->y + (32 - img->h) / 2, img->w, img->h, img->rgba);

	s_font_set_str(font, dtopp->name);
	s_font_get_glyph(font);
	s_putboxrgba(window->surface, dtopp->x + (img->w - font->glyph.img->w) / 2, dtopp->y + 35, font->glyph.img->w, font->glyph.img->h, font->glyph.img->rgba);

	s_handler_init(&hndl);
	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = dtopp->x + img->handler->x;
	hndl->mouse.y = dtopp->y + img->handler->y;
	hndl->mouse.w = img->handler->w;
	hndl->mouse.h = img->handler->h;
	hndl->mouse.c = desktop_icon_handler;
	hndl->mouse.button = MOUSE_LEFTBUTTON;
	hndl->data = dtopp;
	s_handler_add(window, hndl);

	s_image_uninit(img);
	s_free(file);
}

void desktop_icons (s_window_t *window)
{
	int p = 0;
	s_font_t *font;
	dtop_prog_t *dtopp;
        dtop_data_t *dtop_data;
        dtop_data = (dtop_data_t *) window->data;

       	s_font_init(&font, "arial.ttf");
	s_font_set_size(font, 11);
	s_font_set_rgb(font, (dtop_data->text_color >> 0x16) & 0xff,
	                     (dtop_data->text_color >> 0x08) & 0xff,
			     (dtop_data->text_color >> 0x00) & 0xff);

	while (!s_list_eol(dtop_data->progs, p)) {
		dtopp = (dtop_prog_t *) s_list_get(dtop_data->progs, p++);
		desktop_icon(window, dtopp, font);
	}

	s_font_uninit(font);
}

void desktop_atexit (s_window_t *window)
{
	dtop_data_t *dtop_data;
	dtop_data = (dtop_data_t *) window->data;
	while (!s_list_eol(dtop_data->progs, 0)) {
		dtop_prog_t *dtopp = (dtop_prog_t *) s_list_get(dtop_data->progs, 0);
		s_free(dtopp->name);
		s_free(dtopp->icon);
		s_free(dtopp->exec);
		s_free(dtopp);
		s_list_remove(dtop_data->progs, 0);
	}
	s_free(dtop_data->progs);
	s_free(dtop_data->bg_image);
	s_free(dtop_data);
}

void desktop_desktop_handler (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
}

void desktop_start (s_window_t *window, s_config_t *cfg)
{
	int i;
	int j;
	char *str;
	char *ptr;
	char *tmp;
	char *bgpath;
	s_handler_t *hndl;
	dtop_prog_t *dtopp;
	s_config_cat_t *cat;
	s_config_var_t *var;
	dtop_data_t *dtop_data;

	s_window_set_coor(window, WINDOW_NOFORM, 0, 0, window->surface->width, window->surface->height - 30);
	s_window_set_alwaysontop(window, -1);
	s_window_set_resizeable(window, 0);

	s_free(window->surface->vbuf);
	window->surface->width = window->surface->buf->w;
	window->surface->height = window->surface->buf->h;
	window->surface->vbuf = (unsigned char *) s_malloc(window->surface->width * window->surface->height * window->surface->bytesperpixel);

	dtop_data = (dtop_data_t *) s_calloc(1, sizeof(dtop_data_t));
	s_list_init(&(dtop_data->progs));
	
	i = 0;
	while (!s_list_eol(cfg->category, i)) {
		cat = s_list_get(cfg->category, i++);
		if (strcmp(cat->name, "desktop") == 0) {
			j = 0;
			while (!s_list_eol(cat->variable, j)) {
				var = s_list_get(cat->variable, j++);
				if (strcmp(var->name, "show_desktop") == 0) {
					dtop_data->show_desktop = atoi(var->value);
				} else if (strcmp(var->name, "show_icons") == 0) {
					dtop_data->show_icons = atoi(var->value);
				} else if (strcmp(var->name, "scale_img") == 0) {
					dtop_data->scale_img = atoi(var->value);
				} else if (strcmp(var->name, "bg_image") == 0) {
					dtop_data->bg_image = strdup(var->value);
				} else if (strcmp(var->name, "bg_color") == 0) {
					char *ptr = strchr(var->value, 'x');
					if (ptr != NULL) {
						ptr++;
					} else {
						ptr = var->value;
					}
					dtop_data->bg_color = (s_image_hex2int(ptr + 0) << 0x10) |
					                      (s_image_hex2int(ptr + 2) << 0x08) |
							      (s_image_hex2int(ptr + 4) << 0x00);
				} else if (strcmp(var->name, "text_color") == 0) {
					char *ptr = strchr(var->value, 'x');
					if (ptr != NULL) {
						ptr++;
					} else {
						ptr = var->value;
					}
					dtop_data->text_color = (s_image_hex2int(ptr + 0) << 0x10) |
					                        (s_image_hex2int(ptr + 2) << 0x08) |
							        (s_image_hex2int(ptr + 4) << 0x00);
				}
			}
		} else if (strcmp(cat->name, "desktop_prog") == 0) {
			j = 0;
			while (!s_list_eol(cat->variable, j)) {
				var = s_list_get(cat->variable, j++);
				dtopp = (dtop_prog_t *) s_calloc(1, sizeof(dtop_prog_t));

				dtopp->name = strdup(var->name);
				tmp = strdup(var->value);
				str = tmp;
				ptr = strchr(str, '|');
				*ptr = '\0';
				dtopp->icon = strdup(str);
				str = ptr + 1;
				ptr = strchr(str, '|');
				*ptr = '\0';
				dtopp->exec = strdup(str);
				str = ptr + 1;
				ptr = strchr(str, '|');
				*ptr = '\0';
				dtopp->x = atoi(str);
				str = ptr + 1;
				dtopp->y = atoi(str);
				s_free(tmp);

				if ((dtopp->name != NULL) && (*(dtopp->name) != '\0') &&
				    (dtopp->icon != NULL) && (*(dtopp->icon) != '\0') &&
				    (dtopp->exec != NULL) && (*(dtopp->exec) != '\0')) {
					s_list_add(dtop_data->progs, dtopp, -1);
				} else {
					s_free(dtopp->name);
					s_free(dtopp->icon);
					s_free(dtopp->exec);
					s_free(dtopp);
				}
			}
		}
	}

	window->data = (void *) dtop_data;
	s_window_atexit(window, desktop_atexit);
	
	s_fillbox(window->surface, 0, 0,
	                           window->surface->width, window->surface->height,
				   s_rgbcolor(window->surface, dtop_data->bg_color >> 0x10 & 0xFF,
                                                               dtop_data->bg_color >> 0x08 & 0xFF,
							       dtop_data->bg_color >> 0x00 & 0xFF));

	if ((dtop_data->bg_image != NULL) &&
	    (*(dtop_data->bg_image) != '\0')) {
		bgpath = (char *) s_malloc(sizeof(char) * (strlen(DESKTOPDIR "/img/wpaper/") + strlen(dtop_data->bg_image) + 1));
		sprintf(bgpath, "%s/img/wpaper/%s", DESKTOPDIR, dtop_data->bg_image);
		desktop_background(window, bgpath);
		s_free(bgpath);
	}

	if (dtop_data->show_icons == 1) {
                desktop_icons(window);
        }

	s_handler_init(&hndl);
	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = 0;
	hndl->mouse.y = 0;
	hndl->mouse.w = window->surface->width;
	hndl->mouse.h = window->surface->height;
	hndl->mouse.p = desktop_desktop_handler;
	hndl->mouse.button = MOUSE_RIGHTBUTTON;
	hndl->data = 0;
	s_handler_add(window, hndl);
	
	if (dtop_data->show_desktop == 1) {
		s_window_show(window);
	}
	s_window_main(window);
}
