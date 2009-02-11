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

#include "GuiButton.h"
#include "GuiImageCache.h"

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Class constructor
 *
 * Default constructor for the GUI button objects.
 *
 * Initially, both face images associated with the button primitive are empty.
 * User must initialize images using loadReleased() and loadPressed() methods.
 * Automatic image initialization is performed via GuiTemplate class.
 *
 * \param idResource Unique primitive identifier.
 * \param pFrame Pointer to a parent frame containing the primitive.
 */
GuiButton::GuiButton(int idResource, GuiFrame* pFrame)
		: GuiPrimitive(idResource, pFrame)
{
	imgUp = NULL;
	imgDown = NULL;
}

/**
 * \brief Class destructor
 *
 * Destructor for the button primitive objects.
 * Unloads currently loaded button images, if any.
 */
GuiButton::~GuiButton()
{
	unload();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Load released button image
 *
 * Loads an image for the released face of the button primitive.
 *
 * \param fileUp Name of the file containing the released face image.
 * Parameter shoould either contain a full path or be relative to the
 * working directory.
 *
 * \return Returns true if the released button image was loaded
 * successfully.
 */
bool GuiButton::loadReleased(const char* fileUp)
{
	lock();
	if(imgUp != NULL) GuiImageCache::closeImage(imgUp);

	imgUp = GuiImageCache::openImage(fileUp);
	unlock();
	return imgUp != NULL;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Load pressed button image
 *
 * Loads an image for the pressed face of the button primitive.
 *
 * \param fileDown Name of the file containing the pressed face image.
 * Parameter should either contain a full path or be relative to the
 * working directory.
 *
 * \return Returns true if the pressed button image was loaded
 * successfully.
 */
bool GuiButton::loadPressed(const char* fileDown)
{
	lock();
	if(imgDown != NULL) GuiImageCache::closeImage(imgDown);

	imgDown = GuiImageCache::openImage(fileDown);
	unlock();
	return imgDown != NULL;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Unload button images
 *
 * Unloads both pressed and released images if they are loaded and releases
 * resources.
 */
void GuiButton::unload()
{
	lock();
	if(imgUp) GuiImageCache::closeImage(imgUp);
	imgUp = NULL;

	if(imgDown) GuiImageCache::closeImage(imgDown);
	imgDown = NULL;
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
void GuiButton::onPaint(GuiSurface *drawSurface, s_rect_t &paintArea)
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

	s_image_t *img = (selection == PRESSED) ? imgDown : imgUp;

	if(img) drawSurface->drawImagePart(surfRect.x, surfRect.y, &relRect, img);
	unlock();
}


////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Selection change function of the primitive interface
 *
 * Called by the parent container whenever the selection is being
 * changed. Selection change inside a frame typically results of
 * a user touchsreen manipulation.
 *
 * \param type Specifies the new selection state of the given primitive
 */
void GuiButton::onSelectionChanged(SELECTION type)
{
	redraw();
}
