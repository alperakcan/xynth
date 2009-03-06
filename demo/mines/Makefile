
-include ../../.config

ifeq ($(CONFIG_SINGLE_APPLICATION), y)
target-n = mines
target_o-y = single_mines.o
else
target-y = mines
target_o-n = single_mines.o
endif

mines_files-y     = mines.cpp
mines_includes-y  = ../../src/lib ../../src/widgetoo
mines_libraries-y = ../../src/lib/ ../../src/widgetoo
mines_ldflags-y   = -lwidgetoo -lxynth

single_mines.o_files-y    = mines.cpp
single_mines.o_includes-y = ../../src/lib
single_mines.o_cflags-y   = -Dmain=mines_main

include ../../Makefile.lib

ifneq ($(CONFIG_SINGLE_APPLICATION), y)
dist:
	$(CP) mines ../../dist/$(CONFIG_PATH_BIN)/mines
endif
