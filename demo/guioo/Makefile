
-include ../../.config

ifeq ($(CONFIG_SINGLE_APPLICATION), y)
target-n = guioo
target_o-y = single_guioo.o
else
target-y = guioo
target_o-n = single_guioo.o
endif

guioo_files-y     = \
	Demo.cpp \
	DesktopWindow.cpp \
	ShutdownWindow.cpp \
	SlideWindow.cpp \
	StartSplashWindow.cpp \
	Thread.cpp \
	Demo.h \
	DesktopWindow.h \
	ShutdownWindow.h \
	SlideWindow.h \
	StartSplashWindow.h \
	Thread.h

guioo_cflags-y = -DGUIOODIR=\"$(CONFIG_PATH_INSTALL)/$(CONFIG_PATH_SHARE)/guioo\"
guioo_includes-y  = ../../src/lib ../../src/guioo
guioo_libraries-y = ../../src/lib/ ../../src/guioo
guioo_ldflags-y   = -lxynth -lguioo -lpthread

single_guioo.o_files-y    = guioo.c
single_guioo.o_includes-y = ../../src/lib
single_guioo.o_cflags-y   = -Dmain=guioo_main

include ../../Makefile.lib

dist:
ifneq ($(CONFIG_SINGLE_APPLICATION), y)
	$(CP) guioo ../../dist/$(CONFIG_PATH_BIN)/guioo
endif
	$(MKDIR) ../../dist/$(CONFIG_PATH_SHARE)/guioo
	$(CP) data/*.png ../../dist/$(CONFIG_PATH_SHARE)/guioo
	$(CP) data/*.xml ../../dist/$(CONFIG_PATH_SHARE)/guioo
