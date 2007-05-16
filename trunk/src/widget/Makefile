
include ../../Makefile.cfg
include ../../Makefile.parse

DIR = src/widget/

INCDIR = ./ ../lib

OBJS = button.o checkbox.o editbox.o effect.o frame.o object.o progressbar.o \
       signal.o scrollbuffer.o textbox.o window.o listbox.o clistbox.o

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

SUBDIR-Y += widgetc
SUBDIR-Y += widgetr

include ../../Makefile.rules

dist: all
	cp widgetc/widgetc ../../$(DISTBINDIR)
	cp widgetr/widgetr ../../$(DISTBINDIR)
	cp widget.h ../../$(DISTINCDIR)
	cp -a $(TARGET_A) $(TARGET_SO) $(TARGET) ../../$(DISTLIBDIR)
