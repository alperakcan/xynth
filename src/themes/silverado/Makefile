
-include ../../../.config

target_so-y = silverado.so

silverado.so_files-y    = silverado.c
silverado.so_includes-y = ../../lib ../../server

include ../../../Makefile.lib

dist:
	$(CP) silverado.so ../../../dist/$(CONFIG_PATH_THEMES)/silverado.so
