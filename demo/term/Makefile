
-include ../../.config

ifeq ($(CONFIG_SINGLE_APPLICATION), y)
target-n = term
target_o-y = single_term.o
else
target-y = term
target_o-n = single_term.o
endif

term_files-y     = term.c
term_includes-y  = ../../src/lib
term_libraries-y = ../../src/lib/
term_ldflags-y   = -lxynth

single_term.o_files-y    = term.c
single_term.o_includes-y = ../../src/lib
single_term.o_cflags-y   = -Dmain=term_main

include ../../Makefile.lib

ifneq ($(CONFIG_SINGLE_APPLICATION), y)
dist:
	$(CP) term ../../dist/$(CONFIG_PATH_BIN)/term
endif
