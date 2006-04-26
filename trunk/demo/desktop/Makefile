include ../../Makefile.cfg

TARGET_S = ../single_desktop.a
TARGET_D = $(BINDIR)/desktop
TARGET_P = $(SHAREDIR)/desktop

CFLAGS += -I$(INCDIR) -I../../src/lib -DDESKTOPDIR=\"$(INSTALLDIR)/$(TARGET_P)\"


ifeq ($(SINGLE_APP), Y)
	CFLAGS += -Dmain=desktop_main
	TARGET = $(TARGET_S)
else
	TARGET = $(TARGET_D)
endif

obj = desktop.o main.o menu.o taskbar.o

all : $(TARGET) $(TARGET_P)

$(obj) : desktop.h

$(TARGET) : $(obj)
ifeq ($(SINGLE_APP), Y)
	$(AR) $(TARGET) $(obj)
	$(RANLIB) $(TARGET)
else
	$(CC) -o $(TARGET) $(obj) $(LIBS)
endif

$(TARGET_P) : cfg/desktop.cfg
	mkdir -p $(TOPDIR)/$(TARGET_P)
	mkdir -p $(TOPDIR)/$(TARGET_P)/cfg
	mkdir -p $(TOPDIR)/$(TARGET_P)/img
	mkdir -p $(TOPDIR)/$(TARGET_P)/img/icons
	mkdir -p $(TOPDIR)/$(TARGET_P)/img/widget
	mkdir -p $(TOPDIR)/$(TARGET_P)/img/wpaper
	cp cfg/*.cfg $(TOPDIR)/$(TARGET_P)/cfg
	cp img/icons/*.png $(TOPDIR)/$(TARGET_P)/img/icons
	cp img/widget/*.png $(TOPDIR)/$(TARGET_P)/img/widget
	cp img/wpaper/*.png $(TOPDIR)/$(TARGET_P)/img/wpaper

clean :
	rm -f $(TARGET_D) $(TARGET_S) $(obj)
