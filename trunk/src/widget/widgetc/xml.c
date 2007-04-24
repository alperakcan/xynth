/***************************************************************************
    begin                : Sun Mar 18 2007
    copyright            : (C) 2007 by Alper Akcan
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

#define s_malloc malloc
#define s_free free
#define s_realloc realloc

#define debugf(a, b...) {\
	printf(b);\
	printf(" [%s (%s:%d)]\n", __FUNCTION__, __FILE__, __LINE__);\
}

#define XYNTH_H
#define XYNTH__H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "xml.h"

#include "../../lib/xml.c"
