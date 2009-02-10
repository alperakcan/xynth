#ifndef GUIANIMATION_H_
#define GUIANIMATION_H_

#include "GuiPrimitive.h"

#include <list>
using std::list;

/**
 * \brief Animation primitive
 *
 * Given GUI primitive implements the image animation. Animation is played back
 * on frame basis, where every frame is a separate image. Image file loading is
 * delegated to Xynth engine, so pre-configured formats are supported, typically
 * PNG, JPG, XPM. All frame images must be of the same dimension in order for
 * animation primitive to operate correctly.
 *
 * XML code below defines an animation primitive:
 *
 * \code
 *	<item id="ID" type="animation" x="X" y="Y" width="W" height="H">
 * 		<init delay="D" loop="L" />
 * 		<frame file="/path/to/frame1" />
 * 		<frame file="/path/to/frame2" />
 * 		...
 * 		<touch x="tX" y="tY" width="tW" height="tH"> />
 * 	</item>
 * \endcode
 *
 * Where
 * - ID represents the primitive unique identifier
 * - X, Y, W, H is the set of primitive coordinates in the parent frame
 * - D is the inter-frame animation delay
 * - L is the maximum number of animation loops
 * - /path/to/frameN specifies the location of an image file	for N-th frame
 * - tX, tY, tW, tH are the coordinates of the touchable area in the parent frame
 *
 * Touchable area is optional and usually required only for button type elements.
 *
 * \author Andrei Birjukov
 */

class GuiAnimation : public GuiPrimitive
{
public:
	GuiAnimation(int idResource, GuiFrame* pFrame);
	virtual ~GuiAnimation();

	bool addFrame(const char* fileName);
	void closeFrames();

	void setFrameDelay(int frameDelay);
	void setLoopCount(int loopCount);

	void start();
	void stop();
	bool isPlaying();

protected:

	virtual void onInitialize();
	virtual void onPaint(GuiSurface *drawSurface, s_rect_t &paintArea);
	virtual void onTimer(int idTimer);

private:

	list<s_image_t *>		frameList;
	list<s_image_t *>::iterator	frameIterator;

	int 				loopCount;
	s_timer_t *			frameTimer;
	int				frameDelay;
};

#endif /*GUIANIMATION_H_*/
