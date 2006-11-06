/***************************************************************************
    begin                : Sat Feb 28 2004
    copyright            : (C) 2004 - 2006 by Alper Akcan
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

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

struct s_font_ft_s {
        FT_Face face;
	FT_Library library;
};

int s_font_init (s_font_t **font, char *name)
{
        char *name_;
        
	(*font) = (s_font_t *) s_calloc(1, sizeof(s_font_t));
	(*font)->ft = (s_font_ft_t *) s_calloc(1, sizeof(s_font_ft_t));
	if (FT_Init_FreeType(&((*font)->ft->library))) {
		debugf(0, "FT_Init_FreeType");
		goto err0;
	}
        name_ = (char *) s_malloc(strlen(FONTDIR) + strlen(name) + 2);
        sprintf(name_, "%s/%s", FONTDIR, name);
	if (FT_New_Face((*font)->ft->library, name_, 0, &((*font)->ft->face))) {
		debugf(0, "FT_New_Face (%s)", name_);
		goto err1;
	}
	/* this should do the stuff for unicode, who knows ;)
	 */
	{
		FT_Error error;
		error = FT_Select_Charmap((*font)->ft->face, ft_encoding_unicode);
		if ((*font)->ft->face->charmap == NULL ||
		    (*font)->ft->face->charmap->encoding != ft_encoding_unicode) {
		    	if (error) {
				debugf(0, "Unicode charmap not available for this font(%s). Very bad!", name_);
		    	}
		}
	}
	if (s_image_init(&((*font)->glyph.img))) {
		goto err2;
	}
	s_free(name_);
	(*font)->rgb = 0;
	return 0;

err2:	FT_Done_Face((*font)->ft->face);
err1:	FT_Done_FreeType((*font)->ft->library);
	s_free(name_);
err0:	s_free((*font)->ft);
	s_free(*font);
	return -1;
}

int s_font_uninit (s_font_t *font)
{
	if (font == NULL) {
		return 0;
	}
        s_image_uninit(font->glyph.img);
	FT_Done_Face(font->ft->face);
	FT_Done_FreeType(font->ft->library);
	s_free(font->str);
	s_free(font->ft);
	s_free(font);
	return 0;
}

int s_font_set_size (s_font_t *font, int size)
{
	FT_Fixed scale;
	
	if (font->size == size) {
		goto end;
	}
	if (FT_Set_Pixel_Sizes(font->ft->face, 0, size)) {
		debugf(0, "FT_Set_Pixel_Sizes");
		return -1;
	}
	#define FT_FLOOR(X) ((X & -64) / 64)
	#define FT_CEIL(X)  (((X + 63) & -64) / 64)
	scale = font->ft->face->size->metrics.y_scale;
	font->size = size;
	font->ascent  = FT_CEIL(FT_MulFix(font->ft->face->ascender, scale));
	font->descent = FT_CEIL(FT_MulFix(font->ft->face->descender, scale));
	font->height  = font->ascent - font->descent + /* baseline */ 1;
	font->lineskip = FT_CEIL(FT_MulFix(font->ft->face->height, scale));
	font->underline_offset = FT_FLOOR(FT_MulFix(font->ft->face->underline_position, scale));
	font->underline_height = FT_FLOOR(FT_MulFix(font->ft->face->underline_thickness, scale));

#if 0
	printf("\nfont:\n");
	printf("scale   : %d\n", (int) scale);
	printf("size    : %d\n", font->size);
	printf("ascent  : %d\n", font->ascent);
	printf("descent : %d\n", font->descent);
	printf("height  : %d\n", font->height);
	printf("underline_offset: %d\n", font->underline_offset);
	printf("underline_height: %d\n", font->underline_height);
#endif

end:	return 0;
}

int s_font_set_str (s_font_t *font, char *str)
{
	s_free(font->str);
	if (str) { 
		font->str = strdup(str);
	} else {
		font->str = NULL;
	}
	return 0;
}

