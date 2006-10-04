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
	int itemspace=0;
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
	
	if(scrollbar->background->isimg!=0)
	{
		width=scrollbar->background->img_b->w;
	}
	if(scrollbar->scrollup->isimg!=0)
	{
		itemheight=scrollbar->scrollup->img_b->h;
	}

	scrollmiddleh=(int)((double)scrollbar->maxactivescreenitem/(double)(scrollbar->maxactiveitem-1)*(double)(height-(itemheight*2)-itemspace*2));
	
	if(scrollbar->scrollmiddle->isimg!=0)
	{
		if(scrollmiddleh <= scrollbar->scrollmiddle->img_a->h+
				scrollbar->scrollmiddle->img_b->h+
					scrollbar->scrollmiddle->img_c->h)

			scrollmiddleh=scrollbar->scrollmiddle->img_a->h+
				scrollbar->scrollmiddle->img_b->h+
					scrollbar->scrollmiddle->img_c->h;
	}
	else
	{
		if(scrollmiddleh<=21)
			scrollmiddleh=21;
	}
	
	//scrollmiddleh=30;
	
	scrollmiddley=(int)((double)scrollbar->activeitem/(double)(scrollbar->maxactiveitem-1)*(double)(height-(itemheight*2)-scrollmiddleh-2*itemspace))+itemspace;
	scrollmiddlew=scrollbar->scrollmiddle->img_b->w;
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
	
	if(scrollbar->background->isimg==0)
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
	
	if (w_frame_init(window, &((*scrollbar)->frame), FRAME_PLAIN|FRAME_MSHADOW, parent )) {
		goto err0;
	}
	
	(*scrollbar)->activeitem=0;

	w_button_init((*scrollbar)->frame->object->window, &((*scrollbar)->scrollup), 
						(*scrollbar)->frame->object);	
	(*scrollbar)->scrollup->pressed = w_buttonup_pressed;
	w_button_loadstaticimage((*scrollbar)->scrollup->frame->object,
				"upbutton.png","uppressed.png","upontop.png");
		
	w_button_init((*scrollbar)->frame->object->window, &((*scrollbar)->scrolldown), 
						(*scrollbar)->frame->object);
	w_button_loadstaticimage((*scrollbar)->scrolldown->frame->object,
			"downbutton.png","downpressed.png","downontop.png");
	(*scrollbar)->scrolldown->pressed = w_buttondown_pressed;
	
	w_button_init((*scrollbar)->frame->object->window,
		&((*scrollbar)->background), (*scrollbar)->frame->object );
	w_button_loadverticalimage((*scrollbar)->background->frame->object,
				  "scrollbarup.png","scrollbaronepix.png","scrollbardown.png");
				//"scrollup_20.png","scrollmiddle_20.png","scrollbottom_20.png");								
	
	w_button_init((*scrollbar)->frame->object->window,
    					 &((*scrollbar)->scrollmiddle), 
								(*scrollbar)->background->frame->object);
	w_button_loadverticalimage((*scrollbar)->scrollmiddle->frame->object,
				"scrollbuttonup.png","scrollbuttononepix.png","scrollbuttondown.png");	
						
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
