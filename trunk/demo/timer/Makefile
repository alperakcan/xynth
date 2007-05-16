
include ../../Makefile.cfg
include ../../Makefile.parse

DIR = src/demo/timer/

INCDIR  = ../../src/lib
LIBDIR  = ../../src/lib
LDFLAGS = -lxynth

OBJS = timer.o

ifeq ($(SINGLE_APP), Y)
TARGET_A = single_timer.a
CFLAGS  += -Dmain=timer_main
else
TARGET = timer
endif

EXTRA_CLEAN = timer single_timer.a

include ../../Makefile.rules

dist: all
ifneq ($(TARGET),)
	cp -a $(TARGET) ../../$(DISTBINDIR)
endif
