include ../../Makefile.cfg

CPPFLAGS = $(CFLAGS) -I$(INCDIR)

TARGET_D = $(BINDIR)/widget

obj = widget.o

all : $(TARGET_D)

$(TARGET_D) : $(obj)
	$(CXX) -o $(TARGET_D) $(obj) -lwidget $(LIBS)

clean :
	rm -f $(TARGET_D) $(obj)
