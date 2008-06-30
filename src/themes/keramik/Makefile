
-include ../../../.config

subdir-y = ../../../tools/theme
target_so-y = keramik.so

keramik.so_depends-y  = ../../../tools/theme
keramik.so_files-y    = keramik.c
keramik.so_includes-y = ../../lib ../../server

include ../../../Makefile.lib

keramik.c: $(keramik.so_depends-y)
	../../../tools/theme/theme keramik ./ > keramik.c

clean:
	$(RM) keramik.c

dist:
	$(CP) keramik.so ../../../dist/$(CONFIG_PATH_THEMES)/keramik.so
