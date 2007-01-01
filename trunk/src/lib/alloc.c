/***************************************************************************
    begin                : Tue Oct 4 2005
    copyright            : (C) 2005 - 2007 by Alper Akcan
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

#include <stdlib.h>
#include "xynth_.h"

void * s_malloc (unsigned int size)
{
	void *ret;
	ret = malloc(size);
	if (ret == NULL) {
		debugf(DFAT, "Not enough memory!");
	}
	return ret;
}

void * s_calloc (unsigned int nmemb, unsigned int size)
{
	void *ret;
	ret = s_malloc(nmemb * size);
	if (ret == NULL) {
		debugf(DFAT, "Not enough memory!");
	}
	memset(ret, 0, nmemb * size);
	return ret;
}

void * s_realloc (void *ptr, unsigned int size)
{
	void *ret;
	ret = realloc(ptr, size);
	if (ret == NULL) {
		debugf(DFAT, "Not enough memory!");
	}
	return ret;
}

void s_free (void *ptr)
{
	if (ptr != NULL) {
		free(ptr);
	}
}
