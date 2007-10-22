
-include ../../.config

ifeq ($(CONFIG_SINGLE_APPLICATION), y)
target-n = simple
target_o-y = single_simple.o
else
target-y = simple
target_o-n = single_simple.o
endif

simple_files-y     = simple.c
simple_includes-y  = ../../src/lib
simple_libraries-y = ../../src/lib/
simple_ldflags-y   = -lxynth

single_simple.o_files-y    = simple.c
single_simple.o_includes-y = ../../src/lib
single_simple.o_cflags-y   = -Dmain=simple_main

include ../../Makefile.lib

ifneq ($(CONFIG_SINGLE_APPLICATION), y)
dist:
	$(CP) simple ../../dist/$(CONFIG_PATH_BIN)/simple
endif
