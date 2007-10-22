
target_o-y = libfreetype2.o

libfreetype2.o_includes-y = ./ \
                            ./include
libfreetype2.o_cflags-y = -DFT2_BUILD_LIBRARY
libfreetype2.o_files-y = ftbase.c \
                         ftbitmap.c \
                         ftdebug.c \
                         ftglyph.c \
                         ftinit.c \
                         ftsystem.c \
                         autofit.c \
                         sfnt.c \
                         smooth.c \
                         truetype.c

include ../../../Makefile.lib
