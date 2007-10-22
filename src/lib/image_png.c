/***************************************************************************
    begin                : Wed Oct 6 2004
    copyright            : (C) 2004 - 2007 by Alper Akcan
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

#include "xynth_.h"

#if defined(CONFIG_IMAGE_PNG)

#include <png.h>

int s_image_png_is (char *file)
{
	FILE *fp;
	int ret = -1;
	char magic[8];

	if ((fp = fopen(file, "rb")) == NULL) {
		debugf(DCLI | DFAT, "Could not open file (%s) for reading", file);
	}
	fread(magic, 1, sizeof(magic), fp);
	ret = png_sig_cmp((png_bytep) magic, (png_size_t) 0, 8);
	fclose(fp);

	return ret;
}

int s_image_png (char *file, s_image_t *img)
{
	int x;
	int y;
	FILE *fp;
	int width;
	int height;
	char header[8];
	unsigned int *tmp;
	png_byte bit_depth;
	png_infop info_ptr;
	png_structp png_ptr;
	png_byte color_type;
	int number_of_passes;
	png_bytep *row_pointers;
	
	fp = fopen(file, "rb");
	if (!fp) {
		debugf(DFAT, "Could not open file %s", file);
	}
	fread(header, 1, 8, fp);
	if (png_sig_cmp((png_bytep) header, 0, 8)) {
		debugf(DFAT, "File is not a PNG file %s", file);
	}
	
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) {
		debugf(DFAT, "png_create_read_struct failed");
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		debugf(DFAT, "png_create_info_struct failed");
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		debugf(DFAT, "setjmp failed");
	}
	
	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);
	png_read_info(png_ptr, info_ptr);
	width = info_ptr->width;
	height = info_ptr->height;                                                 	
	color_type = info_ptr->color_type;
	bit_depth = info_ptr->bit_depth;
	number_of_passes = png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);
	
	if (setjmp(png_jmpbuf(png_ptr))) {
		debugf(DFAT, "Error during read_image");
	}

	row_pointers = (png_bytep *) s_malloc(sizeof(png_bytep) * height);
	for (y = 0; y < height; y++) {
		row_pointers[y] = (png_byte* ) s_malloc(info_ptr->rowbytes);
	}

        png_read_image(png_ptr, row_pointers);
	
	img->w = width;
	img->h = height;
	img->rgba = (unsigned int *) s_calloc(1, img->w * img->h * sizeof(unsigned int));
	tmp = img->rgba;

	if (info_ptr->color_type == PNG_COLOR_TYPE_RGBA) {
		for (y = 0; y < height; y++) {
			png_byte *row = row_pointers[y];
			for (x = 0; x < width; x++) {
				png_byte *ptr = &(row[x * 4]);

				*tmp |= (ptr[0] << 0x18);
				*tmp |= (ptr[1] << 0x10);
				*tmp |= (ptr[2] << 0x08);
				*tmp |= (~(ptr[3] << 0x00)) & 0xFF;

				ptr[0] = 0;
				ptr[1] = ptr[2];
				tmp++;
			}
		}
	} else if (info_ptr->color_type == PNG_COLOR_TYPE_RGB) {
		for (y = 0; y < height; y++) {
			png_byte *row = row_pointers[y];
			for (x = 0; x < width; x++) {
				png_byte *ptr = &(row[x * 3]);

				*tmp |= (ptr[0] << 0x18);
				*tmp |= (ptr[1] << 0x10);
				*tmp |= (ptr[2] << 0x08);
				*tmp |= 0x0;

				ptr[0] = 0;
				ptr[1] = ptr[2];
				tmp++;
			}
		}
	} else if (info_ptr->color_type == PNG_COLOR_TYPE_PALETTE) {
		int i;
		struct Palette {
			int r;
			int g;
			int b;
			int a;
		};
		struct Palette *pal;
		pal = (struct Palette *) s_malloc(256 * sizeof(struct Palette));
		memset(pal, 0, 256 * sizeof(struct Palette));
		
		for (i = 0; i < 256; i++) {
			pal[i].a = 255;
		}
		
		for (i = 0; i < info_ptr->num_palette; i++) {
			pal[i].r = info_ptr->palette[i].red;
			pal[i].g = info_ptr->palette[i].green;
			pal[i].b = info_ptr->palette[i].blue;
			if (i < info_ptr->num_trans) {
				pal[i].a = info_ptr->trans[i];
			}
		}

		for (y = 0; y < height; y++) {
			png_byte *row = row_pointers[y];
			for (x = 0; x < width; x++) {
				png_byte *ptr = &(row[x]);
				*tmp |= (pal[ptr[0]].r << 0x18);
				*tmp |= (pal[ptr[0]].g << 0x10);
				*tmp |= (pal[ptr[0]].b << 0x08);
				*tmp |= (~(pal[ptr[0]].a << 0x00)) & 0xff;
				tmp++;
			}
		}
		s_free(pal);
	} else {
		debugf(DFAT, "Unknown color_type : %d (%s)", info_ptr->color_type, file);
	}

	for (y = 0; y < height; y++) {
		s_free(row_pointers[y]);
	}
	s_free(row_pointers);
	
	png_destroy_info_struct(png_ptr, &info_ptr);
	png_destroy_read_struct(&png_ptr, NULL, NULL);
	
	fclose(fp);
	
	return 0;
}

#else

int s_image_png_is (char *file)
{
	return -1;
}

int s_image_png (char *file, s_image_t *img)
{
	return -1;
}

#endif
