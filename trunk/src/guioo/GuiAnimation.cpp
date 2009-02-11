/***************************************************************************
 *   Copyright            : (c) 2007-2008 Artec Design LLC                 *
 *                        : (c) 2007-2008 Andrei Birjukov                  *
 *                        : (c) 2007-2008 Anti Sullin                      *
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

#include "GuiAnimation.h"
#include "GuiImageCache.h"

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Class constructor
 *
 * Default constructor for the GUI animation objects. Initializes inter-frame
 * delay to zero and loop count to infinity.
 *
 * Automatic animation frame initialization is performed via GuiTemplate class.
 *
 * \param idResource Unique primitive identifier.
 * \param pFrame Pointer to a parent frame containing the primitive.
 */

GuiAnimation::GuiAnimation(int idResource, GuiFrame* pFrame)
		: GuiPrimitive(idResource, pFrame)
{
	frameDelay = 0;
	loopCount = -1;

	frameIterator = frameList.end();
	frameTimer = NULL;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Class destructor
 *
 * Destructor for the GUI animation objects. Stops the animation timer, if it
 * still exists and deletes all loaded frame images.
 */

GuiAnimation::~GuiAnimation()
{
	if(frameTimer) pHolder->stopTimer(frameTimer);

	if(!frameList.empty())
	{
		list<s_image_t*>::iterator it;
		for(it = frameList.begin(); it != frameList.end(); it ++)
			GuiImageCache::closeImage(*it);

		frameList.clear();
	}
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Add a frame to the animation primitive.
 *
 * Given method inserts a new image frame at the end of the animation frame
 * list. Primitive takes care of loading and decoding the image file data.
 * Primitive also uses a global image cache that increases loading speed if the
 * image is used somewhere else.
 *
 * \param fileName Image file name, either full path or relative to the working
 * directory.
 * \return Returns true if the frame is added successfully.
 */
bool GuiAnimation::addFrame(const char* fileName)
{
	lock();
	s_image_t* imgData = GuiImageCache::openImage(fileName);
	if(imgData != NULL)
	{
		frameList.push_back(imgData);
		unlock();
		return true;
	}

	unlock();
	return false;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Close all frames
 *
 * Unloads all frames currently maintained by the animation primitive.
 */
void GuiAnimation::closeFrames()
{
	lock();
	list<s_image_t*>::iterator it;
	for(it = frameList.begin(); it != frameList.end(); it ++)
		GuiImageCache::closeImage(*it);

	frameList.clear();
	unlock();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Set frame delay
 *
 * Specifies the playback inter-frame delay of the animation primitive.
 * \param frameDelay Inter-frame delay in milliseconds.
 */
void GuiAnimation::setFrameDelay(int frameDelay)
{
	lock();
	this->frameDelay = frameDelay;
	unlock();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Set number of loops
 *
 * Specifies the number of cycles for the animation list playback.
 * \param loopCount Number of animation loops. Set to -1 for infinite looping.
 */

void GuiAnimation::setLoopCount(int loopCount)
{
	lock();
	this->loopCount = loopCount;
	unlock();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Start animation
 *
 * Starts the animation timer with the pre-defined interval. Each time the
 * timer fires, a next image from the animation list is displayed by the
 * primitive. The animation is always started from the first frame.
 */

void GuiAnimation::start()
{
	lock();
	if(frameTimer) pHolder->stopTimer(frameTimer);

	frameIterator = frameList.begin();
	frameTimer = pHolder->startTimer(frameDelay, 0, this);
	unlock();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Stops animation
 *
 * Stops the animation and destroys its timer.
 */

void GuiAnimation::stop()
{
	lock();
	if(frameTimer)
	{
		pHolder->stopTimer(frameTimer);
		frameTimer = NULL;
	}
	unlock();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Check if animation is playing
 * \return Returns true if the animation is currently being played back.
 */

bool GuiAnimation::isPlaying()
{
	lock();
	bool ret = (frameTimer != NULL);
	unlock();

	return ret;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Painting function of the primitive interface
 *
 * Called by the parent frame whenever a primitive redraw is required.
 * \param drawSurface Pointer to a graphical surface where the primitive should
 * draw itself to.
 * \param paintArea The rectangular area of the parent frame surface that
 * should be re-painted by the primitive.
 */

void GuiAnimation::onPaint(GuiSurface *drawSurface, s_rect_t &paintArea)
{
	lock();
	if(!isVisible || frameIterator == frameList.end())
	{
		unlock();
		return;
	}

	s_image_t* imgData = (s_image_t*)*frameIterator;
	if(imgData == NULL)
	{
		unlock();
		return;
	}

	s_rect_t surfRect;
	if(s_region_rect_intersect(&paintArea, &drawArea, &surfRect))
	{
		unlock();
		return;
	}

	s_rect_t relRect = { surfRect.x - drawArea.x, surfRect.y - drawArea.y,
						surfRect.w, surfRect.h };

	#ifdef DEBUG
	printf("GuiAnimation::onPaint: painting (%d, %d, %d, %d) at (%d, %d)\n",
			paintArea.x, paintArea.y, paintArea.w, paintArea.h,
			drawArea.x, drawArea.y);
	#endif

	drawSurface->drawImagePart(surfRect.x, surfRect.y, &relRect, imgData);
	unlock();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Timer call-back function
 *
 * Called by the parent frame when the timer associated to the primitive
 * has fired.
 *
 * \param idTimer Timer unique identifier
 */

void GuiAnimation::onTimer(int idTimer)
{
	// sanity check
	if(frameIterator == frameList.end()) return;

	frameIterator ++;	// advance to the next frame
	if(frameIterator == frameList.end())
	{
		if(loopCount --) frameIterator = frameList.begin();
		else
		{
			stop();
			return;
		}
	}

	redraw();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Initialize function of the primitive interface
 *
 * Called by the parent frame when the primitive is being initialized. Animation
 * primitive automatically starts to spin here.
 */

void GuiAnimation::onInitialize()
{
	start();
}
