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

void w_progressbar_setstatus(w_object_t *object,int progressbarstatus)
{
	w_progressbar_t *progressbar;
	progressbar = (w_progressbar_t*)object->data[OBJECT_PROGRESSBAR];
	if((progressbar->progressbarstatus>=0)&&(progressbar->progressbarstatus<=100))
	{
		progressbar->progressbarstatus = progressbarstatus;
		w_progressbar_geometry(progressbar->frame->object);	
	}	
}

int w_progressbar_textbox_set_str (w_object_t *object, char *str,int barlength)
{
	int x;
	int y;
	int w;
	int h;

	w_textbox_t *textbox;
	textbox = (w_textbox_t *) object->data[OBJECT_TEXTBOX];
	s_font_set_str(textbox->font, str);
	s_font_get_glyph(textbox->font);
	w_textbox_draw(object);

	w = MIN(textbox->frame->object->content->w, textbox->font->img->w);
	h = MIN(textbox->frame->object->content->h, textbox->font->img->h);
	if (textbox->frame->object->content->w == w) { x = 0;
	} else { x = (textbox->frame->object->content->w - w) / 2; }
	if (textbox->frame->object->content->h == h) { y = 0;
	} else { y = (textbox->frame->object->content->h - h) / 2; }
	x += textbox->frame->object->content->x;
	y += textbox->frame->object->content->y;

	s_image_get_mat(textbox->font->img);
	if ((textbox->frame->style & FRAME_MSHAPE) == FRAME_NOFRAME) {
		memset(textbox->frame->object->surface->matrix,
		       0,
		       textbox->frame->object->surface->width * textbox->frame->object->surface->height);
		
				
		s_putmaskpart((char*)(textbox->frame->object->surface->matrix),
		              textbox->frame->object->surface->width,
		              textbox->frame->object->surface->height,
		              x,
		              y,
		              w,
		              h,
		              textbox->font->img->w,
		              textbox->font->img->h,
		              (char*)(textbox->font->img->mat),
		              0,
		              0);
	} else {
		w_frame_draw(textbox->frame->object);
	}
	s_fillbox(textbox->frame->object->surface, 2, 2, barlength, 
							textbox->frame->object->surface->height-4, 
					s_rgbcolor(textbox->frame->object->surface, 0, 0, 200));
	s_putboxpartrgba(textbox->frame->object->surface,
	                 x,
	                 y,
	                 w,
	                 h,
	                 textbox->font->img->w,
	                 textbox->font->img->h,
	                 textbox->font->img->rgba,
	                 0,
	                 0);
	return 0;
}

void w_progressbar_geometry(w_object_t *object)
{
	int width,height;
	char item[10];
	int barlength=0;
	w_progressbar_t *progressbar;
	progressbar = (w_progressbar_t *) object->data[OBJECT_PROGRESSBAR];
	
	sprintf(item,"%%%d",progressbar->progressbarstatus);
	if( (progressbar->frame->object->surface->width!=0) &&
		(progressbar->frame->object->surface->height!=0) )
	{
		width=progressbar->frame->object->surface->width;
		height=progressbar->frame->object->surface->height;
	}
	else
	{
		width=progressbar->frame->object->content->w;
		height=progressbar->frame->object->content->h;
	}		
	w_object_move(progressbar->textbox->frame->object, 2, 2, width-4, height-4);
	if((progressbar->progressbarstatus>=0)&&(progressbar->progressbarstatus<=100))
	{
		barlength =  ( (progressbar->progressbarstatus/100.0) * 
	                   		((progressbar->textbox->frame->object->surface->width-4)) );				
	}
	w_progressbar_textbox_set_str(progressbar->textbox->frame->object, item, barlength);
	w_object_show(progressbar->textbox->frame->object);
}

void w_progressbar_draw (w_object_t *object)
{
	w_progressbar_t *progressbar;
	progressbar = (w_progressbar_t *) object->data[OBJECT_PROGRESSBAR];
	w_frame_draw(progressbar->frame->object);
}

int w_progressbar_init (w_window_t *window, w_progressbar_t **progressbar, w_object_t *parent)
{
	(*progressbar) = (w_progressbar_t *) s_malloc(sizeof(w_progressbar_t));
	
	if (w_frame_init(window, &((*progressbar)->frame), FRAME_PANEL | FRAME_RAISED, parent )) {
		goto err0;
	}
	(*progressbar)->progressbarstatus=0;
	w_textbox_init((*progressbar)->frame->object->window, &((*progressbar)->textbox), 
						(*progressbar)->frame->object);	
						
	(*progressbar)->textbox->frame->style = FRAME_LINEEDITPANEL | FRAME_SUNKEN;
						
	s_handler_init(&((*progressbar)->handler));
	(*progressbar)->handler->type = MOUSE_HANDLER;
	(*progressbar)->handler->mouse.x = 0;
	(*progressbar)->handler->mouse.y = 0;
	(*progressbar)->handler->mouse.w = 0;
	(*progressbar)->handler->mouse.h = 0;
	(*progressbar)->handler->mouse.button = ~MOUSE_NONEBUTTON;
	(*progressbar)->handler->user_data = *progressbar;	
	(*progressbar)->state = 0;

	(*progressbar)->frame->object->data[OBJECT_PROGRESSBAR] = *progressbar;
	(*progressbar)->frame->object->draw = w_progressbar_draw;
	(*progressbar)->frame->object->geometry = w_progressbar_geometry;
	(*progressbar)->frame->object->destroy = w_progressbar_uninit;
		
	return 0;
err0:	s_free(*progressbar);
		printf("error occured in init progressbar\n");
	return -1;
}

void w_progressbar_uninit (w_object_t *object)
{
	w_progressbar_t *progressbar;
		
	progressbar = (w_progressbar_t *) object->data[OBJECT_PROGRESSBAR];
	w_textbox_uninit(progressbar->textbox->frame->object);
	w_frame_uninit(progressbar->frame->object);
	s_handler_uninit(progressbar->handler);
	s_free(progressbar);	
}
