include ../../../../Makefile.cfg

TARGET = libdriver_null.a

obj = kbd.o mouse.o server.o

all: $(TARGET)

$(TARGET): $(obj)
	$(AR) $(TARGET) $(obj)
	$(RANLIB) $(TARGET)

clean:
	rm -rf $(obj) $(TARGET)
