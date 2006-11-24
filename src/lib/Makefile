
include ../../Makefile.cfg
include ../../Makefile.parse

INCDIR += ./libz ./libpng ./freetype2/include

OBJS = alloc.o child.o client.o config.o debug.o driver.o event.o eventq.o font.o \
       gettext.o gettime.o grlib.o handler.o image.o image_gif.o image_png.o \
       image_xpm.o list.o pipe_api.o pollfd.o rect.o socket.o socket_api.o surface.o \
       thread.o timer.o window.o

OBJS += libz/adler32.o libz/compress.o libz/crc32.o libz/gzio.o libz/uncompr.o \
        libz/deflate.o libz/trees.o libz/zutil.o libz/inflate.o libz/infback.o \
        libz/inftrees.o libz/inffast.o

OBJS += libpng/png.o libpng/pngerror.o libpng/pngget.o libpng/pngmem.o \
        libpng/pngpread.o libpng/pngread.o libpng/pngrio.o libpng/pngrtran.o \
        libpng/pngrutil.o libpng/pngset.o libpng/pngtrans.o libpng/pngwio.o \
        libpng/pngwrite.o libpng/pngwtran.o libpng/pngwutil.o

CFLAGS += -DFT2_BUILD_LIBRARY
OBJS += freetype2/ftbase.o freetype2/ftbitmap.o freetype2/ftdebug.o freetype2/ftglyph.o \
        freetype2/ftinit.o freetype2/ftsystem.o freetype2/autofit.o freetype2/sfnt.o \
        freetype2/smooth.o freetype2/truetype.o

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
INCDIR   += $(INCDIR-Y)
LDFLAGS  += $(LDFLAGS-Y)

EXTRA_CLEAN = libxynth.a libxynth.so

EXTRA_DEPEND_FILES = *.h

include ../../Makefile.rules

dist: all
	cp -a xynth.h ../../$(DISTINCDIR)
	cp -a $(TARGET_A) $(TARGET_SO) $(TARGET) ../../$(DISTLIBDIR)
