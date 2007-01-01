/***************************************************************************
    begin                : Wed Oct 8 2003
    copyright            : (C) 2003 - 2007 by Alper Akcan
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

#if defined(VIDEO_VESA)

#include "../../../lib/xynth_.h"
#include "server.h"
#include "vesa.h"
#include "../helper/helper.h"

#include "lrmi-0.10/lrmi.h"
#include "lrmi-0.10/vbe.h"

#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/io.h>
#include <stdint.h>

s_video_input_t s_video_vesa_input_keybd = {
	VIDEO_INPUT_KEYBD,
	s_video_vesa_kbd_init,
	s_video_helper_kbd_update,
	s_video_helper_kbd_uninit,
};

s_video_input_t s_video_vesa_input_mouse = {
	VIDEO_INPUT_MOUSE,
	s_video_helper_mouse_init,
	s_video_helper_mouse_update,
	s_video_helper_mouse_uninit,
};

s_video_input_t *s_video_vesa_input[] = {
	&s_video_vesa_input_keybd,
	&s_video_vesa_input_mouse,
	NULL,
};

s_video_driver_t s_video_vesa = {
	"vesa",
	"/dev/mem",
	s_video_vesa_input,
	s_video_vesa_server_init,
	s_video_vesa_server_uninit,
	s_video_vesa_server_goto_back,
	s_video_vesa_server_comefrom_back,
	s_video_vesa_server_restore,
	NULL,
	NULL,
	NULL
};

static int s_video_vesa_text_mode (void)
{
	struct LRMI_regs r;

	memset(&r, 0, sizeof(r));
	r.eax = 3;
	if (!LRMI_int(0x10, &r)) {
		debugf(DSER, "Can't set text mode (vm86 failure)\n");
		return -1;
	}

	return 0;
}

static int s_video_vesa_restore_state (void *buffer)
{
	int ret = 0;
	struct LRMI_regs r;

	memset(&r, 0, sizeof(r));
	r.eax = 0x4f04;
	r.ecx = 0xf; 	/* all states */
	r.edx = 2; 	/* restore state */
	r.es = (unsigned int) buffer >> 4;
	r.ebx = (unsigned int) buffer & 0xf;
	if (!LRMI_int(0x10, &r)) {
		debugf(DSER, "Can't restore video state (vm86 failure)\n");
		ret = -1;
	} else if ((r.eax & 0xffff) != 0x4f) {
		debugf(DSER, "Restore video state failed\n");
		ret = -1;
	}
	LRMI_free_real(buffer);

	return ret;
}

static void * s_video_vesa_save_state (void)
{
	void *buffer;
	struct LRMI_regs r;

	memset(&r, 0, sizeof(r));
	r.eax = 0x4f04;
	r.ecx = 0xf; 	/* all states */
	r.edx = 0; 	/* get buffer size */
	if (!LRMI_int(0x10, &r)) {
		debugf(DSER, "Can't get video state buffer size (vm86 failure)\n");
		goto err0;
	}
	if ((r.eax & 0xffff) != 0x4f) {
		debugf(DSER, "Get video state buffer size failed\n");
		goto err0;
	}

	buffer = LRMI_alloc_real((r.ebx & 0xffff) * 64);
	if (buffer == NULL) {
		debugf(DSER, "Can't allocate video state buffer\n");
		goto err0;
	}

	memset(&r, 0, sizeof(r));
	r.eax = 0x4f04;
	r.ecx = 0xf; 	/* all states */
	r.edx = 1; 	/* save state */
	r.es = (unsigned int)buffer >> 4;
	r.ebx = (unsigned int)buffer & 0xf;
	if (!LRMI_int(0x10, &r)) {
		debugf(DSER, "Can't save video state (vm86 failure)\n");
		goto err1;
	}
	if ((r.eax & 0xffff) != 0x4f) {
		debugf(DSER, "Save video state failed\n");
		goto err1;
	}

	return buffer;
err1:	LRMI_free_real(buffer);
err0:	return NULL;
}

