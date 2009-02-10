#include <stdio.h>
#include <signal.h>

#include <GuiWindow.h>
#include <GuiImageCache.h>

#include "Demo.h"
#include "Thread.h"
#include "StartSplashWindow.h"
#include "ShutdownWindow.h"
#include "DesktopWindow.h"

GuiTemplate		guiTemplate;

////////////////////////////////////////////////////////////////////////////////
//
int main(void)
{
	// initialize image cache
	if(!GuiImageCache::init())
	{
		printf("Failed initializing image cache\n");
		return -2;
	}

	// initialize GUI layout file
	if(!guiTemplate.loadFile(GUI_LAYOUT_FILE))
	{
		printf("Failed loading GUI layout file\n");
		return -3;
	}

	// create the start splash window
	StartSplashWindow* wndSplash = new StartSplashWindow();
	if(!wndSplash->createWindow(GUI_STYLE_POPUP))
	{
		printf("Failed creating start splash window\n");
		return -4;
	}

	guiTemplate.initWindowLayout(wndSplash);

	wndSplash->showWindow(true);
	wndSplash->setAlwaysOnTop(true);
	wndSplash->redrawWindow();
	wndSplash->startEventLoop();

	Thread::sleep(3000);	// let's assume we're busy here

	// create the desktop window
	DesktopWindow* wndDesktop = new DesktopWindow();
	if(!wndDesktop->createWindow(GUI_STYLE_MAIN))
	{
		printf("Failed creating desktop window\n");
		return -5;
	}

	guiTemplate.initWindowLayout(wndDesktop);

	wndSplash->closeWindow();
	delete wndSplash;

	wndDesktop->showWindow(true);
	wndDesktop->redrawWindow();

	wndDesktop->startEventLoop();
	delete wndDesktop;

	printf("Shutting down now...\n");

	// create the shutdown splash window
	ShutdownWindow* wndShutdown = new ShutdownWindow();
	wndShutdown->createWindow(GUI_STYLE_POPUP);
	guiTemplate.initWindowLayout(wndShutdown);

	wndShutdown->showWindow(true);
	wndShutdown->setAlwaysOnTop(true);
	wndShutdown->redrawWindow();
	wndShutdown->startEventLoop();

	// perform application deinitialization
	Thread::sleep(3000);

	guiTemplate.unload();
	GuiImageCache::release();

	wndShutdown->closeWindow();
	delete wndShutdown;

	return 0;
}
