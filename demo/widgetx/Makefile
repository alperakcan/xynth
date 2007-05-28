
include ../../Makefile.cfg
include ../../Makefile.parse

DIR = src/demo/widgetx/

INCDIR  = ../../src/lib ../../src/widget
LIBDIR  = ../../src/widget ../../src/lib
LDFLAGS = -lwidget -lxynth

ifeq ($(PLATFORM_MINGW), Y)
WIDGETC = ../../src/widget/widgetc/widgetc.exe
else
WIDGETC = ../../src/widget/widgetc/widgetc
endif

have_widgetc := $(wildcard $(WIDGETC))

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

widgetx_xml.c: widgetx.xml $(WIDGETC)
	$(WIDGETC) -f widgetx.xml -o widgetx
endif

dist: all
ifneq ($(TARGET),)
	cp -a $(TARGET) ../../$(DISTBINDIR)
endif

EXTRA_CLEAN = widgetx single_widgetx.a widgetx_xml.c widgetx_xml.h widgetx.o widgetx_xml.o
