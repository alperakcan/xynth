
include ../../Makefile.cfg
include ../../Makefile.parse

DIR = src/demo/load/

INCDIR  = ../../src/lib
LIBDIR  = ../../src/lib
LDFLAGS = -lxynth

OBJS = load.o

ifeq ($(SINGLE_APP), Y)
TARGET_A = single_load.a
CFLAGS  += -Dmain=load_main
else
TARGET = load
endif

EXTRA_CLEAN = load single_load.a

include ../../Makefile.rules

dist: all
ifneq ($(TARGET),)
	cp -a $(TARGET) ../../$(DISTBINDIR)
endif
