
include ../../../Makefile.cfg
include ../../../Makefile.parse

DIR = src/lib/freetype2/

INCDIR = ./ ./include
CFLAGS += -DFT2_BUILD_LIBRARY
OBJS += ftbase.o ftbitmap.o ftdebug.o ftglyph.o \
        ftinit.o ftsystem.o autofit.o sfnt.o \
        smooth.o truetype.o


TARGET_O = libfreetype2.o

include ../../../Makefile.rules