int s_font_set_rgb (s_font_t *font, int r, int g, int b)
{
	font->rgb = ((r & 0xFF) << 0x10) | ((g & 0xFF) << 0x08) | (b & 0xFF);
	return 0;
}

static inline unsigned short * s_font_utf8_to_unicode (unsigned short *unicode, char *utf8, int l)
{
	int i;
	int j;
	unsigned short ch;

	for (i = 0, j = 0; i < l; ++i, ++j) {
		ch = ((const unsigned char *) utf8)[i];
		if (ch >= 0xF0) {
			ch  =  (unsigned short) (utf8[i] & 0x07) << 18;
			ch |=  (unsigned short) (utf8[++i] & 0x3F) << 12;
			ch |=  (unsigned short) (utf8[++i] & 0x3F) << 6;
			ch |=  (unsigned short) (utf8[++i] & 0x3F);
		} else if (ch >= 0xE0) {
			ch  =  (unsigned short) (utf8[i] & 0x0F) << 12;
			ch |=  (unsigned short) (utf8[++i] & 0x3F) << 6;
			ch |=  (unsigned short) (utf8[++i] & 0x3F);
		} else if (ch >= 0xC0) {
			ch  =  (unsigned short) (utf8[i] & 0x1F) << 6;
			ch |=  (unsigned short) (utf8[++i] & 0x3F);
		}
		unicode[j] = ch;
	}
	unicode[j] = 0;

	return unicode;
}

