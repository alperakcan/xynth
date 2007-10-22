
-include ../../../../.config

target_o-$(CONFIG_VIDEO_PSPDEV) = video_pspdev.o

video_pspdev.o_files-y = pspdev.h \
                         event.c \
                         kbd.c \
                         mouse.c \
                         server.c

video_pspdev.o_includes-y = ../../ \
                            ../../../lib \
                            ../helper/

include ../../../../Makefile.lib
