/***************************************************************************
    begin                : Fri Sep 24 2004
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
#include "widgetoo.h"

void SFrame::frameRectContents (void)
{
	switch (frameStyle & SFrame::MShape) {
		case SFrame::NoFrame:
		case SFrame::MenuBarPanel:
		case SFrame::ToolBarPanel:
			frameLineWidth = 1;
			frameMidLineWidth = 0;
			break;
		case SFrame::Panel:
		case SFrame::StyledPanel:
			frameMidLineWidth = 0;
			break;
		case SFrame::WinPanel:
		case SFrame::PopupPanel:
		case SFrame::LineEditPanel:
		case SFrame::TabWidgetPanel:
			frameLineWidth = 2;
			frameMidLineWidth = 0;
			break;
		case SFrame::Box:
		case SFrame::HLine:
		case SFrame::VLine:
		case SFrame::GroupBoxPanel:
			if ((frameStyle & SFrame::MShadow) == SFrame::Plain) {
				frameMidLineWidth = 0;
			}
			break;
	}

	switch (frameStyle & SFrame::MShape) {
		case SFrame::NoFrame:
			objectSetRectContents(objectRectBuffer.rectX,
			                      objectRectBuffer.rectY,
			                      objectRectBuffer.rectW,
			                      objectRectBuffer.rectH);
			return;
		case SFrame::Box:
		case SFrame::HLine:
		case SFrame::VLine:
		case SFrame::GroupBoxPanel:
			switch (frameStyle & SFrame::MShadow) {
				case SFrame::Plain:
sbox_plain:				objectSetRectContents(objectRectBuffer.rectX + frameLineWidth,
                                                              objectRectBuffer.rectY + frameLineWidth,
                                                              objectRectBuffer.rectW - (frameLineWidth * 2),
                                                              objectRectBuffer.rectH - (frameLineWidth * 2));
					break;
				case SFrame::Raised:
				case SFrame::Sunken:
					objectSetRectContents(objectRectBuffer.rectX + (frameLineWidth * 2) + (frameMidLineWidth),
					                      objectRectBuffer.rectY + (frameLineWidth * 2) + (frameMidLineWidth),
					                      objectRectBuffer.rectW - (frameLineWidth * 4) - (frameMidLineWidth * 2),
					                      objectRectBuffer.rectH - (frameLineWidth * 4) - (frameMidLineWidth * 2));
					break;
			}
			break;
		case SFrame::Panel:
		case SFrame::WinPanel:
		case SFrame::PopupPanel:
		case SFrame::StyledPanel:
		case SFrame::MenuBarPanel:
		case SFrame::ToolBarPanel:
		case SFrame::LineEditPanel:
		case SFrame::TabWidgetPanel:
			goto sbox_plain;
			break;
	}
}

void SFrame::frameSetLineWidth (int linewidth)
{
	frameLineWidth = linewidth;
	frameRectContents();
}

void SFrame::frameSetMidLineWidth (int midlinewidth)
{
	frameMidLineWidth = midlinewidth;
	frameRectContents();
}

void SFrame::frameSetStyle (int style)
{
	frameStyle = style & (SFrame::MShape | SFrame::MShadow);
	frameRectContents();
}

void SFrame::objectDraw (void)
{
        int i;
        int j;
        int c[4];

        switch (frameStyle & SFrame::MShape) {
		case SFrame::NoFrame:		return;
		case SFrame::Box:
		case SFrame::GroupBoxPanel:
			switch (frameStyle & SFrame::MShadow) {
				case SFrame::Plain:
sbox_plain:				objectFillBox(0, 0, objectRectBuffer.rectW, objectRectBuffer.rectH, objectRGBColor(0, 0, 0));
					break;
				case SFrame::Raised:
					c[0] = objectRGBColor(96, 96, 96);
					c[1] = objectRGBColor(128, 128, 128);
					c[2] = objectRGBColor(255, 255, 255);
sbox_raised:				for (i = 0; i < frameLineWidth; i++) {
						objectFillBox(i, i, objectRectBuffer.rectW - (i * 2), objectRectBuffer.rectH - (i * 2), c[2]);
						objectFillBox(i + 1, i + 1, objectRectBuffer.rectW - (i * 2) - 1, objectRectBuffer.rectH - (i * 2) - 1, c[0]);
					}
					objectFillBox(i, i, objectRectBuffer.rectW - (i * 2), objectRectBuffer.rectH - (i * 2), c[1]);
					for (j = 0; j < frameLineWidth; j++) {
						objectFillBox(j + i + frameMidLineWidth, j + i + frameMidLineWidth, objectRectBuffer.rectW - ((j + i + frameMidLineWidth) * 2), objectRectBuffer.rectH - ((j + i + frameMidLineWidth) * 2), c[0]);
						objectFillBox(j + 1 + i + frameMidLineWidth, j + 1 + i + frameMidLineWidth, objectRectBuffer.rectW - ((j + i + frameMidLineWidth) * 2) - 1, objectRectBuffer.rectH - ((j + i + frameMidLineWidth) * 2) - 1, c[2]);
					}
					break;
				case SFrame::Sunken:
					c[2] = objectRGBColor(96, 96, 96);
					c[1] = objectRGBColor(128, 128, 128);
					c[0] = objectRGBColor(255, 255, 255);
					goto sbox_raised;
					break;
			}
			break;
		case SFrame::Panel:
		case SFrame::StyledPanel:
			switch (frameStyle & SFrame::MShadow) {
				case SFrame::Plain:
					goto sbox_plain;
					break;
				case SFrame::Raised:
					c[0] = objectRGBColor(96, 96, 96);
					c[1] = objectRGBColor(255, 255, 255);
spanel_raised:				for (i = 0; i < frameLineWidth; i++) {
						objectFillBox(i, i, objectRectBuffer.rectW - (i * 2), objectRectBuffer.rectH - (i * 2), c[0]);
						objectFillBox(i, i, objectRectBuffer.rectW - (i * 2) - 1, objectRectBuffer.rectH - (i * 2) - 1, c[1]);
					}
					break;
				case SFrame::Sunken:
					c[1] = objectRGBColor(96, 96, 96);
					c[0] = objectRGBColor(255, 255, 255);
					goto spanel_raised;
					break;
			}
			break;
		case SFrame::WinPanel:
		case SFrame::PopupPanel:
			switch (frameStyle & SFrame::MShadow) {
				case SFrame::Plain:
					goto sbox_plain;
					break;
				case SFrame::Raised:
swinpanel_raised:			c[0] = objectRGBColor(0, 0, 0);
					c[1] = objectRGBColor(96, 96, 96);
					c[2] = objectRGBColor(220, 220, 220);
					c[3] = objectRGBColor(255, 255, 255);
					objectFillBox(0, 0, objectRectBuffer.rectW, objectRectBuffer.rectH, c[0]);
					objectFillBox(0, 0, objectRectBuffer.rectW - 1, objectRectBuffer.rectH - 1, c[2]);
					objectFillBox(1, 1, objectRectBuffer.rectW - 2, objectRectBuffer.rectH - 2, c[1]);
					objectFillBox(1, 1, objectRectBuffer.rectW - 3, objectRectBuffer.rectH - 3, c[3]);
					break;
swinpanel_sunken:		case SFrame::Sunken:
					c[0] = objectRGBColor(0, 0, 0);
					c[1] = objectRGBColor(96, 96, 96);
					c[2] = objectRGBColor(220, 220, 220);
					objectFillBox(0, 0, objectRectBuffer.rectW, objectRectBuffer.rectH, c[1]);
					objectFillBox(1, 1, objectRectBuffer.rectW - 1, objectRectBuffer.rectH - 1, c[2]);
					objectFillBox(1, 1, objectRectBuffer.rectW - 2, objectRectBuffer.rectH - 2, c[0]);
					objectFillBox(2, 2, objectRectBuffer.rectW - 3, objectRectBuffer.rectH - 3, c[2]);
					break;
			}
			break;
		case SFrame::MenuBarPanel:
			objectFillBox(0, 0, objectRectBuffer.rectW, objectRectBuffer.rectH, objectRGBColor(220, 220, 220));
			break;
		case SFrame::ToolBarPanel:
			c[0] = objectRGBColor(96, 96, 96);
			c[1] = objectRGBColor(255, 255, 255);
			objectFillBox(0, 0, objectRectBuffer.rectW, objectRectBuffer.rectH, c[0]);
			objectFillBox(0, 0, objectRectBuffer.rectW - 1, objectRectBuffer.rectH - 1, c[1]);
			break;
		case SFrame::LineEditPanel:
		case SFrame::TabWidgetPanel:
			switch (frameStyle & SFrame::MShadow) {
				case SFrame::Plain:
				case SFrame::Raised:
					goto swinpanel_raised;
					break;
				case SFrame::Sunken:
					goto swinpanel_sunken;
					break;
			}
			break;
		case SFrame::HLine:
		case SFrame::VLine:
			break;
	}

	objectFillBox(objectRectContents.rectX - objectRectBuffer.rectX,
	              objectRectContents.rectY - objectRectBuffer.rectY,
	              objectRectContents.rectW,
	              objectRectContents.rectH,
	              objectRGBColor(220, 220, 220));
}

void SFrame::objectGeometry (int x, int y, int w, int h)
{
        objectSetRectBuffer(x, y, w, h);
	frameRectContents();
}

SFrame::SFrame (SObject *parent, int style) : SObject(parent)
{
	frameStyle = style & (SFrame::MShape | SFrame::MShadow);
	frameLineWidth = 1;
	frameMidLineWidth = 0;
	frameRectContents();
}

SFrame::~SFrame (void)
{
}
