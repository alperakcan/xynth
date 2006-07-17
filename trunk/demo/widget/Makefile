
include ../../Makefile.cfg
include ../../Makefile.parse

INCDIR  = ../../src/lib ../../src/widget
LIBDIR  = ../../src/widget ../../src/lib
LDFLAGS = -lwidget -lxynth
CC      = $(CXX)

OBJS = widget.o

ifeq ($(SINGLE_APP), Y)
TARGET_A = single_widget.a
CFLAGS  += -Dmain=widget_main
else
TARGET = widget
endif

EXTRA_CLEAN = widget single_widget.a

include ../../Makefile.rules

dist: all
ifneq ($(TARGET),)
	cp -a $(TARGET) ../../$(DISTBINDIR)
endif
