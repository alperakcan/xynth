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
#include "GuiSurface.h"

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Class constructor
 *
 * Constructor for the GUI primitive object that creates a new virtual surface
 * of the given dimension.
 *
 * \param width Specifies the width of the virtual surface
 * \param height Specifies the height of the virtual surface
 * \param bpp Specifies the number of bit per pixels for the virtual surface
 */
GuiSurface::GuiSurface(int width, int height, int bpp)
{
	virtBuf = (char *) s_calloc(width * height, ((bpp - 1) / 8 + 1));
	s_surface_create_from_data(&pSurface, width, height, bpp, virtBuf);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Class destructor
 *
 * Constructor for the GUI primitive object that attaches to an existing
 * Xynth surface. Virtual buffer of the surface is initialized to NULL.
 *
 * \param pSurface Pointer to the surface descriptor that is being attached to
 */
GuiSurface::GuiSurface(s_surface_t *pSurface)
{
	this->pSurface = pSurface;
	this->virtBuf = NULL;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Class destructor
 *
 * Destructor of the GUI surface object. Also releases a virtual buffer if it
 * was created.
 */
GuiSurface::~GuiSurface()
{
	if (virtBuf != NULL) {
		s_free(virtBuf);
		s_surface_destroy(pSurface);
	}
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Get surface width
 * \return Returns the width of the surface in pixels
 */
int GuiSurface::getWidth()
{
	return pSurface->width;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Get surface height
 * \return Returns the height of the surface in pixels
 */
int GuiSurface::getHeight()
{
	return pSurface->height;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Get surface bits per pixel
 * \return Returns the number of bits per pixel of the surface
 */
int GuiSurface::getBitsPerPixel()
{
	return pSurface->bitsperpixel;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Set pixel RGB
 *
 * Writes the given RGB coefficients to a certain pixel
 *
 * \param x X coordinate of the destination pixel
 * \param y Y coordinate of the destination pixel
 * \param rgb RGB coefficients to be written to the destination pixel
 */
void GuiSurface::setPixel(int x, int y, int rgb)
{
	s_setpixel(pSurface, x, y, rgb);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Draw a line
 *
 * Draws a line on the surface.
 *
 * \param x1 X coordinate of the starting point of the line
 * \param y1 Y coordinate of the starting point of the line
 * \param x2 X coordinate of the ending point of the line
 * \param y2 Y coordinate of the ending point of the line
 * \param rgb RGB coefficients of the line color
 */
void GuiSurface::drawLine(int x1, int y1, int x2, int y2, int rgb)
{
	s_line(pSurface, x1, y1, x2, y2, rgb);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief
 *
 * \return
 */
void GuiSurface::drawCircle(int x1, int y1, int radius, int rgb)
{
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief
 *
 * \return
 */
void GuiSurface::fillRect(s_rect_t *pRect, int rgb)
{
	s_fillbox(pSurface, pRect->x, pRect->y, pRect->w, pRect->h, rgb);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief
 *
 * \return
 */
void GuiSurface::drawImage(int x, int y, s_image_t *pBitmap)
{
	s_putboxrgba(pSurface, x, y, pBitmap->w, pBitmap->h, pBitmap->rgba);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief
 *
 * \return
 */
void GuiSurface::drawImagePart(int destX, int destY,
								s_rect_t *imgPart, s_image_t *pBitmap)
{
	#ifdef DEBUG
	printf("GuiSurface::drawImagePart: dest (%d, %d, %d, %d) src (%d, %d, %d, %d)\n",
			destX, destY, pBitmap->w, pBitmap->h, imgPart->x, imgPart->y, imgPart->w, imgPart->h);
	#endif

	s_putboxpartrgba(pSurface, destX, destY,
					imgPart->w, imgPart->h,
					pBitmap->w, pBitmap->h,
					pBitmap->rgba, imgPart->x, imgPart->y);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief
 *
 * \return
 */
void GuiSurface::drawPrimitive(int x1, int y1)
{
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief
 *
 * \return
 */
int GuiSurface::makeColor(int r, int g, int b)
{
	return s_rgbcolor(pSurface, r, g, b);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief
 *
 * \return
 */
int GuiSurface::makeColor(RGB_COLOR *rgb)
{
	return s_rgbcolor(pSurface, rgb->red, rgb->green, rgb->blue);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief
 *
 * \return
 */
RGB_COLOR GuiSurface::stringToRGB(const char *str)
{
	unsigned int r=0, g=0, b=0;
	sscanf(str, "%02x%02x%02x", &r, &g, &b);

	RGB_COLOR ret = {r, g, b};
	return ret;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief
 *
 * \return
 */
void GuiSurface::copySurface(GuiSurface *pSrc)
{
	copySurface(0, 0, pSrc);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief
 *
 * \return
 */
void GuiSurface::copySurface(int x, int y, GuiSurface *pSrc)
{
	s_putbox(pSurface, x, y, pSrc->pSurface->width,
				pSrc->pSurface->height, pSrc->pSurface->vbuf);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief
 *
 * \return
 */
void GuiSurface::copySurfacePart(int destX, int destY,
									s_rect_t *srcPart, GuiSurface *pSrc)
{
	s_putboxpart(pSurface, destX, destY,
				srcPart->w, srcPart->h,
				pSrc->pSurface->width, pSrc->pSurface->height,
				pSrc->pSurface->vbuf,
				srcPart->x, srcPart->y);
}
