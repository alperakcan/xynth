/***************************************************************************
    begin                : Thu Jun 3 2004
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

#if defined(IMAGE_XPM)

#define BUFFSIZE	(1 << 20)

struct {
	char *name;
	int r;
	int g;
	int b;
} rgbRecord[234] = {
	{"AliceBlue", 240, 248, 255},
	{"AntiqueWhite", 250, 235, 215},
	{"Aquamarine", 50, 191, 193},
	{"Azure", 240, 255, 255},
	{"Beige", 245, 245, 220},
	{"Bisque", 255, 228, 196},
	{"Black", 0, 0, 0},
	{"BlanchedAlmond", 255, 235, 205},
	{"Blue", 0, 0, 255},
	{"BlueViolet", 138, 43, 226},
	{"Brown", 165, 42, 42},
	{"burlywood", 222, 184, 135},
	{"CadetBlue", 95, 146, 158},
	{"chartreuse", 127, 255, 0},
	{"chocolate", 210, 105, 30},
	{"Coral", 255, 114, 86},
	{"CornflowerBlue", 34, 34, 152},
	{"cornsilk", 255, 248, 220},
	{"Cyan", 0, 255, 255},
	{"DarkGoldenrod", 184, 134, 11},
	{"DarkGreen", 0, 86, 45},
	{"DarkKhaki", 189, 183, 107},
	{"DarkOliveGreen", 85, 86, 47},
	{"DarkOrange", 255, 140, 0},
	{"DarkOrchid", 139, 32, 139},
	{"DarkSalmon", 233, 150, 122},
	{"DarkSeaGreen", 143, 188, 143},
	{"DarkSlateBlue", 56, 75, 102},
	{"DarkSlateGray", 47, 79, 79},
	{"DarkTurquoise", 0, 166, 166},
	{"DarkViolet", 148, 0, 211},
	{"DeepPink", 255, 20, 147},
	{"DeepSkyBlue", 0, 191, 255},
	{"DimGray", 84, 84, 84},
	{"DodgerBlue", 30, 144, 255},
	{"Firebrick", 142, 35, 35},
	{"FloralWhite", 255, 250, 240},
	{"ForestGreen", 80, 159, 105},
	{"gainsboro", 220, 220, 220},
	{"GhostWhite", 248, 248, 255},
	{"Gold", 218, 170, 0},
	{"Goldenrod", 239, 223, 132},
	{"Gray", 126, 126, 126},
	{"Gray0", 0, 0, 0},
	{"Gray1", 3, 3, 3},
	{"Gray10", 26, 26, 26},
	{"Gray100", 255, 255, 255},
	{"Gray11", 28, 28, 28},
	{"Gray12", 31, 31, 31},
	{"Gray13", 33, 33, 33},
	{"Gray14", 36, 36, 36},
	{"Gray15", 38, 38, 38},
	{"Gray16", 41, 41, 41},
	{"Gray17", 43, 43, 43},
	{"Gray18", 46, 46, 46},
	{"Gray19", 48, 48, 48},
	{"Gray2", 5, 5, 5},
	{"Gray20", 51, 51, 51},
	{"Gray21", 54, 54, 54},
	{"Gray22", 56, 56, 56},
	{"Gray23", 59, 59, 59},
	{"Gray24", 61, 61, 61},
	{"Gray25", 64, 64, 64},
	{"Gray26", 66, 66, 66},
	{"Gray27", 69, 69, 69},
	{"Gray28", 71, 71, 71},
	{"Gray29", 74, 74, 74},
	{"Gray3", 8, 8, 8},
	{"Gray30", 77, 77, 77},
	{"Gray31", 79, 79, 79},
	{"Gray32", 82, 82, 82},
	{"Gray33", 84, 84, 84},
	{"Gray34", 87, 87, 87},
	{"Gray35", 89, 89, 89},
	{"Gray36", 92, 92, 92},
	{"Gray37", 94, 94, 94},
	{"Gray38", 97, 97, 97},
	{"Gray39", 99, 99, 99},
	{"Gray4", 10, 10, 10},
	{"Gray40", 102, 102, 102},
	{"Gray41", 105, 105, 105},
	{"Gray42", 107, 107, 107},
	{"Gray43", 110, 110, 110},
	{"Gray44", 112, 112, 112},
	{"Gray45", 115, 115, 115},
	{"Gray46", 117, 117, 117},
	{"Gray47", 120, 120, 120},
	{"Gray48", 122, 122, 122},
	{"Gray49", 125, 125, 125},
	{"Gray5", 13, 13, 13},
	{"Gray50", 127, 127, 127},
	{"Gray51", 130, 130, 130},
	{"Gray52", 133, 133, 133},
	{"Gray53", 135, 135, 135},
	{"Gray54", 138, 138, 138},
	{"Gray55", 140, 140, 140},
	{"Gray56", 143, 143, 143},
	{"Gray57", 145, 145, 145},
	{"Gray58", 148, 148, 148},
	{"Gray59", 150, 150, 150},
	{"Gray6", 15, 15, 15},
	{"Gray60", 153, 153, 153},
	{"Gray61", 156, 156, 156},
	{"Gray62", 158, 158, 158},
	{"Gray63", 161, 161, 161},
	{"Gray64", 163, 163, 163},
	{"Gray65", 166, 166, 166},
	{"Gray66", 168, 168, 168},
	{"Gray67", 171, 171, 171},
	{"Gray68", 173, 173, 173},
	{"Gray69", 176, 176, 176},
	{"Gray7", 18, 18, 18},
	{"Gray70", 179, 179, 179},
	{"Gray71", 181, 181, 181},
	{"Gray72", 184, 184, 184},
	{"Gray73", 186, 186, 186},
	{"Gray74", 189, 189, 189},
	{"Gray75", 191, 191, 191},
	{"Gray76", 194, 194, 194},
	{"Gray77", 196, 196, 196},
	{"Gray78", 199, 199, 199},
	{"Gray79", 201, 201, 201},
	{"Gray8", 20, 20, 20},
	{"Gray80", 204, 204, 204},
	{"Gray81", 207, 207, 207},
	{"Gray82", 209, 209, 209},
	{"Gray83", 212, 212, 212},
	{"Gray84", 214, 214, 214},
	{"Gray85", 217, 217, 217},
	{"Gray86", 219, 219, 219},
	{"Gray87", 222, 222, 222},
	{"Gray88", 224, 224, 224},
	{"Gray89", 227, 227, 227},
	{"Gray9", 23, 23, 23},
	{"Gray90", 229, 229, 229},
	{"Gray91", 232, 232, 232},
	{"Gray92", 235, 235, 235},
	{"Gray93", 237, 237, 237},
	{"Gray94", 240, 240, 240},
	{"Gray95", 242, 242, 242},
	{"Gray96", 245, 245, 245},
	{"Gray97", 247, 247, 247},
	{"Gray98", 250, 250, 250},
	{"Gray99", 252, 252, 252},
	{"Green", 0, 255, 0},
	{"GreenYellow", 173, 255, 47},
	{"honeydew", 240, 255, 240},
	{"HotPink", 255, 105, 180},
	{"IndianRed", 107, 57, 57},
	{"ivory", 255, 255, 240},
	{"Khaki", 179, 179, 126},
	{"lavender", 230, 230, 250},
	{"LavenderBlush", 255, 240, 245},
	{"LawnGreen", 124, 252, 0},
	{"LemonChiffon", 255, 250, 205},
	{"LightBlue", 176, 226, 255},
	{"LightCoral", 240, 128, 128},
	{"LightCyan", 224, 255, 255},
	{"LightGoldenrod", 238, 221, 130},
	{"LightGoldenrodYellow", 250, 250, 210},
	{"LightGray", 168, 168, 168},
	{"LightPink", 255, 182, 193},
	{"LightSalmon", 255, 160, 122},
	{"LightSeaGreen", 32, 178, 170},
	{"LightSkyBlue", 135, 206, 250},
	{"LightSlateBlue", 132, 112, 255},
	{"LightSlateGray", 119, 136, 153},
	{"LightSteelBlue", 124, 152, 211},
	{"LightYellow", 255, 255, 224},
	{"LimeGreen", 0, 175, 20},
	{"linen", 250, 240, 230},
	{"Magenta", 255, 0, 255},
	{"Maroon", 143, 0, 82},
	{"MediumAquamarine", 0, 147, 143},
	{"MediumBlue", 50, 50, 204},
	{"MediumForestGreen", 50, 129, 75},
	{"MediumGoldenrod", 209, 193, 102},
	{"MediumOrchid", 189, 82, 189},
	{"MediumPurple", 147, 112, 219},
	{"MediumSeaGreen", 52, 119, 102},
	{"MediumSlateBlue", 106, 106, 141},
	{"MediumSpringGreen", 35, 142, 35},
	{"MediumTurquoise", 0, 210, 210},
	{"MediumVioletRed", 213, 32, 121},
	{"MidnightBlue", 47, 47, 100},
	{"MintCream", 245, 255, 250},
	{"MistyRose", 255, 228, 225},
	{"moccasin", 255, 228, 181},
	{"NavajoWhite", 255, 222, 173},
	{"Navy", 35, 35, 117},
	{"NavyBlue", 35, 35, 117},
	{"OldLace", 253, 245, 230},
	{"OliveDrab", 107, 142, 35},
	{"Orange", 255, 135, 0},
	{"OrangeRed", 255, 69, 0},
	{"Orchid", 239, 132, 239},
	{"PaleGoldenrod", 238, 232, 170},
	{"PaleGreen", 115, 222, 120},
	{"PaleTurquoise", 175, 238, 238},
	{"PaleVioletRed", 219, 112, 147},
	{"PapayaWhip", 255, 239, 213},
	{"PeachPuff", 255, 218, 185},
	{"peru", 205, 133, 63},
	{"Pink", 255, 181, 197},
	{"Plum", 197, 72, 155},
	{"PowderBlue", 176, 224, 230},
	{"purple", 160, 32, 240},
	{"Red", 255, 0, 0},
	{"RosyBrown", 188, 143, 143},
	{"RoyalBlue", 65, 105, 225},
	{"SaddleBrown", 139, 69, 19},
	{"Salmon", 233, 150, 122},
	{"SandyBrown", 244, 164, 96},
	{"SeaGreen", 82, 149, 132},
	{"seashell", 255, 245, 238},
	{"Sienna", 150, 82, 45},
	{"SkyBlue", 114, 159, 255},
	{"SlateBlue", 126, 136, 171},
	{"SlateGray", 112, 128, 144},
	{"snow", 255, 250, 250},
	{"SpringGreen", 65, 172, 65},
	{"SteelBlue", 84, 112, 170},
	{"Tan", 222, 184, 135},
	{"Thistle", 216, 191, 216},
	{"tomato", 255, 99, 71},
	{"Transparent", 0, 0, 1},
	{"Turquoise", 25, 204, 223},
	{"Violet", 156, 62, 206},
	{"VioletRed", 243, 62, 150},
	{"Wheat", 245, 222, 179},
	{"White", 255, 255, 255},
	{"WhiteSmoke", 245, 245, 245},
	{"Yellow", 255, 255, 0},
	{"YellowGreen", 50, 216, 56}
};

#define find_sign(a) \
	i = 0;\
	for (j = 0; j < 10; j++) {\
		if ((colors_pp >= (j + 1)) && (buf_tmp[j] != ' ')) {\
			k = (strchr(a, buf_tmp[j]) - a);\
			for (l = j; l > 0; l--) {\
				k *= 92;\
			}\
			i += k;\
		}\
	}

int s_image_xpm_memcmp (char *ptr0, char *ptr1, int n)
{
	while (n--) {
		if (*ptr0 != *ptr1) {
			return 1;
		}
		ptr0++;
		ptr1++;
	}
	return 0;
}

int s_image_xpm_is (char *file)
{
	FILE *fp;
	int ret = -1;
	char magic[9];
	if ((fp = fopen(file, "r")) == NULL) {
		debugf(DCLI | DFAT, "Coult not open file (%s) for reading");
	}
	fread(magic, 1, sizeof(magic), fp);
	ret = memcmp(magic, "/* XPM */", 9);
	fclose(fp);
	return ret;
}

