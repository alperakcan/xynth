/***************************************************************************
    begin                : Sun Feb 2 2003
    copyright            : (C) 2003 - 2008 by Alper Akcan
    email                : alper.akcan@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#include "../lib/xynth_.h"
#include "server.h"

int s_server_id_get (void)
{
	int i;

	for (i = 0; i < S_CLIENTS_MAX; i++) {
		if (xynth_server->id[i] != 1) {
			xynth_server->id[i] = 1;
			return i;
		}
	}

	return -1;
}

int s_server_id_find (int soc)
{
	int i;

	if (soc < 0) {
		return -1;
	}
	
	for (i = 0; i < S_CLIENTS_MAX; i++) {
		if (xynth_server->client[i].soc == soc) {
			return i;
		}
	}

	return -1;
}

void s_server_id_del (int id)
{
	xynth_server->id[id] = -1;
}
