
-include ../../../.config

subdir-y = ../../../tools/theme
target_so-y = silverado.so

silverado.so_depends-y  = ../../../tools/theme
silverado.so_files-y    = silverado.c
silverado.so_includes-y = ../../lib ../../server

include ../../../Makefile.lib

silverado.c: $(silverado.so_depends-y)
	../../../tools/theme/theme silverado ./ > silverado.c

clean:
	$(RM) silverado.c

dist:
	$(CP) silverado.so ../../../dist/$(CONFIG_PATH_THEMES)/silverado.so
