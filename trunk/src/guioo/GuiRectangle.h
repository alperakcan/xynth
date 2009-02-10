#ifndef GUIRECTANGLE_H_
#define GUIRECTANGLE_H_

#include "GuiSurface.h"
#include "GuiPrimitive.h"

/**
 * \brief Rectangle primitive
 * 
 * Given GUI primitive implements a filled rectangle. The rectangle is 
 * painted with the specified color and coordinates inside window frame.
 * 
 * XML code below defines an image primitive:
 * 
 * \code
 *	<item id="ID" type="rect" x="X" y="Y" width="W" height="H">
 * 		<init fore="RRGGBB" />
 * 		<touch x="tX" y="tY" width="tW" height="tH"> />
 * 	</item>
 * \endcode
 * 
 * Where
 * - ID represents the primitive unique identifier
 * - X, Y, W, H is the set of primitive coordinates in the parent frame
 * - RRGGBB represents the RGB color used for the rectangle filling (e.g. "6699cc") 
 * - tX, tY, tW, tH are the coordinates of the touchable area in the parent frame 
 * 
 * Touchable area is optional and usually required only for button type elements.
 * 
 * \author Andrei Birjukov
 */

class GuiRectangle : public GuiPrimitive
{
public:
	GuiRectangle(int idResource, GuiFrame* pFrame);
	virtual ~GuiRectangle();
	
	void setForeGround(const char* rgb);
	void setBackGround(const char* rgb);
	void setGradient(const char* rgb);

protected:

	virtual void onPaint(GuiSurface *drawSurface, s_rect_t &paintArea);

private:

	RGB_COLOR colForeGround;
	RGB_COLOR colBackGround;
	RGB_COLOR colGradient;
	bool drawGradient;
};

#endif /*GUIRECTANGLE_H_*/
