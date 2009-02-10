/***************************************************************************
 *   Copyright            : (c) 2007-2008 Artec Design LLC                 *
 *                        : (c) 2008 Jaanus Sepp
 *                        : (c) 2007-2008 Andrei Birjukov                  *
 *                        : (c) 2007-2008 Anti Sullin                      *
 *   Contact              : andrei.birjukov at artecdesign dot ee          *
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#include <stdio.h>
#include "GuiTextMatrix.h"

#define SELECTION_BAR_WIDTH  2

////////////////////////////////////////////////////////////////////////////////
//

GuiTextMatrix::GuiTextMatrix(int idResource, GuiFrame* pFrame,
		int width, int height) : GuiPrimitive(idResource, pFrame)
{
	fontData = NULL;
	s_image_init(&imageData);
	img_width = width;
	img_height = height;
	imageData->rgba = (unsigned int *)s_calloc(img_width * img_height, sizeof(unsigned int *));
	imageData->x = 0;
	imageData->y = 0;
	imageData->w = img_width;
	imageData->h = img_height;
}

GuiTextMatrix::~GuiTextMatrix()
{
	if(fontData) s_font_uninit(fontData);
	if(imageData) s_image_uninit(imageData);
}

////////////////////////////////////////////////////////////////////////////////
//
bool GuiTextMatrix::setFont(const char* fontName, int fontSize)
{
	if(fontData) s_font_uninit(fontData);
	fontData = NULL;

	if(s_font_init(&fontData, (char*)fontName))
	{
		fontData = NULL;
		return false;
	}

	s_font_set_size(fontData, fontSize);
	return true;
}

////////////////////////////////////////////////////////////////////////////////
//
bool GuiTextMatrix::setDimensions(int cols, int rows)
{
	if (cols < 1) col_count = 1; else col_count = cols;
	if (rows < 1) row_count = 1; else row_count = rows;

	cell_width = img_width / col_count;
	cell_height = img_height / row_count;
	return true;
}

////////////////////////////////////////////////////////////////////////////////
//

void GuiTextMatrix::setColor(const char *pStaticTextColor,
							const char *pInactiveFieldColor,
							const char *pActiveFieldColor,
							const char *pSelectionColor,
							const char *pSelectionBgColor)
{
	if (pStaticTextColor != NULL)
		staticTextColor = GuiSurface::stringToRGB(pStaticTextColor);
	else
	{
		staticTextColor.red = 255;
		staticTextColor.green = 255;
		staticTextColor.blue = 255;
	}


	if (pInactiveFieldColor != NULL)
		inactiveFieldColor = GuiSurface::stringToRGB(pInactiveFieldColor);
	else
		inactiveFieldColor = staticTextColor;

	if (pActiveFieldColor != NULL)
		activeFieldColor = GuiSurface::stringToRGB(pActiveFieldColor);
	else
		activeFieldColor = inactiveFieldColor;

	if (pSelectionColor != NULL)
		selectionColor = GuiSurface::stringToRGB(pSelectionColor);
	else
	{
		selectionColor.red = 0;
		selectionColor.green = 0;
		selectionColor.blue = 0;
	}

	RGB_COLOR tmpColor;
	if (pSelectionBgColor != NULL)
		tmpColor = GuiSurface::stringToRGB(pSelectionBgColor);
	else
		tmpColor = activeFieldColor;

	selectionBgColor = (tmpColor.red << 0x18) |
						(tmpColor.green << 0x10) |
						(tmpColor.blue << 0x08);
}

////////////////////////////////////////////////////////////////////////////////
//

void GuiTextMatrix::setCell(size_t row, size_t col,
				const char* boxText, CellType cellType)
{
	if(fontData == NULL) return;

	switch (cellType)
	{
	case CELL_STATIC_TEXT:
    	s_font_set_rgb(fontData, staticTextColor.red,
						staticTextColor.green, staticTextColor.blue);
		break;
	case CELL_INACTIVE_FIELD:
		s_font_set_rgb(fontData, inactiveFieldColor.red,
						inactiveFieldColor.green, inactiveFieldColor.blue);
		break;
	case CELL_ACTIVE_FIELD:
	case CELL_PRE_SELECTION:
	case CELL_POST_SELECTION:
		s_font_set_rgb(fontData, activeFieldColor.red,
								activeFieldColor.green, activeFieldColor.blue);
		break;
	case CELL_FULL_SELECTION:
		s_font_set_rgb(fontData, selectionColor.red,
						selectionColor.green, selectionColor.blue);
		break;
	}

	s_font_set_str(fontData, (char*)boxText);
	s_font_get_glyph(fontData);

	for (int i = 0; i < cell_height; i++)
	{
		int destOffset = (row * cell_height + i) * imageData->w + col * cell_width;
		for (int j = 0; j < cell_width; j++)
		{
			switch (cellType)
			{
			case CELL_FULL_SELECTION:
				imageData->rgba[destOffset++] = selectionBgColor;
				break;

			case CELL_PRE_SELECTION:
				if (j < SELECTION_BAR_WIDTH)
					imageData->rgba[destOffset++] = selectionBgColor;
				else
					imageData->rgba[destOffset++] = 0xFF;
				break;

			case CELL_POST_SELECTION:
				if (j >= cell_width - SELECTION_BAR_WIDTH)
					imageData->rgba[destOffset++] = selectionBgColor;
				else
					imageData->rgba[destOffset++] = 0xFF;
				break;

			default:
				imageData->rgba[destOffset++] = 0xFF;
				break;
			}
		}
	}

	imageCopy(col * cell_width + ((cell_width - fontData->glyph.img->w) / 2),
			row * cell_height,
			imageData, fontData->glyph.img,
			fontData->ascent - fontData->glyph.yMax - 2);
         //   (CELL_HEIGHT + fontData->ascent + fontData->descent) / 2 - fontData->glyph.yMax + 1);
}

////////////////////////////////////////////////////////////////////////////////
//

int GuiTextMatrix::getCellHeight()
{
	return cell_height;
}

////////////////////////////////////////////////////////////////////////////////
//

int GuiTextMatrix::getColCount()
{
	return col_count;
}

////////////////////////////////////////////////////////////////////////////////
//

int GuiTextMatrix::getRowCount()
{
	return row_count;
}

////////////////////////////////////////////////////////////////////////////////
//

void GuiTextMatrix::imageCopy(int destX, int destY,
								s_image_t *destImg, s_image_t *srcImg,
								int vertOffset)
{
	for (int i = 0; i < srcImg->h; i++)
	{
		int destOffset = (destY + i + vertOffset) * destImg->w + destX;
		for (int j = 0; j < srcImg->w; j++)
		{
			int srcpixel = srcImg->w * i + j;

			if (destImg->rgba[destOffset] == 0xFF)
				destImg->rgba[destOffset++] = srcImg->rgba[srcpixel];
			else
			{
				unsigned char a = (unsigned char)(srcImg->rgba[srcpixel]);
				if (a != 0xFF)
				{
					unsigned char sr = (unsigned char)(srcImg->rgba[srcpixel] >> 0x18);
					unsigned char sg = (unsigned char)(srcImg->rgba[srcpixel] >> 0x10);
					unsigned char sb = (unsigned char)(srcImg->rgba[srcpixel] >> 0x08);

					unsigned char dr = sr;
					unsigned char dg = sr;
					unsigned char db = sr;

					if (a != 0x00)
					{
						dr = (unsigned char)(destImg->rgba[destOffset] >> 0x18);
						dg = (unsigned char)(destImg->rgba[destOffset] >> 0x10);
						db = (unsigned char)(destImg->rgba[destOffset] >> 0x08);
						dr = (a * (dr - sr) >> 8) + sr;
						dg = (a * (dg - sg) >> 8) + sg;
						db = (a * (db - sb) >> 8) + sb;
					}

					destImg->rgba[destOffset++] =
						(dr << 0x18) | (dg << 0x10) | (db << 0x08);
				}
				else
					destOffset++;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
//

void GuiTextMatrix::onPaint(GuiSurface *drawSurface, s_rect_t &paintArea)
{
	if(!isVisible) return;

	s_rect_t surfRect;
	if(s_region_rect_intersect(&paintArea, &drawArea, &surfRect)) return;

	s_rect_t relRect = { surfRect.x - drawArea.x, surfRect.y - drawArea.y, surfRect.w, surfRect.h };
	drawSurface->drawImagePart(surfRect.x, surfRect.y, &relRect, imageData);
}

////////////////////////////////////////////////////////////////////////////////
//
