/***************************************************************************
    begin                : Tue Sep 12 2006
    copyright            : (C) 2006 - 2007 by Alper Akcan
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

#include "../lib/xynth_.h"
#include "widget.h"

#if 0
#include <libintl.h>
#define _(str) gettext(str)
#else
#define _(str) s_gettext(object->window->window, str)
#endif

void w_textbox_slide (w_object_t *object, int vertical, int horizontal, int *ytotal, int *yoffset)
{
	w_textbox_t *textbox;
	textbox = (w_textbox_t *) object->data[OBJECT_TEXTBOX];
	textbox->yoffset -= vertical;
	w_textbox_draw_(object);
	w_object_update(object, object->surface->win);
	(*ytotal) = textbox->height;
	(*yoffset) = textbox->yoffset;
}

int w_textbox_set_properties (w_object_t *object, TEXTBOX_PROPERTIES properties)
{
	w_textbox_t *textbox;
	textbox = (w_textbox_t *) object->data[OBJECT_TEXTBOX];
	textbox->properties = properties;
	return 0;
}
					
int w_textbox_set_image (w_object_t *object, unsigned int style, unsigned int rotation, unsigned int nimgs, char **imgs)
{
	return w_frame_set_image(object, style, rotation, nimgs, imgs);
}

int w_textbox_set_style (w_object_t *object, FRAME_SHAPE shape, FRAME_SHADOW shadow)
{
	return w_frame_set_style(object, shape, shadow);
}

void w_textbox_lines_uninit (w_object_t *object)
{
	w_textbox_t *textbox;
	textbox = (w_textbox_t *) object->data[OBJECT_TEXTBOX];
	while (!s_list_eol(textbox->lines, 0)) {
		s_font_glyph_t *glyph = (s_font_glyph_t *) s_list_get(textbox->lines, 0);
		s_list_remove(textbox->lines, 0);
		s_image_uninit(glyph->img);
		s_free(glyph);
	}
}

#define w_textbox_line_add(fglyph, fstr) {\
	s_font_glyph_t *glyph = (s_font_glyph_t *) s_malloc(sizeof(s_font_glyph_t));\
	s_font_set_str(font, fstr);\
	s_font_get_glyph(fglyph);\
	*glyph = fglyph->glyph;\
	s_list_add(textbox->lines, glyph, -1);\
	textbox->height += glyph->lineskip;\
	s_image_init(&(fglyph->glyph.img));\
}

void w_textbox_lines_calculate (w_object_t *object)
{
	int ptrw;
	int strw;
	int limit;
	int chars;
	char *tmp;
	char *str;
	char *ptr;
	char *ntr;
	char *ttr;
	char *strline;
	char *ptrline;
	s_font_t *font;
	w_textbox_t *textbox;
	textbox = (w_textbox_t *) object->data[OBJECT_TEXTBOX];
	if (textbox->calculate == 0) {
		return;
	}
	if (object->content->w <= 0) {
		return;
	}
	if ((ntr = _(textbox->str)) == NULL) {
		return;
	}
	w_textbox_lines_uninit(object);
	font = w_window_font_get(object->window, textbox->font);
	if (font == NULL) {
		return;
	}
	textbox->height = 0;
	s_font_set_size(font, textbox->size);
	s_font_set_rgb(font, (textbox->rgb >> 0x10) & 0xff, (textbox->rgb >> 0x8) & 0xff, textbox->rgb & 0xff);
	
	ttr = strdup(ntr);
	str = ttr;
	while (*ntr) {
		if ((ntr = strchr(str, '\n')) != NULL) {
			*ntr = '\0';
		}
		if (textbox->properties & TEXTBOX_WRAP &&
		    strlen(str) > 0) {
			strline = (char *) s_malloc(sizeof(char *) * (strlen(str) + 1));
			ptrline = (char *) s_malloc(sizeof(char *) * (strlen(str) + 1));
			memset(ptrline, 0, strlen(str) + 1);
			memset(strline, 0, strlen(str) + 1);
			for (tmp = str; tmp < str + strlen(str); tmp += (chars - 1)) {
				for (chars = 0, limit = 0; limit == 0 && chars <= strlen(tmp);) {
					snprintf(ptrline, chars, "%s", tmp);
					snprintf(strline, chars + 1, "%s", tmp);
					ptrline[chars] = '\0';
					strline[chars + 1] = '\0';
					ptrw = s_font_get_width(font, ptrline);
					strw = s_font_get_width(font, strline);
					if (object->content->w <= strw) {
					    	limit = 1;
					} else {
#if 1
						int charst = 0;
						int charsp = 0;
						while ((object->content->w - font->height) > (strw + charst)) {
							charst += font->size;
							charsp++;
						}
						if (chars + charsp < strlen(tmp)) {
							chars += charsp;
						}
#endif
						chars++;
					}
				}
				if (limit) {
					while (ptrline[0] == ' ') {
						for (ptrw = 0; ptrline[ptrw] && ptrline[ptrw + 1]; ptrw++) {
							ptrline[ptrw] = ptrline[ptrw + 1];
						}
						ptrline[ptrw] = '\0';
					}
					while (strline[0] == ' ') {
						for (strw = 0; strline[strw] && strline[strw + 1]; strw++) {
							strline[strw] = strline[strw + 1];
						}
						strline[strw] = '\0';
					}
					ptrw = strlen(ptrline);
					strw = strlen(strline);
					if (ptrline[ptrw - 1] != ' ' &&
					    strline[strw - 1] != ' ') {
					    	ptr = strdup(ptrline);
					    	for (strw = strlen(ptr) - 1; strw >= 0; strw--) {
					    		if (ptr[strw] != ' ') {
					    			chars--;
					    			ptr[strw] = '\0';
					    		} else {
					    			break;
					    		}
					    	}
					    	if (strw >= 0) {
					    		sprintf(ptrline, "%s", ptr);
					    	}
					    	s_free(ptr);
					}
					w_textbox_line_add(font, ptrline);
				} else {
					w_textbox_line_add(font, strline);
					break;
				}
			}
			s_free(strline);
			s_free(ptrline);
		} else {
			w_textbox_line_add(font, str);
		}
		if (ntr) {
			str = ntr + 1;
			ntr++;
		} else {
			break;
		}
	}
	s_free(ttr);
	textbox->calculate = 0;
}

void w_textbox_draw_ (w_object_t *object)
{
	int x;
	int y;
	int w;
	int h;
	int d;
	int line;
	w_textbox_t *textbox;
	s_font_glyph_t *glyph;
	textbox = (w_textbox_t *) object->data[OBJECT_TEXTBOX];
	if (object->surface->vbuf == NULL) {
		goto end;
	}
	w_frame_draw(object);
	if ((textbox->frame->style & FRAME_MSHAPE) == FRAME_NOFRAME) {
		memset(object->surface->matrix, 0, object->surface->width * object->surface->height);
	}
	textbox->yoffset = MAX(textbox->yoffset, object->content->h - textbox->height);
	textbox->yoffset = MIN(0, textbox->yoffset);
	for (line = 0; !s_list_eol(textbox->lines, line); line++) {
		glyph = (s_font_glyph_t *) s_list_get(textbox->lines, line);
		w = MIN(object->content->w, glyph->img->w);
		h = MIN(object->content->h, glyph->height * textbox->lines->nb_elt);
		if (!(textbox->properties & TEXTBOX_HCENTER) || object->content->w == w) {
			x = object->content->x;
		} else {
			x = (object->content->w - w) / 2;
		}
		if (!(textbox->properties & TEXTBOX_VCENTER) || object->content->h == h) {
			y = object->content->y;
		} else {
			y = (object->content->h - h) / 2;
		}
		y += glyph->lineskip * line;
		y += textbox->yoffset;
		if (!(textbox->properties & TEXTBOX_HCENTER)) {
			d = glyph->img->w - object->content->w;
			if (d > 0) {
				//x -= d;
				//w += d;
			}
		}
		s_image_get_mat(glyph->img);
		if ((textbox->frame->style & FRAME_MSHAPE) == FRAME_NOFRAME) {
			s_putmaskpart(object->surface->matrix, object->surface->width, object->surface->height,
			              x, y + glyph->size - glyph->yMax, w, h, glyph->img->w, glyph->img->h, glyph->img->mat, 0, 0);
			s_putboxpartrgb(object->surface, x, y + glyph->size - glyph->yMax, w, h,
			                glyph->img->w, glyph->img->h, glyph->img->rgba, 0, 0);
		} else { 
			s_putboxpartrgba(object->surface, x, y + glyph->size - glyph->yMax, w, h,
			                 glyph->img->w, glyph->img->h, glyph->img->rgba, 0, 0);		
		}
	}
end:	;
}

void w_textbox_draw (w_object_t *object)
{
	w_textbox_lines_calculate(object);
	w_textbox_draw_(object);
//	w_textbox_lines_uninit(object);
}

int w_textbox_set_rgb (w_object_t *object, int r, int g, int b)
{
	w_textbox_t *textbox;
	textbox = (w_textbox_t *) object->data[OBJECT_TEXTBOX];
	textbox->rgb = ((r & 0xff) << 0x10) | ((g & 0xff) << 0x8) | (b & 0xff);
	textbox->calculate = 1;
	w_textbox_draw(object);
	return 0;
}

int w_textbox_set_size (w_object_t *object, int size)
{
	w_textbox_t *textbox;
	textbox = (w_textbox_t *) object->data[OBJECT_TEXTBOX];
	textbox->size = size;
	textbox->calculate = 1;
	w_textbox_draw(object);
	return 0;
}

int w_textbox_set_str (w_object_t *object, char *str)
{
	w_textbox_t *textbox;
	textbox = (w_textbox_t *) object->data[OBJECT_TEXTBOX];
	if (str == NULL) {
		str = strdup("");
	} else {
		str = strdup(str);
	}
	s_free(textbox->str);
	textbox->str = str;
	textbox->calculate = 1;
	w_textbox_draw(object);
	return 0;
}

void w_textbox_geometry (w_object_t *object)
{
	w_textbox_t *textbox;
	textbox = (w_textbox_t *) object->data[OBJECT_TEXTBOX];
	w_frame_geometry(object);
	textbox->calculate = 1;
	w_textbox_draw(object);
}

int w_textbox_init (w_window_t *window, w_textbox_t **textbox, w_object_t *parent)
{
	(*textbox) = (w_textbox_t *) s_malloc(sizeof(w_textbox_t));

	if (s_list_init(&((*textbox)->lines))) {
		goto err0;
	}
	if (w_frame_init(window, &((*textbox)->frame), FRAME_PANEL | FRAME_RAISED, parent)) {
		goto err1;
	}
	
	(*textbox)->str = NULL;
	(*textbox)->size = 10;
	(*textbox)->rgb = 0;
	(*textbox)->font = strdup("arial.ttf");
	(*textbox)->properties = TEXTBOX_VCENTER | TEXTBOX_HCENTER;
	(*textbox)->height = 0;
	(*textbox)->yoffset = 0;
	(*textbox)->calculate = 0;

	(*textbox)->object = (*textbox)->frame->object;
	(*textbox)->object->type = OBJECT_TEXTBOX;
	(*textbox)->object->draw = w_textbox_draw;
	(*textbox)->object->geometry = w_textbox_geometry;
	(*textbox)->object->destroy = w_textbox_uninit;
	(*textbox)->object->data[OBJECT_TEXTBOX] = *textbox;

	w_window_font_add(window, (*textbox)->font);

	return 0;
err1:	s_list_uninit((*textbox)->lines);
err0:	return -1;
}

void w_textbox_uninit (w_object_t *object)
{
	w_textbox_t *textbox;
	textbox = (w_textbox_t *) object->data[OBJECT_TEXTBOX];
	w_textbox_lines_uninit(object);
	s_free(textbox->str);
	s_free(textbox->font);
	s_list_uninit(textbox->lines);
	w_frame_uninit(object);
	s_free(textbox);
}
