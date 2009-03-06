/***************************************************************************
    begin                : Fri Oct 1 2004
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

#include "xynth_.h"
#include "widgetoo.h"

void SLayoutCell::objectDraw (void)
{
	objectFillBox(0, 0, objectRectBuffer.rectW, objectRectBuffer.rectH, objectRGBColor(220, 220, 220));
}

void SLayoutCell::objectGeometry (int x, int y, int w, int h)
{
	objectSetRectBuffer(x, y, w, h);
	objectSetRectContents(x, y, w, h);
}

SLayoutCell::SLayoutCell (void) : SObject()
{
}

SLayoutCell::~SLayoutCell (void)
{
	delete layoutCellObject;
}

void SLayout::layoutSetColSpacing (int spacing)
{
	if (spacing >= 0) {
		layoutColSpacing = spacing;
	}
}

void SLayout::layoutSetRowSpacing (int spacing)
{
	if (spacing >= 0) {
		layoutRowSpacing = spacing;
	}
}

void SLayout::layoutInsertObject (SObject *object, int row, int col)
{
	layoutCells[row][col].layoutCellObject = object;
	layoutCells[row][col].layoutCellObject->objectWindow = objectWindow;
	layoutCells[row][col].layoutCellObject->objectSetParent(&layoutCells[row][col]);
	layoutCells[row][col].layoutCellObject->objectSetObjectWindow(objectWindow);
}

void SLayout::objectDraw (void)
{
	int lr;
	int lc;

	S_SURFACE_MODE mode;
	mode = objectWindow->surface->mode;
	objectWindow->surface->mode = SURFACE_VIRTUAL;

	objectFillBox(0, 0, objectRectBuffer.rectW, objectRectBuffer.rectH, objectRGBColor(220, 220, 220));
        lr = layoutRows;
	while (lr--) {
		lc = layoutCols;
		while (lc--) {
			layoutCells[lr][lc].draw();
		}
	}

	objectWindow->surface->mode = mode;
	expose(0, 0, objectRectBuffer.rectW, objectRectBuffer.rectH);
}

void SLayout::objectGeometry (int x, int y, int w, int h)
{
	int c;
	int r;
	int w_;
	int h_;
	int lx;
	int ly;
	int lw;
	int lh;
	double lhf;
	double lwf;
	
	w_ = w - ((layoutCols - 1) * layoutColSpacing);
	h_ = h - ((layoutRows - 1) * layoutRowSpacing);
	
	objectSetRectBuffer(x, y, w, h);
	objectSetRectContents(x, y, w, h);

	lx = x;
	lwf = 0;
	for (c = 0; c < layoutCols; c++) {
		ly = y;
		lhf = 0;

		lw = (int) ((double) w_ * layoutColsWeight[c]);
		lwf += (((double) w_ * layoutColsWeight[c]) - (double) lw);
		if (lwf >= 0.999999) {
			lw++;
			lwf -= 1.0;
		}

		for (r = 0; r < layoutRows; r++) {
			lh = (int) ((double) h_ * layoutRowsWeight[r]);
			lhf += (((double) h_ * layoutRowsWeight[r]) - (double) lh);
			if (lhf >= 0.999999) {
				lh++;
				lhf -= 1.0;
			}

			layoutCells[r][c].geometry(lx, ly, lw, lh);
			if ((layoutCells[r][c].objectRectBuffer.rectW > lw) ||
			    (layoutCells[r][c].objectRectBuffer.rectH > lh)) {
				w += (layoutCells[r][c].objectRectBuffer.rectW - lw); 
				h += (layoutCells[r][c].objectRectBuffer.rectH - lh); 
				geometry(x, y, w, h);
				return;
			}

			ly = ly + lh + layoutRowSpacing;
		}

		lx = lx + lw + layoutColSpacing;
	}
}

void SLayout::objectSetObjectWindow (s_window_t *window)
{
	int rows = layoutRows;
	int cols = layoutCols;
	while (rows--) {
		cols = layoutCols;
		while (cols--) {
			layoutCells[rows][cols].objectWindow = window;
		}
	}
}

SLayout::SLayout (SObject *parent, int rows, int cols) : SObject(parent)
{
	layoutRows = rows;
	layoutCols = cols;
	
	layoutRowsWeight = new double[layoutRows];
	while (rows--) {
		layoutRowsWeight[rows] = 1.0 / layoutRows;
	}
	layoutColsWeight = new double[layoutCols];
	while (cols--) {
		layoutColsWeight[cols] = 1.0 / layoutCols;
	}
	
	layoutCells = new SLayoutCell*[layoutRows];
	rows = layoutRows;
	while (rows--) {
		layoutCells[rows] = new SLayoutCell[layoutCols];
		cols = layoutCols;
		while (cols--) {
			layoutCells[rows][cols].objectWindow = objectWindow;
		}
	}
}

SLayout::~SLayout (void)
{
	delete[] layoutRowsWeight;
	delete[] layoutColsWeight;

	while (layoutRows--) {
		delete[] layoutCells[layoutRows];
	}
	delete[] layoutCells;
}
