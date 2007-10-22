
-include ../../../.config

target_so-y = microgui.so

microgui.so_files-y    = microgui.c
microgui.so_includes-y = ../../lib ../../server

include ../../../Makefile.lib

dist:
	$(CP) microgui.so ../../../dist/$(CONFIG_PATH_THEMES)/microgui.so
