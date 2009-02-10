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

#include "GuiImage.h"
#include "GuiImageCache.h"

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Class constructor
 *
 * Default constructor for the GUI image objects. Image data is not loaded,
 * user must do that using loadImage() method. Automatic image initialization
 * is performed via GuiTemplate class.
 *
 * \param idResource Uniquie primitive identifier.
 * \param pFrame Pointer to a parent frame containing the primitive.
 */
GuiImage::GuiImage(int idResource, GuiFrame* pFrame)
		: GuiPrimitive(idResource, pFrame)
{
	imgData = NULL;
	layoutType = IMAGE_LAYOUT_UPPERLEFT;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Class destructor
 *
 * Also releases resources associated with a loaded image.
 */
GuiImage::~GuiImage()
{
	if(imgData != NULL) GuiImageCache::closeImage(imgData);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Load image file
 *
 * Loads an image for the face of the image primitive.
 *
 * \param fileName Name of the file containing the image.
 * Parameter should either contain a full path or be relative to the
 * working directory.
 *
 * Image implementation also uses a global image cache that increases loading
 * speed if the image is used somewhere else.
 *
 * \return Returns true if the image was loaded successfully.
 */
bool GuiImage::loadImage(const char *fileName)
{
	lock();
	if(imgData != NULL) GuiImageCache::closeImage(imgData);

	s_image_t *pImg = GuiImageCache::openImage(fileName);

	imgData = pImg;
	unlock();

	return pImg != NULL;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Unload image
 *
 * Closes the loaded image file.
 */
void GuiImage::close()
{
	lock();

	GuiImageCache::closeImage(imgData);
	imgData = NULL;

	unlock();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Set image layout
 *
 * Currently not implemented.
 */
void GuiImage::setLayout(int layoutType)
{
	lock();
	this->layoutType = layoutType;
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
void GuiImage::onPaint(GuiSurface *drawSurface, s_rect_t &paintArea)
{
	lock();
	if(!isVisible)
	{
		unlock();
		return;
	}

	s_rect_t surfRect;
	if(s_region_rect_intersect(&paintArea, &drawArea, &surfRect))
	{
		unlock();
		return;
	}

	s_rect_t relRect = { surfRect.x - drawArea.x, surfRect.y - drawArea.y,
						surfRect.w, surfRect.h };

	#ifdef DEBUG
	printf("GuiImage::onPaint: painting (%d, %d, %d, %d) at (%d, %d)\n",
			paintArea.x, paintArea.y, paintArea.w, paintArea.h,
			drawArea.x, drawArea.y);
	#endif

	drawSurface->drawImagePart(surfRect.x, surfRect.y, &relRect, imgData);
	unlock();
}
