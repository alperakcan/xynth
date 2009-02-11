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

#include "GuiScrollBar.h"
#include "GuiImageCache.h"

//#define DEBUG

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Class constructor
 *
 * Default constructor for the GUI scroll bar objects. Initializes current
 * scroll bar position and size to 0.
 *
 * Initially, all scroll bar images are not loaded, so one has to initialize
 * them separately via loadBarFill() and loadBackground() and loadBarEdges()
 * methods. Automatic image initialization is performed through GuiTemplate.
 *
 * \param idResource Unique primitive identifier.
 * \param pFrame Pointer to a parent frame containing the primitive.
 */
GuiScrollBar::GuiScrollBar(int idResource, GuiFrame* pFrame)
				: GuiPrimitive(idResource, pFrame)
{
	imgBackground = NULL;
	imgBar = NULL;
	imgEdgeStart = NULL;
	imgEdgeStop = NULL;

	relativePosition = 0;
	relativeSize = 0;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Class destructor
 *
 * Destructor for the scroll bar primitive objects.
 * Unloads currently loaded scroll bar images, if any.
 */
GuiScrollBar::~GuiScrollBar()
{
	unload();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Load scroll bar indicator image
 *
 * Loads an image for the face indicator (or knob) of the scroll bar primitive.
 *
 * \param fileName Name of the file containing the scroll bar face image.
 * Parameter should either contain a full path or be relative to the
 * working directory.
 *
 * \return Returns true if the scroll bar face image was loaded successfully.
 */
bool GuiScrollBar::loadBarFill(const char *fileName)
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
 * Loads an image for the static background of the scroll bar primitive.
 *
 * \param fileName Name of the file containing the scroll bar background image.
 * Parameter should either contain a full path or be relative to the
 * working directory.
 *
 * \return Returns true if the scroll bar background image was loaded
 * successfully.
 */
bool GuiScrollBar::loadBackground(const char *fileName)
{
	lock();
	if(imgBackground != NULL) GuiImageCache::closeImage(imgBackground);

	imgBackground = GuiImageCache::openImage(fileName);

	unlock();
	return imgBackground != NULL;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Load scroll bar edge images
 *
 * Loads images for painting the indicator edges in the scroll bar primitive.
 * Starting and trailing edge attach on both sides the indicator image and
 * have constant dimensions.
 *
 * \param startName Name of the file containing the starting edge image.
 * \param stopName Name of the file containing the trailing edge image.
 *
 * Parameters should either contain a full path or be relative to the
 * working directory.
 *
 * \return Returns true if both scroll bar edge images were loaded
 * successfully.
 */
bool GuiScrollBar::loadBarEdges(const char* startName, const char* stopName)
{
	lock();

	if(imgEdgeStart != NULL) GuiImageCache::closeImage(imgEdgeStart);
	if(imgEdgeStop != NULL) GuiImageCache::closeImage(imgEdgeStop);

	imgEdgeStart = GuiImageCache::openImage(startName);
	imgEdgeStop = GuiImageCache::openImage(stopName);

	unlock();
	return (imgEdgeStart != NULL) && (imgEdgeStop != NULL);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Unload scroll bar images
 *
 * Releases resources associated with loaded scroll bar images.
 */
void GuiScrollBar::unload()
{
	lock();

	if(imgBar)GuiImageCache::closeImage(imgBar);
	if(imgBackground) GuiImageCache::closeImage(imgBackground);
	if(imgEdgeStart) GuiImageCache::closeImage(imgEdgeStart);
	if(imgEdgeStop) GuiImageCache::closeImage(imgEdgeStop);

	imgBar = NULL;
	imgBackground = NULL;
	imgEdgeStart = NULL;
	imgEdgeStop = NULL;

	unlock();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Set scroll bar current parameters
 *
 * Sets current position and size of the scroll bar indicator.
 *
 * \param relPos Indicator relative position affects the resulting location of
 * the face indicator image. Position must be a positive integer
 * between 0 and 100.
 *
 * \param relSize Indicator relative size affects the resulting length of the
 * face indicator image. Size must be a positive integer between 0 and 100.
 */
void GuiScrollBar::setPosition(int relPos, int relSize)
{
	lock();

	#ifdef DEBUG
	printf("GuiScrollBar::setPosition: %d, %d\n", relPos, relSize);
	#endif

	relativePosition = relPos;
	relativeSize = relSize;

	if(relativePosition < 0) relativePosition = 0;
	else if(relativePosition > 100) relativePosition = 100;

	if(relativeSize < 0) relativeSize = 0;
	else if(relativeSize > 100) relativeSize = 100;

	unlock();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Painting function of the primitive interface
 *
 * Called by the parent frame whenever a primitive redraw is required.
 * Only vertical scroll bar alignment is currently supported.
 *
 * \param drawSurface Pointer to a graphical surface where the primitive should
 * draw itself to.
 * \param paintArea The rectangular area of the parent frame surface that
 * should be re-painted by the primitive.
 */
void GuiScrollBar::onPaint(GuiSurface *drawSurface, s_rect_t &paintArea)
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

	#ifdef DEBUG
	printf("GuiScrollBar::onPaint: draw %d %d %d %d int %d %d %d %d\n",
			drawArea.x, drawArea.y, drawArea.w, drawArea.h,
			surfRect.x, surfRect.y, surfRect.w, surfRect.h);
	#endif

	s_rect_t relRect = { surfRect.x - drawArea.x, surfRect.y - drawArea.y,
						surfRect.w, surfRect.h };

	#ifdef DEBUG
	printf("GuiScrollBar::onPaint: relative %d %d %d %d\n",
			relRect.x, relRect.y, relRect.w, relRect.h);
	#endif

	// draw the progress background image
	if(imgBackground)
		drawSurface->drawImagePart(surfRect.x, surfRect.y, &relRect, imgBackground);

	// calculate the filling bar rectangle
	int barLen = relativeSize * drawArea.h / 100;
	int barPos = relativePosition * drawArea.h / 100;

	#ifdef DEBUG
	printf("GuiScrollBar::onPaint: size %d %d %d %d\n",
			relativeSize, relativePosition, barLen, barPos);
	#endif

	// nothing to draw - return now;
	if(barLen == 0)
	{
		unlock();
		return;
	}

	if(barPos + barLen > drawArea.h)
		barPos = drawArea.h - barLen;

	s_rect_t barArea = { drawArea.x, drawArea.y + barPos, drawArea.w, barLen };
	s_rect_t fillArea = barArea;

	if(imgEdgeStart)
	{
		// free some space for an opening bar tip image
		fillArea.y += imgEdgeStart->h;
		fillArea.h -= imgEdgeStart->h;
	}

	if(imgEdgeStop)
	{
		// free some space for a terminating bar tip image
		fillArea.h -= imgEdgeStop->h;
	}

	#ifdef DEBUG
	printf("GuiScrollBar::onPaint: bar %d %d %d %d fill %d %d %d %d\n",
			barArea.x, barArea.y, barArea.w, barArea.h, fillArea.x, fillArea.y, fillArea.w, fillArea.h);
	#endif

	// calculate the filling bar redraw area
	// by intersecting its full rectangle with the update rectangle
	if(s_region_rect_intersect(&paintArea, &fillArea, &surfRect))
	{
		unlock();
		return;
	}

	#ifdef DEBUG
	printf("GuiScrollBar::onPaint: surf %d %d %d %d\n",
			surfRect.x, surfRect.y, surfRect.w, surfRect.h);
	#endif

	relRect.x = surfRect.x - drawArea.x;
	relRect.y = surfRect.y - drawArea.y;
	relRect.w = surfRect.w;
	relRect.h = surfRect.h;

	// draw the scrollbar fill area
	drawSurface->drawImagePart(surfRect.x, surfRect.y, &relRect, imgBar);

	s_rect_t tipArea;
	if(imgEdgeStart)
	{
		tipArea.x = barArea.x;
		tipArea.y = barArea.y;
		tipArea.w = barArea.w;
		tipArea.h = imgEdgeStart->h;

		if(s_region_rect_intersect(&paintArea, &tipArea, &surfRect) == 0)
		{
			relRect.x = surfRect.x - tipArea.x;
			relRect.y = surfRect.y - tipArea.y;
			relRect.w = surfRect.w;
			relRect.h = surfRect.h;

			#ifdef DEBUG
			printf("GuiScrollBar::onPaint: start %d %d %d %d, %d %d %d %d\n",
					surfRect.x, surfRect.y, surfRect.w, surfRect.h,
					relRect.x, relRect.y, relRect.w, relRect.h);
			#endif

			// draw the scrollbar tip
			drawSurface->drawImagePart(surfRect.x, surfRect.y, &relRect, imgEdgeStart);
		}
	}

	if(imgEdgeStop)
	{
		tipArea.x = barArea.x;
		tipArea.y = barArea.y + barArea.h - imgEdgeStop->h;
		tipArea.w = barArea.w;
		tipArea.h = imgEdgeStop->h;

		if(s_region_rect_intersect(&paintArea, &tipArea, &surfRect) == 0)
		{
			relRect.x = surfRect.x - tipArea.x;
			relRect.y = surfRect.y - tipArea.y;
			relRect.w = surfRect.w;
			relRect.h = surfRect.h;

			#ifdef DEBUG
			printf("GuiScrollBar::onPaint: stop %d %d %d %d, %d %d %d %d\n",
					surfRect.x, surfRect.y, surfRect.w, surfRect.h,
					relRect.x, relRect.y, relRect.w, relRect.h);
			#endif

			// draw the scrollbar tip
			drawSurface->drawImagePart(surfRect.x, surfRect.y, &relRect, imgEdgeStop);
		}
	}

	unlock();
}
