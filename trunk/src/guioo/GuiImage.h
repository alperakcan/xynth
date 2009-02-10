#ifndef GUIIMAGE_H_
#define GUIIMAGE_H_

#include "GuiPrimitive.h"

/**
 * \brief Image primitive
 * 
 * Given class implements a static image element. Image data is loaded from
 * a specified graphical file and rendered to the containing window when
 * required. 
 * 
 * XML code below defines an image primitive:
 * 
 * \code
 *	<item id="ID" type="image" x="X" y="Y" width="W" height="H">
 * 		<init file="/path/to/image" />
 * 		<touch x="tX" y="tY" width="tW" height="tH"> />
 * 	</item>
 * \endcode
 * 
 * Where
 * - ID represents the primitive unique identifier
 * - X, Y, W, H is the set of primitive coordinates in the parent frame
 * - /path/to/file specifies the location of an image file
 * - tX, tY, tW, tH are the coordinates of the touchable area in the parent frame 
 * 
 * Touchable area is optional and usually required only for button type elements.
 * 
 * \author Andrei Birjukov
 */

class GuiImage : public GuiPrimitive
{
public:

	enum 
	{
		IMAGE_LAYOUT_UPPERLEFT = 0,
		IMAGE_LAYOUT_SCALE,
		IMAGE_LAYOUT_FILL
	};

	GuiImage(int idResource, GuiFrame* pFrame);
	virtual ~GuiImage();
	
	bool loadImage(const char *fileName);
	void close();
	void setLayout(int layoutType);

protected:

	virtual void onPaint(GuiSurface *drawSurface, s_rect_t &paintArea);

private:

	s_image_t*	imgData;
	int layoutType;
};

#endif /*GUIIMAGE_H_*/
