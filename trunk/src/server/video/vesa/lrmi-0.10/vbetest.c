/*
List the available VESA graphics modes.

This program is in the public domain.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#if defined(__linux__)
#include <sys/io.h>
#include <sys/kd.h>
#include <sys/stat.h>
#elif defined(__NetBSD__) || defined(__OpenBSD__)
#include <time.h>
#include <dev/wscons/wsconsio.h>
#include <machine/sysarch.h>
#elif defined(__FreeBSD__)
#include <sys/consio.h>
#include <machine/sysarch.h>
#endif

#include "lrmi.h"
#include "vbe.h"

struct {
	struct vbe_info_block *info;
	struct vbe_mode_info_block *mode;
	char *win; 	/* this doesn't point directly at the window, see update_window() */
	int win_low, win_high;
} vbe;

static char *run_command = NULL;

void *
save_state(void)
{
	struct LRMI_regs r;
	void *buffer;

	memset(&r, 0, sizeof(r));

	r.eax = 0x4f04;
	r.ecx = 0xf; 	/* all states */
	r.edx = 0; 	/* get buffer size */

	if (!LRMI_int(0x10, &r)) {
		fprintf(stderr, "Can't get video state buffer size (vm86 failure)\n");
		return NULL;
	}

	if ((r.eax & 0xffff) != 0x4f) {
		fprintf(stderr, "Get video state buffer size failed\n");
		return NULL;
	}

	buffer = LRMI_alloc_real((r.ebx & 0xffff) * 64);

	if (buffer == NULL) {
		fprintf(stderr, "Can't allocate video state buffer\n");
		return NULL;
	}

	memset(&r, 0, sizeof(r));

	r.eax = 0x4f04;
	r.ecx = 0xf; 	/* all states */
	r.edx = 1; 	/* save state */
	r.es = (unsigned int)buffer >> 4;
	r.ebx = (unsigned int)buffer & 0xf;

	if (!LRMI_int(0x10, &r)) {
		fprintf(stderr, "Can't save video state (vm86 failure)\n");
		return NULL;
	}

	if ((r.eax & 0xffff) != 0x4f) {
		fprintf(stderr, "Save video state failed\n");
		return NULL;
	}

	return buffer;
}

void
restore_state(void *buffer)
{
	struct LRMI_regs r;

	memset(&r, 0, sizeof(r));

	r.eax = 0x4f04;
	r.ecx = 0xf; 	/* all states */
	r.edx = 2; 	/* restore state */
	r.es = (unsigned int)buffer >> 4;
	r.ebx = (unsigned int)buffer & 0xf;

	if (!LRMI_int(0x10, &r)) {
		fprintf(stderr, "Can't restore video state (vm86 failure)\n");
	} else if ((r.eax & 0xffff) != 0x4f) {
		fprintf(stderr, "Restore video state failed\n");
	}

	LRMI_free_real(buffer);
}

void
text_mode(void)
{
	struct LRMI_regs r;

	memset(&r, 0, sizeof(r));

	r.eax = 3;

	if (!LRMI_int(0x10, &r)) {
		fprintf(stderr, "Can't set text mode (vm86 failure)\n");
	}
}

int
update_window(int address)
{
	struct LRMI_regs r;
	int w, g;

	if (address >= vbe.win_low && address < vbe.win_high)
		return 0;

	g = vbe.mode->win_granularity * 1024;
	w = address / g;

	memset(&r, 0, sizeof(r));

	r.eax = 0x4f05;
	r.ebx = 0;
	r.edx = w;

	LRMI_int(0x10, &r);

	vbe.win_low = w * g;
	vbe.win_high = vbe.win_low + vbe.mode->win_size * 1024;

	vbe.win = (char *)(vbe.mode->win_a_segment << 4);
	vbe.win -= vbe.win_low;

	return 1;
}

void
set_pixel(int x, int y, int r, int g, int b)
{
	int x_res = vbe.mode->x_resolution;
	int y_res = vbe.mode->y_resolution;
	int shift_r = vbe.mode->red_field_position;
	int shift_g = vbe.mode->green_field_position;
	int shift_b = vbe.mode->blue_field_position;
	int pixel_size = (vbe.mode->bits_per_pixel + 7) / 8;
	int bpl = vbe.mode->bytes_per_scanline;
	int c, addr;

	if (x < 0 || x >= x_res || y < 0 || y >= y_res)
		return;

	r >>= 8 - vbe.mode->red_mask_size;
	g >>= 8 - vbe.mode->green_mask_size;
	b >>= 8 - vbe.mode->blue_mask_size;

	c = (r << shift_r) | (g << shift_g) | (b << shift_b);

	addr = y * bpl + (x * pixel_size);

	update_window(addr);

	memcpy(vbe.win + addr, &c, pixel_size);
}

