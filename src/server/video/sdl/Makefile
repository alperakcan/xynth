
-include ../../../../.config

target_o-$(CONFIG_VIDEO_SDL) = video_sdl.o

video_sdl.o_files-y = event.c \
                      kbd.c \
                      mouse.c \
                      sdl.h \
                      server.c
video_sdl.o_includes-y = ../../ \
                         ../../../lib \
                         ../helper/
video_sdl.o_cflags-y = $(shell sdl-config --cflags)

include ../../../../Makefile.lib
