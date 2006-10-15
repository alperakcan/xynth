
include ../../../Makefile.cfg
include ../../../Makefile.parse

OBJS          = microgui.o
INCDIR        = ../../lib/ ../../server
TARGET_SO     = microgui.so

include ../../../Makefile.rules

dist: all
	cp -a $(TARGET_SO) ../../../$(DISTTHEMEDIR)
