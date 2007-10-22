
-include ../../.config

ifeq ($(CONFIG_SINGLE_APPLICATION), y)
target-n = hide
target_o-y = single_hide.o
else
target-y = hide
target_o-n = single_hide.o
endif

hide_files-y     = hide.c
hide_includes-y  = ../../src/lib
hide_libraries-y = ../../src/lib/
hide_ldflags-y   = -lxynth

single_hide.o_files-y    = hide.c
single_hide.o_includes-y = ../../src/lib
single_hide.o_cflags-y   = -Dmain=hide_main

include ../../Makefile.lib

ifneq ($(CONFIG_SINGLE_APPLICATION), y)
dist:
	$(CP) hide ../../dist/$(CONFIG_PATH_BIN)/hide
endif
