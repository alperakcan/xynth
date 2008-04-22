
-include ../../.config
include ../../Makefile.lib

dist:
ifeq ($(CONFIG_PLATFORM_PSPDEV), y)
	@true
else
	$(CP) *.conf ../../dist/$(CONFIG_PATH_CONFIGS)
endif
