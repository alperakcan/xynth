
-include ../../.config

ifeq ($(CONFIG_SINGLE_APPLICATION), y)
target-n = child
target_o-y = single_child.o
else
target-y = child
target_o-n = single_child.o
endif

child_files-y     = child.c
child_includes-y  = ../../src/lib
child_libraries-y = ../../src/lib/
child_ldflags-y   = -lxynth

single_child.o_files-y    = child.c
single_child.o_includes-y = ../../src/lib
single_child.o_cflags-y   = -Dmain=child_main

include ../../Makefile.lib

ifneq ($(CONFIG_SINGLE_APPLICATION), y)
dist:
	$(CP) child ../../dist/$(CONFIG_PATH_BIN)/child
endif
