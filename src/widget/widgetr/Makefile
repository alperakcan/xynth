
include ../../../Makefile.cfg
include ../../../Makefile.parse

INCDIR = ../ ../../lib
LIBDIR = ../ ../../lib
LDFLAGS = -lwidget -lxynth -lexpat

OBJS = widgetr.o table.o parse.o node.o list.o code.o

TARGET = widgetr

include ../../../Makefile.rules

dist: all
ifneq ($(TARGET),)
	cp -a $(TARGET) ../../../$(DISTBINDIR)
endif
