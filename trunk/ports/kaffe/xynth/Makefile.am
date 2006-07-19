# Xynth-based AWT support for standard Java libraries
#
# Copyright (c) 2005
#       Alper Akcan <distchx@yahoo.com>
#

native_LTLIBRARIES = libxynthawt.la

AM_CPPFLAGS = \
	-I$(top_builddir)/include \
	-I$(top_srcdir)/kaffe/kaffevm/jni \
	-I$(top_srcdir)/libraries/clib/classpath

libxynthawt_la_CFLAGS = \
	$(CFLAGS_PG) \
	$(XYNTH_CFLAGS)

libxynthawt_la_LDFLAGS = \
	$(KLIBFLAGS) \
	-no-undefined \
	-module \
	-release $(PACKAGE_VERSION) \
	-export-symbols-regex "^([Jj]ava|kaffe)_"

libxynthawt_la_LIBADD = \
	$(top_builddir)/kaffe/kaffevm/libkaffevm.la $(AWT_LIBS) \
	$(top_builddir)/libraries/clib/classpath/libclasspath.la

libxynthawt_la_SOURCES = \
	font.c \
	toolkit.c \
	window.c

noinst_HEADERS = \
	toolkit.h

CLEANFILES = so_locations