int s_font_get_glyph (s_font_t *font)
{
        int n;
	int x;
	int y;
	int pen_x;
	int pen_y;
	int error;
	int num_chars;
	int xmin;
	int xmax;
	unsigned short *unicode;
	FT_Bool use_kerning;
	FT_UInt glyph_index;
	FT_UInt num_glyphs;
	FT_UInt previous;
	FT_Vector *pos;
        FT_Glyph *glyphs;
        FT_Glyph *images;
	FT_Vector *pens;
	FT_BBox bbox;
	
	num_chars = strlen(font->str);	
	pos = (FT_Vector *) s_malloc(sizeof(FT_Vector) * num_chars);
	glyphs = (FT_Glyph *) s_malloc(sizeof(FT_Glyph) * num_chars);
	images = (FT_Glyph *) s_malloc(sizeof(FT_Glyph) * num_chars);
	pens = (FT_Vector *) s_malloc(sizeof(FT_Vector) * num_chars);

	pen_x = 0;
	pen_y = 0;
	num_glyphs = 0;
	use_kerning = FT_HAS_KERNING(font->ft->face);
	previous = 0;
	
	unicode = (unsigned short *) s_malloc(sizeof(unsigned short) * (num_chars + 1));
	s_font_utf8_to_unicode(unicode, font->str, num_chars);
	for (num_chars = 0; unicode[num_chars] != 0; num_chars++);
	
	for (n = 0; n < num_chars; n++) {
		glyph_index = FT_Get_Char_Index(font->ft->face, unicode[n]);
		if (glyph_index == 0) {
//			debugf(0, "Couldnt get glyph index for char: %c[%d]", font->str[n], font->str[n]);
			continue;
		}
		if (use_kerning && previous && glyph_index) {
			FT_Vector delta;
			if (FT_Get_Kerning(font->ft->face, previous, glyph_index, FT_KERNING_DEFAULT, &delta)) {
				debugf(0, "Couldnt get kerning data for char: %c[%d]", font->str[n], font->str[n]);
			}
			pen_x += delta.x >> 6;
		}
		pos[num_glyphs].x = pen_x;
		pos[num_glyphs].y = pen_y;
		error = FT_Load_Glyph(font->ft->face, glyph_index, FT_LOAD_DEFAULT);
		if (error) {
			debugf(0, "FT_Load_Glyph (%d, 0x%x)", error, error);
			continue;
		}
		error = FT_Get_Glyph(font->ft->face->glyph, &glyphs[num_glyphs]);
		if (error) {
			debugf(0, "FT_Get_Glyph (%d, 0x%x)", error, error);
			continue;
		}
		pen_x += font->ft->face->glyph->advance.x >> 6;
		previous = glyph_index;
		num_glyphs++;
	}
	s_free(unicode);

	bbox.xMin = bbox.yMin =  32000;
	bbox.xMax = bbox.yMax = -32000;
	for (n = 0; n < num_glyphs; n++) {
		FT_BBox glyph_bbox;
		FT_Glyph_Get_CBox(glyphs[n], ft_glyph_bbox_pixels, &glyph_bbox);
		glyph_bbox.xMin += pos[n].x;
		glyph_bbox.xMax += pos[n].x;
		glyph_bbox.yMin += pos[n].y;
		glyph_bbox.yMax += pos[n].y;
		if (glyph_bbox.xMin < bbox.xMin) {
			bbox.xMin = glyph_bbox.xMin;
		}
		if (glyph_bbox.yMin < bbox.yMin) {
			bbox.yMin = glyph_bbox.yMin;
		}
		if (glyph_bbox.xMax > bbox.xMax) {
			bbox.xMax = glyph_bbox.xMax;
		}
		if (glyph_bbox.yMax > bbox.yMax) {
			bbox.yMax = glyph_bbox.yMax;
		}
	}
	xmin =  3200;
	xmax = -3200;
	for (n = 0; n < num_glyphs; n++) {
		images[n] = glyphs[n];
		pens[n].x = 0 + pos[n].x;
		pens[n].y = 0 + pos[n].y;
		error = FT_Glyph_To_Bitmap(&images[n], ft_render_mode_normal, &pens[n], 1);
		if (!error) {
			FT_BitmapGlyph bit = (FT_BitmapGlyph) images[n];
			x = pens[n].x;
			xmin = MIN(xmin, x);
			xmax = MAX(xmax, x + bit->bitmap.width);
		}
	}
	if (xmin > xmax) {
		xmin = 0;
		xmax = 0;
	}
	if (bbox.xMin > bbox.xMax) {                                  
		bbox.xMin = 0;
		bbox.yMin = 0;
		bbox.xMax = 0;
		bbox.yMax = 0;
	}
	font->glyph.img->w = xmax - xmin;
	font->glyph.img->h = bbox.yMax - bbox.yMin;
	font->glyph.yMin = bbox.yMin;
	font->glyph.yMax = bbox.yMax;
	font->glyph.height = font->height;
	font->glyph.lineskip = font->lineskip;
	font->glyph.size = font->size;
	
	s_free(font->glyph.img->rgba);
	font->glyph.img->rgba  = (unsigned int *) s_calloc(font->glyph.img->w * font->glyph.img->h, sizeof(unsigned int *));

	n = font->glyph.img->w * font->glyph.img->h;
	while (n--) {
		font->glyph.img->rgba[n] = 255;
	}
	
	for (n = 0; n < num_glyphs; n++) {
		int i;
		int j;
		FT_BitmapGlyph bit = (FT_BitmapGlyph) images[n];
		x = pens[n].x;
		y = (pens[n].y - bit->top + font->glyph.img->h) - (font->glyph.img->h - font->glyph.yMax);
		for (i = 0; i < bit->bitmap.rows; i++) {
			for (j = 0; j < bit->bitmap.width; j++) {
				if (*(bit->bitmap.buffer + i * bit->bitmap.pitch + j)) {
					*(font->glyph.img->rgba + j + x + ((i + y) * font->glyph.img->w)) = ((font->rgb << 8) & 0xFFFFFF00) | (~*(bit->bitmap.buffer + i * bit->bitmap.pitch + j) & 0xFF);
				}
			}
		}
		FT_Done_Glyph(images[n]);
	}

	s_free(pos);
	s_free(pens);
	s_free(glyphs);
	s_free(images);
	return 0;
}

int s_font_get_width (s_font_t *font, char *str)
{
	int w;
	char *ptr = NULL;
	if (font->str)
		ptr = strdup(font->str);
	s_font_set_str(font, str);
	s_font_get_glyph(font);
	w = font->glyph.img->w;
	s_font_set_str(font, ptr);
	s_free(ptr);
	return w;
}
