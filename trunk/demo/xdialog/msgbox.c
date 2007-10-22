
#include <stdio.h>
#include <stdlib.h>
#include <xynth.h>
#include <widget.h>

#include "xdialog.h"

int xd_msgbox_callback (int argc, char *argv[])
{
	int x;
	int y;
	int width;
	int height;
	char *text;
	
	w_window_t *window;
	w_frame_t *frame;
	w_textbox_t *textbox;
	w_button_t *button;

	text = argv[0];
	height = atoi(argv[1]);
	width = atoi(argv[2]);
	
	w_window_init(&window, WINDOW_MAIN, NULL);

	x = (window->window->surface->width - width) / 2;
	y = (window->window->surface->height - height) / 2;
	w_window_set_coor(window, x, y, width, height);
	
	w_frame_init(window, &frame, FRAME_PANEL | FRAME_RAISED, window->object);
	w_object_move(frame->object, 0, 0, width, height);
	w_object_show(frame->object);
	
	w_object_show(window->object);
	s_window_show(window->window);
	s_window_main(window->window);
	
	return 0;
}

xd_widget_t xd_msgbox = {
	"msgbox",
	3,
	xd_msgbox_callback,
	"<text> <height> <width>",
};