static int s_video_vesa_set_mode (int n)
{
	int fd;
	unsigned int lmembase;

	struct LRMI_regs r;
	s_video_vesa_data_t *priv = (s_video_vesa_data_t *) server->driver->driver_data;

	priv->vbe.state = s_video_vesa_save_state();
	if (priv->vbe.state == NULL) {
		goto err0;
	}
	priv->vbe.mode_n = n;

	memset(&r, 0, sizeof(r));
	r.eax = 0x4f02;
	r.ebx = n;
	if (!LRMI_int(0x10, &r)) {
		debugf(DSER, "Can't set video mode (vm86 failure)\n");
		goto err0;
	} else if ((r.eax & 0xffff) != 0x4f) {
		debugf(DSER, "Set video mode failed\n");
		goto err0;
	}

	memset(&r, 0, sizeof(r));
	r.eax = 0x4f02;
	r.ebx = n | 0x4000; /* linear mode */
	if (!LRMI_int(0x10, &r)) {
		debugf(DSER, "Can't set video mode (vm86 failure)\n");
		goto err0;
	} else if ((r.eax & 0xffff) != 0x4f) {
		debugf(DSER, "Set video mode failed\n");
		goto err0;
	}

	memset(&r, 0, sizeof(r));
	r.eax = 0x4f01;
	r.ecx = n;
	r.es = (unsigned int) priv->vbe.mode >> 4;
	r.edi = (unsigned int) priv->vbe.mode & 0xf;
	if (!LRMI_int(0x10, &r)) {
		debugf(DSER, "Can't get mode info (vm86 failure)\n");
		goto err0;
	}
	if ((r.eax & 0xffff) != 0x4f) {
		debugf(DSER, "Get mode info failed\n");
		goto err0;
	}

	server->window->surface->linear_mem_base = (unsigned int) priv->vbe.mode->phys_base_ptr;
	server->window->surface->linear_mem_size = (unsigned int) (priv->vbe.info->total_memory);
	/* is this ok? haha this is really ugly.
	 */
	server->window->surface->linear_mem_size += (1024 - (priv->vbe.info->total_memory % 1024));
	server->window->surface->linear_mem_size *= (64 * 1024);

	fd = open("/dev/mem", O_RDWR);
	if (fd < 0) {
		debugf(DSER, "open failed");
		goto err1;
	}
	if (server->window->surface->linear_mem_size) {
		lmembase = (unsigned int) mmap((caddr_t) 0, server->window->surface->linear_mem_size,
		                                            PROT_WRITE | PROT_READ, MAP_SHARED, fd,
		                                            (off_t) server->window->surface->linear_mem_base);
		if (lmembase == (unsigned int) MAP_FAILED) {
			debugf(DSER, "mmap failed");
			goto err1;
		}
	} else {
		debugf(DSER, "mmap failed");
			goto err1;
	}
	priv->vbe.addr = (char *) lmembase;

	close(fd);
	return 0;
err1:	close(fd);
err0:	return -1;
}

void s_video_vesa_server_restore (void)
{
	s_video_vesa_data_t *priv = (s_video_vesa_data_t *) server->driver->driver_data;
	if (server->driver->driver_data == NULL) {
		return;
	}
	if (priv->vbe.mode_n < 0) {
		return;
	}
	s_video_vesa_text_mode();
	s_video_vesa_restore_state(priv->vbe.state);
}

void s_video_vesa_server_goto_back (void)
{
	s_video_vesa_data_t *priv = (s_video_vesa_data_t *) server->driver->driver_data;
	s_video_vesa_text_mode();
	s_video_vesa_restore_state(priv->vbe.state);
	s_server_surface_lock_real();
}

void s_video_vesa_server_comefrom_back (void)
{
	s_video_vesa_data_t *priv = (s_video_vesa_data_t *) server->driver->driver_data;
	s_video_vesa_set_mode(priv->vbe.mode_n);
	s_server_surface_refresh();
	s_video_helper_kbd_set_attr();
}

