/**************************++*************************************************
    begin                : Tue Sep 21 2004
    copyright            : (C) 2004 - 2006 by Alper Akcan
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
#include <xynth.h>
#include <widget.h>

int main (int argc, char *argv[])
{
	SFrame *frame;
	SWindow *window;
	SLayout *layout;

	int row;
	int col;
	int shape;
	int shadow;
	int linewidth;
	int midlinewidth;

	window = new SWindow();

	layout = new SLayout(window, 0x0c * 3, 16);
	layout->layoutSetColSpacing(2);
	layout->layoutSetRowSpacing(2);

        row = 0;
	for (shape = 1; shape <= 0x0c; shape++) {
		for (shadow = 0x10; shadow <= 0x30; shadow += 0x10) {
			col = 0;
			for (linewidth = 1; linewidth <= 4; linewidth++) {
				for (midlinewidth = 0; midlinewidth <= 3; midlinewidth++) {
					frame = new SFrame(NULL, shape | shadow);
					frame->frameSetLineWidth(linewidth);
					frame->frameSetMidLineWidth(midlinewidth);
					layout->layoutInsertObject(frame, row, col);
					col++;
				}
			}
			row++;
		}
	}

	window->windowSetCoor(100, 100, 400, 400);
	window->windowSetTitle("Demo - %s", argv[0]);

	window->windowShow();
	window->windowMain();

	delete window;

	return 0;
}
