
include ../../Makefile.cfg
include ../../Makefile.parse

INCDIR  = ../../src/lib ../../src/widget
LIBDIR  = ../../src/widget ../../src/lib
LDFLAGS = -lwidget -lxynth

OBJS = widgetx.o widgetx_xml.o

ifeq ($(SINGLE_APP), Y)
TARGET_A = single_widgetx.a
CFLAGS  += -Dmain=widgetx_main
else
TARGET = widgetx
endif

EXTRA_CLEAN = single_widgetx.a widgetx_xml.c widgetx_xml.h

have_widgetc := $(wildcard ../../src/widget/widgetc)

include ../../Makefile.rules

ifeq ($(strip $(have_widgetc)),)
else
widgetx_xml.c: widgetx.xml
	../../src/widget/widgetc -f widgetx.xml -o widgetx
endif

dist: all
ifneq ($(TARGET),)
	cp -a $(TARGET) ../../$(DISTBINDIR)
endif
