
-include ../../../.config

target.host-$(CONFIG_WIDGET_WIDGETC) = widgetc

widgetc_includes-y = ./ ../ ../../lib/
widgetc_files-y    = widgetc.c \
                     list.c \
                     code.c \
                     header.c \
                     xml.c
widgetc_ldflags-y  = -lexpat

include ../../../Makefile.lib

dist:
	$(CP) widgetc ../../../dist/$(CONFIG_PATH_BIN)/widgetc
