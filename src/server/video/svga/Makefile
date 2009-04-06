
-include ../../../../.config

target_o-$(CONFIG_VIDEO_SVGALIB) = video_svga.o

video_svga.o_files-y = svga.h \
                       kbd.c \
                       server.c

video_svga.o_includes-y = ../../ \
                          ../../../lib \
                          ../helper/

video_svga.o_ldflags-y = -lvga

include ../../../../Makefile.lib
