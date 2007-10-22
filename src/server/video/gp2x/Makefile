
-include ../../../../.config

target_o-$(CONFIG_VIDEO_GP2X) = video_gp2x.o

video_gp2x.o_files-y = gp2x.h \
                       event.c \
                       kbd.c \
                       mouse.c \
                       server.c

video_gp2x.o_includes-y = ../../ \
                          ../../../lib \
                          ../helper/

include ../../../../Makefile.lib
