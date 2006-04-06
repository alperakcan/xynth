include ../../../../Makefile.cfg

CFLAGS += $(shell sdl-config --cflags)

TARGET = libdriver_sdl.a

obj = event.o kbd.o mouse.o server.o

all: $(TARGET)

$(obj): sdl.h

$(TARGET): $(obj)
	$(AR) $(TARGET) $(obj)
	$(RANLIB) $(TARGET)

clean:
	rm -rf $(obj) $(TARGET)
