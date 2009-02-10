#ifndef GUICHECKBOX_H_
#define GUICHECKBOX_H_

#include "GuiPrimitive.h"

/**
 * \brief Checkbox primitive
 * 
 * Given GUI primitive implements a standard check-box element. Visually, 
 * the check-box has two distinct states, being checked and unchecked.
 * Two image files are thus associated with the check-box, covering both states.
 *
 * When the check-box is initially pressed, the checked state is applied and an
 * appropriate image is drawn immediately. The check-box remains checked if 
 * the pressure leaves the check-box within its area. If the pressure is 
 * dragged outside of the check-box and released outside, the check-box restores
 * its unchecked state.
 * 
 * Both images associated with the check-box must have the same dimensions.
 * Images may have arbitrary transparent regions via the PNG alpha channel. 
 * 
 * The check-box primitive does not provide any text and is fully defined
 * using the above two images.
 * 
 * XML code below defines a check-box primitive:
 * 
 * \code
 *	<item id="ID" type="check" x="X" y="Y" width="W" height="H">
 * 		<init on="/path/to/on" off="/path/to/off" />
 * 		<touch x="tX" y="tY" width="tW" height="tH"> />
 * 	</item>
 * \endcode
 * 
 * Where
 * - ID represents the primitive unique identifier
 * - X, Y, W, H is the set of primitive coordinates in the parent frame
 * - /path/to/on specifies the location of a checked image file
 * - /path/to/off specifies the location of an unchecked image file
 * - tX, tY, tW, tH are the coordinates of the touchable area in the parent frame 
 * 
 * \author Andrei Birjukov, Anti Sullin
 */

class GuiCheckBox : public GuiPrimitive
{
public:
	GuiCheckBox(int idResource, GuiFrame* pFrame);
	virtual ~GuiCheckBox();
	
	bool loadEnabled(const char* fileOn);
	bool loadDisabled(const char* fileOff);
	void unload();
	
	void setCheck(bool check);
	bool getCheck();
	
protected:

	virtual void onPaint(GuiSurface *drawSurface, s_rect_t &paintArea);
	virtual void onSelectionChanged(SELECTION type);

	bool		isChecked;
	bool		showChecked;
	
private:

	s_image_t*	imgOn;
	s_image_t*	imgOff;
};

/**
 * \brief Radio button primitive
 * 
 * Given GUI primitive implements a standard radio-box element. Visually, 
 * the radio-box has two distinct states, being checked and unchecked.
 * The difference between a check-box and a radio-box is that check-boxes are 
 * fully independent of each other, while radio-boxes are grouped and only one
 * radio-box can be checked in its group at the same time.
 *    
 * As in the case of the check-box primitive, two image files are associated 
 * with the radio-box, covering both states.
 *
 * When the radio-box is initially pressed, the checked state is applied and an
 * appropriate image is drawn immediately. The radio-box remains checked if 
 * the pressure leaves the radio-box within its area. At the same time the last
 * selected radio-box in the group is deselected, ensuring that the current 
 * selection is the only one. If the pressure is dragged outside of the 
 * radio-box and released outside, the radio-box restores its unchecked state. 
 * 
 * Both images associated with the radio-box must have the same dimensions.
 * Images may have arbitrary transparent regions via the PNG alpha channel. 
 * 
 * The radio-box box primitive does not provide any text and is fully defined
 * using the above two images.
 * 
 * XML code below defines a radio-box primitive:
 * 
 * \code
 *	<item id="ID" type="radio" x="X" y="Y" width="W" height="H">
 * 		<init on="/path/to/on" off="/path/to/off" />
 * 		<touch x="tX" y="tY" width="tW" height="tH"> />
 * 	</item>
 * \endcode
 * 
 * Where
 * - ID represents the primitive unique identifier
 * - X, Y, W, H is the set of primitive coordinates in the parent frame
 * - /path/to/on specifies the location of a checked image file
 * - /path/to/off specifies the location of an unchecked image file
 * - tX, tY, tW, tH are the coordinates of the touchable area in the parent frame 
 * 
 * \author Andrei Birjukov, Anti Sullin
 */

class GuiRadioBox : public GuiCheckBox
{
public:

	GuiRadioBox(int idResource, GuiFrame* pFrame);
	
protected:

	virtual void onSelectionChanged(SELECTION type);
};

#endif /*GUICHECKBOX_H_*/
