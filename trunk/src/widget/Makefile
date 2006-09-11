
include ../../Makefile.cfg
include ../../Makefile.parse

OBJS = button.o frame.o object.o window.o

ifeq ($(PLATFORM_LINUX), Y)
TARGET_A  = libwidget.a
TARGET_SO = libwidget.so
LIBDIR    = ../lib
LDFLAGS   = -lxynth
else
ifeq ($(PLATFORM_PSPDEV), Y)
TARGET_A  = libwidget.a
else
ifeq ($(PLATFORM_MINGW), Y)
TARGET_A  = libwidget.a
else
ifeq ($(PLATFORM_GP2X), Y)
TARGET_A  = libwidget.a
endif
endif
endif
endif

EXTRA_CLEAN = libwidget.a libwidget.so

DEPEND_FILES       = *.c
EXTRA_DEPEND_FILES = *.h

include ../../Makefile.rules

dist: all
	cp -a widget.h ../../$(DISTINCDIR)
	cp -a $(TARGET_A) $(TARGET_SO) $(TARGET) ../../$(DISTLIBDIR)