void
set_mode(int n)
{
	struct LRMI_regs r;

	memset(&r, 0, sizeof(r));

	r.eax = 0x4f02;
	r.ebx = n;

	if (!LRMI_int(0x10, &r)) {
		fprintf(stderr, "Can't set video mode (vm86 failure)\n");
	} else if ((r.eax & 0xffff) != 0x4f) {
		fprintf(stderr, "Set video mode failed\n");
	}

	memset(&r, 0, sizeof(r));

	r.eax = 0x4f01;
	r.ecx = n;
	r.es = (unsigned int)vbe.mode >> 4;
	r.edi = (unsigned int)vbe.mode & 0xf;

	if (!LRMI_int(0x10, &r)) {
		fprintf(stderr, "Can't get mode info (vm86 failure)\n");
		return;
	}

	if ((r.eax & 0xffff) != 0x4f) {
		fprintf(stderr, "Get mode info failed\n");
		return;
	}

	vbe.win_low = vbe.win_high = -1;

	/*
	 Draw a colorful checkerboard
	*/
	if (vbe.mode->memory_model == VBE_MODEL_RGB) {
		int x_res = vbe.mode->x_resolution;
		int y_res = vbe.mode->y_resolution;
		int x, y;

		for (y = 0; y < y_res; ++y) {
			for (x = 0; x < x_res; ++x) {
				int r, g, b;
				if ((x & 16) ^ (y & 16)) {
					r = x * 255 / x_res;
					g = y * 255 / y_res;
					b = 255 - x * 255 / x_res;
				} else {
					r = 255 - x * 255 / x_res;
					g = y * 255 / y_res;
					b = 255 - y * 255 / y_res;
				}

				set_pixel(x, y, r, g, b);
			}
		}
	}
}

void
interactive_set_mode(int n)
{
	void *state;
#if defined(__linux__)
	struct stat stat;
#elif defined(__NetBSD__) || defined(__OpenBSD__)
	struct wsdisplay_fbinfo wsi;
#elif defined(__FreeBSD__)
	struct vid_info vi;
#endif

	if (n == -1) {
		printf("Type a mode number, or 'q' to quit - ");

		if (scanf("%d", &n) != 1)
			return;
	}

#if defined(__linux__)
	if (fstat(0, &stat) != 0) {
		fprintf(stderr, "Can't stat() stdin\n");
		return;
	}

	if ((stat.st_rdev & 0xff00) != 0x400
	 || (stat.st_rdev & 0xff) > 63)
#elif defined(__NetBSD__) || defined(__OpenBSD__)
	if (ioctl(0, WSDISPLAYIO_GINFO, &wsi) == -1)
#elif defined(__FreeBSD__) 
	memset(&vi, 0, sizeof(vi));
	vi.size = sizeof(vi);
	if (ioctl(0, CONS_GETINFO, &vi) == -1)
#endif
	{
		fprintf(stderr, "To switch video modes, "
		 "this program must be run from the console\n");
		return;
	}

	printf("setting mode %d\n", n);

#if defined(__linux__) || defined(__FreeBSD__)
	ioctl(0, KDSETMODE, KD_GRAPHICS);
#elif defined(__NetBSD__) || defined(__OpenBSD__)
	ioctl(0, WSDISPLAYIO_SMODE, WSDISPLAYIO_MODE_MAPPED);
#endif

	state = save_state();

	if (state == NULL)
		return;

	set_mode(n);

	system(run_command);

	sleep(5);

	text_mode();
	restore_state(state);

#if defined(__linux__) || defined(__FreeBSD__)
	ioctl(0, KDSETMODE, KD_TEXT);
#elif defined(__NetBSD__) || defined(__OpenBSD__)
	ioctl(0, WSDISPLAYIO_SMODE, WSDISPLAYIO_MODE_EMUL);
#endif
}

