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

#include "GuiScrollWindow.h"

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Class constructor
 *
 * Implements a default constructor of the scrollable window object.
 * Initializes the scrolling step, draging threshold and animation step to
 * their default values.
 *
 * \param idResource Specifies the unique identifier of the window
 */
GuiScrollWindow::GuiScrollWindow(int idResource)
			: GuiEffectWindow(idResource)
{
	scrollFrame = NULL;
	scrollBar = NULL;
	scrollIndUp = NULL;
	scrollIndDn = NULL;

	pointX = 0;
	pointY = 0;
	frameVirtW = 0;
	frameVirtH = 0;
	frameW = 0;
	frameH = 0;

	scrollStep = SCROLL_STEP;
	animationStep = SCROLL_STEP_ANIM;
	dragThreshold = DRAG_THRESHOLD;

	dragScroll = false;
	buttonScroll = false;
	isDragging = false;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Class destructor
 *
 * Implements a destructor of the scrollable window object.
 */
GuiScrollWindow::~GuiScrollWindow()
{
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Scroll the frame up
 *
 * Scrolls one step up. The size of the scroll step is configurable via the
 * setScrollStep() method.
 */
void GuiScrollWindow::scrollUp()
{
	if(!buttonScroll || scrollFrame == NULL) return;

	int x=0, y=0, dy=0, y0=0;

	scrollFrame->getOffset(&x, &y);

	dy = y > scrollStep ? scrollStep : y;
	y0 = dy < animationStep ? dy : animationStep;

	while(y0 <= dy)
	{
		scrollFrame->setOffset(x, y - y0);
		scrollFrame->redrawFrame();

		updateScrollData(true);

		int ddy = dy - y0;
		if(ddy == 0) break;

		y0 += (ddy < animationStep) ? ddy : animationStep;
	}
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Scroll down
 *
 * Scrolls one step down. The size of the scroll step is configurable via the
 * setScrollStep() method.
 */
void GuiScrollWindow::scrollDown()
{
	if(!buttonScroll || scrollFrame == NULL || frameVirtH <= frameH) return;

	int x=0, y=0, dy=0, y0=0;

	scrollFrame->getOffset(&x, &y);

	dy = frameVirtH - frameH - y > scrollStep ?
			scrollStep : frameVirtH - frameH - y;
	y0 = dy < animationStep ? dy : animationStep;

	while(y0 <= dy)
	{
		scrollFrame->setOffset(x, y + y0);
		scrollFrame->redrawFrame();

		updateScrollData(true);

		int ddy = dy - y0;
		if(ddy == 0) break;

		y0 += (ddy < animationStep) ? ddy : animationStep;
	}

}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Initialization event handler
 *
 * Given function of the window interface is called when the window is being
 * initialized.
 */
void GuiScrollWindow::onInitialize()
{
	scrollFrame = getFrame(FRAME_ID_SCROLL_MOVE);
	stillFrame = getFrame(FRAME_ID_SCROLL_STILL);

	if(stillFrame)
	{
		scrollBar = (GuiScrollBar*)stillFrame->getPrimitive(RES_SCROLL_BAR);
		scrollIndUp = (GuiImage*)stillFrame->getPrimitive(RES_SCROLL_IND_UP);
		scrollIndDn = (GuiImage*)stillFrame->getPrimitive(RES_SCROLL_IND_DN);
	}

	updateScrollData(false);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Enable scroll function via dragging
 *
 * Enables or disables the window scrolling functionality by means of the
 * touchscreen dragging. The window performs scrolling if the pointer has
 * moved for a distance greater than dragging threshold while keeping the
 * pressure.
 *
 * \param flag Specifies if the dragging scoll function is enabled.
 * Initially is disabled
 */
void GuiScrollWindow::enableDragScroll(bool flag)
{
	dragScroll = flag;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Enable scroll function via buttons
 *
 * Enables or disables the window scrolling functionality by means of on-screen
 * buttons. The window performs scrolling if a scroll button press is
 * detected via touchscreen interface.
 *
 * \param flag Specifies if the button scrolling function is enabled.
 * Initially is disabled.
 */
void GuiScrollWindow::enableButtonScroll(bool flag)
{
	buttonScroll = flag;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Set dragging threshold
 *
 * Updates the dragging style scrolling threshold to the specified value.
 * If the pointer is moved across the window under pressure for greater
 * distance than the specified threshold, the window scrolling is executed.
 *
 * \param threshold Drag scrolling threshold, in pixels. Initially set to 20 px.
 */
void GuiScrollWindow::setDragThreshold(int threshold)
{
	dragThreshold = threshold;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Set scrolling step
 *
 * Updates the window scrolling and animation steps to the specified values.
 * The window scrolling is performed in steps, when initiated from an on-screen
 * scroll button press.
 *
 * The viewpoint of the scrollable frame is shifted for the totalStep amount
 * of pixels in total. The scrolling is performed smoothly if the animation
 * step (animStep) is less than totalStep value. In that case, scrollable
 * frame is redrawn every time when shifted for animStep pixels before it
 * eventually reaches totalStep.
 *
 * \param totalStep Specifies the total step size of each scroll operation,
 * in pixels. Initially set to 20 px.
 * \param animStep Specifies the animation step size for smooth window frame
 * when being scrolled. Initially set to 5 px.
 */
void GuiScrollWindow::setScrollStep(int totalStep, int animStep)
{
	scrollStep = totalStep;
	animationStep = animStep;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Touch down event handler
 *
 * Given virtual function is called by the parent class every time when a new
 * touch down event is generated, i.e. the pressure is applied to the
 * touchscreen.
 *
 * \param x Specifies the X coordinate of the touch
 * \param y Specifies the Y coordinate of the touch
 */
void GuiScrollWindow::onTouchDown(int x, int y)
{
	GuiPrimitive *touchPrimitive;

	// save current coordinates
	pointX = x;
	pointY = y;

	x -= windowPos.x;
	y -= windowPos.y;

	lastTouchItem.idPrimitive = RES_ID_INVALID;
	touchItem.idPrimitive = RES_ID_INVALID;

	list<GuiFrame*>::reverse_iterator itFrame;
	for(itFrame = frameList.rbegin(); itFrame != frameList.rend(); itFrame ++)
	{
		GuiFrame* pFrame = (GuiFrame*)*itFrame;
		touchPrimitive = pFrame->getPrimitiveTouch(x, y);

		if(touchPrimitive != NULL)
		{
			isDragging = false;

			touchPrimitive->setSelection(GuiPrimitive::PRESSED);
			touchItem.idFrame = pFrame->getResId();
			touchItem.idPrimitive = touchPrimitive->getResId();

			// check if a scroll button was touched
			if(touchPrimitive->getResId() == RES_SCROLL_UP) scrollUp();
			else if(touchPrimitive->getResId() == RES_SCROLL_DOWN) scrollDown();

			return;
		}
	}

	isDragging = dragScroll;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Touch up event handler
 *
 * Given virtual function is called by the parent class every time when a new
 * touch down event is generated, i.e. the pressure is released from the
 * touchscreen.
 *
 * \param x Specifies the X coordinate of the touch
 * \param y Specifies the Y coordinate of the touch
 */
void GuiScrollWindow::onTouchUp(int x, int y)
{
	if(!isDragging)
	{
		GuiWindow::onTouchUp(x, y);
	}
	else
	{
		onTouchMove(x, y);
		isDragging = false;
	}
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Touch move event handler
 *
 * Given virtual function is called by the parent class every time when a new
 * touch down event is generated, i.e. the pressure is moved across the
 * touchscreen.
 *
 * \param x Specifies the X coordinate of the touch
 * \param y Specifies the Y coordinate of the touch
 */
void GuiScrollWindow::onTouchMove(int x, int y)
{
	int frameX, frameY, newFrameX, newFrameY;

	if(scrollFrame && isDragging)
	{
		int deltaX = x - pointX;
		int deltaY = y - pointY;

		if(abs(deltaX) >= dragThreshold || abs(deltaY) >= dragThreshold)
		{
			scrollFrame->getOffset(&frameX, &frameY);

			// calculate constraints and apply offsets
			if(frameX - deltaX < 0)
				newFrameX = 0;
			else if(frameX - deltaX > frameVirtW - frameW)
				newFrameX = frameVirtW - frameW;
			else newFrameX = frameX - deltaX;

			// and for vertical coordinates
			if(frameY - deltaY < 0)
				newFrameY = 0;
			else if(frameY - deltaY > frameVirtH - frameH)
				newFrameY = frameVirtH - frameH;
			else newFrameY = frameY - deltaY;

			// check that we have moved anywhere
			if(frameX != newFrameX || frameY != newFrameY)
			{
				// redraw the frame
				scrollFrame->setOffset(newFrameX, newFrameY);
				scrollFrame->redrawFrame();

				updateScrollData(true);
			}

			pointX = x;
			pointY = y;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Update scrolling bar
 *
 * Updates the scroll bar position and size, then redraws it
 */
void GuiScrollWindow::updateScrollData(bool doRedraw)
{
	if(!scrollFrame || !stillFrame) return;

	// get frame information
	scrollFrame->getSize(&frameW, &frameH);
	scrollFrame->getVirtSize(&frameVirtW, &frameVirtH);

	int scrollPos = 0;
	scrollFrame->getOffset(NULL, &scrollPos);

	// update scroll bar
	if(scrollBar)
	{
		scrollBar->setPosition(scrollPos * 100 / frameVirtH,
				frameH * 100 / frameVirtH);
		if (doRedraw) scrollBar->redraw();
	}

	// update scroll indicators (arrows)
	if (scrollIndUp) {
		bool showInd = (scrollPos != 0);
		if (scrollIndUp->getVisible() != showInd) {
			scrollIndUp->setVisible(showInd);
			if (doRedraw) scrollIndUp->redraw();
		}
	}
	if (scrollIndDn) {
		bool showInd = ((scrollPos + frameH) < frameVirtH);
		if (scrollIndDn->getVisible() != showInd) {
			scrollIndDn->setVisible(showInd);
			if (doRedraw) scrollIndDn->redraw();
		}
	}
}

