
-include ../../../../.config

target_o-$(CONFIG_VIDEO_CACA) = video_caca.o

video_caca.o_files-y = caca.h \
                       event.c \
                       kbd.c \
                       mouse.c \
                       server.c

video_caca.o_includes-y = ../../ \
                          ../../../lib \
                          ../helper/

include ../../../../Makefile.lib
