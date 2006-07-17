
include ../../../Makefile.cfg
include ../../../Makefile.parse

EXTRA_TARGETS = microgui.c
OBJS          = microgui.o
INCDIR        = ../../lib/ ../../server
TARGET_SO     = microgui.so

THEME_EXEC = ../../../tools/theme/theme

include ../../../Makefile.rules

microgui.c:
	$(THEME_EXEC) microgui microgui > microgui.c

dist: all
	cp -a $(TARGET_SO) ../../../$(DISTTHEMEDIR)
