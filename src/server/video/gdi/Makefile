
-include ../../../../.config

target_o-$(CONFIG_VIDEO_GDI) = video_gdi.o

video_gdi.o_files-y = gdi.h \
                      event.c \
                      kbd.c \
                      mouse.c \
                      server.c

video_gdi.o_includes-y = ../../ \
                         ../../../lib \
                         ../helper/

include ../../../../Makefile.lib
