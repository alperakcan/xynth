#ifndef GUISCROLLBAR_H_
#define GUISCROLLBAR_H_

#include "GuiPrimitive.h"

/**
 * \brief Scrollbar primitive
 * 
 * Given GUI primitive implements a vertical scrollbar. The scrollbar primitive
 * utilizes four images to visualize the location and the size of the current
 * view in relation to the full size view.
 * 
 * The first image is the static background of the scrollbar. The background is
 * always drawn with the same position and dimensions.
 * 
 * The second image is the scrollbar knob that has dynamic location and length
 * depending on location of the current view. Knob location reflects the 
 * scrolling position of the view, while knob length reflects the size of the
 * view in relation to the full sized view. The progress bar knob can be thus 
 * shifted and scaled along the background image. The implementation 'cuts'
 * the knob from the larger image which must have identical dimensions to 
 * the background image.    
 * 
 * The third and fourth images are the starting and trailing edges of the knob,
 * attached to it on both sides in the moving direction. 
 * The edge images may have arbitrary length.
 *
 * XML code below defines a scroll bar primitive:
 * 
 * \code
 *	<item id="ID" type="scroll" x="X" y="Y" width="W" height="H">
 * 		<init bar="/path/to/face" back="/path/to/back" 
 * 			start="/path/to/start" stop="/path/to/stop" />
 * 		<touch x="tX" y="tY" width="tW" height="tH"> />
 * 	</item>
 * \endcode
 * 
 * Where
 * - ID represents the primitive unique identifier
 * - X, Y, W, H is the set of primitive coordinates in the parent frame
 * - /path/to/face specifies the location of a scroll bar indicator image file
 * - /path/to/back specifies the location of a background image file (optional)
 * - /path/to/start specifies the location of a starting edge image file (optional)
 * - /path/to/stop specifies the location of a trailing edge image file (optional)
 * - tX, tY, tW, tH are the coordinates of the touchable area in the parent frame 
 * 
 * Touchable area is optional and usually required only for button type elements.
 * 
 * \author Andrei Birjukov
 */

class GuiScrollBar : public GuiPrimitive
{
public:
	
	GuiScrollBar(int idResource, GuiFrame* pFrame);
	virtual ~GuiScrollBar();
	
	bool loadBackground(const char* fileName);
	bool loadBarFill(const char* fileName);
	bool loadBarEdges(const char* startName, const char* stopName);
	void unload();
	
	void setPosition(int relPos, int relSize);
	
protected:
	
	virtual void onPaint(GuiSurface *drawSurface, s_rect_t &paintArea);

private:

	s_image_t*	imgBackground;
	s_image_t*	imgBar;
	s_image_t*	imgEdgeStart;
	s_image_t*	imgEdgeStop;
	
	int			relativePosition;
	int			relativeSize;
};

#endif /*GUISCROLLBAR_H_*/
