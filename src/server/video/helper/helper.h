/***************************************************************************
    begin                : Thu Oct 9 2003
    copyright            : (C) 2003 - 2008 by Alper Akcan
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

#include "config.h"

#if defined(CONFIG_VIDEO_HELPER_CONSOLE)
/* console.c */
int s_video_helper_console_init (s_server_conf_t *cfg);
int s_video_helper_console_switch (int vt);
#endif /* CONFIG_VIDEO_HELPER_CONSOLE */

#if defined(CONFIG_VIDEO_HELPER_IRMAN)
int s_video_helper_irman_init (char *port);
char * s_video_helper_irman_getcode (void);
int s_video_helper_irman_uninit (void);
#endif /* CONFIG_VIDEO_HERLPER_IRMAN */

#if defined(CONFIG_VIDEO_HELPER_IRAIR)
int s_video_helper_irair_init (void);
int s_video_helper_irair_uninit (void);
char * s_video_helper_irair_getcode (void);
#endif /* CONFIG_VIDEO_HELPER_IRAIR */

#if defined(CONFIG_VIDEO_HELPER_KEYBOARD)
/* kbd.c */
void s_video_helper_kbd_set_attr (void);
int s_video_helper_kbd_init (s_server_conf_t *cfg, int kfd);
int s_video_helper_kbd_update (s_video_input_data_t *keybd);
void s_video_helper_kbd_uninit (void);
#endif /* CONFIG_VIDEO_HELPER_KEYBOARD */

#if defined(CONFIG_VIDEO_HELPER_MODES)
/* modes.c */

