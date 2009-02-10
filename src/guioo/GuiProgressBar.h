#ifndef GUIPROGRESSBAR_H_
#define GUIPROGRESSBAR_H_

#include "GuiPrimitive.h"

/**
 * \brief Progress bar primitive
 * 
 * Given GUI primitive implements a progress bar element. The progress bar 
 * primitive utilizes two graphical images to visualize a progress of 
 * an arbitrary process. 
 * 
 * The first image is the static background of the 
 * progress bar. If specified, the background image is always drawn whenever
 * the primitive is painted. 
 * 
 * The second image is the progress indicator that is drawn on top of 
 * the background image. The indicator image is drawn partially, depending
 * on the current progress atrribute.
 * 
 * Both images associated with progress bar must have the same dimensions.
 * Images may have arbitrary transparent regions via the PNG alpha channel. 
 * 
 * XML code below defines a progress bar primitive:
 * 
 * \code
 *	<item id="ID" type="progress" x="X" y="Y" width="W" height="H">
 * 		<init bar="/path/to/face" back="/path/to/back" />
 * 		<touch x="tX" y="tY" width="tW" height="tH"> />
 * 	</item>
 * \endcode
 *  
 * Where
 * - ID represents the primitive unique identifier
 * - X, Y, W, H is the set of primitive coordinates in the parent frame
 * - /path/to/face specifies the location of a progress indicator face image file
 * - /path/to/back specifies the location of a background image file (optional)
 * - tX, tY, tW, tH are the coordinates of the touchable area in the parent frame 
 * 
 * Touchable area is optional and usually required only for button type elements.
 * 
 * \author Andrei Birjukov
 */

class GuiProgressBar : public GuiPrimitive
{
public:
	GuiProgressBar(int idResource, GuiFrame* pFrame);
	virtual ~GuiProgressBar();

	bool loadBar(const char *fileName);
	bool loadBackground(const char *fileName);
	void unload();
	
	void setMax(int maxProgress);
	void setProgress(int pos);
	
protected:
	
	virtual void onPaint(GuiSurface *drawSurface, s_rect_t &paintArea);

private:

	s_image_t*	imgBar;
	s_image_t*	imgBackground;

	int curProgress;
	int maxProgress;
};

#endif /*GUIPROGRESSBAR_H_*/
