/***************************************************************************
    begin                : Wed Jan 29 2003
    copyright            : (C) 2003 - 2006 by Alper Akcan
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

#include "../lib/xynth_.h"
#include "server.h"

#if defined(PLATFORM_LINUX)
	#include <fcntl.h>
#endif
#if !defined(SINGLE_APP)
	#include <sys/shm.h>
#endif

#if defined(VIDEO_FBDev)
	extern s_video_driver_t s_video_fbdev;
#endif
#if defined(VIDEO_VESA)
	extern s_video_driver_t s_video_vesa;
#endif
#if defined(VIDEO_SVGALib)
	extern s_video_driver_t s_video_svga;
#endif
#if defined(VIDEO_SDL)
	extern s_video_driver_t s_video_sdl;
#endif
#if defined(VIDEO_GDI)
	extern s_video_driver_t s_video_gdi;
#endif
#if defined(VIDEO_PSPDEV)
	extern s_video_driver_t s_video_pspdev;
#endif
#if defined(VIDEO_GP2X)
	extern s_video_driver_t s_video_gp2x;
#endif
#if defined(VIDEO_NULL)
	extern s_video_driver_t s_video_null;
#endif

static s_video_driver_t *video_drivers[] = {
#if defined(VIDEO_FBDev)
	&s_video_fbdev,
#endif
#if defined(VIDEO_VESA)
	&s_video_vesa,
#endif
#if defined(VIDEO_SVGALib)
	&s_video_svga,
#endif
#if defined(VIDEO_SDL)
	&s_video_sdl,
#endif
#if defined(VIDEO_GDI)
	&s_video_gdi,
#endif
#if defined(VIDEO_PSPDEV)
	&s_video_pspdev,
#endif
#if defined(VIDEO_GP2X)
	&s_video_gp2x,
#endif
#if defined(VIDEO_NULL)
	&s_video_null,
#endif
	NULL,
};

int s_server_cfg_check_digit (char *ptr, char *digits)
{
	if (ptr == NULL) {
		return 0;
	}
	return (strlen(ptr) == strspn(ptr, digits));
}

char * s_server_cfg_token (char **ptr)
{
	char *p;

	p = *ptr;
        while(*p == ' ') {
		p++;
	}
	if (*p != '\0' ) {
		char *t = p;
		while ((*t != '\0') && (*t != ' ')) {
			t++;
		}
		if (*t == ' ') {
			*t = '\0';
			t++;
		}
		*ptr = t;
		return p;
	} else {
		*ptr = NULL;
		return NULL;
	}
}

int s_server_cfg (s_server_conf_t *config)
{
	int ret = 0;
#if defined(PLATFORM_PSPDEV)
	config->general.driver = strdup("pspdev");
	config->general.mode = strdup("480x272x16M32");
	config->mouse.type = strdup("MOUSE_PSPDEV");
	config->mouse.device = strdup("/dev/null");
#elif defined(PLATFORM_GP2X)
	config->general.driver = strdup("gp2x");
	config->general.mode = strdup("320x240x64K");
	config->mouse.type = strdup("MOUSE_GP2XDEV");
	config->mouse.device = strdup("/dev/null");
#else
	int i;
	int j;
	s_config_t *cfg;
	s_config_cat_t *cat;
	s_config_var_t *var;

	s_config_init(&cfg);
	if (s_config_parse(cfg, CONFDIR "xynth.conf")) {
		debugf(DSER, "Configuration file parsing failed (%s)", CONFDIR "xynth.conf");
		s_config_uninit(cfg);
		return -1;
	}

	i = 0;
	while (!s_list_eol(cfg->category, i)) {
		cat = s_list_get(cfg->category, i++);
		j = 0;
		while (!s_list_eol(cat->variable, j)) {
			var = s_list_get(cat->variable, j++);
			if (strcasecmp(cat->name, "general") == 0) {
				if (strcasecmp(var->name, "driver") == 0) {
					config->general.driver = strdup(var->value);
				} else if (strcasecmp(var->name, "mode") == 0) {
					config->general.mode = strdup(var->value);
				}
			} else if (strcasecmp(cat->name, "mouse") == 0) {
				if (strcasecmp(var->name, "type") == 0) {
					config->mouse.type = strdup(var->value);
				} else if (strcasecmp(var->name, "device") == 0) {
					config->mouse.device = strdup(var->value);
				} else if (strcasecmp(var->name, "samplerate") == 0) {
					config->mouse.samplerate = atoi(var->value);
				} else if (strcasecmp(var->name, "scale") == 0) {
					config->mouse.scale = atoi(var->value);
					if (config->mouse.scale == 0) {
						config->mouse.scale = 1;
					}
				}
			} else if (strcasecmp(cat->name, "monitor") == 0) {
				char *ptr;
				char *nptr;
				char *nptr_;
				char digits[] = ".0123456789";
				if (strcasecmp(var->name, "horizsync") == 0) {
					nptr = strdup(var->value);
					nptr_ = nptr;
					ptr = s_server_cfg_token(&nptr);
					if (s_server_cfg_check_digit(ptr, digits)) {
						config->monitor.horizsync_min = atof(ptr);
					} else {
						goto hs_bad;
					}
					ptr = s_server_cfg_token(&nptr);
					if (s_server_cfg_check_digit(ptr, digits)) {
						config->monitor.horizsync_max = atof(ptr);
					} else {
hs_bad:						debugf(DSER, "Illegal horizsync setting. Correct usage: horizsync min_kHz max_kHz\n");
						ret = -1;
					}
					s_free(nptr_);
				} else if (strcasecmp(var->name, "vertrefresh") == 0) {
					nptr = strdup(var->value);
					nptr_ = nptr;
					ptr = s_server_cfg_token(&nptr);
					if (s_server_cfg_check_digit(ptr, digits)) {
						config->monitor.vertrefresh_min = atof(ptr);
					} else {
						goto vr_bad;
					}
					ptr = s_server_cfg_token(&nptr);
					if (s_server_cfg_check_digit(ptr, digits)) {
						config->monitor.vertrefresh_max = atof(ptr);
					} else {
vr_bad:						debugf(DSER, "Illegal vertrefresh setting. Correct usage: vertrefresh min_Hz max_Hz\n");
						ret = -1;
					}
					s_free(nptr_);
				}
			}
		}
	}
	s_config_uninit(cfg);
#endif
	{
		s_video_driver_t **vd;
		for (vd = video_drivers; *vd; vd++) {
			if (strcmp((*vd)->driver, config->general.driver) == 0) {
				if ((strlen((*vd)->driver) >= S_FNAME_MAX) ||
				    (strlen((*vd)->device) >= S_FNAME_MAX)) {
					debugf(DSER, "driver:%s, or device:%s name is too long (> %d). skipping", (*vd)->driver, (*vd)->device, S_FNAME_MAX);
					continue;
				}
				server->driver = *vd;
			}
		}
		if (server->driver == NULL) {
			debugf(DSER, "Unknown video driver : %s", config->general.driver);
			ret = -1;
		}
	}

	return ret;
}

int s_server_init (void)
{
        int i;
        int mode;
	void *addr;
	s_server_conf_t config;

#if defined(PLATFORM_LINUX)
	int fd[2];
	setvbuf(stdout, (char *) NULL, _IONBF, 0);
	setvbuf(stderr, (char *) NULL, _IONBF, 0);
	fd[0] = fcntl(fileno(stdout), F_DUPFD, 0);
	fd[1] = fcntl(fileno(stderr), F_DUPFD, 0);
#elif defined(PLATFORM_GP2X)
	setvbuf(stdout, (char *) NULL, _IONBF, 0);
	setvbuf(stderr, (char *) NULL, _IONBF, 0);
#endif

	server = (s_server_t *) s_calloc(1, sizeof(s_server_t));
	for (i = 0; i < S_CLIENTS_MAX; i++) {
		server->id[i] = -1;
		server->pri[i] = -1;
		server->client[i].pid = -1;
		server->client[i].soc = -1;
		server->client[i].type = 0;
		server->client[i].resizeable = 1;
		server->client[i].alwaysontop = 0;
		server->client[i].cursor = MOUSE_CURSOR_ARROW;
	}
	config.general.driver = NULL;
	config.general.mode = NULL;
	config.mouse.type = NULL;
	config.mouse.device = NULL;

        server->window = (s_window_t *) s_calloc(1, sizeof(s_window_t));
        server->window->surface = (s_surface_t *) s_calloc(1, sizeof(s_surface_t));
	server->window->surface->buf = (s_rect_t *) s_calloc(1, sizeof(s_rect_t));
	server->window->surface->win = (s_rect_t *) s_calloc(1, sizeof(s_rect_t));

        if (s_server_cfg(&config)) {
		debugf(DSER, "Error while loading configuration file");
		goto err0;
	}

        server->window->surface->buf->x = 0;
        server->window->surface->buf->y = 0;
	server->window->surface->buf->w = 0;
	server->window->surface->buf->h = 0;
	server->window->surface->linear_buf_width = 0;
	server->window->surface->linear_buf_pitch = 0;
	server->window->surface->linear_buf_height = 0;
	
        if (server->driver->server_init != NULL) {
		mode = server->driver->server_init(&config);
		if (mode < 0) {
			debugf(DSER, "server->driver->server_init(&config) failed");
			goto err0;
		}
	} else {
		debugf(DSER, "server->driver->server_init == NULL");
		goto err0;
	}

        server->window->surface->buf->x = 0;
        server->window->surface->buf->y = 0;
	server->window->surface->buf->w = server->window->surface->width;
	server->window->surface->buf->h = server->window->surface->height;
	server->window->surface->linear_buf_width = server->window->surface->width;
	if (server->window->surface->linear_buf_pitch == 0) {
		server->window->surface->linear_buf_pitch = server->window->surface->width;
	}
	server->window->surface->linear_buf_height = server->window->surface->height;

#if defined(PLATFORM_LINUX)
	close(fileno(stdout));
	fcntl(fd[0], F_DUPFD, 0);
	close(fileno(stderr));
	fcntl(fd[1], F_DUPFD, 0);
#endif

#if defined(SINGLE_APP)
	addr = (void *) s_malloc(sizeof(char) * server->window->surface->width * server->window->surface->height);
#else
        if ((server->window->surface->shm_mid = shmget(IPC_PRIVATE, sizeof(char) * server->window->surface->width * server->window->surface->height, IPC_CREAT | 0644)) < 0) {
		debugf(DSER | DSYS | DFAT, "Can not get id for shared memory");
        }
        if ((addr = (void *) shmat(server->window->surface->shm_mid, NULL, 0)) < 0) {
		debugf(DSER | DSYS | DFAT, "Can not attach the shared memory");
        }
	shmctl(server->window->surface->shm_mid, IPC_RMID, 0);
#endif
        server->window->surface->matrix = (unsigned char *) addr;

	server->window->surface->mode = SURFACE_REAL;
	server->mode = mode;

        s_event_init(&server->window->event);
	s_handlers_init(server->window);
	s_pollfds_init(server->window);
	s_timers_init(server->window);
	s_server_socket_init();
	s_server_mouse_init(&config);
	s_server_kbd_init(&config);

	for (i = 0; i < 20; i++) {
		s_handler_init(&(server->whndl[i]));
	}

	s_free(config.general.driver);
	s_free(config.general.mode);
	s_free(config.mouse.type);
	s_free(config.mouse.device);

	server->window->running = 1;

	return 0;

err0:	s_free(config.general.driver);
	s_free(config.general.mode);
	s_free(config.mouse.type);
	s_free(config.mouse.device);
	s_free(server->window->surface->buf);
	s_free(server->window->surface->win);
	s_free(server->window->surface);
	s_free(server->window);
	return 1;
}

void s_server_quit (s_window_t *window)
{
	s_client_quit(window);
}

void s_server_uninit (void)
{
        int i;
        int v;
        
	s_server_window_handlers_del_mouse();
	for (i = 0; i < 20; i++) {
		s_handler_uninit(server->whndl[i]);
	}
	
	s_timers_uninit(server->window);
	s_pollfds_uninit(server->window);
	s_handlers_uninit(server->window);
	s_server_theme_uninit();
	for (i = 0; i < S_CLIENTS_MAX; i++) {
		s_free(server->client[i].title.str);
		for (v = 0; v < 2; v++) {
			s_free(server->client[i].title.img[v].mat);
			s_free(server->client[i].title.img[v].buf);
		}
	}
        s_event_uninit(server->window->event);

        if (server->driver->server_uninit != NULL) {
		server->driver->server_uninit();
	}

#if defined(SINGLE_APP)
	s_free(server->window->surface->matrix);
#else
	shmdt(server->window->surface->matrix);
#endif
	s_free(server->window->surface->buf);
	s_free(server->window->surface->win);
	s_free(server->window->surface);
	s_free(server->window);
//	s_free(server);
	s_socket_api_uninit();
}

void s_server_goto_back (void)
{
        if (server->driver->server_goto_back != NULL) {
		server->driver->server_goto_back();
	}
}

void s_server_comefrom_back (void)
{
        if (server->driver->server_comefrom_back != NULL) {
		server->driver->server_comefrom_back();
	}
}

void s_server_restore (void)
{
        if (server->driver->server_restore != NULL) {
		server->driver->server_restore();
	}
}

void s_server_fullscreen (void)
{
	if (server->driver->server_fullscreen != NULL) {
		server->driver->server_fullscreen();
        }
}

void s_server_surface_update (s_rect_t *coor)
{
	s_rect_t clip;
	if (server->driver->server_surface_update != NULL) {
		if (s_rect_clip_virtual(server->window->surface, coor->x, coor->y, coor->w, coor->h, &clip)) {
			return;
		}
		server->driver->server_surface_update(&clip);
	}
}
