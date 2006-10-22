
include ../../Makefile.cfg
include ../../Makefile.parse

INCDIR = ../lib ./

OBJS = button.o checkbox.o editbox.o frame.o object.o textbox.o window.o scrollbar.o listbox.o combobox.o progressbar.o

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

EXTRA_TARGETS = widgetc
EXTRA_CLEAN = libwidget.a libwidget.so widgetc.o
EXTRA_DEPEND_FILES = *.h

include ../../Makefile.rules

widgetc: widgetc.o
	rm -rf $@
	PATH=/usr/bin gcc $^ $(LDFLAGS) -lexpat -o $@

dist: all
	cp -a widgetc ../../$(DISTBINDIR)
	cp -a widget.h ../../$(DISTINCDIR)
	cp -a $(TARGET_A) $(TARGET_SO) $(TARGET) ../../$(DISTLIBDIR)
