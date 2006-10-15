
include ../../../Makefile.cfg
include ../../../Makefile.parse

OBJS          = silverado.o
INCDIR        = ../../lib/ ../../server
TARGET_SO     = silverado.so

include ../../../Makefile.rules

dist: all
	cp -a $(TARGET_SO) ../../../$(DISTTHEMEDIR)
