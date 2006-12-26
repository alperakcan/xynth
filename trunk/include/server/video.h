/***************************************************************************
    begin                : Sun Feb 16 2003
    copyright            : (C) 2003 - 2006 by Alper Akcan
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

#ifndef S_VIDEO_H_
#define S_VIDEO_H_

typedef enum {
	VIDEO_INPUT_NONE,
	VIDEO_INPUT_MOUSE,
	VIDEO_INPUT_KEYBD,
	VIDEO_INPUT_IRR,
} VIDEO_INPUT;

struct s_video_input_data_mouse_s {
	int x;
	int y;
	int buttons;
};

struct s_video_input_data_keybd_s {
	int state;
	int button;
	int keycode;
	int scancode;
	int ascii;
};

union s_video_input_data_u {
	s_video_input_data_mouse_t mouse;
	s_video_input_data_keybd_t keybd;
	s_video_input_data_keybd_t irr;
};

struct s_video_input_s {
	VIDEO_INPUT type;
	int (*init) (s_server_conf_t *cfg);
	int (*update) (s_video_input_data_t *idata);
	void (*uninit) (void);
};

struct s_video_driver_s {
	char *driver;
	char *device;

	s_video_input_t **input;

	int (*server_init) (s_server_conf_t *cfg);
	void (*server_uninit) (void);
	void (*server_goto_back) (void);
	void (*server_comefrom_back) (void);
	void (*server_restore) (void);
	void (*server_surface_update) (s_rect_t *coor);
	void (*server_fullscreen) (void);

	void *driver_data;
};

#endif /* S_VIDEO_H_ */
