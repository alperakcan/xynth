include ../../../../Makefile.cfg

TARGET = libdriver_fb.a

obj = fb.o kbd.o server.o timing.o

all: $(TARGET)

$(obj): fbdev.h

$(TARGET): $(obj)
	$(AR) $(TARGET) $(obj)
	$(RANLIB) $(TARGET)

clean:
	rm -rf $(obj) $(TARGET)
