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

void SSize::sizeSet (int w, int h)
{
	sizeW = w;
	sizeH = h;
}

SSize::SSize (int w, int h)
{
	sizeSet(w, h);
}

SSize::~SSize (void)
{
}

void SRect::rectSet (int x, int y, int w, int h)
{
	rectX = x;
	rectY = y;
	rectW = w;
	rectH = h;
}

SRect::SRect (int x, int y, int w, int h)
{
	rectSet(x, y, w, h);
}

SRect::SRect (const SRect &copy)
{
	*this = copy;
}

SRect::~SRect (void)
{
}

void SRect::operator = (const SRect &copy)
{
	if (this == &copy) {
		return;
	}

	rectX = copy.rectX;
	rectY = copy.rectY;
	rectW = copy.rectW;
	rectH = copy.rectH;
}