void
usage_and_quit(int error)
{
	fputs("Usage: vbetest [-m mode] [-c command]\n",
	 error ? stderr : stdout);
	exit(error);
}

int
main(int argc, char *argv[])
{
	struct LRMI_regs r;
	short int *mode_list;
	int i, mode = -1;
#if defined(__NetBSD__) || defined(__OpenBSD__)
	unsigned long iomap[32];
#endif

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-c") == 0) {
			i++;
			if (i == argc)
				usage_and_quit(1);
			run_command = argv[i];
		} else if (strcmp(argv[i], "-m") == 0) {
			char *e;
			i++;
			if (i == argc)
				usage_and_quit(1);
			mode = strtol(argv[i], &e, 10);
			if (e == argv[i])
				usage_and_quit(1);
		} else
			usage_and_quit(1);
	}

	if (!LRMI_init())
		return 1;

	vbe.info = LRMI_alloc_real(sizeof(struct vbe_info_block)
	 + sizeof(struct vbe_mode_info_block));

	if (vbe.info == NULL) {
		fprintf(stderr, "Can't alloc real mode memory\n");
		return 1;
	}

	vbe.mode = (struct vbe_mode_info_block *)(vbe.info + 1);

#if 0
	/*
	 Allow read/write to video IO ports
	*/
	ioperm(0x2b0, 0x2df - 0x2b0, 1);
	ioperm(0x3b0, 0x3df - 0x3b0, 1);
#else
	/*
	 Allow read/write to ALL io ports
	*/
#if defined(__linux__)
	ioperm(0, 1024, 1);
	iopl(3);
#elif defined(__NetBSD__) || defined(__OpenBSD__)
	memset(&iomap[0], 0xff, sizeof(iomap));
	i386_set_ioperm(iomap);
	i386_iopl(3);
#elif defined(__FreeBSD__)
	i386_set_ioperm(0, 0x10000, 1);
#endif
#endif

	memset(&r, 0, sizeof(r));

	r.eax = 0x4f00;
	r.es = (unsigned int)vbe.info >> 4;
	r.edi = 0;

	memcpy(vbe.info->vbe_signature, "VBE2", 4);

	if (!LRMI_int(0x10, &r)) {
		fprintf(stderr, "Can't get VESA info (vm86 failure)\n");
		return 1;
	}

	if ((r.eax & 0xffff) != 0x4f || strncmp(vbe.info->vbe_signature, "VESA", 4) != 0) {
		fprintf(stderr, "No VESA bios\n");
		return 1;
	}

	printf("VBE Version %x.%x\n",
	 (int)(vbe.info->vbe_version >> 8) & 0xff,
	 (int)vbe.info->vbe_version & 0xff);

	printf("%s\n",
	 (char *)(vbe.info->oem_string_seg * 16 + vbe.info->oem_string_off));

	mode_list = (short int *)(vbe.info->video_mode_list_seg * 16 + vbe.info->video_mode_list_off);

	while (*mode_list != -1) {
		memset(&r, 0, sizeof(r));

		r.eax = 0x4f01;
		r.ecx = *mode_list;
		r.es = (unsigned int)vbe.mode >> 4;
		r.edi = (unsigned int)vbe.mode & 0xf;

		if (!LRMI_int(0x10, &r)) {
			fprintf(stderr, "Can't get mode info (vm86 failure)\n");
			return 1;
		}

		if (vbe.mode->memory_model == VBE_MODEL_RGB)
			printf("[%3d] %dx%d (%d:%d:%d)\n",
			 *mode_list,
			 vbe.mode->x_resolution,
			 vbe.mode->y_resolution,
			 vbe.mode->red_mask_size,
			 vbe.mode->green_mask_size,
			 vbe.mode->blue_mask_size);
		else if (vbe.mode->memory_model == VBE_MODEL_256)
			printf("[%3d] %dx%d (256 color palette)\n",
			 *mode_list,
			 vbe.mode->x_resolution,
			 vbe.mode->y_resolution);
		else if (vbe.mode->memory_model == VBE_MODEL_PACKED)
			printf("[%3d] %dx%d (%d color palette)\n",
			 *mode_list,
			 vbe.mode->x_resolution,
			 vbe.mode->y_resolution,
			 1 << vbe.mode->bits_per_pixel);

		mode_list++;
	}

	LRMI_free_real(vbe.info);

	interactive_set_mode(mode);

	return 0;
}
