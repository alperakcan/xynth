/***************************************************************************
 *   Copyright            : (c) 2007-2008 Artec Design LLC                 *
 *                        : (c) 2007-2008 Andrei Birjukov                  *
 *                        : (c) 2007-2008 Anti Sullin                      *
 *   Contact              : andrei.birjukov at artecdesign dot ee          *
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
#include "GuiRectangle.h"

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Class constructor
 *
 * Default constructor for the GUI rectangle objects. Initializes the
 * forerground color to RGB (255, 255, 255) and background to RGB (0, 0, 0)
 *
 * \param idResource Uniquie primitive identifier.
 * \param pFrame Pointer to a parent frame containing the primitive.
 */
GuiRectangle::GuiRectangle(int idResource, GuiFrame* pFrame)
		: GuiPrimitive(idResource, pFrame)
{
	drawGradient = false;

	colForeGround.red = colForeGround.green = colForeGround.blue = 255;
	colBackGround.red = colBackGround.green = colBackGround.blue = 0;
	colGradient.red = colGradient.green = colGradient.blue = 0;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Class desctructor
 *
 * Destructor for the rectangle primitive objects.
 */
GuiRectangle::~GuiRectangle()
{
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Set foreground color
 *
 * Sets foreground color of the rectangle object. Foreground color is used
 * for filling the solid rectangle.
 *
 * Border drawing feature is currently not implemented
 */
void GuiRectangle::setForeGround(const char* rgb)
{
	lock();
	colForeGround = GuiSurface::stringToRGB(rgb);
	unlock();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Set background color
 *
 * Sets background color of the rectangle object. Background color is used
 * for drawing borders
 *
 * Border drawing is currently not implemented.
 */
void GuiRectangle::setBackGround(const char* rgb)
{
	lock();
	colBackGround = GuiSurface::stringToRGB(rgb);
	unlock();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Set gradient
 *
 * Sets gradient color of the rectangle object. Gradient is drawn between the
 * background and gradient colors.
 *
 * Gradient feature is currently not implemented
 */
void GuiRectangle::setGradient(const char* rgb)
{
	lock();

	if(rgb == NULL) drawGradient = false;
	else
	{
		colGradient = GuiSurface::stringToRGB(rgb);
		drawGradient = true;
	}

	unlock();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Painting function of the primitive interface
 *
 * Called by the parent frame whenever a primitive redraw is required.
 * \param drawSurface Pointer to a graphical surface where the primitive should
 * draw itself to.
 * \param paintArea The rectangular area of the parent frame surface that
 * should be re-painted by the primitive.
 */
void GuiRectangle::onPaint(GuiSurface *drawSurface, s_rect_t &paintArea)
{
	lock();
	if(!isVisible)
	{
		unlock();
		return;
	}

	s_rect_t absRect;
	if(s_region_rect_intersect(&paintArea, &drawArea, &absRect))
	{
		unlock();
		return;
	}

	drawSurface->fillRect(&absRect, drawSurface->makeColor(&colForeGround));

	unlock();
}
