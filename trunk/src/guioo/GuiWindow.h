#ifndef GUIWINDOW_H_
#define GUIWINDOW_H_

#include "GuiSurface.h"
#include "GuiFrame.h"
#include "GuiPrimitive.h"

#include <xynth.h>
#include <list>
using std::list;

#define GUI_MAX_GROUPS		4

#define GUI_STYLE_MAIN		0x01
#define GUI_STYLE_DESKTOP	0x02
#define GUI_STYLE_CHILD		0x04
#define GUI_STYLE_TOOLTIP	0x08
#define GUI_STYLE_POPUP		0x10
#define GUI_STYLE_INPUT		0x20
#define GUI_STYLE_MODAL		0x40

class GuiFrame;
class GuiPrimitive;
class GuiWindow;

/**
 * \brief Timer descriptor
 *
 * Given structure contains data of a timer that is associated with a window
 * or a primitive.
 */
struct TIMER_DATA
{
	/** Timer identifier */
	int idTimer;

	/** Pointer to an associated window.
	 * Can be null if the timer is related to a primitive
	 */
	GuiWindow *pWindow;

	/** Pointer to an associated primitive.
	 * Can be null if the timer is related to a window
	 */
	GuiPrimitive *pItem;
};

/**
 * \brief Touchscreen selection element
 *
 * Given structure identifies a GUI primitive by its frame and primitive
 * identifiers.
 */
struct TouchItem
{
	int idFrame;
	int idPrimitive;
};

/**
 * \brief Implements a generic GUI window
 *
 * Given class implements a generic window, being the basic building block of
 * the GUI. The window can be also viewed as a wrapper for the s_window_t type
 * of the Xynth engine and its related API.
 *
 * The window class is a container for GUI frames, that in turn are containers
 * for window primitives. The content of the window may be automatically
 * initialized by means of the skin function provided by GuiTemplate class.
 * In such case, the contained frames and related primitives will be instantiated
 * automatically and inserted into the window.
 *
 * The window class implements timers (see startTimer(), stopTimer()
 * methods) by wrapping Xynth s_timer_t and its related API.
 *
 * User should implement GUI windows by creating classes inherited from
 * GuiWindow (or its derivatives, GuiEffectWindow and GuiScrollWindow) and
 * implement required event handlers.
 *
 * XML code below defines a window:
 *
 * \code
 *	<window id="ID" x="X" y="Y" width="W" height="H">
 * 		<!-- frames are defined here -->
 * 		<frame ...>
 * 			<!-- primitives are defined here -->
 * 		</frame>
 * 	</window>
 * \endcode
 *
 * Where
 * - ID represents the unique identifier of the window
 * - X, Y, W, H is the set of coordinates of the window on the screen
 *
  * \author Andrei Birjukov, Anti Sullin
 */

class GuiWindow
{
public:

	/**
	 * \brief Window visibility identifier
	 *
	 * Enum identifying a window show/hide operation
	 */
	enum SHOW_WINDOW_TYPE
	{
		/** The window is about to be hidden */
		HIDE_WINDOW,
		/** The window is about to be displayed */
		SHOW_WINDOW,
		/** The window is about to be closed */
		CLOSE_WINDOW
	};

	GuiWindow(int idResource);
	virtual ~GuiWindow();

	bool createWindow(int style, GuiWindow *pParent=NULL);
	bool setWindowPos(s_rect_t &wndPos);
	bool showWindow(bool state);
	bool setCursor(s_cursor_type_t cursor);
	bool startEventLoop();
	bool setAlwaysOnTop(bool flag);
	bool setEnabled(bool flag);
	bool redrawWindow();
	bool redrawWindow(s_rect_t &updateArea);
	bool closeWindow(int retCode=0);
	bool startModal(GuiWindow* pChild);
	bool waitForClose();

	int getResId() { return idResource; }
	GuiSurface* getSurface() { return wndSurface; }
	s_window_t* getHandle() { return wndHandle; }
	int getReturnCode() { return returnCode; }
	bool getAlwaysOnTop() { return isAlwaysOnTop; }
	bool getEnabled() { return isEnabled; }

	int addFrame(GuiFrame *pItem);
	GuiFrame* getFrame(int idFrame);
	bool removeFrame(int idFrame);
	int getNumFrames() { return frameList.size(); }

	void lock();
	void unlock();

	s_timer_t* startTimer(int ms, int idTimer, GuiPrimitive *pItem);
	bool stopTimer(s_timer_t* pTimer);

	bool submitKeyEvent(int keyCode, int scanCode, bool isPressed);

	virtual void onInitialize();
	virtual void onPaint(s_rect_t &paintArea);
	virtual void onTouchDown(int x, int y);
	virtual void onTouchUp(int x, int y);
	virtual void onTouchMove(int x, int y);
	virtual void onTouchItem(GuiPrimitive *pItem);
	virtual void onTouchEntered(int x, int y);
	virtual void onTouchExited(int x, int y);
	virtual void onKeyDown(int keyCode, int scanCode);
	virtual void onKeyUp(int keyCode, int scanCode);
	virtual void onClose();
	virtual bool onShowWindow(SHOW_WINDOW_TYPE type, int extCode);
	virtual void onSetFocus();
	virtual void onLeaveFocus();
	virtual bool onModalFinished(GuiWindow *pModal);
	virtual void onTimer(int idTimer);
	virtual void onSystemEvent(int eventCode);

	virtual void offWindowHandler(s_event_t *event);

protected:

	static void eventHandlerProc(s_window_t *window, s_event_t *event);
	static void exitHandlerProc(s_window_t* window);
	static void timerProc(s_window_t* pWnd, s_timer_t *pTimer);
	GuiPrimitive* findPrimitive(TouchItem dPrim);

	/** Window unique identifier */
	int					idResource;

	/** Pointer to the window descriptor (Xynth structure) */
	s_window_t*			wndHandle;

	/** Pointer to the window surface object */
	GuiSurface*			wndSurface;

	/** Pointer to the window general purpose mutex, used for thread safety */
	s_thread_mutex_t*	wndMutex;
	/** Pointer to the window redrawing mutex, used for thread safety */
	s_thread_mutex_t*	wndPaintMutex;
	/** Pointer to the window existence mutex, used for safe termination */
	s_thread_mutex_t*	wndCloseMutex;
	/** Pointer to the window timer mutex, used for timer management */
	s_thread_mutex_t*	wndTimerMutex;

	/** List of contained window frames */
	list<GuiFrame*>		frameList;

	/** Position of the window in relation to the upper left corner of the screen */
	s_rect_t			windowPos;
	/** Specifies currently touched primitive */
	TouchItem			touchItem;
	/** Specifies previously touched primitive */
	TouchItem			lastTouchItem;

	/** Pointer to a parent window object */
	GuiWindow*			wndParent;

	/** Specifies if the window is enabled, i.e. ready to receive user input */
	bool				isEnabled;
	/** Specifies if the window is shutting down after the closeWindow() was called */
	bool				isShuttingDown;
	/** Specifies if the window is always on top */
	bool				isAlwaysOnTop;
	/** Specifies if the window performs self destruction after closure */
	bool				selfDestruct;
	bool				refreshRealSurface;

	/** Contains window style flags */
	int					wndStyle;
	/** Contains window return code */
	int					returnCode;
};

#endif /*GUIWINDOW_H_*/
