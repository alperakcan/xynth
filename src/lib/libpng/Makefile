include ../../../Makefile.cfg

CFLAGS += -fPIC -I../libz

obj = png.o pngerror.o pngget.o pngmem.o pngpread.o pngread.o pngrio.o \
      pngrtran.o pngrutil.o pngset.o pngtrans.o pngwio.o pngwrite.o pngwtran.o \
      pngwutil.o

all: libpng.a

libpng.a: $(obj)
	$(AR) libpng.a $(obj)
	$(RANLIB) libpng.a

clean:
	rm -f $(obj) libpng.a
