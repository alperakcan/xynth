
-include ../../.config

ifeq ($(CONFIG_SINGLE_APPLICATION), y)
target-n = img
target_o-y = single_img.o
else
target-y = img
target_o-n = single_img.o
endif

img_files-y     = img.c
img_includes-y  = ../../src/lib
img_libraries-y = ../../src/lib/
img_ldflags-y   = -lxynth

single_img.o_files-y    = img.c
single_img.o_includes-y = ../../src/lib
single_img.o_cflags-y   = -Dmain=img_main

include ../../Makefile.lib

ifneq ($(CONFIG_SINGLE_APPLICATION), y)
dist:
	$(CP) img ../../dist/$(CONFIG_PATH_BIN)/img
endif
