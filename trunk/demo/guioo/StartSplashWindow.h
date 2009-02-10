#ifndef STARTSPLASHWINDOW_H_
#define STARTSPLASHWINDOW_H_

#include <GuiWindow.h>

#define WND_ID_STARTSPLASH		0x01
#define FRAME_ID_SPLASH			0x01
#define ITEM_ID_ANIMATION		0x03

class StartSplashWindow : public GuiWindow
{
public:
	StartSplashWindow();
	virtual ~StartSplashWindow();

protected:
	virtual void onClose();
};

#endif /*STARTSPLASHWINDOW_H_*/
