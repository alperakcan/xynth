
include ../../Makefile.cfg
include ../../Makefile.parse

DIR = src/python/

INCDIR += ../lib /usr/include/python2.4
LIBDIR += ../lib

OBJS = window.o xynthpy.o

ifeq ($(PLATFORM_LINUX), Y)
TARGET_SO = xynth.so
LDFLAGS   = -lpython2.4 -lxynth
EXTRA_DEPEND_FILES = *.h
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

include ../../Makefile.rules

dist: all
