#ifndef GUIEFFECTWINDOW_H_
#define GUIEFFECTWINDOW_H_

#include <xynth.h>

#include "GuiWindow.h"

/**
 * \brief Implements an effect-enabled GUI window
 * 
 * Given class implements an effect enabled GUI window. Currently, there are
 * window opening and closing effects defined, that are basically slide-in 
 * and slide-out animations.
 *  
 * \author Andrei Birjukov, Anti Sullin
 */

class GuiEffectWindow : public GuiWindow
{
public:
	
	/**
	 * \brief Window opening effect type
	 * 
	 * Enum identifying the effect executed when a window is being opened. 
	 */
	enum OPEN_EFFECT_TYPE
	{
		/** Opening effect is disabled */
		OPEN_EFFECT_NONE=0,

		/**
		 * Opening effect is a slide-in animation from bottom. The initial window 
		 * location is off-screen and the final is as specfied by the original
		 * window position.
		 */
		OPEN_EFFECT_SLIDE_TOP,
		
		/**
		 * Opening effect is a slide-in animation from top. The initial window 
		 * location is off-screen and the final is as specfied by the original
		 * window position.
		 */
		OPEN_EFFECT_SLIDE_BOTTOM,

		/**
		 * Opening effect is a slide-in animation from left. The initial window 
		 * location is off-screen and the final is as specfied by the original
		 * window position.
		 */
		OPEN_EFFECT_SLIDE_LEFT,
		
		/**
		 * Opening effect is a slide-in animation from right. The initial window 
		 * location is off-screen and the final is as specfied by the original
		 * window position.
		 */
		OPEN_EFFECT_SLIDE_RIGHT,
	};

	/**
	 * \brief Window closing effect type
	 * 
	 * Enum identifying the effect executed when a window is being closed. 
	 */
	enum CLOSE_EFFECT_TYPE
	{
		/** Closing effect is disabled */
		CLOSE_EFFECT_NONE=100,

		/**
		 * Closing effect is a slide-out animation to the top. The initial window 
		 * location is unchanged and the final location is off-screen.
		 */
		CLOSE_EFFECT_SLIDE_TOP,

		/**
		 * Closing effect is a slide-out animation to the bottom. The initial window 
		 * location is unchanged and the final location is off-screen.
		 */
		CLOSE_EFFECT_SLIDE_BOTTOM,

		/**
		 * Closing effect is a slide-out animation to the left. The initial window 
		 * location is unchanged and the final location is off-screen.
		 */
		CLOSE_EFFECT_SLIDE_LEFT,

		/**
		 * Closing effect is a slide-out animation to the right. The initial window 
		 * location is unchanged and the final location is off-screen.
		 */
		CLOSE_EFFECT_SLIDE_RIGHT
	};

	GuiEffectWindow(int idResource);
	virtual ~GuiEffectWindow();
	
	void setEffects(OPEN_EFFECT_TYPE openEffect, CLOSE_EFFECT_TYPE closeEffect);
	void setEffectSpeed(int vertSpeed, int horzSpeed);
	
	bool effectInProgress();
	
protected:
	
	virtual bool onShowWindow(SHOW_WINDOW_TYPE type, int extCode);
	virtual void onTimer(int idTimer);
	
private:
	
	s_rect_t			originalPos;
	s_timer_t*			effectTimer;
	s_thread_mutex_t*	effectMutex;
	
	OPEN_EFFECT_TYPE	openEffectType;
	CLOSE_EFFECT_TYPE	closeEffectType;
	int					effectVertSpeed;
	int					effectHorzSpeed;
	
	SHOW_WINDOW_TYPE	showWindowType;
	int					closeRetCode;
};

#endif /*GUIEFFECTWINDOW_H_*/
