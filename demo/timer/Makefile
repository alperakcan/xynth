include ../../Makefile.cfg

CFLAGS += -I$(INCDIR) -I../../src/lib

TARGET_S = ../single_timer.a
TARGET_D = $(BINDIR)/timer

ifeq ($(SINGLE_APP), Y)
	CFLAGS += -Dmain=timer_main
	TARGET = $(TARGET_S)
else
	TARGET = $(TARGET_D)
endif

obj = timer.o

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
