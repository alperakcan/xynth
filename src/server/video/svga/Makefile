include ../../../../Makefile.cfg

TARGET = libdriver_svga.a

obj = kbd.o server.o

all: $(TARGET)

$(TARGET): $(obj)
	$(AR) $(TARGET) $(obj)
	$(RANLIB) $(TARGET)

clean:
	rm -rf $(obj) $(TARGET)
