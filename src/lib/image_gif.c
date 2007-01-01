/***************************************************************************
    begin                : Sun Jul 10 2005
    copyright            : (C) 2005 - 2007 by Alper Akcan
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

#if defined(IMAGE_GIF)

/* this code is heavily based on SDL_image gif source code
   www.libsdl.org/projects/SDL_image/
 */

#define CM_RED		0
#define CM_GREEN	1
#define CM_BLUE		2

#define	MAX_LWZ_BITS		12
#define INTERLACE	    	0x40
#define LOCALCOLORMAP   	0x80
#define	MAXCOLORMAPSIZE		256

#define lm_to_uint(a, b)	(((b) << 8) | (a))
#define bit_set(byte, bit)	(((byte) & (bit)) == (bit))

typedef struct gif_screen_s {
	unsigned int width;
	unsigned int height;
	unsigned char color_map[3][MAXCOLORMAPSIZE];
	unsigned int bit_pixel;
	unsigned int color_resolution;
	unsigned int background;
	unsigned int aspect_ratio;
	int gray_scale;
} gif_screen_t;

typedef struct gif89_s {
	int transparent;
	int delay_time;
	int input_flag;
	int disposal;
} gif89_t;

typedef struct gifdata_s {
	unsigned char buf[280];
	int curbit;
	int lastbit;
	int done;
	int last_byte;
	/*
	 */
	int fresh;
	int code_size;
	int set_code_size;
	int max_code;
	int max_code_size;
	int firstcode;
	int oldcode;
	int clear_code;
	int end_code;
	int table[2][(1 << MAX_LWZ_BITS)];
	int stack[(1 << (MAX_LWZ_BITS)) * 2];
	int *sp;
	/*
	 */
	int zero_data_block;
	/*
	 */
	gif89_t gif89;
	gif_screen_t gif_screen;
} gifdata_t;

static int s_image_gif_read_file (unsigned char *buf, unsigned int size, FILE *fp)
{
	if (fread(buf, 1, size, fp) < size) {
		return -1;
	} else {
		return 0;
	}
}

static int s_image_gif_get_data_block (FILE *fp, unsigned char *buf, gifdata_t *data)
{
	unsigned char count;
	if (s_image_gif_read_file(&count, 1, fp)) {
		return -1;
	}
	data->zero_data_block = count == 0;
	if ((count != 0) && s_image_gif_read_file(buf, count, fp)) {
		return -1;
	}
	return count;
}

static int s_image_gif_do_extension (FILE *fp, int label, gifdata_t *data)
{
	unsigned char buf[256];
	switch (label) {
		case 0x01:			/* Plain Text Extension */
			break;
		case 0xff:			/* Application Extension */
			break;
		case 0xfe:			/* Comment Extension */
			while (s_image_gif_get_data_block(fp, (unsigned char *) buf, data) != 0);
			return 0;
		case 0xf9:			/* Graphic Control Extension */
			(void) s_image_gif_get_data_block(fp, (unsigned char *) buf, data);
			data->gif89.disposal = (buf[0] >> 2) & 0x7;
			data->gif89.input_flag = (buf[0] >> 1) & 0x1;
			data->gif89.delay_time = lm_to_uint(buf[1], buf[2]);
			if ((buf[0] & 0x1) != 0) {
				data->gif89.transparent = buf[3];
			}
			while (s_image_gif_get_data_block(fp, (unsigned char *) buf, data) != 0);
			return 0;
		default:
			{
				char *str = (char *) buf;
				sprintf(str, "unknown label (0x%02x)", label);
			}
			break;
	}
	while (s_image_gif_get_data_block(fp, (unsigned char *) buf, data) != 0);
	return 0;
}

