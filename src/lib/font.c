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
	if (s_image_init(&((*font)->img))) {
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
        s_image_uninit(font->img);
	FT_Done_Face(font->ft->face);
	FT_Done_FreeType(font->ft->library);
	s_free(font->str);
	s_free(font->ft);
	s_free(font);
	return 0;
}

int s_font_set_size (s_font_t *font, int size)
{
	if (font->size == size) {
		goto end;
	}
	if (FT_Set_Pixel_Sizes(font->ft->face, 0, size)) {
		debugf(0, "FT_Set_Pixel_Sizes");
		return -1;
	}
	font->size = size;
	font->ascender = font->ft->face->size->metrics.ascender / 64;
	font->descender = font->ft->face->size->metrics.descender / 64;
	font->height = font->ft->face->size->metrics.height / 64;
	font->max_advance = font->ft->face->size->metrics.max_advance / 64;

#if 0	
	printf("%d\n", size);
	printf("%d\n", font->ft->face->size->metrics.x_ppem);
	printf("%d\n", font->ft->face->size->metrics.y_ppem);
	printf("%ld\n", font->ft->face->size->metrics.ascender / 64);
	printf("%ld\n", font->ft->face->size->metrics.descender / 64);
	printf("%ld\n", font->ft->face->size->metrics.height / 64);
	printf("%ld\n", font->ft->face->size->metrics.max_advance / 64);
#endif

end:	return 0;
}

int s_font_set_str (s_font_t *font, char *str)
{
	s_free(font->str);
	font->str = strdup(str);
	return 0;
}

int s_font_set_rgb (s_font_t *font, int r, int g, int b)
{
	font->rgb = ((r & 0xFF) << 0x10) | ((g & 0xFF) << 0x08) | (b & 0xFF);
	return 0;
}

int s_font_get_glyph (s_font_t *font)
{
        int n;
	int pen_x;
	int pen_y;
	int error;
        int num_chars;
	FT_Bool use_kerning;
	FT_UInt glyph_index;
	FT_UInt num_glyphs;
	FT_UInt previous;
	FT_Vector *pos;
        FT_Glyph *glyphs;
	FT_BBox bbox;
	
	num_chars = strlen(font->str);	
	pos = (FT_Vector *) s_malloc(sizeof(FT_Vector) * num_chars);
	glyphs = (FT_Glyph *) s_malloc(sizeof(FT_Glyph) * num_chars);

	pen_x = 0;
	pen_y = 0;
	num_glyphs = 0;
	use_kerning = FT_HAS_KERNING(font->ft->face);
	previous = 0;

	for (n = 0; n < num_chars; n++) {
		glyph_index = FT_Get_Char_Index(font->ft->face, font->str[n]);
		if (glyph_index == 0) {
			debugf(0, "FT_Get_Char_Index");
		}
		if (use_kerning && previous && glyph_index) {
			FT_Vector delta;
			if (FT_Get_Kerning(font->ft->face, previous, glyph_index, FT_KERNING_DEFAULT, &delta)) {
				debugf(0, "FT_Get_Kerning");
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
	if (bbox.xMin > bbox.xMax) {                                  
		bbox.xMin = 0;
		bbox.yMin = 0;
		bbox.xMax = 0;
		bbox.yMax = 0;
	}

	font->img->w = bbox.xMax - bbox.xMin + 3;
	font->img->h = bbox.yMax - bbox.yMin + 3;
	font->yMin = bbox.yMin;
	font->yMax = bbox.yMax;

	s_free(font->img->rgba);
	font->img->rgba  = (unsigned int *) s_calloc(1, font->img->w * font->img->h * sizeof(unsigned int *));

	n = font->img->w * font->img->h;
	while (n--) {
		font->img->rgba[n] = 255;
	}
	
	for (n = 0; n < num_glyphs; n++) {
		FT_Glyph image;
		FT_Vector pen;
		image = glyphs[n];
		pen.x = 0 + pos[n].x;
		pen.y = 0 + pos[n].y;
		error = FT_Glyph_To_Bitmap(&image, ft_render_mode_normal, &pen, 1);
		if (!error) {
			int i;
			int j;
			int x;
			int y;
			FT_BitmapGlyph bit = (FT_BitmapGlyph) image;
			x = pen.x;
			y = (pen.y - (bit->top - font->img->h)) - (font->img->h - font->yMax);
			for (i = 0; i < bit->bitmap.rows; i++) {
				for (j = 0; j < bit->bitmap.width; j++) {
					if (*(bit->bitmap.buffer + i * bit->bitmap.pitch + j)) {
						*(font->img->rgba + j + x + ((i + y) * font->img->w)) = ((font->rgb << 8) & 0xFFFFFF00) | (~*(bit->bitmap.buffer + i * bit->bitmap.pitch + j) & 0xFF);
					}
				}
			}
			FT_Done_Glyph(image);
		} else {
			debugf(0, "FT_Glyph_To_Bitmap (%d, 0x%x)", error, error);
		}
	}

	s_free(pos);
	s_free(glyphs);
	return 0;
}
