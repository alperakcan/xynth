
include ../../Makefile.cfg
include ../../Makefile.parse

DIR = src/demo/onscreenkeyboard/

INCDIR  = ../../src/lib
LIBDIR  = ../../src/lib
LDFLAGS = -lxynth

OBJS = onscreenkeyboard.o

ifeq ($(SINGLE_APP), Y)
TARGET_A = single_onscreenkeyboard.a
CFLAGS  += -Dmain=onscreenkeyboard_main
else
TARGET = onscreenkeyboard
endif

EXTRA_CLEAN = onscreenkeyboard single_onscreenkeyboard.a

include ../../Makefile.rules

dist: all
ifneq ($(TARGET),)
	cp -a $(TARGET) ../../$(DISTBINDIR)
endif
