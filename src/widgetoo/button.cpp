/***************************************************************************
    begin                : Tue Nov 2 2004
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

void SButton::objectDraw (void)
{
        buttonHandler->handlerAdd();
	SFrame::objectDraw();
}

void SButton::objectGeometry (int x, int y, int w, int h)
{
        buttonHandler->handlerHndl->mouse.x = x;
        buttonHandler->handlerHndl->mouse.y = y;
        buttonHandler->handlerHndl->mouse.w = w;
        buttonHandler->handlerHndl->mouse.h = h;
	SFrame::objectGeometry(x, y, w, h);
}

void SButton::buttonPressed (int button)
{
}

void SButton::buttonReleased (int button)
{
}

void SButton::buttonClicked (int button, int clickCount)
{
}

void SButton::button_cb_o (s_window_t *window, s_event_t *event, s_handler_t *hndl)
{ /* over */
}

void SButton::button_cb_p (s_window_t *window, s_event_t *event, s_handler_t *hndl)
{ /* pressed */
	SButton *btn = (SButton *) hndl->data;
	if ((btn->bstate == 0) &&
	    (event->mouse->buttons == event->mouse->b)) {
		btn->bstate |= event->mouse->b;
		btn->buttonPressed(event->mouse->b);
		btn->frameSetStyle((btn->frameStyle & SFrame::MShape) | SFrame::Sunken);
		btn->draw();
	}
}

void SButton::button_cb_c (s_window_t *window, s_event_t *event, s_handler_t *hndl)
{ /* clicked */
}

void SButton::button_cb_r (s_window_t *window, s_event_t *event, s_handler_t *hndl)
{ /* released */
	SButton *btn = (SButton *) hndl->data;
	if ((btn->bstate != 0) &&
	    (btn->bstate == event->mouse->b)) {
		btn->bstate = 0;
		btn->buttonReleased(event->mouse->b);
		btn->frameSetStyle((btn->frameStyle & SFrame::MShape) | SFrame::Raised);
		btn->draw();
	}
}

void SButton::button_cb_hr (s_window_t *window, s_event_t *event, s_handler_t *hndl)
{ /* mouse button released, but the prev. press was not on us */
}

void SButton::button_cb_ho (s_window_t *window, s_event_t *event, s_handler_t *hndl)
{ /* on over, but mouse button is still pressed */
	SButton *btn = (SButton *) hndl->data;
	if (!(btn->frameStyle & SFrame::Sunken)) {
		if (btn->bstate != 0) {
			btn->frameSetStyle((btn->frameStyle & SFrame::MShape) | SFrame::Sunken);
			btn->draw();
		}
	}
}

void SButton::button_cb_oh (s_window_t *window, s_event_t *event, s_handler_t *hndl)
{ /* not on over, but was on over */
	SButton *btn = (SButton *) hndl->data;
	if (!(btn->frameStyle & SFrame::Raised)) {
		btn->frameSetStyle((btn->frameStyle & SFrame::MShape) | SFrame::Raised);
		btn->draw();
	}
}

void SButton::button_cb_hoh (s_window_t *window, s_event_t *event, s_handler_t *hndl)
{ /* not on over, but was on over. and button is still pressed */
	SButton *btn = (SButton *) hndl->data;
	if (!(btn->frameStyle & SFrame::Raised)) {
		btn->frameSetStyle((btn->frameStyle & SFrame::MShape) | SFrame::Raised);
		btn->draw();
	}
}

void SButton::button_cb_rh (s_window_t *window, s_event_t *event, s_handler_t *hndl)
{ /* mouse button released outside, but the prev. press was on us */
	SButton *btn = (SButton *) hndl->data;
	if (btn->bstate == event->mouse->b) {
		btn->bstate = 0;
	}
}

SButton::SButton (SObject *parent, int style) : SFrame(parent, style | SFrame::Raised)
{
	buttonHandler = new SHandler(this, MOUSE_HANDLER);
	buttonHandler->handlerHndl->mouse.button = ~MOUSE_BUTTON_NONE;

	buttonHandler->handlerHndl->mouse.o = button_cb_o;
	buttonHandler->handlerHndl->mouse.p = button_cb_p;
	buttonHandler->handlerHndl->mouse.c = button_cb_c;

	buttonHandler->handlerHndl->mouse.r = button_cb_r;
	buttonHandler->handlerHndl->mouse.hr = button_cb_hr;
	buttonHandler->handlerHndl->mouse.rh = button_cb_rh;

	buttonHandler->handlerHndl->mouse.ho = button_cb_ho;
	buttonHandler->handlerHndl->mouse.oh = button_cb_oh;
	buttonHandler->handlerHndl->mouse.hoh = button_cb_hoh;
	
	buttonHandler->handlerHndl->data = (void *) this;
	
	bstate = 0;
}

SButton::~SButton (void)
{
	delete buttonHandler;
}
