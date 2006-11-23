
include ../../Makefile.cfg
include ../../Makefile.parse

INCDIR  = ../../src/lib ../../src/widget
LIBDIR  = ../../src/widget ../../src/lib
LDFLAGS = -lwidget -lxynth

have_widgetc := $(wildcard ../../src/widget/widgetc)
have_widgetc += $(wildcard ../../src/widget/widgetc.exe)

ifeq ($(strip $(have_widgetc)),)

include ../../Makefile.rules

else

OBJS = widgetx.o widgetx_xml.o

ifeq ($(SINGLE_APP), Y)
TARGET_A = single_widgetx.a
CFLAGS  += -Dmain=widgetx_main
else
TARGET = widgetx
endif

include ../../Makefile.rules

widgetx_xml.c: widgetx.xml
	../../src/widget/widgetc -f widgetx.xml -o widgetx -c
endif

dist: all
ifneq ($(TARGET),)
	cp -a $(TARGET) ../../$(DISTBINDIR)
endif

EXTRA_CLEAN = widgetx single_widgetx.a widgetx_xml.c widgetx_xml.h widgetx.o widgetx_xml.o
