#ifndef GUITEMPLATE_H_
#define GUITEMPLATE_H_

#include <xynth.h>
#include "GuiWindow.h"

/**
 * \brief Skinnable GUI implemetnation
 * 
 * Given class implements the window skin function. Template object loads an
 * XML file describing the look of the user interface. Once application 
 * windows are created, their GUI primitives may be initialized by loading an
 * appropriate window template. With such appoarch, the major part of the user 
 * interface design may remain separate from the application logic and 
 * actual code.
 * 
 * Once instantiated, the template object builds a DOM tree from the 
 * specified XML GUI skin file. The windows implemented via GuiWindow class
 * may then load their content from a defined template using their unique 
 * window identifiers. The templates in the skin file determine the frames
 * and primitives that apply to corresponding windows. 
 *
 * Below is a structure of XML code that is used to define user interface
 * for multiple windows:
 * 
 * \code
 * <?xml version="1.0"?>
 * <guiskin version="1.0">
 *	<window id="1" ...>
 * 		<frame id="1" ...>
 *			<item id="1" type="button" ...>
 * 				<init ... />
 * 			</item>
 * 			<item id="2" type="image" ...>
 * 				...
 * 			</item>
 * 		</frame>
 * 		<frame id="2" ...>
 * 			...
 * 		</frame>
 * 	</window>
 *	<window id="2" ...>
 * 		<frame id="1" ...>
 * 			...
 * 		</frame>
 * 	</window>
 * </guiskin>
 * \endcode
 * 
 * For more information about the structure of the 'window' tag, please refer
 * to the GuiWindow class documentation. For information about the 'frame' tag, 
 * refer to the GuiFrame class documentation. For information about primitive 
 * tags, refer to a specific primitive class documentation.
 *  
 * \author Andrei Birjukov
 */

class GuiTemplate
{
public:
	GuiTemplate();
	virtual ~GuiTemplate();
	
	bool loadFile(const char* fileName);
	bool initWindowLayout(GuiWindow *pWnd);
	void unload();
	
private:
	
	int initFrames(s_xml_node_t *pNode, GuiWindow *pWnd);
	int initPrimitives(s_xml_node_t *pNode, GuiFrame *pFrame);

	s_xml_node_t *rootNode;
	
};

#endif /*GUITEMPLATE_H_*/
