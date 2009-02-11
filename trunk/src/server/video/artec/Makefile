
-include ../../../../.config

target_o-$(CONFIG_VIDEO_ARTEC) = video_artec.o

video_artec.o_files-y = fb.c \
                        fbdev.h \
                        keypad.c \
                        server.c \
                        touch.c

video_artec.o_includes-y = ../../ \
                           ../../../lib \
                           ../helper/

include ../../../../Makefile.lib
