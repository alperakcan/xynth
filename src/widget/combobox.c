/***************************************************************************
    begin                : Fri Sep 8 2006
    copyright            : (C) 2006 by Alper Akcan
    email                : distchx@yahoo.com
    file author          : Utku Bulkan ,murderdeathkill2@yahoo.com
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

static void button_pressed (w_object_t *object, int buttonp)
{
	w_button_t *button;
	w_combobox_t *combobox;
	
	button = object->data[OBJECT_BUTTON];
	combobox = (button->object->parent)->data[OBJECT_COMBOBOX];
	
	if((combobox)->islistboxvisible==0)
	{
		(combobox)->islistboxvisible=1;
		w_object_move(combobox->listbox->frame->object,
						 0,combobox->itemheight,combobox->frame->object->content->w-combobox->buttonlength,
						 combobox->frame->object->content->h-combobox->itemheight);
		w_object_show(combobox->listbox->frame->object);
		w_object_show(combobox->listbox->scrollbar->frame->object);			
	}
	else if((combobox)->islistboxvisible==1)
	{
		w_textbox_set_str(combobox->editbox->object, 
				(char*)s_list_get(combobox->listbox->list,combobox->listbox->scrollbar->activeitem));
	
		w_object_move(combobox->editbox->object,
							 0,0,combobox->frame->object->content->w-combobox->buttonlength,combobox->buttonlength);
		w_object_show(combobox->editbox->object);
	
		(combobox)->islistboxvisible=0;
		w_object_hide(combobox->listbox->frame->object);		
		w_object_hide(combobox->listbox->scrollbar->frame->object);			
	}
}

static void button_destroy (w_object_t *object)
{
	w_button_uninit(object);
}

static void button_draw (w_object_t *object)
{
	w_button_draw(object);
}

void w_combobox_geometry(w_object_t *object)
{
	w_combobox_t *combobox;
	
	combobox = (w_combobox_t*) object->data[OBJECT_COMBOBOX];
		
	w_textbox_set_str(combobox->editbox->object, 
				(char*)s_list_get(combobox->listbox->list,combobox->listbox->scrollbar->activeitem));
	
	w_object_move(combobox->editbox->object,
						 0,0,object->content->w-combobox->buttonlength,combobox->itemheight);
	w_object_show(combobox->editbox->object);
	w_object_move(combobox->button->object,
						 object->content->w-combobox->buttonlength,0,combobox->buttonlength,combobox->buttonlength);
	w_object_show(combobox->button->object);			
}

void w_combobox_draw (w_object_t *object)
{
	w_combobox_t *combobox;
	combobox = object->data[OBJECT_COMBOBOX];
	w_frame_draw(combobox->frame->object);
}


int w_combobox_init (w_window_t *window, w_combobox_t **combobox, w_object_t *parent)
{
	(*combobox) = (w_combobox_t *) s_malloc(sizeof(w_combobox_t));
	
	if (w_frame_init(window, &((*combobox)->frame), FRAME_PANEL | FRAME_RAISED, parent )) {
		goto err0;
	}
	
	w_listbox_init(window, &((*combobox)->listbox), (*combobox)->frame->object);
	
	w_editbox_init(window, &((*combobox)->editbox), (*combobox)->frame->object);
	w_textbox_loadimages((*combobox)->editbox->textbox->object,
						//"etextleft_20.png","etextmiddle_20.png","etextright_20.png");
						"etextboxleft.png","etextboxonepix.png","etextboxright.png");
	
	w_button_init(window, &((*combobox)->button), (*combobox)->frame->object);
	w_button_loadstaticimage((*combobox)->button->frame->object,"downbutton.png","downpressed.png","downontop.png");
	
	if((*combobox)->editbox->textbox->isimg!=0)
	{
		(*combobox)->itemheight=(*combobox)->editbox->textbox->img_left->h;
		(*combobox)->listbox->itemheight=(*combobox)->editbox->textbox->img_left->h;
	}
	else
	{
		(*combobox)->itemheight=24;
		(*combobox)->listbox->itemheight=24;
	}
		
	if((*combobox)->button->isimg!=0)
		(*combobox)->buttonlength=(*combobox)->button->img_a->w;
	else
		(*combobox)->buttonlength=24;	
			
	(*combobox)->button->pressed = button_pressed;
	(*combobox)->button->frame->object->draw = button_draw;
	(*combobox)->button->frame->object->destroy = button_destroy;
		
	(*combobox)->islistboxvisible=0;
	
	s_handler_init(&((*combobox)->handler));
	(*combobox)->handler->type = MOUSE_HANDLER;
	(*combobox)->handler->mouse.x = 0;
	(*combobox)->handler->mouse.y = 0;
	(*combobox)->handler->mouse.w = 0;
	(*combobox)->handler->mouse.h = 0;
	(*combobox)->handler->mouse.button = ~MOUSE_NONEBUTTON;
	(*combobox)->handler->data = *combobox;	
	(*combobox)->state = 0;

	(*combobox)->frame->object->data[OBJECT_COMBOBOX] = *combobox;
	(*combobox)->frame->object->draw = w_combobox_draw;
	(*combobox)->frame->object->geometry = w_combobox_geometry;
	(*combobox)->frame->object->destroy = w_combobox_uninit;
		
	
		
	return 0;
err0:	s_free(*combobox);
		printf("error occured in init listbox\n");
	return -1;
}

void w_combobox_uninit (w_object_t *object)
{
	w_combobox_t *combobox;

	combobox = (w_combobox_t *) object->data[OBJECT_COMBOBOX];
	
	w_listbox_uninit(combobox->listbox->frame->object);
	w_editbox_uninit(combobox->editbox->object);
	w_button_uninit(combobox->button->frame->object);	
	
	w_frame_uninit(combobox->frame->object);
	s_handler_uninit(combobox->handler);	
	
	s_free(combobox);
}
