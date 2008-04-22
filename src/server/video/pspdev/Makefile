
-include ../../../../.config

target_o-$(CONFIG_VIDEO_PSPDEV) = video_pspdev.o

video_pspdev.o_files-y = pspdev.h \
                         event.c \
                         kbd.c \
                         mouse.c \
                         server.c

video_pspdev.o_cflags-y = -DPSP_DIRECTACCESS -DPSP_BITSPERPIXEL=32

video_pspdev.o_includes-y = ../../ \
                            ../../../lib \
                            ../helper/

ifeq ($(CONFIG_PLATFORM_PSPDEV), y)
	video_pspdev.o_includes-y += $(shell psp-config -p)/include
endif

include ../../../../Makefile.lib
