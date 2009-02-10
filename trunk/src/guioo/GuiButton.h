#ifndef GUIBUTTON_H_
#define GUIBUTTON_H_

#include "GuiPrimitive.h"

/**
 * \brief Button primitive
 * 
 * Given GUI primitive implements a two-state button. The buttan is associated
 * with two images, the first one being the button normal image that is drawn
 * when the button is idle. The second image is the image drawn when the 
 * button is pressed. Both images must have the same dimensions in order to
 * function correctly. Images may have arbitrary transparent regions via the 
 * PNG alpha channel.
 * 
 * The button primitive does not provide any button text, and its visible 
 * part is defined only by the above images. 
 * 
 * XML code below defines a button primitive:
 * 
 * \code
 *	<item id="ID" type="button" x="X" y="Y" width="W" height="H">
 * 		<init up="/path/to/released" down="/path/to/pressed" />
 *		<touch x="tX" y="tY" width="tW" height="tH"> />
 * 	</item>
 * \endcode
 * 
 * Where
 * - ID represents the primitive unique identifier
 * - X, Y, W, H is the set of primitive coordinates in the parent frame
 * - /path/to/released specifies the location of an released button image file
 * - /path/to/pressed specifies the location of a pressed button image file
 * - tX, tY, tW, tH are the coordinates of the touchable area in the parent frame 
 *
 * \author Andrei Birjukov, Anti Sullin
 */

class GuiButton : public GuiPrimitive
{
public:
	GuiButton(int idResource, GuiFrame* pFrame);
	virtual ~GuiButton();
	
	bool loadReleased(const char *fileUp);
	bool loadPressed(const char *fileDown);
	void unload();
	
protected:

	virtual void onPaint(GuiSurface *drawSurface, s_rect_t &paintArea);
	virtual void onSelectionChanged(SELECTION type);
	
private:

	s_image_t*	imgUp;
	s_image_t*	imgDown;
};

#endif /*GUIBUTTON_H_*/
