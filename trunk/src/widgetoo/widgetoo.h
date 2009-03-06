/***************************************************************************
    begin                : Mon Sep 20 2004
    copyright            : (C) 2004 - 2006 by Alper Akcan
    email                : distchx@yahoo.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#ifndef SWIDGET_H
#define SWIDGET_H

class SRect {
public:
	int rectX;
	int rectY;
	int rectW;
	int rectH;

        void rectSet (int x, int y, int w, int h);
	SRect (int x = 0, int y = 0, int w = 0, int h = 0);
	SRect (const SRect &copy);
	~SRect (void);
	void operator = (const SRect &copy);
};

class SSize {
public:
	int sizeW;
	int sizeH;

        void sizeSet (int w, int h);
	SSize (int w = 0, int h = 0);
	~SSize (void);
};

class SObject {
public:
	typedef enum {
		FixedSize   = 0x1,
		FreeResize  = 0x2
	} ResizeMode;

	typedef enum {
		AlignCenter = 0x1
	} Alignment;
	
	SObject *objectChild;
	SObject *objectParent;

	s_window_t *objectWindow;

	int objectAlignment;
	int objectResizeMode;

	SSize objectSizeBufferMin;
	SSize objectSizeBufferMax;
	SSize objectSizeContentsMin;
	SSize objectSizeContentsMax;

	SRect objectRectBuffer;
	SRect objectRectContents;

	int objectRGBColor (int r, int g, int b);
	void objectFillBox (int x, int y, int w, int h, int color);

	void objectSetSizeMin (int w, int h);
	void objectSetSizeMax (int w, int h);
	void objectSetRectBuffer (int x, int y, int w, int h);
	void objectSetRectContents (int x, int y, int w, int h);

	void objectDelFromParent (void);
	void objectSetParent (SObject *parent);
        
	void draw (void);
	void expose (int x, int y, int w, int h);
	void geometry (int x, int y, int w, int h);

	virtual void objectDraw (void) = 0;
	virtual void objectGeometry (int x, int y, int w, int h) = 0;
	virtual void objectSetObjectWindow (s_window_t *window);

	SObject (SObject *parent = NULL, Alignment alignment = SObject::AlignCenter, ResizeMode resizemode = SObject::FreeResize);
	virtual ~SObject (void);
};

class SHandler {
public:
	SObject *handlerObj;
	s_handler_t *handlerHndl;

	void handlerAdd (void);
	void handlerDel (void);

	SHandler (SObject *obj, S_HANDLER type);
	~SHandler (void);
};

class SLayoutCell : public SObject {
public:
	SObject *layoutCellObject;

	virtual void objectDraw (void);
	virtual void objectGeometry (int x, int y, int w, int h);

	SLayoutCell (void);
	~SLayoutCell (void);
};

class SLayout : public SObject {
public:
        int layoutRows;
        int layoutCols;
        int layoutColSpacing;
        int layoutRowSpacing;
	double *layoutRowsWeight;
	double *layoutColsWeight;
	SLayoutCell **layoutCells;
	
        void layoutSetColSpacing (int spacing);
        void layoutSetRowSpacing (int spacing);
        void layoutInsertObject (SObject *object, int row, int col);
        
	virtual void objectDraw (void);
	virtual void objectGeometry (int x, int y, int w, int h);
	virtual void objectSetObjectWindow (s_window_t *window);

	SLayout (SObject *parent, int rows = 1, int cols = 1);
	~SLayout (void);
};

class SFrame : public SObject {
public:
	typedef enum {
		NoFrame		= 0x00,
		Box		= 0x01,
		Panel		= 0x02,
		WinPanel	= 0x03,
		HLine		= 0x04,
		VLine		= 0x05,
		StyledPanel	= 0x06,
		PopupPanel	= 0x07,
		MenuBarPanel	= 0x08,
		ToolBarPanel	= 0x09,
		LineEditPanel	= 0x0a,
		TabWidgetPanel	= 0x0b,
		GroupBoxPanel	= 0x0c,
		MShape		= 0x0f
	} frameShape;
	
	typedef enum {
		Plain		= 0x10,
		Raised		= 0x20,
		Sunken		= 0x40,
		MShadow		= 0xf0
	} frameShadow;
	
	int frameStyle;
	int frameLineWidth;
	int frameMidLineWidth;

	void frameRectContents (void);
	void frameSetStyle (int style);
        void frameSetLineWidth (int width);
        void frameSetMidLineWidth (int width);
        
	virtual void objectDraw (void);
	virtual void objectGeometry (int x, int y, int w, int h);

	SFrame (SObject *parent, int style = (SFrame::NoFrame | SFrame::Plain));
	~SFrame (void);
};


class SButton : public SFrame {
public:
	int bstate;
	SHandler *buttonHandler;
	
	virtual void buttonPressed (int button);
	virtual void buttonReleased (int button);
	virtual void buttonClicked (int button, int clickCount);

	static void button_cb_o (s_window_t *window, s_event_t *event, s_handler_t *hndl);
	static void button_cb_p (s_window_t *window, s_event_t *event, s_handler_t *hndl);
	static void button_cb_c (s_window_t *window, s_event_t *event, s_handler_t *hndl);
	static void button_cb_r (s_window_t *window, s_event_t *event, s_handler_t *hndl);
	static void button_cb_hr (s_window_t *window, s_event_t *event, s_handler_t *hndl);
	static void button_cb_rh (s_window_t *window, s_event_t *event, s_handler_t *hndl);
	static void button_cb_ho (s_window_t *window, s_event_t *event, s_handler_t *hndl);
	static void button_cb_oh (s_window_t *window, s_event_t *event, s_handler_t *hndl);
	static void button_cb_hoh (s_window_t *window, s_event_t *event, s_handler_t *hndl);

	virtual void objectDraw (void);
	virtual void objectGeometry (int x, int y, int w, int h);

	SButton (SObject *parent, int style = SFrame::NoFrame);
	~SButton (void);
};

class SWindow : public SObject {
public:
	void windowSetTitle (char *fmt, ...);
	void windowFormDraw (void);
	void windowSetCoor (int x, int y, int w, int h, int with_form = WINDOW_TYPE_NOFORM);
	void windowSetResizeable (int resizeable);
	void windowShow (void);
	void windowMain (void);
	
        void windowAtExit (s_window_t *window);
	void windowAtEvent (s_window_t *window, s_event_t *event);
	
	static void windowAtExitCallback (s_window_t *window);
	static void windowAtEventCallback (s_window_t *window, s_event_t *event);

	virtual void objectDraw (void);
	virtual void objectGeometry (int x, int y, int w, int h);

	SWindow (s_window_type_t type = WINDOW_TYPE_MAIN, s_window_t *parent = NULL);
	~SWindow (void);
};

#endif
