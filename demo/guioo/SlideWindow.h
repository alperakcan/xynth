#ifndef SLIDEWINDOW_H_
#define SLIDEWINDOW_H_

#include <GuiImage.h>
#include <GuiEffectWindow.h>

#define WND_ID_SLIDE	4
#define FRAME_ID_SLIDE	1
#define RES_ID_IMAGE	2

class SlideWindow : public GuiEffectWindow
{
public:
	
	SlideWindow();
	virtual ~SlideWindow();
	
	bool loadImage(const char* fileName);
	
protected:
	
	virtual void onInitialize();
	virtual void onKeyDown(int keyCode, int scanCode);
	
private:
	
	GuiImage*	imageHolder;
};

#endif /*SLIDEWINDOW_H_*/
