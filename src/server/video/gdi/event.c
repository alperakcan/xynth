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

#include "../../../lib/xynth_.h"
#include "server.h"
#include "gdi.h"

LRESULT CALLBACK MainWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
        int k = 0;
        int m = 0;
	s_rect_t coor;
	BYTE chars[2];
        BYTE keystate[256];
	s_video_gdi_data_t *priv = (s_video_gdi_data_t *) server->driver->driver_data;

	switch (message) {
		case WM_DESTROY:
			s_server_quit(server->window);
			PostQuitMessage(0);
                        break;
		case WM_CREATE:
                        ShowCursor(FALSE);
		case WM_PAINT:
		case WM_DISPLAYCHANGE:
			coor.x = 0;
			coor.y = 0;
			coor.w = server->window->surface->width;
			coor.h = server->window->surface->height;
			s_video_gdi_server_surface_update(&coor);
			break;
		case WM_LBUTTONDOWN:
                        priv->mouse.buttons |= MOUSE_LEFTBUTTON;
                        goto mouse_event;
		case WM_LBUTTONUP:
                        priv->mouse.buttons &= ~MOUSE_LEFTBUTTON;
                        goto mouse_event;
		case WM_RBUTTONDOWN:
                        priv->mouse.buttons |= MOUSE_RIGHTBUTTON;
                        goto mouse_event;
		case WM_RBUTTONUP:
                        priv->mouse.buttons &= ~MOUSE_RIGHTBUTTON;
                        goto mouse_event;
                case WM_MOUSEMOVE:
mouse_event:            priv->mouse.x = (((int) lParam) & 0xFFFF);
                        priv->mouse.y = ((((int) lParam) >> 16) & 0xFFFF);
			if (priv->mouse_fd[1] != -1) {
				s_pipe_api_write(priv->mouse_fd[1], &m, sizeof(int));
			}
                        break;
                case WM_KEYUP:
                case WM_SYSKEYUP:
                        priv->keybd.state = KEYBD_RELEASED;
                        goto keybd_event;
                case WM_KEYDOWN:
                case WM_SYSKEYDOWN:
                        priv->keybd.state = KEYBD_PRESSED;
keybd_event:            GetKeyboardState(keystate);
                        priv->keybd.scancode = (((unsigned int) lParam) >> 16) & 0xFF;
                        priv->keybd.button = priv->keymap[priv->keybd.scancode];
                        priv->keybd.keycode = priv->keymap[priv->keybd.scancode];
                        priv->keybd.ascii = wParam;
                        if (ToAscii(wParam, lParam, keystate, (WORD *) chars, 0) == 1) {
                                priv->keybd.ascii = chars[0];
                        }
			if (priv->keybd_fd[1] != -1) {
	         		s_pipe_api_write(priv->keybd_fd[1], &k, sizeof(int));
          		}
                        break;
//		default:
//      		printf("%d\n", message);
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

void s_video_gdi_server_fullscreen (void)
{
	RECT rect;
        DEVMODE settings;
	s_video_gdi_data_t *priv = (s_video_gdi_data_t *) server->driver->driver_data;
	
	if (priv->fullscreen == 0) {
                EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &priv->dmode);
                memset(&settings, 0, sizeof(DEVMODE));
                settings.dmSize = sizeof(DEVMODE);
                settings.dmBitsPerPel = priv->bpp_windows;
                settings.dmPelsWidth = server->window->surface->width;
                settings.dmPelsHeight = server->window->surface->height;
                settings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
        } else {
                settings.dmSize = sizeof(DEVMODE);
                settings.dmBitsPerPel = priv->dmode.dmBitsPerPel;
                settings.dmPelsWidth = priv->dmode.dmPelsWidth;
                settings.dmPelsHeight = priv->dmode.dmPelsHeight;
                settings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
        }

        switch (ChangeDisplaySettings(&settings, CDS_FULLSCREEN)) {
                case DISP_CHANGE_SUCCESSFUL:
                        break;
                case DISP_CHANGE_BADFLAGS:
                case DISP_CHANGE_BADMODE:
                case DISP_CHANGE_BADPARAM:
                case DISP_CHANGE_FAILED:
                case DISP_CHANGE_NOTUPDATED:
                case DISP_CHANGE_RESTART:
                        break;
        }

	if (priv->fullscreen == 0) {
        	rect.left = 0;
        	rect.right = server->window->surface->width;
        	rect.top = 0;
        	rect.bottom = server->window->surface->height;
                AdjustWindowRect(&rect, GetWindowLong(priv->hwndMain, GWL_STYLE), FALSE);
                SetWindowPos(priv->hwndMain, HWND_TOPMOST, rect.left, rect.top, rect.right + 100, rect.bottom + 100, 0);
                priv->fullscreen = 1;
        } else {
        	rect.left = 0;
        	rect.right = server->window->surface->width;
        	rect.top = 0;
        	rect.bottom = server->window->surface->height;
                AdjustWindowRect(&rect, GetWindowLong(priv->hwndMain, GWL_STYLE), FALSE);
                SetWindowPos(priv->hwndMain, HWND_NOTOPMOST, 0, 0, rect.right - rect.left, rect.bottom - rect.top, 0);
                priv->fullscreen = 0;
        }

	UpdateWindow(priv->hwndMain);
}

void * s_video_gdi_create_window (void *arg)
{
	MSG msg;
	RECT rect;
	HDC mainwindow;
	WNDCLASSEX wndclass;
	char *szMainWndClass = "XynthWindowingSystem";
	s_video_gdi_data_t *priv = (s_video_gdi_data_t *) server->driver->driver_data;

	memset(&wndclass, 0, sizeof(WNDCLASSEX));
	wndclass.lpszClassName = szMainWndClass;
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = MainWndProc;
	wndclass.hInstance = priv->hinst;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	RegisterClassEx(&wndclass);
	
	priv->hwndMain = CreateWindow(szMainWndClass,
	                              "Xynth Windowing system",
                                      WS_POPUP | WS_BORDER | WS_SYSMENU | WS_MINIMIZEBOX | WS_CAPTION,
	                              CW_USEDEFAULT,
	                              CW_USEDEFAULT,
	                              CW_USEDEFAULT,
	                              CW_USEDEFAULT,
	                              NULL,
	                              NULL,
	                              priv->hinst,
	                              NULL);

	rect.left = 0;
	rect.right = server->window->surface->width;
	rect.top = 0;
	rect.bottom = server->window->surface->height;
        AdjustWindowRect(&rect, GetWindowLong(priv->hwndMain, GWL_STYLE), FALSE);
        SetWindowPos(priv->hwndMain, HWND_TOP, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE);

	ShowWindow(priv->hwndMain, TRUE);
	UpdateWindow(priv->hwndMain);
	
	mainwindow = GetDC(priv->hwndMain);
	if (mainwindow == NULL) {
                debugf(DSER | DFAT, "What the fuck is this?");
		return NULL;
	}
	priv->bpp_windows = GetDeviceCaps(mainwindow, BITSPIXEL);
	if (priv->bpp_windows == 24) {
                debugf(DSER | DFAT, "24 bitsperpixel modes are not supported by Xynth Windowing System. "
                                    "Try changing display mode from "
                                    "Display Properties -> Settings -> Color quality.");
        }

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (void *) msg.wParam;
}

#endif /* VIDEO_GDI */
