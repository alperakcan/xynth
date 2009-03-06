
-include ../../.config

target_so-$(CONFIG_WIDGETOO_SHARED) = libwidgetoo.so

libwidgetoo.so_includes-y = ./ ../lib
libwidgetoo.so_files-y = \
	button.cpp \
	frame.cpp \
	handler.cpp \
	layout.cpp \
	object.cpp \
	rect.cpp \
	widgetoo.h \
	window.cpp

libwidgetoo.so_libraries-y = ../lib/
libwidgetoo.so_ldflags-y   = -lxynth

target_a-$(CONFIG_WIDGETOO_STATIC) = libwidgetoo.a

libwidgetoo.a_includes-y = $(libwidgetoo.so_includes-y)
libwidgetoo.a_files-y    = $(libwidgetoo.so_files-y)

include ../../Makefile.lib

dist:
	$(CP) libwidgetoo.so ../../dist/$(CONFIG_PATH_LIB)
	$(CP) libwidgetoo.a ../../dist/$(CONFIG_PATH_LIB)
