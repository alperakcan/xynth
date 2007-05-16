
include ../../Makefile.cfg
include ../../Makefile.parse

DIR = src/demo/img/

INCDIR  = ../../src/lib
LIBDIR  = ../../src/lib
LDFLAGS = -lxynth

OBJS = img.o

ifeq ($(SINGLE_APP), Y)
TARGET_A = single_img.a
CFLAGS  += -Dmain=img_main
else
TARGET = img
endif

EXTRA_CLEAN = img single_img.a

include ../../Makefile.rules

dist: all
ifneq ($(TARGET),)
	cp -a $(TARGET) ../../$(DISTBINDIR)
endif
