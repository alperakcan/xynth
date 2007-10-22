
-include ../../.config

ifeq ($(CONFIG_SINGLE_APPLICATION), y)
target-n = object
target_o-y = single_object.o
else
target-y = object
target_o-n = single_object.o
endif

object_files-y     = object.c
object_includes-y  = ../../src/lib ../../src/widget
object_libraries-y = ../../src/lib/ ../../src/widget/
object_ldflags-y   = -lwidget -lxynth

single_object.o_files-y    = object.c
single_object.o_includes-y = ../../src/lib ../../src/widget
single_object.o_cflags-y   = -Dmain=object_main

include ../../Makefile.lib

ifneq ($(CONFIG_SINGLE_APPLICATION), y)
dist:
	$(CP) object ../../dist/$(CONFIG_PATH_BIN)/object
endif
