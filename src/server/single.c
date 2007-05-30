/***************************************************************************
    begin                : Thu Sep 1 2005
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

#if defined(SINGLE_APP)

#include "../lib/xynth_.h"
#include "server.h"

#if defined(DEMO_DESKTOP)
	extern s_single_app_t single_desktop;
#endif /* DEMO_DESKTOP */
#if defined(DEMO_CHILD)
	extern s_single_app_t single_child;
#endif /* DEMO_CHILD */
#if defined(DEMO_HIDE)
	extern s_single_app_t single_hide;
#endif /* DEMO_HIDE */
#if defined(DEMO_LOAD)
	extern s_single_app_t single_load;
#endif /* DEMO_LOAD */
#if defined(DEMO_SIMPLE)
	extern s_single_app_t single_simple;
#endif /* DEMO_SIMPLE */
#if defined(DEMO_OBJECT)
	extern s_single_app_t single_object;
#endif /* DEMO_OBJECT */
#if defined(DEMO_SETUPWIZARD)
	extern s_single_app_t single_setupwizard;
#endif /* DEMO_SETUPWIZARD */
#if defined(DEMO_ONSCREENKEYBOARD)
	extern s_single_app_t single_onscreenkeyboard;
#endif /* DEMO_ONSCREENKEYBOARD */
#if defined(DEMO_TEMP)
	extern s_single_app_t single_temp;
#endif /* DEMO_TEMP */
#if defined(DEMO_TERM)
	extern s_single_app_t single_term;
#endif /* DEMO_TERM */
#if defined(DEMO_TIMER)
	extern s_single_app_t single_timer;
#endif /* DEMO_TIMER */
#if defined(DEMO_LOGOUT)
	extern s_single_app_t single_xynthlogout;
#endif /* DEMO_LOGOUT */
#if defined(DEMO_WIDGETX)
	extern s_single_app_t single_widgetx;
#endif /* DEMO_WIDGETX */

s_single_app_t *s_server_single_apps[] = {
#if defined(DEMO_DESKTOP)
	&single_desktop,
#endif /* DEMO_DESKTOP */
#if defined(DEMO_CHILD)
	&single_child,
#endif /* DEMO_CHILD */
#if defined(DEMO_HIDE)
	&single_hide,
#endif /* DEMO_HIDE */
#if defined(DEMO_LOAD)
	&single_load,
#endif /* DEMO_LOAD */
#if defined(DEMO_SIMPLE)
	&single_simple,
#endif /* DEMO_SIMPLE */
#if defined(DEMO_OBJECT)
	&single_object,
#endif /* DEMO_OBJECT */
#if defined(DEMO_SETUPWIZARD)
	&single_setupwizard,
#endif /* DEMO_SETUPWIZARD */
#if defined(DEMO_ONSCREENKEYBOARD)
	&single_onscreenkeyboard,
#endif /* DEMO_ONSCREENKEYBOARD */
#if defined(DEMO_TEMP)
	&single_temp,
#endif /* DEMO_TEMP */
#if defined(DEMO_TERM)
	&single_term,
#endif /* DEMO_TERM */
#if defined(DEMO_TIMER)
	&single_timer,
#endif /* DEMO_TIMER */
#if defined(DEMO_LOGOUT)
	&single_xynthlogout,
#endif /* DEMO_LOGOUT */
#if defined(DEMO_WIDGETX)
	&single_widgetx,
#endif /* DEMO_WIDGETX */
	NULL
};

void * s_server_single_app_start (void *arg)
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
#if defined(DEMO_DESKTOP)
	for (sa = s_server_single_apps; *sa; sa++) {
		if (strcmp((*sa)->argv[0], "desktop") == 0) {
			s_thread_create(s_server_single_app_start, *sa);
		}
	}
#else
	for (sa = s_server_single_apps; *sa; sa++) {
		if (strcmp((*sa)->argv[0], "xynthlogout") != 0) {
			(*sa)->tid = s_thread_create(s_server_single_app_start, *sa);
		}
	}
#endif
	return;
}

void s_server_single_stop (void)
{
	s_single_app_t **sa;
	for (sa = s_server_single_apps; *sa; sa++) {
		if ((*sa)->tid) {
			s_free((*sa)->tid);
		}
	}
}

#endif /* SINGLE_APP */
