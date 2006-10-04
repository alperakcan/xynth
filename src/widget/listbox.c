/***************************************************************************
    begin                : Fri Sep 8 2006
    copyright            : (C) 2006 by Alper Akcan
    email				 : distchx@yahoo.com, computationutku@yahoo.com
    file author          : Alper Akcan, Utku Bulkan
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

void w_listbox_geometry(w_object_t *object)
{
	int pos;
	char *item;
	w_listbox_t *listbox;
	w_textbox_t *textbox;
	w_object_t *obj;
	int itemwidth=20;
	int counter;
	
	listbox = (w_listbox_t*) object->data[OBJECT_LISTBOX];
	
	if(listbox->scrollbar->background->isimg!=0)
		itemwidth=listbox->scrollbar->background->img_b->w;

	for (pos = 0; !s_list_eol(object->childs, 0); pos++) {
		obj = (w_object_t *) s_list_get(object->childs, 0);
		w_textbox_uninit(obj);
	}
	
	for (pos = listbox->scrollbar->firstitemonscreen,counter=0; 
				(counter<listbox->scrollbar->maxactivescreenitem) && (!s_list_eol(listbox->list, pos)) ; 
					pos++,counter++) {
		item = (char *) s_list_get(listbox->list, pos);
		w_textbox_init(object->window, &(textbox), object);
		if(pos!=listbox->scrollbar->activeitem)
			textbox->frame->style = FRAME_PLAIN;
		else
			textbox->frame->style = FRAME_PANEL | FRAME_PLAIN;
		w_object_move(textbox->frame->object, 0, counter * listbox->itemheight, object->content->w, listbox->itemheight);
		w_textbox_set_str(textbox->frame->object, item);
		w_object_show(textbox->frame->object);
	}	
	w_object_move(listbox->scrollbar->frame->object,
				listbox->frame->object->surface->buf->x+listbox->frame->object->surface->buf->w,
							listbox->frame->object->surface->buf->y,
										itemwidth,listbox->frame->object->surface->buf->h);
	w_object_show(listbox->scrollbar->frame->object);
}

void w_listbox_draw (w_object_t *object)
{
	w_listbox_t *listbox;
	listbox = object->data[OBJECT_LISTBOX];
	w_frame_draw(listbox->frame->object);
}

static void scrollbar_destroy (w_object_t *object)
{
	w_scrollbar_uninit(object);
}

static void scrollbar_draw (w_object_t *object)
{
	w_scrollbar_draw(object);
}

int w_listbox_add (w_listbox_t *listbox, char *item)
{
	s_list_add(listbox->list,(void*) item,-1);
	listbox->scrollbar->maxactiveitem=listbox->list->nb_elt;
	return 0;
}

static void buttonup_pressed(w_object_t *object, int buttonp)
{
	w_scrollbar_t *scrollbar;
	w_listbox_t *listbox;
	
	scrollbar = (w_scrollbar_t *) (object->parent->data[OBJECT_SCROLLBAR]);
	listbox = (w_listbox_t*) (scrollbar->friendobject);
		
	if( scrollbar->activeitem > 0)  
	{
		if(scrollbar->activeitem == scrollbar->firstitemonscreen )
		{
			scrollbar->firstitemonscreen--;
			scrollbar->activeitem--;
			w_scrollbar_geometry(scrollbar->frame->object);
			w_listbox_geometry(listbox->frame->object);
		}
		else if(scrollbar->activeitem > scrollbar->firstitemonscreen )
		{
			scrollbar->activeitem--;
			w_scrollbar_geometry(scrollbar->frame->object);
			w_listbox_geometry(listbox->frame->object);
		}
	}		
}

static void buttondown_pressed(w_object_t *object, int buttonp)
{
	w_scrollbar_t *scrollbar;
	w_listbox_t *listbox;
	
	scrollbar = (w_scrollbar_t *) (object->parent->data[OBJECT_SCROLLBAR]);
	listbox = (w_listbox_t*) (scrollbar->friendobject);
	
	if(scrollbar->activeitem < scrollbar->maxactiveitem - 1)  
	{
		if(scrollbar->activeitem == scrollbar->firstitemonscreen + scrollbar->maxactivescreenitem -1 )
		{
			scrollbar->firstitemonscreen++;
			scrollbar->activeitem++;
			w_scrollbar_geometry(scrollbar->frame->object);
			w_listbox_geometry(listbox->frame->object);
		}
		else if(scrollbar->activeitem < scrollbar->firstitemonscreen + scrollbar->maxactivescreenitem )
		{
			scrollbar->activeitem++;
			w_scrollbar_geometry(scrollbar->frame->object);
			w_listbox_geometry(listbox->frame->object);
		}
	}		
}


int w_listbox_init (w_window_t *window, w_listbox_t **listbox, w_object_t *parent)
{
	(*listbox) = (w_listbox_t *) s_malloc(sizeof(w_listbox_t));
	
	if (w_frame_init(window, &((*listbox)->frame), FRAME_PANEL | FRAME_RAISED, parent )) {
		goto err0;
	}
	
	w_scrollbar_init(window, &((*listbox)->scrollbar), parent);

	(*listbox)->scrollbar->frame->object->draw = scrollbar_draw;
	(*listbox)->scrollbar->frame->object->destroy = scrollbar_destroy;
	
	(*listbox)->scrollbar->scrollup->pressed = buttonup_pressed;
	(*listbox)->scrollbar->scrolldown->pressed = buttondown_pressed;
	(*listbox)->scrollbar->friendobject= (void *) (*listbox);
	
	(*listbox)->scrollbar->firstitemonscreen=0;
	(*listbox)->scrollbar->activeitemonscreen=0;
		
	s_list_init(&((*listbox)->list));
		
	s_handler_init(&((*listbox)->handler));
	(*listbox)->handler->type = MOUSE_HANDLER;
	(*listbox)->handler->mouse.x = 0;
	(*listbox)->handler->mouse.y = 0;
	(*listbox)->handler->mouse.w = 0;
	(*listbox)->handler->mouse.h = 0;
	(*listbox)->handler->mouse.button = ~MOUSE_NONEBUTTON;
	(*listbox)->handler->user_data = *listbox;	
	(*listbox)->state = 0;

	(*listbox)->frame->object->data[OBJECT_LISTBOX] = *listbox;
	(*listbox)->frame->object->draw = w_listbox_draw;
	(*listbox)->frame->object->geometry = w_listbox_geometry;
	(*listbox)->frame->object->destroy = w_listbox_uninit;
	
	(*listbox)->itemheight=24;
		
	return 0;
err0:	s_free(*listbox);
		printf("error occured in init listbox\n");
	return -1;
}

void w_listbox_uninit (w_object_t *object)
{
	w_listbox_t *listbox;
	w_object_t *obj;
	int pos;
	
	listbox = (w_listbox_t *) object->data[OBJECT_LISTBOX];

	for (pos = 0; !s_list_eol(object->childs, 0); pos++) {
		obj = (w_object_t *) s_list_get(object->childs, 0);
		w_textbox_uninit(obj);
	}
	
	w_scrollbar_uninit(listbox->scrollbar->frame->object);
	w_frame_uninit(listbox->frame->object);
	s_handler_uninit(listbox->handler);	
	s_list_uninit((listbox->list));	
	s_free(listbox);
}
