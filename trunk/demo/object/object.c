/***************************************************************************
    begin                : Wed Mar 9 2005
    copyright            : (C) 2005 - 2006 by Alper Akcan
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "xynth_.h"

typedef struct main_data_s {
	s_object_t *objectp;
	s_object_t *objectc[4];
	s_object_t *objectcc[4];
} main_data_t;

static void object_draw_black (s_object_t *object)
{
	s_fillbox(object->surface, 0, 0, object->surface->width, object->surface->height, s_rgbcolor(object->surface, 0, 0, 0));
}

static void object_draw_red (s_object_t *object)
{
	s_fillbox(object->surface, 0, 0, object->surface->width, object->surface->height, s_rgbcolor(object->surface, 255, 0, 0));
}

static void object_draw_green (s_object_t *object)
{
	s_fillbox(object->surface, 0, 0, object->surface->width, object->surface->height, s_rgbcolor(object->surface, 0, 255, 0));
}

static void object_draw_blue (s_object_t *object)
{
	s_fillbox(object->surface, 0, 0, object->surface->width, object->surface->height, s_rgbcolor(object->surface, 0, 0, 255));
}

static void object_draw_white (s_object_t *object)
{
	s_fillbox(object->surface, 0, 0, object->surface->width, object->surface->height, s_rgbcolor(object->surface, 255, 255, 255));
}

typedef void (*object_draw) (s_object_t *);
object_draw object_draw_p[] = {
	&object_draw_red,
	&object_draw_green,
	&object_draw_blue,
	&object_draw_white,
	&object_draw_black
};

static void handler0_p (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
        main_data_t *mdata = (main_data_t *) window->client->user_data;
	s_putbox(window->surface, 10, 50, mdata->objectp->surface->width, mdata->objectp->surface->height, mdata->objectp->surface->vbuf);
}

static void handler1_p (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	int i;
	int j;
	int w;
	int h;
        main_data_t *mdata = (main_data_t *) window->client->user_data;

	for (i = 0; i < 4; i++) {
		w = (mdata->objectc[i]->parent->surface->width) ? mdata->objectc[i]->parent->surface->width : 1;
		h = (mdata->objectc[i]->parent->surface->height) ? mdata->objectc[i]->parent->surface->height : 1;
		s_object_move(mdata->objectc[i], ((unsigned) rand()) % w,
		                                 ((unsigned) rand()) % h,
		                                 ((unsigned) rand()) % w,
		                                 ((unsigned) rand()) % h
		                                 );
		if (i == 0) {
			for (j = 0; j < 4; j++) {
				w = (mdata->objectcc[j]->parent->surface->width) ? mdata->objectcc[j]->parent->surface->width : 1;
				h = (mdata->objectcc[j]->parent->surface->height) ? mdata->objectcc[j]->parent->surface->height : 1;
				s_object_move(mdata->objectcc[j], ((unsigned) rand()) % w,
			        	                          ((unsigned) rand()) % h,
			                		 	  ((unsigned) rand()) % w,
				                                  ((unsigned) rand()) % h
				                                  );
			}
		}
	}

	handler0_p(window, event, handler);
}

static void handler2_p (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
	int i;
        main_data_t *mdata = (main_data_t *) window->client->user_data;

        i = rand() % 4;
	s_object_show(mdata->objectc[i]);
	s_object_show(mdata->objectcc[i]);

	handler0_p(window, event, handler);
}

static void handler3_p (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
        main_data_t *mdata = (main_data_t *) window->client->user_data;

	s_thread_mutex_lock(mdata->objectc[0]->mut);
	s_fillbox(mdata->objectc[0]->surface, 10,
	                                      10,
	                                      mdata->objectc[0]->surface->width - 20,
	                                      mdata->objectc[0]->surface->height - 20,
	                                      s_rgbcolor(mdata->objectc[0]->surface, rand() % 255, rand() % 255, rand() % 255));
	s_object_update(mdata->objectc[0], mdata->objectc[0]->surface->win);
	s_thread_mutex_unlock(mdata->objectc[0]->mut);
	handler0_p(window, event, handler);
}

static int hide_show = 0;
static void handler4_p (s_window_t *window, s_event_t *event, s_handler_t *handler)
{
        main_data_t *mdata = (main_data_t *) window->client->user_data;
	if (hide_show++ % 2) {
		s_object_show(mdata->objectc[0]);
	} else {
		s_object_hide(mdata->objectc[0]);
	}
	handler0_p(window, event, handler);
}

static void print_object (s_window_t *window, s_object_t *object)
{
	int i;
	int j;
        main_data_t *mdata = (main_data_t *) window->client->user_data;
        if (object == mdata->objectp) {
        	printf("root\n");
        }
	for (i = 0; i < 4; i++) {
		if (mdata->objectc[i] == object) {
			printf("big: %s\n", (i == 0) ? "red" : (i == 1) ? "green" : (i == 2) ? "blue" : "white");
		}
		if (i == 0) {
			for (j = 0; j < 4; j++) {
				if (mdata->objectcc[j] == object) {
					printf("small: %s\n", (j == 0) ? "green" : (j == 1) ? "blue" : (j == 2) ? "white" : "black");
				}
			}
		}
	}
}

static void object_atevent (s_window_t *window, s_event_t *event)
{
	s_object_t *object;
        main_data_t *mdata = (main_data_t *) window->client->user_data;
	if (event->type & MOUSE_EVENT &&
	    event->type & MOUSE_PRESSED &&
	    event->mouse->b == MOUSE_LEFTBUTTON) {
	    	s_object_atposition(mdata->objectp, event->mouse->x - window->surface->buf->x - 10, event->mouse->y -window->surface->buf->y - 50, &object);
	    	print_object(window, object);
	    	
	}
}

int main (int argc, char *argv[])
{
        int i = 0;
        int j = 0;
	int x = 60;
	int y = 60;
	int w = 400;
	int h = 300;
	int mw = 1000;
	int mh = 1000;
	s_handler_t *hndl;
	s_window_t *window;
	main_data_t *mdata;

	srand(time(NULL));

	s_client_init(&window);

	s_window_new(window, WINDOW_MAIN, NULL);
	mw = window->surface->width;
	mh = window->surface->height;

	s_window_set_title(window, "Demo - %s", argv[0]);
	s_window_set_coor(window, 0, x, y, MIN(mw, w), MIN(mh, h));

	s_fillbox(window->surface, 0, 0, 20, 20, s_rgbcolor(window->surface, 222, 222, 222));
	s_fillbox(window->surface, 2, 2, 16, 16, s_rgbcolor(window->surface, 255, 0, 0));
	s_handler_init(&hndl);
	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = 0;
	hndl->mouse.y = 0;
	hndl->mouse.w = 20;
	hndl->mouse.h = 20;
	hndl->mouse.p = handler0_p;
	hndl->mouse.button = MOUSE_LEFTBUTTON;
	s_handler_add(window, hndl);

	s_fillbox(window->surface, 20, 0, 20, 20, s_rgbcolor(window->surface, 222, 222, 222));
	s_fillbox(window->surface, 22, 2, 16, 16, s_rgbcolor(window->surface, 255, 0, 0));
	s_handler_init(&hndl);
	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = 20;
	hndl->mouse.y = 0;
	hndl->mouse.w = 20;
	hndl->mouse.h = 20;
	hndl->mouse.p = handler1_p;
	hndl->mouse.button = MOUSE_LEFTBUTTON;
	s_handler_add(window, hndl);

	s_fillbox(window->surface, 40, 0, 20, 20, s_rgbcolor(window->surface, 222, 222, 222));
	s_fillbox(window->surface, 42, 2, 16, 16, s_rgbcolor(window->surface, 255, 0, 0));
	s_handler_init(&hndl);
	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = 40;
	hndl->mouse.y = 0;
	hndl->mouse.w = 20;
	hndl->mouse.h = 20;
	hndl->mouse.p = handler2_p;
	hndl->mouse.button = MOUSE_LEFTBUTTON;
	s_handler_add(window, hndl);

	s_fillbox(window->surface, 60, 0, 20, 20, s_rgbcolor(window->surface, 222, 222, 222));
	s_fillbox(window->surface, 62, 2, 16, 16, s_rgbcolor(window->surface, 255, 0, 0));
	s_handler_init(&hndl);
	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = 60;
	hndl->mouse.y = 0;
	hndl->mouse.w = 20;
	hndl->mouse.h = 20;
	hndl->mouse.p = handler3_p;
	hndl->mouse.button = MOUSE_LEFTBUTTON;
	s_handler_add(window, hndl);

	s_fillbox(window->surface, 80, 0, 20, 20, s_rgbcolor(window->surface, 222, 222, 222));
	s_fillbox(window->surface, 82, 2, 16, 16, s_rgbcolor(window->surface, 255, 0, 0));
	s_handler_init(&hndl);
	hndl->type = MOUSE_HANDLER;
	hndl->mouse.x = 80;
	hndl->mouse.y = 0;
	hndl->mouse.w = 20;
	hndl->mouse.h = 20;
	hndl->mouse.p = handler4_p;
	hndl->mouse.button = MOUSE_LEFTBUTTON;
	s_handler_add(window, hndl);

	mdata = (main_data_t *) s_malloc(sizeof(main_data_t));
	window->client->user_data = (void *) mdata;
	
	s_object_init(window, &(mdata->objectp), 500, 500, object_draw_black, NULL);
	for (i = 0; i < 4; i++) {
		s_object_init(window, &(mdata->objectc[i]), 100, 100, object_draw_p[i], mdata->objectp);
		if (i == 0) {
			for (j = 0; j < 4; j++) {
				s_object_init(window, &(mdata->objectcc[j]), 20, 20, object_draw_p[j + 1], mdata->objectc[i]);
			}
		}
	}

	s_window_set_coor(window, WINDOW_NOFORM, window->surface->buf->x, window->surface->buf->y,
	                                         mdata->objectp->surface->width + 20, mdata->objectp->surface->height + 60);
        s_fillbox(window->surface, 0, 40, mdata->objectp->surface->width + 20, mdata->objectp->surface->height + 20, s_rgbcolor(window->surface, 222, 222, 222));

	s_client_atevent(window, object_atevent);

	s_window_show(window);
	s_client_main(window);

	s_object_uninit(mdata->objectp);
	s_free(mdata);

	return 0;
}

#if defined(SINGLE_APP)
s_single_app_t single_object = {
	object_main,
	1,
	{"object"}
};
#endif
