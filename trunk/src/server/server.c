/***************************************************************************
    begin                : Wed Jan 29 2003
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

#if defined(CONFIG_PLATFORM_LINUX)
	#include <fcntl.h>
#endif
#if !defined(CONFIG_SINGLE_APPLICATION)
	#include <sys/shm.h>
#endif

#if defined(CONFIG_VIDEO_FBDEV)
	extern s_video_driver_t s_video_fbdev;
#endif
#if defined(CONFIG_VIDEO_VESA)
	extern s_video_driver_t s_video_vesa;
#endif
#if defined(CONFIG_VIDEO_SVGALIB)
	extern s_video_driver_t s_video_svga;
#endif
#if defined(CONFIG_VIDEO_SDL)
	extern s_video_driver_t s_video_sdl;
#endif
#if defined(CONFIG_VIDEO_GDI)
	extern s_video_driver_t s_video_gdi;
#endif
#if defined(CONFIG_VIDEO_PSPDEV)
	extern s_video_driver_t s_video_pspdev;
#endif
#if defined(CONFIG_VIDEO_GP2X)
	extern s_video_driver_t s_video_gp2x;
#endif
#if defined(CONFIG_VIDEO_CACA)
	extern s_video_driver_t s_video_caca;
#endif
#if defined(CONFIG_VIDEO_NULL)
	extern s_video_driver_t s_video_null;
#endif

static s_video_driver_t *video_drivers[] = {
#if defined(CONFIG_VIDEO_FBDEV)
	&s_video_fbdev,
#endif
#if defined(CONFIG_VIDEO_VESA)
	&s_video_vesa,
#endif
#if defined(CONFIG_VIDEO_SVGALIB)
	&s_video_svga,
#endif
#if defined(CONFIG_VIDEO_SDL)
	&s_video_sdl,
#endif
#if defined(CONFIG_VIDEO_GDI)
	&s_video_gdi,
#endif
#if defined(CONFIG_VIDEO_PSPDEV)
	&s_video_pspdev,
#endif
#if defined(CONFIG_VIDEO_GP2X)
	&s_video_gp2x,
#endif
#if defined(CONFIG_VIDEO_CACA)
	&s_video_caca,
#endif
#if defined(CONFIG_VIDEO_NULL)
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
#if defined(CONFIG_PLATFORM_PSPDEV)
	config->general.driver = strdup("pspdev");
	config->general.mode = strdup("480x272x16M32");
	config->mouse.type = strdup("MOUSE_PSPDEV");
	config->mouse.device = strdup("/dev/null");
#elif defined(CONFIG_PLATFORM_GP2X)
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
	if (s_config_parse(cfg, CONFIG_PATH_INSTALL CONFIG_PATH_CONFIGS "/xynth.conf")) {
		debugf(DSER, "Configuration file parsing failed (%s)", CONFIG_PATH_INSTALL CONFIG_PATH_CONFIGS "/xynth.conf");
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
				} else if (strcasecmp(var->name, "rotate") == 0) {
					config->general.rotate = atoi(var->value);
				} else if (strcasecmp(var->name, "console") == 0) {
					config->general.console = strdup(var->value);
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
			} else if (strcasecmp(cat->name, "keyboard") == 0) {
				if (strcasecmp(var->name, "keyboard") == 0) {
					config->keyboard.keyboard = strdup(var->value);
				}
			} else if (strcasecmp(cat->name, "infrared") == 0) {
				if (strcasecmp(var->name, "type") == 0) {
					config->irr.type = strdup(var->value);
				} else if (strcasecmp(var->name, "timestamp") == 0) {
					config->irr.timestamp = atoi(var->value);
				} else if (strcasecmp(var->name, "device") == 0) {
					config->irr.device = strdup(var->value);
				} else if (strcasecmp(var->name, "brate") == 0) {
					config->irr.brate = strdup(var->value);
				} else if (strncmp(var->name, "KEYBOARD_BUTTON_", 10) == 0) {
					s_server_irr_add_code(var->name, var->value);
				}
			} else if (strcasecmp(cat->name, "theme") == 0) {
				if (strcasecmp(var->name, "name") == 0) {
					config->theme.name = strdup(var->value);
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
				} else if (strcasecmp(var->name, "timing") == 0) {
					nptr = strdup(var->value);
					nptr_ = nptr;
					ptr = s_server_cfg_token(&nptr);
					if (s_server_cfg_check_digit(ptr, digits)) {
						config->monitor.pixclock = atoi(ptr);
					} else {
						goto tm_bad;
					}
					ptr = s_server_cfg_token(&nptr);
					if (s_server_cfg_check_digit(ptr, digits)) {
						config->monitor.left_margin = atoi(ptr);
					} else {
						goto tm_bad;
					}
					ptr = s_server_cfg_token(&nptr);
					if (s_server_cfg_check_digit(ptr, digits)) {
						config->monitor.right_margin = atoi(ptr);
					} else {
						goto tm_bad;
					}
					ptr = s_server_cfg_token(&nptr);
					if (s_server_cfg_check_digit(ptr, digits)) {
						config->monitor.upper_margin = atoi(ptr);
					} else {
						goto tm_bad;
					}
					ptr = s_server_cfg_token(&nptr);
					if (s_server_cfg_check_digit(ptr, digits)) {
						config->monitor.lower_margin = atoi(ptr);
					} else {
						goto tm_bad;
					}
					ptr = s_server_cfg_token(&nptr);
					if (s_server_cfg_check_digit(ptr, digits)) {
						config->monitor.hsync_len = atoi(ptr);
					} else {
						goto tm_bad;
					}
					ptr = s_server_cfg_token(&nptr);
					if (s_server_cfg_check_digit(ptr, digits)) {
						config->monitor.vsync_len = atoi(ptr);
					} else {
						goto tm_bad;
					}
					ptr = s_server_cfg_token(&nptr);
					if (s_server_cfg_check_digit(ptr, digits)) {
						config->monitor.vmode = atoi(ptr);
					} else {
tm_bad:						debugf(DSER, "Illegal timing setting. Correct usage: vertrefresh pixclock left_margin right_margin upper_margin lower_margin hsync_len vsync_len vmode\n");
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
				xynth_server->driver = *vd;
			}
		}
		if (xynth_server->driver == NULL) {
			debugf(DSER, "Unknown video driver : %s", config->general.driver);
			ret = -1;
		}
	}

	return ret;
}

void * s_server_loop (void *arg)
{
	while (xynth_server->window->running > 0) {
		if (s_socket_listen_wait(xynth_server->window, -1)) {
			xynth_server->window->running = 0;
			break;
		}
	}
	return NULL;
}

int s_server_init (void)
{
        int i;
        int mode;
	void *addr;
	s_server_conf_t config;

#if defined(CONFIG_PLATFORM_LINUX)
	int fd[2];
	setvbuf(stdout, (char *) NULL, _IONBF, 0);
	setvbuf(stderr, (char *) NULL, _IONBF, 0);
	fd[0] = fcntl(fileno(stdout), F_DUPFD, 0);
	fd[1] = fcntl(fileno(stderr), F_DUPFD, 0);
#elif defined(CONFIG_PLATFORM_GP2X)
	setvbuf(stdout, (char *) NULL, _IONBF, 0);
	setvbuf(stderr, (char *) NULL, _IONBF, 0);
#endif

	xynth_server = (s_server_t *) s_calloc(1, sizeof(s_server_t));
	for (i = 0; i < S_CLIENTS_MAX; i++) {
		xynth_server->id[i] = -1;
		xynth_server->pri[i] = -1;
		xynth_server->client[i].pid = -1;
		xynth_server->client[i].soc = -1;
		xynth_server->client[i].type = 0;
		xynth_server->client[i].resizeable = 1;
		xynth_server->client[i].alwaysontop = 0;
		xynth_server->client[i].cursor = CURSOR_TYPE_ARROW;
	}
	
        xynth_server->window = (s_window_t *) s_calloc(1, sizeof(s_window_t));
        xynth_server->window->surface = (s_surface_t *) s_calloc(1, sizeof(s_surface_t));
	xynth_server->window->surface->buf = (s_rect_t *) s_calloc(1, sizeof(s_rect_t));
	xynth_server->window->surface->win = (s_rect_t *) s_calloc(1, sizeof(s_rect_t));

	memset(&config, 0, sizeof(s_server_conf_t));
        if (s_server_cfg(&config)) {
		debugf(DSER, "Error while loading configuration file");
		goto err0;
	}

        xynth_server->window->surface->buf->x = 0;
        xynth_server->window->surface->buf->y = 0;
	xynth_server->window->surface->buf->w = 0;
	xynth_server->window->surface->buf->h = 0;
	xynth_server->window->surface->linear_buf_width = 0;
	xynth_server->window->surface->linear_buf_pitch = 0;
	xynth_server->window->surface->linear_buf_height = 0;
	
        if (xynth_server->driver->server_init != NULL) {
		mode = xynth_server->driver->server_init(&config);
		if (mode < 0) {
			debugf(DSER, "xynth_server->driver->server_init(&config) failed");
			goto err0;
		}
	} else {
		debugf(DSER, "xynth_server->driver->server_init == NULL");
		goto err0;
	}

	xynth_server->window->surface->bluemask = ((1 << xynth_server->window->surface->bluelength) - 1) << xynth_server->window->surface->blueoffset;
	xynth_server->window->surface->greenmask = ((1 << xynth_server->window->surface->greenlength) - 1) << xynth_server->window->surface->greenoffset;
	xynth_server->window->surface->redmask = ((1 << xynth_server->window->surface->redlength) - 1) << xynth_server->window->surface->redoffset;

	if (config.general.rotate) {
		xynth_server->rotate = config.general.rotate;
		xynth_server->origin_w = xynth_server->window->surface->width;
		xynth_server->origin_h = xynth_server->window->surface->height;
		xynth_server->origin_vbuf = xynth_server->window->surface->linear_buf;
		if (xynth_server->rotate == 90 ||
		    xynth_server->rotate == -90 ||
		    xynth_server->rotate == 270 ||
		    xynth_server->rotate == -270) {
			xynth_server->window->surface->width = xynth_server->origin_h;
			xynth_server->window->surface->height = xynth_server->origin_w;
		}
		xynth_server->window->surface->need_expose = SURFACE_NEEDEXPOSE;
#if defined(CONFIG_SINGLE_APPLICATION)
		addr = (void *) s_malloc(sizeof(char) * xynth_server->window->surface->width * xynth_server->window->surface->height * xynth_server->window->surface->bytesperpixel);
#else
		if ((xynth_server->rotate_shm_id = shmget(IPC_PRIVATE, sizeof(char) * xynth_server->window->surface->width * xynth_server->window->surface->height * xynth_server->window->surface->bytesperpixel, IPC_CREAT | 0644)) < 0) {
			debugf(DSER | DSYS, "Can not get id for shared memory");
			goto err0;
		}
		if ((addr = (void *) shmat(xynth_server->rotate_shm_id, NULL, 0)) < 0) {
			debugf(DSER | DSYS, "Can not attach the shared memory");
			goto err0;
		}
		xynth_server->window->surface->shm_sid = xynth_server->rotate_shm_id;
                shmctl(xynth_server->rotate_shm_id, IPC_RMID, 0);
#endif
		xynth_server->window->surface->vbuf = addr;
		xynth_server->window->surface->linear_buf = addr;
	} else {
		xynth_server->rotate = 0;
	}

        xynth_server->window->surface->buf->x = 0;
        xynth_server->window->surface->buf->y = 0;
	xynth_server->window->surface->buf->w = xynth_server->window->surface->width;
	xynth_server->window->surface->buf->h = xynth_server->window->surface->height;
	xynth_server->window->surface->linear_buf_width = xynth_server->window->surface->width;
	if (xynth_server->window->surface->linear_buf_pitch == 0) {
		xynth_server->window->surface->linear_buf_pitch = xynth_server->window->surface->width;
	}
	xynth_server->window->surface->linear_buf_height = xynth_server->window->surface->height;

#if defined(CONFIG_PLATFORM_LINUX)
	close(fileno(stdout));
	fcntl(fd[0], F_DUPFD, 0);
	close(fileno(stderr));
	fcntl(fd[1], F_DUPFD, 0);
#endif

#if defined(CONFIG_SINGLE_APPLICATION)
	addr = (void *) s_malloc(sizeof(char) * xynth_server->window->surface->width * xynth_server->window->surface->height);
#else
        if ((xynth_server->window->surface->shm_mid = shmget(IPC_PRIVATE, sizeof(char) * xynth_server->window->surface->width * xynth_server->window->surface->height, IPC_CREAT | 0644)) < 0) {
		debugf(DSER | DSYS | DFAT, "Can not get id for shared memory");
        }
        if ((addr = (void *) shmat(xynth_server->window->surface->shm_mid, NULL, 0)) < 0) {
		debugf(DSER | DSYS | DFAT, "Can not attach the shared memory");
        }
	shmctl(xynth_server->window->surface->shm_mid, IPC_RMID, 0);
#endif
        xynth_server->window->surface->matrix = (unsigned char *) addr;

	xynth_server->window->surface->mode = SURFACE_REAL;
	xynth_server->mode = mode;

        s_event_init(&xynth_server->window->event);
	s_handlers_init(xynth_server->window);
	s_pollfds_init(xynth_server->window);
	s_timers_init(xynth_server->window);
	s_server_socket_init();
	
	{
		s_video_input_t **input;
		for (input = xynth_server->driver->input; *input; input++) {
			switch ((*input)->type) {
				case VIDEO_INPUT_MOUSE:
					s_server_mouse_init(&config, *input);
					break;
				case VIDEO_INPUT_KEYBD:
					s_server_kbd_init(&config, *input);
					break;
				case VIDEO_INPUT_IRR:
					s_server_irr_init(&config, *input);
					break;
				default:
					break;
			}
		}
	}
	 
	for (i = 0; i < 20; i++) {
		s_handler_init(&(xynth_server->whndl[i]));
	}

	s_server_theme_init(config.theme.name);

	s_free(config.general.console);
	s_free(config.general.driver);
	s_free(config.general.mode);
	s_free(config.mouse.type);
	s_free(config.mouse.device);
	s_free(config.keyboard.keyboard);
	s_free(config.irr.type);
	s_free(config.irr.device);
	s_free(config.irr.brate);

	xynth_server->window->running = 1;

	return 0;

err0:	s_free(config.general.console);
	s_free(config.general.driver);
	s_free(config.general.mode);
	s_free(config.mouse.type);
	s_free(config.mouse.device);
	s_free(config.keyboard.keyboard);
	s_free(xynth_server->window->surface->buf);
	s_free(xynth_server->window->surface->win);
	s_free(xynth_server->window->surface);
	s_free(xynth_server->window);
	return 1;
}

void s_server_quit (s_window_t *window)
{
	window->running = 0;
	s_window_wakeup(window);
}

void s_server_uninit (void)
{
        int i;
        int v;
        
	s_server_window_handlers_del_mouse();
	for (i = 0; i < 20; i++) {
		s_handler_uninit(xynth_server->whndl[i]);
	}
	
	s_timers_uninit(xynth_server->window);
	s_pollfds_uninit(xynth_server->window);
	s_handlers_uninit(xynth_server->window);
	s_server_theme_uninit();
	for (i = 0; i < S_CLIENTS_MAX; i++) {
		s_free(xynth_server->client[i].title.str);
		for (v = 0; v < 2; v++) {
			s_free(xynth_server->client[i].title.img[v].mat);
			s_free(xynth_server->client[i].title.img[v].buf);
		}
	}
        s_event_uninit(xynth_server->window->event);

        if (xynth_server->driver->server_uninit != NULL) {
		xynth_server->driver->server_uninit();
	}

#if defined(CONFIG_SINGLE_APPLICATION)
	s_free(xynth_server->window->surface->matrix);
#else
	shmdt(xynth_server->window->surface->matrix);
#endif
	s_free(xynth_server->window->surface->buf);
	s_free(xynth_server->window->surface->win);
	s_free(xynth_server->window->surface);
	s_free(xynth_server->window);
//	s_free(server);
	s_socket_api_uninit();
}

void s_server_goto_back (void)
{
        if (xynth_server->driver->server_goto_back != NULL) {
		xynth_server->driver->server_goto_back();
	}
}

void s_server_comefrom_back (void)
{
        if (xynth_server->driver->server_comefrom_back != NULL) {
		xynth_server->driver->server_comefrom_back();
	}
}

void s_server_restore (void)
{
        if (xynth_server->driver->server_restore != NULL) {
		xynth_server->driver->server_restore();
	}
}

void s_server_fullscreen (void)
{
	if (xynth_server->driver->server_fullscreen != NULL) {
		xynth_server->driver->server_fullscreen();
        }
}

void s_server_surface_update (s_rect_t *coor)
{
	if (xynth_server->rotate) {
		char *src;
		char *dst;
		s_rect_t clip;
		s_rect_t inter;
		s_rect_t rotate;
		s_surface_t *surface;

		clip.x = 0;
		clip.y = 0;
		clip.w = xynth_server->window->surface->width;
		clip.h = xynth_server->window->surface->height;
		if (s_region_rect_intersect(&clip, coor, &inter)) {
			return;
		}

		src = s_malloc(inter.w * inter.h * xynth_server->window->surface->bytesperpixel);
		dst = s_malloc(inter.w * inter.h * xynth_server->window->surface->bytesperpixel);
		if (s_surface_create_from_data(&surface, xynth_server->origin_w, xynth_server->origin_h, xynth_server->window->surface->bitsperpixel, xynth_server->origin_vbuf)) {
			s_free(src);
			s_free(dst);
			return;
		}

		s_getbox(xynth_server->window->surface, inter.x, inter.y, inter.w, inter.h, src);
		s_rotatebox(surface, &inter, src, &rotate, dst, xynth_server->rotate);
		s_putbox(surface, rotate.x, rotate.y, rotate.w, rotate.h, dst);
		if (xynth_server->driver->server_surface_update != NULL) {
			xynth_server->driver->server_surface_update(&rotate);
		}

		s_free(src);
		s_free(dst);
		s_surface_destroy(surface);
	} else {
		s_rect_t clip;
		if (xynth_server->driver->server_surface_update != NULL) {
			if (s_region_rect_intersect(xynth_server->window->surface->buf, coor, &clip)) {
				return;
			}
			xynth_server->driver->server_surface_update(&clip);
		}
	}
}
