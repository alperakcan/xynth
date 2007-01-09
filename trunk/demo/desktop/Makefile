
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

EXTRA_DEPEND_FILES = *.h

include ../../Makefile.rules

dist: all
ifneq ($(TARGET),)
	cp -a $(TARGET) ../../$(DISTBINDIR)
endif
	mkdir -p ../../$(DISTSHAREDIR)/desktop
	mkdir -p ../../$(DISTSHAREDIR)/desktop/cfg/
	mkdir -p ../../$(DISTSHAREDIR)/desktop/img/
	mkdir -p ../../$(DISTSHAREDIR)/desktop/img/icons
	mkdir -p ../../$(DISTSHAREDIR)/desktop/img/widget
	mkdir -p ../../$(DISTSHAREDIR)/desktop/img/wpaper
	cp cfg/*.cfg ../../$(DISTSHAREDIR)/desktop/cfg
	cp img/icons/*.png ../../$(DISTSHAREDIR)/desktop/img/icons
	cp img/widget/*.png ../../$(DISTSHAREDIR)/desktop/img/widget
	cp img/wpaper/*.png ../../$(DISTSHAREDIR)/desktop/img/wpaper
