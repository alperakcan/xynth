
-include ../../.config

ifeq ($(CONFIG_SINGLE_APPLICATION), y)
target-n = temp
target_o-y = single_temp.o
else
target-y = temp
target_o-n = single_temp.o
endif

temp_files-y     = temp.c
temp_includes-y  = ../../src/lib
temp_libraries-y = ../../src/lib/
temp_ldflags-y   = -lxynth

single_temp.o_files-y    = temp.c
single_temp.o_includes-y = ../../src/lib
single_temp.o_cflags-y   = -Dmain=temp_main

include ../../Makefile.lib

ifneq ($(CONFIG_SINGLE_APPLICATION), y)
dist:
	$(CP) temp ../../dist/$(CONFIG_PATH_BIN)/temp
endif
