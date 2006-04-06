include ../../../../Makefile.cfg

TARGET = libdriver_gp2x.a

obj = event.o kbd.o mouse.o server.o

all: $(TARGET)

$(TARGET): $(obj)
	$(AR) $(TARGET) $(obj)
	$(RANLIB) $(TARGET)

clean:
	rm -rf $(obj) $(TARGET)
