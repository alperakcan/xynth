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

#ifndef DESKTOP_H
#define DESKTOP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <sys/unistd.h>
#include "xynth_.h"

typedef enum {
	SMENU_MENU,
	SMENU_PROG
} SMENU_TYPE;

typedef struct tbar_clock_s {
	s_rect_t rect;
	s_font_t *font;
	s_window_t *clock;
} tbar_clock_t;

typedef struct smenu_prog_s {
	char *name;
	char *icon;
	char *exec;
	char *menu;
	SMENU_TYPE type;
	s_list_t *progs;
} smenu_prog_t;

typedef struct tbar_smenu_s {
	int running;
	s_rect_t rect;
	s_image_t *img;
	s_list_t *progs;
} tbar_smenu_t;

typedef struct tbar_progs_s {
	s_rect_t rect;
	s_image_t *tbar_img;
	s_font_t *prog_font;
	s_image_t *prog_img[2];
	s_desktop_t *desktop;
} tbar_progs_t;

typedef struct tbar_data_s {
	tbar_progs_t *tbar_progs;
	tbar_clock_t *tbar_clock;
	tbar_smenu_t *tbar_smenu;
} tbar_data_t;

typedef struct dtop_prog_s {
	int x;
	int y;
	char *name;
	char *icon;
	char *exec;
} dtop_prog_t;

typedef struct dtop_data_s {
	int show_icons;
	int show_desktop;
	int scale_img;
	char *bg_image;
	unsigned int bg_color;
	unsigned int text_color;
	s_list_t *progs;
} dtop_data_t;

void desktop_background (s_window_t *window, char *file);
void desktop_icon_handler (s_window_t *window, s_event_t *event, s_handler_t *handler);
void desktop_icon (s_window_t *window, dtop_prog_t *dtopp, s_font_t *font);
void desktop_icons (s_window_t *window);
void desktop_desktop_handler (s_window_t *window, s_event_t *event, s_handler_t *handler);
void desktop_start (s_window_t *window, s_config_t *cfg);

void start_menu_atexit (s_window_t *window);
void start_menu_start (s_window_t *twindow, s_list_t *progs, int wx, int wy);
void start_menu_setup (s_window_t *twindow, s_config_t *cfg);

void taskbar_progs_handler_r (s_window_t *window, s_event_t *event, s_handler_t *handler);
void taskbar_progs_draw_client (tbar_progs_t *tbar_progs, s_surface_t *surface, s_desktop_client_t *client, int x, int y, int w, int h);
void taskbar_progs_draw (s_window_t *window);

void taskbar_start_menu_icon (s_window_t *window);
void taskbar_start_menu_handler_p (s_window_t *window, s_event_t *event, s_handler_t *handler);
void taskbar_start_menu_handler_rh (s_window_t *window, s_event_t *event, s_handler_t *handler);

void taskbar_clock_popup_atexit (s_window_t *window);
void taskbar_clock_popup_atevent (s_window_t *window, s_event_t *event);
void taskbar_clock_handler_o (s_window_t *window, s_event_t *event, s_handler_t *handler);
void taskbar_clock_draw (s_window_t *window, s_timer_t *timer);
void taskbar_clock_timer (s_window_t *window, s_timer_t *timer);

void taskbar_clean_smenu (s_list_t *list);
void taskbar_atexit (s_window_t *window);
void taskbar_atevent (s_window_t *window, s_event_t *event);
void taskbar_start (s_window_t *window, s_config_t *cfg);

int desktop_self_system (char *command);

#endif /* DESKTOP_H */
