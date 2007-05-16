
include ../../Makefile.cfg
include ../../Makefile.parse

DIR = src/demo/term/

INCDIR  = ../../src/lib
LIBDIR  = ../../src/lib
LDFLAGS = -lxynth

OBJS = term.o

ifeq ($(SINGLE_APP), Y)
TARGET_A = single_term.a
CFLAGS  += -Dmain=term_main
else
TARGET = term
endif

EXTRA_CLEAN = term single_term.a

include ../../Makefile.rules

dist: all
ifneq ($(TARGET),)
	cp -a $(TARGET) ../../$(DISTBINDIR)
endif
