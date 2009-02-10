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
#include <xynth.h>
#include <memory.h>
#include <string.h>

#include "GuiPrimitive.h"

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Class constructor
 *
 * Implements default constructor for the GUI primitive object.
 * Initializes selection state to UNSELECTED and primitive coordinates to zeros.
 * The primitive is visible by default.
 *
 * \param idResource Uniquie primitive identifier.
 * \param pFrame Pointer to a parent frame containing the primitive.
 */
GuiPrimitive::GuiPrimitive(int idResource, GuiFrame *pFrame)
{
	this->idResource = idResource;
	selection = UNSELECTED;
	lastselection = UNSELECTED;

	drawArea.x = drawArea.y = 0;
	drawArea.w = drawArea.h = 0;

	touchArea.x = touchArea.y = 0;
	touchArea.w = touchArea.h = 0;

	pHolder = pFrame;
	isVisible = true;

	if(s_thread_mutex_init(&itemMutex) != 0)
		itemMutex = NULL;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Class destructor
 *
 * Implements a destructor for the GUI primitive object.
 * Also destroys the primitive mutex.
 */
GuiPrimitive::~GuiPrimitive()
{
	if(itemMutex) s_thread_mutex_destroy(itemMutex);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Set touch area coordinates
 *
 * Sets coordinates of the touchable area of the given primitive. Coordinates
 * should be set relatively to the upper left corner of the parent frame that
 * contains the primitive. The frame uses the primitive touch area to implement
 * the touchscreen user interface.
 *
 * \param touchArea Rectangle structure containing coordinates of the
 * touchable area.
 */
void GuiPrimitive::setTouchArea(s_rect_t &touchArea)
{
	lock();
	this->touchArea = touchArea;
	unlock();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Get touch area coordinates
 *
 * Retrieves coordinates of the touchable area of the given primitive. The
 * returned coordinates are set relatively to the parent frame that contains
 * the primitive.
 *
 * \param touchArea Reference to the rectangle structure that receives
 * coordinates of the touchable area.
 */
void GuiPrimitive::getTouchArea(s_rect_t &touchArea)
{
	lock();
	touchArea = this->touchArea;
	unlock();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Set primitive position
 *
 * Updates coordinates of the primitive in the parent frame, in relation to the
 * upper left corner of the frame. If the frame has a virtual dimension greater
 * than the displayed one, the primitive coordinates are still set relatively
 * to the frame upper left corner, regardless of the current viewpoint offset.
 *
 * Note that position change does not trigger automatic primitive redrawing,
 * so user must call the redraw() method separately.
 *
 * \param drawArea Rectangle structure containing coordinates of the new
 * position.
 */
void GuiPrimitive::setPosition(s_rect_t &drawArea)
{
	lock();
	this->drawArea = drawArea;
	unlock();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Get primitive position
 *
 * Retrieves coordinates of the primitive in the parent frame, in relation to
 * the upper left corner of the frame.
 *
 * \param drawArea Reference to the rectangle structure that receives the
 * coordinates of the current position.
 */
void GuiPrimitive::getPosition(s_rect_t &drawArea)
{
	lock();
	drawArea = this->drawArea;
	unlock();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Set selection
 *
 * Updates selection state of the primitive. The onSelectionChanged() virtual
 * function will be called when selection is updated notifying the primitive
 * implementation about the change.
 *
 * Note that selection change does not usually trigger automatic primitive
 * redrawing, so user must call the redraw() method separately.
 *
 * \param type Specifies the new selection state.
 */
void GuiPrimitive::setSelection(SELECTION type)
{
	lock();
	#ifdef DEBUG
	printf("GuiPrimitive::setSelection: resId %d set to %d\n",
			idResource, type);
	#endif

	// is the selection changed?
	if(selection != type)
	{
		// backup the last selection in case the user wants to release the mouse outside the frame
		if (type == PRESSED && (selection == SELECTED || selection == UNSELECTED)) {
			lastselection = selection;
		}
		// backup the last selection in case the user wants to release the mouse outside the frame
		if (type == UNKNOWN)
			selection = lastselection;
		else
			selection = type;

		unlock();
		onSelectionChanged(type);
	}
	else unlock();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Get selection
 *
 * Retrives the current selection state of the primitive.
 * \return Returns the current selection state.
 */
GuiPrimitive::SELECTION GuiPrimitive::getSelection()
{
	lock();
	SELECTION sel = selection;
	unlock();

	return sel;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Redaw primitive fully
 *
 * Performs a full primitive redrawing. Parent frame is requested to redraw the
 * current primitive location area.
 */
void GuiPrimitive::redraw()
{
	// request the frame redraw at the given primitive area
	pHolder->redrawFrame(drawArea);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Set visibility
 *
 * Updates the primitive visibility.
 *
 * Note that visibility change does not trigger automatic primitive redrawing,
 * so user must call the redraw() method separately.
 *
 * \param flag Specifies if the primitive is visible or not. When set to false,
 * the primitive implementation should ignore all repainting requests.
 */
void GuiPrimitive::setVisible(bool flag)
{
	lock();
	isVisible = flag;
	unlock();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Get visibility
 *
 * Retrieve current visibility state of the given primitive
 * \return Returns true if the primitive is visible
 */
bool GuiPrimitive::getVisible()
{
	lock();
	bool ret = isVisible;
	unlock();

	return ret;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Initialize function of the primitive interface
 *
 * Given virtual function is called by the parent frame when the primitive is
 * being initialized. Specific implementation of the primitive may override
 * this function if such notification is requred.
 */
void GuiPrimitive::onInitialize()
{
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Painting function of the primitive interface
 *
 * Given virtual function is called by the parent frame whenever a primitive
 * redraw is required. Specific implementation of the primitive must override
 * this function to perform appropriate painting.
 *
 * \param drawSurface Pointer to a graphical surface where the primitive should
 * draw itself to.
 *
 * \param paintArea The rectangular area of the parent frame surface that
 * should be re-painted by the primitive.
 */
void GuiPrimitive::onPaint(GuiSurface *drawSurface, s_rect_t &paintArea)
{
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Selection change function of the primitive interface
 *
 * Given virtual function is called by the parent container whenever the
 * selection is being changed. Selection change inside a frame typically
 * results of a user touchsreen manipulation. Specific implementation of the
 * primitive may override this function if such notification is required.
 *
 * \param type Specifies the new selection state of the given primitive
 */
void GuiPrimitive::onSelectionChanged(SELECTION type)
{
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Timer function of the primitive interface
 *
 * Given virtual function is called by the parent container whenever a timer
 * associated with the primitive is fired. Specific implementation of the
 * primitive may override this function if a timer callback is required.
 *
 * \param idTimer Contains the timer identifier that was specified during
 * creation of the timer.
 */
void GuiPrimitive::onTimer(int idTimer)
{
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Lock the primitive
 *
 * Implements the thread safety of the given primitive.
 */
void GuiPrimitive::lock()
{
	if(itemMutex) s_thread_mutex_lock(itemMutex);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Unlock the primitive
 *
 * Implements the thread safety of the given primitive.
 */
void GuiPrimitive::unlock()
{
	if(itemMutex) s_thread_mutex_unlock(itemMutex);
}
