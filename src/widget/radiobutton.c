/***************************************************************************
    begin                : Fri Sep 8 2006
    copyright            : (C) 2006 by Alper Akcan
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

void w_radiobutton_draw (w_object_t *object)
{
	w_radiobutton_t *radiobutton;
	radiobutton = (w_radiobutton_t *) object->data[OBJECT_BUTTON];
		
	w_frame_draw(object);	
}

void w_radiobutton_geometry (w_object_t *object)
{
	w_radiobutton_t *radiobutton;
	w_frame_geometry(object);
	radiobutton = (w_radiobutton_t *) object->data[OBJECT_RADIOBUTTON];
		
	w_object_move(radiobutton->editbox->textbox->frame->object, radiobutton->buttonwidth, 0, 
					object->content->w-radiobutton->buttonwidth, radiobutton->itemheight);
	//w_textbox_set_str(radiobutton->editbox->textbox->frame->object, "Radio Button");
	w_object_show(radiobutton->editbox->textbox->frame->object);
	
	w_object_move(radiobutton->button->frame->object, 0, 0, radiobutton->buttonwidth, radiobutton->itemheight);
	w_object_show(radiobutton->button->frame->object);	
}

static void button_clicked (s_window_t *window, s_event_t *event, s_handler_t *handler)
{ /* clicked */
	
}

static void button_cb_o (s_window_t *window, s_event_t *event, s_handler_t *handler)
{ /* over */
	
}

static void button_cb_p (s_window_t *window, s_event_t *event, s_handler_t *handler)
{ /* pressed */
	w_button_t *button;
	w_radiobutton_t *radiobutton;
	
	button = (w_button_t *) handler->data;

	w_button_cb_c (window, event, handler);
	
	radiobutton = button->frame->object->parent->data[OBJECT_RADIOBUTTON];
	
	if(radiobutton->state==0)
		radiobutton->state=1;
	else if(radiobutton->state==1)
		radiobutton->state=0;	
	
	if(radiobutton->state==0)
	{
		radiobutton->button->state=0;
		radiobutton->button->frame->style=34;		
	}
	else if(radiobutton->state==1)
	{
		radiobutton->button->state=4;
		radiobutton->button->frame->style=66;
	}
	
	w_button_draw(radiobutton->button->frame->object);
	w_object_move(radiobutton->button->frame->object, 0, 0, radiobutton->buttonwidth, radiobutton->itemheight);
	w_object_show(radiobutton->button->frame->object);	

}


static void button_cb_r (s_window_t *window, s_event_t *event, s_handler_t *handler)
{ /* released */
	
}

static void button_cb_hr (s_window_t *window, s_event_t *event, s_handler_t *handler)
{ /* mouse button released, but the prev. press was not on us */
}

static void button_cb_rh (s_window_t *window, s_event_t *event, s_handler_t *handler)
{ /* mouse button released outside, but the prev. press was on us */
}

static void button_cb_ho (s_window_t *window, s_event_t *event, s_handler_t *handler)
{ /* on over, but mouse button is still pressed */
	
}

static void button_cb_oh (s_window_t *window, s_event_t *event, s_handler_t *handler)
{ /* not on over, but was on over */
	
}

static void button_cb_hoh (s_window_t *window, s_event_t *event, s_handler_t *handler)
{ /* not on over, but was on over. and button is still pressed */
	
}



int w_radiobutton_init (w_window_t *window, w_radiobutton_t **radiobutton, w_object_t *parent)
{
	(*radiobutton) = (w_radiobutton_t *) s_malloc(sizeof(w_radiobutton_t));
	if (w_frame_init(window, &((*radiobutton)->frame), FRAME_BOX , parent)) {
		goto err0;
	}
	
	w_button_init(window, &((*radiobutton)->button), (*radiobutton)->frame->object);
	w_button_loadstaticimage((*radiobutton)->button->frame->object,"radiobutton.png","radiobuttonpressed.png","downontop.png");
	
	(*radiobutton)->button->handler->mouse.c = button_clicked;
	
	
	(*radiobutton)->button->handler->mouse.o = button_cb_o;
	(*radiobutton)->button->handler->mouse.p = button_cb_p;
	(*radiobutton)->button->handler->mouse.r = button_cb_r;
	(*radiobutton)->button->handler->mouse.hr = button_cb_hr;
	(*radiobutton)->button->handler->mouse.rh = button_cb_rh;
	(*radiobutton)->button->handler->mouse.ho = button_cb_ho;
	(*radiobutton)->button->handler->mouse.oh = button_cb_oh;
	(*radiobutton)->button->handler->mouse.hoh = button_cb_hoh;
	
	w_editbox_init(window, &((*radiobutton)->editbox), (*radiobutton)->frame->object);
	//w_textbox_loadimages((*radiobutton)->editbox->textbox->object,
	//					"etextleft_20.png","etextmiddle_20.png","etextright_20.png");

	(*radiobutton)->object=(*radiobutton)->frame->object;
	(*radiobutton)->object->data[OBJECT_RADIOBUTTON] = *radiobutton;
	(*radiobutton)->object->draw = w_radiobutton_draw;
	(*radiobutton)->object->geometry = w_radiobutton_geometry;
	(*radiobutton)->object->destroy = w_radiobutton_uninit;
	
	(*radiobutton)->editbox->object->event = NULL;
	
	(*radiobutton)->editbox->textbox->properties = TEXTBOX_VCENTER | TEXTBOX_WRAP;
	(*radiobutton)->editbox->textbox->frame->style = FRAME_BOX;
	
	(*radiobutton)->state=0;
	
	(*radiobutton)->buttonwidth=20;
	(*radiobutton)->itemheight=20;
	
	if((*radiobutton)->button->isimg!=0)
		(*radiobutton)->buttonwidth = (*radiobutton)->button->img_a->w;
	if((*radiobutton)->editbox->textbox->isimg!=0)
		(*radiobutton)->itemheight = (*radiobutton)->editbox->textbox->img_middle;
			
	return 0;
err0:	s_free(*radiobutton);
	return -1;
}

void w_radiobutton_uninit (w_object_t *object)
{
	w_radiobutton_t *radiobutton;
	radiobutton = (w_radiobutton_t *) object->data[OBJECT_RADIOBUTTON];
	
	w_editbox_uninit(radiobutton->editbox->object);
	w_button_uninit(radiobutton->button->frame->object);
	
	w_frame_uninit(radiobutton->object);
	
	s_free(radiobutton);
}
