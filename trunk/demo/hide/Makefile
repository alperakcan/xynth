include ../../Makefile.cfg

CFLAGS += -I$(INCDIR) -I../../src/lib

TARGET_S = ../single_hide.a
TARGET_D = $(BINDIR)/hide

ifeq ($(SINGLE_APP), Y)
	CFLAGS += -Dmain=hide_main
	TARGET = $(TARGET_S)
else
	TARGET = $(TARGET_D)
endif

obj = hide.o

all : $(TARGET)

$(TARGET) : $(obj)
ifeq ($(SINGLE_APP), Y)
	$(AR) $(TARGET) $(obj)
	$(RANLIB) $(TARGET)
else
	$(CC) -o $(TARGET) $(obj) $(LIBS)
endif

clean :
	rm -f $(TARGET_D) $(TARGET_S) $(obj)
