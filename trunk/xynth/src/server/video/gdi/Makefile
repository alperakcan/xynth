include ../../../../Makefile.cfg

TARGET = libdriver_gdi.a

obj = event.o kbd.o mouse.o server.o

all: $(TARGET)

$(obj): gdi.h

$(TARGET): $(obj)
	$(AR) $(TARGET) $(obj)
	$(RANLIB) $(TARGET)

clean:
	rm -rf $(obj) $(TARGET)
