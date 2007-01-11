## Process this file with automake to produce Makefile.in

libgdkincludedir = $(includedir)/gtk-2.0/gdk

INCLUDES = 				\
	-DG_LOG_DOMAIN=\"Gdk\"		\
	-DGDK_COMPILATION		\
	-I$(top_srcdir)			\
	-I$(top_srcdir)/gdk		\
	-I$(top_builddir)/gdk		\
	-DG_DISABLE_DEPRECATED		\
	-DGDK_PIXBUF_DISABLE_DEPRECATED	\
	-DGDK_DISABLE_DEPRECATED	\
	$(GTK_DEBUG_FLAGS) 		\
	$(GDK_DEP_CFLAGS)

noinst_LTLIBRARIES = libgdk-xynth.la

libgdk_xynth_la_SOURCES = \
	gdkdisplay-xynth.c \
	gdkmain-xynth.c \
	gdkproperty-xynth.c \
	gdk-xynth.c

libgdkinclude_HEADERS =  \
	gdk-xynth.h
