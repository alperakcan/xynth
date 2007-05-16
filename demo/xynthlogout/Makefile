
include ../../Makefile.cfg
include ../../Makefile.parse

DIR = src/demo/xynthlogout/

INCDIR  = ../../src/lib
LIBDIR  = ../../src/lib
LDFLAGS = -lxynth

OBJS = xynthlogout.o

ifeq ($(SINGLE_APP), Y)
TARGET_A = single_xynthlogout.a
CFLAGS  += -Dmain=xynthlogout_main
else
TARGET = xynthlogout
endif

EXTRA_CLEAN = xynthlogout single_xynthlogout.a

include ../../Makefile.rules

dist: all
ifneq ($(TARGET),)
	cp -a $(TARGET) ../../$(DISTBINDIR)
endif
