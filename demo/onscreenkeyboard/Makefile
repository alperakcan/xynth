
-include ../../.config

ifeq ($(CONFIG_SINGLE_APPLICATION), y)
target-n = onscreenkeyboard
target_o-y = single_onscreenkeyboard.o
else
target-y = onscreenkeyboard
target_o-n = single_onscreenkeyboard.o
endif

onscreenkeyboard_files-y     = onscreenkeyboard.c
onscreenkeyboard_includes-y  = ../../src/lib
onscreenkeyboard_libraries-y = ../../src/lib/
onscreenkeyboard_ldflags-y   = -lxynth

single_onscreenkeyboard.o_files-y    = onscreenkeyboard.c
single_onscreenkeyboard.o_includes-y = ../../src/lib
single_onscreenkeyboard.o_cflags-y   = -Dmain=onscreenkeyboard_main

include ../../Makefile.lib

ifneq ($(CONFIG_SINGLE_APPLICATION), y)
dist:
	$(CP) onscreenkeyboard ../../dist/$(CONFIG_PATH_BIN)/onscreenkeyboard
endif
