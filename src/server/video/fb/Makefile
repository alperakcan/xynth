
-include ../../../../.config

target_o-$(CONFIG_VIDEO_FBDEV) = video_fb.o

video_fb.o_files-y = fb.c \
                     fbdev.h \
                     kbd.c \
                     server.c \
                     timing.c

video_fb.o_includes-y = ../../ \
                        ../../../lib \
                        ../helper/

include ../../../../Makefile.lib
