include ../../../../Makefile.cfg

TARGET = libdriver_helper.a

ifeq ($(VIDEO_HELPER_CONSOLE), Y)
	obj += console.o
endif
ifeq ($(VIDEO_HELPER_KBD), Y)
	obj += kbd.o
endif
ifeq ($(VIDEO_HELPER_MODES), Y)
	obj += modes.o
endif
ifeq ($(VIDEO_HELPER_MOUSE), Y)
	obj += mouse.o
endif
ifeq ($(VIDEO_HELPER_CURSOR), Y)
	obj += cursor.o
endif
ifeq ($(VIDEO_HELPER_MTRR), Y)
	obj += mtrr.o
endif

all: $(TARGET)

$(obj): helper.h
cursor.o: cursor.h
console.o: console.c
kbd.o: kbd.c kbd.c
mouse.o: mouse.h mouse.c mouse_1.c mouse_2.c

$(TARGET): $(obj)
	$(AR) $(TARGET) $(obj)
	$(RANLIB) $(TARGET)

clean:
	rm -rf $(obj) *.o $(TARGET)
