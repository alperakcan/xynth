
include ../../../Makefile.cfg
include ../../../Makefile.parse

EXTRA_TARGETS = silverado.c
OBJS          = silverado.o
INCDIR        = ../../lib/ ../../server
TARGET_SO     = silverado.so

THEME_EXEC = ../../../tools/theme/theme

include ../../../Makefile.rules

silverado.c:
	$(THEME_EXEC) silverado silverado > silverado.c

dist: all
	cp -a $(TARGET_SO) ../../../$(DISTTHEMEDIR)
