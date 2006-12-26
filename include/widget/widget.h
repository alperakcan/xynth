/***************************************************************************
    begin                : Mon Sep 20 2004
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

#ifndef W_WIDGET_H_
#define W_WIDGET_H_

/* reduce memory usage, this is a bit hacky
 * some speed consumptions ;|
 * 
 * levels;
 * 	0: no memory optimizations, memory hungary but faster
 * 	1: disables image caching, reduces memory usage, especially
 * 	   for the applications that have several images
 * 	2: will free hided object's and its childs' buffers (vbuf and matrix)
 * 	   slowers show() hide() processes, but cute.
 * 	3: much more memory optimization, too slower, draws everthing on the fly.
 */
#define WIDGET_OPTIMIZE_MEMORY 2

typedef struct w_object_s w_object_t;
typedef struct w_button_s w_button_t;
typedef struct w_checkbox_s w_checkbox_t;
typedef struct w_editbox_s w_editbox_t;
typedef struct w_frame_image_s w_frame_image_t;
typedef struct w_frame_s w_frame_t;
typedef struct w_progressbar_s w_progressbar_t;
typedef struct w_scrollbufferbar_s w_scrollbufferbar_t;
typedef struct w_scrollbuffer_s w_scrollbuffer_t;
typedef struct w_textbox_s w_textbox_t;
typedef struct w_window_s w_window_t;

typedef struct w_effect_s w_effect_t;
typedef struct w_signal_s w_signal_t;

#include "widget/object.h"
#include "widget/frame.h"
#include "widget/button.h"
#include "widget/textbox.h"
#include "widget/editbox.h"
#include "widget/checkbox.h"
#include "widget/progressbar.h"
#include "widget/scrollbuffer.h"
#include "widget/window.h"

#endif /* W_WIDGET_H_ */
