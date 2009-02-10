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
#include "GuiMultiTextBox.h"

//#define DEBUG

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief
 *
 * \return
 */
GuiMultiTextBox::GuiMultiTextBox(int idResource, GuiFrame* pFrame)
					: GuiPrimitive(idResource, pFrame)
{
	isRendered = false;
	fontData = NULL;
	numLines = 0;

	fontSize = 0;
	fontColor.red = fontColor.green = fontColor.blue = 0;

	horzAlign = LEFT;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief
 *
 * \return
 */
GuiMultiTextBox::~GuiMultiTextBox()
{
	if(fontData != NULL)
	{
		for(int i=0; i<numLines; i++)
		{
			if(fontData[i]) s_font_uninit(fontData[i]);
		}

		s_free(fontData);
	}
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief
 *
 * \return
 */
void GuiMultiTextBox::setFont(const char* fontName, int fontSize)
{
	lock();

	this->fontName = fontName;
	this->fontSize = fontSize;
	isRendered = false;

	unlock();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief
 *
 * \return
 */
void GuiMultiTextBox::setColor(const char *pColor)
{
	lock();

	fontColor = GuiSurface::stringToRGB(pColor);
	isRendered = false;

	unlock();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief
 *
 * \return
 */
void GuiMultiTextBox::setText(const char* boxText)
{
	lock();

	bool space = false;
	std::string word;

	for(size_t i = 0; boxText[i] != 0; i ++)
	{
		switch(boxText[i])
		{
		case ' ':
		case '\n':
		case '\r':
		case '\t':

			if(!space)
			{
				fontWords.push_back(word);
				word.clear();
			}
			space = true;
			break;

		default:

			word += boxText[i];
			space = false;
			break;
		}
	}

	if(!space && !word.empty())
		fontWords.push_back(word);

	isRendered = false;
	unlock();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief
 *
 * \return
 */
void GuiMultiTextBox::renderText()
{
	// clean up previous contents
	if(fontData != NULL)
	{
		for(int i=0; i<numLines; i++)
		{
			if(fontData[i]) s_font_uninit(fontData[i]);
			fontData[i] = NULL;
		}

		s_free(fontData);
		fontData = NULL;
	}

	numLines = 0;

	std::list<std::string>::iterator it = fontWords.begin();
	while(true)
	{
		std::string textLine;
		if(it != fontWords.end()) textLine = it->c_str();
		else break;

		s_font_t**	newFontData = (s_font_t**)s_realloc(fontData,
				sizeof(s_font_t*) * (numLines + 1));

		if (newFontData == NULL) return;

		fontData = newFontData;
		fontData[numLines] = NULL;

		s_font_t* font;
		if(s_font_init(&font, (char*)fontName.c_str()))
		{
			return;
		}

		s_font_set_size(font, fontSize);
		s_font_set_rgb(font, fontColor.red, fontColor.green, fontColor.blue);

		std::string textTest;
		while(it != fontWords.end())
		{
			if(!textTest.empty()) textTest += " ";
			textTest += it->c_str();

			s_font_set_str(font, (char*)textTest.c_str());
			s_font_get_glyph(font);

			// the text does not fit anymore
			if(font->glyph.img->w > drawArea.w)
				break;

			textLine = textTest;
			it ++;
		}

		s_font_set_str(font, (char*)textLine.c_str());
		s_font_get_glyph(font);

		fontData[numLines ++] = font;
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
void GuiMultiTextBox::onPaint(GuiSurface *drawSurface, s_rect_t &paintArea)
{
	lock();

	// return immediately if the primitive is not visible
	if(!isVisible)
	{
		unlock();
		return;
	}

	// check if the multi-line text is rendered
	if(!isRendered)
	{
		// render the text and flip the flag
		renderText();
		isRendered = true;
	}

	// sanity check, do we have any data to draw?
	if(fontData == NULL || numLines == 0)
	{
		unlock();
		return;
	}

	// loop through all lines of text
	for(int i = 0; i < numLines; i ++)
	{
		s_font_t* font = fontData[i];

		if (fontData[i] == NULL) continue;

		int ox = 0;
		switch(horzAlign)
		{
		case LEFT:
			ox = 0;
			break;
		case RIGHT:
			ox = drawArea.w - font->glyph.img->w;
			break;
		case HCENTER:
			ox = (drawArea.w - font->glyph.img->w) / 2;
			break;
		}

		s_rect_t textArea = { drawArea.x + ox, drawArea.y + i * fontSize,
				drawArea.w - ox, fontSize};

		s_rect_t surfRect;
		if(s_region_rect_intersect(&paintArea, &textArea, &surfRect))
			continue;

		s_rect_t sourceRect = { surfRect.x - textArea.x, surfRect.y - textArea.y,
							surfRect.w, surfRect.h };

		#ifdef DEBUG
		printf("GuiMultiTextBox::onPaint: line <%d> %d, %d, %d, %d to %d, %d\n",
				i, font->glyph.img->x, font->glyph.img->y,
				font->glyph.img->w, font->glyph.img->h,
				surfRect.x, surfRect.y);
		#endif

		drawSurface->drawImagePart(surfRect.x, surfRect.y,
									&sourceRect, font->glyph.img);
	}

	unlock();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief
 *
 * \return
 */
void GuiMultiTextBox::setAlignment(TextHAlignment horz)
{
	lock();

	horzAlign = horz;

	unlock();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief
 *
 * \return
 */
void GuiMultiTextBox::getRequiredSize(s_rect_t &rect)
{
	lock();

	rect.x = rect.y = 0;
	rect.w = drawArea.w;
	rect.h = numLines * fontSize;

	unlock();
}
