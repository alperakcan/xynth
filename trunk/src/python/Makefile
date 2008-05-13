
-include ../../.config

target_so-y = xynth.so

xynth.so_files-y = window.c \
                   xynthpy.c \
                   xynthpy.h

xynth.so_includes-y = ../lib \
                      $(CONFIG_PYTHON_INCLUDES)

xynth.so_libraries-y = ../lib

xynth.so_ldflags-y = -$(CONFIG_PYTHON_LDFLAGS) -lxynth

include ../../Makefile.lib
