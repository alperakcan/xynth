/***************************************************************************
    begin                : Tue Sep 22 2007
    copyright            : (C) 2007 - 2008 by Alper Akcan
    email                : alper.akcan@gmail.com
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
#include <xynth.h>
#include <widget.h>

#include "xdialog.h"

static const int pbutton_width = 50;
static const int pbutton_height = 25;
static const int object_space = 5;
static int xd_yesno_return = 0;

void xd_yesno_pbutton_yes_clicked (w_pushbutton_t *pbutton)
{
	xd_yesno_return = 0;
	w_window_quit(pbutton->object->window);
}

void xd_yesno_pbutton_no_clicked (w_pushbutton_t *pbutton)
{
	xd_yesno_return = 1;
	w_window_quit(pbutton->object->window);
}

int xd_yesno_callback (int argc, char *argv[])
{
	int x;
	int y;
	int width;
	int height;
	char *text;
	
	w_window_t *window;
	w_frame_t *frame;
	w_textbox_t *textbox;
	w_pushbutton_t *pbutton;

	text = argv[0];
	height = atoi(argv[1]);
	width = atoi(argv[2]);
	
	w_window_init(&window, WINDOW_TYPE_MAIN | WINDOW_TYPE_NOFORM, NULL);

	x = (window->window->surface->linear_buf_width - width) / 2;
	y = (window->window->surface->linear_buf_height - height) / 2;
	w_window_set_coor(window, x, y, width, height);
	
	w_frame_init(window, &frame, FRAME_PANEL | FRAME_SUNKEN, window->object);
	w_object_move(frame->object, 0, 0, width, height);
	w_object_show(frame->object);
	
	w_textbox_init(window, &textbox, frame->object);
	w_textbox_set_properties(textbox->object, TEXTBOX_WRAP);
	w_textbox_set_style(textbox->object, FRAME_PANEL, FRAME_SUNKEN);
	w_textbox_set_str(textbox->object, text);
	w_object_move(textbox->object, object_space, object_space, width - (object_space * 2), height - pbutton_height - (object_space * 3));
	w_object_show(textbox->object);
	
	w_pushbutton_init(window, &pbutton, frame->object);
	w_pushbutton_set_text(pbutton->object, "Yes");
	w_pushbutton_set_clicked(pbutton->object, xd_yesno_pbutton_yes_clicked);
	w_object_move(pbutton->object, (width - ((pbutton_width * 2) + object_space)) / 2, height - pbutton_height - object_space, pbutton_width, pbutton_height);
	w_object_show(pbutton->object);
	
	w_pushbutton_init(window, &pbutton, frame->object);
	w_pushbutton_set_text(pbutton->object, "No");
	w_pushbutton_set_clicked(pbutton->object, xd_yesno_pbutton_no_clicked);
	w_object_move(pbutton->object, ((width - ((pbutton_width * 2) + object_space)) / 2) + pbutton_width + object_space, height - pbutton_height - object_space, pbutton_width, pbutton_height);
	w_object_show(pbutton->object);
	
	w_object_show(window->object);
	w_window_main(window);
	
	return xd_yesno_return;
}

xd_widget_t xd_yesno = {
	"yesno",
	3,
	xd_yesno_callback,
	"<text> <height> <width>",
};
