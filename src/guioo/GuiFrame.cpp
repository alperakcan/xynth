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

#include <time.h>
#include <stdio.h>

#include "GuiFrame.h"

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Class constructor
 *
 * Default constructor for the GUI window frame object.
 * Initializes the frame geometry to zero, resets currently focused item,
 * visibility flag to true and viewpoint offset to zero. Also initializes a
 * frame mutex via Xynth thread API.
 *
 * With the above attributes, the frame is completely empty. User must either
 * manually populate the frame with instantiated GUI primitives by calling
 * addPrimitive() method, or rely on automatic GUI skin function provided
 * by GuiTemplate class.
 *
 * \param idResource Unique identifier of the window frame
 * \param pParent Pointer to a window that contains the given frame
 */
GuiFrame::GuiFrame(int idResource, GuiWindow *pParent)
{
	this->idResource = idResource;

	parentWindow = pParent;
	focusedItem = RES_ID_INVALID;

	frameArea.x = frameArea.y = 0;
	frameArea.w = frameArea.h = 0;
	offsetX = offsetY = 0;
	virtWidth = virtHeight = 0;
	updateRequired = true;
	isVisible = true;

	if(s_thread_mutex_init(&frameMutex) != 0)
		frameMutex = NULL;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Class destructor
 *
 * Destructor for the GUI window frame object.
 * Deletes any contained remaining primitives which were not released
 * before for some reason. Also destroys frame mutex.
 */
GuiFrame::~GuiFrame()
{
	list<GuiPrimitive*>::iterator it;
	for(it = primitiveList.begin(); it != primitiveList.end(); it ++)
	{
		GuiPrimitive* pItem = (GuiPrimitive*)*it;
		delete pItem;
	}

	primitiveList.clear();
	if(frameMutex) s_thread_mutex_destroy(frameMutex);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Add GUI primitive to frame
 *
 * Inserts the provided primitive at the end of the primitive list of the
 * frame. The list is protected by a mutex.
 *
 * \param pItem Pointer to a GUI primitive object to be added
 * \return Returns a number of primitives currently associated with the given
 * frame.
 */
int GuiFrame::addPrimitive(GuiPrimitive *pItem)
{
	lock();

	primitiveList.push_back(pItem);
	int count = primitiveList.size();

	unlock();
	return count;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Get a primitive by identifier
 *
 * Retrieves a primitive object by its unique identifier from the list.
 * The list is protected by a mutex.
 *
 * \param idResource Identifier of a primitive to be retrieved. If multiple
 * primitives have the same identifier, the first one is returned.
 * \return Returns the first primitive object that has a provided ID.
 */
GuiPrimitive* GuiFrame::getPrimitive(int idResource)
{
	lock();
	GuiPrimitive* pRet = NULL;

	list<GuiPrimitive*>::iterator it;
	for(it = primitiveList.begin(); it != primitiveList.end(); it ++)
	{
		if(((GuiPrimitive*)*it)->getResId() == idResource)
		{
			pRet = *it;
			break;
		}
	}

	unlock();
	return pRet;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Remove a primitive by identifier
 *
 * Erases a GUI primitive from the primitive list of the frame. Note that
 * primitive object is not deleted. The first primitive with the provided
 * identifier is erased.
 *
 * \param idResource Identifier of a primitive to be removed
 * \return Returns true if the primitive was found and erased from the list.
 */
bool GuiFrame::removePrimitive(int idResource)
{
	lock();

	list<GuiPrimitive*>::iterator it;
	for(it = primitiveList.begin(); it != primitiveList.end(); it ++)
	{
		GuiPrimitive* pItem = (GuiPrimitive*)*it;
		if(pItem->getResId() == idResource)
		{
			// remove it from the list
			primitiveList.erase(it);
			delete pItem;

			// remove selection too
			if(focusedItem == idResource) focusedItem = RES_ID_INVALID;

			unlock();
			return true;
		}
	}

	unlock();
	return false;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Set viewpoint offset
 *
 * Sets the viewpoint coordinates of the given frame. Viewpoint offset is only
 * relevant for frames that have virtual dimensions larger than the area
 * displayed in the parent window. Given function is useful for implementing
 * scrollable GUI elements. Offset must be always positive.
 *
 * \param x X coordinate of the viewpoint offset in a virtual frame.
 * \param y Y coordinate of the viewpoint offset in a virtual frame.
 */
void GuiFrame::setOffset(int x, int y)
{
	lock();

	offsetX = x;
	offsetY = y;

	unlock();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Get current viewpoint offset
 *
 * \param pX Pointer to an integer that receives current X coordinate of the
 * viewpoint offset. If NULL value is specifed, X coordinate is not returned.
 * \param pY Pointer to an integer that receives current Y coordinate of the
 * viewpoint offset. If NULL value is specifed, Y coordinate is not returned.
 */
void GuiFrame::getOffset(int *pX, int *pY)
{
	if(pX) *pX = offsetX;
	if(pY) *pY = offsetY;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Set frame coordinates
 *
 * Updates coordinates of the given window frame. Coordinates are calculated in
 * relation to the upper left corner of the parent window.
 * The visible are of the frame is bounded by the given rectangle.
 * Note that the frame is not redrawn automatically.
 *
 * \param frameArea Frame coordinate rectangle
 */
void GuiFrame::setGeometry(s_rect_t &frameArea)
{
	lock();
	this->frameArea = frameArea;
	unlock();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Get frame coordinates
 *
 * Reads coordinates of the given window frame. Coordinates are returned in
 * relation to the uppoer left corner of the parent window.
 * The visible are of the frame is bounded by the given rectangle.
 *
 * \param frameArea Reference to the destination frame coordinate rectangle
 */
void GuiFrame::getGeometry(s_rect_t &frameArea)
{
	lock();
	frameArea = this->frameArea;
	unlock();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Get virtual frame size
 *
 * Retrieves the virtual size of the given frame. Virtual size can be larger
 * than the actual frame geometry, this may be used to implement scrollable
 * interface elements.
 *
 * \param pW Pointer to an integer that receives the width of the virtual frame.
 * If NULL value is specified, the width is not returned.
 * \param pH Pointer to an integer that receives the height of the virtual frame.
 * If NULL value is specified, the height is not returned.
 */
void GuiFrame::getVirtSize(int *pW, int *pH)
{
	if(pW) *pW = virtWidth;
	if(pH) *pH = virtHeight;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Set virtual frame size
 *
 * Updates virtual size of the frame. Note that the frame is not redrawn
 * automatically.
 *
 * \param w New width of the virtual frame
 * \param h New height of the virtual frame
 */
void GuiFrame::setVirtSize(int w, int h)
{
	virtWidth = w;
	virtHeight = h;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Set visibility flag
 *
 * Sets the frame visibility flag.
 *
 * \param flag Specifies if the frame is displayed or hidden. Hidden frames
 * ignore all painting requests.
 */
void GuiFrame::setVisible(bool flag)
{
	isVisible = flag;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Get frame size
 *
 * Retrieves the actual size of the given frame that is visible in the window.
 *
 * \param pW Pointer to an integer that receives the width of the frame.
 * If NULL value is specified, the width is not returned.
 * \param pH Pointer to an integer that receives the height of the frame.
 * If NULL value is specified, the height is not returned.
 */
void GuiFrame::getSize(int *pW, int *pH)
{
	if(pW) *pW = frameArea.w;
	if(pH) *pH = frameArea.h;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Redraw the frame
 *
 * Performs the full frame redrawing.
 *
 * \return Returns true if the redraw operation is successful.
 */
bool GuiFrame::redrawFrame()
{
	s_rect_t redrawRect = { offsetX, offsetY, frameArea.w, frameArea.h };
	return redrawFrame(redrawRect);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Redraw the frame partially
 *
 * Performs a partial redrawing of the given frame limited by the rectangular
 * area. Redraw request is passed to the parent window that in turn executes
 * painting procedures for all frame in the given area.
 *
 * \return Returns true if the redraw operation is successful.
 */
bool GuiFrame::redrawFrame(s_rect_t updateArea)
{
	// offset the frame position and calculate absolute coords
	updateArea.x = updateArea.x - offsetX + frameArea.x;
	updateArea.y = updateArea.y - offsetY + frameArea.y;

	s_rect_t redrawRect;
	// calculate the intersection with the frame view area, abort if no drawing needed
	if(s_region_rect_intersect(&updateArea, &frameArea, &redrawRect)) return false;

	updateRequired = true;	// set the update flag now

	// call the window redraw function
	parentWindow->redrawWindow(redrawRect);
	return true;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Painting function of the frame interface
 *
 * Paint method is called by the parent window when frame redraw is required
 * in the given area.
 *
 * \param pSurface Pointer to a graphical surface where the frame should
 * draw itself to.
 * \param paintArea The rectangular area of the surface that should be
 * re-painted by the frame. Note that coordinates of the painting area
 * are relative to the upper left corner of the provided surface.
 */
void GuiFrame::onPaint(GuiSurface *pSurface, s_rect_t &paintArea)
{
	if(!isVisible) return;

	// calculate the update rectangle, abort if no intersection
	s_rect_t absRect;
	if(s_region_rect_intersect(&paintArea, &frameArea, &absRect)) return;

	lock();

	// calculate relative coordinates in the target pSurface
	s_rect_t relRect = { absRect.x, absRect.y, absRect.w, absRect.h };

	list<GuiPrimitive*>::iterator itItem;

	// loop through all primitives in the list, painting them onto the virtual surface
	for(itItem = primitiveList.begin(); itItem != primitiveList.end(); itItem ++)
	{
		#ifdef DEBUG
		clock_t start = clock();
		#endif

		GuiPrimitive* pItem = (GuiPrimitive*)*itItem;

		// calculate real item position from its "virtual" rectangle
		s_rect_t oldPos = pItem->drawArea;
		s_rect_t newPos = {	oldPos.x - offsetX + frameArea.x,
							oldPos.y - offsetY + frameArea.y,
							oldPos.w, oldPos.h };

		// set temporary item coordinates relative to pSurface
		pItem->drawArea = newPos;

		pItem->onPaint(pSurface, relRect);
		pItem->drawArea = oldPos;

		#ifdef DEBUG
		printf("GuiFrame::onPaint: drawn resource %d (%d, %d, %d, %d) in %d ms\n",
			pItem->getResId(), newPos.x, newPos.y, newPos.w, newPos.h,
			(int)((clock() - start) / (CLOCKS_PER_SEC / 1000)));
		#endif
	}

	unlock();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Initialize frame surface
 *
 * GuiFrame implementation does not provide a buffered frame surface. Given
 * method initializes all available GUI primitives contained by the frame.
 *
 * \return Returns true if all components were initialized successfully.
 */
bool GuiFrame::initSurface(int virtWidth, int virtHeight)
{
	this->virtWidth = virtWidth;
	this->virtHeight = virtHeight;

	updateRequired = true;

	// initialize all primitives in the frame
	list<GuiPrimitive*>::iterator it;
	for(it = primitiveList.begin(); it != primitiveList.end(); it ++)
		((GuiPrimitive*)*it)->onInitialize();

	return true;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Get a touchable primitive for given coordinates
 *
 * Retrieves a GUI primitive that has a touch region covering the given point.
 * Each primitive contained by frame may have a non-zero touch region. This
 * area is used for implementing of the touchscreen user interface.
 *
 * \param x X coordinate of the touch point
 * \param y Y coordinate of the touch point
 * \return Returns a primitive object, or NULL if no touchable primitive exists.
 */
GuiPrimitive* GuiFrame::getPrimitiveTouch(int x, int y)
{
	lock();

	// offset given coordinates with current frame position
	x -= (frameArea.x - offsetX);
	y -= (frameArea.y - offsetY);

	list<GuiPrimitive*>::reverse_iterator itItem;
	// reverse loop through all primitives (last ones are on top)
	for(itItem = primitiveList.rbegin(); itItem != primitiveList.rend(); itItem ++)
	{
		GuiPrimitive* pItem = (GuiPrimitive*)*itItem;
		// ignore invisible elements
		if (!pItem->getVisible()) continue;

		s_rect_t itemArea;
		pItem->getTouchArea(itemArea);
		// test if the point is within the rect
		if((x >= itemArea.x) && (x < (itemArea.x + itemArea.w)) &&
			(y >= itemArea.y) && (y < (itemArea.y + itemArea.h)))
		{
			// found a valid element, return it
			unlock();
			return pItem;
		}
	}

	unlock();
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Lock frame
 *
 * Locks the frame mutex, implemented for a thread safe operation.
 */
void GuiFrame::lock()
{
	if(frameMutex) s_thread_mutex_lock(frameMutex);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Unlock frame
 *
 * Unlocks the frame mutex, implemented for a thread safe operation.
 */
void GuiFrame::unlock()
{
	if(frameMutex) s_thread_mutex_unlock(frameMutex);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Start a new timer
 *
 * Starts a new window timer that will fire every 'ms' milliseconds and call
 * the timer event handler. This method is primarily intended for use by GUI
 * primitives contained by the frame.
 *
 * \param ms Timer period in milliseconds
 * \param idTimer Unique identifier of the timer. This value is passed to a
 * timer event handler and may be used by the caller.
 *
 * \param pItem Pointer to a GUI primitive that is associated with the given
 * timer. If the value is not NULL, onTimer() function of the specified
 * primitive is called when the timer fires.
 *
 * \return Returns a pointer to the new timer descriptor.
 */
s_timer_t* GuiFrame::startTimer(int ms, int idTimer, GuiPrimitive *pItem)
{
	return parentWindow->startTimer(ms, idTimer, pItem);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Stop an existing timer
 *
 * Terminates the given timer.
 *
 * \return Returns true if the timer was successfully stopped and timer
 * resourced were released.
 */
bool GuiFrame::stopTimer(s_timer_t* pTimer)
{
	return parentWindow->stopTimer(pTimer);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Set focused primitive
 *
 * Updates the identifier of the focused primitive of the given frame.
 * Also updates the selection state of the previously selected primitive.
 *
 * \param idPrimitive Unique identifier of the primitive that is now in focus.
  */
void GuiFrame::setFocusedItem(int idPrimitive)
{
	// remove focus from a previously selected item in the same frame
	if(focusedItem != RES_ID_INVALID && focusedItem != idPrimitive) {
		GuiPrimitive *focusedPrimitive = getPrimitive(focusedItem);
		if (focusedPrimitive)
			focusedPrimitive->setSelection(GuiPrimitive::UNSELECTED);
	}

	// set new focus
	// NB! don't forget to set selection status of the new focused primitive
	// to GuiPrimitive::SELECTED in the caller function, otherwise
	// unselecting gets filtered out!
	focusedItem = idPrimitive;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Class constructor
 *
 * Default constructor for the GUI window frame object.
 */
GuiBufferedFrame::GuiBufferedFrame(int idResource, GuiWindow *pParent)
				: GuiFrame(idResource, pParent)
{
	virtSurface = NULL;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Class destructor
 *
 * Destructor of the buffered frame. Also destroys the virtual painting surface.
 */
GuiBufferedFrame::~GuiBufferedFrame()
{
	if(virtSurface) delete virtSurface;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Initialize frame surface
 *
 * Initializes virtual frame surface and paints all contained GUI primitives
 * to that surface. GUI elements are also initialized.
 *
 * \return Returns true if all primitives are initialized and painted
 * successfully.
 */
bool GuiBufferedFrame::initSurface(int virtWidth, int virtHeight)
{
	GuiSurface *parentSurface = parentWindow->getSurface();
	virtSurface = new GuiSurface(virtWidth, virtHeight,
									parentSurface->getBitsPerPixel());

	s_rect_t rc = {0, 0, virtWidth, virtHeight};

	// fill with blackness
	virtSurface->fillRect(&rc, 0);

	// call parent initialization
	GuiFrame::initSurface(virtWidth, virtHeight);

	// perform an inital painting of all frame primitives
	list<GuiPrimitive*>::iterator itItem;
	s_rect_t virtRect = { 0, 0, frameArea.w, frameArea.h };
	for(itItem = primitiveList.begin(); itItem != primitiveList.end(); itItem ++)
	{
		GuiPrimitive* pItem = (GuiPrimitive*)*itItem;
		pItem->onPaint(virtSurface, virtRect);
	}

	return virtSurface != NULL;
}


////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Painting function of the frame interface
 *
 * Paint method is called by the parent window when frame redraw is required
 * in the given area.
 *
 * \param pSurface Pointer to a graphical surface where the frame should
 * draw itself to.
 * \param paintArea The rectangular area of the surface that should be
 * re-painted by the frame. Note that coordinates of the painting area
 * are relative to the upper left corner of the provided surface.
 */
void GuiBufferedFrame::onPaint(GuiSurface *pSurface, s_rect_t &paintArea)
{
	if(!isVisible) return;

	// calculate the update rectangle, abort if no intersection
	s_rect_t absRect;
	if(s_region_rect_intersect(&paintArea, &frameArea, &absRect)) return;

	lock();

	// calculate relative coordinates in the target pSurface
	s_rect_t relRect = { absRect.x - paintArea.x, absRect.y - paintArea.y,
						absRect.w, absRect.h };

	// calculate relative coordinates in the virtual frame surface
	s_rect_t virtRect = {	absRect.x - frameArea.x + offsetX,
							absRect.y - frameArea.y + offsetY,
							absRect.w, absRect.h };

	if(updateRequired)
	{
		// copy the backround from the original paint surface
		virtSurface->copySurfacePart(offsetX, offsetY, &relRect, pSurface);

		list<GuiPrimitive*>::iterator itItem;
		// loop through all primitives in the list, painting them onto the virtual surface
		for(itItem = primitiveList.begin(); itItem != primitiveList.end(); itItem ++)
		{
			#ifdef DEBUG
			clock_t start = clock();
			#endif

			GuiPrimitive* pItem = (GuiPrimitive*)*itItem;
			pItem->onPaint(virtSurface, virtRect);

			#ifdef DEBUG
			printf("GuiBufferedFrame::onPaint: drawn resource %d (%d, %d, %d, %d) in %d ms\n",
				pItem->getResId(), virtRect.x, virtRect.y, virtRect.w, virtRect.h,
				(int)((clock() - start) / (CLOCKS_PER_SEC / 1000)));
			#endif
		}

		updateRequired = false;
	}

	// copy the virtual frame surface back onto the original surface
	pSurface->copySurfacePart(relRect.x, relRect.y, &virtRect, virtSurface);
	unlock();
}
