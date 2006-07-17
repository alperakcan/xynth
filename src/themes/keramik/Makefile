
include ../../../Makefile.cfg
include ../../../Makefile.parse

EXTRA_TARGETS = keramik.c
OBJS          = keramik.o
INCDIR        = ../../lib/ ../../server
TARGET_SO     = keramik.so

THEME_EXEC = ../../../tools/theme/theme

include ../../../Makefile.rules

keramik.c:
	$(THEME_EXEC) keramik keramik > keramik.c

dist: all
	cp -a $(TARGET_SO) ../../../$(DISTTHEMEDIR)