static int s_image_gif_read_color_map (FILE *fp, int number, unsigned char buffer[3][MAXCOLORMAPSIZE], int *gray)
{
	int i;
	int flag;
	unsigned char rgb[3];

	flag = 1;

	for (i = 0; i < number; ++i) {
		if (s_image_gif_read_file(rgb, sizeof(rgb), fp)) {
			return 1;
		}
		buffer[CM_RED][i] = rgb[0];
		buffer[CM_GREEN][i] = rgb[1];
		buffer[CM_BLUE][i] = rgb[2];
		flag &= (rgb[0] == rgb[1] && rgb[1] == rgb[2]);
	}
	*gray = 0;
	return 0;
}

static int s_image_gif_get_code (FILE *fp, int code_size, int flag, gifdata_t *data)
{
	int i;
	int j;
	int ret;
	unsigned char count;

	if (flag) {
		data->curbit = 0;
		data->lastbit = 0;
		data->done = 0;
		return 0;
	}
	if ((data->curbit + code_size) >= data->lastbit) {
		if (data->done) {
			if (data->curbit >= data->lastbit)
				return -1;
		}
		if (data->last_byte >= 2) { data->buf[0] = data->buf[data->last_byte - 2]; }
		if (data->last_byte >= 1) { data->buf[1] = data->buf[data->last_byte - 1]; }
		if ((count = s_image_gif_get_data_block(fp, &(data->buf[2]), data)) == 0)
			data->done = 1;
		data->last_byte = 2 + count;
		data->curbit = (data->curbit - data->lastbit) + 16;
		data->lastbit = (2 + count) * 8;
	}
	ret = 0;
	for (i = data->curbit, j = 0; j < code_size; ++i, ++j)
		ret |= ((data->buf[i / 8] & (1 << (i % 8))) != 0) << j;
	data->curbit += code_size;
	return ret;
}

static int s_image_gif_lwz_read_byte (FILE *fp, int flag, int input_code_size, gifdata_t *data)
{
	int i;
	int code;
	int incode;

	if (flag) {
		data->set_code_size = input_code_size;
		data->code_size = data->set_code_size + 1;
		data->clear_code = 1 << data->set_code_size;
		data->end_code = data->clear_code + 1;
		data->max_code_size = 2 * data->clear_code;
		data->max_code = data->clear_code + 2;

		s_image_gif_get_code(fp, 0, 1, data);
		data->fresh = 1;

		for (i = 0; i < data->clear_code; ++i) {
			data->table[0][i] = 0;
			data->table[1][i] = i;
		}
		for (; i < (1 << MAX_LWZ_BITS); ++i)
			data->table[0][i] = data->table[1][0] = 0;
		data->sp = data->stack;
		return 0;
	} else if (data->fresh) {
		data->fresh = 0;
		do {
			data->firstcode = data->oldcode = s_image_gif_get_code(fp, data->code_size, 0, data);
		} while (data->firstcode == data->clear_code);
		return data->firstcode;
	}
	if (data->sp > data->stack)
		return *--(data->sp);
	while ((code = s_image_gif_get_code(fp, data->code_size, 0, data)) >= 0) {
		if (code == data->clear_code) {
			for (i = 0; i < data->clear_code; ++i) {
				data->table[0][i] = 0;
				data->table[1][i] = i;
			}
			for (; i < (1 << MAX_LWZ_BITS); ++i)
				data->table[0][i] = data->table[1][i] = 0;
			data->code_size = data->set_code_size + 1;
			data->max_code_size = 2 * data->clear_code;
			data->max_code = data->clear_code + 2;
			data->sp = data->stack;
			data->firstcode = data->oldcode = s_image_gif_get_code(fp, data->code_size, 0, data);
			return data->firstcode;
		} else if (code == data->end_code) {
			int count;
			unsigned char buf[260];
			if (data->zero_data_block)
				return -2;
			while ((count = s_image_gif_get_data_block(fp, buf, data)) > 0);
			if (count != 0) {
			}
			return -2;
		}
		incode = code;
		if (code >= data->max_code) {
			*(data->sp)++ = data->firstcode;
			code = data->oldcode;
		}
		while (code >= data->clear_code) {
			*(data->sp)++ = data->table[1][code];
			if (code == data->table[0][code]) {
			}
			code = data->table[0][code];
		}
		*(data->sp)++ = data->firstcode = data->table[1][code];
		if ((code = data->max_code) < (1 << MAX_LWZ_BITS)) {
			data->table[0][code] = data->oldcode;
			data->table[1][code] = data->firstcode;
			++(data->max_code);
			if ((data->max_code >= data->max_code_size) &&
			    (data->max_code_size < (1 << MAX_LWZ_BITS))) {
				data->max_code_size *= 2;
				++(data->code_size);
			}
		}
		data->oldcode = incode;
		if (data->sp > data->stack)
			return *--(data->sp);
	}
	return code;
}

