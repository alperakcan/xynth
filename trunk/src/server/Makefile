
-include ../../.config

subdir-$(CONFIG_VIDEO_FBDEV)    = ./video/fb
subdir-$(CONFIG_VIDEO_SDL)     += ./video/sdl
subdir-$(CONFIG_VIDEO_SVGALIB) += ./video/svga
subdir-$(CONFIG_VIDEO_VESA)    += ./video/vesa
subdir-$(CONFIG_VIDEO_PSPDEV)  += ./video/pspdev
subdir-$(CONFIG_VIDEO_GDI)     += ./video/gdi
subdir-$(CONFIG_VIDEO_GP2X)    += ./video/gp2x
subdir-$(CONFIG_VIDEO_CACA)    += ./video/caca
subdir-$(CONFIG_VIDEO_NULL)    += ./video/null
subdir-$(CONFIG_VIDEO_HELPER)  += ./video/helper

target-y  = xynth

xynth_files-y = event.c \
                id.c \
                irr.c \
                kbd.c \
                mouse.c \
                priority.c \
                server.c \
                single.c \
                socket.c \
                start.c \
                surface.c \
                theme.c \
                window.c \
                window_handler.c \
                window_move_resize.c

xynth_files-$(CONFIG_VIDEO_FBDEV)   += ./video/fb/video_fb.o
xynth_files-$(CONFIG_VIDEO_SDL)     += ./video/sdl/video_sdl.o
xynth_files-$(CONFIG_VIDEO_SVGALIB) += ./video/svga/video_svga.o
xynth_files-$(CONFIG_VIDEO_VESA)    += ./video/vesa/video_vesa.o
xynth_files-$(CONFIG_VIDEO_PSPDEV)  += ./video/pspdev/video_pspdev.o
xynth_files-$(CONFIG_VIDEO_GDI)     += ./video/gdi/video_gdi.o
xynth_files-$(CONFIG_VIDEO_GP2X)    += ./video/gp2x/video_gp2x.o
xynth_files-$(CONFIG_VIDEO_CACA)    += ./video/caca/video_caca.o
xynth_files-$(CONFIG_VIDEO_NULL)    += ./video/null/video_null.o
xynth_files-$(CONFIG_VIDEO_HELPER)  += ./video/helper/video_helper.o

ifeq ($(CONFIG_SINGLE_APPLICATION), y)
xynth_files-$(CONFIG_DEMO_CHILD)    += ../../demo/child/single_child.o
xynth_files-$(CONFIG_DEMO_DESKTOP)  += ../../demo/desktop/single_desktop.o
xynth_files-$(CONFIG_DEMO_HIDE)     += ../../demo/hide/single_hide.o
xynth_files-$(CONFIG_DEMO_LOAD)     += ../../demo/load/single_load.o
xynth_files-$(CONFIG_DEMO_SIMPLE)   += ../../demo/simple/single_simple.o
xynth_files-$(CONFIG_DEMO_OBJECT)   += ../../demo/object/single_object.o
xynth_files-$(CONFIG_DEMO_TEMP)     += ../../demo/temp/single_temp.o
xynth_files-$(CONFIG_DEMO_TERM)     += ../../demo/term/single_term.o
xynth_files-$(CONFIG_DEMO_TIMER)    += ../../demo/timer/single_timer.o
xynth_files-$(CONFIG_DEMO_LOGOUT)   += ../../demo/xynthlogout/single_xynthlogout.o
xynth_files-$(CONFIG_DEMO_IMAGE)    += ../../demo/img/single_img.o
xynth_files-$(CONFIG_DEMO_ONSCREENKEYBOARD) += ../../demo/onscreenkeyboard/single_onscreenkeyboard.o
xynth_files-$(CONFIG_DEMO_WIDGETX)  += ../../demo/widgetx/single_widgetx.o
endif

xynth_includes-$(CONFIG_VIDEO_PSPDEV) += $(shell psp-config -p)/include
xynth_includes-y  += ./ ../lib

xynth_libraries-y += ../lib
xynth_libraries-$(CONFIG_VIDEO_PSPDEV) += $(shell psp-config -p)/lib

xynth_ldflags-y += -lxynth
xynth_ldflags-y += -lm
xynth_ldflags-$(CONFIG_VIDEO_SDL)   += $(shell sdl-config --libs)
xynth_ldflags-$(CONFIG_VIDEO_CACA)  += -lcaca

ifeq ($(CONFIG_WIDGET_LIBRARY), y)
xynth_libraries-y += ../widget
xynth_ldflags-y   += -lwidget
endif

ifeq ($(CONFIG_THEME_PLUGIN), y)
xynth_ldflags-y   += -ldl
endif

xynth_ldflags-$(CONFIG_VIDEO_PSPDEV) += -lpspdebug \
                                        -lpspdisplay \
                                        -lpspge \
                                        -lpspctrl \
                                        -lpspsdk \
                                        -lc \
                                        -lpspnet \
                                        -lpspnet_inet \
                                        -lpsputility \
                                        -lpspuser \
                                        -lpspkernel

include ../../Makefile.lib

dist:
ifeq ($(CONFIG_PLATFORM_PSPDEV), y)
	psp-fixup-imports xynth
	mksfo 'Xynth Windowing System' param.sfo
	$(MKDIR) ../../dist/$(CONFIG_PATH_BIN)/xynth
	psp-strip xynth -o ../../dist/$(CONFIG_PATH_BIN)/xynth/eboot.pbp
	$(MKDIR) ../../dist/$(CONFIG_PATH_BIN)/xynth%
	pack-pbp ../../dist/$(CONFIG_PATH_BIN)/xynth%/eboot.pbp \
	         param.sfo \
	         ../../tools/pspdev/icon0.png \
	         NULL \
	         NULL \
	         ../../tools/pspdev/pic1.png \
	         NULL \
	         NULL \
	         NULL
	rm param.sfo
else
	$(CP) xynth ../../dist/$(CONFIG_PATH_BIN)/xynth
endif
