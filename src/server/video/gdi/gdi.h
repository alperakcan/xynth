/***************************************************************************
    begin                : Fri Dec 30 2005
    copyright            : (C) 2005 - 2007 by Caglar Oral
    email                : caglaroral@yahoo.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#if defined(VIDEO_GDI)

#include <windows.h>

#define GDI_MAX_KEYS 256

typedef struct s_video_gdi_data_s {
	HWND hwndMain;
	HINSTANCE hinst;
	DEVMODE dmode;
	int fullscreen;
	s_thread_t *event_tid;
	int keybd_fd[2];
	int mouse_fd[2];
	int bpp_windows;
	s_video_input_data_keybd_t keybd;
	s_video_input_data_mouse_t mouse;
	S_KEYCODE_CODE keymap[GDI_MAX_KEYS];
} s_video_gdi_data_t;

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

void * s_video_gdi_create_window (void *arg);

int s_video_gdi_kbd_init (s_server_conf_t *cfg);
int s_video_gdi_kbd_update (s_video_input_data_t *keybd);
void s_video_gdi_kbd_uninit (void);
void s_video_gdi_kbd_switch (int vt);

int s_video_gdi_mouse_init (s_server_conf_t *cfg);
int s_video_gdi_mouse_update (s_video_input_data_t *mouse);
void s_video_gdi_mouse_uninit (void);

void s_video_gdi_server_uninit (void);
int s_video_gdi_server_init (s_server_conf_t *cfg);
void s_video_gdi_server_surface_update (s_rect_t *coor);
void s_video_gdi_server_fullscreen (void);

#endif /* VIDEO_GDI */