typedef enum {
	TEXT,			/*   0 */
	M320x200x16,		/*   1 */
	M640x200x16,		/*   2 */
	M640x350x16,		/*   3 */
	M640x480x16,		/*   4 */
	M320x200x256,		/*   5 */
	M320x240x256,		/*   6 */
	M320x400x256,		/*   7 */
	M360x480x256,		/*   8 */
	M640x480x2,		/*   9 */
	M640x480x256,		/*  10 */
	M800x600x256,		/*  11 */
	M1024x768x256,		/*  12 */
	M1280x1024x256,		/*  13 */
	M320x200x32K,		/*  14 */
	M320x200x64K,		/*  15 */
	M320x200x16M,		/*  16 */
	M640x480x32K,		/*  17 */
	M640x480x64K,		/*  18 */
	M640x480x16M,		/*  19 */
	M800x600x32K,		/*  20 */
	M800x600x64K,		/*  21 */
	M800x600x16M,		/*  22 */
	M1024x768x32K,		/*  23 */
	M1024x768x64K,		/*  24 */
	M1024x768x16M,		/*  25 */
	M1280x1024x32K,		/*  26 */
	M1280x1024x64K,		/*  27 */
	M1280x1024x16M,		/*  28 */
	M800x600x16,		/*  29 */
	M1024x768x16,		/*  30 */
	M1280x1024x16,		/*  31 */
	M720x348x2,		/*  32 */
	M320x200x16M32,		/*  33 */
	M640x480x16M32,		/*  34 */
	M800x600x16M32,		/*  35 */
	M1024x768x16M32,	/*  36 */
	M1280x1024x16M32,	/*  37 */
	M1152x864x16,		/*  38 */
	M1152x864x256,		/*  39 */
	M1152x864x32K,		/*  40 */
	M1152x864x64K,		/*  41 */
	M1152x864x16M,		/*  42 */
	M1152x864x16M32,	/*  43 */
	M1600x1200x16,		/*  44 */
	M1600x1200x256,		/*  45 */
	M1600x1200x32K,		/*  46 */
	M1600x1200x64K,		/*  47 */
	M1600x1200x16M,		/*  48 */
	M1600x1200x16M32,	/*  49 */
	M320x240x256V,		/*  50 */
	M320x240x32K,		/*  51 */
	M320x240x64K,		/*  52 */
	M320x240x16M,		/*  53 */
	M320x240x16M32,		/*  54 */
	M400x300x256,		/*  55 */
	M400x300x32K,		/*  56 */
	M400x300x64K,		/*  57 */
        M400x300x16M,		/*  58 */
        M400x300x16M32,		/*  59 */
        M512x384x256,		/*  60 */
        M512x384x32K,		/*  61 */
        M512x384x64K,		/*  62 */
        M512x384x16M,		/*  63 */
        M512x384x16M32,		/*  64 */
        M960x720x256,		/*  65 */
        M960x720x32K,		/*  66 */
        M960x720x64K,		/*  67 */
        M960x720x16M,		/*  68 */
        M960x720x16M32,		/*  69 */
        M1920x1440x256,		/*  70 */
        M1920x1440x32K,		/*  71 */
        M1920x1440x64K,		/*  72 */
        M1920x1440x16M,		/*  73 */
        M1920x1440x16M32,	/*  74 */
        M320x400x256V,		/*  75 */
        M320x400x32K,		/*  76 */
        M320x400x64K,		/*  77 */
        M320x400x16M,		/*  78 */
        M320x400x16M32,		/*  79 */
        M640x400x256,		/*  80 */
        M640x400x32K,		/*  81 */
        M640x400x64K,		/*  82 */
        M640x400x16M,		/*  83 */
        M640x400x16M32,		/*  84 */
        M320x480x256,		/*  85 */
        M320x480x32K,		/*  86 */
        M320x480x64K,		/*  87 */
        M320x480x16M,		/*  88 */
        M320x480x16M32,		/*  89 */
        M720x540x256,		/*  90 */
        M720x540x32K,		/*  91 */
	M720x540x64K,		/*  92 */
	M720x540x16M,		/*  93 */
	M720x540x16M32,		/*  94 */
	M848x480x256,		/*  95 */
	M848x480x32K,		/*  96 */
	M848x480x64K,		/*  97 */
	M848x480x16M,		/*  98 */
	M848x480x16M32,		/*  99 */
	M1072x600x256,		/* 100 */
	M1072x600x32K,		/* 101 */
	M1072x600x64K,		/* 102 */
	M1072x600x16M,		/* 103 */
	M1072x600x16M32,	/* 104 */
	M1280x720x256,		/* 105 */
	M1280x720x32K,		/* 106 */
	M1280x720x64K,		/* 107 */
	M1280x720x16M,		/* 108 */
	M1280x720x16M32,	/* 109 */
	M1360x768x256,		/* 110 */
	M1360x768x32K,		/* 111 */
	M1360x768x64K,		/* 112 */
	M1360x768x16M,		/* 113 */
	M1360x768x16M32,	/* 114 */
	M1800x1012x256,		/* 115 */
	M1800x1012x32K,		/* 116 */
	M1800x1012x64K,		/* 117 */
	M1800x1012x16M,		/* 118 */
	M1800x1012x16M32,	/* 119 */
	M1920x1080x256,		/* 120 */
	M1920x1080x32K,		/* 121 */
	M1920x1080x64K,		/* 122 */
	M1920x1080x16M,		/* 123 */
	M1920x1080x16M32,	/* 124 */
	M2048x1152x256,		/* 125 */
	M2048x1152x32K,		/* 126 */
	M2048x1152x64K,		/* 127 */
	M2048x1152x16M,		/* 128 */
	M2048x1152x16M32,	/* 129 */
	M2048x1536x256,		/* 130 */
	M2048x1536x32K,		/* 131 */
	M2048x1536x64K,		/* 132 */
	M2048x1536x16M,		/* 133 */
	M2048x1536x16M32,	/* 134 */
	M512x480x256,		/* 135 */
	M512x480x32K,		/* 136 */
	M512x480x64K,		/* 137 */
	M512x480x16M,		/* 138 */
	M512x480x16M32,		/* 139 */
	M400x600x256,		/* 140 */
	M400x600x32K,		/* 141 */
	M400x600x64K,		/* 142 */
	M400x600x16M,		/* 143 */
	M400x600x16M32,		/* 144 */
	M400x300x256X,		/* 145 */
	M320x200x256V,		/* 146 */
	M480x272x256,		/* 147 */
	M480x272x32K,		/* 148 */
	M480x272x64K,		/* 149 */
	M480x272x16M,		/* 150 */
	M480x272x16M32,		/* 151 */
	M240x320x256,		/* 152 */
	M240x320x32K,		/* 153 */
	M240x320x64K,		/* 154 */
	M240x320x16M,		/* 155 */
	M240x320x16M32,		/* 156 */
	M320x240x256K,		/* 157 */
	M720x480x64K,		/* 158 */
	M640x448x64K,		/* 159 */
	M1280x800x64K,		/* 160 */
	M720x576x64K,		/* 161 */
	M640x512x64K,		/* 162 */
	MAX_MODES,		/* 163 */
} GENERIC_MODES;

typedef struct s_video_helper_mode_info_s {
	int xdim;
	int ydim;
	int colors;
	int xbytes;
	int bytesperpixel;
	char *name;
	GENERIC_MODES number;
} s_video_helper_mode_info_t;

int s_video_helper_mode_find (s_server_conf_t *cfg, s_video_helper_mode_info_t **gmode);
#endif /* CONFIG_VIDEO_HELPER_MODES */

#if defined(CONFIG_VIDEO_HELPER_MOUSE)
/* mouse.c */
int s_video_helper_mouse_init (s_server_conf_t *cfg);
int s_video_helper_mouse_update (s_video_input_data_t *mouse);
void s_video_helper_mouse_uninit (void);
void s_video_helper_mouse_setxrange (int x1, int x2);
void s_video_helper_mouse_setyrange (int y1, int y2);
#endif /* CONFIG_VIDEO_HELPER_MOUSE */

#if defined(CONFIG_VIDEO_HELPER_MTRR)
/* mtrr.c */
void s_video_helper_mtrr_add (unsigned int base, unsigned int size);
#endif /* CONFIG_VIDEO_HELPER_MTRR */

#if defined(CONFIG_VIDEO_HELPER_TSCREEN)
/* tscreen.c */
int s_video_helper_touchscreen_init (s_server_conf_t *cfg);
int s_video_helper_touchscreen_update (s_video_input_data_mouse_t *mouse);
void s_video_helper_touchscreen_uninit (void);
#endif /* CONFIG_VIDEO_HELPER_TSCREEN */
