/***************************************************************************
 *   Copyright            : (c) 2007-2008 Artec Design LLC                 *
 *                        : (c) 2007-2008 Andrei Birjukov                  *
 *                        : (c) 2007-2008 Anti Sullin                      *
 *                        : (c) 2008 Jaanus Sepp
 *   Contact              : andrei.birjukov at artecdesign dot ee          *
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#include "GuiTemplate.h"
#include "GuiImage.h"
#include "GuiCheckBox.h"
#include "GuiEditBox.h"
#include "GuiTextBox.h"
#include "GuiMultiTextBox.h"
#include "GuiTextMatrix.h"
#include "GuiButton.h"
#include "GuiRectangle.h"
#include "GuiAnimation.h"
#include "GuiProgressBar.h"
#include "GuiScrollBar.h"

/**
 * \mainpage
 *
 * Welcome to the \b guioo documentation.
 *
 * Guioo is a GUI class library that exists as an extension to the Xynth GUI
 * engine. The library is designed for use by C++ applications on the client
 * side of the GUI engine and is mostly targeted to embedded devices with
 * simple static interfaces.
 *
 * The goals of the guioo library are as follows:
 * - Implement a set of standard elements of the user interface (GUI
 * primitives)
 * - Provide a layer of isolation between the code of an application and
 * the design of a user interface
 * - Enable for a fully customizable GUI in terms of look and feel that supports
 * transparent primitives
 * - Provide building blocks for static and scrollable windows
 * - Implement an additional window sub-layering via window frames
 * - Implement interface for touchscreen and hardware button input
 * - Provide a convenient interface to timers
 *
 * \author Andrei Birjukov, Anti Sullin @ Artec Design LLC
 */

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Class constructor
 *
 * Default constructor for the GUI template object. DOM tree is empty at the
 * moment of creation.
 */
