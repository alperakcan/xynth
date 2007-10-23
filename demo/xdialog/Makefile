
-include ../../.config

ifeq ($(CONFIG_SINGLE_APPLICATION), y)
target-n = xdialog
target_o-y = single_xdialog.o
else
target-y = xdialog
target_o-n = single_xdialog.o
endif

xdialog_files-y     = xdialog.c msgbox.c yesno.c
xdialog_includes-y  = ../../src/lib ../../src/widget
xdialog_libraries-y = ../../src/lib ../../src/widget
xdialog_ldflags-y   = -lwidget -lxynth

single_xdialog.o_files-y    = xdialog.c msgbox.c yesno.c
single_xdialog.o_includes-y = ../../src/lib ../../src/widget
single_xdialog.o_cflags-y   = -Dmain=xdialog_main

include ../../Makefile.lib

ifneq ($(CONFIG_SINGLE_APPLICATION), y)
dist:
	$(CP) xdialog ../../dist/$(CONFIG_PATH_BIN)/xdialog
endif
