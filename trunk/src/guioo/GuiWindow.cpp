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

#include "GuiWindow.h"

//#define DEBUG
//#define DEBUG_TIMER

////////////////////////////////////////////////////////////////////////////////
/*
 * \brief Class constructor
 *
 * Default contructor of the generic GUI window object.
 * Initializes window location and size to zeros, also resets window style,
 * return code and a number of flags, and initializes thread safety mutexes.
 *
 * \param idResource Unique window identifier
 */

GuiWindow::GuiWindow(int idResource)
{
	this->idResource = idResource;
	wndHandle = NULL;
	wndSurface = NULL;
	touchItem.idPrimitive = RES_ID_INVALID;
	lastTouchItem.idPrimitive = RES_ID_INVALID;

	windowPos.x = windowPos.y = 0;
	windowPos.w = windowPos.h = 0;

	if(s_thread_mutex_init(&wndMutex) != 0)
		wndMutex = NULL;

	if(s_thread_mutex_init(&wndPaintMutex) != 0)
		wndPaintMutex = NULL;

	if(s_thread_mutex_init(&wndTimerMutex) != 0)
		wndTimerMutex = NULL;

	wndCloseMutex = NULL;

	isEnabled = true;
	isShuttingDown = false;
	isAlwaysOnTop = false;
	selfDestruct = false;
	refreshRealSurface = true;

	wndStyle = 0;
	returnCode = 0;

	#ifdef DEBUG
	printf("GuiWindow(%p)::GuiWindow: new window %d\n", this, idResource);
	#endif
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Class destructor
 *
 * Destructor for the generic window objects.
 * If the given window is a child or a popup window, the destructor will
 * block until the window thread terminates.
 *
 * Also releases window descriptor, surface, mutexes and deletes frames, if
 * there are any remaining for some reason. Self destructable will perform
 * deletion of the window object automatically, if selfDestruct flag is set.
 */

GuiWindow::~GuiWindow()
{
	#ifdef DEBUG
	printf("GuiWindow(%p)::~GuiWindow: id %d\n", this, idResource);
	#endif

	// wait for our main window thread to terminate
	if(isShuttingDown)
	{
		#ifdef DEBUG
		printf("GuiWindow(%p)::~GuiWindow: locking\n", this);
		#endif
		// wait for the window to be uninited and Xynth will free all the stuff itself
		//if(wndStyle & (GUI_STYLE_CHILD | GUI_STYLE_TOOLTIP | GUI_STYLE_POPUP))
		if(wndCloseMutex) s_thread_mutex_lock(wndCloseMutex);

		#ifdef DEBUG
		printf("GuiWindow(%p)::~GuiWindow: id %d exited\n", this, idResource);
		#endif
	}

	// now we can kill the window safely
	list<GuiFrame*>::iterator it;
	for(it = frameList.begin(); it != frameList.end(); it ++)
	{
		GuiFrame* pFrame = (GuiFrame*)*it;
		delete pFrame;
	}

	frameList.clear();

	if(wndHandle) s_window_uninit(wndHandle);
	if(wndSurface) delete wndSurface;
	if(wndMutex) s_thread_mutex_destroy(wndMutex);
	if(wndPaintMutex) s_thread_mutex_destroy(wndPaintMutex);
	if(wndCloseMutex) s_thread_mutex_destroy(wndCloseMutex);
	if(wndTimerMutex) s_thread_mutex_destroy(wndTimerMutex);

	#ifdef DEBUG
	printf("GuiWindow(%p)::~GuiWindow: id %d terminated\n", this, idResource);
	#endif
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Create window
 *
 * Initializes window data and calls Xynth engine, creating a new window and
 * connecting it to the server. The created window afterwards needs to be shown
 * (showWindow()), redrawn (redrawWindow()), and the window loop must be
 * launched to start a normal operation.
 *
 * \param style Window style flags
 * - GUI_STYLE_MAIN creates a main window that blocks the calling thread when
 *   startWindowProc is called
 * - GUI_STYLE_CHILD creates a child window that runs independently and is
 *   attached to the parent window
 * - GUI_STYLE_TOOLTIP creates a temporary child window that is automatically
 *   destroyed when it loses focus
 * - GUI_STYLE_POPUP creates a popup window that runs independently and does
 *   not have a parent window
 * - GUI_STYLE_MODAL specifies that the window is modal, deactivates the parent
 *   window and blocks its input until the modal window is closed
 * - GUI_STYLE_INPUT specifies that the window will receive all input events
 *   even if it is not in focus
 * - GUI_STYLE_DESKTOP specifies that the window is a desktop
 *
 * \param pParent Pointer to the parent window. Required for child, modal
 * and tooltip windows
 *
 * \return Returns true if the window has been created successfully.
 */

bool GuiWindow::createWindow (int style, GuiWindow *pParent)
{
	s_window_type_t xynthStyle;

	if (wndHandle != NULL) {
		return false;
	}

	// modal, child and tooltip styles are only possible if there is a parent
	if (style & (GUI_STYLE_MODAL | GUI_STYLE_CHILD | GUI_STYLE_TOOLTIP) && pParent == NULL) {
		return false;
	}

	s_window_init(&wndHandle);

	xynthStyle = WINDOW_TYPE_NOFORM;
	if (style & GUI_STYLE_MAIN)    xynthStyle = (s_window_type_t) (xynthStyle | WINDOW_TYPE_MAIN);
	if (style & GUI_STYLE_DESKTOP) xynthStyle = (s_window_type_t) (xynthStyle | WINDOW_TYPE_DESKTOP);
	if (style & GUI_STYLE_CHILD)   xynthStyle = (s_window_type_t) (xynthStyle | WINDOW_TYPE_CHILD);
	if (style & GUI_STYLE_TOOLTIP) xynthStyle = (s_window_type_t) (xynthStyle | WINDOW_TYPE_TEMP);
	if (style & GUI_STYLE_POPUP)   xynthStyle = (s_window_type_t) (xynthStyle | WINDOW_TYPE_POPUP);
	if (style & GUI_STYLE_INPUT)   xynthStyle = (s_window_type_t) (xynthStyle | WINDOW_TYPE_INPUT);

	s_window_t *parentHandle = (pParent == NULL) ? NULL : pParent->wndHandle;
	s_window_new(wndHandle, xynthStyle, parentHandle);
	wndHandle->surface->mode = (S_SURFACE_MODE) (wndHandle->surface->mode & ~SURFACE_REAL);

	if (wndSurface) delete wndSurface;
	wndSurface = new GuiSurface(wndHandle->surface);

	wndHandle->data = (void*)this;

	s_window_atevent(wndHandle, eventHandlerProc);
	s_window_atexit(wndHandle, exitHandlerProc);

	wndParent = pParent;
	wndStyle = style;
	isShuttingDown = false;

	// modal windows must automatically delete themselves
	if(wndStyle & GUI_STYLE_MODAL)  selfDestruct = true;

	#ifdef DEBUG
	printf("GuiWindow(%p)::createWindow: id %d style %x\n", this, idResource, wndStyle);
	#endif
	return true;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Set window position and size
 *
 * Updates the window coordinate rectangle, including window position and
 * dimensions. Using negative x and y coordinates, window may be placed
 * outside the screen. Width and height must be always positive.
 *
 * \param wndPos New window coordinate rectangle (x, y, w, h). Coordinates
 * should start with (0, 0) at the upper left corner of the screen.
 * \return Returns true if the new window coordinates are set successfully.
 */

bool GuiWindow::setWindowPos(s_rect_t &wndPos)
{
	if(wndHandle == NULL) return false;

	lock();
	s_window_set_coor(wndHandle, 0, wndPos.x, wndPos.y,
						wndPos.w, wndPos.h);

	windowPos = wndPos;
	unlock();

	return true;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Launch window loop
 *
 * Starts window event handler procedure. If the given window was created with
 * GUI_STYLE_MAIN, the given method blocks until the window is closed.
 * Otherwise, a new thread is spawned and the mehtod returns immediately.
 *
 * \return Returns true if the event handler has been started successfully.
 */

bool GuiWindow::startEventLoop()
{
	if(wndHandle == NULL) return false;

	onInitialize();

	s_thread_mutex_init(&wndCloseMutex);
	s_thread_mutex_lock(wndCloseMutex);

	// call the regular main
	s_window_main(wndHandle);
	return true;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Show or hide window
 *
 * Controls the window visibility, i.e. shows or hides the window.
 * onShowWindow() virtual function is called internally to notify the derived
 * classes about the new state.
 *
 * \param state Set to true to make the window visible.
 * \return Returns true if the window visibility has been updated successfully.
 */

bool GuiWindow::showWindow(bool state)
{
	if(wndHandle == NULL) return false;

	if(wndStyle & GUI_STYLE_MODAL)
	{
		wndParent->setEnabled(!state);
		if(wndParent->isAlwaysOnTop)
			s_window_set_alwaysontop(wndParent->wndHandle, state ? 0 : 1);
	}

	// perform custom show/hide actions
	if(!onShowWindow(state ? SHOW_WINDOW : HIDE_WINDOW, 0)) return true;

	if(state) s_window_show(wndHandle);
	else s_window_hide(wndHandle);

	return true;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Set always-on-top property
 *
 * Sets or resets the window always-on-top flag, i.e. fixes the window at the
 * top-most position in the current window Z-order.
 *
 * \param flag Specifies if the window is always displayed on top.
 * \return Returns true if the window state has been updated successfully.
 */

bool GuiWindow::setAlwaysOnTop(bool flag)
{
	if(wndHandle == NULL) return false;
	isAlwaysOnTop = flag;
	s_window_set_alwaysontop(wndHandle, (int)flag);
	return true;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Enable or disable window
 *
 * Sets the window enable flag, i.e. activates or deactivates the window.
 * Deactivated windows do not process any keypad or touchscreen events.
 *
 * \param flag Specifies if the window is active
 * \return Returns true if the window state has been updated successfully.
 */

bool GuiWindow::setEnabled(bool flag)
{
	if(wndHandle == NULL) return false;
	isEnabled = flag;
	return true;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Set window cursor
 *
 * Specifies the desired cursor type for the given window. Multiple cursor
 * types are defined by the Xynth engine.
 *
 * \param cursor Selects the new cursor type, refer to Xynth documentation
 * for available options.
 * \return Returns true if the new cursor is set successfully.
 */

bool GuiWindow::setCursor(s_cursor_type_t cursor)
{
	if(wndHandle == NULL) return false;
	s_window_set_cursor(wndHandle, cursor);
	return true;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Redraw window completely
 *
 * Performs the complete window redraw.
 *
 * \return Returns true if the redrawing is completed successfully.
 */
bool GuiWindow::redrawWindow()
{
	s_rect_t updateRect = { 0, 0, windowPos.w, windowPos.h };
	return redrawWindow(updateRect);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Redraw window partially
 *
 * Redraws a selected area of the window. Redraw area is given as rectangle.
 * Redraw call blocks until all GUI primitives in the are have been repainted.
 *
 * \param updateArea Specifies the rectangular window area to be redrawn.
 * Coordinates should start with (0, 0) from the upper left corner of the window
 * \return Returns true if the redrawing is completed successfully.
 */
bool GuiWindow::redrawWindow(s_rect_t &updateArea)
{
	if(wndHandle == NULL) return false;
	lock();

	// as we currently don't have any queues for that, just call onPaint
	onPaint(updateArea);

	unlock();
	return true;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Close window
 *
 * Sends a command to the window instructing it to close and release all
 * contained GUI primitives. Also, virtual onShowWindow() function is called
 * in the calling thread to notify the derived classes about state change.
 *
 * Close call is not blocking in the sense that the function will return to
 * caller before the window is actually closed.
 *
 * \param retCode Specifies the window return code. Return code may be
 * retrieved after the window is closed for further processing.
 *
 * \return Returns true if the closing procedure was initiated.
 */
bool GuiWindow::closeWindow(int retCode)
{
	if(wndHandle == NULL || isShuttingDown) return false;

	#ifdef DEBUG
	printf("GuiWindow(%p)::closeWindow: id %d code %d\n", this, idResource, retCode);
	#endif

	setEnabled(false);	// set window state to disabled

	if(!onShowWindow(CLOSE_WINDOW, retCode))
	{
		#ifdef DEBUG
		printf("GuiWindow(%p)::closeWindow: postponed\n", this);
		#endif
		return true;
	}

	returnCode = retCode;
	isShuttingDown = true;
	s_window_quit(wndHandle);

	#ifdef DEBUG
	printf("GuiWindow(%p)::closeWindow: done\n", this);
	#endif

	return true;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Wait for window closure
 *
 * Blocks until the window is closed.
 *
 * \return Returns true after the window is closed.
 */
bool GuiWindow::waitForClose()
{
	if(wndCloseMutex == NULL) return false;
	return s_thread_mutex_lock(wndCloseMutex) == 0;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Add a new frame to the window
 *
 * Inserts a pointer to the specified frame to the end of the frame list.
 *
 * \return Returns the number of contained frames after insertion.
 */
int GuiWindow::addFrame(GuiFrame *pItem)
{
	lock();
	frameList.push_back(pItem);
	int ret = frameList.size();
	unlock();

	return ret;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Get frame by identifier
 *
 * Retrieves a GUI frame object given a unique frame identifier.
 *
 * \param idFrame Specifies the frame identifier.
 * \return Returns a pointer to the GUI frame object. Returns NULL if no frames
 * with the specified identifier were found.
 */
GuiFrame* GuiWindow::getFrame(int idFrame)
{
	lock();
	GuiFrame* pRet = NULL;

	list<GuiFrame*>::iterator it;
	for(it = frameList.begin(); it != frameList.end(); it ++)
	{
		if(((GuiFrame*)*it)->getResId() == idFrame)
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
 * \brief Remove frame
 *
 * Deletes the first frame with the specified identifier from the list.
 * The frame resources and all contained primitives are released automatically.
 *
 * \param idFrame Specifies the identifier of the frame to be deleted
 * \return Returns true if a frame was found and deleted. Returns false if no
 * frames with the given identifier were found.
 */
bool GuiWindow::removeFrame(int idFrame)
{
	lock();

	list<GuiFrame*>::iterator it;
	for(it = frameList.begin(); it != frameList.end(); it ++)
	{
		GuiFrame* pItem = (GuiFrame*)*it;
		if(pItem->getResId() == idFrame)
		{
			frameList.erase(it);
			delete pItem;
			unlock();
			return true;
		}
	}

	unlock();
	return false;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Initialization event handler
 *
 * Given function of the window interface is called when the window is being
 * initialized.
 */
void GuiWindow::onInitialize()
{
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Painting event handler
 *
 * Given function of the window interface is called when the window graphical
 * content requires redrawing. The implementation traverses the list of
 * contained GUI frames and performs their repainting to the window surface,
 * given the coordinates of a rectangular drawing area.
 *
 * \param paintArea Reference to the rectangle structure that contains
 * coordinates of the area that needs drawing.
 */
void GuiWindow::onPaint(s_rect_t &paintArea)
{
	list<GuiFrame*>::iterator itFrame;

	#ifdef DEBUG
	clock_t start = clock();
	#endif

	s_thread_mutex_lock(wndPaintMutex);

	for(itFrame = frameList.begin(); itFrame != frameList.end(); itFrame ++)
	{
		GuiFrame* pFrame = (GuiFrame*)*itFrame;
		pFrame->onPaint(wndSurface, paintArea);
	}

	// If we have some kind of periodic refresh (ie moving effect in progress),
	// we can disable the refreshing for every minor change and synchronize the
	// display by that
	if (refreshRealSurface) {
		// finally, copy the virtual surface data onto the window "real" surface
		s_rect_t paintRect = { paintArea.x + windowPos.x, paintArea.y + windowPos.y, paintArea.w, paintArea.h };
		wndHandle->surface->mode = (S_SURFACE_MODE) (wndHandle->surface->mode | SURFACE_REAL);
		s_surface_changed(wndHandle, &paintRect, 0);
		wndHandle->surface->mode = (S_SURFACE_MODE) (wndHandle->surface->mode & ~SURFACE_REAL);
	}

	s_thread_mutex_unlock(wndPaintMutex);

	#ifdef DEBUG
	printf("GuiWindow(%p)::onPaint: drawn (%d, %d, %d, %d) in %d ms\n", this,
			paintArea.x, paintArea.y, paintArea.w, paintArea.h,
			(int)((clock() - start) / (CLOCKS_PER_SEC / 1000)));
	#endif
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Touch down event handler
 *
 * Given funtion of the window interface implements the default behavior for
 * the touch down event. The frame list is traversed and the first primitive
 * found with a matching touchable region is updated with the new selection
 * state.
 *
 * User windows may need to override this event handler to implement different
 * handling.
 *
 * \param x Contains the X coordinate of the touched point
 * \param y Contains the Y coordinate of the touched point
 */
void GuiWindow::onTouchDown(int x, int y)
{
	GuiPrimitive *touchPrimitive;
	x -= windowPos.x;
	y -= windowPos.y;

	lastTouchItem.idPrimitive = RES_ID_INVALID;
	touchItem.idPrimitive = RES_ID_INVALID;

	#ifdef DEBUG
	printf("GuiWindow(%p)::onTouchDown: at (%d, %d)\n", this, x, y);
	#endif

	list<GuiFrame*>::reverse_iterator itFrame;
	for(itFrame = frameList.rbegin(); itFrame != frameList.rend(); itFrame ++)
	{
		GuiFrame* pFrame = (GuiFrame*)*itFrame;
		touchPrimitive = pFrame->getPrimitiveTouch(x, y);

		if(touchPrimitive != NULL)
		{
			touchPrimitive->setSelection(GuiPrimitive::PRESSED);
			touchItem.idFrame = pFrame->getResId();
			touchItem.idPrimitive = touchPrimitive->getResId();
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Touch up event handler
 *
 * Given funtion of the window interface implements the default behavior for
 * the touch up event. If the pointer is released within the same primitive
 * area that it was pressed in, the onTouchItem() event handler is called.
 * The state of current selection is also maintained.
 *
 * User windows may need to override this event handler to implement different
 * handling.
 *
 * \param x Contains the X coordinate of the released point
 * \param y Contains the Y coordinate of the released point
 */
void GuiWindow::onTouchUp(int x, int y)
{
	x -= windowPos.x;
	y -= windowPos.y;

	lastTouchItem.idPrimitive = RES_ID_INVALID;

	#ifdef DEBUG
	printf("GuiWindow(%p)::onTouchUp: at (%d, %d)\n", this, x, y);
	#endif

	if(touchItem.idPrimitive != RES_ID_INVALID) {
		GuiPrimitive *touchPrimitive = findPrimitive(touchItem);

		list<GuiFrame*>::reverse_iterator itFrame;
		for(itFrame = frameList.rbegin(); itFrame != frameList.rend(); itFrame ++)
		{
			GuiFrame* pFrame = (GuiFrame*)*itFrame;
			GuiPrimitive *pItem = pFrame->getPrimitiveTouch(x, y);

			// was the release at the same location?
			if(touchPrimitive && touchPrimitive == pItem)
			{
				// set new selection
				touchPrimitive->setSelection(GuiPrimitive::SELECTED);

				// update the focused primitive
				// (un-focuses the last focus if available)
				pFrame->setFocusedItem(touchItem.idPrimitive);

				// call notification function
				onTouchItem(touchPrimitive);
				touchItem.idPrimitive = RES_ID_INVALID;
				return;
			}
		}
	}

	if(touchItem.idPrimitive != RES_ID_INVALID)
	{
		GuiPrimitive *touchPrimitive = findPrimitive(touchItem);

		// indicate the last PRESSED item that the mouse was released not on the same
		// item, but in the window
		touchPrimitive->setSelection(GuiPrimitive::UNKNOWN);
		touchItem.idPrimitive = RES_ID_INVALID;
	}
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Off-window event handler
 *
 * Implements default behavior for the touch operations outside of the window
 * area.
 *
 * \param event Pointer to the Xynth event structure
 */
void GuiWindow::offWindowHandler(s_event_t *event)
{
	// Mouse left the window
	if(touchItem.idPrimitive != RES_ID_INVALID &&
			(event->type & EVENT_TYPE_MOUSE_RELEASED || event->type & EVENT_TYPE_MOUSE_EXIT))
	{	// indicate the last PRESSED item that the mouse was released not on the same
		// item (outside the window)
		GuiPrimitive *touchPrimitive = findPrimitive(touchItem);
		if (touchPrimitive) {
			touchPrimitive->setSelection(GuiPrimitive::UNKNOWN);
			lastTouchItem = touchItem;
		}
		touchItem.idPrimitive = RES_ID_INVALID;
	}
	// Mouse was released out-of-the-window. We're done.
	if (event->type & EVENT_TYPE_MOUSE_RELEASED) {
		lastTouchItem.idPrimitive = RES_ID_INVALID;
	}
	// Mouse came back to window and was not depressed inbetween.
	// Restore the state.
	if(lastTouchItem.idPrimitive != RES_ID_INVALID &&
			(event->type & EVENT_TYPE_MOUSE_ENTER))
	{
		touchItem = lastTouchItem;
		GuiPrimitive *touchPrimitive = findPrimitive(touchItem);
		if (touchPrimitive)
			touchPrimitive->setSelection(GuiPrimitive::PRESSED);
		lastTouchItem.idPrimitive = RES_ID_INVALID;
	}

}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Touch move event handler
 *
 * Given funtion of the window interface is called when a touch move event is
 * received. User windows may need to override this event handler to implement
 * appropriate processing.
 *
 * \param x Contains the X coordinate of the current touch point
 * \param y Contains the Y coordinate of the current touch point
 */

void GuiWindow::onTouchMove(int x, int y)
{
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Touch enter event handler
 *
 * Given funtion of the window interface is called when a touch enter event is
 * received. User windows may need to override this event handler to implement
 * appropriate processing.
 *
 * \param x Contains the X coordinate of the touch point
 * \param y Contains the Y coordinate of the touch point
 */

void GuiWindow::onTouchEntered(int x, int y)
{
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Touch primitive event handler
 *
 * Given funtion of the window interface is called when a specific primitive is
 * touched. The primitive must be pressed and released for this event to be
 * generated. User windows may need to override this event handler to implement
 * appropriate processing.
 *
 * \param pItem Pointer to the GUI primitive that was touched
 */

void GuiWindow::onTouchItem(GuiPrimitive *pItem)
{
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Touch exit event handler
 *
 * Given funtion of the window interface is called when a touch exit event is
 * received. User windows may need to override this event handler to implement
 * appropriate processing.
 *
 * \param x Contains the X coordinate of the touch point
 * \param y Contains the Y coordinate of the touch point
 */
void GuiWindow::onTouchExited(int x, int y)
{
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Key press event handler
 *
 * Given function of the window interface is called when a key down event is
 * received. User windows may need to override this event handler to implement
 * appropriate processing.
 *
 * \param keyCode Specifies the identifier of the pressed key
 * \param scanCode Specifies the scan code of the pressed key
 */
void GuiWindow::onKeyDown(int keyCode, int scanCode)
{
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Key release event handler
 *
 * Given function of the window interface is called when a key up event is
 * received. User windows may need to override this event handler to implement
 * appropriate processing.
 *
 * \param keyCode Specifies the identifier of the released key
 * \param scanCode Specifies the scan code of the released key
 */
void GuiWindow::onKeyUp(int keyCode, int scanCode)
{
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Window closure event handler
 *
 * Given function of the window interface is called after the window is closed.
 * The event handler loop is about to be terminated, but all frames and
 * primitives are still available for access.
 *
 * User windows may need to override this event handler to implement
 * appropriate processing.
 */
void GuiWindow::onClose()
{
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Window visibility event handler
 *
 * Given function of the window interface is called before the window is shown,
 * hidden, or closed.
 *
 * User windows may need to override this event handler to implement
 * appropriate processing.
 *
 * \param type Specifies the type of new window visibility
 * \param extCode Specifies the extended code. Extended code is determined
 * only when type is CLOSE_WINDOW and contains the return code
 *
 * \return If true is returned by the handler, the caller proceeds with its
 * show, hide or closure operation. In case of false, the operation is aborted.
 */
bool GuiWindow::onShowWindow(SHOW_WINDOW_TYPE type, int extCode)
{
	return true;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Window set focus event handler
 *
 * Given function of the window interface is called when the window receives
 * focus. User windows may need to override this event handler to implement
 * appropriate processing.
 */
void GuiWindow::onSetFocus()
{
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Window leave focus event handler
 *
 * Given function of the window interface is called when the window loses the
 * focus. User windows may need to override this event handler to implement
 * appropriate processing.
 */
void GuiWindow::onLeaveFocus()
{
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Modal window termination event handler
 *
 * Given function of the window interface is called when a child modal window
 * is closed. The data of the child modal window is available for access.
 *
 * User windows may need to override this event handler to implement
 * appropriate processing.
 *
 * \param pModal Pointer to the child modal window being closed.
 * \return Return true if the parent window should be re-activated
 */

bool GuiWindow::onModalFinished(GuiWindow *pModal)
{
	#ifdef DEBUG
	printf("GuiWindow(%p)::onModalFinished: id %d\n", this, pModal->getResId());
	#endif
	return true;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Main event handler procedure
 *
 * Implements the window event handler procedure. Given static function is
 * called by the Xynth engine when a window receives a new event.
 *
 * \param window Pointer to the window descriptor that receives an event
 * \param event Pointer to the event descriptor
 */
void GuiWindow::eventHandlerProc(s_window_t *window, s_event_t *event)
{
	// get the object pointer
	GuiWindow *pWindow = (GuiWindow*)window->data;

	if(pWindow->getEnabled())
	{
		// Is the click in this window?
		// If the previous click was in this window, we get another notification
		// with MOUSE_EXITED to un-select all controls that were left selected etc.
		// We better not over-react.
		if (event->mouse->x > pWindow->windowPos.x &&
				event->mouse->x < pWindow->windowPos.x + pWindow->windowPos.w &&
				event->mouse->y > pWindow->windowPos.y &&
				event->mouse->y < pWindow->windowPos.y + pWindow->windowPos.h)
		{
			if (event->type & EVENT_TYPE_MOUSE_OVER) {
				pWindow->onTouchMove(event->mouse->x, event->mouse->y);
			}

			if (event->type & EVENT_TYPE_MOUSE_PRESSED) {
				pWindow->onTouchDown(event->mouse->x, event->mouse->y);
			}

			if (event->type & EVENT_TYPE_MOUSE_RELEASED) {
				pWindow->onTouchUp(event->mouse->x, event->mouse->y);
			}

			if (event->type & EVENT_TYPE_MOUSE_ENTER) {
				pWindow->offWindowHandler(event);
				pWindow->onTouchEntered(event->mouse->x, event->mouse->y);
			}

			if (event->type & EVENT_TYPE_MOUSE_EXIT) {
				pWindow->onTouchExited(event->mouse->x, event->mouse->y);
			}
		}
		else
		{
			pWindow->offWindowHandler(event);

			// input windows still wants to receive mouse events
			if(pWindow->wndStyle & GUI_STYLE_INPUT)
			{
				if (event->type & EVENT_TYPE_MOUSE_OVER) {
					pWindow->onTouchMove(event->mouse->x, event->mouse->y);
				}

				if (event->type & EVENT_TYPE_MOUSE_PRESSED) {
					pWindow->onTouchDown(event->mouse->x, event->mouse->y);
				}

				if (event->type & EVENT_TYPE_MOUSE_RELEASED) {
					pWindow->onTouchUp(event->mouse->x, event->mouse->y);
				}
			}
		}

		if(event->type & EVENT_TYPE_KEYBOARD_PRESSED)
		{
			pWindow->onKeyDown(event->keybd->keycode, event->keybd->scancode);
		}

		if(event->type & EVENT_TYPE_KEYBOARD_RELEASED)
		{
			pWindow->onKeyUp(event->keybd->keycode, event->keybd->scancode);
		}
	}

	if(event->type & EVENT_TYPE_FOCUS_IN)
	{
		pWindow->onSetFocus();
	}

	if(event->type & EVENT_TYPE_FOCUS_OUT)
	{
		pWindow->onLeaveFocus();
	}

	if(event->type & EVENT_TYPE_QUIT)
	{
		pWindow->onClose();
		// free frame and primitive resources
		list<GuiFrame*>::iterator it;
		for(it = pWindow->frameList.begin(); it != pWindow->frameList.end(); it ++)
		{
			GuiFrame* pFrame = (GuiFrame*)*it;
			delete pFrame;
		}
		pWindow->frameList.clear();
	}

	if (event->type & EVENT_TYPE_EXPOSE) {
		pWindow->redrawWindow();
	}
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Window termination event handler
 *
 * Implements the window termination event handler. Given static function is
 * called by the Xynth engine at the end of the window life cycle.
 *
 * \param window Pointer to the descriptor of the window being terminated
 */

void GuiWindow::exitHandlerProc(s_window_t* window)
{
	// get the object pointer
	GuiWindow *pWindow = (GuiWindow*)window->data;

	#ifdef DEBUG
	printf("exitHandlerProc: %d\n", pWindow->getResId());
	#endif

	// reset the handle so we would not try to deinit it again later
	pWindow->wndHandle = NULL;

	if(pWindow->wndStyle & GUI_STYLE_MODAL)
	{
		// notify the parent about modal dialog termination
		if(pWindow->wndParent->onModalFinished(pWindow))
		{
			// finally, enable the parent window
			pWindow->wndParent->setEnabled(true);

			// restore the always-on-top status
			s_window_set_alwaysontop(pWindow->wndParent->wndHandle,
					pWindow->wndParent->isAlwaysOnTop ? 1 : 0);
		}
	}

	bool selfDestruct = pWindow->selfDestruct;

	// signal that the window may be safely destroyed
	if(pWindow->wndCloseMutex)
		s_thread_mutex_unlock(pWindow->wndCloseMutex);

	if(selfDestruct) delete pWindow;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Lock the window
 *
 * Locks the window mutex and protects resources
 */
void GuiWindow::lock()
{
	if(wndMutex) s_thread_mutex_lock(wndMutex);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Unlock the window
 *
 * Unlocks the window mutex and unprotects resources
 */
void GuiWindow::unlock()
{
	if(wndMutex) s_thread_mutex_unlock(wndMutex);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Start a new timer
 *
 * Starts a new window timer that will fire every 'ms' milliseconds and call
 * the timer event handler.
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

s_timer_t* GuiWindow::startTimer(int ms, int idTimer, GuiPrimitive *pItem=NULL)
{
	if(wndTimerMutex) s_thread_mutex_lock(wndTimerMutex);

	s_timer_t *pTimer = NULL;
	s_timer_init(&pTimer);

	#ifdef DEBUG
	printf("GuiWindow(%p)::startTimer: starting timer %d, %d ms\n", this, idTimer, ms);
	#endif

	if(pTimer != NULL)
	{
		pTimer->cb = timerProc;

		// create and fill the timer data structure
		TIMER_DATA *pTD = new TIMER_DATA;
		pTD->idTimer = idTimer;
		pTD->pItem = pItem;
		pTD->pWindow = this;

		pTimer->data = (void*)pTD;
		pTimer->timeval = (long long)ms;
		pTimer->interval = (long long)ms;

		if(s_timer_add(wndHandle, pTimer) <= 0)
		{
			#ifdef DEBUG_TIMER
			printf("GuiWindow(%p)::startTimer: failed adding timer\n", this);
			#endif
			s_timer_uninit(pTimer);
			pTimer = NULL;
		}
	}

	#ifdef DEBUG_TIMER
	printf("GuiWindow(%p)::startTimer: done %p\n", this, pTimer);
	#endif

	if(wndTimerMutex) s_thread_mutex_unlock(wndTimerMutex);
	return pTimer;
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
bool GuiWindow::stopTimer(s_timer_t* pTimer)
{
	if(wndTimerMutex) s_thread_mutex_lock(wndTimerMutex);

	#ifdef DEBUG_TIMER
	printf("GuiWindow(%p)::stopTimer: stopping timer %p\n", this, pTimer);
	#endif

	if(s_timer_del(wndHandle, pTimer) < 0)
	{
		#ifdef DEBUG_TIMER
		printf("GuiWindow(%p)::stopTimer: failed removing timer\n", this);
		#endif
		if(wndTimerMutex) s_thread_mutex_unlock(wndTimerMutex);
		return false;
	}

	// this is a quick hack against the xynth timer problem
	// (timer proc gets called after timer is actually removed)
	usleep(1000);

	if(pTimer->data != NULL)
	{
		delete (TIMER_DATA*)pTimer->data;
		pTimer->data = NULL;
	}

	if(s_timer_uninit(pTimer) != 0)
	{
		#ifdef DEBUG_TIMER
		printf("GuiWindow(%p)::stopTimer: failed releasing timer\n", this);
		#endif
		if(wndTimerMutex) s_thread_mutex_unlock(wndTimerMutex);
		return false;
	}

	if(wndTimerMutex) s_thread_mutex_unlock(wndTimerMutex);
	return true;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Timer callback procedure
 *
 * Given procedure is called by the Xynth engine when a timer event is received.
 * The procedure further calls an appropriate timer event handler of an
 * associated window or primitive.
 *
 * \param pWnd Pointer to the window descriptor
 * \param pTimer Pointer to the timer descriptor
 */
void GuiWindow::timerProc(s_window_t* pWnd, s_timer_t *pTimer)
{
	#ifdef DEBUG_TIMER
	printf("GuiWindow::timerProc: window %p, timer %p\n", pWnd, pTimer);
	#endif

	TIMER_DATA *pTD = (TIMER_DATA*)pTimer->data;
	if(pTD->pItem != NULL) pTD->pItem->onTimer(pTD->idTimer);
	else if (pTD->pWindow != NULL) pTD->pWindow->onTimer(pTD->idTimer);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Timer event handler
 *
 * Given event handler is called when a timer associated with a window is fired.
 *
 * \param idTimer Specifies the identifier of the timer that was set during
 * creation
 */
void GuiWindow::onTimer(int idTimer)
{
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief System event handler
 *
 * Given event handler is called when a global system event is received.
 *
 * \param eventCode Specifies the identifier of the system event
 */

// this virtual call is currently not used internally at all
// feel free to use it to distribute any events in your application
void GuiWindow::onSystemEvent(int eventCode)
{
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Submit a key event to the queue
 *
 * Given function submits a simulated key event to the event queue of the given
 * window. Either key press or release events may be submitted. An appropriate
 * onKeyXXX() event handler will be called after the event is dispatched.
 *
 * \param keyCode Specifies the key identifier
 * \param scanCode Specifies the key scan code
 * \param isPressed Specifies whether the key is pressed or released
 * \return Returns true if the key event was sent successfully
 */
bool GuiWindow::submitKeyEvent(int keyCode, int scanCode, bool isPressed)
{
	s_event_t *event;
	s_event_init(&event);
	event->type = isPressed ? EVENT_TYPE_KEYBOARD_PRESSED : EVENT_TYPE_KEYBOARD_RELEASED;

	event->keybd->keycode = (s_keyboard_button_t) keyCode;
	event->keybd->scancode = scanCode;

	#ifdef DEBUG
	printf("GuiWindow::submitKeyEvent: %d - %d %s\n", keyCode, scanCode,
			isPressed ? "pressed" : "released");
	#endif

	s_eventq_add(wndHandle, event);
	return true;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Find a primitive
 *
 * Given function searches for the primitive with the specified identifiers.
 *
 * \param item Specifies the frame and primitive identifiers
 * \return Returns a pointer to the primitive object or NULL if no primitive
 * is found.
 */
GuiPrimitive* GuiWindow::findPrimitive(TouchItem item)
{
	GuiFrame *pFrame = getFrame(item.idFrame);
	if (!pFrame) return NULL;
	return pFrame->getPrimitive(item.idPrimitive);
}
