
include ../../../Makefile.cfg
include ../../../Makefile.parse

LDFLAGS = -lexpat

OBJS = widgetc.o list.o node.o lang.o gen_code.o gen_header.o parse.o

TARGET = widgetc

CC := PATH=/usr/bin:${PATH} gcc

include ../../../Makefile.rules

dist: all
ifneq ($(TARGET),)
	cp -a $(TARGET) ../../../$(DISTBINDIR)
endif