static int s_image_gif_read_image (FILE *fp, int len, int height, int cmapSize, unsigned char cmap[3][MAXCOLORMAPSIZE], int gray, int interlace, int ignore, gifdata_t *data, unsigned int *rgba)
{
	int v;
	int xpos = 0;
	int ypos = 0;
	int pass = 0;
	unsigned char c;

	if (s_image_gif_read_file(&c, 1, fp)) {
		debugf(DFAT, "s_image_gif_read_file error");
	}
        if (s_image_gif_lwz_read_byte(fp, 1, c, data) < 0) {
		debugf(DFAT, "s_image_gif_lwz_read_byte error");
	}
	if (ignore) {
		while (s_image_gif_lwz_read_byte(fp, 0, c, data) >= 0);
		return 1;
	}
	while ((v = s_image_gif_lwz_read_byte(fp, 0, c, data)) >= 0) {
		*(rgba + ypos * len + xpos) = ((cmap[CM_RED][v] << 0x18) |
					       (cmap[CM_GREEN][v] << 0x10) |
		                               (cmap[CM_BLUE][v] << 0x08) |
		                               ((data->gif89.transparent == v) ? 0xFF : 0x0));
		++xpos;
		if (xpos == len) {
			xpos = 0;
			if (interlace) {
				switch (pass) {
					case 0:
					case 1: ypos += 8; break;
					case 2: ypos += 4; break;
					case 3: ypos += 2; break;
				}
				if (ypos >= height) {
					++pass;
					switch (pass) {
						case 1: ypos = 4; break;
						case 2: ypos = 2; break;
						case 3: ypos = 1; break;
						default: goto end;
					}
				}
			} else {
				++ypos;
			}
		}
		if (ypos >= height)
			break;
	}
end:	return 0;
}

int s_image_gif_is (char *file)
{
	FILE *fp;
	int ret = -1;
	char magic[6];
	
	fp = fopen(file, "r");
	if (fp == NULL) {
		debugf(DFAT, "Could not open file %s", file);
	}
	fread(magic, 1, sizeof(magic), fp);
	if ((strncmp(magic, "GIF", 3) == 0) &&
	    ((memcmp(magic + 3, "87a", 3) == 0) ||
	     (memcmp(magic + 3, "89a", 3) == 0))) {
		 ret = 0;
	}
	fclose(fp);
	return ret;
}

