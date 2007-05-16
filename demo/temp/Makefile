
include ../../Makefile.cfg
include ../../Makefile.parse

DIR = src/demo/temp/

INCDIR  = ../../src/lib
LIBDIR  = ../../src/lib
LDFLAGS = -lxynth

OBJS = temp.o

ifeq ($(SINGLE_APP), Y)
TARGET_A = single_temp.a
CFLAGS  += -Dmain=temp_main
else
TARGET = temp
endif

EXTRA_CLEAN = temp single_temp.a

include ../../Makefile.rules

dist: all
ifneq ($(TARGET),)
	cp -a $(TARGET) ../../$(DISTBINDIR)
endif
