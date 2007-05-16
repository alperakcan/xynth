
include ../../Makefile.cfg
include ../../Makefile.parse

DIR = src/demo/object

INCDIR  = ../../src/lib ../../src/widget
LIBDIR  = ../../src/widget ../../src/lib
LDFLAGS = -lwidget -lxynth

OBJS = object.o

ifeq ($(SINGLE_APP), Y)
TARGET_A = single_object.a
CFLAGS  += -Dmain=object_main
else
TARGET = object
endif

EXTRA_CLEAN = object single_object.a

include ../../Makefile.rules

dist: all
ifneq ($(TARGET),)
	cp -a $(TARGET) ../../$(DISTBINDIR)
endif
