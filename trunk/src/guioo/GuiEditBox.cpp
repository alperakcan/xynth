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

#include "GuiEditBox.h"

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief
 *
 * \param idResource Unique primitive identifier.
 * \param pFrame Pointer to a parent frame containing the primitive.
 */
GuiEditBox::GuiEditBox(int idResource, GuiFrame* pFrame)
		: GuiPrimitive(idResource, pFrame)
{
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief
 *
 * \return
 */
GuiEditBox::~GuiEditBox()
{
}