int s_image_xpm (char *file, s_image_t *img)
{
	int i;
	int j;
	int k;
	int l;
	int m;
	int x;
	int y;
	char *buf;
	char *buf_tmp;
	int colors = 0;
	int colors_pp = 0;
	char color_hex[10];
	unsigned int *rgba_tmp = NULL;
	struct xpm_rgb_s {
		char sign[10];
		unsigned char a;
		unsigned char r;
		unsigned char g;
		unsigned char b;
	} *rgb = NULL;
	char sign_str0[] = " .+@#$%&*=-;>,')!~{]^/(_:<[}|1234567890abcdefg"
                           "hijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ`";
	char sign_str1[] = " .XoO+@#$%&*=-;:>,<1234567890qwertyuipasdfghjk"
	                   "lzxcvbnmMNBVCZASDFGHJKLPIUYTREWQ!~^/()_`'][{}|";
	FILE *fp;

	buf = (char *) s_malloc(sizeof(char) * BUFFSIZE + 1);
	buf_tmp = buf;

	if ((fp = fopen(file, "r")) == NULL) {
		debugf(DCLI | DFAT, "Coult not open file (%s) for reading");
	}

	while (!feof(fp)) {
		fgets(buf, BUFFSIZE, fp);
		if (strncmp(buf, "/*", 2) == 0) {
			continue;
		}
		if (buf[0] != '"') {
			continue;
		}
		sscanf(buf + 1, "%d %d %d %d", &(img->w), &(img->h), &colors, &colors_pp);
		rgb = (struct xpm_rgb_s *) s_calloc(1, colors * sizeof(struct xpm_rgb_s));
		img->rgba = (unsigned int *) s_calloc(1, img->w * img->h * sizeof(unsigned int));
		rgba_tmp = img->rgba;
		break;
	}

	if ((rgb == NULL) || (rgba_tmp == NULL)) {
		debugf(DCLI | DFAT, "Not enough memory");
	}

        for (i = 0; i < colors; i++) {
		fgets(buf, BUFFSIZE, fp);
		if (strncmp(buf, "/*", 2) == 0) {
			continue;
		}
		for (j = 0; j < colors_pp; j++) {
			rgb[i].sign[j] = buf[j + 1];
		}
		rgb[i].sign[j] = '\0';
		if ((buf_tmp = strstr(buf, "c #")) != NULL) {
			sscanf(buf_tmp, "c #%s", color_hex);
			rgb[i].a = 0;
			rgb[i].r = s_image_hex2int(color_hex);
			rgb[i].g = s_image_hex2int(color_hex + 2);
			rgb[i].b = s_image_hex2int(color_hex + 4);
		} else if ((buf_tmp = strstr(buf, "c ")) != NULL) {
			char rgbname[30];
			sscanf(buf_tmp, "c %s", rgbname);
			rgbname[strlen(rgbname) - 2] = 0;
			if (strcasecmp(rgbname, "none") == 0) {
				goto color_none;
			}
			for (k = 0; k < 234; k++) {
				if (strcasecmp(rgbname, rgbRecord[k].name) == 0) {
					rgb[i].a = 0;
					rgb[i].r = rgbRecord[k].r;
					rgb[i].g = rgbRecord[k].g;
					rgb[i].b = rgbRecord[k].b;
					break;
				}
			}
		} else {
color_none:		rgb[i].a = 255;
			rgb[i].r = 0;
			rgb[i].g = 0;
			rgb[i].b = 0;
		}
	}

	for (y = 0; y < img->h; y++) {
		fgets(buf, BUFFSIZE, fp);
		if (strncmp(buf, "/*", 2) == 0) {
			continue;
		}
		buf_tmp = buf + 1;
		for (x = 0; x < img->w; x++, buf_tmp += colors_pp) {
			find_sign(sign_str0);
			m = i;
			find_sign(sign_str1);
			i = (m < i) ? m : i;
			for (; i < colors; i++) {
				if (s_image_xpm_memcmp(buf_tmp, rgb[i].sign, colors_pp) == 0) {
					*rgba_tmp |= (rgb[i].r << 0x18);
					*rgba_tmp |= (rgb[i].g << 0x10);
					*rgba_tmp |= (rgb[i].b << 0x08);
					*rgba_tmp |= (rgb[i].a << 0x00);
					rgba_tmp++;
					break;
				}
			}
		}
	}

	s_free(rgb);
	s_free(buf);
	fclose(fp);
	
	return 0;
}

#else

int s_image_xpm_is (char *file)
{
	return -1;
}

int s_image_xpm (char *file, s_image_t *img)
{
	return -1;
}

#endif