int s_image_gif (char *file, s_image_t *img)
{
	int ret;
	FILE *fp;
	int image;
	int bit_pixel;
	int gray_scale;
	char version[4];
	unsigned char c;
	int image_number;
	gifdata_t *gifdata;
	int image_count = 0;
	unsigned char buf[16];
	int use_global_color_map;
	unsigned char localcolor_map[3][MAXCOLORMAPSIZE];

	s_image_t *tmp;

	ret = 0;
	image_number = 1;

	gifdata = (gifdata_t *) s_malloc(sizeof(gifdata_t));
	memset(gifdata, 0, sizeof(gifdata_t));

	fp = fopen(file, "r");
	if (fp == NULL) {
		debugf(DFAT, "Could not open file %s", file);
	}

	if (s_image_gif_read_file(buf, 6, fp)) {
		debugf(DFAT, "s_image_gif_read_file error");
	}
	if (strncmp((char *) buf, "GIF", 3) != 0) {
		debugf(DFAT, "%s is not gif file", file);
	}

	strncpy(version, (char *) buf + 3, 3);
	version[3] = '\0';
	if ((strcmp(version, "87a") != 0) && (strcmp(version, "89a") != 0)) {
		debugf(DFAT, "%s is not gif file", file);
	}

	gifdata->gif89.transparent = -1;
	gifdata->gif89.delay_time = -1;
	gifdata->gif89.input_flag = -1;
	gifdata->gif89.disposal = 0;

	if (s_image_gif_read_file(buf, 7, fp)) {
		debugf(DFAT, "s_image_gif_read_file error");
	}

	gifdata->gif_screen.width = lm_to_uint(buf[0], buf[1]);
	gifdata->gif_screen.height = lm_to_uint(buf[2], buf[3]);
	gifdata->gif_screen.bit_pixel = 2 << (buf[4] & 0x07);
	gifdata->gif_screen.color_resolution = (((buf[4] & 0x70) >> 3) + 1);
	gifdata->gif_screen.background = buf[5];
	gifdata->gif_screen.aspect_ratio = buf[6];

	if (bit_set(buf[4], LOCALCOLORMAP)) {
		if (s_image_gif_read_color_map(fp, gifdata->gif_screen.bit_pixel, gifdata->gif_screen.color_map, &gifdata->gif_screen.gray_scale)) {
			debugf(DFAT, "s_image_gif_read_color_map error");
		}
	}

	do {
		if (s_image_gif_read_file(&c, 1, fp)) {
			debugf(DFAT, "s_image_gif_read_file error");
		}
		if (c == ';') {
			if (image_count < image_number) {
				break;
			}
		}
		if (c == '!') {
			if (s_image_gif_read_file(&c, 1, fp)) {
				debugf(DFAT, "s_image_gif_read_file error");
			}
			s_image_gif_do_extension(fp, c, gifdata);
			continue;
		}
		if (c != ',') {
			continue;
		}
		++image_count;

		if (s_image_gif_read_file(buf, 9, fp)) {
			debugf(DFAT, "s_image_gif_read_file error");
		}
		use_global_color_map = !bit_set(buf[8], LOCALCOLORMAP);
		bit_pixel = 1 << ((buf[8] & 0x07) + 1);

		if (image_count == image_number) {
			s_image_init(&tmp);
			tmp->x = lm_to_uint(buf[0], buf[1]);
			tmp->y = lm_to_uint(buf[2], buf[3]);
			tmp->w = lm_to_uint(buf[4], buf[5]);
			tmp->h = lm_to_uint(buf[6], buf[7]);
			tmp->delay = gifdata->gif89.delay_time * 10;
			tmp->rgba = (unsigned int *) s_calloc(1, tmp->w * tmp->h * sizeof(unsigned int));
			s_list_add(img->layers, tmp, -1);
		}

		if (!use_global_color_map) {
			if (s_image_gif_read_color_map(fp, bit_pixel, localcolor_map, &gray_scale)) {
				debugf(DFAT, "s_image_gif_read_color_map error");
			}
			image = s_image_gif_read_image(fp, lm_to_uint(buf[4], buf[5]),
					       lm_to_uint(buf[6], buf[7]),
					       bit_pixel, localcolor_map, gray_scale,
					       bit_set(buf[8], INTERLACE),
					       image_count != image_number,
					       gifdata, tmp->rgba);
			if (image == 0) { image_number++; };
		} else {
			image = s_image_gif_read_image(fp, lm_to_uint(buf[4], buf[5]),
					       lm_to_uint(buf[6], buf[7]),
					       gifdata->gif_screen.bit_pixel, gifdata->gif_screen.color_map,
					       gifdata->gif_screen.gray_scale, bit_set(buf[8], INTERLACE),
					       image_count != image_number,
					       gifdata, tmp->rgba);
			if (image == 0) { image_number++; };
		}
	} while (1);

	fclose(fp);
	s_free(gifdata);

	return 0;
}

#else

int s_image_gif_is (char *file)
{
	return -1;
}

int s_image_gif (char *file, s_image_t *img)
{
	return -1;
}

#endif
