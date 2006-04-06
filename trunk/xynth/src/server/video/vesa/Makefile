include ../../../../Makefile.cfg

TARGET = libdriver_vesa.a

obj = kbd.o server.o lrmi-0.10/lrmi.o

all: $(TARGET)

lrmi-0.10/lrmi.o:
	CC=$(CC) $(MAKE) lrmi.o -C lrmi-0.10

$(TARGET): $(obj)
	$(AR) $(TARGET) $(obj)
	$(RANLIB) $(TARGET)

clean:
	rm -rf $(obj) $(TARGET)
	$(MAKE) clean -C lrmi-0.10
