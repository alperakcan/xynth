#ifndef SHUTDOWNWINDOW_H_
#define SHUTDOWNWINDOW_H_

#include <GuiWindow.h>

#define WND_ID_SHUTDOWN		0x02

class ShutdownWindow : public GuiWindow
{
public:
	ShutdownWindow();
	virtual ~ShutdownWindow();
};

#endif /*SHUTDOWNWINDOW_H_*/
