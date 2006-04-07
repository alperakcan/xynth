include ../../Makefile.cfg

CPPFLAGS = $(CFLAGS) -I$(INCDIR) -I../../src/lib

TARGET_D = $(BINDIR)/mines

obj = mines.o

all : $(TARGET_D)

$(TARGET_D) : $(obj)
	$(CXX) -o $(TARGET_D) $(obj) -lwidget $(LIBS)

clean :
	rm -f $(TARGET_D) $(obj)
