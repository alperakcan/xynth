
#include "../../../lib/xynth_.h"
#include "server.h"
#include "../helper/helper.h"
#include "fbdev.h"

#include <sys/mman.h>

s_video_input_t armfb_input_keypad = {
	VIDEO_INPUT_KEYBD,
	armfb_keypad_init,
	armfb_keypad_update,
	armfb_keypad_uninit,
};

s_video_input_t armfb_input_touch = {
	VIDEO_INPUT_MOUSE,
	armfb_touch_init,
	armfb_touch_update,
	armfb_touch_uninit,
};

s_video_input_t *armfb_input[] = {
	&armfb_input_touch,
	&armfb_input_keypad,
	NULL,
};

s_video_driver_t s_video_artec = {
	"armfb",
	"/dev/fb0",
	armfb_input,
	armfb_server_init,
	armfb_server_uninit,
	armfb_server_goto_back,
	armfb_server_comefrom_back,
	armfb_server_restore,
	NULL,
	NULL,
	NULL
};

void armfb_server_restore (void)
{
	armfb_close();
}

void armfb_server_goto_back (void)
{
	s_server_surface_lock();
	armfb_putvaro();
}

void armfb_server_comefrom_back (void)
{
	armfb_putvar();
	s_server_surface_refresh();
	// s_video_helper_kbd_set_attr();	FIXME
}

void armfb_server_mtrr_init (void)
{
}

void armfb_server_surface_setrgbpalette (void)
{
	int i;
	int r;
	int g;
	int b;

	for (i = 0; i < 256; i++) {
		b = (i & 7) * (64 / 8);		/* 3 bits */
		g = ((i & 56) >> 3) * (64 / 8);	/* 3 bits */
		r = ((i & 192) >> 6) * (64 / 4);/* 2 bits */
		armfb_setpalette(i, r, g, b);
	}
}

void armfb_server_surface_init (s_video_helper_mode_info_t *gmode)
{
	xynth_server->window->surface->width = armfb.v_scr.xres;
	xynth_server->window->surface->height = armfb.v_scr.yres;
	xynth_server->window->surface->bitsperpixel = armfb.bitsperpixel;
	xynth_server->window->surface->bytesperpixel = armfb.bytesperpixel;
	xynth_server->window->surface->blueoffset = armfb.v_scr.blue.offset;
	xynth_server->window->surface->greenoffset = armfb.v_scr.green.offset;
	xynth_server->window->surface->redoffset = armfb.v_scr.red.offset;
	xynth_server->window->surface->bluelength = armfb.v_scr.blue.length;
	xynth_server->window->surface->greenlength = armfb.v_scr.green.length;
	xynth_server->window->surface->redlength = armfb.v_scr.red.length;
	xynth_server->window->surface->colors = armfb.colors;
	
	debugf(DSER, "FBDEV : RGB %d %d %d", xynth_server->window->surface->redoffset,
		xynth_server->window->surface->greenoffset, xynth_server->window->surface->blueoffset);

	xynth_server->window->surface->linear_mem_base = 0;
	xynth_server->window->surface->linear_mem_size = armfb.f_scr.smem_len;

	if (xynth_server->window->surface->linear_mem_size){
		xynth_server->window->surface->linear_mem_base = (unsigned int) mmap((caddr_t) 0, xynth_server->window->surface->linear_mem_size,
						                               PROT_WRITE | PROT_READ,
						                               MAP_SHARED,
						                               armfb.fd,
						                               (off_t) xynth_server->window->surface->linear_mem_base);
		if ((int) xynth_server->window->surface->linear_mem_base == -1) {
			debugf(DSER | DSYS | DFAT, "FBDEV : mmap failed");
		}
	} else {
		debugf(DSER | DFAT, "FBDEV : mmap failed");
	}

	xynth_server->window->surface->vbuf = (unsigned char*) xynth_server->window->surface->linear_mem_base;
	xynth_server->window->surface->linear_buf = xynth_server->window->surface->vbuf;
	xynth_server->window->surface->linear_mem_base = 0;

	armfb_server_surface_setrgbpalette();
}

int armfb_server_init (s_server_conf_t *cfg)
{
	s_video_helper_mode_info_t *gmode;
	
	if (s_video_helper_mode_find(cfg, &gmode)) {
		debugf(DSER, "Mode %s cannot be found", cfg->general.mode);
		goto err0;
	}
        
	armfb_init(cfg);

	armfb_server_setmode(cfg, gmode);
	armfb_server_surface_init(gmode);
	armfb_server_mtrr_init();
	return gmode->number;

err0:	return -1;
}

void armfb_server_surface_uninit (void)
{
	munmap((void *) xynth_server->window->surface->linear_mem_base, xynth_server->window->surface->linear_mem_size);
}

void armfb_server_uninit (void)
{
	armfb_server_surface_uninit();
//	armfb_setmode(0);
}

void armfb_server_setmode (s_server_conf_t *cfg, s_video_helper_mode_info_t *gmode)
{
	armfb.v_scr.xres = gmode->xdim;
	armfb.v_scr.yres = gmode->ydim;
	armfb.v_scr.xres_virtual = gmode->xdim;
	armfb.v_scr.yres_virtual = gmode->ydim;
	armfb.v_scr.xoffset = 0;
	armfb.v_scr.yoffset = 0;
	armfb.v_scr.bits_per_pixel = gmode->bytesperpixel * 8;
	armfb.v_scr.grayscale = 0;
	armfb.v_scr.nonstd = 0;

	switch (gmode->colors)	// FIXME
	{
	case 1<<8:
		armfb.bitsperpixel = 8;
		break;
	case 1<<15:
		armfb.bitsperpixel = 15;
		break;
	case 1<<16:
		armfb.bitsperpixel = 16;
		break;
	case 1<<18:
		armfb.bitsperpixel = 18;
		break;
	case 1<<24:
		armfb.bitsperpixel = 24;
		break;
	}
	
	armfb.bytesperpixel = gmode->bytesperpixel;
	armfb.colors = gmode->colors;

	armfb.v_scr.red.msb_right = 0;
	armfb.v_scr.green.msb_right = 0;
	armfb.v_scr.blue.msb_right = 0;

	// timing adjustment not required
	
#ifdef DEBUG
	void armfb_dump_vscr (struct fb_var_screeninfo *vscr);
	armfb_dump_vscr(&(armfb.v_scr));
#endif

	armfb_putvar();

	switch (armfb.f_scr.type) {
		case FB_TYPE_PACKED_PIXELS:
			break;
		default :
			debugf(DSER | DSYS | DFAT, "FBDEV : Unsupported type(%d) requested", armfb.f_scr.type);
	}

	switch (armfb.f_scr.visual) {
		case FB_VISUAL_TRUECOLOR:
		case FB_VISUAL_DIRECTCOLOR:
			break;
		case FB_VISUAL_PSEUDOCOLOR:
			break;
		default :
			debugf(DSER | DSYS | DFAT, "FBDEV : Unsupported visual(%d) requested", armfb.f_scr.visual);
	}
}
