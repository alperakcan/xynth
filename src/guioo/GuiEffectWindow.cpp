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

#include "GuiEffectWindow.h"

#define SLIDE_HORZ_STEP		40
#define SLIDE_VERT_STEP		40

//#define DEBUG

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Class constructor
 *
 * Default constructor for the effect-enabled window object.
 * Effect mutex is created here. Initially, all effects are disabled.
 */
GuiEffectWindow::GuiEffectWindow(int idResource)
			: GuiWindow(idResource)
{
	originalPos.x = originalPos.y = originalPos.w = originalPos.h = 0;
	effectTimer = NULL;

	openEffectType = OPEN_EFFECT_NONE;
	closeEffectType = CLOSE_EFFECT_NONE;
	showWindowType = SHOW_WINDOW;
	closeRetCode = 0;

	effectVertSpeed = SLIDE_HORZ_STEP;
	effectHorzSpeed = SLIDE_VERT_STEP;

	s_thread_mutex_init(&effectMutex);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Class destructor
 *
 * Destructor for the effect-enabled window object. Also destroys the effect
 * mutex.
 */
GuiEffectWindow::~GuiEffectWindow()
{
	s_thread_mutex_destroy(effectMutex);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Show window event handler
 *
 * Given virtual function is called by the parent class whenever the window is
 * being shown, hidden or closed.
 *
 * When the window is being shown with an enabled effect function, a timer is
 * created that takes care of the window slide-in animation.
 * If the window is being hidden or closed with an enabled effect function,
 * another timer is created that takes care of the window slide-out animation.
 *
 * \return Retuns true if the window is being shown or effects are disabled,
 * returns false if the window is being closed or hidden and effects are
 * enabled.
 */
bool GuiEffectWindow::onShowWindow(SHOW_WINDOW_TYPE type, int extCode)
{
	s_thread_mutex_lock(effectMutex);

	showWindowType = type;
	closeRetCode = extCode;

	if(type == SHOW_WINDOW)
	{
		originalPos = windowPos;
		switch(openEffectType)
		{
		case OPEN_EFFECT_NONE:
			s_thread_mutex_unlock(effectMutex);
			return true;

		case OPEN_EFFECT_SLIDE_BOTTOM:
			windowPos.y = -windowPos.h;
			refreshRealSurface = false;
			break;
		case OPEN_EFFECT_SLIDE_TOP:
			windowPos.y = wndHandle->surface->height;
			refreshRealSurface = false;
			break;
		case OPEN_EFFECT_SLIDE_LEFT:
			windowPos.x = wndHandle->surface->width;
			refreshRealSurface = false;
			break;
		case OPEN_EFFECT_SLIDE_RIGHT:
			windowPos.x = -windowPos.w;
			refreshRealSurface = false;
			break;
		}

		setWindowPos(windowPos);

		effectTimer = startTimer(30, openEffectType, NULL);
		#ifdef DEBUG
		printf("GuiEffectWindow::onShowWindow: started timer %x\n", effectTimer);
		#endif
		s_thread_mutex_unlock(effectMutex);
		return true;
	}
	else
	{
		originalPos = windowPos;

		if(effectTimer)
		{
			#ifdef DEBUG
			printf("GuiEffectWindow::onShowWindow: stopping timer %x\n", effectTimer);
			#endif
			stopTimer(effectTimer);
			effectTimer = NULL;
		}

		if(closeEffectType == CLOSE_EFFECT_NONE)
		{
			#ifdef DEBUG
			printf("GuiEffectWindow::onShowWindow: no effect\n");
			#endif
			s_thread_mutex_unlock(effectMutex);
			return true;
		}

		#ifdef DEBUG
		printf("GuiEffectWindow::onShowWindow: starting timer...\n");
		#endif

		effectTimer = startTimer(30, closeEffectType, NULL);
		refreshRealSurface = false;
		s_thread_mutex_unlock(effectMutex);

		#ifdef DEBUG
		printf("%x done\n", effectTimer);
		#endif
		return false;
	}
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Timer event handler
 *
 * Given virtual function is called by the parent window whenever a timer is
 * fired. In the context of effect-enabled windows, the timer is used to
 * implement window slide-in and slide-out animations.
 *
 * \param idTimer Specifies the timer identifier
 */
void GuiEffectWindow::onTimer(int idTimer)
{
	s_thread_mutex_lock(effectMutex);

	// check if we have a stale timer event
	if(effectTimer == NULL)
	{
		#ifdef DEBUG
		printf("stale timer %d\n", idTimer);
		#endif
		s_thread_mutex_unlock(effectMutex);
		return;
	}

	#ifdef DEBUG
	printf("GuiEffectWindow::onTimer: %d locked\n", idTimer);
	#endif

	switch(idTimer)
	{
	case OPEN_EFFECT_SLIDE_BOTTOM:

		if(windowPos.y >= originalPos.y)
		{
			stopTimer(effectTimer);
			effectTimer = NULL;
			break;
		}

		windowPos.y += originalPos.y - windowPos.y < effectVertSpeed ?
				originalPos.y - windowPos.y : effectVertSpeed;

		if(windowPos.y >= originalPos.y)
			refreshRealSurface = true;

		setWindowPos(windowPos);
		break;

	case OPEN_EFFECT_SLIDE_RIGHT:

		if(windowPos.x >= originalPos.x)
		{
			stopTimer(effectTimer);
			effectTimer = NULL;
			break;
		}

		windowPos.x += originalPos.x - windowPos.x < effectHorzSpeed ?
				originalPos.x - windowPos.x : effectHorzSpeed;

		if(windowPos.x >= originalPos.x)
			refreshRealSurface = true;


		#ifdef DEBUG
		printf("GuiEffectWindow::onTimer: open %d %d %d %d\n",
				windowPos.x, windowPos.y, windowPos.w, windowPos.h);
		#endif
		setWindowPos(windowPos);
		break;

	case OPEN_EFFECT_SLIDE_TOP:

		if(windowPos.y <= originalPos.y)
		{
			stopTimer(effectTimer);
			effectTimer = NULL;
			break;
		}

		windowPos.y -= windowPos.y - originalPos.y < effectVertSpeed ?
				windowPos.y - originalPos.y : effectVertSpeed;

		if(windowPos.y <= originalPos.y)
			refreshRealSurface = true;

		setWindowPos(windowPos);
		break;

	case OPEN_EFFECT_SLIDE_LEFT:

		if(windowPos.x <= originalPos.x)
		{
			stopTimer(effectTimer);
			effectTimer = NULL;
			break;
		}

		windowPos.x -= windowPos.x - originalPos.x < effectHorzSpeed ?
				windowPos.x - originalPos.x : effectHorzSpeed;

		if(windowPos.x <= originalPos.x)
			refreshRealSurface = true;

		setWindowPos(windowPos);
		break;

	case CLOSE_EFFECT_SLIDE_BOTTOM:

		if(windowPos.y >= wndHandle->surface->height)
		{
			stopTimer(effectTimer);
			effectTimer = NULL;

			if(showWindowType == HIDE_WINDOW)
			{
				s_window_hide(wndHandle);
			}
			else if(showWindowType == CLOSE_WINDOW)
			{
				returnCode = closeRetCode;
				isShuttingDown = true;
				s_window_quit(wndHandle);
			}
			break;
		}

		windowPos.y += effectVertSpeed;
		setWindowPos(windowPos);
		break;

	case CLOSE_EFFECT_SLIDE_RIGHT:

		if(windowPos.x >= wndHandle->surface->width)
		{
			stopTimer(effectTimer);
			effectTimer = NULL;

			if(showWindowType == HIDE_WINDOW)
			{
				s_window_hide(wndHandle);
			}
			else if(showWindowType == CLOSE_WINDOW)
			{
				returnCode = closeRetCode;
				isShuttingDown = true;
				s_window_quit(wndHandle);
			}
			break;
		}

		windowPos.x += effectHorzSpeed;

		#ifdef DEBUG
		printf("GuiEffectWindow::onTimer: close %d %d %d %d\n",
				windowPos.x, windowPos.y, windowPos.w, windowPos.h);
		#endif

		setWindowPos(windowPos);
		break;

	case CLOSE_EFFECT_SLIDE_TOP:

		if(windowPos.y + windowPos.h <= 0)
		{
			stopTimer(effectTimer);
			effectTimer = NULL;

			if(showWindowType == HIDE_WINDOW)
			{
				s_window_hide(wndHandle);
			}
			else if(showWindowType == CLOSE_WINDOW)
			{
				returnCode = closeRetCode;
				isShuttingDown = true;
				s_window_quit(wndHandle);
			}
			break;
		}

		windowPos.y -= effectVertSpeed;
		setWindowPos(windowPos);
		break;

	case CLOSE_EFFECT_SLIDE_LEFT:

		if(windowPos.x + windowPos.w <= 0)
		{
			stopTimer(effectTimer);
			effectTimer = NULL;

			if(showWindowType == HIDE_WINDOW)
			{
				s_window_hide(wndHandle);
			}
			else if(showWindowType == CLOSE_WINDOW)
			{
				returnCode = closeRetCode;
				isShuttingDown = true;
				s_window_quit(wndHandle);
			}
			break;
		}

		windowPos.x -= effectHorzSpeed;
		setWindowPos(windowPos);
		break;
	}

	s_thread_mutex_unlock(effectMutex);
	#ifdef DEBUG
	printf("GuiEffectWindow::onTimer: %d unlocked\n", idTimer);
	#endif
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Set window effects
 *
 * Specifies the type of window opening and closing effects. Can also disable
 * effects completely.
 *
 * \param openEffect Sets the window opening effect, slide-in animations are
 * currently supported.
 * \param closeEffect Sets the window closing effect, slide-out animations are
 * currently supported.
 */
void GuiEffectWindow::setEffects(OPEN_EFFECT_TYPE openEffect, CLOSE_EFFECT_TYPE closeEffect)
{
	s_thread_mutex_lock(effectMutex);

	openEffectType = openEffect;
	closeEffectType = closeEffect;

	s_thread_mutex_unlock(effectMutex);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Set effect speed
 *
 * Updates the speed of window effects or animations in both vertical and
 * horizontal direction.
 *
 * \param vertSpeed Specifies the step of a vertical slide animation,
 * in pixels
 * \param horzSpeed Specifies the step of a horizontal slide animation,
 * in pixels
 */
void GuiEffectWindow::setEffectSpeed(int vertSpeed, int horzSpeed)
{
	s_thread_mutex_lock(effectMutex);

	effectVertSpeed = vertSpeed;
	effectHorzSpeed = horzSpeed;

	s_thread_mutex_unlock(effectMutex);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Checks if an effect is running
 *
 * \return Returns true if a window effect is currently running
 */
bool GuiEffectWindow::effectInProgress()
{
	s_thread_mutex_lock(effectMutex);
	bool ret = effectTimer != NULL;
	s_thread_mutex_unlock(effectMutex);
	return ret;
}
