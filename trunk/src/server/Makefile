
-include ../../.config

subdir-$(CONFIG_VIDEO_FBDEV)    = ./video/fb
subdir-$(CONFIG_VIDEO_SDL)     += ./video/sdl
subdir-$(CONFIG_VIDEO_SVGALIB) += ./video/svga
subdir-$(CONFIG_VIDEO_VESA)    += ./video/vesa
subdir-$(CONFIG_VIDEO_PSPDEV)  += ./video/pspdev
subdir-$(CONFIG_VIDEO_GDI)     += ./video/gdi
subdir-$(CONFIG_VIDEO_GP2X)    += ./video/gp2x
subdir-$(CONFIG_VIDEO_CACA)    += ./video/caca
subdir-$(CONFIG_VIDEO_ARTEC)   += ./video/artec
subdir-$(CONFIG_VIDEO_NULL)    += ./video/null
subdir-$(CONFIG_VIDEO_HELPER)  += ./video/helper

target-y += xynth
target_so-$(CONFIG_LIBRARY_SHARED) += libserver.so
target_a-$(CONFIG_LIBRARY_STATIC)  += libserver.a

libserver.so_files-y = event.c \
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

libserver.so_files-$(CONFIG_VIDEO_FBDEV)   += ./video/fb/video_fb.o
libserver.so_files-$(CONFIG_VIDEO_SDL)     += ./video/sdl/video_sdl.o
libserver.so_files-$(CONFIG_VIDEO_SVGALIB) += ./video/svga/video_svga.o
libserver.so_files-$(CONFIG_VIDEO_VESA)    += ./video/vesa/video_vesa.o
libserver.so_files-$(CONFIG_VIDEO_PSPDEV)  += ./video/pspdev/video_pspdev.o
libserver.so_files-$(CONFIG_VIDEO_GDI)     += ./video/gdi/video_gdi.o
libserver.so_files-$(CONFIG_VIDEO_GP2X)    += ./video/gp2x/video_gp2x.o
libserver.so_files-$(CONFIG_VIDEO_CACA)    += ./video/caca/video_caca.o
libserver.so_files-$(CONFIG_VIDEO_ARTEC)   += ./video/artec/video_artec.o
libserver.so_files-$(CONFIG_VIDEO_NULL)    += ./video/null/video_null.o
libserver.so_files-$(CONFIG_VIDEO_HELPER)  += ./video/helper/video_helper.o

ifeq ($(CONFIG_SINGLE_APPLICATION), y)
libserver.so_files-$(CONFIG_DEMO_CHILD)    += ../../demo/child/single_child.o
libserver.so_files-$(CONFIG_DEMO_DESKTOP)  += ../../demo/desktop/single_desktop.o
libserver.so_files-$(CONFIG_DEMO_HIDE)     += ../../demo/hide/single_hide.o
libserver.so_files-$(CONFIG_DEMO_LOAD)     += ../../demo/load/single_load.o
libserver.so_files-$(CONFIG_DEMO_SIMPLE)   += ../../demo/simple/single_simple.o
libserver.so_files-$(CONFIG_DEMO_OBJECT)   += ../../demo/object/single_object.o
libserver.so_files-$(CONFIG_DEMO_TEMP)     += ../../demo/temp/single_temp.o
libserver.so_files-$(CONFIG_DEMO_TERM)     += ../../demo/term/single_term.o
libserver.so_files-$(CONFIG_DEMO_TIMER)    += ../../demo/timer/single_timer.o
libserver.so_files-$(CONFIG_DEMO_LOGOUT)   += ../../demo/xynthlogout/single_xynthlogout.o
libserver.so_files-$(CONFIG_DEMO_IMAGE)    += ../../demo/img/single_img.o
libserver.so_files-$(CONFIG_DEMO_ONSCREENKEYBOARD) += ../../demo/onscreenkeyboard/single_onscreenkeyboard.o
libserver.so_files-$(CONFIG_DEMO_WIDGETX)  += ../../demo/widgetx/single_widgetx.o
endif

libserver.so_includes-$(CONFIG_VIDEO_PSPDEV) += $(shell psp-config -p)/include
libserver.so_includes-y  += ./ ../lib

libserver.so_libraries-y += ../lib
libserver.so_libraries-$(CONFIG_VIDEO_PSPDEV) += $(shell psp-config -p)/lib

libserver.so_ldflags-y += -lxynth
libserver.so_ldflags-y += -lm
libserver.so_ldflags-$(CONFIG_VIDEO_SDL)   += $(shell sdl-config --libs)
libserver.so_ldflags-$(CONFIG_VIDEO_CACA)  += -lcaca

ifeq ($(CONFIG_WIDGET_LIBRARY), y)
libserver.so_libraries-y += ../widget
libserver.so_ldflags-y   += -lwidget
endif

ifeq ($(CONFIG_THEME_PLUGIN), y)
libserver.so_ldflags-y   += -ldl
endif

libserver.so_ldflags-$(CONFIG_VIDEO_PSPDEV) += -lpspdebug \
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

libserver.a_files-y    = $(libserver.so_files-y)
libserver.a_cflags-y   = $(libserver.so_cflags-y)
libserver.a_includes-y = $(libserver.so_includes-y)

xynth_files-y      = start.c
xynth_includes-y  += ./ $(libserver.so_includes-y)
xynth_libraries-y += ./ $(libserver.so_libraries-y)
xynth_ldflags-y   += -lserver $(libserver.so_ldflags-y)
xynth_depends-$(CONFIG_LIBRARY_SHARED) += libserver.so
xynth_depends-$(CONFIG_LIBRARY_STATIC) += libserver.a

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
	$(CP) libserver.so ../../dist/$(CONFIG_PATH_LIB)/libserver.so
	$(CP) libserver.a ../../dist/$(CONFIG_PATH_LIB)/libserver.a
endif
