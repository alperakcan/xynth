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

#if defined(CONFIG_THEME_PLUGIN)
	#include <dlfcn.h>
#else
	#include "theme_static.h"
#endif

void s_server_theme_set (char *name)
{
	s_theme_form_t f;
	s_theme_state_t v;
	s_theme_button_t b;

#if defined(CONFIG_THEME_PLUGIN)
	char *tfile;
	char *error;
	void *handle;
	void (*theme_init) (s_theme_t *theme);

	tfile = (char *) s_malloc(strlen(CONFIG_PATH_INSTALL) + strlen(CONFIG_PATH_THEMES) + strlen(name) + 10);
	sprintf(tfile, "%s/%s/%s.so", CONFIG_PATH_INSTALL, CONFIG_PATH_THEMES, name);
	handle = dlopen(tfile, RTLD_LAZY);
	if (!handle) {
		debugf(DSER | DFAT, "%s", dlerror());
	}

	theme_init = dlsym(handle, "theme_init");
	if ((error = dlerror()) != NULL)  {
		debugf(DSER | DFAT, "%s", error);
	}
	free(tfile);
#else
#endif
	s_server_theme_uninit();

	memset(&(xynth_server->theme), 0, sizeof(s_theme_t));
	theme_init(&xynth_server->theme);

        for (b = 0; b < THEME_BUTTON_MAX; b++) {
		for (v = 0; v < 3; v++) {
			s_image_get_buf(xynth_server->window->surface, &xynth_server->theme.button[v][b]);
			s_image_get_mat(&xynth_server->theme.button[v][b]);
			s_image_get_handler(&xynth_server->theme.button[v][b]);
			s_image_free_rgba(&xynth_server->theme.button[v][b]);
		}
	}

	for (f = 0; f < THEME_FORM_MAX; f++) {
		for (v = 0; v < 2; v++) {
			s_image_get_buf(xynth_server->window->surface, &xynth_server->theme.form[v][f]);
			s_image_get_mat(&xynth_server->theme.form[v][f]);
			s_image_get_handler(&xynth_server->theme.form[v][f]);
			s_image_free_rgba(&xynth_server->theme.form[v][f]);
		}
	}

	s_font_init(&(xynth_server->theme.font[0]), "arial.ttf");
	s_font_set_size(xynth_server->theme.font[0], xynth_server->theme.form[0][THEME_FORM_TOP_3].h - 8);
	s_font_init(&(xynth_server->theme.font[1]), "arial.ttf");
	s_font_set_size(xynth_server->theme.font[1], xynth_server->theme.form[1][THEME_FORM_TOP_3].h - 8);

#if defined(CONFIG_THEME_PLUGIN)
	dlclose(handle);
#endif

	s_server_surface_refresh();
}

void s_server_theme_init (char *name)
{
	s_server_theme_set((name == NULL) ? "silverado" : name);
}

void s_server_theme_uninit (void)
{
	int f;
	int b;
	int v;

	s_font_uninit(xynth_server->theme.font[0]);
	s_font_uninit(xynth_server->theme.font[1]);

	for (f = 0; f < THEME_FORM_MAX; f++) {
		for (v = 0; v < 2; v++) {
			s_image_free_buf(&xynth_server->theme.form[v][f]);
			s_image_free_mat(&xynth_server->theme.form[v][f]);
			s_image_free_rgba(&xynth_server->theme.form[v][f]);
			s_image_free_handler(&xynth_server->theme.form[v][f]);
			s_image_layers_uninit(&xynth_server->theme.form[v][f]);
		}
	}
        for (b = 0; b < THEME_BUTTON_MAX; b++) {
		for (v = 0; v < 3; v++) {
			s_image_free_buf(&xynth_server->theme.button[v][b]);
			s_image_free_mat(&xynth_server->theme.button[v][b]);
			s_image_free_rgba(&xynth_server->theme.button[v][b]);
			s_image_free_handler(&xynth_server->theme.button[v][b]);
			s_image_layers_uninit(&xynth_server->theme.button[v][b]);
		}
	}
}