void s_video_vesa_surface_uninit (void)
{
	s_video_vesa_data_t *priv = (s_video_vesa_data_t *) server->driver->driver_data;
	s_video_vesa_text_mode();
	s_video_vesa_restore_state(priv->vbe.state);
	munmap((void *) server->window->surface->linear_mem_base, server->window->surface->linear_mem_size);
	s_free(priv);
	server->driver->driver_data = NULL;
}

void s_video_vesa_server_uninit (void)
{
        s_video_vesa_surface_uninit();
}

void s_video_vesa_server_surface_setrgbpalette (void)
{
}

static void s_video_vesa_surface_init (s_video_helper_mode_info_t *gmode)
{
	s_video_vesa_data_t *priv = (s_video_vesa_data_t *) server->driver->driver_data;

        server->window->surface->width = gmode->xdim;
        server->window->surface->height = gmode->ydim;
        server->window->surface->bytesperpixel = gmode->bytesperpixel;
        server->window->surface->bitsperpixel = priv->vbe.mode->bits_per_pixel;
	server->window->surface->blueoffset = priv->vbe.mode->blue_field_position;
	server->window->surface->greenoffset = priv->vbe.mode->green_field_position;
	server->window->surface->redoffset = priv->vbe.mode->red_field_position;
	server->window->surface->bluelength = priv->vbe.mode->blue_mask_size;
	server->window->surface->greenlength = priv->vbe.mode->green_mask_size;
	server->window->surface->redlength = priv->vbe.mode->red_mask_size;
	server->window->surface->colors = gmode->colors;

        switch (gmode->colors) {
		default:
		case 256:
			server->window->surface->bitsperpixel = 8;
			server->window->surface->blueoffset = 0;
			server->window->surface->greenoffset = 3;
			server->window->surface->redoffset = 6;
			server->window->surface->bluelength = 3;
			server->window->surface->greenlength = 3;
			server->window->surface->redlength = 2;
			break;
		case 32768:
			server->window->surface->bitsperpixel = 15;
			server->window->surface->blueoffset = 0;
			server->window->surface->greenoffset = 5;
			server->window->surface->redoffset = 10;
			server->window->surface->bluelength = 5;
			server->window->surface->greenlength = 5;
			server->window->surface->redlength = 5;
			break;
		case 65536:
			server->window->surface->bitsperpixel = 16;
			server->window->surface->blueoffset = 0;
			server->window->surface->greenoffset = 5;
			server->window->surface->redoffset = 11;
			server->window->surface->bluelength = 5;
			server->window->surface->greenlength = 6;
			server->window->surface->redlength = 5;
			break;
		case 256 * 65536:
			server->window->surface->bitsperpixel = server->window->surface->bytesperpixel * 8;
			server->window->surface->blueoffset = 0;
			server->window->surface->greenoffset = 8;
			server->window->surface->redoffset = 16;
			server->window->surface->bluelength = 8;
			server->window->surface->greenlength = 8;
			server->window->surface->redlength = 8;
			break;
	}

	server->window->surface->vbuf = priv->vbe.addr;
	server->window->surface->linear_buf = priv->vbe.addr;
        server->window->surface->linear_mem_base = server->window->surface->linear_mem_base;
	server->window->surface->linear_mem_size = server->window->surface->linear_mem_size;

	s_video_helper_mtrr_add(server->window->surface->linear_mem_base, server->window->surface->linear_mem_size);
	s_video_vesa_server_surface_setrgbpalette();
}

