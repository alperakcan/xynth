/***************************************************************************
    begin                : Thu Sep 1 2005
    copyright            : (C) 2005 - 2009 by Alper Akcan
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

#include "config.h"

#if defined(CONFIG_SINGLE_APPLICATION)

#include "../lib/xynth_.h"
#include "server.h"

#if defined(CONFIG_DEMO_DESKTOP)
	extern s_single_app_t single_desktop;
#endif /* CONFIG_DEMO_DESKTOP */
#if defined(CONFIG_DEMO_CHILD)
	extern s_single_app_t single_child;
#endif /* CONFIG_DEMO_CHILD */
#if defined(CONFIG_DEMO_HIDE)
	extern s_single_app_t single_hide;
#endif /* CONFIG_DEMO_HIDE */
#if defined(CONFIG_DEMO_LOAD)
	extern s_single_app_t single_load;
#endif /* CONFIG_DEMO_LOAD */
#if defined(CONFIG_DEMO_SIMPLE)
	extern s_single_app_t single_simple;
#endif /* CONFIG_DEMO_SIMPLE */
#if defined(CONFIG_DEMO_OBJECT)
	extern s_single_app_t single_object;
#endif /* CONFIG_DEMO_OBJECT */
#if defined(CONFIG_DEMO_SETUPWIZARD)
	extern s_single_app_t single_setupwizard;
#endif /* CONFIG_DEMO_SETUPWIZARD */
#if defined(CONFIG_DEMO_ONSCREENKEYBOARD)
	extern s_single_app_t single_onscreenkeyboard;
#endif /* CONFIG_DEMO_ONSCREENKEYBOARD */
#if defined(CONFIG_DEMO_TEMP)
	extern s_single_app_t single_temp;
#endif /* CONFIG_DEMO_TEMP */
#if defined(CONFIG_DEMO_TERM)
	extern s_single_app_t single_term;
#endif /* CONFIG_DEMO_TERM */
#if defined(CONFIG_DEMO_TIMER)
	extern s_single_app_t single_timer;
#endif /* CONFIG_DEMO_TIMER */
#if defined(CONFIG_DEMO_LOGOUT)
	extern s_single_app_t single_xynthlogout;
#endif /* CONFIG_DEMO_LOGOUT */
#if defined(CONFIG_DEMO_WIDGETX)
	extern s_single_app_t single_widgetx;
#endif /* CONFIG_DEMO_WIDGETX */

s_single_app_t *s_server_single_apps[] = {
#if defined(CONFIG_DEMO_DESKTOP)
	&single_desktop,
#endif /* CONFIG_DEMO_DESKTOP */
#if defined(CONFIG_DEMO_CHILD)
	&single_child,
#endif /* CONFIG_DEMO_CHILD */
#if defined(CONFIG_DEMO_HIDE)
	&single_hide,
#endif /* CONFIG_DEMO_HIDE */
#if defined(CONFIG_DEMO_LOAD)
	&single_load,
#endif /* CONFIG_DEMO_LOAD */
#if defined(CONFIG_DEMO_SIMPLE)
	&single_simple,
#endif /* CONFIG_DEMO_SIMPLE */
#if defined(CONFIG_DEMO_OBJECT)
	&single_object,
#endif /* CONFIG_DEMO_OBJECT */
#if defined(CONFIG_DEMO_SETUPWIZARD)
	&single_setupwizard,
#endif /* CONFIG_DEMO_SETUPWIZARD */
#if defined(CONFIG_DEMO_ONSCREENKEYBOARD)
	&single_onscreenkeyboard,
#endif /* CONFIG_DEMO_ONSCREENKEYBOARD */
#if defined(CONFIG_DEMO_TEMP)
	&single_temp,
#endif /* CONFIG_DEMO_TEMP */
#if defined(CONFIG_DEMO_TERM)
	&single_term,
#endif /* CONFIG_DEMO_TERM */
#if defined(CONFIG_DEMO_TIMER)
	&single_timer,
#endif /* CONFIG_DEMO_TIMER */
#if defined(CONFIG_DEMO_LOGOUT)
	&single_xynthlogout,
#endif /* CONFIG_DEMO_LOGOUT */
#if defined(CONFIG_DEMO_WIDGETX)
	&single_widgetx,
#endif /* CONFIG_DEMO_WIDGETX */
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
#if defined(CONFIG_DEMO_DESKTOP)
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

#endif /* CONFIG_SINGLE_APPLICATION */
