include ../../Makefile.cfg

CFLAGS += -I$(INCDIR) -I../../src/lib

TARGET_S = ../single_temp.a
TARGET_D = $(BINDIR)/temp

ifeq ($(SINGLE_APP), Y)
	CFLAGS += -Dmain=temp_main
	TARGET = $(TARGET_S)
else
	TARGET = $(TARGET_D)
endif

obj = temp.o

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
