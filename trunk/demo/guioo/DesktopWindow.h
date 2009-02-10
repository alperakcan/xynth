#ifndef DESKTOPWINDOW_H_
#define DESKTOPWINDOW_H_

#include <GuiWindow.h>

#define WND_ID_DESKTOP		0x03

class DesktopWindow : public GuiWindow
{
public:
	
	DesktopWindow();
	virtual ~DesktopWindow();
	
protected:

	virtual void onInitialize();
	virtual void onClose();
	virtual void onKeyDown(int keyCode, int scanCode);
	virtual void onTimer(int idTimer);
	
private:
	
	void slideImage(bool up);
	
	int				selectedItem;
	
	GuiWindow*		oldSlideWindow;
	GuiWindow*		newSlideWindow;
	
	s_timer_t*		slideShowTimer;
};

#endif /*DESKTOPWINDOW_H_*/
