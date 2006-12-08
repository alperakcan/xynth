
#include "../lib/xynth_.h"
#include "server.h"
#include "video/helper/helper.h"

typedef enum {
	IRR_NONE,
	IRR_IRMAN,
} IRR_TYPE;

static IRR_TYPE irr_type = IRR_NONE;

typedef struct s_video_helper_irr_codes_s {
	char *code;
	S_KEYCODE_CODE key;
} s_video_helper_irr_codes_t;

static unsigned int irr_codes_size = 0;
static s_video_helper_irr_codes_t *irr_codes = NULL;

int s_video_helper_irr_init (s_server_conf_t *cfg)
{
	if (strcasecmp(cfg->irr.type, "IRR_IRMAN") == 0) {
		#if defined(VIDEO_HELPER_IRMAN)
		irr_type = IRR_IRMAN;
		return s_video_helper_irman_init(cfg->irr.device);
		#endif
	}
	return -1;
}

void s_video_helper_irr_uninit (void)
{
	if (irr_type == IRR_IRMAN) {
		#if defined(VIDEO_HELPER_IRMAN)
		s_video_helper_irman_uninit();
		#endif
	}
}

int s_video_helper_irr_update (s_video_input_data_t *keybd)
{
	if (irr_type == IRR_IRMAN) {
		#if defined(VIDEO_HELPER_IRMAN)
		char *code;
		s_video_helper_irr_codes_t *codes;
		code = s_video_helper_irman_getcode();
		if (code) {
			for (i = 0; i < irr_codes_size; i++) {
				codes = &irr_codes[i];
				if (strcmp(codes->code, code) == 0) {
					keybd->keybd.state = KEYBD_PRESSED;
					keybd->keybd.button = codes->key;
					keybd->keybd.keycode = codes->key;
					return 1;
				}
			}
		}
		#endif
	}
	return 0;
}

int s_server_irr_add_code (char *key, char *code)
{
	unsigned int size;
	s_video_helper_irr_codes_t *codes;
	size = irr_codes_size + 1;
	codes = (s_video_helper_irr_codes_t *) s_malloc(sizeof(s_video_helper_irr_codes_t) * size);
	if (size > 1) {
		memcpy(codes, irr_codes, sizeof(s_video_helper_irr_codes_t) * (size - 1));
	}
	codes[size - 1].code = strdup(code);
	codes[size - 1].key = s_server_keyname_to_keycode(key);
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
	s_free(irr_codes_size);
	return 0;
}

int s_server_irr_update (s_window_t *window, s_pollfd_t *pfd)
{
	int force_release = 0;
	s_video_input_t *irr;
	s_video_input_data_t idata;
	irr = (s_video_input_t *) pfd->data;
	server->window->event->type = 0;
	memset(&idata, 0, sizeof(s_video_input_data_t));
	if (irr->update != NULL) {
		force_release = irr->update(&idata);
		s_server_event_parse_keyboard(&(idata.irr));
	}
	s_server_event_changed();
	if (force_release) {
		server->window->event->type = 0;
		idata.keybd.state = KEYBD_RELEASED;
		s_server_event_parse_keyboard(&(idata.irr));
		s_server_event_changed();
	}
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
		s_pollfd_init(&pfd);
		pfd->fd = fd;
		pfd->pf_in = s_server_irr_update;
		pfd->pf_close = s_server_irr_uninit;
		pfd->data = irr;
		s_pollfd_add(server->window, pfd);
	}
}
