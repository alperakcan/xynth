
include ../../Makefile.cfg
include ../../Makefile.parse

INCDIR += ../lib
LIBDIR += ../lib

TARGET-$(PLATFORM_LINUX)  = xynth
TARGET-$(PLATFORM_PSPDEV) = xynth.elf
TARGET-$(PLATFORM_MINGW)  = xynth.exe
TARGET-$(PLATFORM_GP2X)   = xynth

OBJS = event.o id.o kbd.o mouse.o priority.o single.o server.o socket.o start.o \
       surface.o theme.o window.o window_handler.o window_move_resize.o

OBJS-$(VIDEO_FBDev)       += video/fb/fb.o video/fb/kbd.o video/fb/server.o video/fb/timing.o
VLDFLAGS-$(VIDEO_FBDev)   += -lm

OBJS-$(VIDEO_SDL)         += video/sdl/event.o video/sdl/kbd.o video/sdl/mouse.o video/sdl/server.o
CFLAGS-$(VIDEO_SDL)       += $(shell sdl-config --cflags)
VLDFLAGS-$(VIDEO_SDL)     += $(shell sdl-config --libs)

OBJS-$(VIDEO_SVGALib)     += video/svga/kbd.o video/svga/server.o
VLDFLAGS-$(VIDEO_SVGALib) += -lvga -lm

OBJS-$(VIDEO_VESA)        += video/vesa/kbd.o video/vesa/server.o video/vesa/lrmi-0.10/lrmi.o

TARGET-$(VIDEO_PSPDEV)     = xynth.elf
OBJS-$(VIDEO_PSPDEV)      += video/pspdev/event.o video/pspdev/kbd.o video/pspdev/mouse.o video/pspdev/server.o
VLDFLAGS-$(VIDEO_PSPDEV)  += -lm -lpspdebug -lpspdisplay -lpspge -lpspctrl -lpspsdk -lc -lpsputility -lpspuser -lpspkernel

OBJS-$(VIDEO_GDI)         += video/gdi/event.o video/gdi/kbd.o video/gdi/mouse.o video/gdi/server.o
LIBDIR-$(VIDEO_GDI)       += video/gdi/pthread_w32
VLDFLAGS-$(VIDEO_GDI)     += -lpthreadGC2 -lgdi32
TARGET-$(VIDEO_GDI)        = xynth.exe

OBJS-$(VIDEO_GP2X)        += video/gp2x/event.o video/gp2x/kbd.o video/gp2x/mouse.o video/gp2x/server.o
VLDFLAGS-$(VIDEO_GP2X)    += -lpthread -lm -static

OBJS-$(VIDEO_NULL)        += video/null/kbd.o video/null/mouse.o video/null/server.o

OBJS-$(VIDEO_HELPER)      += video/helper/console.o video/helper/cursor.o video/helper/kbd.o video/helper/modes.o video/helper/mouse.o video/helper/mtrr.o

ifeq ($(SINGLE_APP), Y)
	LDFLAGS-$(DEMO_CHILD) += ../../demo/child/single_child.a
	LDFLAGS-$(DEMO_DESKTOP) += ../../demo/desktop/single_desktop.a
	LDFLAGS-$(DEMO_HIDE) += ../../demo/hide/single_hide.a
	LDFLAGS-$(DEMO_LOAD) += ../../demo/load/single_load.a
	LDFLAGS-$(DEMO_SIMPLE) += ../../demo/simple/single_simple.a
	LDFLAGS-$(DEMO_OBJECT) += ../../demo/object/single_object.a
	LDFLAGS-$(DEMO_ONSCREENKEYBOARD) += ../../demo/onscreenkeyboard/single_onscreenkeyboard.a
	LDFLAGS-$(DEMO_TEMP) += ../../demo/temp/single_temp.a
	LDFLAGS-$(DEMO_TERM) += ../../demo/term/single_term.a
	LDFLAGS-$(DEMO_TIMER) += ../../demo/timer/single_timer.a
	LDFLAGS-$(DEMO_LOGOUT) += ../../demo/xynthlogout/single_xynthlogout.a
endif

LDFLAGS-$(THEME_PLUGIN) += -ldl

