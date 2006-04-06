include ../../Makefile.cfg

CFLAGS += -I$(INCDIR) -I../../src/lib/

TARGET_D = $(BINDIR)/img

obj = img.o

all : $(TARGET_D)

$(TARGET_D) : $(obj)
	$(CC) -o $(TARGET_D) $(obj) $(LIBS)

clean :
	rm -f $(TARGET_D) $(obj)
