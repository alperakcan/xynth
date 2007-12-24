
-include ../../.config

subdir-y := ./libz
subdir-y += ./libpng
subdir-y += ./freetype2
subdir-y += ./expat
subdir-y += ./pixman

target_a-y = libxynth.a

libxynth.a_includes-y := ./
libxynth.a_includes-y += ./libz
libxynth.a_includes-y += ./libpng
libxynth.a_includes-y += ./freetype2/include
libxynth.a_includes-y += ./expat
libxynth.a_includes-y += ./pixman

libxynth.a_files-y = alloc.c \
                     child.c \
                     config.c \
                     debug.c \
                     driver.c \
                     event.c \
                     eventq.c \
                     font.c \
                     gettime.c \
                     grlib.c \
                     handler.c \
                     hashtable.c \
                     image.c \
                     image_gif.c \
                     image_png.c \
                     image_xpm.c \
                     list.c \
                     pipe_api.c \
                     pollfd.c \
                     rect.c \
                     region.c \
                     render.c \
                     socket.c \
                     socket_api.c \
                     surface.c \
                     thread.c \
                     timer.c \
                     window.c \
                     xml.c

libxynth.a_files-y += ./expat/libexpat.o
libxynth.a_files-y += ./freetype2/libfreetype2.o
libxynth.a_files-y += ./libpng/libpng.o
libxynth.a_files-y += ./libz/libz.o
libxynth.a_files-y += ./pixman/libpixman.o

target_so-y = libxynth.so

libxynth.so_includes-y = $(libxynth.a_includes-y)
libxynth.so_ldflags-y = -lpthread \
                        -lm
libxynth.so_files-y = $(libxynth.a_files-y)

include ../../Makefile.lib

dist:
	$(CP) xynth.h ../../dist/$(CONFIG_PATH_INCLUDE)/xynth.h
	$(CP) libxynth.a ../../dist/$(CONFIG_PATH_LIB)/libxynth.a
	$(CP) libxynth.so ../../dist/$(CONFIG_PATH_LIB)/libxynth.so
