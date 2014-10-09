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
#include "video/helper/helper.h"

typedef enum {
	IRR_NONE,
	IRR_IRMAN,
	IRR_AIR,
} IRR_TYPE;

static IRR_TYPE irr_type = IRR_NONE;

typedef struct s_video_helper_irr_codes_s {
	char *code;
	s_keyboard_button_t key;
	unsigned int repeat;
} s_video_helper_irr_codes_t;

static int irr_code = -1;
static int irr_timestamp = 200;
static unsigned long long irr_time = 0;
static unsigned int irr_codes_size = 0;
static s_video_helper_irr_codes_t *irr_codes = NULL;

int s_video_helper_irr_init (s_server_conf_t *cfg)
{
	if (strcasecmp(cfg->irr.type, "IRR_IRMAN") == 0) {
		#if defined(CONFIG_VIDEO_HELPER_IRMAN)
		irr_type = IRR_IRMAN;
		return s_video_helper_irman_init(cfg->irr.device);
		#endif
	} else if (strcasecmp(cfg->irr.type, "IRR_AIR") == 0) {
		#if defined(CONFIG_VIDEO_HELPER_IRAIR)
		irr_type = IRR_AIR;
		return s_video_helper_irair_init();
		#endif
	}
	return -1;
}

void s_video_helper_irr_uninit (void)
{
	if (irr_type == IRR_IRMAN) {
		#if defined(CONFIG_VIDEO_HELPER_IRMAN)
		s_video_helper_irman_uninit();
		#endif
	} else if (irr_type == IRR_AIR) {
		#if defined(CONFIG_VIDEO_HELPER_IRAIR)
		s_video_helper_irair_uninit();
		#endif
	}
}

int s_video_helper_irr_update (s_video_input_data_t *keybd)
{
	if (irr_type == IRR_IRMAN) {
		#if defined(CONFIG_VIDEO_HELPER_IRMAN)
		char *code;
		s_video_helper_irr_codes_t *codes;
		code = s_video_helper_irman_getcode();
		if (code) {
			for (i = 0; i < irr_codes_size; i++) {
				codes = &irr_codes[i];
				if (strcmp(codes->code, code) == 0) {
					keybd->keybd.state = EVENT_TYPE_KEYBOARD_PRESSED;
					keybd->keybd.button = codes->key;
					keybd->keybd.keycode = codes->key;
					return i;
				}
			}
		}
		#endif
	} else if (irr_type == IRR_AIR) {
		#if defined(CONFIG_VIDEO_HELPER_IRAIR)
		int i;
		char *code;
		s_video_helper_irr_codes_t *codes;
		code = s_video_helper_irair_getcode();
		if (code) {
			for (i = 0; i < irr_codes_size; i++) {
				codes = &irr_codes[i];
				if (strcmp(codes->code, code) == 0) {
					keybd->keybd.state = EVENT_TYPE_KEYBOARD_PRESSED;
					keybd->keybd.button = codes->key;
					keybd->keybd.keycode = codes->key;
					return i;
				}
			}
		}
		#endif
	}
	return -1;
}

static int s_server_irr_code_tokenize (char *value, char token, int *n, char ***tokens)
{
	char *tmp;
	char **tok;
	unsigned int nt;
	unsigned int count;
	for (count = 0, tmp = value; *tmp != 0; tmp++) {
		if (*tmp == token) {
			count++;
		}
	}
	tok = (char **) s_malloc(sizeof(char*) * (count + 1));
	if (tok == NULL) {
		*n = 0;
		return -1;
	}
	nt = 0;
	tmp = value;
	tok[nt] = tmp;
	for (; *tmp != 0; tmp++) {
		if (*tmp == token) {
			*tmp++ = '\0';
			if (*tmp == 0) {
				break;
			} else {
				tok[++nt] = tmp;
			}
		}
	}
	*n = nt + 1;
	*tokens = tok;
	return 0;
}

int s_server_irr_add_code (char *key, char *code)
{
	int ntokens;
	char **tokens;
	unsigned int size;
	s_video_helper_irr_codes_t *codes;
	if (s_server_irr_code_tokenize(code, '|', &ntokens, &tokens)) {
		return -1;
	}
	if (ntokens != 2) {
		return -1;
	}
	size = irr_codes_size + 1;
	codes = (s_video_helper_irr_codes_t *) s_malloc(sizeof(s_video_helper_irr_codes_t) * size);
	if (size > 1) {
		memcpy(codes, irr_codes, sizeof(s_video_helper_irr_codes_t) * (size - 1));
	}
	codes[size - 1].repeat = atoi(tokens[0]);
	codes[size - 1].code = strdup(tokens[1]);
	codes[size - 1].key = s_server_keyname_to_keycode(key);
	s_free(tokens);
	s_free(irr_codes);
	irr_codes = codes;
	irr_codes_size = size;
	return 0;
}

int s_server_irr_uninit (s_window_t *window, s_pollfd_t *pfd)
{
	s_video_input_t *irr;
	irr = (s_video_input_t *) pfd->data;
        if (irr->uninit != NULL) {
		irr->uninit();
	}
	s_free(irr_codes);
	irr_codes_size = 0;
	return 0;
}

int s_server_irr_update (s_window_t *window, s_pollfd_t *pfd)
{
	int code = -1;
	s_video_input_t *irr;
	unsigned long long time;
	unsigned long long difftime;
	s_video_input_data_t idata;
	irr = (s_video_input_t *) pfd->data;
	xynth_server->window->event->type = 0;
	memset(&idata, 0, sizeof(s_video_input_data_t));
	if (irr->update != NULL) {
		code = irr->update(&idata);
	}
	if (code < 0) {
		/* return if no code received */
		irr_code = code;
		return 0;
	}
	time = s_gettimeofday();
	difftime = time - irr_time;
	irr_time = time;
	if (difftime <= irr_timestamp) {
		/* continues press */
		if (irr_code == code && irr_codes[code].repeat == 0) {
			/* return if continuos press is not allowed for the code */
		    	return 0;
		}
	} else {
		/* new key */
	}
	irr_code = code;
	irr_time = time; 
	s_server_event_parse_keyboard(&(idata.irr));
	xynth_server->window->event->type |= EVENT_TYPE_KEYBOARD_EXTENSION_IRR;
	s_server_event_changed();
	xynth_server->window->event->type = 0;
	idata.keybd.state = EVENT_TYPE_KEYBOARD_RELEASED;
	s_server_event_parse_keyboard(&(idata.irr));
	xynth_server->window->event->type |= EVENT_TYPE_KEYBOARD_EXTENSION_IRR;
	s_server_event_changed();
	return 0;
}

void s_server_irr_init (s_server_conf_t *cfg, s_video_input_t *irr)
{
	int fd = -1;
	s_pollfd_t *pfd;
        if (irr->init != NULL) {
		if (strcasecmp(cfg->irr.type, "IRR_NONE") != 0) {
			fd = irr->init(cfg);
			if (fd < 0) {
				debugf(DSER, "irr->init(cfg) failed");
			}
		}
	}
	if (fd < 0) {
		debugf(DSER, "irr support disabled");
	} else {
		irr_timestamp = cfg->irr.timestamp;
		s_pollfd_init(&pfd);
		pfd->fd = fd;
		pfd->pf_in = s_server_irr_update;
		pfd->pf_close = s_server_irr_uninit;
		pfd->data = irr;
		s_pollfd_add(xynth_server->window, pfd);
	}
}
