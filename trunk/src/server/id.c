/***************************************************************************
    begin                : Wed Jan 29 2003
    copyright            : (C) 2003 - 2014 by Alper Akcan
    email                : alper.akcan@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is dual licensed free software; you can redistribute     * 
 *   it and/or modify it under the terms of the GNU Lesser General Public  *
 *   License, as published by the Free Software Foundation (version 2.1 or * 
 *   later) OR alternatively under the terms of BSD (3-clause) license.    *
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
