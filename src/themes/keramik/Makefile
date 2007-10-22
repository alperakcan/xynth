
-include ../../../.config

target_so-y = keramik.so

keramik.so_files-y    = keramik.c
keramik.so_includes-y = ../../lib ../../server

include ../../../Makefile.lib

dist:
	$(CP) keramik.so ../../../dist/$(CONFIG_PATH_THEMES)/keramik.so
