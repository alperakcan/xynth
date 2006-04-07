include ../../Makefile.cfg

CFLAGS += -fPIC

THEME_EXEC = ../../tools/theme/theme

all: $(THEMEDIR) $(THEME_EXEC) xynth.h xynth_.h server.h keramik.c microgui.c silverado.c

$(THEMEDIR):
	mkdir -p $(THEMEDIR)

$(THEME_EXEC):
	$(MAKE) -C ../../tools/theme/

xynth.h:
	ln -fs ../lib/xynth.h xynth.h
xynth_.h:
	ln -fs ../lib/xynth_.h xynth_.h
server.h:
	ln -fs ../server/server.h server.h

keramik.c:
	$(THEME_EXEC) keramik keramik > keramik.c
	$(CC) -c $(CFLAGS) keramik.c
	$(CC) -s -shared -Wl,-soname,keramik.so -o $(THEMEDIR)/keramik.so keramik.o

microgui.c:
	$(THEME_EXEC) microgui microgui > microgui.c
	$(CC) -c $(CFLAGS) microgui.c
	$(CC) -s -shared -Wl,-soname,microgui.so -o $(THEMEDIR)/microgui.so microgui.o

silverado.c:
	$(THEME_EXEC) silverado silverado > silverado.c
	$(CC) -c $(CFLAGS) silverado.c
	$(CC) -s -shared -Wl,-soname,silverado.so -o $(THEMEDIR)/silverado.so silverado.o

clean:
	$(MAKE) clean -C ../../tools/theme
	rm -rf $(THEMEDIR) $(THEME_EXEC) *.o *.c *.h
