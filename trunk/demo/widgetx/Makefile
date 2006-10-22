
include ../../Makefile.cfg
include ../../Makefile.parse

INCDIR  = ../../src/lib ../../src/widget
LIBDIR  = ../../src/widget ../../src/lib
LDFLAGS = -lwidget -lxynth

OBJS = widgetx.o

ifeq ($(SINGLE_APP), Y)
TARGET_A = single_widgetx.a
CFLAGS  += -Dmain=widgetx_main
else
TARGET = widgetx
endif

EXTRA_CLEAN = single_widgetx.a

include ../../Makefile.rules

dist: all
ifneq ($(TARGET),)
	cp -a $(TARGET) ../../$(DISTBINDIR)
endif
