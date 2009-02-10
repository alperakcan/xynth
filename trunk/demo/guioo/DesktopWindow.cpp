#include <linux/input.h>

#include "Demo.h"
#include "DesktopWindow.h"
#include "SlideWindow.h"

#define NUM_SLIDES			11
#define SLIDESHOW_TIMER		1
#define SLIDESHOW_INTERVAL	4000

const char *slideFiles[NUM_SLIDES] = {
	GUIOODIR "/slide-1.png",
	GUIOODIR "/slide-2.png",
	GUIOODIR "/slide-3.png",
	GUIOODIR "/slide-4.png",
	GUIOODIR "/slide-5.png",
	GUIOODIR "/slide-6.png",
	GUIOODIR "/slide-7.png",
	GUIOODIR "/slide-8.png",
	GUIOODIR "/slide-9.png",
	GUIOODIR "/slide-10.png",
	GUIOODIR "/slide-11.png",
};

////////////////////////////////////////////////////////////////////////////////
//
DesktopWindow::DesktopWindow()
			: GuiWindow(WND_ID_DESKTOP)
{
	oldSlideWindow = NULL;
	newSlideWindow = NULL;

	selectedItem = -1;
	slideShowTimer = NULL;
}

DesktopWindow::~DesktopWindow()
{
}

////////////////////////////////////////////////////////////////////////////////
//
void DesktopWindow::onInitialize()
{
	slideShowTimer = startTimer(SLIDESHOW_INTERVAL, SLIDESHOW_TIMER, NULL);
}

////////////////////////////////////////////////////////////////////////////////
//
void DesktopWindow::onClose()
{
	if(slideShowTimer)
	{
		stopTimer(slideShowTimer);
		slideShowTimer = NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////
//
void DesktopWindow::onKeyDown(int keyCode, int scanCode)
{
	if(slideShowTimer)
	{
		stopTimer(slideShowTimer);
		slideShowTimer = NULL;
	}

	switch(keyCode)
	{
	case KEYBOARD_BUTTON_UP:

		slideImage(true);
		break;

	case KEYBOARD_BUTTON_DOWN:

		slideImage(false);
		break;

	case KEYBOARD_BUTTON_ESCAPE:

		if(oldSlideWindow) oldSlideWindow->closeWindow();
		if(newSlideWindow) newSlideWindow->closeWindow();

		closeWindow();
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////
//
void DesktopWindow::slideImage(bool up)
{
	if(oldSlideWindow) oldSlideWindow->closeWindow();

	SlideWindow *wnd = new SlideWindow();
	wnd->createWindow(GUI_STYLE_CHILD | GUI_STYLE_MODAL, this);
	guiTemplate.initWindowLayout(wnd);

	selectedItem += up ? -1 : 1;

	if(selectedItem < 0)
		selectedItem = NUM_SLIDES - 1;
	if(selectedItem >= NUM_SLIDES)
		selectedItem = 0;

	wnd->loadImage(slideFiles[selectedItem]);

	if(up) wnd->setEffects(GuiEffectWindow::OPEN_EFFECT_SLIDE_TOP,
			GuiEffectWindow::CLOSE_EFFECT_NONE);
	else wnd->setEffects(GuiEffectWindow::OPEN_EFFECT_SLIDE_BOTTOM,
			GuiEffectWindow::CLOSE_EFFECT_NONE);

	wnd->showWindow(true);
	wnd->redrawWindow();

	// simple 2-window stack
	oldSlideWindow = newSlideWindow;
	newSlideWindow = wnd;

	wnd->startEventLoop();
}

////////////////////////////////////////////////////////////////////////////////
//
void DesktopWindow::onTimer(int idTimer)
{
	switch(idTimer)
	{
	case SLIDESHOW_TIMER:

		slideImage(false);
		break;
	}

	GuiWindow::onTimer(idTimer);
}
