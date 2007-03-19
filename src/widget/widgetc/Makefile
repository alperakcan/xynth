
include ../../../Makefile.cfg
include ../../../Makefile.parse

LDFLAGS = -lexpat
INCDIR = ./ ../ ../../lib

OBJS = widgetc.o list.o node.o code.o header.o parse.o xml.o

TARGET = widgetc

CC := PATH=/usr/bin:${PATH} gcc

include ../../../Makefile.rules

dist: all
ifneq ($(TARGET),)
	cp -a $(TARGET) ../../../$(DISTBINDIR)
endif
