/***************************************************************************
    begin                : Sun Feb 16 2003
    copyright            : (C) 2003 - 2006 by Alper Akcan
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

#ifndef THEME_H_
#define THEME_H_

typedef enum {
	TOP_L	= 0x0,
	TOP_1,
	TOP_2,
	TOP_3,
	TOP_4,
	TOP_5,
	TOP_R,
	LEFT,
	RIGHT,
	BTM_L,
	BTM,
	BTM_R,
	FORM_MAX
} THEME_FORM;

typedef enum {
	CLOSE	= 0x0,
	MAXIMIZE,
	HIDE,
	MENU,
	BTNS_MAX
} THEME_BTN;

typedef enum {
	INACTIVE,
	ACTIVE,
	PRESSED
} THEME_STATE;

struct s_theme_s {
	int title_full;
	int text_color[2];
	int text_v_off[2];
	s_image_t form[2][FORM_MAX];
	s_image_t button[3][BTNS_MAX];
	struct {
		int h;
		int w;
		int w_;
	} form_min;
	s_font_t *font[2];
};

/* theme.c */
void s_server_theme_init (void);
void s_server_theme_set (char *name);
void s_server_theme_uninit (void);

#endif /*THEME_H_*/
