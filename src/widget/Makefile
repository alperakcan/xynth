
include ../../Makefile.cfg
include ../../Makefile.parse

INCDIR = ../../include/widget ../lib ./

OBJS = button.o checkbox.o editbox.o frame.o object.o progressbar.o scrollbuffer.o textbox.o window.o

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
EXTRA_DEPEND_FILES = *.h

SUBDIR-Y = widgetc widgetr

include ../../Makefile.rules

dist: all
	cp widgetc/widgetc ../../$(DISTBINDIR)
	cp widgetr/widgetr ../../$(DISTBINDIR)
	cp -a widget.h ../../$(DISTINCDIR)
	cp -a $(TARGET_A) $(TARGET_SO) $(TARGET) ../../$(DISTLIBDIR)
