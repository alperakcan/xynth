/***************************************************************************
    begin                : Tue Nov 2 2004
    copyright            : (C) 2004 - 2006 by Alper Akcan
    email                : distchx@yahoo.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#include "xynth_.h"
#include "widgetoo.h"

void SHandler::handlerAdd (void)
{
	s_handler_add(handlerObj->objectWindow, handlerHndl);
}

void SHandler::handlerDel (void)
{
	s_handler_del(handlerObj->objectWindow, handlerHndl);
}

SHandler::SHandler (SObject *obj, S_HANDLER type)
{
        handlerObj = obj;
        
	if (s_handler_init(&handlerHndl)) {
		goto err0;
	}
	handlerHndl->type = type;

	return;
err0:	return;
}

SHandler::~SHandler (void)
{
	handlerDel();
	s_handler_uninit(handlerHndl);
}
