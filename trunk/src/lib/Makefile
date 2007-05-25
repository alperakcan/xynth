
include ../../Makefile.cfg
include ../../Makefile.parse

DIR = src/lib/

INCDIR = ./libz ./libpng ./freetype2/include ./expat ./pixman

OBJS = alloc.o child.o config.o debug.o driver.o event.o eventq.o font.o \
       gettext.o gettime.o grlib.o handler.o hashtable.o image.o image_gif.o \
       image_png.o image_xpm.o list.o pipe_api.o pollfd.o rect.o socket.o \
       socket_api.o surface.o render.o thread.o timer.o window.o xml.o

EXTRA_OBJS = expat/libexpat.o libpng/libpng.o libz/libz.o pixman/libpixman.o \
             freetype2/libfreetype2.o

TARGET_A-$(PLATFORM_LINUX)  = libxynth.a
TARGET_SO-$(PLATFORM_LINUX) = libxynth.so
LDFLAGS-$(PLATFORM_LINUX)   = -lpthread -lm

TARGET_A-$(PLATFORM_PSPDEV)  = libxynth.a

TARGET_A-$(PLATFORM_MINGW)  = libxynth.a
ifeq ($(PLATFORM_MINGW), Y)
INCDIR-$(THREAD_PTHREADS) += ../server/video/gdi/pthread_w32
endif

TARGET_A-$(PLATFORM_GP2X)  = libxynth.a
TARGET_SO-$(PLATFORM_GP2X) = libxynth.so
LDFLAGS-$(PLATFORM_GP2X)   = -lpthread -lm

TARGET_A  = $(TARGET_A-Y)
TARGET_SO = $(TARGET_SO-Y)

EXTRA_CLEAN = libxynth.a libxynth.so
EXTRA_TARGETS_CLEAN = extra_clean

EXTRA_DEPEND_FILES = *.h

include ../../Makefile.rules

expat/libexpat.o:
	@$(MAKE) -C expat

libz/libz.o:
	@$(MAKE) -C libz

libpng/libpng.o:
	@$(MAKE) -C libpng

pixman/libpixman.o:
	@$(MAKE) -C pixman

freetype2/libfreetype2.o:
	@$(MAKE) -C freetype2

extra_clean:
	@$(foreach F, expat libz libpng freetype2 pixman, $(MAKE) -C $F clean || exit 1;)

dist: all
	cp -a xynth.h ../../$(DISTINCDIR)
	cp -a $(TARGET_A) $(TARGET_SO) $(TARGET) ../../$(DISTLIBDIR)
