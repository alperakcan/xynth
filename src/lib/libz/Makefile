include ../../../Makefile.cfg

CFLAGS += -fPIC -I./

obj = adler32.o compress.o crc32.o gzio.o uncompr.o deflate.o trees.o zutil.o \
      inflate.o infback.o inftrees.o inffast.o

all: libz.a

libz.a: $(obj)
	$(AR) libz.a $(obj)
	$(RANLIB) libz.a

clean:
	rm -f $(obj) libz.a
