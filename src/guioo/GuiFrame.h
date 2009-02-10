#ifndef GUIFRAME_H_
#define GUIFRAME_H_

#include "GuiWindow.h"
#include "GuiSurface.h"
#include "GuiPrimitive.h"

#include <list>
using std::list;

class GuiWindow;
class GuiPrimitive;

/**
 * \brief Window frame implementation
 * 
 * Given class implements a container for GUI primitives referred to as the 
 * frame. Any window may have a number of frames which may overlap each other. 
 * The frame may contain a number of primitives, each implementing some 
 * visualization function. The idea of the frame is:
 *
 * - to provide a virtual drawing space, simplifying implementation of 
 *   scrollalble elements
 * - to group elements logically with stand-alone selection settings, 
 *   e.g. a group of radio boxes 
 * - to provide a container for graphical primitives
 * 
 * GuiFrame does not have a virtual painting surface used for buffered
 * drawing. Please refer to GuiBufferedFrame if your application requires
 * a buffered frame. 
 *  
 * XML code below defines a window frame:
 * 
 * \code
 *	<frame id="ID" x="X" y="Y" width="W" height="H" buf="V" vx="VX" vy="VY">
 * 		<!-- primitives are defined here -->
 * 	</frame>
 * \endcode
 * 
 * Where
 * - ID represents the unique identifier of the frame
 * - X, Y, W, H is the set of frame coordinates in the parent window
 * - V is the buffering flag (optional). When set to "y", enables the 
 * - surface buffering. Refer to GuiBufferedFrame for more details.	
 * - vx is the virtual width of the frame (optional)
 * - vy is the virtual height of the frame (optional)
 *  
 * Virtual dimensions of the frame should be only specified for scrollable
 * frames.
 * 
 * \author Andrei Birjukov, Anti Sullin
 */

class GuiFrame
{
friend class GuiWindow;

public:
	GuiFrame(int idResource, GuiWindow *pParent);
	virtual ~GuiFrame();
	
	int addPrimitive(GuiPrimitive *pItem);
	GuiPrimitive* getPrimitive(int idResource);
	bool removePrimitive(int idResource);
	int getNumPrimitives() { return primitiveList.size(); }
	void setFocusedItem(int idPrimitive);

	GuiPrimitive* getPrimitiveTouch(int x, int y);
	
	void setOffset(int x, int y);
	void getOffset(int *pX, int *pY);
	void setGeometry(s_rect_t &frameArea);
	void getGeometry(s_rect_t &frameArea);
	void getVirtSize(int *pX, int *pY);
	void getSize(int *pX, int *pY);
	void setVirtSize(int w, int h);
	void setVisible(bool flag);

	bool redrawFrame();
	bool redrawFrame(s_rect_t updateArea);

	s_timer_t* startTimer(int ms, int idTimer, GuiPrimitive *pItem);
	bool stopTimer(s_timer_t* pTimer);
	
	void lock();
	void unlock();
	
	int getResId() { return idResource; }

	virtual bool initSurface(int virtWidth, int virtHeight);

protected:
	
	virtual void onPaint(GuiSurface *pSurface, s_rect_t &paintArea);
	
	s_rect_t frameArea;
	int idResource;
	
	int virtWidth;
	int virtHeight;
	int offsetX;
	int offsetY;

	list<GuiPrimitive*> primitiveList;
	int focusedItem;
	GuiWindow* parentWindow;
	s_thread_mutex_t* frameMutex;

	bool updateRequired;
	bool isVisible;
};

/**
 * \brief Window buffered frame implementation
 * 
 * Given class implements a buffered frame that provides a separate drawing
 * surface for faster scrolling. The buffered frame class is based on the
 * standard GuiFrame class and inherits most of its methods and attributes.
 * 
 * GuiBufferedFrame implements its own painting function to take advantage
 * of the buffered painting mechanism.  
 *  
 * \author Andrei Birjukov, Anti Sullin
 */

class GuiBufferedFrame : public GuiFrame
{
public:
	GuiBufferedFrame(int idResource, GuiWindow *pParent);
	virtual ~GuiBufferedFrame();

	virtual bool initSurface(int virtWidth, int virtHeight);

protected:
	virtual void onPaint(GuiSurface *pSurface, s_rect_t &paintArea);
	
private:
	GuiSurface* virtSurface;
};

#endif /*GUIFRAME_H_*/
