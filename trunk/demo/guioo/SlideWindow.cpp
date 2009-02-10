#include "SlideWindow.h"

////////////////////////////////////////////////////////////////////////////////
//
SlideWindow::SlideWindow()
		: GuiEffectWindow(WND_ID_SLIDE)
{
	imageHolder = NULL;
}

SlideWindow::~SlideWindow()
{
}

////////////////////////////////////////////////////////////////////////////////
//
void SlideWindow::onInitialize()
{
}

////////////////////////////////////////////////////////////////////////////////
//
bool SlideWindow::loadImage(const char* fileName)
{
	GuiFrame* imageFrame = getFrame(FRAME_ID_SLIDE);
	if(!imageFrame) return false;
	
	imageHolder = new GuiImage(RES_ID_IMAGE, imageFrame);
	if(!imageHolder->loadImage(fileName))
	{
		printf("loadImage: failed loading image");
		return false;
	}
	
	s_rect_t rect = {0, 0, 0, 0};
	imageFrame->getSize(&rect.w, &rect.h);
	
	imageHolder->setPosition(rect);
	imageFrame->addPrimitive(imageHolder);
	imageHolder->redraw();
	
	return true;
}

////////////////////////////////////////////////////////////////////////////////
//
void SlideWindow::onKeyDown(int keyCode, int scanCode)
{
	if(effectInProgress()) return;
	wndParent->onKeyDown(keyCode, scanCode);
}
