
-include ../../.config

subdir-$(CONFIG_WIDGET_WIDGETC) := widgetc
subdir-$(CONFIG_WIDGET_WIDGETR) += widgetr

target_so-$(CONFIG_WIDGET_SHARED) = libwidget.so

widgetc_depends-y = libwidget.so
widgetr_depends-y = libwidget.so

libwidget.so_includes-y = ./ ../lib
libwidget.so_files-y = button.c \
                       checkbox.c \
                       editbox.c \
                       effect.c \
                       frame.c \
                       object.c \
                       progressbar.c \
                       pushbutton.c \
                       signal.c \
                       scrollbuffer.c \
                       textbox.c \
                       window.c \
                       listbox.c \
                       clistbox.c \
                       toolbar.c
libwidget.so_libraries-y = ../lib/
libwidget.so_ldflags-y   = -lxynth

target_a-$(CONFIG_WIDGET_STATIC) = libwidget.a

libwidget.a_includes-y = $(libwidget.so_includes-y)
libwidget.a_files-y    = $(libwidget.so_files-y)

include ../../Makefile.lib

dist:
	$(CP) widget.h ../../dist/$(CONFIG_PATH_INCLUDE)
	$(CP) libwidget.so ../../dist/$(CONFIG_PATH_LIB)
	$(CP) libwidget.a ../../dist/$(CONFIG_PATH_LIB)
