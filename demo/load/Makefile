
-include ../../.config

ifeq ($(CONFIG_SINGLE_APPLICATION), y)
target-n = load
target_o-y = single_load.o
else
target-y = load
target_o-n = single_load.o
endif

load_files-y     = load.c
load_includes-y  = ../../src/lib
load_libraries-y = ../../src/lib/
load_ldflags-y   = -lxynth

single_load.o_files-y    = load.c
single_load.o_includes-y = ../../src/lib
single_load.o_cflags-y   = -Dmain=load_main

include ../../Makefile.lib

ifneq ($(CONFIG_SINGLE_APPLICATION), y)
dist:
	$(CP) load ../../dist/$(CONFIG_PATH_BIN)/load
endif
