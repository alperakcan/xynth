
include ../../Makefile.cfg
include ../../Makefile.parse

INCDIR  = ../../src/lib
LIBDIR  = ../../src/lib
LDFLAGS = -lxynth

OBJS = hide.o

ifeq ($(SINGLE_APP), Y)
TARGET_A = single_hide.a
CFLAGS  += -Dmain=hide_main
else
TARGET = hide
endif

EXTRA_CLEAN = hide single_hide.a

DEPEND_FILES       = *.c

include ../../Makefile.rules

dist: all
ifneq ($(TARGET),)
	cp -a $(TARGET) ../../$(DISTBINDIR)
endif
