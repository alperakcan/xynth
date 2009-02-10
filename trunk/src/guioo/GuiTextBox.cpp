/***************************************************************************
 *   Copyright            : (c) 2007-2008 Artec Design LLC                 *
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
#include "GuiTextBox.h"

#define REGULAR_FONT	"trebuc.ttf"
#define BOLD_FONT	"trebucbd.ttf"
#define ITALIC_FONT	"trebucit.ttf"

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Class constructor
 *
 * Default constructor for GUI text box objects. Initializes vertical alignment
 * to top and horizontal alignment to left.
 *
 * \param idResource Uniquie primitive identifier.
 * \param pFrame Pointer to a parent frame containing the primitive.
 */
GuiTextBox::GuiTextBox(int idResource, GuiFrame* pFrame)
		: GuiPrimitive(idResource, pFrame)
{
	fontData = NULL;
	horzAlign = LEFT;
	vertAlign = TOP;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Class destructor
 *
 * Destructor for the text box primitive objects.
 * Releases currently initialized font resources, if any.
 */
GuiTextBox::~GuiTextBox()
{
	if(fontData) s_font_uninit(fontData);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Set font name and size
 *
 * Initializes the text box with given TTF font name and size. Font name maps
 * to a TTF file name supported by Xynth engine.
 *
 * \param fontName Contains the TTF font name as a null-terminated string
 * \param fontSize Specifies the font size, in pixels
 * \return Returns true if the text box was initialized successfully
 */
bool GuiTextBox::setFont(const char* fontName, int fontSize)
{
	lock();

	if(fontData) s_font_uninit(fontData);
	fontData = NULL;

	if(s_font_init(&fontData, (char*)fontName))
	{
		fontData = NULL;
		unlock();
		return false;
	}

	s_font_set_size(fontData, fontSize);
	unlock();
	return true;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Set text color
 *
 * Initializes the font color that is used to render the text box. Note that
 * the function call does not include actual rendering. To pre-render the text
 * box, call setText() method.
 *
 * \param pColor Specifies the font color as hex RGB values encoded in a
 * null-terminated character string. String format is RRGGBB.
 */
void GuiTextBox::setColor(const char *pColor)
{
	lock();

	if(fontData != NULL)
	{
		RGB_COLOR rgb = GuiSurface::stringToRGB(pColor);
		s_font_set_rgb(fontData, rgb.red, rgb.green, rgb.blue);
	}

	unlock();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Set text color
 *
 * Initializes the font color that is used to render the text box. Note that
 * the function call does not include actual rendering. To pre-render the text
 * box, call setText() method.
 *
 * \param pColor Specifies the font color as RGB values in RGB_COLOR structure.
 */
void GuiTextBox::setColor(RGB_COLOR *pColor)
{
	lock();

	if(fontData != NULL)
	{
		s_font_set_rgb(fontData, pColor->red, pColor->green, pColor->blue);
	}

	unlock();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Set text string
 *
 * Initializes and pre-renders a text box with the given text string.
 *
 * \param boxText Null-terminated text string in UTF-8 format
 */
void GuiTextBox::setText(const char* boxText)
{
	lock();

	if(fontData != NULL)
	{
		s_font_set_str(fontData, (char*)boxText);
		s_font_get_glyph(fontData);
	}

	unlock();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Set formatted text
 *
 * Initializes text box with parameters given by the FormattedTextLine
 * structure.
 *
 * \param pText Pointer to a data structure containing the text string, as well
 * as the font size, color, style, and alignment.
 * \return Returns true if the text was pre-rendered successfully
 */
bool GuiTextBox::setText(FormattedTextLine *pText)
{
	lock();

	if(fontData) s_font_uninit(fontData);

	fontData = NULL;

	const char *fontName = NULL;
	switch(pText->fontStyle)
	{
	case REGULAR:
		fontName = REGULAR_FONT;
		break;

	case BOLD:
		fontName = BOLD_FONT;
		break;

	case ITALIC:
		fontName = ITALIC_FONT;
		break;
	}

	horzAlign = pText->textAlign;
	vertAlign = VCENTER;

	if(s_font_init(&fontData, fontName))
	{
		fontData = NULL;
		unlock();
		return false;
	}

	s_font_set_size(fontData, pText->fontSize);
	s_font_set_rgb(fontData, pText->fontColor.red,
			pText->fontColor.green, pText->fontColor.blue);

	s_font_set_str(fontData, (char*)pText->lineText.c_str());

	s_font_get_glyph(fontData);

	unlock();
	return true;
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
void GuiTextBox::onPaint(GuiSurface *drawSurface, s_rect_t &paintArea)
{
	lock();

	if(!isVisible || fontData == NULL)
	{
		unlock();
		return;
	}

	int ox = 0, oy = 0;
	switch(horzAlign)
	{
	case LEFT:
		ox = 0;
		break;
	case RIGHT:
		ox = drawArea.w - fontData->glyph.img->w;
		break;
	case HCENTER:
		ox = (drawArea.w - fontData->glyph.img->w) / 2;
		break;
	}

	switch(vertAlign)
	{
	case TOP:
		oy = 0;
		break;
	case BOTTOM:
		oy = drawArea.h - fontData->glyph.img->h;
		break;
	case VCENTER:
		oy = (drawArea.h - fontData->glyph.img->h) / 2;
		break;
	case VNORMAL:
		oy = fontData->ascent - fontData->glyph.yMax + 1;
		break;
	}

	s_rect_t textArea = { drawArea.x + ox, drawArea.y + oy,
			drawArea.w - ox, drawArea.h - oy};

	s_rect_t surfRect;
	if(s_region_rect_intersect(&paintArea, &textArea, &surfRect))
	{
		unlock();
		return;
	}

	s_rect_t sourceRect = { surfRect.x - textArea.x, surfRect.y - textArea.y,
						surfRect.w, surfRect.h };

	drawSurface->drawImagePart(surfRect.x, surfRect.y,
								&sourceRect, fontData->glyph.img);

	unlock();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Set text alignment
 *
 * Sets the vertical and horizontal alignment of a text string inside the
 * given text box.
 *
 * \param horz Specifies the horizontal text alignment (left, rigth, centered)
 * \param vert Specifies the vertical text alignment (top, bottom, centered)
 */
void GuiTextBox::setAlignment(TextHAlignment horz, TextVAlignment vert)
{
	lock();

	horzAlign = horz;
	vertAlign = vert;

	unlock();
}


////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Retrieve rendered text dimensions
 *
 * Calculates the dimensions of the specified text when rendered. Given method
 * may appear useful to estimate the size of a text box before drawing on the
 * screen.
 *
 * \param rect Reference to a rectangle structure that receives width and
 * height of the rendered text
 */
void GuiTextBox::getRequiredSize(s_rect_t &rect)
{
	lock();

	rect.x = rect.y = rect.w = rect.h = 0;
	if(fontData && fontData->glyph.img)
	{
		rect.w = fontData->glyph.img->w;
		rect.h = fontData->glyph.img->h;
	}

	unlock();
}
