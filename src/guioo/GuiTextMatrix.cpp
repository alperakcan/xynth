/***************************************************************************
 *   Copyright            : (c) 2007-2008 Artec Design LLC                 *
 *                        : (c) 2007-2008 Andrei Birjukov                  *
 *                        : (c) 2007-2008 Anti Sullin                      *
 *                        : (c) 2007-2008 Jaanus Sepp                      *
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
/**
 * \brief Class constructor
 *
 * Default constructor for GUI text matrix objects. Initializes the height and
 * width of the matrix.
 *
 * \param idResource Unique primitive identifier.
 * \param pFrame Pointer to a parent frame containing the primitive.
 * \param width Specifies the width of the text matrix, in pixels.
 * \param height Specifies the height of the text matrix, in pixels.
 */
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

/**
 * \brief Class destructor
 *
 * Destructor for the text matrix primitive objects.
 * Releases currently initialized font and image resources.
 */
GuiTextMatrix::~GuiTextMatrix()
{
	if(fontData) s_font_uninit(fontData);
	if(imageData) s_image_uninit(imageData);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Set font name and size
 *
 * Initializes the text matrix with the given TTF font name and size.
 * Font name maps to a TTF file name supported by Xynth engine.
 *
 * \param fontName Contains the TTF font name as a null-terminated string
 * \param fontSize Specifies the font size, in pixels
 * \return Returns true if the text matrix was initialized successfully
 */
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
/**
 * \brief Set text matrix dimensions
 *
 * Initializes the width and height of the text matrix.
 *
 * \param cols Specifies the number of columns in the text matrix.
 * \param rows Specifies the number of rows in the text matrix.
 * \return Returns true if the call was successfull.
 */
bool GuiTextMatrix::setDimensions(int cols, int rows)
{
	if (cols < 1) col_count = 1; else col_count = cols;
	if (rows < 1) row_count = 1; else row_count = rows;

	cell_width = img_width / col_count;
	cell_height = img_height / row_count;
	return true;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Initialize matrix colors
 *
 * Assigns font color parameters to the text matrix. The color values are
 * given in text format, as null-terminated strings containing hex-encoded
 * RGB channels (RRGGBB).
 *
 * \param pStaticTextColor Specifies the font color of a static text.
 * \param pInactiveFieldColor Specifies the font color of an inactive field.
 * \param pActiveFieldColor Specifies the font color of an active field.
 * \param pSelectionColor Specifies the selection foreground color.
 * \param pSelectionBgColor Specifies the selection background color.
 */
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
/**
 * \brief Set cell content
 *
 * Writes a character to the specified cell of the text matrix.
 * The character is given in the UTF-8 format and the specified cell style is
 * also applied. Redrawing is not performed automatically.
 *
 * \param row Specifies the cell row index.
 * \param col Specifies the cell column index.
 * \param boxText Points to a null-terminated string containing the cell
 * character, in UTF-8 format
 * \param cellType Specifies the cell type.
 */
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

	imageCopy(col * cell_width, row * cell_height,
			imageData, fontData->glyph.img,
			(cell_width - fontData->glyph.img->w) / 2,
			fontData->ascent - fontData->glyph.yMax - 2);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Get cell height
 *
 * \return Returns the height of the cell, in pixels.
 */
int GuiTextMatrix::getCellHeight()
{
	return cell_height;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Get column count
 *
 * \return Returns the number of columns in the text matrix.
 */
int GuiTextMatrix::getColCount()
{
	return col_count;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Get row count
 *
 * \return Returns the number of rows in the text matrix.
 */
int GuiTextMatrix::getRowCount()
{
	return row_count;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *
 */
void GuiTextMatrix::imageCopy(int destX, int destY,
								s_image_t *destImg, s_image_t *srcImg,
								int horOffset, int vertOffset)
{
	int fromRow = 0;

	if (vertOffset < 0)
		fromRow = -vertOffset;

	int toRow = srcImg->h;

	if (srcImg->h + vertOffset > cell_height)
		toRow = cell_height - vertOffset;

	for (int i = fromRow; i < toRow; i++)
	{
		int fromCol = 0;

		if (horOffset < 0)
			fromCol = -horOffset;

		int toCol = srcImg->w;

		if (srcImg->w + horOffset > cell_width)
			toCol = cell_width - horOffset;


		int destOffset = (destY + vertOffset + i) * destImg->w +
							destX + horOffset + fromCol;

		for (int j = fromCol; j < toCol; j++)
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
/**
 * \brief Painting function of the primitive interface
 *
 * Called by the parent frame whenever a primitive redraw is required.
 * \param drawSurface Pointer to a graphical surface where the primitive should
 * draw itself to.
 * \param paintArea The rectangular area of the parent frame surface that
 * should be re-painted by the primitive.
 */
void GuiTextMatrix::onPaint(GuiSurface *drawSurface, s_rect_t &paintArea)
{
	if(!isVisible) return;

	s_rect_t surfRect;
	if(s_region_rect_intersect(&paintArea, &drawArea, &surfRect)) return;

	s_rect_t relRect = { surfRect.x - drawArea.x, surfRect.y - drawArea.y, surfRect.w, surfRect.h };
	drawSurface->drawImagePart(surfRect.x, surfRect.y, &relRect, imageData);
}
