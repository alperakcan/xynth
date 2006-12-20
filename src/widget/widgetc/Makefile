
include ../../../Makefile.cfg
include ../../../Makefile.parse

LDFLAGS = -lexpat

OBJS = widgetc.o

TARGET = widgetc

CC := PATH=/usr/bin:${PATH} ${CC}

include ../../../Makefile.rules

dist: all
ifneq ($(TARGET),)
	cp -a $(TARGET) ../../../$(DISTBINDIR)
endif
