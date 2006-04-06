/***************************************************************************
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

#include "../lib/xynth_.h"
#include "widget.h"

int SObject::objectRGBColor (int r, int g, int b)
{
	return s_rgbcolor(objectWindow->surface, r, g, b);
}

void SObject::objectFillBox (int x, int y, int w, int h, int color)
{
	s_rect_t coor;
	s_rect_t rect = (s_rect_t) {x, y, w, h};
	s_rect_t clip = (s_rect_t) {0, 0, objectRectBuffer.rectW, objectRectBuffer.rectH};
	if (s_rect_intersect(&rect, &clip, &coor) == 0) {
		s_fillbox(objectWindow->surface, objectRectBuffer.rectX + coor.x, objectRectBuffer.rectY + coor.y, coor.w, coor.h, color);
	}
}

void SObject::objectSetSizeMin (int w, int h)
{
	if (w < 0) {
		w = 0;
	}
	if (h < 0) {
		h = 0;
	}
	if ((objectRectBuffer.rectW - objectRectContents.rectW) > w) {
		w += ((objectRectBuffer.rectW - objectRectContents.rectW) - w);
	}
	if ((objectRectBuffer.rectH - objectRectContents.rectH) > h) {
		h += ((objectRectBuffer.rectH - objectRectContents.rectH) - h);
	}
	objectSizeBufferMin.sizeSet(w, h);
	objectSizeContentsMin.sizeSet(w - (objectRectBuffer.rectW - objectRectContents.rectW),
	                              h - (objectRectBuffer.rectH - objectRectContents.rectH));
}

void SObject::objectSetSizeMax (int w, int h)
{
	objectSizeBufferMax.sizeSet(w, h);
	objectSizeContentsMax.sizeSet(w - (objectRectBuffer.rectW - objectRectContents.rectW),
	                              h - (objectRectBuffer.rectH - objectRectContents.rectH));
}

void SObject::objectSetRectBuffer (int x, int y, int w, int h)
{
	objectRectBuffer.rectSet(x, y, w, h);
}

void SObject::objectSetRectContents (int x, int y, int w, int h)
{
	objectRectContents.rectSet(x, y, w, h);
}

void SObject::objectSetObjectWindow (s_window_t *window)
{
	objectWindow = window;
}

void SObject::objectDelFromParent (void)
{
	if (objectParent == NULL) {
		return;
	}
	objectParent->objectChild = NULL;
}

void SObject::objectSetParent (SObject *parent)
{
	objectDelFromParent();
	objectParent = parent;
	if (objectParent != NULL) {
		objectParent->objectChild = this;
		objectWindow = objectParent->objectWindow;
	}
}

void SObject::draw (void)
{
	objectDraw();
	if (objectChild != NULL) {
		objectChild->draw();
	}
}

void SObject::expose (int x, int y, int w, int h)
{
	s_rect_t coor;
	s_rect_t rect = (s_rect_t) {x, y, w, h};
	s_rect_t clip = (s_rect_t) {0, 0, objectRectBuffer.rectW, objectRectBuffer.rectH};

	if ((objectWindow->surface->mode & SURFACE_REAL) &&
	    (s_rect_intersect(&rect, &clip, &coor) == 0)) {
		coor.x += (objectRectBuffer.rectX + objectWindow->surface->buf.x);
		coor.y += (objectRectBuffer.rectY + objectWindow->surface->buf.y);
		s_surface_changed(objectWindow, &coor);
	}
}

void SObject::geometry (int x, int y, int w, int h)
{
	objectGeometry(x, y, w, h);
	objectSetSizeMin(objectSizeBufferMin.sizeW, objectSizeBufferMin.sizeH);
	objectSetSizeMax(objectSizeBufferMax.sizeW, objectSizeBufferMax.sizeH);

	if ((objectRectBuffer.rectW < objectSizeBufferMin.sizeW) ||
	    (objectRectBuffer.rectH < objectSizeBufferMin.sizeH)) {
		w = (objectRectBuffer.rectW < objectSizeBufferMin.sizeW) ? objectSizeBufferMin.sizeW : objectRectBuffer.rectW;
		h = (objectRectBuffer.rectH < objectSizeBufferMin.sizeH) ? objectSizeBufferMin.sizeH : objectRectBuffer.rectH;
		geometry(x, y, w, h);
		return;
	}

	if (objectParent != NULL) {
		if (objectParent->objectResizeMode == SObject::FreeResize) {
			if ((objectRectBuffer.rectW > objectParent->objectRectContents.rectW) ||
			    (objectRectBuffer.rectH > objectParent->objectRectContents.rectH)) {
				x = objectParent->objectRectBuffer.rectX;
				y = objectParent->objectRectBuffer.rectY;
				w = (objectRectBuffer.rectW > objectParent->objectRectContents.rectW) ? objectRectBuffer.rectW + (objectParent->objectRectBuffer.rectW - objectParent->objectRectContents.rectW) : objectParent->objectRectBuffer.rectW;
				h = (objectRectBuffer.rectH > objectParent->objectRectContents.rectH) ? objectRectBuffer.rectH + (objectParent->objectRectBuffer.rectH - objectParent->objectRectContents.rectH) : objectParent->objectRectBuffer.rectH;
				objectParent->geometry(x, y, w, h);
				return;
			}
		}
	}
	
	if (objectChild != NULL) {
		objectChild->geometry(objectRectContents.rectX, objectRectContents.rectY, objectRectContents.rectW, objectRectContents.rectH);
	}
}

SObject::SObject (SObject *parent, Alignment alignment, ResizeMode resizemode)
{
	objectChild = NULL;
	objectParent = parent;
	objectAlignment = alignment;
	objectResizeMode = resizemode;

	objectSetRectBuffer(0, 0, 0, 0);
	objectSetRectContents(0, 0, 0, 0);

	objectSetSizeMin(0, 0);
	objectSetSizeMax(~(1 << 31), ~(1 << 31));

	objectSetParent(parent);
}

SObject::~SObject (void)
{
	objectDelFromParent();
	delete objectChild;
}
