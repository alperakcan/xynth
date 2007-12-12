## Makefile.am for gtk+/gdk/gdk-xynth

libgdkincludedir = $(includedir)/gtk-2.0/gdk

INCLUDES = 				\
	-DG_LOG_DOMAIN=\"Gdk-Xynth\"	\
	-DGDK_COMPILATION		\
	-I$(top_srcdir)			\
	-I$(top_srcdir)/gdk		\
	-I$(top_builddir)/gdk		\
	-DG_DISABLE_DEPRECATED		\
	@GTK_DEBUG_FLAGS@		\
	@GDK_DEP_CFLAGS@		

LDADDS = 		\
	@GDK_DEP_LIBS@		

noinst_LTLIBRARIES = libgdk-xynth.la

libgdk_xynth_la_SOURCES =	\
	gdkcolor-xynth.c \
	gdkcursor-xynth.c \
	gdkdisplay-xynth.c \
	gdkdnd-xynth.c \
	gdkdrawable-xynth.c \
	gdkevents-xynth.c \
	gdkgc-xynth.c \
	gdkgeometry-xynth.c \
	gdkglobals-xynth.c \
	gdkimage-xynth.c \
	gdkinput-xynth.c \
	gdkkeys-xynth.c \
	gdkmain-xynth.c \
	gdkpixmap-xynth.c \
	gdkscreen-xynth.c \
	gdkvisual-xynth.c \
	gdkwindow-xynth.c \
	gdkxynth.c

libgdkinclude_HEADERS =		\
	gdkxynth.h
