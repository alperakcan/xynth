
include ../../../Makefile.cfg
include ../../../Makefile.parse

DIR = src/themes/silverado/

OBJS          = silverado.o
INCDIR        = ../../lib ../../server
TARGET_SO     = silverado.so

include ../../../Makefile.rules

dist: all
	cp -a $(TARGET_SO) ../../../$(DISTTHEMEDIR)
