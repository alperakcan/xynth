
include ../../Makefile.cfg
include ../../Makefile.parse

INCDIR += ../lib /usr/include/python2.4
LIBDIR += ../lib

OBJS = window.o xynthpy.o

ifeq ($(PLATFORM_LINUX), Y)
TARGET_SO = libxynthpy.so
LDFLAGS   = -lpython2.4 -lxynth
else
ifeq ($(PLATFORM_PSPDEV), Y)
else
ifeq ($(PLATFORM_MINGW), Y)
else
ifeq ($(PLATFORM_GP2X), Y)
endif
endif
endif
endif

DEPEND_FILES       = *.c
EXTRA_DEPEND_FILES = *.h

include ../../Makefile.rules

dist: all