int s_video_vesa_server_init (s_server_conf_t *cfg)
{
	struct LRMI_regs r;
	short int *mode_list;

	s_video_vesa_data_t *priv;
	s_video_helper_mode_info_t *gmode;

	priv = (s_video_vesa_data_t *) s_malloc(sizeof(s_video_vesa_data_t));
	server->driver->driver_data = (void *) priv;
	priv->vbe.mode_n = -1;
	priv->console_fd = -1;
	
	priv->console_fd = s_video_helper_console_init();
	if (priv->console_fd < 0) {
		debugf(DSER, "Unable to get console");
		goto err0;
	}
	if (s_video_helper_mode_find(cfg, &gmode)) {
		debugf(DSER, "Couldn't find mode: %s", cfg->general.mode);
		goto err0;
	}

	if (!LRMI_init()) {
		debugf(DSER, "LRMI_init() failed");
		goto err0;
	}
	priv->vbe.info = LRMI_alloc_real(sizeof(struct vbe_info_block) +
	                                 sizeof(struct vbe_mode_info_block));
	if (priv->vbe.info == NULL) {
		debugf(DSER, "Not enough memory!");
		goto err1;
	}
	priv->vbe.mode = (struct vbe_mode_info_block *) (priv->vbe.info + 1);

	ioperm(0, 1024, 1);
	iopl(3);

	memset(&r, 0, sizeof(r));
	r.eax = 0x4f00;
	r.es = (unsigned int) priv->vbe.info >> 4;
	r.edi = 0;
	memcpy(priv->vbe.info->vbe_signature, "VBE2", 4);
	if (!LRMI_int(0x10, &r)) {
		debugf(DSER, "Can't get VESA info (vm86 failure)");
		goto err1;
	}
	if ((r.eax & 0xffff) != 0x4f || strncmp(priv->vbe.info->vbe_signature, "VESA", 4) != 0) {
		debugf(DSER, "No VESA bios");
		goto err1;
	}

	debugf(DSER, "VBE Version %x.%x", (int) (priv->vbe.info->vbe_version >> 8) & 0xff,
	                                       (int) (priv->vbe.info->vbe_version) & 0xff);

	debugf(DSER, "%s", (char *) (priv->vbe.info->oem_string_seg * 16 + priv->vbe.info->oem_string_off));

	mode_list = (short int *) (priv->vbe.info->video_mode_list_seg * 16 + priv->vbe.info->video_mode_list_off);

	while (*mode_list != -1) {
		memset(&r, 0, sizeof(r));

		r.eax = 0x4f01;
		r.ecx = *mode_list;
		r.es = (unsigned int) priv->vbe.mode >> 4;
		r.edi = (unsigned int) priv->vbe.mode & 0xf;

		if (!LRMI_int(0x10, &r)) {
			debugf(DSER, "Can't get mode info (vm86 failure)");
			goto err1;
		}
/*
		if (priv->vbe.mode->memory_model == VBE_MODEL_RGB) {
			debugf(DSER, "[%3d] %dx%d (%d:%d:%d)", *mode_list,
			                                            priv->vbe.mode->x_resolution,
			                                            priv->vbe.mode->y_resolution,
			                                            priv->vbe.mode->red_mask_size,
			                                            priv->vbe.mode->green_mask_size,
			                                            priv->vbe.mode->blue_mask_size);
		} else if (priv->vbe.mode->memory_model == VBE_MODEL_256) {
			debugf(DSER, "[%3d] %dx%d (256 color palette)", *mode_list,
			                                                     priv->vbe.mode->x_resolution,
			                                                     priv->vbe.mode->y_resolution);
		} else if (priv->vbe.mode->memory_model == VBE_MODEL_PACKED) {
			debugf(DSER, "[%3d] %dx%d (%d color palette)", *mode_list,
			                                                    priv->vbe.mode->x_resolution,
			                                                    priv->vbe.mode->y_resolution,
			                                                    1 << priv->vbe.mode->bits_per_pixel);
                }
*/
                if ((priv->vbe.mode->x_resolution == gmode->xdim) &&
                    (priv->vbe.mode->y_resolution == gmode->ydim) &&
                    ((1 << ((priv->vbe.mode->bits_per_pixel == 32) ? 24 : priv->vbe.mode->bits_per_pixel)) == gmode->colors)) {
			s_video_vesa_set_mode(*mode_list);
			s_video_vesa_surface_init(gmode);
			return gmode->number;
		}
		mode_list++;
	}

err1:	LRMI_free_real(priv->vbe.info);
	debugf(DSER, "Mode %s cannot be found", cfg->general.mode);
err0:	s_free(priv);
	server->driver->driver_data = NULL;
	return -1;
}

#endif /* VIDEO_VESA */
