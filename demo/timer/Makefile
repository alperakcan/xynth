
-include ../../.config

ifeq ($(CONFIG_SINGLE_APPLICATION), y)
target-n = timer
target_o-y = single_timer.o
else
target-y = timer
target_o-n = single_timer.o
endif

timer_files-y     = timer.c
timer_includes-y  = ../../src/lib
timer_libraries-y = ../../src/lib/
timer_ldflags-y   = -lxynth

single_timer.o_files-y    = timer.c
single_timer.o_includes-y = ../../src/lib
single_timer.o_cflags-y   = -Dmain=timer_main

include ../../Makefile.lib

ifneq ($(CONFIG_SINGLE_APPLICATION), y)
dist:
	$(CP) timer ../../dist/$(CONFIG_PATH_BIN)/timer
endif
