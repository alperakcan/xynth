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

#include "GuiCheckBox.h"
#include "GuiImageCache.h"

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Class constructor
 *
 * Default constructor for the GUI check-box objects. Initializes check-box
 * state to 'unchecked'.
 *
 * Initially, both check-box images are not loaded, so one has to initialize
 * them separately via loadEnabled() and loadDisabled() methods.
 * Automatic image initialization is performed via GuiTemplate class.
 *
 * \param idResource Uniquie primitive identifier.
 * \param pFrame Pointer to a parent frame containing the primitive.
 */
GuiCheckBox::GuiCheckBox(int idResource, GuiFrame* pFrame)
		: GuiPrimitive(idResource, pFrame)
{
	imgOn = NULL;
	imgOff = NULL;
	isChecked = false;
	showChecked = false;
}

/**
 * \brief Class destructor
 *
 * Destructor for the check-box primitive objects.
 * Unloads currently loaded check-box images, if any.
 *
 */
GuiCheckBox::~GuiCheckBox()
{
	unload();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Load enabled check-box image
 *
 * Loads an image for the enabled face of the check-box primitive.
 *
 * \param fileOn Name of the file containing the enabled face image.
 * Parameter should either contain a full path or be relative to the
 * working directory.
 *
 * \return Returns true if the enabled check-box image was loaded
 * successfully.
 */
bool GuiCheckBox::loadEnabled(const char* fileOn)
{
	lock();
	if(imgOn != NULL) GuiImageCache::closeImage(imgOn);

	imgOn = GuiImageCache::openImage(fileOn);
	unlock();

	return imgOn != NULL;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Load disabled check-box image
 *
 * Loads an image for the disabled face of the check-box primitive.
 *
 * \param fileOff Name of the file containing the disabled face image.
 * Parameter should either contain a full path or be relative to the
 * working directory.
 *
 * \return Returns true if the disabled check-box image was loaded
 * successfully.
 */
bool GuiCheckBox::loadDisabled(const char* fileOff)
{
	lock();
	if(imgOff != NULL) GuiImageCache::closeImage(imgOff);

	imgOff = GuiImageCache::openImage(fileOff);
	unlock();

	return imgOff != NULL;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Unload images
 *
 * Releases resources associated with loaded check-box images.
 */
void GuiCheckBox::unload()
{
	lock();
	GuiImageCache::closeImage(imgOn);
	GuiImageCache::closeImage(imgOff);
	unlock();
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
void GuiCheckBox::onPaint(GuiSurface *drawSurface, s_rect_t &paintArea)
{
	lock();
	if(!isVisible)
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

	s_image_t *img = showChecked ? imgOn : imgOff;
	drawSurface->drawImagePart(surfRect.x, surfRect.y, &relRect, img);
	unlock();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Selection change function of the primitive interface
 *
 * Called by the parent container whenever the selection is being
 * changed. Selection change inside a frame typically results of
 * a user touchsreen manipulation.
 *
 * \param type Specifies the new selection state of the given primitive
 */
void GuiCheckBox::onSelectionChanged(SELECTION type)
{
	// on every press, change the check state to the opposite
	switch (type) {
	case PRESSED:
		showChecked = !isChecked;
		break;
	case SELECTED:
		isChecked = showChecked;
		break;
	case UNKNOWN:
		showChecked = isChecked;
		break;
	default:
		break;
	}

	redraw();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Set current check state
 *
 * Sets current check state of the check-box primitive to a specified
 * value. Also updates current selection of the containing frame.
 *
 * \param check New check state, 'true' if the check-box is checked
 */
void GuiCheckBox::setCheck(bool check)
{
	lock();
	isChecked = check;
	showChecked = check;
	unlock();

	if (check) {
		setSelection(GuiPrimitive::SELECTED);
		pHolder->setFocusedItem(idResource);
	}

	redraw();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Get current check state
 *
 * Returns current check state of the check-box primitive.
 *
 * \return Returns true if the check-box is checked.
 */
bool GuiCheckBox::getCheck()
{
	lock();
	bool check = isChecked;
	unlock();

	return check;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Class constructor
 *
 * Default constructor for the GUI radio-box objects. Initializes radio-box
 * state to 'unchecked'.
 *
 * GuiRadioBox class is derived from the GuiCheckBox parent and inherits all
 * check-box methods except for the selection logic which is customized.
 *
 * Initially, both radio-box images are not loaded,
 * so one has to initialize them separately via loadEnabled() and
 * loadDisabled() methods.
 *
 * \param idResource Uniquie primitive identifier.
 * \param pFrame Pointer to a parent frame containing the primitive.
 */
GuiRadioBox::GuiRadioBox(int idResource, GuiFrame* pFrame)
		: GuiCheckBox(idResource, pFrame)
{
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Selection change function of the primitive interface
 *
 * Called by the parent container whenever the selection is being
 * changed. Selection change inside a frame typically results of
 * a user touchsreen manipulation.
 *
 * \param type Specifies the new selection state of the given primitive
 */
void GuiRadioBox::onSelectionChanged(SELECTION type)
{
	// set the checked state according to selection
	switch (type) {
	case PRESSED:
		showChecked = true;
		break;
	case SELECTED:
		isChecked = true;
		showChecked = true;
		break;
	case UNSELECTED:
		isChecked = false;
		showChecked = false;
	case UNKNOWN:
		showChecked = isChecked;
	}

	redraw();
}
