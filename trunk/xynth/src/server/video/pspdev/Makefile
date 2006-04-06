include ../../../../Makefile.cfg

TARGET = libdriver_pspdev.a

obj = event.o kbd.o mouse.o server.o

all: $(TARGET)

$(obj): pspdev.h

$(TARGET): $(obj)
	$(AR) $(TARGET) $(obj)
	$(RANLIB) $(TARGET)

clean:
	rm -rf $(obj) $(TARGET)
