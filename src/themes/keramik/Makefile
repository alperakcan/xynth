
include ../../../Makefile.cfg
include ../../../Makefile.parse

OBJS          = keramik.o
INCDIR        = ../../lib ../../server
TARGET_SO     = keramik.so

include ../../../Makefile.rules

dist: all
	cp -a $(TARGET_SO) ../../../$(DISTTHEMEDIR)
