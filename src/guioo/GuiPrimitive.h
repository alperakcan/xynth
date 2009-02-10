#ifndef GUIPRIMITIVE_H_
#define GUIPRIMITIVE_H_

#include "GuiSurface.h"
#include "GuiFrame.h"

#define MAX_PARAM_BUFFER	256

#define RES_ID_INVALID		(-1)

class GuiFrame;

/**
 * \brief Generic GUI primitive implementation
 * 
 * Given class implements a generic GUI primitive. Real primitives must 
 * inherit from this class and override required functions, such as onPaint().
 * GuiPrimitive encapsulates most of basic attributes of a GUI element, being
 * its identifier, in-frame position, selection state, touchable region and
 * visibility flag.  
 * 
 * \author Andrei Birjukov, Anti Sullin
 */

class GuiPrimitive
{
friend class GuiWindow;
friend class GuiFrame;
friend class GuiBufferedFrame;

public:

	/** 
	 * \brief Primitive selection state
	 * 
	 * Identifies the current selection state of the primitive
	 */
	
	enum SELECTION
	{
		/**
		 * Selection state is unknown. Not valid for state, indicates that mouse
		 * pressure was released off-frame and the state should be changed to 
		 * a previously valid one.
		 */
		UNKNOWN,
		
		/** 
		 * Primitive is not selected.
		 * Touch pressure was released on another primitive within the same frame.
		 */
		UNSELECTED,
		
		/**
		 * Primitive is selected.
		 * Touch pressure was released on the primitive.
		 */
		SELECTED,
		
		/** 
		 * Primitive is actively held selected.
		 * Touch pressure is held on the primitive.
		 */
		PRESSED 
	};
	
	GuiPrimitive(int idResource, GuiFrame *pFrame);
	virtual ~GuiPrimitive();
	
	void setTouchArea(s_rect_t &touchArea);
	void getTouchArea(s_rect_t &touchArea);
	
	void redraw();
	void setPosition(s_rect_t &drawArea);
	void getPosition(s_rect_t &drawArea);
	
	void setSelection(SELECTION type);
	SELECTION getSelection();
	
	void setVisible(bool flag);
	bool getVisible();
	
	int getResId() { return idResource; }

	void lock();
	void unlock();
			
protected:

	virtual void onInitialize();
	virtual void onPaint(GuiSurface *drawSurface, s_rect_t &paintArea);
	virtual void onSelectionChanged(SELECTION type);
	virtual void onTimer(int idTimer);

	/** Contains the unique identifier of the GUI primitive */
	int					idResource;
	
	/** Contains the current selection state of the primitive */
	SELECTION			selection;
	
	/** Contains the previous selection state of the primitive */
	SELECTION			lastselection;
	
	/** Specifies the drawing rectangle, containing coordinates of the primitive 
	 * in relation to the parent frame */
	s_rect_t			drawArea;
	
	/** Specifies the coordinates of a rectangular touchable area associated with 
	 * the primitive*/
	s_rect_t			touchArea;
	
	/** Contains a pointer to the parent frame that contains the given primitive */
	GuiFrame*			pHolder;
	
	/** Contains a visibility flag that defines if the primitive is visible 
	 * and requires painting */
	bool				isVisible;
	
	/** Contains a primitive mutex used for thread safety */ 
	s_thread_mutex_t*	itemMutex;
};

#endif /*GUIPRIMITIVE_H_*/
