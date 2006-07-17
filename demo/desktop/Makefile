
include ../../Makefile.cfg
include ../../Makefile.parse

DESKTOPDIR = $(INSTALLDIR)/$(SHAREDIR)/desktop
CFLAGS += -DDESKTOPDIR=\"$(DESKTOPDIR)\"
INCDIR  = ../../src/lib
LIBDIR  = ../../src/lib
LDFLAGS = -lxynth

OBJS = desktop.o main.o menu.o taskbar.o

ifeq ($(SINGLE_APP), Y)
TARGET_A = single_desktop.a
CFLAGS  += -Dmain=desktop_main
else
TARGET = desktop
endif

EXTRA_CLEAN = desktop single_desktop.a

include ../../Makefile.rules

dist: all
ifneq ($(TARGET),)
	cp -a $(TARGET) ../../$(DISTBINDIR)
endif
	mkdir -p ../../$(DISTTOPDIR)/$(SHAREDIR)/desktop
	mkdir -p ../../$(DISTTOPDIR)/$(SHAREDIR)/desktop/cfg/
	mkdir -p ../../$(DISTTOPDIR)/$(SHAREDIR)/desktop/img/
	mkdir -p ../../$(DISTTOPDIR)/$(SHAREDIR)/desktop/img/icons
	mkdir -p ../../$(DISTTOPDIR)/$(SHAREDIR)/desktop/img/widget
	mkdir -p ../../$(DISTTOPDIR)/$(SHAREDIR)/desktop/img/wpaper
	cp cfg/*.cfg ../../$(DISTTOPDIR)/$(SHAREDIR)/desktop/cfg
	cp img/icons/*.png ../../$(DISTTOPDIR)/$(SHAREDIR)/desktop/img/icons
	cp img/widget/*.png ../../$(DISTTOPDIR)/$(SHAREDIR)/desktop/img/widget
	cp img/wpaper/*.png ../../$(DISTTOPDIR)/$(SHAREDIR)/desktop/img/wpaper
