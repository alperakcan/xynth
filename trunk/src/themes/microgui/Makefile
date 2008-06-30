
-include ../../../.config

subdir-y = ../../../tools/theme
target_so-y = microgui.so

microgui.so_depends-y  = ../../../tools/theme
microgui.so_files-y    = microgui.c
microgui.so_includes-y = ../../lib ../../server

include ../../../Makefile.lib

microgui.c: $(microgui.so_depends-y)
	../../../tools/theme/theme microgui ./ > microgui.c

clean:
	$(RM) microgui.c

dist:
	$(CP) microgui.so ../../../dist/$(CONFIG_PATH_THEMES)/microgui.so