OBJS    += $(OBJS-Y)
CFLAGS  += $(CFLAGS-Y)
LIBDIR  += $(LIBDIR-Y)
LDFLAGS += $(LDFLAGS-Y) -lxynth $(VLDFLAGS-Y)
TARGET   = $(TARGET-Y)

EXTRA_CLEAN = xynth xynth.exe xynth.elf $(OBJS-N) $(OBJS-)

DEPEND_FILES       = *.c video/*/*.c
EXTRA_DEPEND_FILES = *.h

include ../../Makefile.rules

dist: all
	cp -a $(TARGET) ../../$(DISTBINDIR)
ifeq ($(PLATFORM_MINGW), Y)
ifeq ($(THREAD_PTHREADS), Y)
	cp -a video/gdi/pthread_w32/pthreadGC2.dll ../../$(DISTBINDIR)
endif
else
ifeq ($(PLATFORM_PSPDEV), Y)
	psp-fixup-imports ../../$(DISTBINDIR)/$(TARGET)
	mksfo 'Xynth Windowing System' ../../$(DISTBINDIR)/param.sfo
ifeq ($(PSP-VERSION), 1.5)
	mkdir -p ../../$(DISTBINDIR)/xynth
	psp-strip ../../$(DISTBINDIR)/$(TARGET) -o ../../$(DISTBINDIR)/xynth/eboot.pbp
	mkdir -p ../../$(DISTBINDIR)/xynth%
	pack-pbp ../../$(DISTBINDIR)/xynth%/eboot.pbp \
	         ../../$(DISTBINDIR)/param.sfo \
	         ../../tools/pspdev/icon0.png \
	         NULL \
	         NULL \
	         ../../tools/pspdev/pic1.png \
	         NULL \
	         NULL \
	         NULL
else
ifeq ($(PSP-VERSION), 1.0)
	mkdir -p ../../$(DISTBINDIR)/xynth
	psp-strip ../../$(DISTBINDIR)/$(TARGET) -o ../../$(DISTBINDIR)/$(TARGET).strip
	pack-pbp ../../$(DISTBINDIR)/xynth/eboot.pbp \
	         ../../$(DISTBINDIR)/param.sfo \
	         ../../tools/pspdev/icon0.png \
	         NULL \
	         NULL \
	         ../../tools/pspdev/pic1.png \
	         NULL \
	         ../../$(DISTBINDIR)/$(TARGET).strip \
	         NULL
	rm ../../$(DISTBINDIR)/$(TARGET).strip
endif
endif
else
ifeq ($(PLATFORM_GP2X), Y)
	echo "#!/bin/bash"                          >  ../../$(DISTTOPDIR)/$(TARGET).gpu
	echo "sync"                                 >> ../../$(DISTTOPDIR)/$(TARGET).gpu
	echo "mount /mnt/sd -o remount,sync"        >> ../../$(DISTTOPDIR)/$(TARGET).gpu
	echo "export PATH=\`pwd\`/bin:\$$PATH"          >> ../../$(DISTTOPDIR)/$(TARGET).gpu
	echo "export LD_LIBRARY_PATH=\`pwd\`/lib:\$$LD_LIBRARY_PATH" >> ../../$(DISTTOPDIR)/$(TARGET).gpu
	echo "cd bin"                               >> ../../$(DISTTOPDIR)/$(TARGET).gpu
ifeq ($(SINGLE_APP), Y)
	echo "./xynth &> ../xynth.log"              >> ../../$(DISTTOPDIR)/$(TARGET).gpu
else
	echo "./xynth &> ../xynth.log& sleep 2; ./desktop &> ../xynth_desktop.log" >> ../../$(DISTTOPDIR)/$(TARGET).gpu
endif
	echo "sync"                                 >> ../../$(DISTTOPDIR)/$(TARGET).gpu
	echo "mount /mnt/sd -o remount,async"       >> ../../$(DISTTOPDIR)/$(TARGET).gpu
	echo "cd /usr/gp2x"                         >> ../../$(DISTTOPDIR)/$(TARGET).gpu
	echo "exec ./gp2xmenu"                      >> ../../$(DISTTOPDIR)/$(TARGET).gpu
	chmod +x ../../$(DISTTOPDIR)/$(TARGET).gpu
	cp ../../tools/gp2x/xynth.png ../../$(DISTTOPDIR)
endif
endif
endif
