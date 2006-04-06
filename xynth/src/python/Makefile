include ../../Makefile.cfg

TARGET_P = xynth

CFLAGS = -fPIC -I$(INCDIR) -I/usr/include/python2.4

obj = window.o xynthpy.o

all: $(obj)
	$(CC) -shared $(obj) -o $(TARGET_P).so -L$(LIBDIR) -lpython2.4 -lxynth

clean:
	rm -rf $(obj) $(TARGET_P).so
