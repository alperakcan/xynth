
-include ../../../../.config

target_o-$(CONFIG_VIDEO_NULL) = video_null.o

video_null.o_files-y = null.h \
                       kbd.c \
                       mouse.c \
                       server.c

video_null.o_includes-y = ../../ \
                          ../../../lib \
                          ../helper/

include ../../../../Makefile.lib
