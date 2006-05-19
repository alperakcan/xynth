include ../../../Makefile.cfg

CFLAGS += -fPIC -DFT2_BUILD_LIBRARY -I./include

obj = ftbase.o ftbitmap.o ftdebug.o ftglyph.o ftinit.o ftsystem.o autofit.o \
      sfnt.o smooth.o truetype.o

all: libfreetype.a

libfreetype.a: $(obj)
	$(AR) libfreetype.a $(obj)
	$(RANLIB) libfreetype.a

clean:
	rm -f $(obj) libfreetype.a