GuiTemplate::GuiTemplate()
{
	rootNode = NULL;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Class destructor
 *
 * Destructor for the GUI template objects.
 * Also releases the DOM tree and frees resources.
 */
GuiTemplate::~GuiTemplate()
{
	if(rootNode) s_xml_node_uninit(rootNode);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Load skin file
 *
 * Loads GUI window templates from the specified XML skin file. The DOM tree is
 * built and kept in memory until explicitly unloaded or the template object
 * is destroyed.
 *
 * \param fileName Specifies the path to the XML skin file
 * \return Returns true if the skin file is loaded successfully.
 */
bool GuiTemplate::loadFile(const char* fileName)
{
	if(rootNode) s_xml_node_uninit(rootNode);
	rootNode = NULL;

	// load the skin file
	int ret = s_xml_parse_file(&rootNode, (char*)fileName);
	if(ret != 0)
	{
		s_xml_node_uninit(rootNode);
		rootNode = NULL;
		return false;
	}

	// check the first XML node
	if(strcmp(rootNode->name, "guiskin"))
	{
		s_xml_node_uninit(rootNode);
		rootNode = NULL;
		return false;
	}

	// check the skin version
	char *ver = s_xml_node_get_attr_value(rootNode, "version");
	if(strcmp(ver, "1.0"))
	{
		s_xml_node_uninit(rootNode);
		rootNode = NULL;
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Unload skin
 *
 * Releases DOM tree and frees resources.
 */
void GuiTemplate::unload()
{
	s_xml_node_uninit(rootNode);
	rootNode = NULL;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Initialize window components
 *
 * Given method builds the user interface for the specified window from a
 * template stored to a loaded XML skin file. The window is looked up from
 * the loaded DOM tree basing on its unique identifier. Frames and primitives
 * are automatically instantiated and inserted into the given window.
 *
 *
 * \param pWnd Pointer to a window object for which GUI elements are loaded
 * \return Returns true if the window interface is built successfully.
 */
bool GuiTemplate::initWindowLayout(GuiWindow *pWnd)
{
	if(rootNode == NULL) return false;
	s_list_node_t *listNode = rootNode->nodes->node;

	// find the right window first
	for(int nodeItem = 0; nodeItem < rootNode->nodes->nb_elt; nodeItem ++)
	{
		s_xml_node_t *xmlNode = (s_xml_node_t*)listNode->element;

		// what we got here is a window
		if(!strcmp(xmlNode->name, "window"))
		{
			// grab window ID
			char* resId = s_xml_node_get_attr_value(xmlNode, "id");
			int iResId = (resId != NULL) ? atoi(resId) : -1;

			// found a window with the given resource ID
			if(pWnd->getResId() == iResId)
			{
				// grab window properties
				char* wndX = s_xml_node_get_attr_value(xmlNode, "x");
				char* wndY = s_xml_node_get_attr_value(xmlNode, "y");
				char* wndWidth = s_xml_node_get_attr_value(xmlNode, "width");
				char* wndHeight = s_xml_node_get_attr_value(xmlNode, "height");
				char* wndOnTop = s_xml_node_get_attr_value(xmlNode, "ontop");

				// validate properties
				if(wndX == NULL || wndY == NULL ||
					wndWidth == NULL || wndHeight == NULL) return -1;

				// set window properties
				s_rect_t wndPos = { atoi(wndX), atoi(wndY), atoi(wndWidth), atoi(wndHeight) };
				pWnd->setWindowPos(wndPos);
				if(wndOnTop) pWnd->setAlwaysOnTop(atoi(wndOnTop));

				// add frames to the window
				return initFrames(xmlNode, pWnd) > 0;
			}
		}

		listNode = (s_list_node_t*)listNode->next;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Initialize frames of the given window
 *
 * Loads and initializes GUI frames for the given window. Calls to primitive
 * initialization function for each valid frame.
 *
 * \param pNode Pointer to a DOM node containing the elements of the given
 * window
 * \param pWnd Pointer to a window that requires initialization
 * \return Returns a number of frames inserted into the window.
 */
int GuiTemplate::initFrames(s_xml_node_t *pNode, GuiWindow *pWnd)
{
	int numFrames = 0;
	s_list_node_t *listNode = pNode->nodes->node;
	for(int nodeItem = 0; nodeItem < pNode->nodes->nb_elt; nodeItem ++)
	{
		s_xml_node_t *xmlNode = (s_xml_node_t*)listNode->element;

		// what we got here is a frame of a window
		if(!strcmp(xmlNode->name, "frame"))
		{
			// grab frame properties
			char* frameX = s_xml_node_get_attr_value(xmlNode, "x");
			char* frameY = s_xml_node_get_attr_value(xmlNode, "y");
			char* frameWidth = s_xml_node_get_attr_value(xmlNode, "width");
			char* frameHeight = s_xml_node_get_attr_value(xmlNode, "height");
			char* virtWidth = s_xml_node_get_attr_value(xmlNode, "vx");
			char* virtHeight = s_xml_node_get_attr_value(xmlNode, "vy");
			char* frameBuf = s_xml_node_get_attr_value(xmlNode, "buf");
			char* frameVisible = s_xml_node_get_attr_value(xmlNode, "visible");

			// grab frame ID
			char* resId = s_xml_node_get_attr_value(xmlNode, "id");
			int iResId = (resId != NULL) ? atoi(resId) : -1;

			// validate properties
			if(frameX == NULL || frameY == NULL ||
				frameWidth == NULL || frameHeight == NULL) return -1;

			if(virtWidth == NULL) virtWidth = frameWidth;
			if(virtHeight == NULL) virtHeight = frameHeight;

			// initialize frame object
			GuiFrame *pFrame;
			if(frameBuf && !strcmp(frameBuf, "y"))
			{
				// create a buffered frame
				pFrame = new GuiBufferedFrame(iResId, pWnd);
			}
			else
			{
				// create a non-buffered, or transparent frame
				pFrame = new GuiFrame(iResId, pWnd);
			}

			s_rect_t frameArea = { atoi(frameX), atoi(frameY),
								atoi(frameWidth), atoi(frameHeight) };

			pFrame->setGeometry(frameArea);

			if(frameVisible)
				pFrame->setVisible(strcmp(frameVisible, "y") == 0);

			// add primitives to the frame
			initPrimitives(xmlNode, pFrame);

			// initialize the frame surface
			if(!pFrame->initSurface(atoi(virtWidth), atoi(virtHeight)))
			{
				delete pFrame;
				return -1;
			}

			// finally, add the frame to the parent window
			pWnd->addFrame(pFrame);
			numFrames ++;
		}

		listNode = (s_list_node_t*)listNode->next;
	}

	return numFrames;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Initialize primitives of the given frame
 *
 * Creates and initializes GUI primitive for the given frame.
 *
 * \param pNode Pointer to a DOM node containing the elements of the given
 * frame
 * \param pFrame Pointer to a frame that requires initialization
 * \return Returns a number of primitives inserted into the frame.
 */
int GuiTemplate::initPrimitives(s_xml_node_t *pNode, GuiFrame *pFrame)
{
	int numPrimitives = 0;
	s_list_node_t *listNode = pNode->nodes->node;
	for(int nodeItem = 0; nodeItem < pNode->nodes->nb_elt; nodeItem ++)
	{
		s_xml_node_t *xmlNode = (s_xml_node_t*)listNode->element;

		// what we got here is a GUI primitive
		if(!strcmp(xmlNode->name, "item"))
		{
			// grab primitive properties
			char* itemX = s_xml_node_get_attr_value(xmlNode, "x");
			char* itemY = s_xml_node_get_attr_value(xmlNode, "y");
			char* itemWidth = s_xml_node_get_attr_value(xmlNode, "width");
			char* itemHeight = s_xml_node_get_attr_value(xmlNode, "height");
			char* itemType = s_xml_node_get_attr_value(xmlNode, "type");
			char* itemRes = s_xml_node_get_attr_value(xmlNode, "id");
			char* itemVisible = s_xml_node_get_attr_value(xmlNode, "visible");

			// validate properties
			if(itemX == NULL || itemY == NULL || itemWidth == NULL ||
				itemHeight == NULL || itemType == NULL || itemRes == NULL) continue;

			// extract primitive init and touch child nodes
			s_xml_node_t *xmlInit = NULL;
			s_xml_node_t *xmlTouch = NULL;
			s_list_node_t *childNode = xmlNode->nodes->node;

			for(int nodeParams=0; nodeParams < xmlNode->nodes->nb_elt; nodeParams++)
			{
				s_xml_node_t *xmlTemp = (s_xml_node_t*)childNode->element;
				if(!strcmp(xmlTemp->name, "init")) xmlInit = xmlTemp;
				else if(!strcmp(xmlTemp->name, "touch")) xmlTouch = xmlTemp;
				childNode = (s_list_node_t*)childNode->next;
			}

			// initialize the primitive object
			GuiPrimitive *pItem = NULL;

			// create an appropriate primitive basing on specified type
			if(!strcmp(itemType, "image"))
			{
				if(xmlInit == NULL) continue;
				pItem = new GuiImage(atoi(itemRes), pFrame);

				char* layoutType = s_xml_node_get_attr_value(xmlInit, "layout");
				char* fileName = s_xml_node_get_attr_value(xmlInit, "file");

				if(fileName) ((GuiImage*)pItem)->loadImage(fileName);
				if(layoutType) ((GuiImage*)pItem)->setLayout(atoi(layoutType));
			}
			else if(!strcmp(itemType, "button"))
			{
				if(xmlInit == NULL) continue;

				char* fileDown = s_xml_node_get_attr_value(xmlInit, "down");
				char* fileUp = s_xml_node_get_attr_value(xmlInit, "up");

				if(fileUp == NULL && fileDown == NULL) continue;
				pItem = new GuiButton(atoi(itemRes), pFrame);

				if(fileUp) ((GuiButton*)pItem)->loadReleased(fileUp);
				if(fileDown) ((GuiButton*)pItem)->loadPressed(fileDown);
			}
			else if(!strcmp(itemType, "rect"))
			{
				if(xmlInit == NULL) continue;
				pItem = new GuiRectangle(atoi(itemRes), pFrame);

				char* colBackgnd = s_xml_node_get_attr_value(xmlInit, "back");
				char* colForegnd = s_xml_node_get_attr_value(xmlInit, "fore");
				char* colGrad = s_xml_node_get_attr_value(xmlInit, "grad");

				if(colBackgnd) ((GuiRectangle*)pItem)->setBackGround(colBackgnd);
				if(colForegnd) ((GuiRectangle*)pItem)->setForeGround(colForegnd);
				if(colGrad) ((GuiRectangle*)pItem)->setGradient(colGrad);
			}
			else if(!strcmp(itemType, "check"))
			{
				if(xmlInit == NULL) continue;

				char* fileOn = s_xml_node_get_attr_value(xmlInit, "on");
				char* fileOff = s_xml_node_get_attr_value(xmlInit, "off");
				char* checkState = s_xml_node_get_attr_value(xmlInit, "check");

				if(fileOn == NULL || fileOff == NULL) continue;
				pItem = new GuiCheckBox(atoi(itemRes), pFrame);

				((GuiCheckBox*)pItem)->loadEnabled(fileOn);
				((GuiCheckBox*)pItem)->loadDisabled(fileOff);
				if(checkState) ((GuiCheckBox*)pItem)->setCheck(atoi(checkState) == 1);
			}
			else if(!strcmp(itemType, "radio"))
			{
				if(xmlInit == NULL) continue;

				char* fileOn = s_xml_node_get_attr_value(xmlInit, "on");
				char* fileOff = s_xml_node_get_attr_value(xmlInit, "off");
				char* checkState = s_xml_node_get_attr_value(xmlInit, "check");

				if(fileOn == NULL || fileOff == NULL) continue;
				pItem = new GuiRadioBox(atoi(itemRes), pFrame);

				((GuiRadioBox*)pItem)->loadEnabled(fileOn);
				((GuiRadioBox*)pItem)->loadDisabled(fileOff);
				if(checkState) ((GuiRadioBox*)pItem)->setCheck(atoi(checkState) == 1);
			}
			else if(!strcmp(itemType, "text"))
			{
				if(xmlInit == NULL) continue;

				char* textFont = s_xml_node_get_attr_value(xmlInit, "font");
				char* textSize = s_xml_node_get_attr_value(xmlInit, "size");
				char* textAlign = s_xml_node_get_attr_value(xmlInit, "align");

				if(textFont == NULL || textSize == NULL) continue;
				pItem = new GuiTextBox(atoi(itemRes), pFrame);

				char* textColor = s_xml_node_get_attr_value(xmlInit, "color");
				char* textStr = s_xml_node_get_attr_value(xmlInit, "text");

				((GuiTextBox*)pItem)->setFont(textFont, atoi(textSize));
				if(textColor) ((GuiTextBox*)pItem)->setColor(textColor);
				if(textStr) ((GuiTextBox*)pItem)->setText(textStr);
				if(textAlign && strlen(textAlign) >= 2)
				{
					// default text position is top-left
					GuiTextBox::TextHAlignment hPos = GuiTextBox::LEFT;
					GuiTextBox::TextVAlignment vPos = GuiTextBox::TOP;

					switch(textAlign[0])	// determine the horizontal position
					{
					case 'l':
						hPos = GuiTextBox::LEFT;
						break;
					case 'r':
						hPos = GuiTextBox::RIGHT;
						break;
					case 'c':
						hPos = GuiTextBox::HCENTER;
						break;
					}

					switch(textAlign[1])	// determine the vertical position
					{
					case 't':
						vPos = GuiTextBox::TOP;
						break;
					case 'b':
						vPos = GuiTextBox::BOTTOM;
						break;
					case 'c':
						vPos = GuiTextBox::VCENTER;
						break;
					case 'n':
						vPos = GuiTextBox::VNORMAL;
						break;
					}

					((GuiTextBox*)pItem)->setAlignment(hPos, vPos);
				}
			}
			else if(!strcmp(itemType, "multitext"))
			{
				if(xmlInit == NULL) continue;

				char* textFont = s_xml_node_get_attr_value(xmlInit, "font");
				char* textSize = s_xml_node_get_attr_value(xmlInit, "size");
				char* textAlign = s_xml_node_get_attr_value(xmlInit, "align");

				if(textFont == NULL || textSize == NULL) continue;
				pItem = new GuiMultiTextBox(atoi(itemRes), pFrame);

				char* textColor = s_xml_node_get_attr_value(xmlInit, "color");
				char* textStr = s_xml_node_get_attr_value(xmlInit, "text");

				((GuiMultiTextBox*)pItem)->setFont(textFont, atoi(textSize));
				if(textColor) ((GuiMultiTextBox*)pItem)->setColor(textColor);
				if(textStr) ((GuiMultiTextBox*)pItem)->setText(textStr);
				if(textAlign && strlen(textAlign) >= 1)
				{
					// default text position is top-left
					GuiMultiTextBox::TextHAlignment hPos = GuiMultiTextBox::LEFT;

					switch(textAlign[0])	// determine the horizontal position
					{
					case 'l':
						hPos = GuiMultiTextBox::LEFT;
						break;
					case 'r':
						hPos = GuiMultiTextBox::RIGHT;
						break;
					case 'c':
						hPos = GuiMultiTextBox::HCENTER;
						break;
					}

					((GuiMultiTextBox*)pItem)->setAlignment(hPos);
				}
			}
			else if(!strcmp(itemType, "matrix"))
			{
				if(xmlInit == NULL) return -1;

				char* textFont = s_xml_node_get_attr_value(xmlInit, "font");
				char* textSize = s_xml_node_get_attr_value(xmlInit, "size");

				if(textFont == NULL || textSize == NULL) return -1;
				pItem = new GuiTextMatrix(atoi(itemRes), pFrame, atoi(itemWidth), atoi(itemHeight));

				char *staticTextColor =
					s_xml_node_get_attr_value(xmlInit, "static_text_color");
				char *inactiveFieldColor =
					s_xml_node_get_attr_value(xmlInit, "inactive_field_color");
				char *activeFieldColor =
					s_xml_node_get_attr_value(xmlInit, "active_field_color");
				char *selectionColor =
					s_xml_node_get_attr_value(xmlInit, "selection_color");
				char *selectionBgColor =
					s_xml_node_get_attr_value(xmlInit, "selection_bg_color");

				char *cols = s_xml_node_get_attr_value(xmlInit, "cols");
				char *rows = s_xml_node_get_attr_value(xmlInit, "rows");

				((GuiTextMatrix*)pItem)->setFont(textFont, atoi(textSize));
				((GuiTextMatrix*)pItem)->setColor(staticTextColor,
													inactiveFieldColor,
													activeFieldColor,
													selectionColor,
													selectionBgColor);

				if ((cols != NULL) && (rows != NULL))
					((GuiTextMatrix*)pItem)->setDimensions(atoi(cols), atoi(rows));
			}
			else if(!strcmp(itemType, "edit"))
			{
				if(xmlInit == NULL) continue;
				pItem = new GuiEditBox(atoi(itemRes), pFrame);
			}
			else if(!strcmp(itemType, "animation"))
			{
				if(xmlInit == NULL) continue;
				pItem = new GuiAnimation(atoi(itemRes), pFrame);
				childNode = xmlNode->nodes->node;

				for(int nodeParams=0;
						nodeParams < xmlNode->nodes->nb_elt; nodeParams++)
				{
					s_xml_node_t *xmlTemp = (s_xml_node_t*)childNode->element;
					if(!strcmp(xmlTemp->name, "frame"))
					{
						char* fileName = s_xml_node_get_attr_value(xmlTemp, "file");
						if(fileName) ((GuiAnimation*)pItem)->addFrame(fileName);
					}

					childNode = (s_list_node_t*)childNode->next;
				}

				char* frameDelay = s_xml_node_get_attr_value(xmlInit, "delay");
				char* loopCount = s_xml_node_get_attr_value(xmlInit, "loop");
				if(frameDelay) ((GuiAnimation*)pItem)->setFrameDelay(atoi(frameDelay));
				if(loopCount) ((GuiAnimation*)pItem)->setLoopCount(atoi(loopCount));
			}
			else if(!strcmp(itemType, "progress"))
			{
				if(xmlInit == NULL) continue;

				char* fileBack = s_xml_node_get_attr_value(xmlInit, "back");
				char* fileBar = s_xml_node_get_attr_value(xmlInit, "bar");

				if(fileBar == NULL) continue;
				pItem = new GuiProgressBar(atoi(itemRes), pFrame);

				if(fileBack) ((GuiProgressBar*)pItem)->loadBackground(fileBack);
				((GuiProgressBar*)pItem)->loadBar(fileBar);
			}
			else if(!strcmp(itemType, "scroll"))
			{
				if(xmlInit == NULL) continue;

				char* fileBack = s_xml_node_get_attr_value(xmlInit, "back");
				char* fileBar = s_xml_node_get_attr_value(xmlInit, "bar");
				char* fileStart = s_xml_node_get_attr_value(xmlInit, "start");
				char* fileStop = s_xml_node_get_attr_value(xmlInit, "stop");

				if(fileBar == NULL) continue;
				pItem = new GuiScrollBar(atoi(itemRes), pFrame);

				if(fileBack) ((GuiScrollBar*)pItem)->loadBackground(fileBack);
				((GuiScrollBar*)pItem)->loadBarFill(fileBar);
				if(fileStart && fileStop)
					((GuiScrollBar*)pItem)->loadBarEdges(fileStart, fileStop);
			}

			// initialize the primitive position
			s_rect_t itemArea = { atoi(itemX), atoi(itemY),
								atoi(itemWidth), atoi(itemHeight) };

			if(pItem == NULL) continue;

			pItem->setPosition(itemArea);

			// set visibility flag
			pItem->setVisible(!(itemVisible != NULL && !strcmp(itemVisible, "n")));

			// extract data for the touch region, if any
			if(xmlTouch != NULL)
			{
				char* touchX = s_xml_node_get_attr_value(xmlTouch, "x");
				char* touchY = s_xml_node_get_attr_value(xmlTouch, "y");
				char* touchWidth = s_xml_node_get_attr_value(xmlTouch, "width");
				char* touchHeight = s_xml_node_get_attr_value(xmlTouch, "height");

				if(touchX && touchY && touchWidth && touchHeight)
				{
					s_rect_t touchArea = { atoi(touchX), atoi(touchY),
											atoi(touchWidth), atoi(touchHeight) };
					pItem->setTouchArea(touchArea);
				}
				else xmlTouch = NULL;
			}

			// finally, add the primitive to the frame
			pFrame->addPrimitive(pItem);
			numPrimitives ++;
		}

		listNode = (s_list_node_t*)listNode->next;
	}

	return numPrimitives;
}

