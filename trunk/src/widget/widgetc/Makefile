
include ../../../Makefile.cfg
include ../../../Makefile.parse

LDFLAGS = -lexpat
INCDIR = ./ ../ ../../lib

OBJS = widgetc.o list.o code.o header.o xml.o

TARGET_HOST = widgetc

#CC := PATH=/usr/bin:${PATH} gcc

include ../../../Makefile.rules

dist: all
ifneq ($(TARGET),)
	cp -a $(TARGET) ../../../$(DISTBINDIR)
endif
