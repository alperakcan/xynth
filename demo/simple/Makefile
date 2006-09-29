
include ../../Makefile.cfg
include ../../Makefile.parse

INCDIR  = ../../src/lib
LIBDIR  = ../../src/lib
LDFLAGS = -lxynth

OBJS = simple.o

ifeq ($(SINGLE_APP), Y)
TARGET_A = single_simple.a
CFLAGS  += -Dmain=simple_main
else
TARGET = simple
endif

EXTRA_CLEAN = simple single_simple.a

include ../../Makefile.rules

dist: all
ifneq ($(TARGET),)
	cp -a $(TARGET) ../../$(DISTBINDIR)
endif
