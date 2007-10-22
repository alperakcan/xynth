
-include ../../.config

WIDGETC = ../../../src/widget/widgetc/.widgetc/widgetc 

ifeq ($(CONFIG_SINGLE_APPLICATION), y)
target-n = widgetx
target_o-y = single_widgetx.o
else
target-y = widgetx
target_o-n = single_widgetx.o
endif

widgetx_files-y     = widgetx.xws widgetx.c
widgetx_includes-y  = ../../src/lib ../../src/widget .widgetx
widgetx_libraries-y = ../../src/lib ../../src/widget
widgetx_ldflags-y   = -lwidget -lxynth

single_widgetx.o_files-y    = widgetx.xws widgetx.c
single_widgetx.o_includes-y = ../../src/lib ../../src/widget .single_widgetx.o
single_widgetx.o_libraries  = ../../src/lib ../../src/widget
single_widgetx.o_cflags-y   = -Dmain=widgetx_main -DCONFIG_SINGLE_APPLICATION


include ../../Makefile.lib

ifneq ($(CONFIG_SINGLE_APPLICATION), y)
dist:
	$(CP) widgetx ../../dist/$(CONFIG_PATH_BIN)/widgetx
endif
