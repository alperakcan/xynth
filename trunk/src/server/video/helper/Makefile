
-include ../../../../.config

target_o-$(CONFIG_VIDEO_HELPER) = video_helper.o

video_helper.o_files-y = console.c \
                         helper.h \
                         irman.c \
                         irair.c \
                         kbd.c \
                         kbd.h \
                         modes.c \
                         mouse.c \
                         mouse.h \
                         mtrr.c \
                         tscreen.c

video_helper.o_includes-y = ../../ \
                            ../../../lib

include ../../../../Makefile.lib
