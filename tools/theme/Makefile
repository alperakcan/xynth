include ../../Makefile.cfg

THEME_EXEC = theme
obj = theme.o
libs = ../../dist/lib/libxynth.a ../../src/lib/libpng/libpng.a ../../src/lib/libz/libz.a -lpthread -lm

all: $(obj)
	$(CC) $(CFLAGS) -o $(THEME_EXEC) $(obj) $(libs)

$(obj): xynth.h xynth_.h server.h ../../dist/lib/libxynth.a

xynth.h:
	ln -sf ../../src/lib/xynth.h xynth.h

xynth_.h:
	ln -sf ../../src/lib/xynth_.h xynth_.h

server.h:
	ln -sf ../../src/server/server.h server.h

../../dist/lib/libxynth.a:
	$(MAKE) -C ../../src/lib/

clean:
	rm -rf $(obj) *.h $(THEME_EXEC)

