
include ../../Makefile.cfg
include ../../Makefile.parse

DIR = src/demo/mines/

INCDIR  = ../../src/lib ../../src/widget
LIBDIR  = ../../src/widget ../../src/lib
LDFLAGS = -lwidget -lxynth
CC      = $(CXX)

OBJS = mines.o

ifeq ($(SINGLE_APP), Y)
TARGET_A = single_mines.a
CFLAGS  += -Dmain=mines_main
else
TARGET = mines
endif

EXTRA_CLEAN = mines single_mines.a

include ../../Makefile.rules

dist: all
ifneq ($(TARGET),)
	cp -a $(TARGET) ../../$(DISTBINDIR)
endif
