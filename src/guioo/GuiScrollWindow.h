#ifndef GUISCROLLWINDOW_H_
#define GUISCROLLWINDOW_H_

#include "GuiEffectWindow.h"
#include "GuiScrollBar.h"
#include "GuiImage.h"

#define FRAME_ID_SCROLL_MOVE	1000
#define FRAME_ID_SCROLL_STILL	1001
#define RES_SCROLL_UP			1002
#define RES_SCROLL_DOWN			1003
#define RES_SCROLL_BAR			1004
#define RES_SCROLL_IND_UP		1005
#define RES_SCROLL_IND_DN		1006

#define SCROLL_STEP				20
#define SCROLL_STEP_ANIM		5
#define DRAG_THRESHOLD			20

/**
 * \brief Implements a scrollable GUI window
 *
 * Given class implements a scrollable window. The scrollable window is built
 * using a window frame with a virtual size greater than the visible frame area
 * displayed in the window. The dimensions of the viewable area remain
 * constant, while the virtual frame size may vary. User may move the viewing
 * point across the virtual frame surface - such approach increases the
 * scrolling speed with a penalty of a greater memory usage.
 *
 * Scrollable window assumes that the window has a frame with a constant
 * identifier and all primitives that need to be scrolled must be created
 * inside that frame. Scrolling may be performed using on-screen buttons that
 * should be instantiated in a separate non-scrollable frame. Alternatively,
 * scrolling may be performed with mouse/touchscreen dragging.
 *
 * Scrollalbe window also encapsulates a scrollbar. If the scrollbar feature
 * is enabled, the scrollbar is automatically recalculated and redrawn whenever
 * the window is scrolled.
 *
 * \author Andrei Birjukov, Anti Sullin
 */

class GuiScrollWindow : public GuiEffectWindow
{
public:

	GuiScrollWindow(int idResource);
	virtual ~GuiScrollWindow();

	void enableDragScroll(bool flag);
	void enableButtonScroll(bool flag);

	void setDragThreshold(int threshold);
	void setScrollStep(int totalStep, int animStep);

	virtual void scrollUp();
	virtual void scrollDown();

protected:

	void updateScrollData(bool doRedraw);

	virtual void onInitialize();
	virtual void onTouchDown(int x, int y);
	virtual void onTouchUp(int x, int y);
	virtual void onTouchMove(int x, int y);

	int				pointX;
	int				pointY;

	int				frameVirtW;
	int				frameVirtH;
	int				frameW;
	int				frameH;

	int				scrollStep;
	int				animationStep;
	int				dragThreshold;

	bool			dragScroll;
	bool			buttonScroll;

	GuiFrame*		scrollFrame;
	GuiFrame*		stillFrame;
	GuiScrollBar*	scrollBar;
	GuiImage*		scrollIndUp;
	GuiImage*		scrollIndDn;

	bool			isDragging;
};

#endif /*GUISCROLLWINDOW_H_*/
