
-include ../../.config

ifeq ($(CONFIG_SINGLE_APPLICATION), y)
target-n = desktop
target_o-y = single_desktop.o
else
target-y = desktop
target_o-n = single_desktop.o
endif

desktop_files-y = desktop.c main.c menu.c taskbar.c
desktop_cflags-y = -DDESKTOPDIR=\"$(CONFIG_PATH_INSTALL)/$(CONFIG_PATH_SHARE)/desktop\"
desktop_includes-y = ../../src/lib/
desktop_libraries-y = ../../src/lib/
desktop_ldflags-y = -lxynth

single_desktop.o_files-y = $(desktop_files-y)
single_desktop.o_cflags-y = $(desktop_cflags-y) -Dmain=desktop_main
single_desktop.o_includes-y = $(desktop_includes-y)
single_desktop.o_libraries-y = $(desktop_libraries-y)
single_desktop.o_ldflags-y = $(desktop_ldflags-y)

include ../../Makefile.lib

dist:
ifneq ($(CONFIG_SINGLE_APPLICATION), y)
	$(CP) desktop ../../dist/$(CONFIG_PATH_BIN)/desktop
endif
	$(MKDIR) ../../dist/$(CONFIG_PATH_SHARE)/desktop
	$(MKDIR) ../../dist/$(CONFIG_PATH_SHARE)/desktop/cfg/
	$(MKDIR) ../../dist/$(CONFIG_PATH_SHARE)/desktop/img/
	$(MKDIR) ../../dist/$(CONFIG_PATH_SHARE)/desktop/img/icons
	$(MKDIR) ../../dist/$(CONFIG_PATH_SHARE)/desktop/img/widget
	$(MKDIR) ../../dist/$(CONFIG_PATH_SHARE)/desktop/img/wpaper
	$(CP) cfg/*.cfg ../../dist/$(CONFIG_PATH_SHARE)/desktop/cfg
	$(CP) img/icons/*.png ../../dist/$(CONFIG_PATH_SHARE)/desktop/img/icons
	$(CP) img/widget/*.png ../../dist/$(CONFIG_PATH_SHARE)/desktop/img/widget
	$(CP) img/wpaper/*.png ../../dist/$(CONFIG_PATH_SHARE)/desktop/img/wpaper
