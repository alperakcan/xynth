/***************************************************************************
    begin                : Thu Sep 1 2005
    copyright            : (C) 2005 - 2006 by Alper Akcan
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

#if defined(SINGLE_APP)

#include "../lib/xynth_.h"
#include "server.h"

#if defined(SINGLE_APP_DESKTOP)
	extern s_single_app_t single_desktop;
#endif /* SINGLE_APP_DESKTOP */
#if defined(SINGLE_APP_CHILD)
	extern s_single_app_t single_child;
#endif /* SINGLE_APP_CHILD */
#if defined(SINGLE_APP_HIDE)
	extern s_single_app_t single_hide;
#endif /* SINGLE_APP_HIDE */
#if defined(SINGLE_APP_LOAD)
	extern s_single_app_t single_load;
#endif /* SINGLE_APP_LOAD */
#if defined(SINGLE_APP_SIMPLE)
	extern s_single_app_t single_simple;
#endif /* SINGLE_APP_SIMPLE */
#if defined(SINGLE_APP_OBJECT)
	extern s_single_app_t single_object;
#endif /* SINGLE_APP_OBJECT */
#if defined(SINGLE_APP_ONSCREENKEYBOARD)
	extern s_single_app_t single_onscreenkeyboard;
#endif /* SINGLE_APP_ONSCREENKEYBOARD */
#if defined(SINGLE_APP_TEMP)
	extern s_single_app_t single_temp;
#endif /* SINGLE_APP_TEMP */
#if defined(SINGLE_APP_TERM)
	extern s_single_app_t single_term;
#endif /* SINGLE_APP_TERM */
#if defined(SINGLE_APP_TIMER)
	extern s_single_app_t single_timer;
#endif /* SINGLE_APP_TIMER */
#if defined(SINGLE_APP_LOGOUT)
	extern s_single_app_t single_xynthlogout;
#endif /* SINGLE_APP_LOGOUT */

s_single_app_t *s_server_single_apps[] = {
#if defined(SINGLE_APP_DESKTOP)
	&single_desktop,
#endif /* SINGLE_APP_DESKTOP */
#if defined(SINGLE_APP_CHILD)
	&single_child,
#endif /* SINGLE_APP_CHILD */
#if defined(SINGLE_APP_HIDE)
	&single_hide,
#endif /* SINGLE_APP_HIDE */
#if defined(SINGLE_APP_LOAD)
	&single_load,
#endif /* SINGLE_APP_LOAD */
#if defined(SINGLE_APP_SIMPLE)
	&single_simple,
#endif /* SINGLE_APP_SIMPLE */
#if defined(SINGLE_APP_OBJECT)
	&single_object,
#endif /* SINGLE_APP_OBJECT */
#if defined(SINGLE_APP_ONSCREENKEYBOARD)
	&single_onscreenkeyboard,
#endif /* SINGLE_APP_ONSCREENKEYBOARD */
#if defined(SINGLE_APP_TEMP)
	&single_temp,
#endif /* SINGLE_APP_TEMP */
#if defined(SINGLE_APP_TERM)
	&single_term,
#endif /* SINGLE_APP_TERM */
#if defined(SINGLE_APP_TIMER)
	&single_timer,
#endif /* SINGLE_APP_TIMER */
#if defined(SINGLE_APP_LOGOUT)
	&single_xynthlogout,
#endif /* SINGLE_APP_LOGOUT */
	NULL
};

void * s_server_single_app (void *arg)
{
	s_single_app_t *sapp = (s_single_app_t *) arg;
	debugf(DSER, "started application %s", sapp->argv[0]);
	sapp->fonk(sapp->argc, sapp->argv);
	return NULL;
}

void s_server_single_start (void)
{
	s_single_app_t **sa;
	debugf(DSER, "starting applications");
#if defined(SINGLE_APP_DESKTOP)
	for (sa = s_server_single_apps; *sa; sa++) {
		if (strcmp((*sa)->argv[0], "desktop") == 0) {
			s_thread_create(s_server_single_app, *sa);
		}
	}
#else
	for (sa = s_server_single_apps; *sa; sa++) {
		if (strcmp((*sa)->argv[0], "xynthlogout") != 0) {
			s_thread_create(s_server_single_app, *sa);
		}
	}
#endif
	return;
}

void s_server_single_stop (void)
{
}

#endif /* SINGLE_APP */
