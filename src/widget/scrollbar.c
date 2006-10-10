/***************************************************************************
    begin                : Fri Sep 8 2006
    copyright            : (C) 2006 by Alper Akcan
    email                : distchx@yahoo.com
    file author          : Utku Bulkan, murderdeathkill2@yahoo.com
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

void w_scrollbar_geometry(w_object_t *object)
{
	int width,height;
	int itemspace=2;
	int itemheight=24;
	w_scrollbar_t *scrollbar;
	int scrollmiddlex,scrollmiddley,scrollmiddlew,scrollmiddleh;	
	
	scrollbar = (w_scrollbar_t *) object->data[OBJECT_SCROLLBAR];
	
	if( (scrollbar->frame->object->surface->width!=0) &&
		(scrollbar->frame->object->surface->height!=0) )
	{
		width=scrollbar->frame->object->surface->width;
		height=scrollbar->frame->object->surface->height;
	}
	else
	{
		width=scrollbar->frame->object->content->w;
		height=scrollbar->frame->object->content->h;
	}
	
	scrollmiddleh=(int)((double)scrollbar->maxactivescreenitem/(double)(scrollbar->maxactiveitem-1)*(double)(height-(itemheight*2)-itemspace*2));
	
	scrollmiddley=(int)((double)scrollbar->activeitem/(double)(scrollbar->maxactiveitem-1)*(double)(height-(itemheight*2)-scrollmiddleh-2*itemspace))+itemspace;
	scrollmiddlew=17;
	scrollmiddlex=(width-scrollmiddlew)/2;
	
	w_object_move(scrollbar->background->object,0,itemheight,width,height-(itemheight*2));
	w_object_move(scrollbar->scrollup->frame->object, 0, 0, width, itemheight);
	w_object_move(scrollbar->scrolldown->frame->object, 0, height-itemheight, width, itemheight);
	w_object_move(scrollbar->scrollmiddle->frame->object, scrollmiddlex, 
							scrollmiddley, scrollmiddlew, scrollmiddleh);

	w_object_show(scrollbar->scrollup->frame->object);
	w_object_show(scrollbar->scrolldown->frame->object);

	w_object_show(scrollbar->background->object);
	w_object_show(scrollbar->scrollmiddle->frame->object);
}

void w_scrollbar_draw (w_object_t *object)
{
	w_scrollbar_t *scrollbar;
	scrollbar = (w_scrollbar_t *) object->data[OBJECT_SCROLLBAR];

	w_frame_draw(scrollbar->frame->object);
	memset(scrollbar->frame->object->surface->matrix, 0, 
			scrollbar->frame->object->surface->width * scrollbar->frame->object->surface->height);

	w_frame_draw(scrollbar->background->object);
	
}

void w_buttonup_pressed(w_object_t *object, int buttonp)
{
	w_scrollbar_t *scrollbar;
	scrollbar = (w_scrollbar_t *) (object->parent->data[OBJECT_SCROLLBAR]);

	if(scrollbar->activeitem>0)
	{
		scrollbar->activeitem--;
		w_scrollbar_geometry(scrollbar->frame->object);
	}
}

void w_buttondown_pressed(w_object_t *object, int buttonp)
{
	w_scrollbar_t *scrollbar;
	scrollbar = (w_scrollbar_t *) (object->parent->data[OBJECT_SCROLLBAR]);
	
	if(scrollbar->activeitem<scrollbar->maxactiveitem)
	{
		scrollbar->activeitem++;
		w_scrollbar_geometry(scrollbar->frame->object);
	}
}


int w_scrollbar_init (w_window_t *window, w_scrollbar_t **scrollbar, w_object_t *parent)
{
	(*scrollbar) = (w_scrollbar_t *) s_malloc(sizeof(w_scrollbar_t));
	
	if (w_frame_init(window, &((*scrollbar)->frame), FRAME_PANEL | FRAME_RAISED, parent )) {
		goto err0;
	}
	
	(*scrollbar)->activeitem=0;

	w_button_init((*scrollbar)->frame->object->window, &((*scrollbar)->scrollup), 
						(*scrollbar)->frame->object);	
	(*scrollbar)->scrollup->pressed = w_buttonup_pressed;
	//w_frame_set_image((*scrollbar)->scrollup->object,
	//				FRAME_PANEL | FRAME_RAISED,FRAME_IMAGE_VERTICAL,1,"upbutton.png");
	//w_frame_set_image((*scrollbar)->scrollup->object,	
	//				FRAME_PANEL | FRAME_SUNKEN,FRAME_IMAGE_VERTICAL,1,"uppressed.png");					
		
	w_button_init((*scrollbar)->frame->object->window, &((*scrollbar)->scrolldown), 
						(*scrollbar)->frame->object);
	(*scrollbar)->scrolldown->pressed = w_buttondown_pressed;
	//w_frame_set_image((*scrollbar)->scrolldown->object,
	//				FRAME_PANEL | FRAME_RAISED,FRAME_IMAGE_VERTICAL,1,"downbutton.png");
	//w_frame_set_image((*scrollbar)->scrolldown->object,
	//					FRAME_PANEL | FRAME_SUNKEN,FRAME_IMAGE_VERTICAL,1,"downpressed.png");					
	
	w_button_init((*scrollbar)->frame->object->window,
		&((*scrollbar)->background), (*scrollbar)->frame->object );

	//w_frame_set_image((*scrollbar)->background->frame->object,
	//				FRAME_PANEL | FRAME_RAISED,FRAME_IMAGE_VERTICAL,3,
	//			  "scrollbarup.png","scrollbaronepix.png","scrollbardown.png");
	(*scrollbar)->background->object->event=NULL;
	
	w_button_init((*scrollbar)->frame->object->window,
    					 &((*scrollbar)->scrollmiddle), 
								(*scrollbar)->background->frame->object);
	//w_frame_set_image((*scrollbar)->scrollmiddle->frame->object,
	//				FRAME_PANEL | FRAME_RAISED,FRAME_IMAGE_VERTICAL,3,
	//			  "scrollbuttonup.png","scrollbuttononepix.png","scrollbuttondown.png");
	(*scrollbar)->scrollmiddle->object->event=NULL;
						
	s_handler_init(&((*scrollbar)->handler));
	(*scrollbar)->handler->type = MOUSE_HANDLER;
	(*scrollbar)->handler->mouse.x = 0;
	(*scrollbar)->handler->mouse.y = 0;
	(*scrollbar)->handler->mouse.w = 0;
	(*scrollbar)->handler->mouse.h = 0;
	(*scrollbar)->handler->mouse.button = ~MOUSE_NONEBUTTON;
	(*scrollbar)->handler->data = *scrollbar;	
	(*scrollbar)->state = 0;

	(*scrollbar)->frame->object->data[OBJECT_SCROLLBAR] = *scrollbar;
	(*scrollbar)->frame->object->draw = w_scrollbar_draw;
	(*scrollbar)->frame->object->geometry = w_scrollbar_geometry;
	(*scrollbar)->frame->object->destroy = w_scrollbar_uninit;
		
	return 0;
err0:	s_free(*scrollbar);
		printf("error occured in init scrollbar\n");
	return -1;
}

void w_scrollbar_uninit (w_object_t *object)
{
	w_scrollbar_t *scrollbar;
		
	scrollbar = (w_scrollbar_t *) object->data[OBJECT_SCROLLBAR];
	
	w_button_uninit(scrollbar->scrollup->frame->object);
	w_button_uninit(scrollbar->scrolldown->frame->object);
	w_button_uninit(scrollbar->scrollmiddle->frame->object);
	w_button_uninit(scrollbar->background->object);
	w_frame_uninit(scrollbar->frame->object);
	s_handler_uninit(scrollbar->handler);
	s_free(scrollbar);	
}
