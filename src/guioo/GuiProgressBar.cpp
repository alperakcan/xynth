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

#include "GuiProgressBar.h"
#include "GuiImageCache.h"

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Class constructor
 *
 * Default constructor for the GUI progress bar objects. Initializes current
 * progress attribute to 0 and maximum progress to 100.
 *
 * Background and progress indicator images are not loaded, one must load them
 * using loadBackground() and loadBar() methods. Automatic image initialization
 * is performed via GuiTemplate class.
 *
 * \param idResource Uniquie primitive identifier.
 * \param pFrame Pointer to a parent frame containing the primitive.
 */
GuiProgressBar::GuiProgressBar(int idResource, GuiFrame* pFrame)
		: GuiPrimitive(idResource, pFrame)
{
	imgBar = NULL;
	imgBackground = NULL;

	curProgress = 0;
	maxProgress = 100;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Class destructor
 *
 * Destructor for the progress bar primitive objects.
 * Unloads currently loaded progress bar images, if any.
 */
GuiProgressBar::~GuiProgressBar()
{
	unload();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Load progress indicator image
 *
 * Loads an image for the progress indicator face of the progress bar primitive.
 *
 * \param fileName Name of the file containing the progress indicator face
 * image. Parameter should either contain a full path or be relative to the
 * working directory.
 *
 * \return Returns true if the progress indicator face image was loaded
 * successfully.
 */
bool GuiProgressBar::loadBar(const char *fileName)
{
	lock();
	if(imgBar != NULL) GuiImageCache::closeImage(imgBar);

	imgBar = GuiImageCache::openImage(fileName);

	unlock();
	return imgBar != NULL;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Load background image
 *
 * Loads an image for the static background of the progress bar primitive.
 *
 * \param fileName Name of the file containing the progress bar background
 * image. Parameter should either contain a full path or be relative to the
 * working directory.
 *
 * \return Returns true if the progress bar background image was loaded
 * successfully.
 */
bool GuiProgressBar::loadBackground(const char *fileName)
{
	lock();
	if(imgBackground != NULL) GuiImageCache::closeImage(imgBackground);

	imgBackground = GuiImageCache::openImage(fileName);

	unlock();
	return imgBackground != NULL;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Unload images
 *
 * Releases resources associated with loaded progress bar images.
 */
void GuiProgressBar::unload()
{
	lock();

	if(imgBar) GuiImageCache::closeImage(imgBar);
	if(imgBackground) GuiImageCache::closeImage(imgBackground);

	imgBar = NULL;
	imgBackground = NULL;

	unlock();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Set progress bar maximum value
 *
 * Sets maximum level of the progress bar. Currently displayed progress is
 * calculated in relation to its maximum value.
 *
 * \param maxProgress Maximum level of the progress bar. Default value is 100.
 */
void GuiProgressBar::setMax(int maxProgress)
{
	lock();
	this->maxProgress = maxProgress;
	unlock();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Set current progress value
 *
 * Sets current progress value of the progress bar. Progress bar indicator
 * is drawn correspondingly to its current and maximum values.
 *
 * \param pos Specifies current progress value, set to 0 for no progress and
 * to maxProgress for a complete i.
 */
void GuiProgressBar::setProgress(int pos)
{
	lock();
	curProgress = pos;
	unlock();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Painting function of the primitive interface
 *
 * Called by the parent frame whenever a primitive redraw is required.
 *
 * NB! only horizontal layout is currently supported
 *
 * \param drawSurface Pointer to a graphical surface where the primitive should
 * draw itself to.
 * \param paintArea The rectangular area of the parent frame surface that
 * should be re-painted by the primitive.
 */
void GuiProgressBar::onPaint(GuiSurface *drawSurface, s_rect_t &paintArea)
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

	// draw the progress background image
	if(imgBackground)
		drawSurface->drawImagePart(surfRect.x, surfRect.y, &relRect, imgBackground);

	// calculate the progress bar rectangle
	s_rect_t barArea = drawArea;
	if(maxProgress > 0) barArea.w = drawArea.w * curProgress / maxProgress;
	if(s_region_rect_intersect(&paintArea, &barArea, &surfRect))
	{
		unlock();
		return;
	}

	relRect.x = surfRect.x - drawArea.x;
	relRect.y = surfRect.y - drawArea.y;
	relRect.w = surfRect.w;
	relRect.h = surfRect.h;

	// draw the progress bar
	drawSurface->drawImagePart(surfRect.x, surfRect.y, &relRect, imgBar);
	unlock();
}
