
include ../../../Makefile.cfg
include ../../../Makefile.parse

DIR = src/themes/keramik/

OBJS          = keramik.o
INCDIR        = ../../lib ../../server
TARGET_SO     = keramik.so

include ../../../Makefile.rules

dist: all
	cp -a $(TARGET_SO) ../../../$(DISTTHEMEDIR)
