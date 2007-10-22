
-include ../../.config

ifeq ($(CONFIG_SINGLE_APPLICATION), y)
target-n = xynthlogout
target_o-y = single_xynthlogout.o
else
target-y = xynthlogout
target_o-n = single_xynthlogout.o
endif

xynthlogout_files-y     = xynthlogout.c
xynthlogout_includes-y  = ../../src/lib
xynthlogout_libraries-y = ../../src/lib/
xynthlogout_ldflags-y   = -lxynth

single_xynthlogout.o_files-y    = xynthlogout.c
single_xynthlogout.o_includes-y = ../../src/lib
single_xynthlogout.o_cflags-y   = -Dmain=xynthlogout_main

include ../../Makefile.lib

ifneq ($(CONFIG_SINGLE_APPLICATION), y)
dist:
	$(CP) xynthlogout ../../dist/$(CONFIG_PATH_BIN)/xynthlogout
endif
