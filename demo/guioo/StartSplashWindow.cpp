#include <GuiAnimation.h>
#include "StartSplashWindow.h"

StartSplashWindow::StartSplashWindow()
				: GuiWindow(WND_ID_STARTSPLASH)
{
}

StartSplashWindow::~StartSplashWindow()
{
}

void StartSplashWindow::onClose()
{
	GuiFrame *pFrame = getFrame(FRAME_ID_SPLASH);
	if(!pFrame) return;
	
	GuiAnimation* pAnim =  (GuiAnimation*)pFrame->getPrimitive(ITEM_ID_ANIMATION);
	if(!pAnim) return;
	
	pAnim->stop();
}
